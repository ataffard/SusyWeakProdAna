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

//SusyWeakProdAna 
#include "SusyWeakProdAna/SusyHistos.h"
#include "SusyWeakProdAna/PhysicsTools.h"
#include "SusyWeakProdAna/SusyBaseAna.h"

using namespace Susy;

enum TL_CATG{TT=0, TL, LT, LL, XX_Undef};


class SusyFakeAna: public SusyBaseAna
{
 public:
  
  SusyFakeAna(SusyHistos* _histos);
  virtual ~SusyFakeAna(){};
  
  void doAnalysis(float w, unsigned int isys=DGSys_NOM);
  void end();

  
  // Event selection. Specify which leptons to use.
  bool selectEvent(LeptonVector* baseLeptons, 
		   LeptonVector* signalLeptons, 
		   const JetVector* jets,
		   const Met* met,
		   float w);
  
  void getEventWeights(float w,
		       LeptonVector* leptons, 
		       const JetVector* jets,
		       const Met* met);


  bool passSelections(LeptonVector* baseLeptons, 
		      LeptonVector* leptons, 
		      const JetVector* jets,
		      const Met* met);

  //>=1 CJet SS events 2 Tight leptons - For SR's FR, fractions studies 
  bool passSS1j(LeptonVector* leptons, 
		const JetVector* jets,
		const Met* met);


  //SSEM - for Ele SF 
  bool passSSEM(LeptonVector* leptons, 
		const JetVector* jets,
		const Met* met);

  //HF tag-probe - for muon SF
  bool passHFTagProbe(LeptonVector* leptons, 
		      const JetVector* jets,
		      const Met* met);

  //MC only. Extraction region for FR/Eff
  bool passMCExtractionEff(LeptonVector* leptons, 
			   const JetVector* jets,
			   const Met* met);

  //Z Tag -probe for Eff real
  bool passZTagProbe(LeptonVector* leptons, 
		     const JetVector* jets,
		     const Met* met);

  //Conv in Z events for conv FR
  bool passConv(LeptonVector* leptons, 
		const JetVector* jets,
		const Met* met);
  
  //Probe in Z events - for HF/LF SF
  bool passZHFLF(LeptonVector* leptons, 
		 const JetVector* jets,
		 const Met* met);


  //Z tag & probe
  //mode:0 electron, 1:muon
  //category: 1 both tag/probe are tight - pick randomly one for probe
  //          2 both tag/probe leading is tight, second is loose and used as probe
  //          3 both tag/probe second is tight, leading is loose and used as probe
  //void ZTagProbe(const Lepton* &_tag, const Lepton* &_probe, int &category, int mode);
  
  //HF tag & probe
  //mode:0 electron, 1:muon probe
  //tag side: muon inside b-jet
  //probe away size baseline lepton 
  //category: 0: not found, 1:  
  //bool HFTagProbe(const Lepton* &_tag, const Lepton* &_probe, int &category, int mode);

  
  ClassDef(SusyFakeAna, 1);
  
  protected:
  SusyHistos*  _hh;

  //Event variables
  TRandom3* _random;

  float _ww;           //full event weight either full lumi or unblinded 
  float _wwBck;  
  float _wwBTag;
  float _wwqFlip;
  

  int n_pass_SS1j;
  int n_pass_SSEM;
  int n_pass_HFTagProbe;
  int n_pass_MCExtractionEff;
  int n_pass_ZTagProbe;
  int n_pass_ZConv;
  int n_pass_ZHFLF;


 
};

#endif

