#include <iomanip>
#include "TCanvas.h"
#include "SusyWeakProdAna/Susy2LepAna.h"
#include "SusyWeakProdAna/PhysicsTools.h"
#include "SusyWeakProdAna/SusyAnaCommon.h"

using namespace std;
using namespace Susy;

const char* const DIL_FLAV[] = {"EE", "MM", "EM"};
const char* const DIL_QQ[] = {"OS", "SS"};

/*--------------------------------------------------------------------------------*/
// Susy2LepAna Constructor
/*--------------------------------------------------------------------------------*/
Susy2LepAna::Susy2LepAna(SusyHistos* _histos):
  _hh(_histos),
  m_nLepMin(2),
  m_nLepMax(2),
  m_cutNBaseLep(true),
  m_ET(ET_Unknown)
{
  n_readin       = 0;
  n_pass_LAr     = 0;
  n_pass_BadJet  = 0;
  n_pass_BadMuon = 0;
  n_pass_Cosmic  = 0;

  // The rest are channel specific.
  for(int i=0; i<ET_N; ++i){
    n_pass_dil[i]  = 0;
    n_pass_nLep[i]    = 0;
    n_pass_trig[i]    = 0;
    n_pass_flavor[i]  = 0;
    n_pass_mll[i]     = 0;
    n_pass_ss[i]      = 0;
    n_pass_os[i]      = 0;
    
    // SR1
    n_pass_SR1jv[i]   = 0;
    n_pass_SR1Zv[i]   = 0;
    n_pass_SR1MET[i]  = 0;
    
    // SR2
    n_pass_SR2jv[i]   = 0;
    n_pass_SR2MET[i]  = 0;
    
    // SR3
    n_pass_SR3ge2j[i] = 0;
    n_pass_SR3Zv[i]   = 0;
    n_pass_SR3bjv[i]  = 0;
    n_pass_SR3mct[i]  = 0;
    n_pass_SR3MET[i]  = 0;
    
    // SR4
    n_pass_SR4jv[i]    = 0;
    n_pass_SR4Zv[i]    = 0;
    n_pass_SR4MET[i]   = 0;
    n_pass_SR4MT2[i]   = 0;

    // SR5
    n_pass_SR5jv[i]        = 0;
    n_pass_SR5MET[i]       = 0;
    n_pass_SR5Zv[i]        = 0;
    n_pass_SR5L0pt[i]      = 0;
    n_pass_SR5SUMpt[i]     = 0;
    n_pass_SR5dPhiMETLL[i] = 0;
    n_pass_SR5dPhiMETL1[i] = 0;
    
  }

  m_trigObj = new DilTrigLogic(false);

  m_ET = ET_Unknown;
}

/*--------------------------------------------------------------------------------*/
// Hook the containers
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::hookContainers(Susy::SusyNtObject* _ntPtr,
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
void Susy2LepAna::doAnalysis()
{
  reset();
  n_readin++;

  // Check Event
  // Cleaning, ==2 leptons, trigger, Mll
  if(!selectEvent(v_sigLep, v_baseLep)) return;

  // Count SS and OS
  if(sameSign(v_sigLep)){
    n_pass_ss[m_ET]++;
    DG2L_qprod=SS;
    isDG2L_SR0_SS=true;
  }
  
  if(oppositeSign(v_sigLep)){
    n_pass_os[m_ET]++;
    DG2L_qprod=OS;
    isDG2L_SR0_OS=true;
  }
  
  // Check Signal regions
  passSR1(v_sigLep, v_sigJet, m_met);
  passSR2(v_sigLep, v_sigJet, m_met);
  passSR3(v_sigLep, v_sigJet, m_met);
  passSR4(v_sigLep, v_sigJet, m_met);
  passSR5(v_sigLep, v_sigJet, m_met);

  fillHistograms();

  return;
}

/*--------------------------------------------------------------------------------*/
// End of job
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::end()
{
  cout << endl;
  cout << endl;
  cout << "**********************************************" << endl;
  cout << "Susy2LepAna::event counters" <<endl;

  cout << "read in:       " << n_readin       << endl;
  cout << "pass LAr:      " << n_pass_LAr     << endl;
  cout << "pass BadJet:   " << n_pass_BadJet  << endl;
  cout << "pass BadMu:    " << n_pass_BadMuon << endl;
  cout << "pass Cosmic:   " << n_pass_Cosmic  << endl;

  string v_ET[ET_N] = {"ee","mm","em","Unknown"};
  for(int i=0; i<ET_N; ++i){
    cout << "====================================" << endl;
    cout << "For dilepton type: " << v_ET[i]       << endl;
    
    cout << "pass category: " << n_pass_dil[i]  << endl;
    cout << "pass trig:     " << n_pass_trig[i]    << endl;
    cout << "pass SF:       " << n_pass_flavor[i]  << endl;
    cout << "pass nLep:     " << n_pass_nLep[i]    << endl;
    cout << "pass mll:      " << n_pass_mll[i]     << endl;
    cout << "pass OS:       " << n_pass_os[i]      << endl;
    cout << "pass SS:       " << n_pass_ss[i]      << endl;
    cout << "---------------------------------"    << endl;
    cout << "pass SR1 JV:   " << n_pass_SR1jv[i]   << endl;
    cout << "pass SR1 ZV:   " << n_pass_SR1Zv[i]   << endl;
    cout << "pass SR1 MET:  " << n_pass_SR1MET[i]  << endl;
    cout << "---------------------------------"    << endl;
    cout << "pass SR2 JV:   " << n_pass_SR2jv[i]   << endl;
    cout << "pass SR2 MET:  " << n_pass_SR2MET[i]  << endl;
    cout << "---------------------------------"    << endl;
    cout << "pass SR3 >=2j: " << n_pass_SR3ge2j[i] << endl;
    cout << "pass SR3 ZV:   " << n_pass_SR3Zv[i]   << endl;
    cout << "pass SR3 bV:   " << n_pass_SR3bjv[i]  << endl;
    cout << "pass SR3 mct:  " << n_pass_SR3mct[i]  << endl;
    cout << "pass SR3 MET:  " << n_pass_SR3MET[i]  << endl;
    cout << "---------------------------------"    << endl;
    cout << "pass SR4 JV:   " << n_pass_SR4jv[i]   << endl;
    cout << "pass SR4 ZV:   " << n_pass_SR4Zv[i]   << endl;
    cout << "pass SR4 MET:  " << n_pass_SR4MET[i]  << endl;
    cout << "pass SR4 Mt2:   " << n_pass_SR4MT2[i]  << endl;
    cout << "---------------------------------"    << endl;
    cout << "pass SR5 JV:           " << n_pass_SR5jv[i]        << endl;
    cout << "pass SR5 MET:          " << n_pass_SR5MET[i]       << endl;
    cout << "pass SR5 ZV:           " << n_pass_SR5Zv[i]        << endl;
    cout << "pass SR5 l0 Pt:        " << n_pass_SR5L0pt[i]      << endl;
    cout << "pass SR5 Sum Pt:       " << n_pass_SR5SUMpt[i]     << endl;
    cout << "pass SR5 dPhi(Met,ll): " << n_pass_SR5dPhiMETLL[i] << endl;
    cout << "pass SR5 dPhi(Met,l1): " << n_pass_SR5dPhiMETL1[i] << endl;


   }

}

/*--------------------------------------------------------------------------------*/
// Reset
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::reset()
{
  isDG2L_SR0_OS =false;
  isDG2L_SR0_SS =false;
  isDG2L_SR1 =false;
  isDG2L_SR2 =false;
  isDG2L_SR3 =false;
  isDG2L_SR4 =false;
  
  isDG2L_CR0 =false;
  isDG2L_CR1 =false;

  _ww = 0;

  DG2L_flavor=999;
  DG2L_qprod=999;

  metRel = 0;
  mT2    = 0;
  mCT    = 0;

  m_ET = ET_Unknown;
 

}

/*--------------------------------------------------------------------------------*/
// Full event selection
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::selectEvent(const LeptonVector* leptons, const LeptonVector* baseLeps)
{
  // In this method place all event selection cuts.
  int flag = nt->evt()->evtFlag[NtSys_NOM];

  if( !passLAr(flag) )              return false;
  n_pass_LAr++;
  if( !passBadJet(flag) )           return false;
  n_pass_BadJet++;
  if( !passBadMuon(flag) )          return false;
  n_pass_BadMuon++;
  if( !passCosmic(flag) )           return false;
  n_pass_Cosmic++;
  if(!passNBaseLepCut(baseLeps))    return false;
  
  // Get Event Type to continue cutflow
  m_ET = getDiLepEvtType(*baseLeps);
  
  if(m_ET == ET_Unknown) return false;
  n_pass_dil[m_ET]++;
  

  if( !passTrigger(baseLeps) )     return false;  
  n_pass_flavor[m_ET]++;
  
  if( !passNLepCut(leptons) )       return false;
  
  if( !passMll(leptons) )           return false;
  
  setEventWeight(LUMIMODE); //set _ww to the appropriate weighting

  for(uint j=0; j<DG2L_NSR; j++)
    _hh->H1FILL(_hh->DG2L_cutflow[j][m_ET],0,_ww); 
  
  return true;
}
/*--------------------------------------------------------------------------------*/
// Event weight
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::setEventWeight(int mode)
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
// Signal Region 1 
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passSR1(const LeptonVector* leptons, const JetVector* jets, const Met *met)
{
  DG2L_SR SR = DG2L_SR1; 

  // Only for OS events
  if( !oppositeSign(leptons) )           return false;  
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],1,_ww); 

  // Jet Veto
  if( !passJetVeto(jets) )               return false;
  n_pass_SR1jv[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],2,_ww); 

  // Reject events with mll in Z window
  if( !passZVeto(leptons))               return false;
  n_pass_SR1Zv[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],3,_ww); 

  // Reject if Met_rel < 100
  if( !passMETRel(met,leptons,jets) )    return false;
  n_pass_SR1MET[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],4,_ww); 

  isDG2L_SR1=true;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passSR2(const LeptonVector* leptons, const JetVector* jets, const Met *met)
{
  DG2L_SR SR = DG2L_SR2; 

  // Only SS
  if( !sameSign(leptons) )               return false;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],1,_ww); 
    
  // Check Jet Veto
  if( !passJetVeto(jets) )               return false;
  n_pass_SR2jv[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],2,_ww); 

  // Check MET rel > 100
  if( !passMETRel(met,leptons,jets) )    return false;
  n_pass_SR2MET[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],3,_ww); 

  isDG2L_SR2=true;
  return true;

}

/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passSR3(const LeptonVector* leptons, const JetVector* jets, const Met *met)
{
  DG2L_SR SR = DG2L_SR3; 

  // Only SFOS
  if( !oppositeSign(leptons) )           return false;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],1,_ww); 

  if( !sameFlavor(leptons) )             return false;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],2,_ww); 

  // Require at least 2 jets Pt > 30
  if( !passge2Jet(jets) )                return false;
  n_pass_SR3ge2j[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],3,_ww); 

  // Apply a Zveto
  if( !passZVeto(leptons) )              return false;
  n_pass_SR3Zv[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],4,_ww); 

  // Apply b jet veto
  if( !passbJetVeto(jets) )              return false;
  n_pass_SR3bjv[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],5,_ww); 

  // Veto top-tag events 
  if( !passTopTag(*leptons,*jets,met) )    return false;
  n_pass_SR3mct[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],6,_ww); 

  // MetRel > 50
  if( !passMETRel(met,leptons,jets,50) ) return false;
  n_pass_SR3MET[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],7,_ww); 

  isDG2L_SR3=true;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passSR4(const LeptonVector* leptons, const JetVector* jets, const Met *met)
{
  DG2L_SR SR = DG2L_SR4; 
  // For OS
  if( !oppositeSign(leptons) )          return false;  
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],1,_ww); 

  // Check Jet Veto
  if( !passJetVeto(jets) )              return false;
  n_pass_SR4jv[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],2,_ww); 

  // Check Z Veto
  if( !passZVeto(leptons) )             return false;
  n_pass_SR4Zv[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],3,_ww); 

  // Check METRel > 40
  if( !passMETRel(met,leptons,jets,40) ) return false;
  n_pass_SR4MET[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],4,_ww); 

  // Check MT2 > 90
  if( !passMT2(leptons, met, 90) )      return false;
  n_pass_SR4MT2[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],5,_ww); 
  
  isDG2L_SR4=true;
  return true;

}

/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passSR5(const LeptonVector* leptons, const JetVector* jets, const Met *met)
{

  DG2L_SR SR = DG2L_SR5; 

  // OS only
  if( !oppositeSign(leptons) )           return false;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],1,_ww); 

  // Jet Veto
  if( !passJetVeto(jets) )               return false;
  n_pass_SR5jv[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],2,_ww); 

  // MetRel > 40
  if( !passMETRel(met,leptons,jets,40) ) return false;
  n_pass_SR5MET[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],3,_ww); 
  
  // Z Veto
  if( !passZVeto(leptons) )              return false;
  n_pass_SR5Zv[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],4,_ww); 

  // Leading lepton Pt > 50
  float pt0 = leptons->at(0)->Pt();
  if( pt0 < 50 )                         return false;
  n_pass_SR5L0pt[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],5,_ww); 

  // Sum of Pt > 100
  float pt1 = leptons->at(1)->Pt();
  if( pt0 + pt1 < 100 )                  return false;
  n_pass_SR5SUMpt[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],6,_ww);   

  // dPhi(met, ll) > 2.5
  TLorentzVector metlv = met->lv();
  TLorentzVector ll = (*leptons->at(0) + *leptons->at(1));
  if( !passdPhi(metlv, ll, 2.5) )    return false;
  n_pass_SR5dPhiMETLL[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],7,_ww); 

  // dPhi(met, l1) > 0.5
  TLorentzVector l1 = *leptons->at(1);
  if( !passdPhi(metlv, l1, 0.5) )    return false;
  n_pass_SR5dPhiMETL1[m_ET]++;
  _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],8,_ww); 

  isDG2L_SR5=true;
  return true;

}

/*--------------------------------------------------------------------------------*/
// Generic cuts
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passNLepCut(const LeptonVector* leptons)
{
  uint nLep = leptons->size();
  if(m_nLepMin>=0 && nLep < m_nLepMin) return false;
  if(m_nLepMax>=0 && nLep > m_nLepMax) return false;
  n_pass_nLep[m_ET]++;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passNBaseLepCut(const LeptonVector* baseLeptons)
{
  if(m_cutNBaseLep){
    uint nLep = baseLeptons->size();
    if(m_nLepMin>=0 && nLep < m_nLepMin) return false;
    if(m_nLepMax>=0 && nLep > m_nLepMax) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passTrigger(const LeptonVector* leptons)
{
  //ANYES - TODO
  n_pass_trig[m_ET]++;
  return true;

  /*
  if(leptons->size() < 1){
    n_pass_trig[m_ET]++;
    return true;
  }

  int run         = nt->evt()->run;
  DataStream strm = nt->evt()->stream;
  if( m_trigObj->passDilTrig(*leptons, run, strm) ){
    n_pass_trig[m_ET]++;
    return true;
  }
  return false;
  */
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::sameFlavor(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;
  return (leptons->at(0)->isMu() == leptons->at(1)->isMu());
  //return (leptons->at(0)->isEle() && leptons->at(1)->isEle());
  //return (leptons->at(0)->isMu() && leptons->at(1)->isMu());
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::oppositeFlavor(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;
  return !(leptons->at(0)->isMu() == leptons->at(1)->isMu());
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::sameSign(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;
  return leptons->at(0)->q * leptons->at(1)->q > 0;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::oppositeSign(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;
  return leptons->at(0)->q * leptons->at(1)->q < 0;

}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passMll(const LeptonVector* leptons, float mll)
{
  if( leptons->size() < 2 ) return false;
  if( (*leptons->at(0) + *leptons->at(1)).M() < mll ) return false;
  n_pass_mll[m_ET]++;
  return true;
}
 
/*--------------------------------------------------------------------------------*/
// Signal region cuts
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passJetVeto(const JetVector* jets)
{
  bool failjet = false;
  for(uint i=0; i<jets->size(); ++i){
    const Jet* jet = jets->at(i);
    if( jet->Pt() < 30         ) continue;
    if( fabs(jet->Eta()) > 2.5 ) continue;
    if( jet->jvf < 0.75        ) continue;
    failjet = true;
    break;
  }
  
  if( failjet ) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passbJetVeto(const JetVector* jets)
{
  bool hasbjet = false;
  for(uint i=0; i<jets->size(); ++i){
    const Jet* jet = jets->at(i);
    if( !isBJet(jet,MV1_85) ) continue;
    hasbjet = true;
    break;
  }
  
  if( hasbjet ) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passge2Jet(const JetVector* jets)
{
  // Excessive methods!!!!!! 
  return (jets->size() >= 2);
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passZVeto(const LeptonVector* leptons, float Zlow, float Zhigh)
{
  if( leptons->size() < 2 ) return false;
  float mll = (*leptons->at(0) + *leptons->at(1)).M();
  if( Zlow < mll && mll < Zhigh ) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passMETRel(const Met *met, const LeptonVector* leptons, 
				 const JetVector* jets, float metMax){
  
  metRel = getMetRel(met,*leptons,*jets);
  if(  metRel < metMax ) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passdPhi(TLorentzVector v0, TLorentzVector v1, float cut)
{
  return v0.DeltaPhi(v1) > cut;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passMT2(const LeptonVector* leptons, const Met* met, float cut)
{
  if( leptons->size() < 2 ) return false;

  // necessary variables
  TLorentzVector metlv = met->lv();
  TLorentzVector l0    = *leptons->at(0);
  TLorentzVector l1    = *leptons->at(1);

  double pTMiss[3] = {0.0, metlv.Px(), metlv.Py()};
  double pA[3]     = {0.0, l0.Px(), l0.Py()};
  double pB[3]     = {0.0, l1.Px(), l1.Py()};
  
  // Create Mt2 object
  mt2_bisect::mt2 mt2_event;
  mt2_event.set_momenta(pA,pB,pTMiss);
  mt2_event.set_mn(0); // LSP mass = 0 is Generic
  
  mT2 = mt2_event.get_mt2();
  return (mt2_event.get_mt2() > cut);
  
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::fillHistograms()
{
  for(uint iSR=0; iSR<nHSR_DG2L; iSR++){//loop all SR's
    if(iSR==0 && !isDG2L_SR0_OS) continue;
    if(iSR==1 && !isDG2L_SR0_SS) continue;
    if(iSR==2 && !isDG2L_SR1) continue;
    if(iSR==3 && !isDG2L_SR2) continue;
    if(iSR==4 && !isDG2L_SR3) continue;
    if(iSR==5 && !isDG2L_SR4) continue;
    if(iSR==6 && !isDG2L_SR5) continue;

    int q1=0;
    int q2=0;
    int qqType=0;
    TLorentzVector _ll;
    for(uint ilep=0; ilep<v_sigLep->size(); ilep++){
      const Susy::Lepton* _l = v_sigLep->at(ilep);
      _ll = _ll + (*_l);
      if(ilep==0){
	_hh->H1FILL(_hh->DG2L_ptl1[iSR][m_ET],_l->Pt(),_ww); 
	_hh->H1FILL(_hh->DG2L_etal1[iSR][m_ET],_l->Eta(),_ww); 
	_hh->H1FILL(_hh->DG2L_d0Sl1[iSR][m_ET],_l->d0/_l->errD0,_ww); 
	_hh->H1FILL(_hh->DG2L_z0sinthetal1[iSR][m_ET],_l->z0*sin(_l->Theta()),_ww); 
	q1=_l->q;
      }
      else if(ilep==1){
	_hh->H1FILL(_hh->DG2L_ptl2[iSR][m_ET],_l->Pt(),_ww); 
	_hh->H1FILL(_hh->DG2L_etal2[iSR][m_ET],_l->Eta(),_ww); 
	_hh->H1FILL(_hh->DG2L_d0Sl2[iSR][m_ET],_l->d0/_l->errD0,_ww); 
	_hh->H1FILL(_hh->DG2L_z0sinthetal2[iSR][m_ET],_l->z0*sin(_l->Theta()),_ww); 
	q2=_l->q;
      }

    }//leptons 
    
    if(q1*q2<0){//OS
      qqType = (q1<0 && q2>0) ? 0 : 1;  //-+/+-
    }
    else if(q1*q2>0){//SS
      qqType = (q1<0 && q2<0) ? 0 : 1;  //--/++
    }
    _hh->H1FILL(_hh->DG2L_qq[iSR][m_ET],qqType,_ww); 


    _hh->H1FILL(_hh->DG2L_mll[iSR][m_ET],_ll.M(),_ww); 
    _hh->H1FILL(_hh->DG2L_etmiss[iSR][m_ET],m_met->lv().Pt(),_ww); 
    _hh->H1FILL(_hh->DG2L_metrel[iSR][m_ET],metRel,_ww); 
    _hh->H1FILL(_hh->DG2L_mt2[iSR][m_ET],mT2,_ww); 

    float corrNpv = nt->evt()->nVtx;
    if(nt->evt()->isMC) corrNpv = GetNVertexBsCorrected(nt->evt()->nVtx);
    _hh->H1FILL(_hh->DG2L_npv[iSR][m_ET],corrNpv,_ww); 
    _hh->H1FILL(_hh->DG2L_mu[iSR][m_ET],nt->evt()->avgMu,_ww); 

    int nBJets=0;
    _hh->H1FILL(_hh->DG2L_nJets[iSR][m_ET],v_sigJet->size(),_ww); 
    for(uint ijet=0; ijet<v_sigJet->size(); ijet++){
      const Susy::Jet* _j = v_sigJet->at(ijet);
      if(isBJet(_j,MV1_85)){
	nBJets++;
	_hh->H1FILL(_hh->DG2L_ptbj[iSR][m_ET],_j->Pt(),_ww); 
	_hh->H1FILL(_hh->DG2L_etabj[iSR][m_ET],_j->Eta(),_ww); 
      }
      if(ijet==0){
	_hh->H1FILL(_hh->DG2L_ptj1[iSR][m_ET],_j->Pt(),_ww); 
	_hh->H1FILL(_hh->DG2L_etaj1[iSR][m_ET],_j->Eta(),_ww); 
      }
      if(ijet==1){
	_hh->H1FILL(_hh->DG2L_ptj2[iSR][m_ET],_j->Pt(),_ww); 
	_hh->H1FILL(_hh->DG2L_etaj2[iSR][m_ET],_j->Eta(),_ww); 
      }
      if(ijet==2){
	_hh->H1FILL(_hh->DG2L_ptj3[iSR][m_ET],_j->Pt(),_ww); 
	_hh->H1FILL(_hh->DG2L_etaj3[iSR][m_ET],_j->Eta(),_ww); 
      }
      if(ijet==3){
	_hh->H1FILL(_hh->DG2L_ptj4[iSR][m_ET],_j->Pt(),_ww); 
	_hh->H1FILL(_hh->DG2L_etaj4[iSR][m_ET],_j->Eta(),_ww); 
      }
    }
    _hh->H1FILL(_hh->DG2L_nBJets[iSR][m_ET],nBJets,_ww); 

  }//end loop SR's
  
}
