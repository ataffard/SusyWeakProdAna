#include <iomanip>
#include "SusyWeakProdAna/Susy3LepAna.h"
#include "SusyWeakProdAna/SusyAnaCommon.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// Susy3LepAna Constructor
/*--------------------------------------------------------------------------------*/
Susy3LepAna::Susy3LepAna(SusyHistos* _histos) :
  _hh(_histos),
  m_nLep3Min   (  3  ),
  m_nLep3Max   (  3  ),
  m_nLep4Min   (  4  ),
  m_nLep4Max   (  99  ),
  m_selectZ   (false),
  m_vetoZ     (true),
  m_selectB   (false),
  m_vetoB     (true),
  m_selectSFOS(true),
  m_vetoSFOS  (false),
  m_metMin    ( 75  ),
  m_minMt     ( -1  ),
  m_writeOut  (false)
{
  n_readin        = 0;
  n_pass_LAr      = 0;
  n_pass_BadJet   = 0;
  n_pass_BadMuon  = 0;
  n_pass_Cosmic   = 0;
  n_pass_nLep3    = 0;
  n_pass_nLep4    = 0;
  n_pass_trig     = 0;
  n_pass_sfos     = 0;
  n_pass_met      = 0;
  n_pass_z        = 0;
  n_pass_bjet     = 0;
  n_pass_mt       = 0;
  n_pass_lep30    = 0;

  m_trigObj = new TrilTrigLogic(nt);
  m_trigObj->loadTriggerMaps();

  //  setAnaType(Ana_3Lep);

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

  // Check Event
  if(!selectEvent(v_sigLep, v_sigJet, m_met)) return;

  fillHistograms();

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
  cout << "pass LAr    :  " << n_pass_LAr      << endl;
  cout << "pass BadJet :  " << n_pass_BadJet   << endl;
  cout << "pass BadMu  :  " << n_pass_BadMuon  << endl;
  cout << "pass Cosmic :  " << n_pass_Cosmic   << endl;
  cout << "pass nLep   :  " << n_pass_nLep3    << endl;
  cout << "pass trig   :  " << n_pass_trig     << "\t(" << 100.*n_pass_trig/n_pass_nLep3 << "%)" << endl;
  cout << "pass sfos   :  " << n_pass_sfos     << endl;
  cout << "pass met    :  " << n_pass_met      << endl;
  cout << "pass z      :  " << n_pass_z        << endl;
  cout << "pass b-jet  :  " << n_pass_bjet     << endl;
  cout << "pass mt     :  " << n_pass_mt       << endl;
  cout << "pass 30x3   :  " << n_pass_lep30    << endl;

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
  
  isDGML_SR1a = false;
  isDGML_SR1b = false;
  isDGML_SR2  = false;
  isML_4Lep   = false;
  isML_4LepnoZ = false;
  

}
/*--------------------------------------------------------------------------------*/
// Full event selection
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::selectEvent(const LeptonVector* leptons, const JetVector* jets, const Met* met)
{
  // In this method place all event selection cuts.
  int flag = nt->evt()->evtFlag[NtSys_NOM];

  if( !passLAr(flag            ))  return false;
  n_pass_LAr++;
  if( !passBadJet(flag         ))  return false;
  n_pass_BadJet++;
  if( !passBadMuon(flag        ))  return false;
  n_pass_BadMuon++;
  if( !passCosmic (flag        ))  return false;
  n_pass_Cosmic++;

  //3 lep SR
  if( passNLep3Cut(leptons     )){
    if( !passTrigger(leptons     ))  return false;
    setEventWeight(LUMIMODE); //set _ww to the appropriate weighting
    
    for(uint j=0; j<ML_NSR; j++)
      _hh->H1FILL(_hh->ML_cutflow[j],0,_ww); 
    
    if( !passSFOSCut(leptons     ))  return false;
    for(uint j=0; j<ML_NSR; j++)
      _hh->H1FILL(_hh->ML_cutflow[j],1,_ww); 
    
    //SR1a
    ML_SR SR=ML_SR1a;
    if( !passMetCut (met         ))  return false;
    _hh->H1FILL(_hh->ML_cutflow[SR],2,_ww); 
    if( !passZCut   (leptons     ))  return false;
    _hh->H1FILL(_hh->ML_cutflow[SR],3,_ww); 
    if( !passBJetCut(            ))  return false;
    _hh->H1FILL(_hh->ML_cutflow[SR],4,_ww); 
    isDGML_SR1a =true;

    //SR1b
    SR=ML_SR1b;
    if( !passMtCut  (leptons,met ))  return false;
    _hh->H1FILL(_hh->ML_cutflow[SR],2,_ww); 

    if( !passLepPtCut(leptons)    )  return false;
    _hh->H1FILL(_hh->ML_cutflow[SR],3,_ww); 
    isDGML_SR1b = true;
  }
  //>=4 lep SR's
  else if( passNLep4Cut(leptons     )){
    
  }
  else return false;


  if( m_writeOut ) {
    out << nt->evt()->run << " " << nt->evt()->event << endl;
  }

  return true;
}
/*--------------------------------------------------------------------------------*/
// Event weight
/*--------------------------------------------------------------------------------*/
void Susy3LepAna::setEventWeight(int mode)
{
  if(mode==0) _ww=nt->evt()->fullWeight();
  if(mode==1){
    _ww=nt->evt()->wPileup1fb 
      * nt->evt()->xsec 
      * nt->evt()->lumiSF;
  }
  for(uint ilep=0; ilep<v_sigLep->size(); ilep++){
    const Susy::Lepton* _l = v_sigLep->at(ilep);
    _ww *= _l->effSF;
  }
  //TODO: Add trigger weighting 
}

/*--------------------------------------------------------------------------------*/
// Analysis cuts
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passNLep3Cut(const LeptonVector* leptons)
{
  uint nLep = leptons->size();
  if(m_nLep3Min>=0 && nLep < m_nLep3Min) return false;
  if(m_nLep3Max>=0 && nLep > m_nLep3Max) return false;
  n_pass_nLep3++;
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
  n_pass_nLep4++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passTrigger(const LeptonVector* leptons) 
{
  //ANYES - TODO
  n_pass_trig++;
  return true;
  /*
  if(!m_trigObj->passTriggerMatching(*leptons)) return false;
  n_pass_trig++;
  return true;
  */
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passSFOSCut(const LeptonVector* leptons)
{
  bool sfos = hasSFOS(*leptons);
  if(m_vetoSFOS   &&  sfos) return false;
  if(m_selectSFOS && !sfos) return false;
  n_pass_sfos++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passMetCut(const Met* met)
{
  double missEt = met->lv().Et();
  if( m_metMin >= 0 && missEt < m_metMin ) return false;
  n_pass_met++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passLepPtCut(const LeptonVector* leptons)
{
  if(leptons->size()<3) return false;
  int nLep30=0;
  for(uint iL=0; iL<leptons->size(); iL++){
    if(leptons->at(iL)->Pt()>30) nLep30++;
  }
  if(nLep30<3) return false;
  n_pass_lep30++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passZCut(const LeptonVector* leptons)
{
  bool hasz = hasZ(*leptons);
  if( m_vetoZ   &&  hasz ) return false;
  if( m_selectZ && !hasz ) return false;
  n_pass_z++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passBJetCut( )
{
  bool hasB = hasBJet(*v_sigJet);
  if( m_vetoB   &&  hasB ) return false;
  if( m_selectB && !hasB ) return false;
  n_pass_bjet++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::passMtCut(const LeptonVector* leptons, const Met* met)
{
  // Find the best Z candidate pair, use remaining lepton to form Mt
  if(m_minMt > 0)
  {
    uint zl1, zl2;
    bestZ(zl1, zl2, *leptons);

    for(uint iL=0; iL<leptons->size(); iL++)
    {
      if(iL!=zl1 && iL!=zl2)
      {
        if( Mt(leptons->at(iL),met) < m_minMt ) return false;
      }
    }
  }

  n_pass_mt++;
  return true;
}

/*--------------------------------------------------------------------------------*/
void Susy3LepAna::fillHistograms()
{

  for(uint iSR=0; iSR<ML_NSR; iSR++){
    if(iSR==0 && !isDGML_SR1a)   continue;
    if(iSR==1 && !isDGML_SR1b)   continue;
    if(iSR==2 && !isDGML_SR2)    continue;
    if(iSR==3 && !isML_4Lep)     continue;
    if(iSR==4 && !isML_4LepnoZ)  continue;
    //    if(iSR==5 && !isML_stgProd) continue;
    //    if(iSR==6 && !isML_UED)     continue;
    
    _hh->H1FILL(_hh->ML_nLep[iSR], v_sigLep->size(), _ww); 
    _hh->H1FILL(_hh->ML_evtCatgUnOrdered[iSR], evtCatgUnOrd(v_sigLep), _ww);
    _hh->H1FILL(_hh->ML_evtCatgOSpair[iSR], evtCatgOrd(v_sigLep), _ww);

    bool sfos = hasSFOS(*v_sigLep);
    TLorentzVector _3l;
    TLorentzVector _4l;
    for(uint ilep=0; ilep<v_sigLep->size(); ilep++){
      const Susy::Lepton* _l = v_sigLep->at(ilep);
      if(ilep>=4) continue;
      if(ilep<3) _3l += (*_l);
      if(ilep<4) _4l += (*_l);
      if(ilep==0){
	_hh->H1FILL(_hh->ML_ptl1[iSR],_l->Pt(),_ww); 
	_hh->H1FILL(_hh->ML_etal1[iSR],_l->Eta(),_ww); 
	_hh->H1FILL(_hh->ML_d0Sl1[iSR],_l->d0/_l->errD0,_ww); 
	_hh->H1FILL(_hh->ML_z0sinthetal1[iSR],_l->z0*sin(_l->Theta()),_ww); 
      }
      else if(ilep==1){
	_hh->H1FILL(_hh->ML_ptl2[iSR],_l->Pt(),_ww); 
	_hh->H1FILL(_hh->ML_etal2[iSR],_l->Eta(),_ww); 
	_hh->H1FILL(_hh->ML_d0Sl2[iSR],_l->d0/_l->errD0,_ww); 
	_hh->H1FILL(_hh->ML_z0sinthetal2[iSR],_l->z0*sin(_l->Theta()),_ww); 
      }
      else if(ilep==2){
	_hh->H1FILL(_hh->ML_ptl3[iSR],_l->Pt(),_ww); 
	_hh->H1FILL(_hh->ML_etal3[iSR],_l->Eta(),_ww); 
	_hh->H1FILL(_hh->ML_d0Sl3[iSR],_l->d0/_l->errD0,_ww); 
	_hh->H1FILL(_hh->ML_z0sinthetal3[iSR],_l->z0*sin(_l->Theta()),_ww); 
      }
      else if(ilep==3){
	_hh->H1FILL(_hh->ML_ptl4[iSR],_l->Pt(),_ww); 
	_hh->H1FILL(_hh->ML_etal4[iSR],_l->Eta(),_ww); 
	_hh->H1FILL(_hh->ML_d0Sl4[iSR],_l->d0/_l->errD0,_ww); 
	_hh->H1FILL(_hh->ML_z0sinthetal4[iSR],_l->z0*sin(_l->Theta()),_ww); 
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
      bestZ(zl1, zl2, *v_sigLep);
      for(uint iL=0; iL<v_sigLep->size(); iL++)	{
	if(iL!=zl1 && iL!=zl2)  mT= Mt(v_sigLep->at(iL),m_met);
      }
      _hh->H1FILL(_hh->ML_SFOSMT[iSR],mT,_ww); 
    }
    
    //Dilepton mass
    for(uint iL1=0; iL1<v_sigLep->size(); iL1++)
      for(uint iL2=iL1+1; iL2<v_sigLep->size(); iL2++){
	float mll = Mll(v_sigLep->at(iL1),v_sigLep->at(iL2));
	_hh->H1FILL(_hh->ML_AllMll[iSR],mll,_ww); 
	if(isSFOS(v_sigLep->at(iL1),v_sigLep->at(iL2)))
	  _hh->H1FILL(_hh->ML_SFOSMll[iSR],mll,_ww); 
      }
    
    int nBJets=0;
    _hh->H1FILL(_hh->ML_nJets[iSR],v_sigJet->size(),_ww); 
    for(uint ijet=0; ijet<v_sigJet->size(); ijet++){
      const Susy::Jet* _j = v_sigJet->at(ijet);
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
    
  }//loop SR's

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
int Susy3LepAna::evtCatgOrd(const LeptonVector* leptons){
  int evtCatgOrd=99;
  if(leptons->size()<3) return evtCatgOrd;
  
  int lType[3];
  int qProdLeadPair=1;
  for(uint iL=0; iL<leptons->size(); iL++){
    lType[iL] = (leptons->at(iL)->isEle()) ? 0 : 1;
    if(iL<2) qProdLeadPair = leptons->at(iL)->q;
  }

  if(qProdLeadPair>0) return evtCatgOrd=99; //SS

  if(lType[0]==0 && lType[1]==0){ //eeX
    if(lType[2]==0) evtCatgOrd=0;
    if(lType[2]==1) evtCatgOrd=1;
  }
  else if(lType[0]==1 && lType[1]==1){//mmX
    if(lType[2]==0) evtCatgOrd=2;
    if(lType[2]==1) evtCatgOrd=3;
  }
  else if((lType[0]==0 && lType[1]==1) || 
	  (lType[1]==1 && lType[1]==0)){
    if(lType[2]==0) evtCatgOrd=4;
    if(lType[2]==1) evtCatgOrd=5;
  }
  
  return evtCatgOrd=99;
}
