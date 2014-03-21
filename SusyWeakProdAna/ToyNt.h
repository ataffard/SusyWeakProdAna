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
#include "SusyWeakProdAna/SusySelection.h"

using namespace Susy;

class ToyNt: public SusyNtTools
{

 public: 

  ToyNt(TString MCID, TString suffix);
  ~ToyNt(){};

  //Toggles blocks of vars to save
  void setBlocks(bool metD=false, bool dijetB=false, 
		 bool OS2LB=false, bool SS2LB=false, 
		 bool ZBalB=false, bool diverVarsB=false,
		 bool fakeB=false);

  void BookTree(); //Book the branches

  void WriteTree(); //Write the Tree
  void SaveTree();  //Save the Tree
  
  void setSumOfMcWeights(double sumOfMcWeights); //Histo of MCWeights
  
  string getFilename() const {return filename.Data();} //Tree output filename

  
  void FillTreeEvent(int run, int event, int mcId,
		     float npv, float npvCorr, double w, double wbtag, double wqflip);

  void FillTreeLeptons(const LeptonVector* leptons, 
		       ElectronVector& baseElectrons, MuonVector& baseMuons, 
		       const Met* met, int nVtx, bool isMc, int llType);

 void FillTreeSignalJets(const JetVector* jets, 
			  const LeptonVector* leptons, 
			  const Met* met,
			  const ElectronVector* preElectrons, 
			  const MuonVector* preMuons);

  void FillTreeMetVar(const Met* met,float metrel);

  void FillTreeEventVar(float mlj, float mljj,
			float sphericity, float sphericityTrans,
			float llAcoplanarity, float jjAcoplanarity,
			bool topTag, float mllCollApprox);

  void FillMT2(float mT2, float mT2jj, float mT2J);
  void FillMCT(float mct, float mctPerp, float mctPara);
  void FillJZB(float JZBjets, float JZBmet);


  //Must be called after FillTreeSignalJets
  //Other baselibne jet get added to the arrays of jet vars
  void FillTreeOtherJets(JetVector* jets, const LeptonVector* leptons, const Met* met);
  void findRecoilJet();

  void FillTreeFakeLeptons(const LeptonVector* leptons,
			   ElectronVector& baseElectrons, MuonVector& baseMuons,
			   const Met* met, int nVtx, bool isMc);
  




  //
  // Branch Block toggles
  //
  bool metDetails;
  bool dijetBlock;
  bool OS2LBlock;
  bool SS2LBlock;
  bool ZBalanceBlock;
  bool diversVarsBlock;
  bool fakeBlock;
  

  //
  // TTree branches
  //
  //Event Block
  int     _b_run;
  int     _b_event;
  int     _b_mcId;
  float   _b_npv;
  float   _b_npvCorr;
  double  _b_w;
  double  _b_wbtag;
  double  _b_wqflip;
  
  //Lepton Block
  int     _b_nlep;
  float   _b_l_pt[2];
  float   _b_l_eta[2];
  float   _b_l_phi[2];
  float   _b_l_e[2];
  float   _b_l_Y[2]; //rapidity   - remove
  int     _b_l_q[2];
  float   _b_l_ptcone30[2];
  float   _b_l_etcone30[2];
  float   _b_l_etconetopo30[2];
  float   _b_l_d0[2];  
  float   _b_l_d0Err[2]; 
  float   _b_l_z0[2];
  bool    _b_l_isEle[2];
  bool    _b_l_isT[2];     //is signal lepton  
  int     _b_l_org[2];     //MC truth org info PR, CONV, HF, LF 
  bool    _b_l_isQFlip[2];  //--OK


  //Dilepton Block
  bool    _b_isOS;
  bool    _b_isGenuineSS;    //MC only - True SS no conv.
  int     _b_llType;
  float   _b_dphi_ll;        //dPhi between lepton
  float   _b_deta_ll;        //dEta between lepton
  float   _b_dR_ll;          //dR between lepton
  float   _b_pTll;           //pT dilepton system
  float   _b_phill;          //phi dilepton system
  float   _b_mll;            //dilepton inv mass
  float   _b_mll_collApprox; //coll approx mll
  float   _b_dphi_ll_j1;     //dphi ll & leading signal jet


  //MET Block  - add detail level to drop refTerm - OFF by default
  float   _b_met;
  float   _b_met_phi;
  float   _b_metrel;
  float   _b_met_refEle;
  float   _b_met_refMuo;
  float   _b_met_refJet;
  float   _b_met_cellout;

  //Jet Block
  int     _b_nJets;  //all jets
  int     _b_nSJets; //signal jets
  int     _b_nCJets; 
  int     _b_nBJets; 
  int     _b_nFJets;
  bool    _b_j_isC20[25];
  bool    _b_j_isB20[25];
  bool    _b_j_isF30[25];
  float   _b_j_pt[25];
  float   _b_j_eta[25];
  float   _b_j_phi[25];
  float   _b_j_e[25];
  float   _b_j_Y[25];//rapidity - remove
  float   _b_j_jvf[25];
  float   _b_j_mv1[25];
  int     _b_j_nEle[25]; //number of pre-E inside dR<0.4 of jet
  int     _b_j_nMu[25];  //number of pre-Mu inside dR<0.4 of jet
  bool    _b_j_isTruth[25];
  int     _b_j_label[25];


  //General event kinematics
  float   _b_mEff;     //HT: lepton, jet , met Sum pT
  float   _b_ST;       //ST: sum pT jets
  bool    _b_topTag;

  //MET & lepton/Jet kinematics  
  float   _b_dphi_metl[2]; 
  float   _b_mTl[2];
  float   _b_dphi_metcl;  //closest lepton
  float   _b_dphi_metcj;  //closest jet


  //Dijet system 2 leading central jets ON/OFF, OFF by default
  float   _b_mjj;      //central 2 jet
  float   _b_pTjj;     //central 2 jet
  float   _b_dRjj;     //leading jets
  float   _b_dEtajj;   //leading jets


  //
  // Analysis specific branches
  //
 
  //2L-OS mT2 mcT  ON/OFF, OFF by default
  float   _b_mT2;
  float   _b_mT2jj;
  float   _b_mT2J;
  float   _b_mct;
  float   _b_mctPerp;
  float   _b_mctPara;
 
  //2L-SS kinematic vars ON/OFF, OFF by default
  float   _b_mWWT; 
  float   _b_mlj;
  float   _b_mljj;
 
   
  //Z balance studies kine  - ON/OFF: OFF by default
  int     _b_nOJets; //Other jets 
  bool    _b_j_isOJ[25];   
  bool    _b_j_isRecoil[25];  // jet recoild against Z
  bool    _b_j_isSublead[25]; // 2nd leading jet for Z recoil
  float   _b_dphi_Zj;     //dPhi ll & other jets (Z balance studies)  
  float   _b_dphi_ll_oj1; //dphi ll & leading other jet
  float   _b_dphi_metcoj; //closest other jet


 //Divers - kine vars  - ON/OFF, OFF by default
  float   _b_sphericity;
  float   _b_sphericityTrans;
  float   _b_llAcoplanarity;
  float   _b_jjAcoplanarity;
  float   _b_JZBjets;
  float   _b_JZBmet;
  float   _b_pTll_Tb; //CONF 2013-048 
  float   _b_dPhib; 



  //For Fake studies
  int  _b_ll_FType;          //Fake  category LL, TL, LT, TT 
  
  //Fake selection flags  All OK
  bool  _b_pass_SS1j; 
  bool  _b_pass_SSEM;
  bool  _b_pass_HFTP;

  int  _b_SSEM_tagIdx;     //Idx tag 
  int  _b_SSEM_probeIdx;   //Idx probe
  int  _b_HFTP_tagIdx;     //
  int  _b_HFTP_probeIdx;   //




 private:

  TString filename;
  TFile* file; 
  TTree* tree;
  void   clearOutputBranches();
  TString _mcid;

};
#endif 
