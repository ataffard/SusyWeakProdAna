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

#include "SusyMatrixMethod/DiLeptonMatrixMethod.h"
#include "ChargeFlip/chargeFlip.h"

//SusyWeakProdAna 
#include "SusyWeakProdAna/SusyHistos.h"


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

    float eventWeight(int mode=1);

    void doAnalysis();
    void fillHistograms(uint iSR,
			const LeptonVector* leptons, 
			const JetVector* jets,
			const Met* met,
			float _ww);
    void end();
   
    // Full event selection. Specify which leptons to use.
    bool selectEvent(const LeptonVector* leptons, 
		     const LeptonVector* baseLeptons, 
		     const JetVector* jets,
		     const Met* met);
	
    // Reset flags & var
    void reset();
    
    // Event Cleaning
    bool passEventCleaning();
    bool passBadFCAL(const JetVector* jets, int run, bool isMC=false);

    //Set cuts for a give SR, CR etc...
    void setSelection(std::string s);
    
    // Cut methods
    bool passMll20(const LeptonVector* leptons);
    bool passTrigger(const LeptonVector* leptons);
    bool passNLepCut(const LeptonVector* leptons);
    bool passIsPromptLepton(const LeptonVector* leptons, bool isMC=false);
    bool passFlavor(const LeptonVector* leptons);
    bool passQQ(const LeptonVector* leptons);
    bool passJetVeto(const JetVector* jets);
    bool passZVeto(const LeptonVector* leptons, float Zlow = MZ-10, float Zhigh = MZ+10);
    bool passMETRel(const Met *met, const LeptonVector* leptons, const JetVector* jets);
    bool passMET(const Met *met);
    bool passbJet(const JetVector* jets, float cutVal=MV1_85);
    bool passge2CJet(const JetVector* jets);
    bool passMT2(const LeptonVector* leptons, const Met* met);
    bool passMll(const LeptonVector* leptons);
    bool passTopTagger(const LeptonVector* leptons, const JetVector* jets, const Met* met);
    bool passLeadLepPt(const LeptonVector* leptons);
    bool passSumLepPt(const LeptonVector* leptons);
    bool passPtll(const LeptonVector* leptons);
    bool passDPhiMetll(const LeptonVector* leptons, const Met* met);
    bool passDPhiMetl1(const LeptonVector* leptons, const Met* met);
    bool passdPhi(TLorentzVector v0, TLorentzVector v1, float cut);

    bool passBlindData(bool isMC, int iSR, float metRel, float mt2);

    float getBTagSF(const Susy::Event*, const JetVector* jets);
    float getFakeWeight(const LeptonVector* leptons, uint nVtx, bool isMC, int iSR, float metrel);

    bool  hasQFlip(const LeptonVector* leptons);
    float getQFlipProb(const LeptonVector* leptons, const Met* met);

    float JZBJet(const JetVector* jets, const LeptonVector* leptons);
    float JZBEtmiss(const Met *met, const LeptonVector* leptons);

    //Other functions
    void print_SRjveto();
    void print_SRSSjveto();
    void print_SR2jets();
    void print_SRmT2();
    void print_CRZ();
    void print_NTOP();
    void print_NWW();
    void print_NZX();
    void print_line(string s, float a, float b, float c);
    
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

    bool m_useLooseLep;
    int m_method;

    //containers
    ElectronVector*      v_baseEle;     // baseline electrons
    ElectronVector*      v_sigEle;      // signal electrons
    MuonVector*          v_baseMu;      // baseline muons
    MuonVector*          v_sigMu;       // signal muons
    LeptonVector*        v_baseLep;     // baseline leptons
    LeptonVector*        v_sigLep;      // signal leptons
    JetVector*           v_baseJet ;    // signal jets
    JetVector*           v_sigJet;      // signal jets
    const Susy::Met*     m_met;         // Met
   
    const Susy::Met*     qFlip_met;     //Met for event w/ qFlip ele
    const Susy::Met*     org_met;     //Met for event w/ qFlip ele

    //Event variables
    float _inc;          //To set counter inc to 1 or _ww.
    uint   SR;

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
    bool                m_vetoSF;       // veto SF
    bool                m_selZ;         // select Z (SFOS applied)
    bool                m_vetoZ;        // veto z
    bool                m_selB;         // sel  b-jet using m_btagPtMin;
    bool                m_vetoB;        // veto b-jet using m_btagPtMin
    bool                m_vetoJ;        // veto jet using m_jetPtMin
    bool                m_sel2J;        // select 2 jet using m_jetPtMin
    float               m_metMin;       // min Met
    float               m_metMax;       // max Met
    float               m_metRelMin;    // min MetRel
    float               m_metRelMax;    // max MetRel
    bool                m_topTag;       // use top tagger
    float               m_mt2Min;       // min MT2 
    float               m_mtMin;        // min MT 
    float               m_mtMax;        // max MT 
    float               m_mllMin;       // min Mll 
    float               m_lepLeadPtMin; // lead lepton pt min 
    float               m_pTllMin;      // min Ptll
    float               m_pTllMax;      // max Ptll
    float               m_dPhiMetll;    // dphi Met & ll
    float               m_dPhiMetl1;    // dphi Met & l1
    
    DiLepEvtType        m_ET;           // Dilepton event type to store cf
    std::string         m_sel;          // event selection string
    enum DIL_SR{DIL_SRjveto=0, DIL_SRSSjveto=1, DIL_SR2jets=2, DIL_SRmT2=3,
		DIL_SRmT2b=4,
		DIL_CRZ=5, DIL_NTOP=6, DIL_NWW1=7, DIL_NWW2=8, DIL_NWW3=9,
		DIL_ZXCR1=10, DIL_ZXCR3=11, DIL_ZXCR4=12,
		DIL_CR2LepOS=13, DIL_CR2LepSS=14,
		DIL_NSR=15
    };


    // Event counters
    float                n_readin;
    float                n_pass_HotSpot;
    float                n_pass_BadJet;
    float                n_pass_BadMuon;
    float                n_pass_Cosmic;
    float                n_pass_BadFCAL;
    float                n_pass_atleast2BaseLep; //>=2 base lept
    float                n_pass_exactly2BaseLep; //=2 base lept
    float                n_pass_mll20;           //mll>20 SS/OS all flavors

    float                n_pass_trig[ET_N];
    float                n_pass_signalLep[ET_N];
    float                n_pass_truth[ET_N];
    float                n_pass_dil[ET_N];   //Channel
    float                n_pass_os[ET_N][DIL_NSR];
    float                n_pass_ss[ET_N][DIL_NSR];
    float                n_pass_flav[ET_N][DIL_NSR];
    float                n_pass_Z[ET_N][DIL_NSR];
    float                n_pass_Jveto[ET_N][DIL_NSR];
    float                n_pass_2Jet[ET_N][DIL_NSR];
    float                n_pass_bJet[ET_N][DIL_NSR];
    float                n_pass_topTag[ET_N][DIL_NSR];
    float                n_pass_metRel[ET_N][DIL_NSR];
    float                n_pass_met[ET_N][DIL_NSR];
    float                n_pass_mt2[ET_N][DIL_NSR];
    float                n_pass_mt2b[ET_N][DIL_NSR];
    float                n_pass_mll[ET_N][DIL_NSR];
    float                n_pass_pTll[ET_N][DIL_NSR];
    float                n_pass_leadLepPt[ET_N][DIL_NSR];
    float                n_pass_sumLepPt[ET_N][DIL_NSR];
    float                n_pass_dPhiMetll[ET_N][DIL_NSR];
    float                n_pass_dPhiMetl1[ET_N][DIL_NSR];
    

    float _tmp;

};

#endif
