#ifndef SusyWeakProdAna_SusyFakeAna_h
#define SusyWeakProdAna_SusyFakeAna_h

//////////////////////////////////////////////////////////
// General script to implement basic selection with all //
// signal region cut methods.                           //
//////////////////////////////////////////////////////////

#include <fstream>

// Root Packages
#include "TRandom3.h"

// Susy Common
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNt.h"
#include "SusyNtuple/SusyNtTools.h"

//SusyWeakProdAna 
#include "SusyWeakProdAna/SusyHistos.h"
#include "SusyWeakProdAna/PhysicsTools.h"

using namespace Susy;

enum TL_CATG{TT=0, TL, LT, LL, XX_Undef};


class SusyFakeAna: public SusyNtTools
{
 public:
  
  SusyFakeAna(SusyHistos* _histos);
  virtual ~SusyFakeAna(){};
  
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
  void doMuonAnalysis();
  void doElectronAnalysis();
  void reset();
  void end();

  //Other functions
  void setEventWeight(int mode=0);

  //Standard event cleaning
  bool eventCleaning(); 
  
  //CR info from baseline lepton for 2L, 3L
  void getCRInfo(const LeptonVector* leptons, const JetVector* jets, const Met *met);

  // Fill histograms with probe values
  // type: PR:0, HF: 1, LF:2
  // method 0:selected via tag-probe, 1: truth match
  void fillMuonHisto(const Lepton* _mProbe, LEP_TYPE t, int m, const Lepton* _mTag=NULL);
  void fillElectronHisto(const Lepton* _eProbe,LEP_TYPE  t, int m,const  Lepton* _eTag=NULL);

  //Z tag & probe
  //mode:0 electron, 1:muon
  //category: 1 both tag/probe are tight - pick randomly one for probe
  //          2 both tag/probe leading is tight, second is loose and used as probe
  //          3 both tag/probe second is tight, leading is loose and used as probe
  void ZTagProbe(const Lepton* &_tag, const Lepton* &_probe, int &category, int mode);
  
  //HF tag & probe
  //mode:0 electron, 1:muon probe
  //tag side: muon inside b-jet
  //probe away size baseline lepton 
  //category: 0: not found, 1:  
  bool HFTagProbe(const Lepton* &_tag, const Lepton* &_probe, int &category, int mode);
  
  //LF tag & probe
  bool LFTagProbe(const Lepton* &_tag, const Lepton* &_probe, int &category, int mode);

  //TODO ADD CONV 

  //Get Tight-loose category
  TL_CATG getTL(bool T1, bool T2);
  
  int nJetPtCut(const JetVector& jets, float minPt=20){
    int nJ=0;
    for(uint i=0; i<jets.size(); i++)
      if(jets[i]->Pt()>minPt) nJ++;
    return nJ;
  }

  float lowestMll(const LeptonVector& leps){
    uint nLep = leps.size();
    float mll=9999;
    for(uint i=0; i<nLep; i++){
      for(uint j=i+1; j<nLep; j++){
	float m = Mll(leps[i],leps[j]);
	if(m<mll) mll=m;
      }
    }
    return mll;
  }

  bool passMuIso2011(const Muon* m){
    if(m->ptcone20<1.8)return true;
    return false;
  }

  bool passEleIso2011(const Electron* e){
    if(e->ptcone20/e->Pt()<0.1) return true;
    return false;
  }

  ClassDef(SusyFakeAna, 1);
  
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
  JetVector*           v_baseJet;     // signal jets
  JetVector*           v_sigJet;      // signal jets
  const Susy::Met*     m_met;         // Met
  
  
  //Event variables
  TRandom3* _random;

  float _ww;           //full event weight either full lumi or unblinded 
  float _wwBck;  

  //Event flags
  /* CR fake for DG2L & ML */
  bool isDG2L_CR0;     //2lep OS & SS
  bool isDG2L_CR1;     //CR0 + !Z <2 jets
  bool isDG2L_CR2;     //CR0 + !Z +>=2 jet
  bool isDG2L_CR3;     //CR2 + !btag
  bool isDG2L_CR4;
  bool isML_CR0;       //==3 lep
  bool isML_CR1;       //==3 lep SFOS Zpeak
  bool isML_CR2;       //==3 lep SFOS Zveto, bjet veto
  bool isML_CR3;       //CR2+ !btag
  bool isML_CR4;       //=3lep, 2jet pt>50
  bool isML_CR5;       //=3lep, 2jet pt>30, OSSFmll <50 



};

#endif

