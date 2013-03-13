#ifndef SusyWeakProdAna_Susy2LepAna_h
#define SusyWeakProdAna_Susy2LepAna_h

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
#include "SusyNtuple/SleptonXsecReader.h"

#include "SusyMatrixMethod/DiLeptonMatrixMethod.h"
#include "ChargeFlip/chargeFlip.h"

#include "HistFitterTree/HistFitterTree.h"

//SusyWeakProdAna 

#include "SusyWeakProdAna/SUSYCrossSection.h"
#include "SusyWeakProdAna/SusyHistos.h"

#include "SusyWeakProdAna/ToyNt.h"


class Susy2LepAna: public SusyNtTools
{
  public:

    Susy2LepAna(SusyHistos* _histos);
    virtual ~Susy2LepAna(){};

    ofstream out;
    
    // Debug level
    void setDebug(int dbg) { m_dbg = dbg; }
    int dbg() { return m_dbg; }
    void setUseLooseLep(bool b) { 
      if(b) cout << ">>> Using base leptons !!! "  << endl;
      m_useLooseLep = b; 
    }
    void setMethod(int m){
      m_method = m;
      cout << ">>> Using method " << SMETHOD[m_method] << endl;
    }

    void hookContainers(Susy::SusyNtObject* _ntPtr,
			ElectronVector* _baseEleA, ElectronVector* _sigEleA,
			MuonVector*     _baseMuoA, MuonVector*     _sigMuoA,
			LeptonVector*   _baseLepA, LeptonVector*   _sigLepA,
			JetVector*      _baseJetA, JetVector*      _sigJetA
			);
    void hookMet(const Susy::Met* _met){m_met = _met;}

    //MC weight
    float eventWeight(int mode=1);
    float getLepSFWeight(const LeptonVector* leptons, uint iSys=DGSys_NOM);
    float getTriggerWeight(const LeptonVector* leptons, 
			   float met, int nSignalJets, int npv,
			   uint iSys=DGSys_NOM);
    float getBTagSF(const Susy::Event*, const JetVector* jets, uint iSys=DGSys_NOM);
    float getFakeWeight(const LeptonVector* leptons, uint nVtx, bool isMC, 
			int iSR, float metrel, uint iSys=DGSys_NOM);



    void doAnalysis(unsigned int isys=DGSys_NOM);
    void fillHistograms(uint iSR,
			uint iSYS,
			const LeptonVector* leptons, 
			const JetVector* jets,
			const Met* met,
			float _ww);
    void end();
   
    // Full event selection. Specify which leptons to use.
    bool selectEvent(LeptonVector* leptons, 
		     LeptonVector* baseLeptons, 
		     const JetVector* jets,
		     const Met* met);
	

    map<int,float> getSleptonSumWs(                      ) { return m_sleptonSumWs;  };
    void           setSleptonSumWs( map<int,float> sumws ) { m_sleptonSumWs = sumws; };

    // Reset flags & var
    void reset();
    void resetCounter();

    // Event Cleaning
    bool passEventCleaning();
    bool passBadFCAL(const JetVector* jets, int run, bool isMC=false);

    //Set cuts for a give SR, CR etc...
    void setSelection(std::string s,DiLepEvtType dilType);
    
    // Cut methods
    bool passMll20(const LeptonVector* leptons);
    bool passTrigger(const LeptonVector* leptons, const Met *met);
    bool passNLepCut(const LeptonVector* leptons);
    bool passIsPromptLepton(const LeptonVector* leptons, bool isMC=false);
    bool passFlavor(const LeptonVector* leptons);
    bool passQQ(const LeptonVector* leptons);
    bool passFullJetVeto(const JetVector* jets);
    bool passZVeto(const LeptonVector* leptons, float Zlow = MZ-10, float Zhigh = MZ+10);
    bool passMETRel(const Met *met, const LeptonVector* leptons, const JetVector* jets);
    bool passMET(const Met *met);
    bool passFJet(const JetVector* jets);
    bool passBJet(const JetVector* jets);
    bool passLJet(const JetVector* jets);
    bool passCentralJet(const JetVector* jets);
    bool passLead2JetsPt(const JetVector* jets);
    bool passMjj(const JetVector* jets);
    bool passMT2(const LeptonVector* leptons, const Met* met);
    bool passMWWT(const LeptonVector* leptons, const Met* met);
    bool passMll(const LeptonVector* leptons);
    bool passTopTagger(const LeptonVector* leptons, const JetVector* jets, const Met* met);
    bool passLead2LepPt(const LeptonVector* leptons);
    bool passPtll(const LeptonVector* leptons);
    bool passDPhiMetll(const LeptonVector* leptons, const Met* met);
    bool passDPhiMetl1(const LeptonVector* leptons, const Met* met);
    bool passdPhi(TLorentzVector v0, TLorentzVector v1, float cut);
    bool passdPhill(const LeptonVector* leptons);

    bool passBlindData(bool isMC, int iSR, float metRel, float mt2);

    
    bool  isGenuineSS(const LeptonVector* leptons);
    bool  hasQFlip(const LeptonVector* leptons);
    float getQFlipProb(const LeptonVector* leptons, Met* met, uint iSys=DGSys_NOM);

    float JZBJet(const JetVector* jets, const LeptonVector* leptons);
    float JZBEtmiss(const Met *met, const LeptonVector* leptons);

    void saveOriginal();
    void restoreOriginal(LeptonVector& leptons, const Met *met);
    void clearVectors();

    void setMcSysMinMax(uint sys1=DGSys_NOM, uint sys2=DGSys_XS_DN){
      _sys1 = sys1;
      _sys2 = sys2;
      cout << "Setting Sys range of HFT from " <<  DG2LSystNames[sys1]
	   << " --> " << DG2LSystNames[sys2] << endl;
    }; 
    void initializeHistFitterTree();
    float writeIntoHistFitterTree(uint iSR,
				  LeptonVector* leptons, 
				  const LeptonVector* baseLeptons, 
				  const JetVector* signalJets, 
				  const JetVector* baseJets,
				  const Met* met);
    bool validSystForHFT(uint iSR);
    
    void initializeToyNt();
    void fillToyNt(uint iSR,
		   uint iSYS,
		   const LeptonVector* leptons, 
		   const JetVector* jets,
		   const Met* met,
		   float _ww);

    //Other functions
    void print_SROSjveto();
    void print_SRmT2();
    void print_SR2jets();
    void print_SRZjets();
    void print_SRSSjets();
    void print_SRWW();
    void print_CRZ();
    void print_ZXCR();
    void print_TOPCR();
    void print_WWCR();
    void print_VRSS();

    void print_line(string s, float a, float b, float c);

    void dumpEvent();

    void moveHFTOutput();
    
    
    ClassDef(Susy2LepAna, 1);

  protected:
    TRandom3* _random;

    SusyHistos*  _hh;
    int m_dbg;                  // debug level

    Susy::SusyNtObject* nt;
    
    //External packages
    DilTrigLogic*       m_trigObj;      // My trigger logic class
    SusyMatrixMethod::DiLeptonMatrixMethod m_matrix_method;
    chargeFlip*         m_chargeFlip;

    HistFitterTree*  m_histFitterTrees[ DGSys_GEN ];

    ToyNt*  m_toyNt;

    bool m_useLooseLep;
    int  m_method;
    bool m_writeHFT;
    string  HFTName;

    bool m_writeToyNt;

    SleptonXsecReader*   m_SleptonXSecReader;
    map<int,float>       m_sleptonSumWs;

    SUSY::CrossSectionDB*                       m_susyXsec;     // SUSY cross section database
    std::map<int,SUSY::CrossSectionDB::Process> m_xsecMap;      // our own xsec map for faster lookup times
    

    //containers - from SusyNt
    ElectronVector*      v_baseEle;     // baseline electrons
    ElectronVector*      v_sigEle;      // signal electrons
    MuonVector*          v_baseMu;      // baseline muons
    MuonVector*          v_sigMu;       // signal muons
    LeptonVector*        v_baseLep;     // baseline leptons
    LeptonVector*        v_sigLep;      // signal leptons
    JetVector*           v_baseJet ;    // signal jets
    JetVector*           v_sigJet;      // signal jets
    const Susy::Met*     m_met;         // Met
   
    //These are save copy of the original values from SusyNt
    //Use to restore to original within the SR's loop
    //Since fro SS region, Ele pt, met are modified
    ElectronVector       v_save_sigEle;  // signal electrons 
    MuonVector           v_save_sigMu;   // signal muon 
    LeptonVector         v_save_sigLep;  // baseline leptons 
    Susy::Met            new_met;       // original met


    //Event variables
    float _inc;          //To set counter inc to 1 or _ww.
    uint   SR;           //Signal region
    uint   SYST;         //Current Syst being handled
    uint   _sys1;        //Sys loop over - need to book HFT
    uint   _sys2;


    float metRel;
    float mT2;
    float mCT;

    // Cut variables
    uint                m_nLepMin;      // min leptons
    uint                m_nLepMax;      // max leptons
    bool                m_cutNBaseLep;  // apply nLep cuts to baseline leptons as well as signal
    bool                m_selOS;        // select OS 
    bool                m_selSS;        // select SS 
    bool                m_selSF;        // select SF (no flavor sel by default)
    bool                m_selOF;        // select OF (no flavor sel by default)
    bool                m_vetoSF;       // veto SF
    bool                m_selZ;         // select Z (SFOS applied)
    bool                m_vetoZ;        // veto z
    bool                m_selB;         // sel  b-jet using m_btagPtMin;
    bool                m_vetoB;        // veto b-jet using m_btagPtMin
    bool                m_vetoF;        // veto fwd-jet using
    bool                m_vetoJ;        // Full jet veto
    int                 m_minC20;       // min number of C20
    int                 m_minCJet;      // min number of central C20+B20
    float               m_metMin;       // min Met
    float               m_metMax;       // max Met
    float               m_metRelMin;    // min MetRel
    float               m_metRelMax;    // max MetRel
    bool                m_topTag;       // use top tagger
    float               m_mt2Min;       // min MT2 
    float               m_mt2Max;       // max MT2 
    float               m_lepLeadPtMin; // lead lepton pt min 
    float               m_pTl0Min;      // min pT (leading lepton)
    float               m_pTl1Min;      // min pT (second leading lepton)
    float               m_pTllMin;      // min Ptll
    float               m_pTllMax;      // max Ptll
    float               m_lowMll;       // low Mll bound
    float               m_highMll;      // high Mll bound
    bool                m_mllIn;        // Apply mll cut rejecting event inside
    float               m_dPhillMax;    // max dPhi(ll) 
    float               m_lowMjj;       // min Mjj (leading jets)
    float               m_highMjj;      // max Mjj (leading jets)
    float               m_lowMTWW;      // min MT(ll,Etmiss)
    float               m_highMTWW;     // max MT(ll,Etmiss)
    float               m_pTj0Min;      // min pT (leading jets)
    float               m_pTj1Min;      // min pT (second leading jets)

    float               m_dPhiMetll;    // dphi Met & ll
    float               m_dPhiMetl1;    // dphi Met & l1

    
    DiLepEvtType        m_ET;           // Dilepton event type to store cf
    std::string         m_sel;          // event selection string

    // Event counters
    float                n_readin;
    float                n_pass_HotSpot;
    float                n_pass_BadJet;
    float                n_pass_BadMuon;
    float                n_pass_Cosmic;
    float                n_pass_BadFCAL;
    float                n_pass_DeadRegion;
    float                n_pass_atleast2BaseLep; //>=2 base lept
    float                n_pass_exactly2BaseLep; //=2 base lept
    float                n_pass_mll20;           //mll>20 SS/OS all flavors

    float                n_pass_signalLep[ET_N];
    float                n_pass_dil[ET_N];   //Channel
    float                n_pass_trig[ET_N];
    float                n_pass_truth[ET_N];
    float                n_pass_os[ET_N][DIL_NSR];
    float                n_pass_ss[ET_N][DIL_NSR];
    float                n_pass_flav[ET_N][DIL_NSR];
    float                n_pass_Z[ET_N][DIL_NSR];  //Z sel or veto

    float                n_pass_FullJveto[ET_N][DIL_NSR];  //Full jet veto
    float                n_pass_FJet[ET_N][DIL_NSR]; //Forward jet
    float                n_pass_BJet[ET_N][DIL_NSR]; //b-jet
    float                n_pass_LJet[ET_N][DIL_NSR]; //central LF
    float                n_pass_CJet[ET_N][DIL_NSR]; //central C20+B20
    float                n_pass_JetPt[ET_N][DIL_NSR];
    float                n_pass_mjj[ET_N][DIL_NSR];
    float                n_pass_leadLepPt[ET_N][DIL_NSR];
    float                n_pass_mll[ET_N][DIL_NSR]; //Cut on  mll
    float                n_pass_pTll[ET_N][DIL_NSR];
    float                n_pass_dPhill[ET_N][DIL_NSR];
    float                n_pass_mWWT[ET_N][DIL_NSR];
    float                n_pass_topTag[ET_N][DIL_NSR];
    float                n_pass_metRel[ET_N][DIL_NSR];
    float                n_pass_mt2[ET_N][DIL_NSR];
    float                n_pass_mt2b[ET_N][DIL_NSR];





    float                n_pass_met[ET_N][DIL_NSR];
    float                n_pass_dPhiMetll[ET_N][DIL_NSR];
    float                n_pass_dPhiMetl1[ET_N][DIL_NSR];
    

    float _tmp;

    ofstream evtDump;

    ifstream sigXsfile;



};

#endif
