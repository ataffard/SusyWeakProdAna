#include <iomanip>
#include <iostream>
#include "TCanvas.h"
#include "SusyWeakProdAna/SusyBaseAna.h"
#include "SusyWeakProdAna/PhysicsTools.h"

#include "SusyWeakProdAna/SusyAnaCommon.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// SusyBaseAna Constructor
/*--------------------------------------------------------------------------------*/
SusyBaseAna::SusyBaseAna(SusyHistos* _histos, bool is2LAna, bool isWHAna, bool qFlipd0):
  SusySelection(is2LAna,qFlipd0),
  _hh(_histos),
  m_useLooseLep(false),
  m_writeToyNt(false),
  m_susyXsec(0)
{
  reset();
  resetCounter();

  _random = new TRandom3(2012300958);
  
  //-------------------------
  //2L trigger and Fake
  //-------------------------
  bool useReweightUtils=false;//true;
  m_trigObj = new DilTrigLogic(LUMW,useReweightUtils);  
  if(USE_MCTRIG && !USE_DGWEIGHT){
    m_trigObj->useMCTrigger();
    cout << " MESSAGE: Using MC trigger decision " << endl;
  }

  // Configure using fake rates file
  // Currently rates are provided as function of pT only, so only use PT as second option
  //string _fakeInput  =  string(getenv("WORKAREA")) + 
  //"/SusyMatrixMethod/data/pass3_Summer2013.root"; //Summer 2013 2L paper!
  string _fakeInput  =  string(getenv("WORKAREA")) + 
    "/SusyMatrixMethod/data/forDavide_Sep11_2013.root"; //WH ana!
  cout << "Loading fake MM " << _fakeInput << endl;
  m_matrix_method.configure(_fakeInput, SusyMatrixMethod::PT,
			    SusyMatrixMethod::PT,
			    SusyMatrixMethod::PT,
			    SusyMatrixMethod::PT);


  //-------------------------
  //3L Trigger
  //-------------------------
  m_trig3LObj = new TrilTrigLogic();
  m_trig3LObj->loadTriggerMaps();
  //if(m_trigAccOnly) m_trigLogic->setAccOnly(true); //to not apply trigger, just pt Thrs

  //Open signal cross section file
  string _sSigFile = string(getenv("WORKAREA")) +
    "/SusyWeakProdAna/data/" + "Simplified_CrossSections.txt";
  sigXsfile.open(_sSigFile.c_str());

  // Get the handle on the SleptonXsecReader
  string sleptonXSecFile = string(getenv("ROOTCOREDIR")) +
    "/data/SusyNtuple/DLiSlep_SignalUncertainties_All.root" ;
  string sleptonDSIDFile =  string(getenv("ROOTCOREDIR")) +
    "/data/SusyNtuple/samplesList_pMSSM_DLiSlep.txt";

  m_SleptonXSecReader    = new SleptonXsecReader();
  //  sleptonDSIDFile.c_str(),sleptonXSecFile.c_str(),"SignalUncertainties"); 
  
  string xsecFileName  = gSystem->ExpandPathName("$ROOTCOREDIR/data/SusyWeakProdAna/susy_crosssections_8TeV.txt");
  m_susyXsec = new SUSY::CrossSectionDB(xsecFileName);

  susyXS = new XSReader();
  susyXS->LoadXSInfo();

  if(is2LAna)      setAnaType(Ana_2Lep);
  else if(isWHAna) setAnaType(Ana_2LepWH);
  else             setAnaType(Ana_3Lep);


}

/*--------------------------------------------------------------------------------*/
// Hook the containers
/*--------------------------------------------------------------------------------*/
void SusyBaseAna::hookContainers(Susy::SusyNtObject* _ntPtr,
				 ElectronVector* _preEleA, ElectronVector* _baseEleA, ElectronVector* _sigEleA,
				 MuonVector*     _preMuoA, MuonVector*     _baseMuoA, MuonVector*     _sigMuoA,
				 LeptonVector*   _baseLepA, LeptonVector*   _sigLepA,
				 JetVector*      _preJetA, JetVector*      _baseJetA, JetVector*      _sigJetA,
				 TauVector*      _baseTauA, TauVector*     _sigTauA
				 )
{
  nt         = _ntPtr;
  v_preEle   = _preEleA;
  v_baseEle  = _baseEleA;
  v_sigEle   = _sigEleA;
  v_preMu    = _preMuoA;
  v_baseMu   = _baseMuoA;
  v_sigMu    = _sigMuoA;
  v_baseLep  = _baseLepA;
  v_sigLep   = _sigLepA;
  v_preJet   = _preJetA;
  v_baseJet  = _baseJetA;
  v_sigJet   = _sigJetA;
  v_baseTau  = _baseTauA;
  v_sigTau   = _sigTauA;

  if(DUMP_RUNEVT){
    string dumpName = _hh->sampleName()+ "_dump.txt";
    evtDump.open (dumpName.c_str());
  }

}


/*--------------------------------------------------------------------------------*/
// End of job
/*--------------------------------------------------------------------------------*/
void SusyBaseAna::finish()
{
  
  if(DUMP_RUNEVT){
    evtDump.close ();
  }  
  
  
  if(m_writeToyNt){
    m_toyNt->setSumOfMcWeights(nt->evt()->sumw); 
    m_toyNt->SaveTree();

    string dir =  string(getenv("HISTOANA")) + "/SusyAna/ToyNtOutputs";
    gSystem->mkdir(dir.c_str(),kTRUE);

    string cmd = "mv " + m_toyNt->getFilename() + " " + dir;
    std::cout << "Moving ToyNt " << cmd << std::endl;
    gSystem->Exec(cmd.c_str());

    delete m_toyNt;
  }

  clearVectors();
  sigXsfile.close();  
}

/*--------------------------------------------------------------------------------*/
// Event weight
/*--------------------------------------------------------------------------------*/
float SusyBaseAna::eventWeight(int mode)
{
  bool useSumWMap = true; 
  if( !nt->evt()->isMC) return 1; //Data weight =1

  float _evtW=nt->evt()->w;

  if(mode==NOLUMI) _evtW= nt->evt()->w; //raw weight - generator included!
  if(mode==LUMI21FB){ //Moriond dataset
    if(USE_MCWEIGHT) _evtW =  nt->evt()->w;
    else{
      int id = nt->evt()->mcChannel;
      _evtW = getEventWeight(nt->evt(),LUMI_A_L,useSumWMap,m_MCSumWs);

      //Replace XS for Higgs samples
      /*
      if((nt->evt()->mcChannel >= 129477 && nt->evt()->mcChannel <= 129494)|| //WZ Powheg update XS MCNLO !!!
	 (nt->evt()->mcChannel >= 126949 && nt->evt()->mcChannel <= 126951)   //ZZ->llvv x3 Xs
	 ){
	if(m_xsecMap.find(id) == m_xsecMap.end()) {
	  m_xsecMap[id] = m_susyXsec->process(id);
	}
	xs = m_xsecMap[id].xsect() * m_xsecMap[id].kfactor() * m_xsecMap[id].efficiency();
      }
      */
            
      //Overwrite Xs value
      if(isSimplifiedModelGrid(id)){
	float xs    = susyXS->GetXS(id);
	float sumw  = 0;
	map<unsigned int, float>::const_iterator sumwMapIter = m_MCSumWs->find(id);
        if(sumwMapIter != m_MCSumWs->end()) sumw = sumwMapIter->second;
        else{
          cout << "SusyBaseAna::eventWeight - ERROR - requesting to use sumw map but "
               << "mcid " << id << " not found!" << endl;
          abort();
        }
	if(dbg()>10) cout << " Xs org " << nt->evt()->xsec << " new " << xs 
			  << " sumW file " <<  nt->evt()->sumw << " on-the-fly " << sumw << endl;
	_evtW = nt->evt()->w * nt->evt()->wPileup * xs * LUMI_A_L / sumw;
      }
      
      
      if(id==176322 || id==176325 || id==176480){
	float xs = nt->evt()->xsec;
	float sumw = nt->evt()->sumw;
	//SUSY WW-like
	if(id==176322) xs = 0.425175*0.9;
	if(id==176325) xs = 0.167127*0.9;
	if(id==176480) xs = 0.5420344;//0.616257;
	_evtW = nt->evt()->w * nt->evt()->wPileup * xs * LUMI_A_L / sumw;
      }

      if(dbg()>10)
	cout << "Ana W: " << nt->evt()->w 
	     << " pileup " << nt->evt()->wPileup 
	     << " xsec " <<  nt->evt()->xsec 
	     << " lumi " << LUMI_A_L 
	     << " sumw " << nt->evt()->sumw << endl;
    }
  }
  
  // Correct the cross section for DLiSlep
  if(
     (nt->evt()->mcChannel >= 166501 && nt->evt()->mcChannel <= 166658) ||
     (nt->evt()->mcChannel >= 175420 && nt->evt()->mcChannel <= 175583) ||
     (nt->evt()->mcChannel >= 177423 && nt->evt()->mcChannel <= 177496) 
     ) {
    if(m_SleptonXSecReader!=NULL) {
      if( nt->evt()->susyFinalState==201 ) 
	_evtW *= (m_SleptonXSecReader->getCrossSection(nt->evt()->mcChannel,SleptonPoint::kLeftHanded )/nt->evt()->xsec)*(nt->evt()->sumw/m_sleptonSumWs[201]);   
      else if( nt->evt()->susyFinalState==202 )
	_evtW *= (m_SleptonXSecReader->getCrossSection(nt->evt()->mcChannel,SleptonPoint::kRightHanded)/nt->evt()->xsec)*(nt->evt()->sumw/m_sleptonSumWs[202]);   
      else if( nt->evt()->susyFinalState==216 )
	_evtW *= (m_SleptonXSecReader->getCrossSection(nt->evt()->mcChannel,SleptonPoint::kLeftHanded )/nt->evt()->xsec)*(nt->evt()->sumw/m_sleptonSumWs[216]);   
      else if( nt->evt()->susyFinalState==217 )
	_evtW *= (m_SleptonXSecReader->getCrossSection(nt->evt()->mcChannel,SleptonPoint::kRightHanded)/nt->evt()->xsec)*(nt->evt()->sumw/m_sleptonSumWs[217]);   
      else _evtW = 0;  //protection against undefined final state
    }
    else
      cout << "m_SleptonXSecReader is null, won't touch the event weight" << endl;
  }
  

  return _evtW;
}
/*--------------------------------------------------------------------------------*/
// determine if Simplified model grid
/*--------------------------------------------------------------------------------*/
bool SusyBaseAna::isSimplifiedModelGrid(int dsId)
{
  if(//mode A w/ slepton 
     (dsId >= 144871 && dsId <= 144896) || (dsId >= 176531 && dsId <= 176557) || (dsId >= 157461 && dsId <= 157968) ||
     //mode C w/ slepton
     (dsId >= 144902 && dsId <= 144927) || (dsId >= 157969 && dsId <= 157986) || (dsId >= 176558 && dsId <= 176573) ||
     //mode A lepW
     (dsId >= 164274 && dsId <= 164323 ) || (dsId >= 174665 && dsId <= 174840 ) || (dsId >= 178792 && dsId <= 178801 ) ||
     //mode A hadW
     (dsId >= 164324 && dsId <= 164373 ) || (dsId >= 178766 && dsId <= 178791 ) ||                                         
     //mode C lepW
     (dsId >= 164374 && dsId <= 164423) ||
     //WH 
     (dsId >= 176574 && dsId <= 176707)  || (dsId >= 177501 && dsId <= 177528) 
      )
    return true;

  return false;
}
/*--------------------------------------------------------------------------------*/
// Lepton SF weight
/*--------------------------------------------------------------------------------*/
float SusyBaseAna::getLepSFWeight(const LeptonVector* leptons, uint iSys)
{
  float _wLepSF=1;
  if(USE_LEPSF && nt->evt()->isMC){
    for(uint ilep=0; ilep<leptons->size(); ilep++){
      const Susy::Lepton* _l = leptons->at(ilep);
      //TODO dealing with those syst here 
      _wLepSF *= _l->effSF;
      if(dbg()>10) cout << "lep" << ilep << " SF " << _l->effSF << endl; 
    }
  }
  return _wLepSF;
}
/*--------------------------------------------------------------------------------*/
// Trigger weight
/*--------------------------------------------------------------------------------*/
float  SusyBaseAna::getTriggerWeight(const LeptonVector* leptons, 
				     float met, int nSignalJets, int npv,
				     uint iSys){
  float _wTrig=1;
  
  if(USE_DGWEIGHT && nt->evt()->isMC) {
    uint iiSys = DGSys_NOM;
    if(iSys==DGSys_TRIGSF_EL_UP) iiSys=NtSys_TRIGSF_EL_UP;
    if(iSys==DGSys_TRIGSF_EL_DN) iiSys=NtSys_TRIGSF_EL_DN;
    if(iSys==DGSys_TRIGSF_MU_UP) iiSys=NtSys_TRIGSF_MU_UP;
    if(iSys==DGSys_TRIGSF_MU_DN) iiSys=NtSys_TRIGSF_MU_DN;
    
    _wTrig =  m_trigObj->getTriggerWeight(*leptons,  nt->evt()->isMC, 
					  met, nSignalJets, npv,
					  (SusyNtSys) iiSys);
    if(_wTrig<0 || _wTrig>1) {
      //cout << "WARNING Trigger weight out of bound - set to 0 or 1 " << DIL_FLAV[m_ET]  << " " << _wtrig << endl;
      if(_wTrig<0) _wTrig=0;
      if(_wTrig>0) _wTrig=1;
      
    }
    if(_wTrig != _wTrig){
      float _wTrigNoSys = m_trigObj->getTriggerWeight(*leptons, nt->evt()->isMC, 
						      met, nSignalJets, npv,
						      (SusyNtSys) 0);
      cout << "WARNING OVERWRITE TRIG SYS BECAUSE OF NAN" << endl;
      _wTrig = _wTrigNoSys;
      if(dbg()>10){
	cout << "No sys "<< _wTrigNoSys << " " << iiSys << " trigW " << _wTrig
	     << " pt1 " << leptons->at(0)->Pt() << " pt2 " << leptons->at(1)->Pt() << endl; 
      }
    }
  }
  return _wTrig;
}
/*--------------------------------------------------------------------------------*/
float SusyBaseAna::getBTagSF(const Susy::Event*, const JetVector* jets, uint iSys)
{
  if(!nt->evt()->isMC) return 1;
  if(!USE_BWEIGHT)     return 1;

  JetVector  valJets;
  valJets.clear();
  for(uint i=0; i<jets->size(); ++i){
    Jet* jet = jets->at(i);
    if( jet->Pt() < JET_PT_L20_CUT        ) continue;
    if( fabs(jet->detEta) > JET_ETA_CUT_2L ) continue;
    valJets.push_back(jet);
  }
  
  if(valJets.size()==0) return 1;//safety.

  //Get sys naming convention
  uint _sys = DGSys_NOM;
  if(iSys==DGSys_BJet_DN) _sys=BTag_BJet_DN; 
  if(iSys==DGSys_CJet_DN) _sys=BTag_CJet_DN; 
  if(iSys==DGSys_LJet_DN) _sys=BTag_LJet_DN; 
  if(iSys==DGSys_BJet_UP) _sys=BTag_BJet_UP; 
  if(iSys==DGSys_CJet_UP) _sys=BTag_CJet_UP; 
  if(iSys==DGSys_LJet_UP) _sys=BTag_LJet_UP; 

  return bTagSF(nt->evt(),valJets, nt->evt()->mcChannel, (BTagSys) _sys);

}


/*--------------------------------------------------------------------------------*/
// Save/restore orignal lepton - to deal with lepton smearing w/ qFlip
/*--------------------------------------------------------------------------------*/
void SusyBaseAna::saveOriginal()
{
  //Make copy of Met
  new_met.clear();
  new_met = *m_met;

  //Make copy of Electrons/Muons/Leptons
  clearVectors();//for safety

  for(uint iEle=0; iEle<v_sigEle->size(); iEle++){
    Susy::Electron* _e = new Electron(*(v_sigEle->at(iEle)));
    v_save_sigEle.push_back(_e);
  }
  for(uint iMuo=0; iMuo<v_sigMu->size(); iMuo++){
    Susy::Muon* _m = new Muon(*(v_sigMu->at(iMuo)));
    v_save_sigMu.push_back(_m);
  }
  buildLeptons(v_save_sigLep, v_save_sigEle, v_save_sigMu);

  if(dbg()>10){
    cout << ">>> Original Lep" <<endl;
    for(uint iLep=0; iLep<v_save_sigLep.size(); iLep++){
      Susy::Lepton* _l = v_save_sigLep.at(iLep);
      cout << "\t ";  _l->print();
    }
  }
}
/*--------------------------------------------------------------------------------*/
void SusyBaseAna::restoreOriginal(LeptonVector& leptons, const Met *met)
{
  new_met.clear();
  new_met = *met;

  //Not pretty - but essentially the ana change the v_sigEle !!!
  for(uint iEle=0; iEle<v_sigEle->size(); iEle++){
    Susy::Electron* _e = v_sigEle->at(iEle);
    _e->clear();
    _e->operator=(*(v_save_sigEle.at(iEle)));
  }
  for(uint iMuo=0; iMuo<v_sigMu->size(); iMuo++){
    Susy::Muon* _m = v_sigMu->at(iMuo);
    _m->clear();
    _m->operator=(*(v_save_sigMu.at(iMuo)));
  }  

  leptons.clear();
  buildLeptons(leptons, *v_sigEle, *v_sigMu);

  if(dbg()>10){
    cout << "Restored Lep " << leptons.size() <<endl;
    for(uint iLep=0; iLep<leptons.size(); iLep++){
      Susy::Lepton* _l = leptons.at(iLep);
    cout << "\t "; _l->print();
    }
  }
  std::sort(leptons.begin(), leptons.end(), comparePt);
}
/*--------------------------------------------------------------------------------*/
void SusyBaseAna::clearVectors()
{
  for(uint i=0; i<v_save_sigEle.size(); i++){
    delete v_save_sigEle[i];
  }
  for(uint i=0; i<v_save_sigMu.size(); i++){
    delete v_save_sigMu[i];
  }
  v_save_sigEle.clear();
  v_save_sigMu.clear();
  v_save_sigLep.clear();
}

/*--------------------------------------------------------------------------------*/
// To print cutflow
/*--------------------------------------------------------------------------------*/
void SusyBaseAna::print_line(string s, float a, float b, float c)
{
  cout << setprecision(2) << std::fixed 
       << s << "\t" 
       << a << "\t" 
       << b << "\t" 
       << c << endl;
}
/*--------------------------------------------------------------------------------*/
void SusyBaseAna::print_line(string s, int istart, int iend, 
			     float array[LEP_N])
{
  cout << setprecision(2) << std::fixed;
  cout << s << "\t";
  for(int i=istart; i<=iend; i++)
    cout << array[i] << "\t";
  cout << endl;
}
/*--------------------------------------------------------------------------------*/
void SusyBaseAna::print_line(string s, int istart, int iend, int sr,
			     float array[LEP_N][SR_N])
{
  cout << setprecision(2) << std::fixed;
  cout << s << "\t";
  for(int i=istart; i<=iend; i++)
    cout << array[i][sr] << "\t";
  cout << endl;
}

/*--------------------------------------------------------------------------------*/
// Debuging 
/*--------------------------------------------------------------------------------*/
void SusyBaseAna::dumpEvent()
{
  cout << ">>> ";  nt->evt()->print();
  
  cout << "Baseline objects" << endl;
  for(uint iEl=0; iEl < v_baseEle->size(); iEl++){
    cout << "  ";
    v_baseEle->at(iEl)->print();
  }
  for(uint iMu=0; iMu < v_baseMu->size(); iMu++){
    cout << "  ";
    v_baseMu->at(iMu)->print();
  }
  for(uint iJ=0; iJ < v_baseJet->size(); iJ++){
    cout << "  ";
    v_baseJet->at(iJ)->print();
  }

  cout << "Signal objects" << endl;
  for(uint iEl=0; iEl < v_sigEle->size(); iEl++){
    cout << "  ";
    v_sigEle->at(iEl)->print();
  }
  for(uint iMu=0; iMu < v_sigMu->size(); iMu++){
    cout << "  ";
    v_sigMu->at(iMu)->print();
  }
  for(uint iJ=0; iJ < v_sigJet->size(); iJ++){
    cout << "  ";
    v_sigJet->at(iJ)->print();
  }
  cout << "N_C20 " <<  numberOfCLJets(*v_sigJet) 
       << " N_B20 " << numberOfCBJets(*v_sigJet)
       << " N_F30 " << numberOfFJets(*v_sigJet)
       << endl;
}

/*--------------------------------------------------------------------------------*/
// Initialize ToyNt
/*--------------------------------------------------------------------------------*/
void SusyBaseAna::initializeToyNt()
{
  if(m_writeToyNt==true) return; //Already initialized

  // Set the writeToyNt flag
  m_writeToyNt=true;
  cout << " ===> Initialising ToyNt Outputs " << endl;
  
  if(nt->evt()->isMC){
    // Read MC ID, only run over one unique DS while using this option!!!
    TString mcId = "";
    mcId.Form("%i",nt->evt()->mcChannel);
    m_toyNt = new ToyNt(mcId,TString(TOYNT_SR));
  }
  else{
    if(m_method == FLEP){ //DD Fake lepton
      TString ds(string(_hh->sampleName()+"_FAKE"));
      m_toyNt = new ToyNt(ds,TString(TOYNT_SR));
    }
    else{ //Data 
      TString ds(string(_hh->sampleName()+"_DATA"));
      m_toyNt = new ToyNt(ds,TString(TOYNT_SR));
    }
  }
}

/*--------------------------------------------------------------------------------*/
void SusyBaseAna::fillToyNt(uint iSR,uint iSYS,
				 const LeptonVector* leptons, 
				 const JetVector* jets,
				 const Met* met,
				 float _ww)
{
  float corrNpv = nt->evt()->nVtx;
  if(nt->evt()->isMC) corrNpv = GetNVertexBsCorrected(nt->evt()->nVtx);
  
  m_toyNt->FillTreeEvent(nt->evt()->run,
			 nt->evt()->event,
			 nt->evt()->mcChannel,
			 nt->evt()->nVtx,
			 corrNpv,
			 iSR,
			 m_ET,
			 _ww);	 
  m_toyNt->FillTreeLeptons(leptons,*v_baseEle,*v_baseMu,met,nt->evt()->nVtx,nt->evt()->isMC);
  bool _topTag  = passTopTag(*leptons,*v_sigJet,met);
  float mt2 =  getMT2(*leptons, met);
  float metRel = getMetRel(met,*leptons,*jets);
  /*  
  std::cout << "event " <<  nt->evt()->event 
	    << " Met " << met->lv().Pt() << " " << met->lv().Px() << " " << met->lv().Py()
	    << " L0 " << (*leptons->at(0)).Px() << " " << (*leptons->at(0)).Py()
	    << " L1 " << (*leptons->at(1)).Px() << " " << (*leptons->at(1)).Py() 
    	    << " " << mt2 << endl;
  */
  float mll_collApprox = mZTauTau(*leptons->at(0),*leptons->at(1),met->lv());

  float sphericity=-999;
  float sphericityTrans=-999;
  vector<float> px;
  vector<float> py;
  vector<float> pz;
  px.push_back(leptons->at(0)->Px());
  px.push_back(leptons->at(1)->Px());

  py.push_back(leptons->at(0)->Py());
  py.push_back(leptons->at(1)->Py());

  pz.push_back(leptons->at(0)->Pz());
  pz.push_back(leptons->at(1)->Pz());

  float mt2jj=-999;
  float jjAcoplanarity=-999;
  if(jets->size()>=2){
    const TLorentzVector* j1TLV=NULL;
    const TLorentzVector* j2TLV=NULL;
    int iC20j=0;
    for(uint ijet=0; ijet<jets->size(); ijet++){
      const Susy::Jet* jet = jets->at(ijet);
      if(!isCentralLightJet(jet)) continue;
      if(iC20j==0) j1TLV = &(*jet);
      if(iC20j==1) j2TLV = &(*jet);
      iC20j++;
    }
    if(j1TLV && j2TLV){
      px.push_back(j1TLV->Px());
      px.push_back(j2TLV->Px());

      py.push_back(j1TLV->Py());
      py.push_back(j2TLV->Py());

      pz.push_back(j1TLV->Pz());
      pz.push_back(j2TLV->Pz());
    }
    if(j1TLV && j2TLV){
      mt2jj = getMT2(j1TLV, j2TLV, met);
      jjAcoplanarity = acoplanarity(*j1TLV, *j2TLV);
    }
  }

  float llAcoplanarity = acoplanarity(*leptons->at(0),*leptons->at(1));

  sphericity = Sphericity(px,py,pz,false);
  sphericityTrans = Sphericity(px,py,pz,true);

  m_toyNt->FillTreeEventVar(met,metRel,mt2,
			    mt2jj, sphericity, sphericityTrans,
			    llAcoplanarity,jjAcoplanarity,
			    _topTag,mll_collApprox);
  m_toyNt->FillTreeSignalJets(jets,leptons,met,v_preEle, v_preMu);
  //m_toyNt->FillTreeOtherJets(v_baseJet,leptons,met);
  
  float mct     = mCT(*leptons->at(0),*leptons->at(1));
  float mctPerp = mCTperp(*leptons->at(0),*leptons->at(1),met->lv());
  float mctPara = mCTpara(*leptons->at(0),*leptons->at(1),met->lv());
  m_toyNt->FillMCT(mct, mctPerp, mctPara);

  float JZBj = JZBJet(v_sigJet,leptons);
  float JZBm = JZBEtmiss(met,leptons);
  m_toyNt->FillJZB(JZBj, JZBm);
  

  //Write entry to TTree
  m_toyNt->WriteTree();

}
