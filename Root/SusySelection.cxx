#include <iomanip>
#include <iostream>
#include "TCanvas.h"
#include "SusyWeakProdAna/SusySelection.h"
#include "SusyWeakProdAna/PhysicsTools.h"

#include "SusyWeakProdAna/SusyAnaCommon.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// SusySelection Constructor
/*--------------------------------------------------------------------------------*/
SusySelection::SusySelection(bool is2LAna, bool qFlipd0):
  SusyNtTools(),
  m_dbg(0),
  m_cutNBaseLep(true),
  m_nLepMin(2), //Anyes Set these in thr 2L/WH
  m_nLepMax(2),
  m_nLep3Min(3), //Set in 3: and 4L code note here!
  m_nLep3Max(3),
  m_nLep4Min(4),
  m_nLep4Max(99),
  m_selOS(false),
  m_selSS(false),
  m_selSF    (false),
  m_selOF    (false),
  m_vetoSF   (false),
  m_selSFOS   (false),
  m_vetoSFOS   (false),
  m_selZ       (false),
  m_vetoZ      (false),
  m_vetoExtZ   (false),
  m_selZllZll  (false),
  m_selB       (false),
  m_vetoB      (false),
  m_vetoF      (false),
  m_vetoJ      (false),
  m_minC20     ( -1  ),
  m_maxC20     ( -1  ),
  m_minCJet    ( -1  ),
  m_minB20     ( -1  ),
  m_maxB20     ( -1  ),
  m_metMin     ( -1  ),
  m_metMax     ( -1  ),
  m_metRelMin  ( -1  ),
  m_metRelMax  ( -1  ),
  m_topTag     (false),
  m_mt2Min     ( -1  ),
  m_mt2Max     ( -1  ),
  m_mtMin      ( -1  ),
  m_mtMax      ( -1  ),
  m_lepLeadPtMin( -1  ),
  m_pTl0Min     ( -1  ),
  m_pTl1Min     ( -1  ),
  m_IsoMin      ( -1  ),
  m_d0SMin      ( -1  ),
  m_pTllMin     ( -1  ),
  m_pTllMax     ( -1  ),
  m_pTllBound   (false),
  m_lowMll      ( -1  ),
  m_highMll     ( -1  ),
  m_mllIn       (false),
  m_dPhillMax   ( -1  ),
  m_dRllMin     ( -1  ),
  m_dRllMax     ( -1  ),
  m_lowMjj      ( -1  ),
  m_highMjj     ( -1  ),
  m_lowMTWW     ( -1  ),
  m_highMTWW    ( -1  ),
  m_pTj0Min     ( -1  ),
  m_pTj1Min     ( -1  ),
  m_dPhiMetll   ( -1  ),
  m_dPhiMetl1   ( -1  ),
  m_dPhillJ0Min ( -1  ),
  m_dPhillJ0Max ( -1  ),
  m_dPhillMetMin ( -1  ),
  m_dPhillMetMax ( -1  ),
  m_MetMeffMin  ( -1  ),
  m_MetMeffMax  ( -1  ),
  m_MeffMin     ( -1  ),
  m_MeffMax     ( -1  ),
  m_HTMin     ( -1  ),
  m_HTMax     ( -1  ),
  m_vetoLooseSFOSinZ (false),

  m_ET(ET_Unknown)
{
  reset();
  resetCounter();

  _random = new TRandom3(2012300958);

  //Configure ChargeFlip tool
  string chargeFlipInput =  string(getenv("WORKAREA")) + 
    "/ChargeFlip/data/chargeflip_map.root";
  if(qFlipd0) chargeFlipInput =  string(getenv("WORKAREA")) + 
    "/ChargeFlip/data/d0_chargeflip_map.root";
  cout << "Using flip map: " << chargeFlipInput <<endl;
  m_chargeFlip = new chargeFlip(chargeFlipInput);




}

/*--------------------------------------------------------------------------------*/
// Reset 
/*--------------------------------------------------------------------------------*/
void SusySelection::resetCounter()
{
  n_pass_SUSYGrid = 0;
  n_readin       = 0;
  n_pass_GRL     = 0;
  n_pass_LarErr  = 0;
  n_pass_TileErr = 0;
  n_pass_TTCVeto = 0;
  n_pass_GoodVtx = 0;
  n_pass_TileTrip= 0;
  n_pass_HotSpot = 0;
  n_pass_BadJet  = 0;
  n_pass_BadMuon = 0;
  n_pass_Cosmic  = 0;
  n_pass_BadFCAL = 0;
  n_pass_DeadRegion  = 0;
  n_pass_atleast2BaseLep = 0;
  n_pass_exactly2BaseLep = 0;
  n_pass_mll20   = 0;
  n_pass_nBase3Lep   = 0;
  n_pass_nBase4Lep   = 0;

  // The rest are channel specific.
  for(int i=0; i<LEP_N; ++i){
    n_pass_signalLep[i]   = 0;
    n_pass_dil[i]         = 0;
    n_pass_nLep3[i]       = 0;
    n_pass_nLep4[i]       = 0;
    n_pass_tauVeto[i]     = 0;
    n_pass_trig[i]        = 0;
    n_pass_truth[i]       = 0;

    for(int j=0; j<SR_N; ++j){
      n_pass_3Ltrig[i][j]    = 0;
    
      n_pass_ss[i][j]        = 0;
      n_pass_os[i][j]        = 0;
      n_pass_flav[i][j]      = 0;
      n_pass_Z[i][j]         = 0;
      n_pass_ZllZll[i][j]    = 0;
      n_pass_FullJveto[i][j] = 0;
      n_pass_FJet[i][j]      = 0;
      n_pass_BJet[i][j]      = 0;
      n_pass_LJet[i][j]      = 0;
      n_pass_CJet[i][j]      = 0;
      n_pass_NBJet[i][j]      = 0;
      n_pass_JetPt[i][j]     = 0;
      n_pass_mjj[i][j]       = 0;
      n_pass_leadLepPt[i][j] = 0;
      n_pass_MuIso[i][j]     = 0;
      n_pass_EleD0S[i][j]    = 0;
      n_pass_mll[i][j]       = 0;
      n_pass_pTll[i][j]      = 0;
      n_pass_pTllBound[i][j] = 0;
      n_pass_dPhill[i][j]    = 0;
      n_pass_dRll[i][j]      = 0;
      n_pass_mWWT[i][j]      = 0;
      n_pass_topTag[i][j]    = 0;
      n_pass_metRel[i][j]    = 0;
      n_pass_mt2[i][j]       = 0;
      n_pass_minMt[i][j]     = 0;

      n_pass_met[i][j]       = 0;
      n_pass_dPhiMetll[i][j] = 0;
      n_pass_dPhiMetl1[i][j] = 0;

      n_pass_dPhillJ0[i][j]   = 0;
      n_pass_dPhillMet[i][j]  = 0;
      n_pass_MetMeff[i][j]    = 0;
      n_pass_Meff[i][j]       = 0;
      n_pass_HT[i][j]         = 0;
      n_pass_looseSFOSinZ[i][j] = 0;

      n_pass_sfos[i][j]      = 0;
      n_pass_mt3L[i][j]      = 0;

    }
  }

}
/*--------------------------------------------------------------------------------*/
// Reset  per event
/*--------------------------------------------------------------------------------*/
void SusySelection::reset()
{
  _inc = 1;
  SR=0;//DIL_NSR;
  m_ET = ET_Unknown;


}
/*--------------------------------------------------------------------------------*/
// Set cut selection
/*--------------------------------------------------------------------------------*/
void SusySelection::resetCuts()
{
  m_cutNBaseLep = true;
  m_nLepMin = 2;
  m_nLepMax = 2;
  m_nLep3Min = 3;
  m_nLep3Max = 3;
  m_nLep4Min = 4;
  m_nLep4Max = 99;
  m_selOS    = false;
  m_selSS    = false;
  m_selSF    = false;
  m_selOF    = false;
  m_vetoSF   = false;
  m_selSFOS  = false;
  m_vetoSFOS = false;
  m_selZ     = false;
  m_vetoZ    = false;
  m_vetoExtZ = false;
  m_selZllZll = false;
  m_selB     = false;
  m_vetoB    = false;
  m_vetoF    = false;
  m_vetoJ    = false;
  m_minC20   =    -1;
  m_maxC20   =    -1;
  m_minCJet  =    -1;
  m_minB20   =    -1;
  m_maxB20   =    -1;
  m_metMin     =  -1;
  m_metMax     =  -1;
  m_metRelMin  =  -1;
  m_metRelMax  =  -1;
  m_topTag     = false;
  m_mt2Min     =  -1;
  m_mt2Max     =  -1;
  m_mtMin     =  -1;
  m_mtMax     =  -1;
  m_lepLeadPtMin = -1;
  m_pTl0Min      = -1;
  m_pTl1Min      = -1;
  m_IsoMin       = -1;
  m_d0SMin       = -1;
  m_pTllMin = -1;
  m_pTllMax = -1;
  m_pTllBound = false;
  m_lowMll    =  -1;
  m_highMll   =  -1;
  m_mllIn     = false; 
  m_dPhillMax =  -1;
  m_lowMjj    =  -1;
  m_highMjj   =  -1;
  m_lowMTWW   =  -1;
  m_highMTWW  =  -1;
  m_pTj0Min   =  -1;
  m_pTj1Min   =  -1;
  m_dRllMin   =  -1;
  m_dRllMax   =  -1;
  m_dPhiMetll =  -1;
  m_dPhiMetl1 =  -1;
  m_dPhillJ0Min = -1;
  m_dPhillJ0Max = -1;
  m_dPhillMetMin = -1;
  m_dPhillMetMax = -1;
  m_MetMeffMin  = -1;
  m_MetMeffMax  = -1;
  m_MeffMin     = -1;
  m_MeffMax     = -1;
  m_HTMin       = -1;
  m_HTMax       = -1;
  m_vetoLooseSFOSinZ = false;

}

/*--------------------------------------------------------------------------------*/
// Event cleaning
/*--------------------------------------------------------------------------------*/
bool SusySelection::passEventCleaning()
{
  uint iiSys = SYST;
  if(SYST>DGSys_RESOST) iiSys = DGSys_NOM;

  int cutFlag = cleaningCutFlags(nt->evt()->cutFlags[iiSys],
				 *v_preMu, *v_baseMu,
				 *v_preJet, *v_baseJet);
  
  //
  // SUSY grid simplified model: remove higgsino events
  //
  if( nt->evt()->eventWithSusyProp ) return false;
  if(SYST==DGSys_NOM) n_pass_SUSYGrid +=_inc;

  //
  // Updated Summer 2013
  //
  if(!passGRL(cutFlag)) { 
    if(dbg()>15) cout << "GRL " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_GRL+=_inc;

 
  if(!passTileTripCut(cutFlag)) { 
    if(dbg()>15) cout << "TileTrip " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_TileTrip+=_inc;

  if(!passLarErr(cutFlag)) { 
    if(dbg()>15) cout << "LarErr " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_LarErr+=_inc;

  //TEMPORARY
  JetVector jets = getPreJets(nt, (SusyNtSys) iiSys);
  e_j_overlap(*v_baseEle, jets, J_E_DR, true);
  t_j_overlap(*v_sigTau, jets, J_T_DR, true);
  if(hasBadJet(jets)){
    if(dbg()>15) cout << "bad jet " << endl;
    jets.clear();
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_BadJet+=_inc;
  jets.clear();

  /*
  if(hasBadJet(*v_baseJet)){
    if(dbg()>15) cout << "bad jet " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_BadJet+=_inc;
  */
   
  if( !passDeadRegions(*v_preJet, m_met, nt->evt()->run, nt->evt()->isMC)){
     if(dbg()>15) cout<<"Fail Dead Regions" << endl; 
     return false;
  }
  if(SYST==DGSys_NOM) n_pass_DeadRegion+=_inc;
  

  if(hasBadMuon(*v_preMu)) {
    if(dbg()>15) cout << "bad muon " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_BadMuon+=_inc;

  if(hasCosmicMuon(*v_baseMu)){
    if(dbg()>15) cout << "cosmic " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_Cosmic+=_inc;

  if(hasHotSpotJet(*v_preJet)) { 
    if(dbg()>15) cout << "hot spot " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_HotSpot+=_inc;

  /*
  if(!passTileErr(cutFlag)) { 
    if(dbg()>15) cout << "TileErr " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_TileErr+=_inc;
  */

  if(!passTTCVeto(cutFlag)) { 
    if(dbg()>15) cout << "TTCVeto " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_TTCVeto+=_inc;

  if(!passGoodVtx(cutFlag)) { 
    if(dbg()>15) cout << "Good Vtx " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_GoodVtx+=_inc;

  return true;
}

/*--------------------------------------------------------------------------------*/
// FCAL bad C1-C8 run206248-207332
/*--------------------------------------------------------------------------------*/
bool SusySelection::passBadFCAL(const JetVector* jets, int run, bool isMC)
{
  bool inBadFCAL=hasJetInBadFCAL(*jets,run,isMC);

  if(!isMC){ //Veto data C1-C8
    if(run>=BAD_FCAL_RUN1 && 
       run <=BAD_FCAL_RUN2 && 
       inBadFCAL) return false;
  }
  if(SYST==DGSys_NOM) n_pass_BadFCAL+=_inc;
  return true;
}

/*--------------------------------------------------------------------------------*/
// Generic cuts
/*--------------------------------------------------------------------------------*/
bool SusySelection::passTauVeto(const TauVector* taus)
{
  if(taus->size()==0){
    if(SYST==DGSys_NOM) n_pass_tauVeto[m_ET]+=_inc;
    return true;
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passNLepCut(const LeptonVector* leptons)
{
  uint nLep = leptons->size();
  if(m_nLepMin>=0 && nLep < m_nLepMin) return false;
  if(m_nLepMax>=0 && nLep > m_nLepMax) return false;
  //Tighten muon eta to 2.4
  for(uint ilep=0; ilep<leptons->size(); ilep++){
    const Susy::Lepton* _l = leptons->at(ilep);
    if(_l->isEle()) continue;
    if(fabs(_l->Eta())>2.4) return false;
  }
  if(SYST==DGSys_NOM) n_pass_signalLep[m_ET]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passNBase3LepCut(const LeptonVector* baseLeptons)
{
  if(m_cutNBaseLep){
    uint nLep = baseLeptons->size();
    if(m_nLep3Min>=0 && nLep < m_nLep3Min) return false;
    if(m_nLep3Max>=0 && nLep > m_nLep3Max) return false;
  }
  if(SYST==DGSys_NOM) n_pass_nBase3Lep+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passNBase4LepCut(const LeptonVector* baseLeptons)
{
  if(m_cutNBaseLep){
    uint nLep = baseLeptons->size();
    if(m_nLep4Min>=0 && nLep < m_nLep4Min) return false;
    if(m_nLep4Max>=0 && nLep > m_nLep4Max) return false;
  }
  if(SYST==DGSys_NOM) n_pass_nBase4Lep+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passNLep3Cut(const LeptonVector* leptons)
{
  uint nLep = leptons->size();

  if(m_nLep3Min>=0 && nLep < m_nLep3Min) return false;
  if(m_nLep3Max>=0 && nLep > m_nLep3Max) return false;

  if(SYST==DGSys_NOM) n_pass_nLep3[m_ET]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passNLep4Cut(const LeptonVector* leptons)
{
  uint nLep = leptons->size();
  if(m_nLep4Min>=0 && nLep < m_nLep4Min) return false;
  if(m_nLep4Max>=0 && nLep > m_nLep4Max) return false;

  if(SYST==DGSys_NOM) n_pass_nLep4[m_ET]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passTrigger(const LeptonVector* leptons, DilTrigLogic* trigObj, 
				const Met *met)
{
  if(leptons->size() < 1) return false;
  if( trigObj->passDilTrig(*leptons, met->lv().Pt(), nt->evt()) ){
    if(SYST==DGSys_NOM) n_pass_trig[m_ET]+=_inc;
    return true;
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::pass3LTrigger(const LeptonVector* leptons, const TauVector* taus,
				  TrilTrigLogic* trig3LObj)
{
  bool useDilepTrigs=true;
  if(!trig3LObj->passTriggerMatching(*leptons, *taus, nt->evt(), useDilepTrigs))  return false;
  if(SYST==DGSys_NOM) n_pass_3Ltrig[m_ET][SR]+=_inc;
  return true;
}

/*--------------------------------------------------------------------------------*/
bool SusySelection::passIsPromptLepton(const LeptonVector* leptons, int method, bool isMC)
{
  if(leptons->size() < 1) return false;
  
  if(isMC && method == RLEP){
    for(uint ilep=0; ilep<leptons->size(); ilep++){
      const Susy::Lepton* _l = leptons->at(ilep);
      int org        = _l->mcOrigin;
      int type       = _l->mcType;
      int mcId       = nt->evt()->mcChannel;
      int truthMatch = _l->truthType;
      bool isEle     = _l->isEle();
      
      bool isReal       = isPT(org,type,mcId,truthMatch,isEle,"dummy");
      bool isChargeFlip =  _l->isEle() ? ((Electron*) _l)->isChargeFlip : false; 
      
      //Anyes 040613 - Hack gg2WW
      if(mcId >= 169471 && mcId <=169479) return true;

      if( !( isReal && !isChargeFlip) ) return false; //W/o including charge flip
      //if( !( isReal  || isChargeFlip) ) return false; //To include the charge flip
    }
  }
  if(SYST==DGSys_NOM) n_pass_truth[m_ET]+=_inc;
  return true;
}

/*--------------------------------------------------------------------------------*/
// To determine if have true OS event - ie neither e has qFlip
/*--------------------------------------------------------------------------------*/
bool SusySelection::hasQFlip(const LeptonVector* leptons){
  if(leptons->size() < 1) return false;

  const Susy::Lepton* _l1 = leptons->at(0);
  const Susy::Lepton* _l2 = leptons->at(1);
  bool l1_isQFlip = _l1->isEle() ? ((Electron*) _l1)->isChargeFlip : false;
  bool l2_isQFlip = _l2->isEle() ? ((Electron*) _l2)->isChargeFlip : false;
  
  if(l1_isQFlip || l2_isQFlip) return true;

  return false;
}

/*--------------------------------------------------------------------------------*/
float SusySelection::getQFlipProb(LeptonVector* leptons, Met* met, uint iSys)
{
  if(leptons->size() < 1) return 1;
  Susy::Lepton* _l1 = leptons->at(0);
  Susy::Lepton* _l2 = leptons->at(1);
  
  TVector2 _new_met(met->lv().Px(),met->lv().Py());
  TLorentzVector _l1_tlv(*_l1);
  TLorentzVector _l2_tlv(*_l2);
  int _pdg1 = _l1->isEle() ? 11 : 13; _pdg1 *= _l1->q; //right convention???
  int _pdg2 = _l2->isEle() ? 11 : 13; _pdg2 *= _l2->q;
  
  int _sys = 0;
  if(iSys==DGSys_BKGMETHOD_UP) _sys=1;
  if(iSys==DGSys_BKGMETHOD_DN) _sys=-1;

  m_chargeFlip->setSeed(nt->evt()->event); //Do the seed here using evt #
  float cfP = m_chargeFlip->OS2SS(_pdg1, &_l1_tlv, 
				  _pdg2, &_l2_tlv, 
				  &_new_met, _sys);

  cfP*=  m_chargeFlip->overlapFrac().first; // QFLIP_RESCLALE;
  //cfP*= QFLIP_RESCLALE;
  
  if(dbg()>5){
    cout << "qFlip - original " <<endl;
    cout << "\t l1 org_pt " << _l1->Pt() 
	 << "\t l2 org_pt " << _l2->Pt() 
	 << "\t org met Pt  " << met->lv().Pt() 
	 << " met Phi " << met->lv().Phi()
	 << endl;
  }
  
   met->Et = _new_met.Mod();
   met->phi = _new_met.Phi();
  
  _l1->pt = _l1_tlv.Pt(); _l1->resetTLV();
  _l2->pt = _l2_tlv.Pt(); _l2->resetTLV();

  //resort the vector
  std::sort(leptons->begin(), leptons->end(), comparePt);

  if(dbg()>5){
    cout << "qFlip - smear " <<endl;
    cout << "\t l1 new_pt " << _l1->Pt() 
	 << "\t l2 new_pt " << _l2->Pt() 
	 << "\t new met   " << met->lv().Pt() << " phi " << met->lv().Phi()
	 << "\t cfP " << cfP << " " << QFLIP_RESCLALE
	 << endl;
  }




  return cfP;  
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passFlavor(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;
  bool sameFlav = isSameFlav(leptons->at(0), leptons->at(1));
  if(m_selSF && !sameFlav) return false;
  if(m_selOF && sameFlav) return false;
  if(SYST==DGSys_NOM) n_pass_flav[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passQQ(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;  
  float qq = leptons->at(0)->q * leptons->at(1)->q;
  if(m_selOS && qq >0 ) return false;
  if(m_selSS && qq <0 ) return false;
  if(m_selOS && SYST==DGSys_NOM) n_pass_os[m_ET][SR]+=_inc;
  if(m_selSS && SYST==DGSys_NOM) n_pass_ss[m_ET][SR]+=_inc;
  return true;
}

/*--------------------------------------------------------------------------------*/
bool SusySelection::isGenuineSS(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;  
  float qq = leptons->at(0)->q * leptons->at(1)->q;
  if(hasQFlip(leptons)) return false;
  if(qq <0 ) return false;
  return true;
}

/*--------------------------------------------------------------------------------*/
// Signal region cuts  - use all signal jets C20+B20+F30
/*--------------------------------------------------------------------------------*/
bool SusySelection::passFullJetVeto(const JetVector* jets)
{
  if(m_vetoJ && jets->size()>0) return false;

  if(SYST==DGSys_NOM) n_pass_FullJveto[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passBJet(const JetVector* jets){
  int nBJet= numberOfCBJets(*jets);
  if(m_vetoB && nBJet>0) return false;
  if(m_selB  && nBJet<=0) return false;
  if(SYST==DGSys_NOM) n_pass_BJet[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passFJet(const JetVector* jets){
  int nFJet= numberOfFJets(*jets);
  if(m_vetoF && nFJet>0) return false;
  if(SYST==DGSys_NOM) n_pass_FJet[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passLJet(const JetVector* jets){
  int nLJet= numberOfCLJets(*jets);
  if(m_minC20>-1 && nLJet<m_minC20) return false;
  if(m_maxC20>-1 && nLJet>m_maxC20) return false;
  if(SYST==DGSys_NOM) n_pass_LJet[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passCentralJet(const JetVector* jets){
  int nCJet= numberOfCLJets(*jets)+ numberOfCBJets(*jets);
  if(m_minCJet>-1 && nCJet<m_minCJet) return false;
  if(SYST==DGSys_NOM) n_pass_CJet[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passNBJet(const JetVector* jets){
  int nBJet= numberOfCBJets(*jets);
  if(m_minB20>-1 && nBJet<m_minB20) return false;
  if(m_maxB20>-1 && nBJet>m_maxB20) return false;
  if(SYST==DGSys_NOM) n_pass_NBJet[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passLead2JetsPt(const JetVector* jets){
  if(m_pTj0Min <0 && m_pTj1Min <0 ) return true; //cut not applied
  if(m_pTj0Min >-1 && jets->size()==1 && jets->at(0)->Pt() < m_pTj0Min) return false;

  if(jets->size()<2) return false;
  if(jets->at(0)->Pt() < m_pTj0Min) return false;
  if(jets->at(1)->Pt() < m_pTj1Min) return false;
  if(SYST==DGSys_NOM) n_pass_JetPt[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passMjj(const JetVector* jets){
  if(m_lowMjj <0 && m_highMjj <0 ) return true; //cut not applied 
  if(jets->size()<2) return false;
  float mjj=Mll(jets->at(0),jets->at(1));
  if(mjj < m_lowMjj) return false;
  if(mjj > m_highMjj) return false;
  if(SYST==DGSys_NOM) n_pass_mjj[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passZVeto(const LeptonVector* leptons, float Zlow, float Zhigh)
{
  if( leptons->size() < 2 ) return false;
  
  //Z veto on EE/MM/EM of OS
  bool hasz=false;
  bool OS = (leptons->at(0)->q*leptons->at(1)->q < 0) ? 1 : 0;
  float mll=Mll(leptons->at(0),leptons->at(1));
  if(mll>Zlow && mll<Zhigh && OS) hasz=true;
    
  if(m_vetoZ && hasz) return false;
  if(m_selZ){
    //hasz = hasZWindow(*leptons,Zlow, Zhigh); //has SFOS requirement
    if( !hasz) return false;
  }
  
  if(SYST==DGSys_NOM) n_pass_Z[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passMll(const LeptonVector* leptons)
{
  float mll=Mll(leptons->at(0),leptons->at(1));
  if(m_mllIn){  //reject inside mll window
    if(m_lowMll <0 && m_highMll<0) return true;
    if(mll > m_lowMll && mll < m_highMll) return false;
  }
  else { //upper or lower bound on mll
    if(m_lowMll>-1  && mll < m_lowMll) return false;
    if(m_highMll>-1 && mll > m_highMll) return false;
  }
  if(SYST==DGSys_NOM) n_pass_mll[m_ET][SR]+=_inc;

  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passMETRel(const Met *met, const LeptonVector* leptons, 
			     const JetVector* jets)
{
  float metRel = getMetRel(met,*leptons,*jets);
  if(m_metRelMin>=0 &&  metRel < m_metRelMin ) return false;
  if(m_metRelMax>=0 &&  metRel > m_metRelMax ) return false;
  if(SYST==DGSys_NOM) n_pass_metRel[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passLead2LepPt(const LeptonVector* leptons)
{
  if( leptons->size() < 1 ) return false;
  if(m_pTl0Min >-1 && leptons->at(0)->Pt() < m_pTl0Min) return false;
  if(m_pTl1Min >-1 && leptons->at(1)->Pt() < m_pTl1Min) return false;
  if(SYST==DGSys_NOM) n_pass_leadLepPt[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passMuoIso(const LeptonVector* leptons, bool useRelative)
{
  if( m_IsoMin>0 ){          
    for(uint ilep=0; ilep<leptons->size(); ilep++){
      const Susy::Lepton* _l = leptons->at(ilep);
      if(_l->isEle()) continue;
      float etcone30 = muEtConeCorr( (Muon*) _l, 
				     *v_baseEle, *v_baseMu, 
				     nt->evt()->nVtx, nt->evt()->isMC);
      if(useRelative) etcone30 /= _l->Pt();
      if(m_IsoMin>-1 && etcone30>m_IsoMin) return false;
    }
  }
  
  if(SYST==DGSys_NOM) n_pass_MuIso[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passEleD0S(const LeptonVector* leptons)
{
  if( m_d0SMin>0 ){
    for(uint ilep=0; ilep<leptons->size(); ilep++){
      const Susy::Lepton* _l = leptons->at(ilep);
      if(!_l->isEle()) continue;
      float d0S = fabs(_l->d0Sig(true));
      if(m_d0SMin>-1 && d0S>m_d0SMin) return false;
    }
  }

  if(SYST==DGSys_NOM) n_pass_EleD0S[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passPtll(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;
  TLorentzVector ll = (*leptons->at(0) + *leptons->at(1));
  if( m_pTllMin >-1 && ll.Pt() < m_pTllMin) return false;
  if( m_pTllMax >-1 && ll.Pt() > m_pTllMax) return false;
  if(SYST==DGSys_NOM) n_pass_pTll[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passPtllBound(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;
  TLorentzVector ll = (*leptons->at(0) + *leptons->at(1));
  float dRll = leptons->at(0)->DeltaR(*leptons->at(1));
  TF1* BoundLow = new TF1("expo","expo",1.5,4.0);
  BoundLow->FixParameter(0,6.11861);
  BoundLow->FixParameter(1,-11.18815e-01);
  float pTll_val = BoundLow->Eval(dRll);
  BoundLow->Delete();

  if( m_pTllBound && ll.Pt() < pTll_val ) return false;

  if(SYST==DGSys_NOM) n_pass_pTllBound[m_ET][SR]+=_inc;
  return true;
}

/*--------------------------------------------------------------------------------*/
bool SusySelection::passdPhill(const LeptonVector* leptons){
  if( leptons->size() < 2 ) return false;
  float dPhi = fabs(leptons->at(0)->DeltaPhi(*leptons->at(1)));
  if(m_dPhillMax>-1 && dPhi>m_dPhillMax) return false;
  if(SYST==DGSys_NOM) n_pass_dPhill[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passdRll(const LeptonVector* leptons){
  if( leptons->size() < 2 ) return false;
  float dR = leptons->at(0)->DeltaR(*leptons->at(1));
  if(m_dRllMin>-1 && dR<m_dRllMin) return false;
  if(m_dRllMax>-1 && dR>m_dRllMax) return false;

  if(SYST==DGSys_NOM) n_pass_dRll[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passMET(const Met *met)
{
  float etmiss = met->lv().Pt();
  if(m_metMin>=0 &&  etmiss < m_metMin ) return false;
  if(m_metMax>=0 &&  etmiss > m_metMax ) return false;
  if(SYST==DGSys_NOM) n_pass_met[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passDPhiMetll(const LeptonVector* leptons, const Met* met)
{
  TLorentzVector metlv = met->lv();
  TLorentzVector ll = (*leptons->at(0) + *leptons->at(1));
  if( m_dPhiMetll>-1 && !passdPhi(metlv, ll, m_dPhiMetll) ) return false;
  if(SYST==DGSys_NOM) n_pass_dPhiMetll[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passDPhiMetl1(const LeptonVector* leptons, const Met* met)
{
  TLorentzVector metlv = met->lv();
  TLorentzVector ll = *leptons->at(1);
  if( m_dPhiMetl1>-1 && !passdPhi(metlv, ll, m_dPhiMetl1) ) return false;
  if(SYST==DGSys_NOM) n_pass_dPhiMetl1[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passTopTagger(const LeptonVector* leptons, const JetVector* jets, const Met* met)
{
  if(m_topTag && !passTopTag(*leptons,*jets,met)) return false;
  if(SYST==DGSys_NOM) n_pass_topTag[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passdPhi(TLorentzVector v0, TLorentzVector v1, float cut)
{
  return v0.DeltaPhi(v1) > cut;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passDPhillJ0(const LeptonVector* leptons, const JetVector* jets){
  if( !(m_dPhillJ0Min>-1 || m_dPhillJ0Max>-1)) return true; //cut not apply
  if( leptons->size() < 2 ) return false;
  if(jets->size()==0) return false;

  TLorentzVector ll = (*leptons->at(0) + *leptons->at(1));
  float dPhi = fabs(ll.DeltaPhi(*jets->at(0)));

  if(m_dPhillJ0Min>-1 && dPhi<m_dPhillJ0Min) return false;
  if(m_dPhillJ0Max>-1 && dPhi>m_dPhillJ0Max) return false;
  if(SYST==DGSys_NOM) n_pass_dPhillJ0[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passDPhillMET(const LeptonVector* leptons, const Met* met){
  if( !(m_dPhillMetMin>-1 || m_dPhillMetMax>-1)) return true; //cut not apply
  if( leptons->size() < 2 ) return false;

  TLorentzVector ll = (*leptons->at(0) + *leptons->at(1));
  float dPhi = fabs(ll.DeltaPhi(met->lv()));

  if(m_dPhillMetMin>-1 && dPhi<m_dPhillMetMin) return false;
  if(m_dPhillMetMax>-1 && dPhi>m_dPhillMetMax) return false;
  if(SYST==DGSys_NOM) n_pass_dPhillMet[m_ET][SR]+=_inc;
  return true;
}

/*--------------------------------------------------------------------------------*/
bool SusySelection::passMeff(const JetVector* jets, const Met* met){
  float mEff = Meff(*jets,met,JET_PT_CUT);
  
  if(m_MeffMin>-1 && mEff<m_MeffMin) return false;
  if(m_MeffMax>-1 && mEff>m_MeffMax) return false;
  if(SYST==DGSys_NOM) n_pass_Meff[m_ET][SR]+=_inc;
  return true;

}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passMetMeff(const LeptonVector* leptons,const JetVector* jets, const Met* met, bool useLepton){

  float mEff;
  if(useLepton) mEff=Meff(*leptons,*jets,met,JET_PT_CUT);
  else mEff = Meff(*jets,met,JET_PT_CUT);
  float ratio = (met->lv().Pt()>0) ?  met->lv().Pt()/mEff: 0;
  
  if(m_MetMeffMin>-1 && ratio<m_MetMeffMin) return false;
  if(m_MetMeffMax>-1 && ratio>m_MetMeffMax) return false;
  if(SYST==DGSys_NOM) n_pass_MetMeff[m_ET][SR]+=_inc;
  return true;

}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passHT(const LeptonVector* leptons,const JetVector* jets, const Met* met){
  float ht = Meff(*leptons,*jets,met,JET_PT_CUT);
  
  if(m_HTMin>-1 && ht<m_HTMin) return false;
  if(m_HTMax>-1 && ht>m_HTMax) return false;
  if(SYST==DGSys_NOM) n_pass_HT[m_ET][SR]+=_inc;
  return true;

}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passMT2(const LeptonVector* leptons, const Met* met)
{
  float mT2 = getMT2(*leptons, met);
  if(m_mt2Min>-1 && mT2<m_mt2Min) return false;
  if(m_mt2Max>-1 && mT2>m_mt2Max) return false;
  if(SYST==DGSys_NOM) n_pass_mt2[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passMinMT(const LeptonVector* leptons, const Met* met)
{
  if( leptons->size() < 2 ) return false;
  float mT1 = Mt(leptons->at(0), met);
  float mT2 = Mt(leptons->at(1), met);
  float minMt = TMath::Min(mT1,mT2);
  if(m_mtMin>-1 && minMt<m_mtMin) return false;
  if(m_mtMax>-1 && minMt>m_mtMax) return false;
  if(SYST==DGSys_NOM) n_pass_minMt[m_ET][SR]+=_inc;
  return true;
}

/*--------------------------------------------------------------------------------*/
bool SusySelection::passMWWT(const LeptonVector* leptons, const Met* met)
{
  if( leptons->size() < 2 ) return false;
  TLorentzVector _ll = (*leptons->at(0) + *leptons->at(1));
  float mWT = mT(_ll, met->lv());
  
  if(m_lowMTWW>-1 && mWT<m_lowMTWW) return false;
  if(m_highMTWW>-1 && mWT>m_highMTWW) return false;
  if(SYST==DGSys_NOM) n_pass_mWWT[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passMll20(const LeptonVector* leptons)
{
  float mll=Mll(leptons->at(0),leptons->at(1));
  if(mll<20) return false;
  return true;
}

/*--------------------------------------------------------------------------------*/
bool SusySelection::passSFOSCut(const LeptonVector* leptons)
{
  bool sfos = hasSFOS(*leptons);
  if(m_vetoSFOS &&  sfos) return false;
  if(m_selSFOS  && !sfos) return false;
  if(SYST==DGSys_NOM)  n_pass_sfos[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::passZCut(const LeptonVector* leptons)
{
  if(m_vetoZ || m_selZ || m_vetoExtZ){
    bool z = hasZ(*leptons, 10., m_vetoExtZ);
    if(m_vetoZ && z) return false;
    if(m_selZ && !z) return false;
    if(m_vetoExtZ && z) return false;
  }
  if(SYST==DGSys_NOM)  n_pass_Z[m_ET][SR]+=_inc;
  return true;
}

/*--------------------------------------------------------------------------------*/
bool SusySelection::passZllZll(const LeptonVector* leptons)
{
  uint dmy;
  bool ZZ = hasZllZll(*leptons,&dmy,&dmy,&dmy,&dmy);
  if(m_selZllZll && !ZZ) return false;

  if(SYST==DGSys_NOM)  n_pass_ZllZll[m_ET][SR]+=_inc;
  return true;

}

/*--------------------------------------------------------------------------------*/
bool SusySelection::passMtCut(const LeptonVector* leptons, const Met* met)
{
  if(m_mtMin > 0 || m_mtMax > 0){
    uint nLep = leptons->size();
    uint zl1, zl2;
    // Find the best Z candidate pair, use remaining lepton to form Mt
    if(nLep==3 && findBestZ(zl1, zl2, *leptons)){
      for(uint iL=0; iL<nLep; iL++){
        if(iL!=zl1 && iL!=zl2){
          // Only process the leading non-Z lepton
          // Is this how we want to apply it generally?
          float mt = Mt(leptons->at(iL), met);
          if(m_mtMin > 0 && mt < m_mtMin) return false;
          if(m_mtMax > 0 && mt > m_mtMax) return false;
          break;
        }
      }
    }
    // For 2 leptons, use maximum mt
    else if(nLep==2){
      float mt = max(Mt(leptons->at(0), met), Mt(leptons->at(1), met));
      if(m_mtMin > 0 && mt < m_mtMin) return false;
      if(m_mtMax > 0 && mt > m_mtMax) return false;
    }
    // General case, use all leptons
    else{
      for(uint iL=0; iL<nLep; iL++){
        float mt = Mt(leptons->at(iL), met);
        if(m_mtMin > 0 && mt < m_mtMin) return false;
        if(m_mtMax > 0 && mt > m_mtMax) return false;
      }
    }
  }
  if(SYST==DGSys_NOM) n_pass_mt3L[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
float SusySelection::JZBJet(const JetVector* jets, const LeptonVector* leptons)
{
  TVector2 sumPtJet(0,0);
  TLorentzVector Z = (*leptons->at(0) + *leptons->at(1));

  for(uint i=0; i<jets->size(); ++i){
   const Jet* jet = jets->at(i);
   TVector2 v_pt(jet->Px(),jet->Py());
   sumPtJet += v_pt;
  }

  return fabs(sumPtJet.Mod())-fabs(Z.Pt());
}
/*--------------------------------------------------------------------------------*/
float SusySelection::JZBEtmiss(const Met *met, const LeptonVector* leptons)
{
  TLorentzVector Z = (*leptons->at(0) + *leptons->at(1));
  TVector2 Z_2(Z.Px(), Z.Py());
  TVector2 met_2(-met->lv().Px(), -met->lv().Py());
  TVector2 vv = met_2 - Z_2; //-met->lv() - Z;

  return fabs(vv.Mod())-fabs(Z.Pt());

}
/*--------------------------------------------------------------------------------*/
bool  SusySelection::passSFOSLooseLepton(SusyNtObject* susyNt, const LeptonVector leptons, 
					 float minMll, float maxMll)
{
  if(!m_vetoLooseSFOSinZ) return true;

  getPreLeptons(susyNt);
  bool hasSFOSinZ=false;
  LeptonVector candLooseLep = findSFOSinZ(&v_preLep, &leptons, hasSFOSinZ, minMll, maxMll);

  if(dbg()>0 && candLooseLep.size()>0){
    cout << "Found loose lepton SFOS in Z " << candLooseLep.size() << endl;
  }

  if(hasSFOSinZ) return false;
  if(SYST==DGSys_NOM) n_pass_looseSFOSinZ[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusySelection::emulateWW(LeptonVector* leptons, Met* met)
{
  //Pick randomly the lepton to transform into v
  // maintain that the 2 remaining leptons are OS
  uint idx_l1=-999;
  uint idx_l2=-999;
  bool z = hasZ(*leptons, &idx_l1, &idx_l2, 10., m_vetoExtZ);
  if(!z) {
    cout << "no Z"  << endl;
    return false; //Cannot emulate, not WZ event
  }

  uint idx_l3=-999;
  if(idx_l1==0 && idx_l2==1) idx_l3=2;
  else if(idx_l1==0 && idx_l2==2) idx_l3=1;
  else if(idx_l1==1 && idx_l2==0) idx_l3=2;
  else if(idx_l1==1 && idx_l2==2) idx_l3=0;
  else if(idx_l1==2 && idx_l2==0) idx_l3=1;
  else if(idx_l1==2 && idx_l2==1) idx_l3=0;
  if(idx_l3<0){
    cout << "l3 idx " << endl;
    return false;
  }

  TLorentzVector Z = (*leptons->at(idx_l1) + *leptons->at(idx_l2));
  uint idx_l=-999;
  
  if((*leptons->at(idx_l3)).q<0)
    idx_l = ((*leptons->at(idx_l1)).q<0)? idx_l1 : idx_l2;
  if((*leptons->at(idx_l3)).q>0)
    idx_l = ((*leptons->at(idx_l1)).q>0)? idx_l1 : idx_l2;
  
  if(idx_l<0){
    cout << "idx l " << endl;
    return false;
  }

  //randomly pick between idx_l and idx_l3
  //float x = _random->Rndm();
  //Pick the lepton from the Z, maintaining OS events
  uint idx_pick = idx_l; //(x<0.5)? idx_l3 : idx_l;

  if(dbg()>5){
    cout << "\nEmul WW run: " << nt->evt()->run  << " event: " << nt->evt()->event  << endl;
    cout << "  Z " << Z.M() 
	 << " l1 idx " << idx_l1 << " q " << (*leptons->at(idx_l1)).q << " " << (*leptons->at(idx_l1)).Pt() 
	 << "   l2 idx " << idx_l2 << " q " << (*leptons->at(idx_l2)).q << " " << (*leptons->at(idx_l2)).Pt() 
	 << "   l3 idx " << idx_l3 << " q " << (*leptons->at(idx_l3)).q << " " << (*leptons->at(idx_l3)).Pt() 
	 << endl;
    cout << "  pick idx " << idx_pick << " pt " << (*leptons->at(idx_pick)).Pt()
	 << " org met " << met->lv().Pt() << " " << met->lv().Phi() << endl;
  }

  //Add lepton to met
  float met_px = met->lv().Px() - (*leptons->at(idx_pick)).Px();
  float met_py = met->lv().Py() - (*leptons->at(idx_pick)).Py();

  met->Et = sqrt(pow(met_px,2) + pow(met_py,2));
  met->phi = atan(met_py/met_px);
  

  //Reset TLV of the picked lepton
  Susy::Lepton* _l = leptons->at(idx_pick);
  _l->pt=0; _l->eta=0; _l->phi=0; _l->m=0;
  _l->resetTLV();

  std::sort(leptons->begin(), leptons->end(), comparePt);

  if(dbg()>5){
    cout << "  new met " << met->lv().Pt() << " " << met->lv().Phi() << endl;
    cout << "   reset lepton " << idx_pick << " " << (*leptons->at(idx_pick)).Pt() << endl; 
  }

  //leptons->erase(leptons->begin()+idx_pick);//not comptible w/ save-restore original
  
  return true;
}


/*--------------------------------------------------------------------------------*/
uint SusySelection::get3LType(const LeptonVector& leptons)
{
  int ne=0;
  int nm=0;

  for(uint i=0; i<leptons.size(); ++i){
    bool isE = leptons.at(i)->isEle();
    if(isE)  ne++;
    if(!isE) nm++;
  }

  if(ne==3 && nm ==0) return ET_eee;
  else if(ne==2 && nm ==1) return ET_eem;
  else if(ne==1 && nm ==2) return ET_emm;
  else if(ne==0 && nm ==3) return ET_mmm;
  

  return ET_Ukn;

}
/*--------------------------------------------------------------------------------*/
uint SusySelection::get4LType(const LeptonVector& leptons)
{
  int ne=0;
  int nm=0;
  uint maxNLep=leptons.size();
  if(maxNLep>4) maxNLep=4;
  for(uint i=0; i<maxNLep; ++i){
    bool isE = leptons.at(i)->isEle();
    if(isE)  ne++;
    if(!isE) nm++;
  }

  if(ne==4 && nm ==0) return ET_eeee;
  else if(ne==3 && nm ==1) return ET_eeem;
  else if(ne==2 && nm ==2) return ET_eemm;
  else if(ne==1 && nm ==3) return ET_emmm;
  else if(ne==0 && nm ==4) return ET_mmmm;


  return ET_Ukn;
}


/*--------------------------------------------------------------------------------*/
void SusySelection::sumArray(){
  
  //3L
  for(int i=ET_eee; i<ET_lll; i++){
    n_pass_nLep3[ET_lll]+= n_pass_nLep3[i];
    n_pass_tauVeto[ET_lll]+= n_pass_tauVeto[i];
   
    for(int iSR=0; iSR<ML_VRZZ; iSR++){
      n_pass_3Ltrig[ET_lll][iSR]+= n_pass_3Ltrig[i][iSR]; 
      n_pass_sfos[ET_lll][iSR]+= n_pass_sfos[i][iSR]; 
      n_pass_met[ET_lll][iSR]+= n_pass_met[i][iSR]; 
      n_pass_Z[ET_lll][iSR]+= n_pass_Z[i][iSR];
      n_pass_ZllZll[ET_lll][iSR]+= n_pass_ZllZll[i][iSR]; 
      n_pass_BJet[ET_lll][iSR]+= n_pass_BJet[i][iSR]; 
      n_pass_mt3L[ET_lll][iSR]+= n_pass_mt3L[i][iSR];
      n_pass_NBJet[ET_lll][iSR]+= n_pass_NBJet[i][iSR]; 
    }
  }


  //4L
  for(int i=ET_eeee; i<ET_llll; i++){
    n_pass_nLep4[ET_llll]+= n_pass_nLep4[i];
    
    for(int iSR=ML_VRZZ; iSR<ML_NSR; iSR++){
      n_pass_3Ltrig[ET_llll][iSR]+= n_pass_3Ltrig[i][iSR]; 
      n_pass_met[ET_llll][iSR]+= n_pass_met[i][iSR]; 
      n_pass_Z[ET_llll][iSR]+= n_pass_Z[i][iSR]; 
      n_pass_ZllZll[ET_llll][iSR]+= n_pass_ZllZll[i][iSR]; 
      n_pass_BJet[ET_llll][iSR]+= n_pass_BJet[i][iSR]; 
      n_pass_NBJet[ET_llll][iSR]+= n_pass_NBJet[i][iSR]; 
    }
  }

}


/*--------------------------------------------------------------------------------*/
// Select leptons pT>6GeV
/*--------------------------------------------------------------------------------*/
void SusySelection::getPreLeptons(Susy::SusyNtObject* susyNt)
{
  ElectronVector preElecs;
  for(uint ie=0; ie<susyNt->ele()->size(); ++ie){
    Electron* e = & susyNt->ele()->at(ie);
    if(e->Pt() < 6.0) continue;
    preElecs.push_back(e);
  }

  MuonVector preMus;
  for(uint im=0; im<susyNt->muo()->size(); ++im){
    Muon* mu = & susyNt->muo()->at(im);
    if(mu->Pt() < 6.0) continue;
    preMus.push_back(mu);
  }

  v_preLep.clear();
  buildLeptons(v_preLep, preElecs, preMus);

}
/*--------------------------------------------------------------------------------*/
// Select Loose leptons 
/*--------------------------------------------------------------------------------*/
LeptonVector SusySelection::getLooseLeptons(LeptonVector* preLeptons,
					    const LeptonVector*  leptons)
{
  LeptonVector looseLeptons;

  for(uint ilep=0; ilep<preLeptons->size(); ++ilep){
    bool overlap=false;
    if(dbg()>0) cout << "prelep pT " << preLeptons->at(ilep)->Pt() << endl;
    for(uint slep=0; slep<leptons->size(); ++slep){
      if(preLeptons->at(ilep) == leptons->at(slep)) overlap=true;
    }
    
    if(overlap) continue; //Not signal/baseline lepton
    if(!passBasicLeptonSelection(preLeptons->at(ilep)) )  continue; //pass IP
    
    float mindR=999;//Not within 0.05 of signal/baseline leptons
    for(uint slep=0; slep<leptons->size(); ++slep){
      if( (preLeptons->at(ilep)->isEle()==leptons->at(slep)->isEle()) ||
	  (preLeptons->at(ilep)->isMu()==leptons->at(slep)->isMu()) )
	mindR = preLeptons->at(ilep)->DeltaR(*leptons->at(slep));
    }
    if(mindR<E_E_DR) {
      //cout << " \t too close DR "<< mindR << endl; 
      continue;
    }
    
    if(dbg()>0) cout << " looseLep " << preLeptons->at(ilep)->Pt() << endl;
    looseLeptons.push_back(static_cast<Lepton*>(preLeptons->at(ilep)));
  }
  
  return looseLeptons;
}

/*--------------------------------------------------------------------------------*/
// Check pass IP cuts 
/*--------------------------------------------------------------------------------*/
bool SusySelection::passBasicLeptonSelection(const Lepton* l)
{
  if(l->isEle()){
    if(fabs(l->d0Sig(true)) >= ELECTRON_D0SIG_CUT_WH) return false;
    if(fabs(l->z0SinTheta(true)) >= ELECTRON_Z0_SINTHETA_CUT) return false;
  }
  else if(l->isMu()){
    if(fabs(l->d0Sig(true)) >= MUON_D0SIG_CUT) return false;
    if(fabs(l->z0SinTheta(true)) >= MUON_Z0_SINTHETA_CUT) return false;
  }

  return true;
}

/*--------------------------------------------------------------------------------*/
// find loose leptons that have SFOS w/ input leptons and within mass window
/*--------------------------------------------------------------------------------*/
LeptonVector SusySelection::findSFOSinZ(LeptonVector* preLeptons, const LeptonVector* leptons, 
					bool &hasSFOSinZ, float minMll, float maxMll){
  
  hasSFOSinZ=false;
  LeptonVector candLooseLeptons;

  if(dbg()>0) cout << "Input prelepton " << preLeptons->size() << endl;

  LeptonVector looseLeptons = getLooseLeptons(preLeptons, leptons);

  for(uint ilep=0; ilep<looseLeptons.size(); ++ilep){
    for(uint slep=0; slep<leptons->size(); ++slep){
      if(!isSFOS(looseLeptons.at(ilep), leptons->at(slep))) continue;
      if(isZWindow(looseLeptons.at(ilep), leptons->at(slep), minMll, maxMll)){
	hasSFOSinZ=true;
	float mll = Mll(looseLeptons.at(ilep), leptons->at(slep)); 
	if(dbg()>0)
	  cout << "\t\t sigLep " << leptons->at(slep)->Pt() 
	       << "\t\t candLoose " << looseLeptons.at(ilep)->Pt()
	       << "\t\t mll " <<  mll 
	       << endl;
	candLooseLeptons.push_back(static_cast<Lepton*>(looseLeptons.at(ilep)));
      }
    }
  }

  if(dbg()>0) cout << " \t output cand preLepton " << candLooseLeptons.size() << endl;
  return candLooseLeptons;

}
