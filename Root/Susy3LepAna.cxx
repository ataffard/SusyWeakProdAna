#include <iomanip>
#include "SusyWeakProdAna/Susy3LepAna.h"
#include "SusyWeakProdAna/PhysicsTools.h"

#include "SusyWeakProdAna/SusyAnaCommon.h"

using namespace std;
using namespace Susy;

const string ML_SRNAME[] = {"SR3Lep", "SRB", 
			    "SR1a","SR1b","SR2",
			    "VR0", "VR1","VR2","VR3",
			    "VRWZ", "NRWZ", 
			    "SR4Lep", "SR4LepNoZ"
};

/*--------------------------------------------------------------------------------*/
// Susy3LepAna Constructor
/*--------------------------------------------------------------------------------*/
Susy3LepAna::Susy3LepAna(SusyHistos* _histos) :
  m_dbg(0),
  _hh(_histos),
  m_useLooseLep(false),
  m_cutNBaseLep(false),
  m_nLep3Min   (  3  ),
  m_nLep3Max   (  3  ),
  m_nLep4Min   (  4  ),
  m_nLep4Max   (  99  ),
  m_selZ       (false),
  m_vetoZ      (false),
  m_selB       (false),
  m_vetoB      (false),
  m_selSFOS    (false),
  m_vetoSFOS   (false),
  m_metMin     ( -1  ),
  m_metMax     ( -1  ),
  m_minMt      ( -1  ),
  m_lepPtMin   ( -1  ),
  m_writeOut   (false)
{
  n_readin        = 0;
  n_pass_HotSpot  = 0;
  n_pass_BadJet   = 0;
  n_pass_BadMuon  = 0;
  n_pass_Cosmic   = 0;

  for(int i=0; i<ML_NSR; ++i){
    n_pass_nLep3[i]    = 0;
    n_pass_nLep4[i]    = 0;
    n_pass_trig[i]     = 0;
    n_pass_sfos[i]     = 0;
    n_pass_met[i]      = 0;
    n_pass_z[i]        = 0;
    n_pass_bjet[i]     = 0;
    n_pass_mt[i]       = 0;
    n_pass_lep30[i]    = 0;
  }

  m_trigObj = new TrilTrigLogic();
  m_trigObj->loadTriggerMaps();

  if(m_writeOut) {
    out.open("event.dump");
  }
}
/*--------------------------------------------------------------------------------*/
// Hook the containers
/*--------------------------------------------------------------------------------*/
void Susy3LepAna::hookContainers(Susy::SusyNtObject* _ntPtr,
				 ElectronVector* _baseEleA, ElectronVector* _sigEleA,
				 MuonVector*     _baseMuoA, MuonVector*     _sigMuoA,
				 LeptonVector*   _baseLepA, LeptonVector*   _sigLepA,
				 JetVector*      _baseJetA, JetVector*      _sigJetA)
{
  nt         = _ntPtr;
  v_baseEle  = _baseEleA;
  v_sigEle   = _sigEleA;
  v_baseMu   = _baseMuoA;
  v_sigMu    = _sigMuoA;
  v_baseLep  = _baseLepA;
  v_sigLep   = _sigLepA;
  v_baseJet  = _baseJetA;
  v_sigJet   = _sigJetA;
}

/*--------------------------------------------------------------------------------*/
// Main process loop function 
/*--------------------------------------------------------------------------------*/
void Susy3LepAna::doAnalysis()
{
  n_readin++;

  reset();
  // Check Event
  if(m_useLooseLep){  //use baseline leptons - for fake MM estimate
    if(!selectEvent(v_baseLep, v_sigJet, m_met)) return;
  }
  else{
    if(!selectEvent(v_sigLep, v_sigJet, m_met)) return;
  }

  return;
}

/*--------------------------------------------------------------------------------*/
// The Terminate() function is the last function to be called
/*--------------------------------------------------------------------------------*/
void Susy3LepAna::end()
{
  
  cout << endl;
  cout << endl;
  cout << "**********************************************" << endl;
  cout << "Susy3LepAna event counters"    << endl;
  cout << "read in     :  " << n_readin        << endl;
  cout << "pass HotSpot:  " << n_pass_HotSpot  << endl;
  cout << "pass BadJet :  " << n_pass_BadJet   << endl;
  cout << "pass BadMu  :  " << n_pass_BadMuon  << endl;
  cout << "pass Cosmic :  " << n_pass_Cosmic   << endl;
  

  //loop over SR and print only the relevant ones
  for(uint iSR=ML_SR3Lep; iSR<ML_NSR; iSR++){
    if(iSR>= ML_SR4lep) continue;
    cout << "====================================" << endl;
    cout << "Selected SR " << ML_SRNAME[iSR] << endl;
    
    cout << "pass nLep   :  " << n_pass_nLep3[iSR]    << endl;
    cout << "pass trig   :  " << n_pass_trig[iSR]     << "\t(" << 100.*n_pass_trig[iSR]/n_pass_nLep3[iSR] << "%)" << endl;
    cout << "pass sfos   :  " << n_pass_sfos[iSR]     << endl;
    cout << "pass met    :  " << n_pass_met[iSR]      << endl;
    cout << "pass z      :  " << n_pass_z[iSR]        << endl;
    cout << "pass b-jet  :  " << n_pass_bjet[iSR]     << endl;
    cout << "pass mt     :  " << n_pass_mt[iSR]       << endl;
    cout << "pass 30x3   :  " << n_pass_lep30[iSR]    << endl;
  }
  
  if(m_writeOut) {
    out.close();
  }

}

/*--------------------------------------------------------------------------------*/
// Reset
/*--------------------------------------------------------------------------------*/
void Susy3LepAna::reset()
{
  _ww=0;
}
/*--------------------------------------------------------------------------------*/
// Full event selection
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::selectEvent(const LeptonVector* leptons, 
			      const JetVector* jets, const Met* met)
{
  if(!passEventCleaning() ) return false;
  
  for(uint iSR=ML_SR3Lep; iSR<ML_NSR; iSR++){
    string sSR=ML_SRNAME[iSR];
    setSelection(sSR);
    SR=iSR;

    if(iSR>=ML_SR4lep){ //4Lep
       int icut=0;
       if(!passNLep4Cut(leptons)) continue;
       if(!passTrigger(leptons))  continue;
       setEventWeight(LUMIMODE); //set _ww to the appropriate weighting
       float _wwBck= _ww;
       float bWeight = getBTagSF(nt->evt(),v_sigJet);
       if( LUMIMODE != NOLUMI ) _ww = _wwBck * bWeight;
       
       _hh->H1FILL(_hh->ML_cutflow[SR],icut++,_ww);
       
       if( !passMetCut (met))     continue;
       _hh->H1FILL(_hh->ML_cutflow[SR],icut++,_ww); 
      
       if(!passZCut(leptons))     continue;
       _hh->H1FILL(_hh->ML_cutflow[SR],icut++,_ww); 
    }
    else{ //3Lep
      int icut=0;
      if(!passNBaseLepCut(v_baseLep)) continue;
      if(!passNLep3Cut(leptons)) continue;
      if(!passTrigger(leptons))  continue;
      setEventWeight(LUMIMODE); //set _ww to the appropriate weighting
      float _wwBck= _ww;
      
      _hh->H1FILL(_hh->ML_cutflow[SR],icut++,_ww);

      if( !passSFOSCut(leptons)) continue;
      _hh->H1FILL(_hh->ML_cutflow[SR],icut++,_ww); 
      
      if( !passMetCut (met))     continue;
      _hh->H1FILL(_hh->ML_cutflow[SR],icut++,_ww); 

      if(!passZCut(leptons))     continue;
      _hh->H1FILL(_hh->ML_cutflow[SR],icut++,_ww); 
      
      float bWeight = getBTagSF(nt->evt(),v_sigJet);
      if( LUMIMODE != NOLUMI ) _ww = _wwBck * bWeight;
      
      if( !passBJetCut())        continue;
      _hh->H1FILL(_hh->ML_cutflow[SR],icut++,_ww);

      /*
      if(dbg()==-99 && hasSFOS(*v_sigLep)){
	uint zl1, zl2;
	float mT=0;
	bestZ(zl1, zl2, *v_sigLep);
	for(uint iL=0; iL<v_sigLep->size(); iL++)	{
	  if(iL!=zl1 && iL!=zl2)  mT= Mt(v_sigLep->at(iL),m_met);
	}
	cout << "PASS " << ML_SRNAME[SR] 
	     << " " << nt->evt()->run << " " << nt->evt()->event 
	     << " " << v_sigJet->size() << " " << mT
	     <<endl;
      }
      */
      
      if( !passMtCut  (leptons,met ))  continue;
      _hh->H1FILL(_hh->ML_cutflow[SR],icut++,_ww); 
      
      if( !passLepPtCut(leptons)    )  continue;
      _hh->H1FILL(_hh->ML_cutflow[SR],icut++,_ww); 
    }
    
    if(dbg()==-99) cout << "PASS " << ML_SRNAME[SR] 
			<< " " << nt->evt()->run 
			<< " " << nt->evt()->event <<endl;
    fillHistograms(iSR, leptons, jets, met);
  }

  if( m_writeOut ) {
    out << nt->evt()->run << " " << nt->evt()->event << endl;
  }

  return true;
}

/*--------------------------------------------------------------------------------*/
// Set Selection 
/*--------------------------------------------------------------------------------*/
void Susy3LepAna::setSelection(std::string s)
{
  m_sel=s;

  m_cutNBaseLep=false;
  m_nLep3Min=3;  
  m_nLep3Max=3;
  m_nLep4Min=4;
  m_nLep4Max=99; 
  m_selZ=false;
  m_vetoZ=false;
  m_selB=false;
  m_vetoB=false;
  m_selSFOS=false;
  m_vetoSFOS=false;
  m_metMin=-1;  
  m_metMax=-1;  
  m_minMt=-1;  
  m_lepPtMin=-1;

  if(m_sel=="SR3Lep"){
    //Default - 3 signal leptons 
  }
  else if (m_sel=="SRB"){ //Baseline SR
    m_selSFOS=true;
    m_metMin=75;
  }
  else if (m_sel=="SR1a"){ //Zveto loose
    m_selSFOS=true;
    m_metMin=75;
    m_vetoZ=true;
    m_vetoB=true;
  }
  else if (m_sel=="SR1b"){ //Zveto tight
    m_selSFOS=true;
    m_metMin=75;
    m_vetoZ=true;
    m_vetoB=true;
    m_minMt=90;
    m_lepPtMin=30;
  }
  else if (m_sel=="SR2"){ //In Z peal
    m_selZ=true;
    m_selSFOS=true;
    m_metMin=75;
    m_minMt=90;
  }
  else if (m_sel=="VR0"){ //Low met validation inclusive
    m_metMax=75;
  }
  else if (m_sel=="VR1"){ //Intermediate met Zveto
    m_metMin=30;
    m_metMax=75;
    m_selSFOS=true;
    m_vetoZ=true; 
  }
  else if (m_sel=="VR2"){ //High Met veto SFOS
    m_metMin=50;
    m_vetoSFOS=true;
  }
  else if (m_sel=="VR3"){ //Intermediate met in Z peak
    m_metMin=50;
    m_metMax=75;
    m_selSFOS=true;
    m_selZ=true;
  }
  else if (m_sel=="VRWZ"){ //WZ validation region
    m_cutNBaseLep = true;
    m_selSFOS = true;
    m_selZ = true;
    m_metMin = 55;
    m_metMax = 70;
    m_minMt = 40;
    m_vetoB = true;
  }
  else if (m_sel=="NRWZ"){//WZ normalisation region
    m_cutNBaseLep = true;
    m_selSFOS = true;
    m_selZ = true;
    m_metMax = 50;
    m_minMt = 40;
    m_vetoB = true;
  }
  else if(m_sel=="SR4Lep"){
    //Default - 4 signal leptons 
    m_metMin = 50;
  }
  else if(m_sel=="SR4LepNoZ"){
    m_metMin = 50;
    m_vetoZ=true;
  }


}

/*--------------------------------------------------------------------------------*/
// Event weight
/*--------------------------------------------------------------------------------*/
void Susy3LepAna::setEventWeight(int mode)
{
  _ww=1;
  if(mode==NOLUMI) _ww= 1;
  else if(mode==LUMI1FB){
    _ww=getEventWeightAB3(nt->evt());
  }
  else if(mode==LUMI5FB){
    _ww=getEventWeightAB(nt->evt());
  }
  else if(mode==LUMI10FB){
    _ww=getEventWeight(nt->evt());
  }
  
  if(mode>0 && nt->evt()->isMC){
    for(uint ilep=0; ilep<v_sigLep->size(); ilep++){
      const Susy::Lepton* _l = v_sigLep->at(ilep);
      _ww *= _l->effSF;
    }
  }

  //TODO: Add trigger weighting 
}
/*--------------------------------------------------------------------------------*/
// Event cleaning
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passEventCleaning()
{
  int cutFlag = nt->evt()->evtFlag[NtSys_NOM];

  if(!passHotSpot(cutFlag)) return false;
  n_pass_HotSpot++;

  if(!passBadJet (cutFlag)) return false;
  n_pass_BadJet++;

  if(!passBadMuon(cutFlag)) return false;
  n_pass_BadMuon++;

  if(!passCosmic (cutFlag)) return false;
  n_pass_Cosmic++;

  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passNBaseLepCut(const LeptonVector* baseLeptons)
{
  if(m_cutNBaseLep){
    uint nLep = baseLeptons->size();
    if(m_nLep3Min>=0 && nLep < m_nLep3Min) return false;
    if(m_nLep3Max>=0 && nLep > m_nLep3Max) return false;
  }
  return true;
}

/*--------------------------------------------------------------------------------*/
// Analysis cuts
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passNLep3Cut(const LeptonVector* leptons)
{
  uint nLep = leptons->size();
  if(m_nLep3Min>=0 && nLep < m_nLep3Min) return false;
  if(m_nLep3Max>=0 && nLep > m_nLep3Max) return false;
  n_pass_nLep3[SR]++;
  return true;
}
/*--------------------------------------------------------------------------------*/
// Analysis cuts
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passNLep4Cut(const LeptonVector* leptons)
{
  uint nLep = leptons->size();
  if(m_nLep4Min>=0 && nLep < m_nLep4Min) return false;
  if(m_nLep4Max>=0 && nLep > m_nLep4Max) return false;
  n_pass_nLep4[SR]++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passTrigger(const LeptonVector* leptons) 
{
  if(!m_trigObj->passTriggerMatching(*leptons, nt->evt())) return false;
  n_pass_trig[SR]++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passSFOSCut(const LeptonVector* leptons)
{
  bool sfos = hasSFOS(*leptons);
  if(m_vetoSFOS &&  sfos) return false;
  if(m_selSFOS  && !sfos) return false;
  n_pass_sfos[SR]++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passMetCut(const Met* met)
{
  double missEt = met->lv().Et();
  if( m_metMin >= 0 && missEt < m_metMin ) return false;
  if( m_metMax >= 0 && missEt > m_metMax ) return false;
  n_pass_met[SR]++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passZCut(const LeptonVector* leptons)
{
  bool hasz = hasZ(*leptons);
  if( m_vetoZ  &&  hasz ) return false;
  if( m_selZ   && !hasz ) return false;
  n_pass_z[SR]++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passBJetCut( )
{
  bool hasB = hasBJet(*v_sigJet);
  if( m_vetoB &&  hasB ) return false;
  if( m_selB  && !hasB ) return false;
  n_pass_bjet[SR]++;
  return true;
}
/*--------------------------------------------------------------------------------*/
float Susy3LepAna::getBTagSF(const Susy::Event*, const JetVector* jets)
{
  if(!(m_vetoB || m_selB)) return 1; //Not using btag
  //Kepp just in case need to change pt threshold
  JetVector  valJets;
  valJets.clear();
  for(uint i=0; i<jets->size(); ++i){
    const Jet* jet = jets->at(i);
    //    if(jet->Pt()>m_btagPtMin) 
      valJets.push_back(jet);
  }
  
  if(valJets.size()==0) return 1;//safety.
  return bTagSF(nt->evt(),valJets);
}

/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passMtCut(const LeptonVector* leptons, const Met* met)
{
  // Find the best Z candidate pair, use remaining lepton to form Mt
  if(m_minMt > 0){
    uint zl1, zl2;
    bestZ(zl1, zl2, *leptons);
    for(uint iL=0; iL<leptons->size(); iL++){
      if(iL!=zl1 && iL!=zl2){
        if( Mt(leptons->at(iL),met) < m_minMt ) return false;
      }
    }
  }
  n_pass_mt[SR]++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passLepPtCut(const LeptonVector* leptons)
{
  if(m_lepPtMin > 0){
    int nLep30=0;
    for(uint iL=0; iL<leptons->size(); iL++){
      if(leptons->at(iL)->Pt()<m_lepPtMin) continue;
      nLep30++;
    }
    if(nLep30<3) return false;
    n_pass_lep30[SR]++;
    return true;
  }
  n_pass_lep30[SR]++;
  return true;
}
/*--------------------------------------------------------------------------------*/
void Susy3LepAna::fillHistograms(uint iSR,
				 const LeptonVector* leptons, 
				 const JetVector* jets,
				 const Met* met)
{
  _hh->H1FILL(_hh->ML_pred[iSR],0,_ww); 

  _hh->H1FILL(_hh->ML_nLep[iSR], leptons->size(), _ww); 
  _hh->H1FILL(_hh->ML_evtCatgUnOrdered[iSR], evtCatgUnOrd(leptons), _ww);
  _hh->H1FILL(_hh->ML_evtCatgOSpair[iSR], evtCatgOrd(leptons,true), _ww);
  _hh->H1FILL(_hh->ML_evtCatgSSpair[iSR], evtCatgOrd(leptons,false), _ww);
  
  bool sfos = hasSFOS(*leptons);
  TLorentzVector _3l;
  TLorentzVector _4l;
  for(uint ilep=0; ilep<leptons->size(); ilep++){
    const Susy::Lepton* _l = leptons->at(ilep);
    if(ilep>=4) continue;
    if(ilep<3) _3l += (*_l);
    if(ilep<4) _4l += (*_l);
    if(ilep==0){
      _hh->H1FILL(_hh->ML_ptl1[iSR],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etal1[iSR],_l->Eta(),_ww); 
      _hh->H1FILL(_hh->ML_d0Sl1[iSR],_l->d0/_l->errD0,_ww); 
      _hh->H1FILL(_hh->ML_z0sinthetal1[iSR],_l->z0SinTheta(),_ww); 
      _hh->H1FILL(_hh->ML_orgl1[iSR],getType(_l->mcOrigin,_l->mcType,_hh->sampleName()),_ww); 
    }
    else if(ilep==1){
      _hh->H1FILL(_hh->ML_ptl2[iSR],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etal2[iSR],_l->Eta(),_ww); 
      _hh->H1FILL(_hh->ML_d0Sl2[iSR],_l->d0/_l->errD0,_ww); 
      _hh->H1FILL(_hh->ML_z0sinthetal2[iSR],_l->z0SinTheta(),_ww); 
      _hh->H1FILL(_hh->ML_orgl2[iSR],getType(_l->mcOrigin,_l->mcType,_hh->sampleName()),_ww); 
    }
    else if(ilep==2){
      _hh->H1FILL(_hh->ML_ptl3[iSR],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etal3[iSR],_l->Eta(),_ww); 
      _hh->H1FILL(_hh->ML_d0Sl3[iSR],_l->d0/_l->errD0,_ww); 
      _hh->H1FILL(_hh->ML_z0sinthetal3[iSR],_l->z0SinTheta(),_ww); 
      _hh->H1FILL(_hh->ML_orgl3[iSR],getType(_l->mcOrigin,_l->mcType,_hh->sampleName()),_ww); 
    }
    else if(ilep==3){
      _hh->H1FILL(_hh->ML_ptl4[iSR],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etal4[iSR],_l->Eta(),_ww); 
      _hh->H1FILL(_hh->ML_d0Sl4[iSR],_l->d0/_l->errD0,_ww); 
      _hh->H1FILL(_hh->ML_z0sinthetal4[iSR],_l->z0SinTheta(),_ww); 
      _hh->H1FILL(_hh->ML_orgl4[iSR],getType(_l->mcOrigin,_l->mcType,_hh->sampleName()),_ww); 
    }
  }
  
  _hh->H1FILL(_hh->ML_AllMlll[iSR],_3l.M(),_ww); 
  if(sfos){
    _hh->H1FILL(_hh->ML_SFOSMlll[iSR],_3l.M(),_ww); 
    
    //Compute MT
    //Find the 2 SFOS leptons - closest to Z peak
    //Use the 3rd one to reco MT
    uint zl1, zl2;
    float mT=0;
    bestZ(zl1, zl2, *leptons);
    for(uint iL=0; iL<leptons->size(); iL++)	{
      if(iL!=zl1 && iL!=zl2)  mT= Mt(leptons->at(iL),met);
    }
    _hh->H1FILL(_hh->ML_SFOSMT[iSR],mT,_ww); 
  }
  
  //Etmiss
  _hh->H1FILL(_hh->ML_etmiss[iSR],met->lv().Pt(),_ww); 
  _hh->H1FILL(_hh->ML_metRefEle[iSR],met->refEle,_ww); 
  _hh->H1FILL(_hh->ML_metRefGam[iSR],met->refGamma,_ww); 
  _hh->H1FILL(_hh->ML_metRefMuo[iSR],met->refMuo,_ww); 
  _hh->H1FILL(_hh->ML_metRefJet[iSR],met->refJet,_ww); 
  _hh->H1FILL(_hh->ML_metRefSJet[iSR],met->softJet,_ww); 
  _hh->H1FILL(_hh->ML_metCellout[iSR],met->refCell,_ww); 

  //Dilepton mass
  for(uint iL1=0; iL1<leptons->size(); iL1++)
    for(uint iL2=iL1+1; iL2<leptons->size(); iL2++){
      float mll = Mll(leptons->at(iL1),leptons->at(iL2));
      _hh->H1FILL(_hh->ML_AllMll[iSR],mll,_ww); 
      if(isSFOS(leptons->at(iL1),leptons->at(iL2)))
	_hh->H1FILL(_hh->ML_SFOSMll[iSR],mll,_ww); 
    }
  
  int nBJets=0;
  _hh->H1FILL(_hh->ML_nJets[iSR],jets->size(),_ww); 
  for(uint ijet=0; ijet<jets->size(); ijet++){
    const Susy::Jet* _j = jets->at(ijet);
    if(isBJet(_j,MV1_85)){
      nBJets++;
      _hh->H1FILL(_hh->ML_ptbj[iSR],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etabj[iSR],_j->Eta(),_ww); 
    }
    if(ijet==0){
      _hh->H1FILL(_hh->ML_ptj1[iSR],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etaj1[iSR],_j->Eta(),_ww); 
    }
    if(ijet==1){
      _hh->H1FILL(_hh->ML_ptj2[iSR],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etaj2[iSR],_j->Eta(),_ww); 
    }
    if(ijet==2){
      _hh->H1FILL(_hh->ML_ptj3[iSR],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etaj3[iSR],_j->Eta(),_ww); 
    }
    if(ijet==3){
      _hh->H1FILL(_hh->ML_ptj4[iSR],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etaj4[iSR],_j->Eta(),_ww); 
    }
  }
  _hh->H1FILL(_hh->ML_nBJets[iSR],nBJets,_ww); 
  
}
/*--------------------------------------------------------------------------------*/
int Susy3LepAna::evtCatgUnOrd(const LeptonVector* leptons){
  int evtCatgUnOrd=99;
  int nE=0;
  int nM=0;
  if(leptons->size()<3) return evtCatgUnOrd;
  for(uint iL=0; iL<leptons->size(); iL++){
    if( leptons->at(iL)->isEle() ) nE++;
    if( leptons->at(iL)->isMu()  ) nM++;
  }
  
  if(nE==3)      evtCatgUnOrd=0;
  else if(nE==2) evtCatgUnOrd=1;
  else if(nM==3) evtCatgUnOrd=2;
  else if(nM==2) evtCatgUnOrd=3;
  
  return evtCatgUnOrd;
}

/*--------------------------------------------------------------------------------*/
int Susy3LepAna::evtCatgOrd(const LeptonVector* leptons, bool useOS){
  int evtCatgOrd=99;
  if(leptons->size()<3) return evtCatgOrd;
  if(leptons->size()>3) return evtCatgOrd;
  
  int lType[3];
  int qProdLeadPair=1;
  for(uint iL=0; iL<leptons->size(); iL++){
    lType[iL] = (leptons->at(iL)->isEle()) ? 0 : 1;
    if(iL<2) qProdLeadPair *= leptons->at(iL)->q;
  }

  if(useOS && qProdLeadPair>0) return evtCatgOrd=99; //SS
  else if(!useOS && qProdLeadPair<0) return evtCatgOrd=99; //OS

  if(lType[0]==0 && lType[1]==0){ //eeX
    if(lType[2]==0) evtCatgOrd=0;
    if(lType[2]==1) evtCatgOrd=1;
  }
  else if(lType[0]==1 && lType[1]==1){//mmX
    if(lType[2]==0) evtCatgOrd=2;
    if(lType[2]==1) evtCatgOrd=3;
  }
  else if((lType[0]==0 && lType[1]==1) || 
	  (lType[0]==1 && lType[1]==0)){
    if(lType[2]==0) evtCatgOrd=4;
    if(lType[2]==1) evtCatgOrd=5;
  }
  return evtCatgOrd=99;
}
