#include <iomanip>
#include "SusyWeakProdAna/SusyFakeAna.h"
#include "SusyWeakProdAna/SusyAnaCommon.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// SusyFakeAna Constructor
/*--------------------------------------------------------------------------------*/
SusyFakeAna::SusyFakeAna(SusyHistos* _histos):
  SusyBaseAna(_histos,false,true,true)
{
  cout << "-------------------------------" << endl;
  cout << "    Running Fake Analysis        " << endl;
  cout << "-------------------------------" << endl;

  _random = new TRandom3(2055704344);

  n_pass_SS1j       = 0;
  n_pass_SSEM       = 0;
  n_pass_HFTagProbe = 0;

}

/*--------------------------------------------------------------------------------*/
// Main process loop function 
/*--------------------------------------------------------------------------------*/
void SusyFakeAna::doAnalysis(unsigned int isys)
{
  reset();
  SYST = isys;

  if(FILL_TOYNT){
    bool metDetails      = false;
    bool dijetBlock      = true; //off ?
    bool OS2LBlock       = false;
    bool SS2LBlock       = true;
    bool ZBalanceBlock   = false;
    bool diversVarsBlock = false;
    bool fakeBlock       = true;

    initializeToyNt(metDetails, dijetBlock, 
		    OS2LBlock, SS2LBlock, ZBalanceBlock,
		    diversVarsBlock,fakeBlock);
  }
  

  if(!CUTFLOW && v_baseLep->size()<NBASELEPMIN) return;
  if(!selectEvent(v_baseLep, v_sigLep, v_sigJet, m_met)) return;
  
  
  return;
}
/*--------------------------------------------------------------------------------*/
// The Terminate() function is the last function to be called
/*--------------------------------------------------------------------------------*/
void SusyFakeAna::end()
{

  
  cout << "------------------------------------------------------------------------" << endl;
  cout << "SusyFakeAna::event counters" <<endl;
  cout << std::setprecision(0) << std::fixed;
  cout << "read in:            " << n_readin        << endl;
  cout << "pass SUSY grid:     " << n_pass_SUSYGrid << endl;
  cout << "pass GRL:           " << n_pass_GRL      << endl;
  cout << "pass TileTrip:      " << n_pass_TileTrip << endl;
  cout << "pass LarErr:        " << n_pass_LarErr   << endl;
  cout << "pass BadJet:        " << n_pass_BadJet  << endl;
  cout << "pass DeadRegion:    " << n_pass_DeadRegion << endl;
  cout << "pass BadMu:         " << n_pass_BadMuon << endl;
  cout << "pass Cosmic:        " << n_pass_Cosmic  << endl;
  cout << "pass HotSpot:       " << n_pass_HotSpot << endl;
  cout << "pass TTCVeto:       " << n_pass_TTCVeto  << endl;
  cout << "pass Good Vtx       " << n_pass_GoodVtx  << endl;

  cout << "pass atleast 2 base " << n_pass_atleast2BaseLep << endl;
  cout << "pass exactly 2 base " << n_pass_exactly2BaseLep << endl;
  cout << "pass mll20          " << n_pass_mll20   << endl;
  cout << "------------------------------------------------------------------------" << endl;
    
  cout << std::setprecision(1) << std::fixed;

  cout << "------------------------------------------------------------------------" << endl;
  cout << "------------------------------------------------------------------------" << endl;
  cout << " Pass SS1j "        <<  n_pass_SS1j       << endl; 
  cout << " Pass SSEM "        <<  n_pass_SSEM       << endl; 
  cout << " Pass HF TagProbe " <<  n_pass_HFTagProbe << endl;
  cout << "------------------------------------------------------------------------" << endl;
  cout << "------------------------------------------------------------------------" << endl;

  finish();

}


/*--------------------------------------------------------------------------------*/
// Full event selection
/*--------------------------------------------------------------------------------*/
bool SusyFakeAna::selectEvent(LeptonVector* baseLeps, 
			      LeptonVector* signalLeptons, 
			      const JetVector* signalJets,
			      const Met* met)
{

  //
  //Basic pre-selection
  //
  if( !passEventCleaning() ) return false;

  if( baseLeps->size() < NBASELEPMIN ){ 
    if(dbg()>10) cout<<"Fail baselepMIN " << endl;  
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_atleast2BaseLep+=_inc;

  if(baseLeps->size()>NBASELEPMAX ){ 
    if(dbg()>10) cout<<"Fail baselepMAX " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_exactly2BaseLep+=_inc;

  if(! passMll20(baseLeps)){ 
    if(dbg()>10) cout<<"Fail Mll20 " << endl; 
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_mll20+=_inc;
  
  m_ET = getDiLepEvtType(*baseLeps);
  if(m_ET==ET_me) m_ET=ET_em; //Keep EM & ME together
  
  if( !passNLepCut(baseLeps) ){ 
    if(dbg()>10) cout<<"Fail Nlep " << endl; 
    return false;
  }

  if( !passTauVeto(v_sigTau) ){
    if(dbg()>10) cout << "Fail Tau veto " << endl;
    return false;
  }

  if( !passTrigger(baseLeps, m_trigObj, met) ){ 
    if(dbg()>10) cout<<"Fail Trig " << endl;  
    return false; 
  }

  //Event weights
  getEventWeights(baseLeps, signalJets, met);


  if(passSelections(baseLeps, signalLeptons, signalJets, met)){
    m_toyNt->_b_isGenuineSS  =  isGenuineSS(baseLeps,nt->evt()->isMC);
    fillToyNt(SYST, baseLeps, signalJets, met,_ww, _wwBTag, _wwqFlip);
  }

  return true;

}

/*--------------------------------------------------------------------------------*/
// Get event weights
/*--------------------------------------------------------------------------------*/
void SusyFakeAna::getEventWeights(LeptonVector* leptons, 
				  const JetVector* jets,
				  const Met* met)
{
  _ww              = eventWeight(LUMIMODE); 
  float _lepSFW    = getLepSFWeight(leptons);
  float _trigW     = getTriggerWeight(leptons, met->lv().Pt(), jets->size(), nt->evt()->nVtx, SYST);
  _ww              *= _lepSFW * _trigW;  //If we use bTag event, add the BTag weight 

  _wwBTag          = getBTagSF(nt->evt(),v_baseJet,SYST);
  
  float qFlipWeight=1;
  if( nt->evt()->isMC &&  m_ET!=ET_mm && !isGenuineSS(leptons,nt->evt()->isMC)){
    new_met.clear();
    new_met = *met;
    qFlipWeight = getQFlipProb(leptons,&new_met,SYST);
  }
  _wwqFlip = _ww * qFlipWeight;;


}


/*--------------------------------------------------------------------------------*/
// Various CR selection for the fake
/*--------------------------------------------------------------------------------*/
bool SusyFakeAna::passSelections(LeptonVector* baseLeptons, 
				 LeptonVector* leptons, 				 
				 const JetVector* jets,
				 const Met* met)
{
  bool pass_SS1j=false;
  bool pass_SSEM=false;
  bool pass_HFTagProbe=false;

  if( passSS1j(baseLeptons,jets,met) ){
    n_pass_SS1j++;
    m_toyNt->_b_pass_SS1j=true;
    pass_SS1j=true;
  }

  if( passSSEM(baseLeptons,jets,met) ){
    n_pass_SSEM++;
    m_toyNt->_b_pass_SSEM=true;
    pass_SSEM=true;
  }


  if( passHFTagProbe(baseLeptons,jets,met) ){
    n_pass_HFTagProbe++;
    m_toyNt->_b_pass_HFTP=true;
    pass_HFTagProbe=true;
  }

  if(pass_SS1j || pass_SSEM || pass_HFTagProbe) return true;

  return false;
}


/*--------------------------------------------------------------------------------*/
// SS EM event for Electron SF computation
/*--------------------------------------------------------------------------------*/
bool SusyFakeAna::passSS1j(LeptonVector* leptons, 
			   const JetVector* jets,
			   const Met* met)
{
  m_selSS     = true;
  m_minC20    = 1;

  if(!passLJet(jets)) return false;

  if( nt->evt()->isMC &&  m_ET!=ET_mm){
    return true; //GenuineSS and OS pass - OS to wieght w/ qFlip
  }
  else{
    if(!passQQ(leptons)) return false;
  }


  return true;
}

/*--------------------------------------------------------------------------------*/
// SS EM event for Electron SF computation
/*--------------------------------------------------------------------------------*/
bool SusyFakeAna::passSSEM(LeptonVector* leptons, 
			   const JetVector* jets,
			   const Met* met)
{
  //Cut values
  const float cut_mtMax=100;
   
  if(m_ET != ET_em) return false;

  //cout << endl;

  int nProbe = 0;
  float mt = -999;
  int idxTag=-1;
  int idxProbe=-1;
  for(uint i=0; i<leptons->size(); ++i){
    Susy::Lepton* _l = leptons->at(i);
    if(!_l->isEle()){ //muon - flag it as the tag
      idxTag = i;
    }
    else{ //electron - probe
      mt = Mt(leptons->at(i), met); 
      idxProbe = i;  
      nProbe++;
    }
  } 
  //if(mt>cut_mtMax) continue;


  if(nProbe<1) return false;
  
  m_toyNt->_b_SSEM_tagIdx   = idxTag;
  m_toyNt->_b_SSEM_probeIdx = idxProbe;


  //  cout << "SSEM tag " << idxTag << " pT " << leptons->at(idxTag)->Pt()
  //       << " probe " << idxProbe << " pT " << leptons->at(idxProbe)->Pt() << endl;

  return true;
}


/*--------------------------------------------------------------------------------*/
// HF Tag probe for muon/electron HF SF computation
/*--------------------------------------------------------------------------------*/
bool SusyFakeAna::passHFTagProbe(LeptonVector* leptons, 
				 const JetVector* jets,
				 const Met* met)
{
  //Cut values
  const float cut_mllMin = 40;
  const float cut_metMax = 40;
  const float cut_mtMax  = 100;

  if( !(m_ET==ET_em  ||  m_ET==ET_mm)) return false;

  //Look for a muon inside a b-jet - use preMuons
  Lepton* tag   = NULL;
  float   nTag  = 0;
  float   minDR = 999;
  for(uint im=0; im<nt->muo()->size(); ++im){
    Muon* mu = &(nt->muo()->at(im));
    if(mu->Pt() < 6.0) continue;

    for(uint ij=0; ij<jets->size(); ++ij){
      const Jet* jet = jets->at(ij);
      if(!isCentralBJet(jet)) continue;

      float dR = jet->DeltaR(*mu);
      if(dR<minDR && dR<M_J_DR){
	minDR=dR;
	tag = (Lepton*) &(nt->muo()->at(im));
	nTag++;
      }
    }
  }
  if(nTag<1) return false;


  //Look for the probe e or mu. Check that's not the tag
  Lepton* probe   = NULL;
  int nProbe = 0;
  int idxTag=-1;
  int idxProbe=-1;
  for(uint i=0; i<leptons->size(); ++i){
    Susy::Lepton* _l = leptons->at(i);
    if(_l==tag){ //Only stored if pass baseline cuts
      idxTag=i;
      continue; 
    }
    probe = _l;
    idxProbe = i;
    nProbe++;
  }
  if(nProbe!=1) return false;
    
  //Kinematics cuts from MM events
  if(tag->isMu() && probe->isMu()){//MM event
    float mll= Mll(tag,probe);
    if(fabs(mll-MZ)<10) return false;
    if(mll<cut_mllMin) return false;
  }
  
  //Met and mT(probe) cuts
  if(met->lv().Pt()>cut_metMax) return false;
  if(Mt(probe, met)>cut_mtMax) return false;
  
  m_toyNt->_b_HFTP_tagIdx=idxTag;
  m_toyNt->_b_HFTP_probeIdx=idxProbe;

  return true;

}



/*--------------------------------------------------------------------------------*/
// Z tag & probe
// mode:0 electron, 1:muon
/*--------------------------------------------------------------------------------*/
  /*
void SusyFakeAna::ZTagProbe(const Lepton* &_tag, const Lepton* &_probe, int &category, int mode)
{
  category = 0;
  if(nt->evt()->isMC){
    TString ss = _hh->sampleName(); 
    if(mode==0 && !ss.Contains("Zee")) return;
    if(mode==1 && !ss.Contains("Zmumu")) return;
  }
  else{
    if(mode==0 && nt->evt()->stream!=Stream_Egamma) return;
    if(mode==1 && nt->evt()->stream!=Stream_Muons) return;
  }
  
  if(mode==0 && v_baseEle->size()<2) return;
  if(mode==1 && v_baseMu->size()<2) return;

  float mZclosest=999999;
  TL_CATG llType = XX_Undef;
  float mll =0;
  for(uint i=0; i<v_baseLep->size(); i++){
    const Lepton* _l1 = v_baseLep->at(i);
    for(uint j=i+1; j<v_baseLep->size(); j++){
      const Lepton* _l2 = v_baseLep->at(j);   
      if(!isSFOS(_l1, _l2)) continue;
      if(!isZ(_l1, _l2))    continue;

      bool l1_isTight=isSignalLepton(_l1,*v_baseEle,*v_baseMu,nt->evt()->nVtx,nt->evt()->isMC);
      bool l2_isTight=isSignalLepton(_l2,*v_baseEle,*v_baseMu,nt->evt()->nVtx,nt->evt()->isMC);

      llType = getTL(l1_isTight, l2_isTight);
      if( llType == LL ) continue;
      
      mll = Mll(_l1, _l2);
      float dmZ = fabs(mll - MZ);
      if(dmZ>mZclosest) continue;
      mZclosest=dmZ;
  
      //ADD TRIGGER REQ ON TAG

      //Pick the tag & the probe
      if( llType == TT){
	float x = _random->Rndm();
	if(x<0.5){ _tag=_l1; _probe=_l2; }
	else{      _tag=_l2; _probe=_l1; }
	category = 1;
      }
      else if( llType == TL){
	_tag=_l1; _probe=_l2;
	category = 2;
      }
      else if( llType == LT){
	_tag=_l2; _probe=_l1;
	category = 3;
      }
    }
  }

  if(dbg()>3 && category != 0){
    cout << "Z TP mode " << mode 
	 << " llType " << llType
	 << " mll " << mll 
	 << "\n\t Tag "; _tag->print();
    cout << "\t Probe "; _probe->print();
  }

}
  */

/*--------------------------------------------------------------------------------*/
// HF tag & probe
/*--------------------------------------------------------------------------------*/
/*
bool SusyFakeAna::HFTagProbe(const Lepton* &_tag, const Lepton* &_probe, int &category, int mode)
{
  const float mdR=0.6;    //dR(mu,j-btag)
  const float amdR=1;     //min dR to away side (l,bMuJet)

  if(m_met->lv().Pt()>40) return false;

  //TODO ADD TRIGGER
  
  //find b-tag muon-jet
  MuonVector muons = getPreMuons(nt, NtSys_NOM);
  JetVector  bMuJets;
  MuonVector bMuons;
  //Loop over the baseline Jets 
  for(uint j=0; j<v_baseJet->size(); j++){
    Jet* _j = v_baseJet->at(j);
    if(_j->Pt()<JET_PT_B20_CUT) continue;
    if(fabs(_j->Eta())>JET_ETA_CUT) continue;
    if(!isBJet(_j,MV1_85)) continue;
    bool hasMu=false;
    for(uint i=0; i<muons.size(); i++){
      Muon* _m = muons.at(i);
      if(_m->DeltaR(*_j)>mdR) continue;
      bMuons.push_back(_m);
      hasMu=true;
    }
    if(hasMu) bMuJets.push_back(_j);
  }
  if(bMuJets.size()==0) return false;
  _tag = bMuons[0];//pick 1st one

  //Find away side lepton - furthest away
  float mindR=0;
  for(uint ilep=0; ilep<v_baseLep->size(); ilep++){
    const Lepton* _lep = v_baseLep->at(ilep);
    if(mode==0 && !_lep->isEle()) continue;
    if(mode==1 && !_lep->isMu()) continue;
    //Need to check that probe cannot overlap tag ?
    //for(uint imu=0; imu<bMuons.size(); imu++){}
    for(uint ibj=0; ibj<bMuJets.size(); ibj++){
      const Jet* _j = bMuJets.at(ibj);
      float dR = _lep->DeltaR(*_j);
      if(dR<amdR || dR<mindR) continue;
      mindR=dR;
      _probe = _lep;
    }
  }
  if(_probe==NULL) return false;

  if(Mt(_probe, m_met)>40) return false;
  
  if(dbg()>3){
    cout << "HF " << mode 
	 << " nbMuJets " << bMuJets.size()
	 << " nbMu " << bMuons.size()
	 << "\n\t bJet "; bMuJets[0]->print();   
    cout << "\t Tag "; _tag->print();
    cout << "\t Probe "; _probe->print();
  }

  return true;
}
*/

