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

using namespace Susy;

class ToyNt  {

 public: 

  ToyNt(TString MCID, TString suffix);
  ~ToyNt(){};

  void WriteTree();
  void SaveTree();
  void setSumOfMcWeights(double sumOfMcWeights);
  string getFilename() const {return filename.Data();}

  void FillTreeEvent(int run, int event, float npv, float npvCorr, int iSR, int llType, double w);
  void FillTreeLeptons(const LeptonVector* leptons, const Met* met);
  void FillTreeMet(const Met* met,float metrel, float mT2);
  void FillTreeSignalJets(const JetVector* jets, const LeptonVector* leptons, const Met* met);
  void FillTreeOtherJets(JetVector* jets, const LeptonVector* leptons, const Met* met);
  void findRecoilJet();

private:
  TString filename;
  TFile* file; 
  TTree* tree;
  void   clearOutputBranches();

  TString _mcid;

  //
  // TTree branches
  //
  int     _b_run;
  int     _b_event;
  float   _b_npv;
  float   _b_npvCorr;
  int     _b_iSR;
  double  _b_w;
  int     _b_llType;

  int     _b_nlep;
  float   _b_l_pt[2];
  float   _b_l_eta[2];
  float   _b_l_phi[2];
  float   _b_l_e[2];
  int     _b_l_q[2];
  bool    _b_l_isEle[2];
  float   _b_dphi_metl[2]; 
  float   _b_mTl[2];
   
  float   _b_pTll;
  float   _b_phill;
  float   _b_dphi_ll;
  bool    _b_isOS;
  float   _b_mll;

  float   _b_mWWT; 
  float   _b_dphi_metcl; //closest lepton
  float   _b_mT2;

  int     _b_nJets;  //all jets
  int     _b_nSJets; //signal jets
  int     _b_nCJets; 
  int     _b_nBJets; 
  int     _b_nFJets;
  int     _b_nOJets; //Other jets

  bool    _b_j_isC25[25];
  bool    _b_j_isB20[25];
  bool    _b_j_isF30[25];
  bool    _b_j_isOJ[25];
  bool    _b_j_isRecoil[25];  // jet recoild against Z
  bool    _b_j_isSublead[25]; // 2nd leading jet for Z recoil

  float   _b_j_pt[25];
  float   _b_j_eta[25];
  float   _b_j_phi[25];
  float   _b_j_e[25];
  float   _b_j_jvf[25];
  bool    _b_j_isTruth[25];
  int     _b_j_label[25];

  float   _b_met;
  float   _b_met_phi;
  float   _b_metrel;
  float   _b_met_refEle;
  float   _b_met_refMuo;
  float   _b_met_refJet;
  float   _b_met_cellout;
  
  float   _b_dphi_metcj;  //closest jet
  float   _b_dphi_metcoj; //closest other jet

  float   _b_dphi_ll_j1;  //dphi ll & leading signal jet
  float   _b_dphi_ll_oj1; //dphi ll & leading other jet

  float   _b_dphi_Zj;

  float   _b_mEff;
  float   _b_ST;
  float   _b_mjj;
  
};
#endif 