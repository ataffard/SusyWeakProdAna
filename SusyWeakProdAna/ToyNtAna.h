//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Dec 15 06:11:12 2012 by ROOT version 5.32/00
// from TChain ToyNt/
//////////////////////////////////////////////////////////

#ifndef ToyNtAna_h
#define ToyNtAna_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class ToyNtAna : public TSelector {
 public :
  
  ToyNtAna(TTree * /*tree*/ =0) ;
  virtual ~ToyNtAna() { }
  
  virtual Int_t   Version() const { return 2; }
  virtual void    Begin(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual Bool_t  Notify();
  virtual Bool_t  Process(Long64_t entry);
  virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) {
    m_entry=entry;
    return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0;
  }
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate();
  virtual void    Terminate();
  
  // Sample name - can be used however you like
  std::string sampleName() { return m_sample; }
  void setSampleName(std::string s) { m_sample = s; }
  
  // Debug level
  void setDebug(int dbg) { m_dbg = dbg; }
  int dbg() { return m_dbg; }

  TDirectory* _histoDir;
  std::string m_sample;       // sample name string

  TTree          *fChain;     //!pointer to the analyzed TTree or TChain

  Long64_t m_entry;           // Current entry in the current tree (not chain index!)
  Long64_t m_chainEntry;      // Current entry in the full TChain
  int   m_dbg;                // debug level
    
 
  // Declaration of leaf types
  Int_t           run;
  Int_t           event;
  Float_t         npv;
  Float_t         npvCorr;
  Int_t           iSR;
  Int_t           llType;
  Double_t        w;
  Int_t           nlep;
  Float_t         l_pt[2];   //[nlep]
  Float_t         l_eta[2];   //[nlep]
  Float_t         l_phi[2];   //[nlep]
  Float_t         l_e[2];   //[nlep]
  Int_t           l_q[2];   //[nlep]
  Float_t         l_ptcone30[2];   //[nlep]
  Float_t         l_etcone30[2];   //[nlep]
  Float_t         l_etconetopo30[2];   //[nlep]
  Float_t         l_d0[2];   //[nlep]
  Float_t         l_z0[2];   //[nlep]
  Bool_t          l_isEle[2];   //[nlep]
  Float_t         dphi_metl[2];   //[nlep]
  Float_t         mTl[2];   //[nlep]
  Float_t         pTll;
  Float_t         phill;
  Float_t         mll;
  Float_t         dphi_ll;
  Bool_t          isOS;
  Int_t           nJets;
  Int_t           nSJets;
  Int_t           nCJets;
  Int_t           nBJets;
  Int_t           nFJets;
  //   Int_t           nFJets;
  Int_t           nOJets;
  Bool_t          j_isC25[26];   //[nJets]
  Bool_t          j_isB20[26];   //[nJets]
  Bool_t          j_isF30[26];   //[nJets]
  Bool_t          j_isOJ[26];   //[nJets]
  Float_t         j_pt[26];   //[nJets]
  Float_t         j_eta[26];   //[nJets]
  Float_t         j_phi[26];   //[nJets]
  Float_t         j_e[26];   //[nJets]
  Float_t         j_jvf[26];   //[nJets]
  Float_t         j_mv1[26];   //[nJets]
  Bool_t          j_isTruth[26];   //[nJets]
  Int_t           j_label[26];   //[nJets]
  Bool_t          j_isRecoil[26];   //[nJets]
  Bool_t          j_isSublead[26];   //[nJets]
  Float_t         met;
  Float_t         met_phi;
  Float_t         metrel;
  Float_t         met_refEle;
  Float_t         met_refMuo;
  Float_t         met_refJet;
  Float_t         met_cellout;
  Float_t         met_mWWT;
  Float_t         dphi_metcl;
  Float_t         dphi_metcj;
  Float_t         dphi_metcoj;
  Float_t         dphi_ll_j1;
  Float_t         dphi_ll_oj1;
  Float_t         dphi_Zj;
  Float_t         mT2;
  Float_t         mEff;
  Float_t         ST;
  Float_t         mjj;
  
  // List of branches
  TBranch        *b_run;   //!
  TBranch        *b_event;   //!
  TBranch        *b_npv;   //!
  TBranch        *b_npvCorr;   //!
  TBranch        *b_iSR;   //!
  TBranch        *b_llType;   //!
  TBranch        *b_w;   //!
  TBranch        *b_nlep;   //!
  TBranch        *b_l_pt;   //!
  TBranch        *b_l_eta;   //!
  TBranch        *b_l_phi;   //!
  TBranch        *b_l_e;   //!
  TBranch        *b_l_q;   //!
  TBranch        *b_l_ptcone30;   //!
  TBranch        *b_l_etcone30;   //!
  TBranch        *b_l_etconetopo30;   //!
  TBranch        *b_l_d0;   //!
  TBranch        *b_l_z0;   //!
  TBranch        *b_l_isEle;   //!
  TBranch        *b_dphi_metl;   //!
  TBranch        *b_mTl;   //!
  TBranch        *b_pTll;   //!
  TBranch        *b_phill;   //!
  TBranch        *b_mll;   //!
  TBranch        *b_dphi_ll;   //!
  TBranch        *b_isOS;   //!
  TBranch        *b_nJets;   //!
  TBranch        *b_nSJets;   //!
  TBranch        *b_nCJets;   //!
  TBranch        *b_nBJets;   //!
  TBranch        *b_nFJets;   //!
  //   TBranch        *b_nFJets;   //!
  TBranch        *b_nOJets;   //!
  TBranch        *b_j_isC25;   //!
  TBranch        *b_j_isB20;   //!
  TBranch        *b_j_isF30;   //!
  TBranch        *b_j_isOJ;   //!
  TBranch        *b_j_pt;   //!
  TBranch        *b_j_eta;   //!
  TBranch        *b_j_phi;   //!
  TBranch        *b_j_e;   //!
  TBranch        *b_j_jvf;   //!
  TBranch        *b_j_mv1;   //!
  TBranch        *b_j_isTruth;   //!
  TBranch        *b_j_label;   //!
  TBranch        *b_j_isRecoil;   //!
  TBranch        *b_j_isSublead;   //!
  TBranch        *b_met;   //!
  TBranch        *b_met_phi;   //!
  TBranch        *b_metrel;   //!
  TBranch        *b_met_refEle;   //!
  TBranch        *b_met_refMuo;   //!
  TBranch        *b_met_refJet;   //!
  TBranch        *b_met_cellout;   //!
  TBranch        *b_mWWT;   //!
  TBranch        *b_dphi_metcl;   //!
  TBranch        *b_dphi_metcj;   //!
  TBranch        *b_dphi_metcoj;   //!
  TBranch        *b_dphi_ll_j1;   //!
  TBranch        *b_dphi_ll_oj1;   //!
  TBranch        *b_dphi_Zj;   //!
  TBranch        *b_mT2;   //!
  TBranch        *b_mEff;   //!
  TBranch        *b_ST;   //!
  TBranch        *b_mjj;   //!
  
  

  ClassDef(ToyNtAna,0);
};

#endif

#ifdef ToyNtAna_cxx
void ToyNtAna::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("npv", &npv, &b_npv);
   fChain->SetBranchAddress("npvCorr", &npvCorr, &b_npvCorr);
   fChain->SetBranchAddress("iSR", &iSR, &b_iSR);
   fChain->SetBranchAddress("llType", &llType, &b_llType);
   fChain->SetBranchAddress("w", &w, &b_w);
   fChain->SetBranchAddress("nlep", &nlep, &b_nlep);
   fChain->SetBranchAddress("l_pt", l_pt, &b_l_pt);
   fChain->SetBranchAddress("l_eta", l_eta, &b_l_eta);
   fChain->SetBranchAddress("l_phi", l_phi, &b_l_phi);
   fChain->SetBranchAddress("l_e", l_e, &b_l_e);
   fChain->SetBranchAddress("l_q", l_q, &b_l_q);
   fChain->SetBranchAddress("l_ptcone30", l_ptcone30, &b_l_ptcone30);
   fChain->SetBranchAddress("l_etcone30", l_etcone30, &b_l_etcone30);
   fChain->SetBranchAddress("l_etconetopo30", l_etconetopo30, &b_l_etconetopo30);
   fChain->SetBranchAddress("l_d0", l_d0, &b_l_d0);
   fChain->SetBranchAddress("l_z0", l_z0, &b_l_z0);
   fChain->SetBranchAddress("l_isEle", l_isEle, &b_l_isEle);
   fChain->SetBranchAddress("dphi_metl", dphi_metl, &b_dphi_metl);
   fChain->SetBranchAddress("mTl", mTl, &b_mTl);
   fChain->SetBranchAddress("pTll", &pTll, &b_pTll);
   fChain->SetBranchAddress("phill", &phill, &b_phill);
   fChain->SetBranchAddress("mll", &mll, &b_mll);
   fChain->SetBranchAddress("dphi_ll", &dphi_ll, &b_dphi_ll);
   fChain->SetBranchAddress("isOS", &isOS, &b_isOS);
   fChain->SetBranchAddress("nJets", &nJets, &b_nJets);
   fChain->SetBranchAddress("nSJets", &nSJets, &b_nSJets);
   fChain->SetBranchAddress("nCJets", &nCJets, &b_nCJets);
   fChain->SetBranchAddress("nBJets", &nBJets, &b_nBJets);
   fChain->SetBranchAddress("nFJets", &nFJets, &b_nFJets);
//    fChain->SetBranchAddress("nFJets", &nFJets, &b_nFJets);
   fChain->SetBranchAddress("nOJets", &nOJets, &b_nOJets);
   fChain->SetBranchAddress("j_isC25", j_isC25, &b_j_isC25);
   fChain->SetBranchAddress("j_isB20", j_isB20, &b_j_isB20);
   fChain->SetBranchAddress("j_isF30", j_isF30, &b_j_isF30);
   fChain->SetBranchAddress("j_isOJ", j_isOJ, &b_j_isOJ);
   fChain->SetBranchAddress("j_pt", j_pt, &b_j_pt);
   fChain->SetBranchAddress("j_eta", j_eta, &b_j_eta);
   fChain->SetBranchAddress("j_phi", j_phi, &b_j_phi);
   fChain->SetBranchAddress("j_e", j_e, &b_j_e);
   fChain->SetBranchAddress("j_jvf", j_jvf, &b_j_jvf);
   fChain->SetBranchAddress("j_mv1", j_mv1, &b_j_mv1);
   fChain->SetBranchAddress("j_isTruth", j_isTruth, &b_j_isTruth);
   fChain->SetBranchAddress("j_label", j_label, &b_j_label);
   fChain->SetBranchAddress("j_isRecoil", j_isRecoil, &b_j_isRecoil);
   fChain->SetBranchAddress("j_isSublead", j_isSublead, &b_j_isSublead);
   fChain->SetBranchAddress("met", &met, &b_met);
   fChain->SetBranchAddress("met_phi", &met_phi, &b_met_phi);
   fChain->SetBranchAddress("metrel", &metrel, &b_metrel);
   fChain->SetBranchAddress("met_refEle", &met_refEle, &b_met_refEle);
   fChain->SetBranchAddress("met_refMuo", &met_refMuo, &b_met_refMuo);
   fChain->SetBranchAddress("met_refJet", &met_refJet, &b_met_refJet);
   fChain->SetBranchAddress("met_cellout", &met_cellout, &b_met_cellout);
   fChain->SetBranchAddress("met_mWWT", &met_mWWT, &b_mWWT);
   fChain->SetBranchAddress("dphi_metcl", &dphi_metcl, &b_dphi_metcl);
   fChain->SetBranchAddress("dphi_metcj", &dphi_metcj, &b_dphi_metcj);
   fChain->SetBranchAddress("dphi_metcoj", &dphi_metcoj, &b_dphi_metcoj);
   fChain->SetBranchAddress("dphi_ll_j1", &dphi_ll_j1, &b_dphi_ll_j1);
   fChain->SetBranchAddress("dphi_ll_oj1", &dphi_ll_oj1, &b_dphi_ll_oj1);
   fChain->SetBranchAddress("dphi_Zj", &dphi_Zj, &b_dphi_Zj);
   fChain->SetBranchAddress("mT2", &mT2, &b_mT2);
   fChain->SetBranchAddress("mEff", &mEff, &b_mEff);
   fChain->SetBranchAddress("ST", &ST, &b_ST);
   fChain->SetBranchAddress("mjj", &mjj, &b_mjj);
}

Bool_t ToyNtAna::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef ToyNtAna_cxx
