#ifndef SusyWeakProdAna_SusyBaseAna_h
#define SusyWeakProdAna_SusyBaseAna_h

//////////////////////////////////////////////////////////
// General script to implement basic selection with all //
// signal region cut methods.                           //
//////////////////////////////////////////////////////////

#include <fstream>

// Root Packages
#include "TRandom3.h"

//Susy Fake Matrix
#include "SusyMatrixMethod/DiLeptonMatrixMethod.h"       //2L OS EWK
#include "SameSignMatrixMethod/DiLeptonMatrixMethod.h"   //2L SS WH

//HistFitter
#include "HistFitterTree/HistFitterTree.h"
#include "SusyXSReader/XSReader.h"

//SusyWeakProdAna 
#include "SusyWeakProdAna/SusySelection.h"
#include "SusyWeakProdAna/SusyHistos.h"

#include "SusyWeakProdAna/ToyNt.h"
#include "SusyWeakProdAna/XsecUncertainty.h"

#include "SusyNtuple/SusyNtAna.h"

///
/// \class SusyBaseAna
/// \brief Base class inherited by all analysis classes.
///
/// Contains common functions used by all analysis code. 
///

class SusyBaseAna: public SusySelection
{
  public:

  SusyBaseAna(SusyHistos* _histos, bool is2LAna=true, bool isWHAna=false, bool qFlipd0=false);
    virtual ~SusyBaseAna(){};


    
    //Use loose lepton when runing on data - fake estimate
    void setUseLooseLep(bool b) { 
      if(b) cout << ">>> Using base leptons !!! "  << endl;
      m_useLooseLep = b; 
    }
    //Filter prompt lepton events
    void setMethod(int m){ 
      m_method = m;
      cout << ">>> Using method " << SMETHOD[m_method] << endl;
    }

    //Hook containers 
    void hookContainers(Susy::SusyNtObject* _ntPtr,
			ElectronVector* _preEleA, ElectronVector* _baseEleA, ElectronVector* _sigEleA,
			MuonVector*     _preMuoA, MuonVector*     _baseMuoA, MuonVector*     _sigMuoA,
			LeptonVector*   _baseLepA, LeptonVector*   _sigLepA,
		        JetVector*      _preJetA, JetVector*      _baseJetA, JetVector*      _sigJetA,
			TauVector*      _baseTauA, TauVector*     _sigTauA
			);
    void hookMet(const Susy::Met* _met){m_met = _met;}

    float eventWeight(int mode=1, uint iSys=DGSys_NOM);
    bool  isSimplifiedModelGrid(int dsId);
    float getXsUncert(uint dsid);

    const SumwMap* getMCSumWs(           ) { return m_MCSumWs;  };
    void           setMCSumWs( const SumwMap* sumwMap ) { m_MCSumWs = sumwMap; };

    float getLepSFWeight(const LeptonVector* leptons, uint iSys=DGSys_NOM);
    float getTriggerWeight(const LeptonVector* leptons, 
			   float met, int nSignalJets, int npv,
			   uint iSys=DGSys_NOM);
    float getBTagSF(const Susy::Event*, const JetVector* jets, uint iSys=DGSys_NOM);

    void finish();

    //To print cutflow
    void print_line(string s, float a, float b, float c);
    void print_line(string s, int istart, int iend, float array[LEP_N]);
    void print_line(string s, int istart, int iend, int sr, float array[LEP_N][SR_N]);

    
 
    //Save orginal Met and lepton
    // Needed to handle chargeFlip
    void saveOriginal();
    void restoreOriginal(LeptonVector& leptons, const Met *met);
    void clearVectors();
  
    void setMcSysMinMax(uint sys1=DGSys_NOM, uint sys2=DGSys_Pileup_DN){
      _sys1 = sys1;
      _sys2 = sys2;
      cout << "Setting Sys range of HFT from " <<  DGSystNames[sys1]
	   << " --> " << DGSystNames[sys2] << endl;
    }; 

    //ToyNt
    void initializeToyNt(bool metD=false, bool dijetB=false, 
			 bool OS2LB=false, bool SS2LB=false, 
			 bool ZBalB=false, bool diverVarsB=false,
			 bool fakeB=false,
			 bool LFVB=false, bool razorB=false);
    void fillToyNt(uint iSYS,
		   const LeptonVector* leptons, 
		   const JetVector* jets,
		   const Met* met,
		   float _ww, float _wwBTag, float _wwQFlip);

    //For debugging
    void dumpEvent();
    void dumpLeptons(const LeptonVector* leptons);
    void dumpJets(const JetVector* jets);
    void dumpTrigger();

   
    ClassDef(SusyBaseAna, 1);

  protected:

    SusyHistos*  _hh;        //Histograms
    ToyNt*       m_toyNt;    // toyNt


    //External packages
    DilTrigLogic*       m_trigObj;      // 2L trigger logic
    TrilTrigLogic*      m_trig3LObj;    // 3L trigger logic class

    SusyMatrixMethod::DiLeptonMatrixMethod m_matrix_method;
    SameSignMatrixMethod::DiLeptonMatrixMethod m_matrix_methodWH;

    //Analysis flags
    bool    m_useLooseLep;
    int     m_method;
    bool    m_writeToyNt;
    uint    _sys1;        //Sys loop over - need to book HFT
    uint    _sys2;
    
    XSReader* susyXS;
    const SumwMap*       m_MCSumWs; 

    ofstream out;
    ofstream evtDump;
    ifstream sigXsfile;

};

#endif
