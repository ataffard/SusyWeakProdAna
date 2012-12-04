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


class ToyNt  {

 public: 

  ToyNt(TString MCID, TString suffix);
  ~ToyNt(){};

  void WriteTree();
  void SaveTree();
  void FillTree();
  void setSumOfMcWeights(double sumOfMcWeights);

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
  float   _b_npv;
  float   _b_npvCorr;
  
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



  int     _b_nOJets; 




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

  float   _b_mEff;
  float   _b_ST;
  

  
  


};
#endif 
