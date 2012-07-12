#ifndef SusyWeakProdAna_Susy3LepAna_h
#define SusyWeakProdAna_Susy3LepAna_h

//////////////////////////////////////////////////////////
// General script to implement basic selection with all //
// signal region cut methods.                           //
//////////////////////////////////////////////////////////

#include <fstream>

// Root Packages

// Susy Common
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtObject.h"
#include "SusyNtuple/SusyNtTools.h"
#include "SusyNtuple/TrilTrigLogic.h"

//SusyWeakProdAna 
#include "SusyWeakProdAna/SusyHistos.h"


class Susy3LepAna: public SusyNtTools
{
  public:

    Susy3LepAna(SusyHistos* _histos);
    virtual ~Susy3LepAna(){};

    // Output Text File
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
    bool selectEvent(const LeptonVector* leptons, const JetVector* jets, const Susy::Met* met);
		  
    // Reset flags & var
    void reset();
   
    // Cut methods
    bool passNLep3Cut(const LeptonVector* leptons);
    bool passNLep4Cut(const LeptonVector* leptons);
    bool passTrigger(const LeptonVector* leptons);
    bool passSFOSCut(const LeptonVector* leptons);
    bool passMetCut (const Susy::Met* met       );
    bool passZCut   (const LeptonVector* leptons);
    bool passBJetCut(                           );
    bool passMtCut  (const LeptonVector* leptons, const Susy::Met* met);
    bool passLepPtCut(const LeptonVector* leptons);
            
    // Selection region // remove
    void setSelection(std::string s) { m_sel = s; }

    //Other functions
    void setEventWeight(int mode=0);

    //categorize event as eee=0, eem=1, mmm=2,  mme=3;
    int evtCatgUnOrd(const LeptonVector* leptons);
    //categorize event using the ordered highest pt OS pair
    //eee/eem=0,  mmm/mme=1, eme/mee=2, emm/mem=3
    int evtCatgOrd(const LeptonVector* leptons);
 
      
    ClassDef(Susy3LepAna, 1);

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

    std::string         m_sel;          // event selection string

    TrilTrigLogic*      m_trigObj;      // My trigger logic class

    //Multilep enum 
    enum ML_SR{ML_SR1a=0,
	       ML_SR1b=1, ML_SR2=2, 
	       ML_4lep=3, ML_4lepnoZ=4, 
	       //ML_StgProd=5, ML_UED=6,
	       ML_NSR=5};
    enum ML_CATG{eee=0, mmm=1, eem=2, mme=3, eme=4, emm=5}; //2st 2 are leading pt


    //Event variables
    float _ww;           //full event weight either full lumi or unblinded 
    
    bool isDGML_SR1a;    //>=3 lep
    bool isDGML_SR1b;    //SR1: ==3lep - SFOS pair, Etmiss>50, Zveto, bjet veto
    bool isDGML_SR2;     //SR2: ==3lep - SFOS pair in Z peak, Etmiss>50
    bool isML_4Lep;      //SR3 4Lep: >=4 lep, Etmiss>50
    bool isML_4LepnoZ;   //SR4 SR3+ Zveto


    // Cut variables
    uint                m_nLep3Min;     // min leptons
    uint                m_nLep3Max;     // max leptons
    uint                m_nLep4Min;     // min leptons
    uint                m_nLep4Max;     // max leptons
    bool                m_selectZ;      // switch to select Zs
    bool                m_vetoZ;        // switch to veto Zs
    bool                m_selectB;      // switch to select b-tagged jets
    bool                m_vetoB;        // switch to veto b-tagged jets
    bool                m_selectSFOS;   // switch to select SFOS pairs
    bool                m_vetoSFOS;     // switch to veto SFOS pairs
    double              m_metMin;       // min MET cut
    double              m_minMt;        // minimum Mt cut

    bool                m_writeOut;     // switch to control output dump

    // Event counters
    uint                n_readin;
    uint                n_pass_LAr;
    uint                n_pass_BadJet;
    uint                n_pass_BadMuon;
    uint                n_pass_Cosmic;
    uint                n_pass_nLep3;
    uint                n_pass_nLep4;
    uint                n_pass_trig;
    uint                n_pass_sfos;
    uint                n_pass_met;
    uint                n_pass_z;
    uint                n_pass_bjet;
    uint                n_pass_mt;
    uint                n_pass_lep30;

};

#endif
