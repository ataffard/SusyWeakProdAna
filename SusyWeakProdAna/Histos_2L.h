#ifndef SusyCommon_Histos_2L_h
#define SusyCommon_Histos_2L_h

//////////////////////////////////////////////////////////
// Lepton histograms - DG2L studies
//////////////////////////////////////////////////////////

#include "SusyNtuple/TGuiUtils.h"

#include "SusyWeakProdAna/Histos_Common.h"
#include <map>

const char* const DIL_FLAV[] = {"EE", "MM", "EM"};
const char* const DIL_QQ[] = {"OS", "SS"};


enum DIL_SR{DIL_SRmT2a=0, DIL_SRmT2b, DIL_SRmT2c,
	    DIL_SRZjets, DIL_SRSSjets,
	    DIL_SRWWa, DIL_SRWWb, DIL_SRWWc, 
	    DIL_CRWWmet, DIL_CRWWmt2,
	    DIL_CRTOPmet, DIL_CRTOPmt2, DIL_CRTOPZjets,
	    DIL_CRZVmet, 
	    DIL_CRZVmt2a, DIL_CRZVmt2b, DIL_CRZVmt2c, DIL_CRZVmt2d,
	    DIL_CRZVZjets,  
	    DIL_VRSS, 
	    DIL_CRZ, DIL_CRZjets, DIL_CRZjveto,
	    DIL_CR2LepOS, DIL_CR2LepSS, DIL_CR2LepSS40,
	    DIL_preSROSjveto, DIL_preSRmT2,
	    DIL_preSRZjets, DIL_preSRWW, DIL_preSRSS,
	    DIL_optimSRZjets,
	    DIL_optimSRjets,
	    DIL_optimSRSS,
	    DIL_optimSR0jet,
	    DIL_NSR
};

const string DIL_SRNAME[] = {"SRmT2a", "SRmT2b","SRmT2c",
			     "SRZjets","SRSSjets",  
			     "SRWWa", "SRWWb", "SRWWc", 
			     "CRWWmet", "CRWWmt2",
			     "CRTOPmet", "CRTOPmt2", "CRTOPZjets",
			     "CRZVmet", 
			     "CRZVmt2a", "CRZVmt2b", "CRZVmt2c", "CRZVmt2d", 
			     "CRZVZjets",
			     "VRSS", 
			     "CRZ", "CRZjets", "CRZjveto", 
			     "CR2LepOS", "CR2LepSS", "CR2LepSS40",
			     "preSROSjveto", "preSRmT2",
			     "preSRZjets", "preSRWW", "preSRSS",
			     "optimSRZjets",
			     "optimSRjets",
			     "optimSRSS",
			     "optimSR0jet",
			     	     
};

class Histos_2L
{

 public:

  Histos_2L(){
    _utils = new TGuiUtils();
    gROOT->SetStyle("ATLAS");
    _utils->atlasStyle->SetOptStat("emr");
    _utils->atlasStyle->SetOptStat("");
  };
  virtual ~Histos_2L(){};
  
  void Book2LHistograms(TDirectory* _hDir, bool useSys=true);

  TGuiUtils* _utils;

  typedef TH1F        *HDG2L[DIL_NSR][3][DGSys_N]; //3: EE, MM, EM
  
  //2lep DG
  HDG2L DG2L_pred;
  HDG2L DG2L_Zcount;
  HDG2L DG2L_cutflow;
  HDG2L DG2L_nJets;
  HDG2L DG2L_nCJets;
  HDG2L DG2L_nFJets;
  HDG2L DG2L_nBJets;
  HDG2L DG2L_nSoftJets; //Not 2L jets
  HDG2L DG2L_qq; //SS: --/++; OS: -+/+-
  HDG2L DG2L_mll;
  HDG2L DG2L_mllcoarse;
  HDG2L DG2L_mllcoarser;
  HDG2L DG2L_mjj;
  HDG2L DG2L_pTll;
  HDG2L DG2L_mWWT;
  HDG2L DG2L_dPhill;
  HDG2L DG2L_dPhilMet; //New
  HDG2L DG2L_dPhiJetMet; //New
  HDG2L DG2L_mTl1; //Mt using leading lepton 
  HDG2L DG2L_mTl2; //Mt using subleading lepton 
  HDG2L DG2L_JZBJet;
  HDG2L DG2L_JZBEtmiss;
  HDG2L DG2L_etmiss;
  HDG2L DG2L_etmissPhi;
  HDG2L DG2L_metrel;
  HDG2L DG2L_metrel1;
  HDG2L DG2L_metrel2;
  HDG2L DG2L_metrel3;
  HDG2L DG2L_metRefEle;
  HDG2L DG2L_metRefGam;
  HDG2L DG2L_metRefMuo;
  HDG2L DG2L_metRefJet;
  HDG2L DG2L_metRefSJet;
  HDG2L DG2L_metCellout;
  HDG2L DG2L_mt2;
  HDG2L DG2L_mt2b;
  HDG2L DG2L_mEff; //New
  HDG2L DG2L_ST;   //New
  HDG2L DG2L_npv;
  HDG2L DG2L_mu;
  HDG2L DG2L_ptl1;
  HDG2L DG2L_ptl2;
  HDG2L DG2L_etal1;
  HDG2L DG2L_etal2;
  HDG2L DG2L_ePt; //new
  HDG2L DG2L_mPt; //new
  HDG2L DG2L_eEta;//new
  HDG2L DG2L_mEta;//new
  HDG2L DG2L_d0Sl1;
  HDG2L DG2L_d0Sl2;
  HDG2L DG2L_z0sinthetal1;
  HDG2L DG2L_z0sinthetal2;
  HDG2L DG2L_orgl1;
  HDG2L DG2L_orgl2;
  HDG2L DG2L_ptj1;
  HDG2L DG2L_ptj2;
  HDG2L DG2L_ptj3;
  HDG2L DG2L_ptj4;
  HDG2L DG2L_etaj1;
  HDG2L DG2L_etaj2;
  HDG2L DG2L_etaj3;
  HDG2L DG2L_etaj4;
  HDG2L DG2L_jvfj1;
  HDG2L DG2L_jvfj2;
  HDG2L DG2L_ptbj;
  HDG2L DG2L_etabj;
  HDG2L DG2L_jvfbj;
  HDG2L DG2L_ptSj1; //softJet
  HDG2L DG2L_ptSj2; //softJet
  HDG2L DG2L_etaSj1; //softJet
  HDG2L DG2L_etaSj2; //softJet
  HDG2L DG2L_jvfSj1; //softJet
  HDG2L DG2L_jvfSj2; //softJet

  std::map<int,int> runBins;

  ClassDef(Histos_2L,1);

};


#endif
