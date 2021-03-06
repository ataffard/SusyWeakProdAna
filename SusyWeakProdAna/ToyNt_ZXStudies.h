#ifndef ToyNt_ZXStudies_h
#define ToyNt_ZXStudies_h

// Root Packages
#include "TTree.h"

#include <fstream>

#include "SusyWeakProdAna/ToyNtAna.h"
#include "SusyNtuple/TGuiUtils.h"

class ToyNt_ZXStudies : public ToyNtAna
{
  public:
  
  ToyNt_ZXStudies();
  virtual ~ToyNt_ZXStudies(){};
  
  ofstream out;
  
  // Begin is called before looping on entries
  virtual void    Begin(TTree *tree);
  // Terminate is called after looping is finished
  virtual void    Terminate();
  
  // Main event loop function
  virtual Bool_t  Process(Long64_t entry);
  
  void            Analyze();
  
  ClassDef(ToyNt_ZXStudies, 1);
  
 protected:
  TGuiUtils*   _utils;
  
  void bookHistograms(TDirectory* hDir);
  void saveHistograms(TDirectory* hDir);
  
  static const int nCUT=30;

  TH1F* h_pTll[3][nCUT];
  TH1F* h_met[3][nCUT];
  TH1F* h_metrel[3][nCUT];
  
  TH1F* h_nJets[3][nCUT];
  TH1F* h_nC20[3][nCUT];
  TH1F* h_nB20[3][nCUT];
  TH1F* h_nF30[3][nCUT];

  TH1F* h_jPt[3][nCUT];
  TH1F* h_j1Pt[3][nCUT];
  

  int nCentralJ(float pTmin=25, float jvf=0.2, 
		bool notBTag=true, bool useAbs=false);
  int nBJet(float pTmin=20);
  int nFwdJ(float pTmin=30);
  
};

#endif
