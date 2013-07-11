#ifndef ToyNt_SROptimization_h
#define ToyNt_SROptimization_h

// Root Packages
#include "TTree.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "SusyWeakProdAna/ToyNtAna.h"
#include "SusyNtuple/TGuiUtils.h"

class ToyNt_SROptimization : public ToyNtAna
{
  public:
  
  ToyNt_SROptimization();
  virtual ~ToyNt_SROptimization(){};
  
  ofstream out;
  
  // Begin is called before looping on entries
  virtual void    Begin(TTree *tree);
  // Terminate is called after looping is finished
  virtual void    Terminate();
  
  // Main event loop function
  virtual Bool_t  Process(Long64_t entry);
  
  void            Analyze();
  void            PrintInfo();

  ClassDef(ToyNt_SROptimization, 1);

 protected:
  TGuiUtils*   _utils;
  
  void bookHistograms(TDirectory* hDir);
  void saveHistograms(TDirectory* hDir);
  void addHistograms();  

  void fillHistograms(int icut);
  
  bool passCut(int icut);

  int nEvtProcess;
  

  std::vector<std::string> LEP;
  std::vector<std::string> CUTS;

  static const unsigned int nCUT=17;

  TH1F* h_yield[4][nCUT];
  TH1F* h_metrel[4][nCUT];
    

  float nEvtPass[4][nCUT];

  
};

#endif
