#ifndef SusyWeakProdAna_Susy2LepAna_h
#define SusyWeakProdAna_Susy2LepAna_h

//////////////////////////////////////////////////////////
// General script to implement basic selection with all //
// signal region cut methods.                           //
//////////////////////////////////////////////////////////

#include <fstream>

// Root Packages

// Common Packages
#include "Mt2/mt2_bisect.h" // I don't want to recode this..

// Susy Common
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtObject.h"
#include "SusyNtuple/SusyNtTools.h"
#include "SusyNtuple/DilTrigLogic.h"

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

    void hookContainers(Susy::SusyNtObject* _ntPtr,
			ElectronVector* _baseEleA, ElectronVector* _sigEleA,
			MuonVector*     _baseMuoA, MuonVector*     _sigMuoA,
			LeptonVector*   _baseLepA, LeptonVector*   _sigLepA,
			JetVector*      _baseJetA, JetVector*      _sigJetA
			);
    void hookMet(const Susy::Met* _met){m_met = _met;}

    void doAnalysis();
    void fillHistograms();
    void end();

    // Full event selection. Specify which leptons to use.
    bool selectEvent(const LeptonVector* leptons, const LeptonVector* baseLeptons);
	
    // Reset flags & var
    void reset();
	     
    // Signal regions
    bool passSR1(const LeptonVector* leptons, const JetVector* jets, const Met* met);
    bool passSR2(const LeptonVector* leptons, const JetVector* jets, const Met* met);
    bool passSR3(const LeptonVector* leptons, const JetVector* jets, const Met* met);
    bool passSR4(const LeptonVector* leptons, const JetVector* jets, const Met* met);
    bool passSR5(const LeptonVector* leptons, const JetVector* jets, const Met* met);

    // Cut methods
    bool passNLepCut(const LeptonVector* leptons);
    bool passNBaseLepCut(const LeptonVector* baseLeptons);
    bool passTrigger(const LeptonVector* leptons);
    bool sameFlavor(const LeptonVector* leptons);
    bool oppositeFlavor(const LeptonVector* leptons);
    bool sameSign(const LeptonVector* leptons);
    bool oppositeSign(const LeptonVector* leptons);
    bool passMll(const LeptonVector* leptons, float mll = 20);

    // Signal Region Cuts
    bool passJetVeto(const JetVector* jets);
    bool passZVeto(const LeptonVector* leptons, float Zlow = 81.2, float Zhigh = 101.2);
    bool passMETRel(const Met *met, const LeptonVector* leptons, 
		    const JetVector* jets, float maxMet = 100);
    bool passbJetVeto(const JetVector* jets);
    bool passge2Jet(const JetVector* jets);
    bool passdPhi(TLorentzVector v0, TLorentzVector v1, float cut);
    bool passMT2(const LeptonVector* leptons, const Met* met, float cut);

    //Other functions
    void setEventWeight(int mode=0);


    ClassDef(Susy2LepAna, 1);

  protected:
    int m_dbg;                  // debug level
    SusyHistos*  _hh;

    Susy::SusyNtObject* nt;
    
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

    DilTrigLogic*       m_trigObj;      // My trigger logic class
    
    //2L enum
    enum DG2L_SR{DG2L_SR0_OS=0, DG2L_SR0_SS=1, 
		 DG2L_SR1=2, DG2L_SR2=3, DG2L_SR3=4, DG2L_SR4=5, DG2L_SR5=6, 
		 DG2L_NSR=7};
    enum DIL_QPROD{OS=0, SS=1};


    // Signal Region flags
    bool isDG2L_SR0_OS;  //2lep OS
    bool isDG2L_SR0_SS;  //2lep SS
    bool isDG2L_SR1;     //SR1: OS-jet veto (no anaJet, Zveto, EtmissRel>100
    bool isDG2L_SR2;     //SR2: SS-jet veto (no anaJet, EtmissRel>100)
    bool isDG2L_SR3;     //SR3: OSSF- w/jet  (>=2 anaJet, bjet veto, Zveto, EtmissRel>100, mcT)
    bool isDG2L_SR4;     //SR4: OS Jet Veto, Zveto MetRel, Mt2 (slepton)
    bool isDG2L_SR5;     //SR5: OS Jet Veto, Zveto, metRel, lep pt, sumPt, dPhi 

    //Control region flags
    bool isDG2L_CR0;     //CR0: OS Z 
    bool isDG2L_CR1;     //CR0 Etmiss<40
 

    //Event variables
    float _ww;           //full event weight either full lumi or unblinded 

    int DG2L_flavor;
    int DG2L_qprod;
    float metRel;
    float mT2;
    float mCT;


    // Cut variables
    uint                m_nLepMin;      // min leptons
    uint                m_nLepMax;      // max leptons
    bool                m_cutNBaseLep;  // apply nLep cuts to baseline leptons as well as signal

    DiLepEvtType        m_ET;           // Dilepton event type to store cf

    // Event counters
    uint                n_readin;
    uint                n_pass_LAr;
    uint                n_pass_BadJet;
    uint                n_pass_BadMuon;
    uint                n_pass_Cosmic;
    uint                n_pass_dil[ET_N];
    uint                n_pass_flavor[ET_N];
    uint                n_pass_nLep[ET_N];
    uint                n_pass_mll[ET_N];    
    uint                n_pass_os[ET_N];
    uint                n_pass_ss[ET_N];
    uint                n_pass_trig[ET_N];

    // SR1 counts
    uint                n_pass_SR1jv[ET_N];
    uint                n_pass_SR1Zv[ET_N];
    uint                n_pass_SR1MET[ET_N];

    // SR2 counts
    uint                n_pass_SR2jv[ET_N];
    uint                n_pass_SR2MET[ET_N];

    // SR3 counts
    uint                n_pass_SR3ge2j[ET_N];
    uint                n_pass_SR3Zv[ET_N];
    uint                n_pass_SR3bjv[ET_N];
    uint                n_pass_SR3mct[ET_N];
    uint                n_pass_SR3MET[ET_N];

    // SR4 counts
    uint                n_pass_SR4jv[ET_N];
    uint                n_pass_SR4Zv[ET_N];
    uint                n_pass_SR4MET[ET_N];
    uint                n_pass_SR4MT2[ET_N];
    
    // SR5 counts
    uint                n_pass_SR5jv[ET_N];
    uint                n_pass_SR5MET[ET_N];
    uint                n_pass_SR5Zv[ET_N];
    uint                n_pass_SR5L0pt[ET_N];
    uint                n_pass_SR5SUMpt[ET_N];
    uint                n_pass_SR5dPhiMETLL[ET_N];
    uint                n_pass_SR5dPhiMETL1[ET_N];
    



};

#endif
