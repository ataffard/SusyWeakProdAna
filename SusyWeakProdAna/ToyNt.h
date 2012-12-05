//------------------------------------------
//   ToyNt
//   for simple studies of cuts etc...
//
//-------------------------------------------
#ifndef ToyNt_h
#define ToyNt_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TH1F.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "TLorentzVector.h"
#include "TParameter.h"

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtObject.h"
#include "SusyNtuple/SusyNtTools.h"

class ToyNt  {

 public: 

  ToyNt(TString MCID, TString suffix);
  ~ToyNt(){};

  void WriteTree();
  void SaveTree();
  void setSumOfMcWeights(double sumOfMcWeights);

  void FillTree();

  void FillTreeEvent(TString mcid, int run, int event, int npv, int npvCorr);
  void FillTreeLeptons();
  void FillTreeSignalJets();
  void FillTreeOtherJets();
  void FillTreeKin();
  void FillTreeMet();

private:

  TFile* file; 
  TTree* tree;
  void   clearOutputBranches();


  //
  // TTree branches
  //

  TString _b_mcid;
  int     _b_run;
  int     _b_event;
  int     _b_npv;
  int     _b_npvCorr;
  
  int     _b_nlep;
  float   _b_l_pt[2];
  float   _b_l_eta[2];
  float   _b_l_phi[2];
  float   _b_l_e[2];
  float   _b_l_q[2];
  float   _b_l_isEle[2];
   
  float   _b_pTll;
  float   _b_dphi_ll;
  bool    _b_isOS;

  int     _b_nSJets; //signal jets
  int     _b_nCJets; 
  int     _b_nBJets; 
  int     _b_nFJets;
 
  bool    _b_isC25[5];
  bool    _b_isB20[5];
  bool    _b_isF30[5];
  
  float   _b_j_pt[5];
  float   _b_j_eta[5];
  float   _b_j_phi[5];
  float   _b_j_e[5];
  float   _b_j_jvf[5];

  int     _b_nOJets; //Other jets
  float   _b_oj_pt[5];
  float   _b_oj_eta[5];
  float   _b_oj_phi[5];
  float   _b_oj_e[5];
  float   _b_oj_jvf[5];

  float   _b_met;
  float   _b_met_phi;
  float   _b_metrel;
  float   _b_met_refEle;
  float   _b_met_refMuo;
  float   _b_met_refJet;
  float   _b_met_cellout;
  
  float   _b_mTl1;
  float   _b_mTl2;
  float   _b_mWWT;
  
  float   _b_dphi_metcl; //closest lepton
  float   _b_dphi_metl1; 
  float   _b_dphi_metl2; 

  float   _b_dphi_metcj; //closest jet
  float   _b_dphi_metcoj; //closest other jet

  float   _b_dphi_ll_oj1; //dphi ll & leading other jet

  float   _b_mEff;
  float   _b_ST;
  float   _b_mjj;
  
  

  
  


};
#endif 
