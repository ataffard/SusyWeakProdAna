#include <iomanip>
#include "SusyWeakProdAna/SusyFakeAna.h"
#include "SusyWeakProdAna/SusyAnaCommon.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// SusyFakeAna Constructor
/*--------------------------------------------------------------------------------*/
SusyFakeAna::SusyFakeAna(SusyHistos* _histos):
  _hh(_histos)
{
  _random = new TRandom3(2055704344);
}

/*--------------------------------------------------------------------------------*/
// Hook the containers
/*--------------------------------------------------------------------------------*/
void SusyFakeAna::hookContainers(Susy::SusyNtObject* _ntPtr,
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
void SusyFakeAna::doAnalysis()
{
  reset();
  if( !eventCleaning() ) return;
  setEventWeight();
  _wwBck = _ww;

  getCRInfo(v_baseLep, v_sigJet, m_met);

  doMuonAnalysis();
  doElectronAnalysis();

  return;
}

/*--------------------------------------------------------------------------------*/
// ID CR's for 2L and 3L to measure FR & EFF  
/*--------------------------------------------------------------------------------*/
void SusyFakeAna::getCRInfo(const LeptonVector* leptons, const JetVector* jets, const Met *met)
{
  //2 Lepton CR's
  if(leptons->size()==2){
    isDG2L_CR0=true;
    if(!hasZ(*leptons)){
      if(jets->size()<2) isDG2L_CR1=true; //jet veto kills stat
      else if(jets->size()>=2){
	isDG2L_CR2=true;
	if(!hasBJet(*jets, MV1_85)) isDG2L_CR3=true;
      }
    }
  }
  
  //3 Lepton CR's
  else if(leptons->size()==3){
    isML_CR0=true;
    if(hasSFOS(*leptons) && hasZ(*leptons)) isML_CR1=true;
    if(hasSFOS(*leptons) && !hasZ(*leptons)){
      isML_CR2=true;
      if(!hasBJet(*jets, MV1_85)) isML_CR3=true;
    }
    if(nJetPtCut(*jets,50)>=2) isML_CR4=true;
    if(nJetPtCut(*jets,30)>=2 && lowestMll(*leptons)<50 ) isML_CR5=true;
   }

}

/*--------------------------------------------------------------------------------*/
// Muon real/fake analysis
/*--------------------------------------------------------------------------------*/
void SusyFakeAna::doMuonAnalysis()
{
  static const int _mode=1;
  const Lepton* _tag   = NULL;
  const Lepton* _probe = NULL;
  int   _catg   = 0;
  int   _method = 0;
  
  _ww = _wwBck; //restore weight before btag weight

  //Data driven selection
  ZTagProbe(_tag, _probe, _catg, _mode);
  if(_catg == 0){//Z not found - Try HF/LF tag-probe
    bool hasHF = HFTagProbe(_tag, _probe, _catg, _mode);
    if(hasHF){
      _ww *= bTagSF(nt->evt(),*v_sigJet);
      fillMuonHisto(_probe,HF,_method); 
    }
    else{
      bool hasLF = LFTagProbe(_tag, _probe, _catg, _mode);
      if(hasLF) fillMuonHisto(_probe,LF,_method); 
    }
  }
  else{
    float mll = Mll(_tag,_probe);
    _hh->H1FILL(_hh->Mmm_os,mll,_ww);
    fillMuonHisto(_probe,PR,_method,_tag);
  }

  //MC - do Fill histo using Truth matching
  if(nt->evt()->isMC){
    _method=1;
    for(uint i=0; i< v_baseMu->size(); i++){
      LEP_TYPE lType = getType(v_baseMu->at(i)->mcOrigin,
			       v_baseMu->at(i)->mcType,
			       _hh->sampleName(),
			       nt->evt()->mcChannel,
			       v_baseMu->at(i)->truthMatchType,
			       false,
			       false);
      //cout << "sample " <<_hh->sampleName() << " type " << lType << endl;
      //      TString ss = _hh->sampleName(); 
      //      if(ss.Contains("simplifiedModel")) lType=PR; //Simplified model - muon categorized at unknown 
      fillMuonHisto(v_baseMu->at(i),lType,_method);
    }
  }

}
/*--------------------------------------------------------------------------------*/
// Electron real/fake analysis
/*--------------------------------------------------------------------------------*/
void SusyFakeAna::doElectronAnalysis()
{
  static const int _mode=0;
  const Lepton* _tag   = NULL;
  const Lepton* _probe = NULL;
  int   _catg   = 0;
  int   _method = 0;

  //Data driven selection
  ZTagProbe(_tag, _probe, _catg, _mode);
  if(_catg == 0){//Z not found - Try HF/LF tag-probe
    bool hasHF = HFTagProbe(_tag, _probe, _catg, _mode);
    if(hasHF){
      _ww *= bTagSF(nt->evt(),*v_sigJet);
      fillElectronHisto(_probe,HF,_method); 
    }
    else{
      bool hasLF = LFTagProbe(_tag, _probe, _catg, _mode);
      if(hasLF) fillElectronHisto(_probe,LF,_method); 
    }
  }
  else{
    float mll = Mll(_tag,_probe);
    _hh->H1FILL(_hh->Mee_os,mll,_ww);
    fillElectronHisto(_probe,PR,_method,_tag);
  }

  //MC - do Fill histo using Truth matching
  if(nt->evt()->isMC){
    _method=1;
    for(uint i=0; i< v_baseEle->size(); i++){
      LEP_TYPE lType = getType(v_baseEle->at(i)->mcOrigin,
			       v_baseEle->at(i)->mcType,
			       _hh->sampleName(),
			       nt->evt()->mcChannel,
			       v_baseEle->at(i)->truthMatchType,
			       v_baseEle->at(i)->isEle(),
			       v_baseEle->at(i)->isChargeFlip);
      fillElectronHisto(v_baseEle->at(i),lType,_method);
    }
  }
}
/*--------------------------------------------------------------------------------*/
// Reset all vars
/*--------------------------------------------------------------------------------*/
void SusyFakeAna::reset()
{
  isDG2L_CR0=false;
  isDG2L_CR1=false;
  isDG2L_CR2=false;
  isDG2L_CR3=false;
  isML_CR0=false;
  isML_CR1=false;
  isML_CR2=false;
  isML_CR3=false;
  isML_CR4=false;
  isML_CR5=false;
}
/*--------------------------------------------------------------------------------*/
// The Terminate() function is the last function to be called
/*--------------------------------------------------------------------------------*/
void SusyFakeAna::end()
{
 
}
/*--------------------------------------------------------------------------------*/
// Event weight
/*--------------------------------------------------------------------------------*/
void SusyFakeAna::setEventWeight(int mode)
{
  _ww=1;
  if(mode==NOLUMI) _ww= 1; //raw weight
  else if(mode==LUMI1FB){
    _ww=getEventWeightAB3(nt->evt());
  }
  else if(mode==LUMI5FB){
    _ww=getEventWeightAB(nt->evt());
  }
  else if(mode==LUMI13FB){
    _ww=getEventWeight(nt->evt());
  }
  
}

/*--------------------------------------------------------------------------------*/
// Event cleaning
/*--------------------------------------------------------------------------------*/
bool SusyFakeAna::eventCleaning()
{
  //  int flag = nt->evt()->evtFlag[NtSys_NOM];
  int flag = nt->evt()->cutFlags[NtSys_NOM];
  if( !passLAr(flag) )              return false;
  if( !passBadJet(flag) )           return false;
  if( !passBadMuon(flag) )          return false;
  if( !passCosmic(flag) )           return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
//FillMuonHisto
/*--------------------------------------------------------------------------------*/
void SusyFakeAna::fillMuonHisto(const Lepton* _mProbe, LEP_TYPE t, int m, const Lepton* _mTag)
{
  static const bool useUnbiased=true;
  if(m==1 && t==4) return;//trm but unknown lepton

  const Muon* _m = (const Muon*) _mProbe;

  const Jet* _closestJet=NULL;
  float mindR=999;
  float mindPhiJMet=999;
  for(uint j=0; j<v_baseJet->size(); j++){
    const Jet* _j = v_baseJet->at(j);
    if(! isCentralLightJet(_j)) continue;
    if(! isCentralBJet(_j)) continue;
    if(! isForwardJet(_j)) continue;

    float dPhi = fabs(TVector2::Phi_mpi_pi(_j->Phi()-m_met->lv().Phi()))*TMath::RadToDeg();
    if(dPhi<mindPhiJMet) mindPhiJMet=dPhi;
    if(_m->DeltaR(*_j)>mindR) continue;
    mindR = _m->DeltaR(*_j);
    _closestJet = _j;
  }

  float _metRel = getMetRel(m_met,*v_baseLep,*v_baseJet);
  float dPhilmet = fabs(TVector2::Phi_mpi_pi(_m->Phi()-m_met->lv().Phi()))*TMath::RadToDeg();
  
  float d0=_m->d0;
  float d0Err=_m->errD0;
  float d0Sig=_m->d0Sig();
  float z0SinTheta = _m->z0SinTheta();
  if(useUnbiased){
   d0=_m->d0Unbiased;
   d0Err=_m->errD0Unbiased;
   d0Sig=_m->d0Unbiased/_m->errD0Unbiased;
   z0SinTheta =_m->z0Unbiased*sin(_m->Theta());
   //z0SinTheta =_m->z0Unbiased*sin(_m->thetaPV);
  }

  if(nt->evt()->isMC){
    LEP_TYPE lType = getType(_m->mcOrigin,
			     _m->mcType,
			     _hh->sampleName(),
			     nt->evt()->mcChannel,
			     _m->truthMatchType,
			     false,
			     false);
    _hh->H1FILL(_hh->m_type[m][t],lType,_ww);
    _hh->H1FILL(_hh->m_org[m][t],_m->mcOrigin,_ww);
  }
  _hh->H1FILL(_hh->m_pt[m][t],_m->Pt(),_ww);
  _hh->H1FILL(_hh->m_eta[m][t],_m->Eta(),_ww);
  _hh->H1FILL(_hh->m_d0[m][t],d0,_ww);
  _hh->H1FILL(_hh->m_d0S[m][t],d0Sig,_ww);
  _hh->H1FILL(_hh->m_z0sintheta[m][t],z0SinTheta,_ww);
  _hh->H1FILL(_hh->m_dR_jet[m][t],mindR,_ww);
  
  //Histo to closest jet dR<0.6
  if(_closestJet && mindR<0.6){
    float ptrel = ptRel(*_closestJet, *_m);
    float sd0 = signedD0(d0,d0Err, *_m, *_closestJet);
    _hh->H1FILL(_hh->m_cjet_pt[m][t],_closestJet->Pt(),_ww);
    //    _hh->H1FILL(_hh->m_cjet_nTrk[m],[t],_closestJet->nTrk,_ww);
    _hh->H1FILL(_hh->m_ptrel[m][t],ptrel,_ww);
    _hh->H1FILL(_hh->m_signd0[m][t],sd0,_ww);

    if(isBJet(_closestJet, MV1_85)) 
      _hh->H1FILL(_hh->m_cjet_isBTag[m][t],1,_ww);
    else
      _hh->H1FILL(_hh->m_cjet_isBTag[m][t],0,_ww);
    
    if(_closestJet->truthLabel==5){
      _hh->H1FILL(_hh->m_cjet_bj_pt[m][t],_closestJet->Pt(),_ww);
      _hh->H1FILL(_hh->m_cjet_idB[m][t],1,_ww);
      //if(_cJet->getI("idxB")>-99){
      //McTruth* _mc = &(rdNt->_mcA)[_cJet->getI("idxB")];
      //H1s_Fill(m_cjet_b_pt,method,type,_mc->Pt(),_ww);
      //}
    }
    else _hh->H1FILL(_hh->m_cjet_idB[m][t],0,_ww);
  }

  bool passNI1=true;
  bool passIP=true;
  bool passNI2=true;
  
  float PtCone20 = _m->ptcone20;
  float PtCone30 = _m->ptcone30;
  float EtCone30 = _m->etcone30;
  float PtIso30  = muPtConeCorr(_m, nt->evt()->nVtx, nt->evt()->isMC);
  float EtIso30  = muEtConeCorr(_m, nt->evt()->nVtx, nt->evt()->isMC);
  
  if(PtIso30/_m->Pt()>MUON_PTCONE30_PT_CUT) passNI1=false;
  if(EtIso30/_m->Pt()>MUON_ETCONE30_PT_CUT) passNI2=false;
  if(fabs(d0Sig)>MUON_D0SIG_CUT) passIP=false;
  if(fabs(z0SinTheta)>MUON_Z0_SINTHETA_CUT) passIP=false;
  
  //Isolation
  _hh->H1FILL(_hh->m_ptCone20[m][t],PtCone20,_ww);
  _hh->H1FILL(_hh->m_ptCone30[m][t],PtCone30,_ww);
  _hh->H1FILL(_hh->m_etCone30[m][t],EtCone30,_ww);
  _hh->H1FILL(_hh->m_ptCone20Rel[m][t],PtCone20/_m->Pt(),_ww);
  _hh->H1FILL(_hh->m_ptCone30Rel[m][t],PtCone30/_m->Pt(),_ww);
  _hh->H1FILL(_hh->m_etCone30Rel[m][t],EtCone30/_m->Pt(),_ww);

  if(passIP){
    _hh->H2FILL(_hh->m_ptCone30Pt[m][t],_m->Pt(),PtIso30,_ww);
    _hh->H2FILL(_hh->m_ptCone30RelPt[m][t],_m->Pt(),PtIso30/_m->Pt(),_ww);
    _hh->H2FILL(_hh->m_etCone30Pt[m][t],_m->Pt(),EtIso30,_ww);
    _hh->H2FILL(_hh->m_etCone30RelPt[m][t],_m->Pt(),EtIso30/_m->Pt(),_ww);
  }


  //Pick Iso --- make some plots 
  _hh->PFILL(_hh->m_iso_mu[m][t],nt->evt()->avgMu,PtIso30,_ww);
  _hh->PFILL(_hh->m_iso_npv[m][t],nt->evt()->nVtx,PtIso30,_ww);
  
  //Isolation  -vs- pt - vs Npv/<mu>
  _hh->H3FILL(_hh->m_ptCone20_npv[m][t],PtCone20,_m->Pt(),nt->evt()->nVtx,_ww);
  _hh->H3FILL(_hh->m_ptCone30_npv[m][t],PtCone30,_m->Pt(),nt->evt()->nVtx,_ww);
  _hh->H3FILL(_hh->m_etCone30_npv[m][t],EtCone30,_m->Pt(),nt->evt()->nVtx,_ww);
  _hh->H3FILL(_hh->m_ptCone20Rel_npv[m][t],PtCone20/_m->Pt(),_m->Pt(),nt->evt()->nVtx,_ww);
  _hh->H3FILL(_hh->m_ptCone30Rel_npv[m][t],PtCone30/_m->Pt(),_m->Pt(),nt->evt()->nVtx,_ww);
  _hh->H3FILL(_hh->m_etCone30Rel_npv[m][t],EtCone30/_m->Pt(),_m->Pt(),nt->evt()->nVtx,_ww);

  _hh->H3FILL(_hh->m_ptCone20_mu[m][t],PtCone20,_m->Pt(),nt->evt()->avgMu,_ww);
  _hh->H3FILL(_hh->m_ptCone30_mu[m][t],PtCone30,_m->Pt(),nt->evt()->avgMu,_ww);
  _hh->H3FILL(_hh->m_etCone30_mu[m][t],EtCone30,_m->Pt(),nt->evt()->avgMu,_ww);
  _hh->H3FILL(_hh->m_ptCone20Rel_mu[m][t],PtCone20/_m->Pt(),_m->Pt(),nt->evt()->avgMu,_ww);
  _hh->H3FILL(_hh->m_ptCone30Rel_mu[m][t],PtCone30/_m->Pt(),_m->Pt(),nt->evt()->avgMu,_ww);
  _hh->H3FILL(_hh->m_etCone30Rel_mu[m][t],EtCone30/_m->Pt(),_m->Pt(),nt->evt()->avgMu,_ww);
  
  //Fill histo to compute real_eff & fake
  for(uint k=0; k<nCR; k++){
    int iCR=999;
    if(k==0) iCR=0;
    if(k==1 && isDG2L_CR0) iCR=k;
    if(k==2 && isDG2L_CR1) iCR=k;
    if(k==3 && isDG2L_CR2) iCR=k;
    if(k==4 && isDG2L_CR3) iCR=k;
    if(k==5 && isML_CR0)   iCR=k;
    if(k==6 && isML_CR1)   iCR=k;
    if(k==7 && isML_CR2)   iCR=k;
    if(k==8 && isML_CR3)   iCR=k;
    if(k==9 && isML_CR4)   iCR=k;
    if(k==10 && isML_CR5)  iCR=k;
    if(iCR==999) continue;
    
    //Denominator
    _hh->H1FILL(_hh->m_loose_pt[m][t][iCR],_m->Pt(),_ww);  
    _hh->H1FILL(_hh->m_loose_eta[m][t][iCR],_m->Eta(),_ww);
    _hh->H1FILL(_hh->m_loose_d0S[m][t][iCR],d0Sig,_ww); 
    _hh->H1FILL(_hh->m_loose_z0sintheta[m][t][iCR],z0SinTheta,_ww);   
    _hh->H1FILL(_hh->m_loose_met[m][t][iCR],m_met->lv().Pt(),_ww);  
    _hh->H1FILL(_hh->m_loose_metrel[m][t][iCR],_metRel,_ww); 
    _hh->H1FILL(_hh->m_loose_dPhilmet[m][t][iCR],dPhilmet,_ww);  
    _hh->H1FILL(_hh->m_loose_dPhijmet[m][t][iCR],mindPhiJMet,_ww);  
    _hh->H1FILL(_hh->m_loose_nJets[m][t][iCR],v_sigJet->size(),_ww); 
    
    //Numerator default isolation 2011
    if(passMuIso2011(_m)){
      _hh->H1FILL(_hh->m_tight_pt[m][t][iCR],_m->Pt(),_ww);  
      _hh->H1FILL(_hh->m_tight_eta[m][t][iCR],_m->Eta(),_ww);
      _hh->H1FILL(_hh->m_tight_d0S[m][t][iCR],d0Sig,_ww); 
      _hh->H1FILL(_hh->m_tight_z0sintheta[m][t][iCR],z0SinTheta,_ww);   
      _hh->H1FILL(_hh->m_tight_met[m][t][iCR],m_met->lv().Pt(),_ww); 
      _hh->H1FILL(_hh->m_tight_metrel[m][t][iCR],_metRel,_ww); 
      _hh->H1FILL(_hh->m_tight_dPhilmet[m][t][iCR],dPhilmet,_ww);  
      _hh->H1FILL(_hh->m_tight_dPhijmet[m][t][iCR],mindPhiJMet,_ww);   
      _hh->H1FILL(_hh->m_tight_nJets[m][t][iCR],v_sigJet->size(),_ww); 
    }
    
    if(passNI1){ //ptCone30Corr
      _hh->H1FILL(_hh->m_tightNI_pt[m][t][iCR],_m->Pt(),_ww);  
      _hh->H1FILL(_hh->m_tightNI_eta[m][t][iCR],_m->Eta(),_ww);
      _hh->H1FILL(_hh->m_tightNI_d0S[m][t][iCR],d0Sig,_ww); 
      _hh->H1FILL(_hh->m_tightNI_z0sintheta[m][t][iCR],z0SinTheta,_ww);   
      _hh->H1FILL(_hh->m_tightNI_met[m][t][iCR],m_met->lv().Pt(),_ww); 
      if(passIP){ //IP
	_hh->H1FILL(_hh->m_tightNIIP_pt[m][t][iCR],_m->Pt(),_ww);  
	_hh->H1FILL(_hh->m_tightNIIP_eta[m][t][iCR],_m->Eta(),_ww);
	_hh->H1FILL(_hh->m_tightNIIP_d0S[m][t][iCR],d0Sig,_ww); 
	_hh->H1FILL(_hh->m_tightNIIP_z0sintheta[m][t][iCR],z0SinTheta,_ww);   
	_hh->H1FILL(_hh->m_tightNIIP_met[m][t][iCR],m_met->lv().Pt(),_ww); 
	if(passNI2){//ptCone + IP + Etcone
	  _hh->H1FILL(_hh->m_tightNI2_pt[m][t][iCR],_m->Pt(),_ww);  
	  _hh->H1FILL(_hh->m_tightNI2_eta[m][t][iCR],_m->Eta(),_ww);
	  _hh->H1FILL(_hh->m_tightNI2_d0S[m][t][iCR],d0Sig,_ww); 
	  _hh->H1FILL(_hh->m_tightNI2_z0sintheta[m][t][iCR],z0SinTheta,_ww);   
	  _hh->H1FILL(_hh->m_tightNI2_met[m][t][iCR],m_met->lv().Pt(),_ww); 
	}
      }
    }
  }//CR's
  
}
/*--------------------------------------------------------------------------------*/
//FillElectronHisto
/*--------------------------------------------------------------------------------*/
void SusyFakeAna::fillElectronHisto(const Lepton* _eProbe, LEP_TYPE t, int m, const Lepton* _eTag)
{
  static const bool useUnbiased=true;
  if(m==1 && t==4) return;//trm but unknown lepton
  
  const Electron* _e = (const Electron*) _eProbe;

  const Jet* _closestJet=NULL;
  float mindR=999;
  float mindPhiJMet=999;
  for(uint j=0; j<v_baseJet->size(); j++){
    const Jet* _j = v_baseJet->at(j);
    if(_j->jvf<JET_JVF_CUT) continue;
    //    if(_j->Pt()<JET_PT_CUT_3L) continue;//pt20
    if(fabs(_j->Eta())>JET_ETA_CUT) continue;
    float dPhi = fabs(TVector2::Phi_mpi_pi(_j->Phi()-m_met->lv().Phi()))*TMath::RadToDeg();
    if(dPhi<mindPhiJMet) mindPhiJMet=dPhi;
    if(_e->DeltaR(*_j)>mindR) continue;
    mindR = _e->DeltaR(*_j);
    _closestJet = _j;
  }

  float _metRel = getMetRel(m_met,*v_baseLep,*v_baseJet);
  float dPhilmet = fabs(TVector2::Phi_mpi_pi(_e->Phi()-m_met->lv().Phi()))*TMath::RadToDeg();
  
  float d0=_e->d0;
  float d0Err=_e->errD0;
  float d0Sig=_e->d0Sig();
  float z0SinTheta = _e->z0SinTheta();
  if(useUnbiased){
   d0=_e->d0Unbiased;
   d0Err=_e->errD0Unbiased;
   d0Sig=_e->d0Unbiased/_e->errD0Unbiased;
   z0SinTheta =_e->z0Unbiased*sin(_e->Theta());
  }

  if(nt->evt()->isMC){
    LEP_TYPE lType = getType(_e->mcOrigin,
			     _e->mcType,
			     _hh->sampleName(),
			     nt->evt()->mcChannel,
			     _e->truthMatchType,
			     _e->isEle(),
			     _e->isChargeFlip);
    _hh->H1FILL(_hh->e_type[m][t],lType,_ww);
    _hh->H1FILL(_hh->e_org[m][t],_e->mcOrigin,_ww);
  }
  _hh->H1FILL(_hh->e_pt[m][t],_e->Pt(),_ww);
  _hh->H1FILL(_hh->e_eta[m][t],_e->Eta(),_ww);
  _hh->H1FILL(_hh->e_d0[m][t],d0,_ww);
  _hh->H1FILL(_hh->e_d0S[m][t],d0Sig,_ww);
  _hh->H1FILL(_hh->e_z0sintheta[m][t],z0SinTheta,_ww);
  _hh->H1FILL(_hh->e_dR_jet[m][t],mindR,_ww);

  if(_closestJet && mindR<0.6){
    float ptrel = ptRel(*_closestJet, *_e);
    float sd0 = signedD0(d0,d0Err, *_e, *_closestJet);
    _hh->H1FILL(_hh->e_cjet_pt[m][t],_closestJet->Pt(),_ww);
    //    _hh->H1FILL(_hh->e_cjet_nTrk[m],[t],_closestJet->nTrk,_ww);
    _hh->H1FILL(_hh->e_ptrel[m][t],ptrel,_ww);
    _hh->H1FILL(_hh->e_signd0[m][t],sd0,_ww);

    if(isBJet(_closestJet, MV1_85)) 
      _hh->H1FILL(_hh->e_cjet_isBTag[m][t],1,_ww);
    else
      _hh->H1FILL(_hh->e_cjet_isBTag[m][t],0,_ww);
    
    if(_closestJet->truthLabel==5){
      _hh->H1FILL(_hh->e_cjet_bj_pt[m][t],_closestJet->Pt(),_ww);
      _hh->H1FILL(_hh->e_cjet_idB[m][t],1,_ww);
      //if(_cJet->getI("idxB")>-99){
      //McTruth* _mc = &(rdNt->_mcA)[_cJet->getI("idxB")];
      //H1s_Fill(e_cjet_b_pt,method,type,_mc->Pt(),_ww);
      //}
    }
    else _hh->H1FILL(_hh->e_cjet_idB[m][t],0,_ww);
  }
  
  bool passNI1=true;
  bool passIP=true;
  bool passNI2=true;

  float PtIso20 = _e->ptcone20;
  float PtIso30 = _e->ptcone30;
  float EtIso30 = elEtTopoConeCorr(_e, nt->evt()->nVtx, nt->evt()->isMC);
  float EtConeTopo30 = _e->topoEtcone30Corr;

  if(PtIso30/_e->Pt()>ELECTRON_PTCONE30_PT_CUT) passNI1=false;
  if(EtIso30/_e->Pt()>ELECTRON_TOPOCONE30_PT_CUT) passNI2=false;
  if(fabs(d0Sig)>ELECTRON_D0SIG_CUT) passIP=false;
  if(fabs(z0SinTheta)>ELECTRON_Z0_SINTHETA_CUT) passIP=false;

   //Isolation
  _hh->H1FILL(_hh->e_ptCone20[m][t],PtIso20,_ww);
  _hh->H1FILL(_hh->e_ptCone30[m][t],PtIso30,_ww);
  _hh->H1FILL(_hh->e_etConeTopoCorr30[m][t],EtConeTopo30,_ww);
  _hh->H1FILL(_hh->e_ptCone20Rel[m][t],PtIso20/_e->Pt(),_ww);
  _hh->H1FILL(_hh->e_ptCone30Rel[m][t],PtIso30/_e->Pt(),_ww);
  _hh->H1FILL(_hh->e_etConeTopoCorr30Rel[m][t],EtConeTopo30/_e->Pt(),_ww);

  if(_e->tightPP && passIP){
    _hh->H2FILL(_hh->e_ptCone30Pt[m][t],_e->Pt(),PtIso30,_ww);
    _hh->H2FILL(_hh->e_ptCone30RelPt[m][t],_e->Pt(),PtIso30/_e->Pt(),_ww);
    _hh->H2FILL(_hh->e_etCone30Pt[m][t],_e->Pt(),EtIso30,_ww);
    _hh->H2FILL(_hh->e_etCone30RelPt[m][t],_e->Pt(),EtIso30/_e->Pt(),_ww);
  }

  //Pick Iso --- make some plots 
  _hh->PFILL(_hh->e_iso_mu[m][t],nt->evt()->avgMu,PtIso30,_ww);
  //std::cout <<"Muon fill " << m << " " << t << " org " << _e->mcOrigin << " nvtx " << nt->evt()->nVtx << " iso " << PtIso30 <<endl;
  _hh->PFILL(_hh->e_iso_npv[m][t],nt->evt()->nVtx,PtIso30,_ww);

  //Isolation  -vs- pt - vs Npv/<mu>
  _hh->H3FILL(_hh->e_ptCone20_npv[m][t],PtIso20,_e->Pt(),nt->evt()->nVtx,_ww);
  _hh->H3FILL(_hh->e_ptCone30_npv[m][t],PtIso30,_e->Pt(),nt->evt()->nVtx,_ww);
  _hh->H3FILL(_hh->e_etConeTopoCorr30_npv[m][t],EtConeTopo30,_e->Pt(),nt->evt()->nVtx,_ww);
  _hh->H3FILL(_hh->e_ptCone20Rel_npv[m][t],PtIso20/_e->Pt(),_e->Pt(),nt->evt()->nVtx,_ww);
  _hh->H3FILL(_hh->e_ptCone30Rel_npv[m][t],PtIso30/_e->Pt(),_e->Pt(),nt->evt()->nVtx,_ww);
  _hh->H3FILL(_hh->e_etConeTopoCorr30Rel_npv[m][t],EtConeTopo30/_e->Pt(),_e->Pt(),nt->evt()->nVtx,_ww);

  _hh->H3FILL(_hh->e_ptCone20_mu[m][t],PtIso20,_e->Pt(),nt->evt()->avgMu,_ww);
  _hh->H3FILL(_hh->e_ptCone30_mu[m][t],PtIso30,_e->Pt(),nt->evt()->avgMu,_ww);
  _hh->H3FILL(_hh->e_etConeTopoCorr30_mu[m][t],EtConeTopo30,_e->Pt(),nt->evt()->avgMu,_ww);
  _hh->H3FILL(_hh->e_ptCone20Rel_mu[m][t],PtIso20/_e->Pt(),_e->Pt(),nt->evt()->avgMu,_ww);
  _hh->H3FILL(_hh->e_ptCone30Rel_mu[m][t],PtIso30/_e->Pt(),_e->Pt(),nt->evt()->avgMu,_ww);
  _hh->H3FILL(_hh->e_etConeTopoCorr30Rel_mu[m][t],EtConeTopo30/_e->Pt(),_e->Pt(),nt->evt()->avgMu,_ww);

  //Fill histo to compute real_eff & fake
  for(uint k=0; k<nCR; k++){
    int iCR=999;
    if(k==0) iCR=0;
    if(k==1 && isDG2L_CR0) iCR=k;
    if(k==2 && isDG2L_CR1) iCR=k;
    if(k==3 && isDG2L_CR2) iCR=k;
    if(k==4 && isDG2L_CR3) iCR=k;
    if(k==5 && isML_CR0)   iCR=k;
    if(k==6 && isML_CR1)   iCR=k;
    if(k==7 && isML_CR2)   iCR=k;
    if(k==8 && isML_CR3)   iCR=k;
    if(k==9 && isML_CR4)   iCR=k;
    if(k==10 && isML_CR5)  iCR=k;
    if(iCR==999) continue;

    //Denominator
   _hh->H1FILL(_hh->e_loose_pt[m][t][iCR],_e->Pt(),_ww);  
   _hh->H1FILL(_hh->e_loose_eta[m][t][iCR],_e->Eta(),_ww);
   _hh->H1FILL(_hh->e_loose_d0S[m][t][iCR],d0Sig,_ww); 
   _hh->H1FILL(_hh->e_loose_z0sintheta[m][t][iCR],z0SinTheta,_ww);   
   _hh->H1FILL(_hh->e_loose_met[m][t][iCR],m_met->lv().Pt(),_ww);  
   _hh->H1FILL(_hh->e_loose_metrel[m][t][iCR],_metRel,_ww); 
   _hh->H1FILL(_hh->e_loose_dPhilmet[m][t][iCR],dPhilmet,_ww);  
   _hh->H1FILL(_hh->e_loose_dPhijmet[m][t][iCR],mindPhiJMet,_ww);  
   _hh->H1FILL(_hh->e_loose_nJets[m][t][iCR],v_sigJet->size(),_ww); 

   if(_e->tightPP){
     _hh->H1FILL(_hh->e_tightPP_pt[m][t][iCR],_e->Pt(),_ww);  
     _hh->H1FILL(_hh->e_tightPP_eta[m][t][iCR],_e->Eta(),_ww);
     _hh->H1FILL(_hh->e_tightPP_d0S[m][t][iCR],d0Sig,_ww); 
     _hh->H1FILL(_hh->e_tightPP_z0sintheta[m][t][iCR],z0SinTheta,_ww);   
     _hh->H1FILL(_hh->e_tightPP_met[m][t][iCR],m_met->lv().Pt(),_ww); 

     //Numerator default isolation 2011
     if(passEleIso2011(_e)){
       _hh->H1FILL(_hh->e_tight_pt[m][t][iCR],_e->Pt(),_ww);  
       _hh->H1FILL(_hh->e_tight_eta[m][t][iCR],_e->Eta(),_ww);
       _hh->H1FILL(_hh->e_tight_d0S[m][t][iCR],d0Sig,_ww); 
       _hh->H1FILL(_hh->e_tight_z0sintheta[m][t][iCR],z0SinTheta,_ww);   
       _hh->H1FILL(_hh->e_tight_met[m][t][iCR],m_met->lv().Pt(),_ww); 
       _hh->H1FILL(_hh->e_tight_metrel[m][t][iCR],_metRel,_ww); 
       _hh->H1FILL(_hh->e_tight_dPhilmet[m][t][iCR],dPhilmet,_ww);  
       _hh->H1FILL(_hh->e_tight_dPhijmet[m][t][iCR],mindPhiJMet,_ww);   
       _hh->H1FILL(_hh->e_tight_nJets[m][t][iCR],v_sigJet->size(),_ww); 
     }//2011 iso
     if(passNI1){ //ptCone30Corr
       _hh->H1FILL(_hh->e_tightNI_pt[m][t][iCR],_e->Pt(),_ww);  
       _hh->H1FILL(_hh->e_tightNI_eta[m][t][iCR],_e->Eta(),_ww);
       _hh->H1FILL(_hh->e_tightNI_d0S[m][t][iCR],d0Sig,_ww); 
       _hh->H1FILL(_hh->e_tightNI_z0sintheta[m][t][iCR],z0SinTheta,_ww);   
       _hh->H1FILL(_hh->e_tightNI_met[m][t][iCR],m_met->lv().Pt(),_ww); 
       if(passIP){ //IP
	 _hh->H1FILL(_hh->e_tightNIIP_pt[m][t][iCR],_e->Pt(),_ww);  
	 _hh->H1FILL(_hh->e_tightNIIP_eta[m][t][iCR],_e->Eta(),_ww);
	 _hh->H1FILL(_hh->e_tightNIIP_d0S[m][t][iCR],d0Sig,_ww); 
	 _hh->H1FILL(_hh->e_tightNIIP_z0sintheta[m][t][iCR],z0SinTheta,_ww);   
	 _hh->H1FILL(_hh->e_tightNIIP_met[m][t][iCR],m_met->lv().Pt(),_ww); 
	 if(passNI2){//ptCone + IP + Etcone
	   _hh->H1FILL(_hh->e_tightNI2_pt[m][t][iCR],_e->Pt(),_ww);  
	   _hh->H1FILL(_hh->e_tightNI2_eta[m][t][iCR],_e->Eta(),_ww);
	   _hh->H1FILL(_hh->e_tightNI2_d0S[m][t][iCR],d0Sig,_ww); 
	   _hh->H1FILL(_hh->e_tightNI2_z0sintheta[m][t][iCR],z0SinTheta,_ww);   
	   _hh->H1FILL(_hh->e_tightNI2_met[m][t][iCR],m_met->lv().Pt(),_ww); 
	 }
       }
     }//2012 iso
   }//tight PP
  }//CR's

  
}
/*--------------------------------------------------------------------------------*/
// Z tag & probe
// mode:0 electron, 1:muon
/*--------------------------------------------------------------------------------*/
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

      bool l1_isTight=isSignalLepton(_l1,nt->evt()->nVtx,nt->evt()->isMC);
      bool l2_isTight=isSignalLepton(_l2,nt->evt()->nVtx,nt->evt()->isMC);

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

/*--------------------------------------------------------------------------------*/
// Get Tight-Loose category
/*--------------------------------------------------------------------------------*/
TL_CATG SusyFakeAna::getTL(bool T1, bool T2)
{
  TL_CATG catg = XX_Undef;
  if     ( T1  && T2  ) catg = TT;
  else if( T1  && !T2 ) catg = TL;
  else if( !T1 && T2  ) catg = LT;
  else if( !T1 && !T2 ) catg = LL;

  return catg;
}
/*--------------------------------------------------------------------------------*/
// HF tag & probe
/*--------------------------------------------------------------------------------*/
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
    const Jet* _j = v_baseJet->at(j);
    if(_j->Pt()<JET_PT_B20_CUT) continue;
    if(fabs(_j->Eta())>JET_ETA_CUT) continue;
    if(!isBJet(_j,MV1_85)) continue;
    bool hasMu=false;
    for(uint i=0; i<muons.size(); i++){
      const Muon* _m = muons.at(i);
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

/*--------------------------------------------------------------------------------*/
// LF tag & probe
/*--------------------------------------------------------------------------------*/
bool SusyFakeAna::LFTagProbe(const Lepton* &_tag, const Lepton* &_probe, int &category, int mode)
{

  return false;
}
