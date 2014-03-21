#ifndef SusyWeakProdAna_SusySelection_h
#define SusyWeakProdAna_SusySelection_h

//////////////////////////////////////////////////////////
// General script to implement basic selection with all //
// signal region cut methods.                           //
//////////////////////////////////////////////////////////

#include <fstream>

// Root Packages
#include "TRandom3.h"

// Susy Common
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtObject.h"
#include "SusyNtuple/SusyNtTools.h"
#include "SusyNtuple/DilTrigLogic.h"
#include "SusyNtuple/TrilTrigLogic.h"
#include "SusyNtuple/SleptonXsecReader.h"

#include "ChargeFlip/chargeFlip.h"

#include "SusyWeakProdAna/SusyHistos.h"

static const int LEP_N = 15;    //2L, 3L, 4L channels rounded up
static const int SR_N  = 100;   //Large enought to cover all the SR/CR/VR's for all ana


enum LEP_TYPE{PR=0, CONV, HF, LF, TYPE_Undef};

class SusySelection: public SusyNtTools
{
 public:
  
  SusySelection(bool is2LAna=false, bool qFlipd0=true);
  virtual ~SusySelection(){};
  
  // Debug level
  void setDebug(int dbg) { m_dbg = dbg; }
  int  dbg() { return m_dbg; }
  
  
  // Reset flags & var
  void resetCuts();
  void reset();
  void resetCounter();
  
  // Event Cleaning
  bool passEventCleaning();
  bool passBadFCAL(const JetVector* jets, int run, bool isMC=false);
  
  // Cut methods
  bool passMll20(const LeptonVector* leptons);
  bool passTrigger(const LeptonVector* leptons, DilTrigLogic* trigObj, const Met *met);
  bool pass3LTrigger(const LeptonVector* leptons, const TauVector* taus, TrilTrigLogic* trig3LObj);
  bool passTauVeto(const TauVector* taus);
  bool passNLepCut(const LeptonVector* leptons);
  bool passNBase3LepCut(const LeptonVector* leptons);
  bool passNBase4LepCut(const LeptonVector* leptons);
  bool passNLep3Cut(const LeptonVector* leptons);
  bool passNLep4Cut(const LeptonVector* leptons);
  bool passIsPromptLepton(const LeptonVector* leptons, int method, bool isMC=false);
  bool passFlavor(const LeptonVector* leptons);
  bool passQQ(const LeptonVector* leptons);
  bool passFullJetVeto(const JetVector* jets);
  bool passZVeto(const LeptonVector* leptons, bool useOS=true, float Zlow = MZ-10, float Zhigh = MZ+10);
  bool passMETRel(const Met *met, const LeptonVector* leptons, const JetVector* jets);
  bool passMET(const Met *met);
  bool passFJet(const JetVector* jets);
  bool passBJet(const JetVector* jets);
  bool passLJet(const JetVector* jets);
  bool passCentralJet(const JetVector* jets);
  bool passNBJet(const JetVector* jets);
  bool passLead2JetsPt(const JetVector* jets);
  bool passMjj(const JetVector* jets);
  bool passMljj(const LeptonVector* leptons,const JetVector* jets); //if only 1-jet reco mlj
  bool passMT2(const LeptonVector* leptons, const Met* met);
  bool passMaxMT(const LeptonVector* leptons, const Met* met);
  bool passMWWT(const LeptonVector* leptons, const Met* met);
  bool passMll(const LeptonVector* leptons);
  bool passTopTagger(const LeptonVector* leptons, const JetVector* jets, const Met* met);
  bool passLead2LepPt(const LeptonVector* leptons);
  bool passMuoIso(const LeptonVector* leptons, bool useRelative=true);
  bool passEleD0S(const LeptonVector* leptons);
  bool passPtll(const LeptonVector* leptons);
  bool passPtllBound(const LeptonVector* leptons);
  bool passDPhiMetll(const LeptonVector* leptons, const Met* met);
  bool passDPhiMetl1(const LeptonVector* leptons, const Met* met);
  bool passdPhi(TLorentzVector v0, TLorentzVector v1, float cut);
  bool passdPhill(const LeptonVector* leptons);
  bool passdEtall(const LeptonVector* leptons);
  bool passdRll(const LeptonVector* leptons);
  bool passDPhillJ0(const LeptonVector* leptons, const JetVector* jets);
  bool passDPhillMET(const LeptonVector* leptons, const Met* met);
  bool passMetMeff(const LeptonVector* leptons,const JetVector* jets, const Met* met, bool useLepton=false);
  bool passMeff(const JetVector* jets, const Met* met);
  bool passHT(const LeptonVector* leptons,const JetVector* jets, const Met* met);
  bool passSFOSLooseLepton(SusyNtObject* susyNt, const LeptonVector leptons, 
			   float minMll=MZ-20, float maxMll=MZ+20);
  


  //SS & charge flip 
  bool  isGenuineSS(const LeptonVector* leptons, bool isMC);
  bool  hasQFlip(const LeptonVector* leptons);
  float getQFlipProb(LeptonVector* leptons, Met* met, uint iSys=DGSys_NOM);
  
  //3L/4L specific
  bool passSFOSCut(const LeptonVector* leptons);
  bool passZCut(const LeptonVector* leptons);
  bool passZllZll(const LeptonVector* leptons);
  bool passMtCut(const LeptonVector* leptons, const Met* met);

  //Complex vars (move to another class?)
  float JZBJet(const JetVector* jets, const LeptonVector* leptons);
  float JZBEtmiss(const Met *met, const LeptonVector* leptons);
   
  //Emulation WW from WZ: 1 lepton -> neutrino
  bool emulateWW(LeptonVector* leptons, Met* met);

  //Multilep classification
  uint get3LType(const LeptonVector& leptons);
  uint get4LType(const LeptonVector& leptons);

  //Sum 3L & 4L 
  void sumArray();

  //Functions to find loose lepton reconstructing Z
  LeptonVector findSFOSinZ(LeptonVector* preLeptons, const LeptonVector* leptons, 
			   bool &hasSFOSinZ, float minMll=MZ-20, float maxMll=MZ+20);
  void         getPreLeptons(Susy::SusyNtObject* susyNt);
  LeptonVector getLooseLeptons(LeptonVector* preLeptons,const LeptonVector*  leptons); 
  bool         passBasicLeptonSelection(const Lepton* l);

  //Lepton origin
  bool  isPT(const Lepton* lep);
  bool  isConv(const Lepton* lep);
  bool  isLF(const Lepton* lep);
  bool  isHF(const Lepton* lep);
  bool  isQFlip(const Lepton* lep);
  LEP_TYPE  getType(const Lepton* lep);
  




  ClassDef(SusySelection, 1);
  
 protected:
  TRandom3* _random;
  Susy::SusyNtObject* nt;
  
  int m_dbg;                  // debug level
  
  //External packages
  chargeFlip*         m_chargeFlip;
  
  //containers - from SusyNt
  ElectronVector*      v_preEle;      //selected electrons before ORing
  MuonVector*          v_preMu;       //selected muons before ORing
  LeptonVector         v_preLep;      //selected lepton (sum of ele+mu pT>6)
  JetVector*           v_preJet;      //selected jets before ORing
  ElectronVector*      v_baseEle;     // baseline electrons
  ElectronVector*      v_sigEle;      // signal electrons
  MuonVector*          v_baseMu;      // baseline muons
  MuonVector*          v_sigMu;       // signal muons
  LeptonVector*        v_baseLep;     // baseline leptons
  LeptonVector*        v_sigLep;      // signal leptons
  JetVector*           v_baseJet ;    // signal jets
  JetVector*           v_sigJet;      // signal jets
  TauVector*           v_baseTau;     // base Taus
  TauVector*           v_sigTau;      // signal Taus
  const Susy::Met*     m_met;         // Met
  
  //These are save copy of the original values from SusyNt
  //Use to restore to original within the SR's loop
  //Since for SS region, Ele pt, met are modified, or emulation of WW from WZ events
  ElectronVector       v_save_sigEle;  // signal electrons 
  MuonVector           v_save_sigMu;   // signal muon 
  LeptonVector         v_save_sigLep;  // baseline leptons 
  Susy::Met            new_met;       // original met
  
  //Event variables
  float _inc;          //To set counter inc to 1 or _ww.
  uint   SR;           //Signal region
  uint   SYST;         //Current Syst being handled
  
  // Cut variables
  bool                m_cutNBaseLep;  // apply nLep cuts to baseline leptons as well as signal
  uint                m_nLepMin;      // min 2 leptons
  uint                m_nLepMax;      // max 2 leptons
  uint                m_nLep3Min;     // min leptons
  uint                m_nLep3Max;     // max leptons
  uint                m_nLep4Min;     // min leptons
  uint                m_nLep4Max;     // max leptons
  
  bool                m_selOS;        // select OS 
  bool                m_selSS;        // select SS 
  bool                m_selSF;        // select SF (no flavor sel by default)
  bool                m_selOF;        // select OF (no flavor sel by default)
  bool                m_vetoSF;       // veto SF
  bool                m_selSFOS;      // switch to select SFOS pairs
  bool                m_vetoSFOS;     // switch to veto SFOS pairs
  bool                m_selZ;         // select Z (SFOS applied)
  bool                m_vetoZ;        // veto z
  bool                m_vetoExtZ;     // extended multilepton Z veto
  bool                m_selZllZll;    // select 2 SFOS pair within Z
  bool                m_selB;         // sel  b-jet using m_btagPtMin;
  bool                m_vetoB;        // veto b-jet using m_btagPtMin
  bool                m_vetoF;        // veto fwd-jet using
  bool                m_vetoJ;        // Full jet veto
  int                 m_minC20;       // min number of C20
  int                 m_maxC20;       // max number of C20
  int                 m_minCJet;      // min number of central C20+B20
  int                 m_minB20;       // min number of B20
  int                 m_maxB20;       // max number of B20
  float               m_metMin;       // min Met
  float               m_metMax;       // max Met
  float               m_metRelMin;    // min MetRel
  float               m_metRelMax;    // max MetRel
  bool                m_topTag;       // use top tagger
  float               m_mt2Min;       // min MT2 
  float               m_mt2Max;       // max MT2 
  float               m_mtMin;        // min MT 
  float               m_mtMax;        // max MT 
  float               m_mtMaxLow;     // max MT lower bound
  float               m_mtMaxHigh;    // max MT upper bound 
  float               m_lepLeadPtMin; // lead lepton pt min 
  float               m_pTl0Min;      // min pT (leading lepton)
  float               m_pTl1Min;      // min pT (second leading lepton)
  float               m_pTl1Max;      // max pT (second leading lepton)
  float               m_IsoMin;       // min iso value
  float               m_d0SMin;       // min d0signif value
  float               m_pTllMin;      // min Ptll
  float               m_pTllMax;      // max Ptll
  bool                m_pTllBound;    // Ptll boundary 2D function
  float               m_lowMll;       // low Mll bound
  float               m_highMll;      // high Mll bound
  bool                m_mllIn;        // Apply mll cut rejecting event inside
  float               m_dPhillMax;    // max dPhi(ll) 
  float               m_dPhillMin;    // min dPhi(ll) 
  float               m_dEtallMax;    // max dPhi(ll) 
  float               m_dRllMin;      // min dR(ll) 
  float               m_dRllMax;      // max dR(ll) 
  float               m_lowMjj;       // min Mjj (leading jets)
  float               m_highMjj;      // max Mjj (leading jets)
  float               m_highMljj;     // max Mljj (dijet system closest to one lepton)
  float               m_lowMljj;     // max Mljj (dijet system closest to one lepton)
  float               m_lowMTWW;      // min MT(ll,Etmiss)
  float               m_highMTWW;     // max MT(ll,Etmiss)
  float               m_pTj0Min;      // min pT (leading jets)
  float               m_pTj1Min;      // min pT (second leading jets)
  float               m_dPhiMetll;    // dphi Met & ll
  float               m_dPhiMetl1;    // dphi Met & l1
  float               m_dPhillJ0Min;  // min dPhi ll & lead Jet
  float               m_dPhillJ0Max;  // max dPhi ll & lead Jet
  float               m_dPhillMetMin; // min dPhi ll & met
  float               m_dPhillMetMax; // max dPhi ll & met
  float               m_MetMeffMin;   // min Met/Meff
  float               m_MetMeffMax;   // min Met/Meff
  float               m_MeffMin;      // min Meff
  float               m_MeffMax;      // min Meff
  float               m_HTMin;        // min HT
  float               m_HTMax;        // min HT
  bool                m_vetoLooseSFOSinZ;  // veto SF
  
  //DiLepEvtType        m_ET;           // Dilepton event type to store cf
  uint       m_ET;           // Multilep event type to store cf
  
  
  // Event counters
  float                n_readin;
  float                n_pass_SUSYGrid;
  float                n_pass_GRL;
  float                n_pass_LarErr;
  float                n_pass_TileErr;
  float                n_pass_TTCVeto;
  float                n_pass_GoodVtx;
  float                n_pass_TileTrip;
  float                n_pass_HotSpot;
  float                n_pass_BadJet;
  float                n_pass_BadMuon;
  float                n_pass_Cosmic;
  float                n_pass_BadFCAL;
  float                n_pass_DeadRegion;
  float                n_pass_atleast2BaseLep; //>=2 base lept
  float                n_pass_exactly2BaseLep; //=2 base lept
  float                n_pass_mll20;           //mll>20 SS/OS all flavors
  float                n_pass_nBase3Lep;
  float                n_pass_nBase4Lep;
  
  float                n_pass_dil[LEP_N];   //Channel
  float                n_pass_nLep3[LEP_N];
  float                n_pass_nLep4[LEP_N];
  float                n_pass_tauVeto[LEP_N];
  float                n_pass_signalLep[LEP_N];
  float                n_pass_trig[LEP_N];
  float                n_pass_truth[LEP_N];
  float                n_pass_3Ltrig[LEP_N][SR_N];
  float                n_pass_os[LEP_N][SR_N];
  float                n_pass_ss[LEP_N][SR_N];
  float                n_pass_flav[LEP_N][SR_N];
  float                n_pass_Z[LEP_N][SR_N];  //Z sel or veto
  float                n_pass_ZllZll[LEP_N][SR_N];  //Z sel or veto
  float                n_pass_FullJveto[LEP_N][SR_N];  //Full jet veto
  float                n_pass_FJet[LEP_N][SR_N]; //Forward jet
  float                n_pass_BJet[LEP_N][SR_N]; //b-jet
  float                n_pass_LJet[LEP_N][SR_N]; //central LF
  float                n_pass_CJet[LEP_N][SR_N]; //central C20+B20
  float                n_pass_NBJet[LEP_N][SR_N]; //b-jets
  float                n_pass_JetPt[LEP_N][SR_N];
  float                n_pass_mjj[LEP_N][SR_N];
  float                n_pass_mljj[LEP_N][SR_N];
  float                n_pass_leadLepPt[LEP_N][SR_N];
  float                n_pass_MuIso[LEP_N][SR_N];
  float                n_pass_EleD0S[LEP_N][SR_N];
  float                n_pass_mll[LEP_N][SR_N]; //Cut on  mll
  float                n_pass_pTll[LEP_N][SR_N];
  float                n_pass_pTllBound[LEP_N][SR_N];
  float                n_pass_dPhill[LEP_N][SR_N];
  float                n_pass_dEtall[LEP_N][SR_N];
  float                n_pass_dRll[LEP_N][SR_N];
  float                n_pass_mWWT[LEP_N][SR_N];
  float                n_pass_topTag[LEP_N][SR_N];
  float                n_pass_metRel[LEP_N][SR_N];
  float                n_pass_mt2[LEP_N][SR_N];
  float                n_pass_maxMt[LEP_N][SR_N];
  float                n_pass_met[LEP_N][SR_N];
  float                n_pass_dPhiMetll[LEP_N][SR_N];
  float                n_pass_dPhiMetl1[LEP_N][SR_N];
  float                n_pass_dPhillJ0[LEP_N][SR_N];
  float                n_pass_dPhillMet[LEP_N][SR_N];
  float                n_pass_MetMeff[LEP_N][SR_N];
  float                n_pass_Meff[LEP_N][SR_N];
  float                n_pass_HT[LEP_N][SR_N];
  float                n_pass_looseSFOSinZ[LEP_N][SR_N];
  
  float                n_pass_sfos[LEP_N][SR_N];
  float                n_pass_mt3L[LEP_N][SR_N];

};

#endif
