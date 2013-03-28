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


enum DIL_SR{DIL_SROSjveto=0, DIL_SRmT2a, DIL_SRmT2b,
	    DIL_SR2jets, DIL_SRZjets, DIL_SRSSjets,
	    DIL_SRWWa, DIL_SRWWb, DIL_SRWWc, 
	    DIL_ZXCRjveto, DIL_ZXCR2jets, DIL_ZXCRpremT2, DIL_ZXCRmT2a, DIL_ZXCRmT2b,
	    DIL_ZXCRWW, DIL_ZXCRWW2, 
	    DIL_CRTOP, DIL_CRTOPWWa, DIL_CRTOPWWb, DIL_CRTOPWWc, 
	    DIL_CRWW, DIL_CRWW2, DIL_CRWW3, DIL_CRWW4, DIL_CRWW5, 
	    DIL_CRWWa, DIL_CRWWb, DIL_CRWWc,
	    DIL_VRSS, DIL_VRSSbtag,
	    DIL_CRZ,  DIL_CRZjveto,
	    DIL_CR2LepOS, DIL_CR2LepSS, DIL_CR2LepSS40,
	    DIL_preSROSjveto, DIL_preSRmT2,
	    DIL_preSR2jets, DIL_preSRZjets, DIL_preSRSS,  
	    DIL_NSR
};

const string DIL_SRNAME[] = {"SROSjveto", "SRmT2a", "SRmT2b",
			     "SR2jets", "SRZjets","SRSSjets",  
			     "SRWWa", "SRWWb", "SRWWc", 
			     "ZXCRjveto","ZXCR2jets","ZXCRpremT2","ZXCRmT2a","ZXCRmT2b",
			     "ZXCRWW","ZXCRWW2",
			     "CRTOP", "CRTOPWWa", "CRTOPWWb", "CRTOPWWc", 
			     "CRWW","CRWW2","CRWW3","CRWW4","CRWW5",
			     "CRWWa", "CRWWb", "CRWWc", 
			     "VRSS", "VRSSbtag",
			     "CRZ", "CRZjveto", 
			     "CR2LepOS", "CR2LepSS", "CR2LepSS40",
			     "preSROSjveto", "preSRmT2",
			     "preSR2jets", "preSRZjets", "preSRSS",
			     	     
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
  HDG2L DG2L_metRefEle;
  HDG2L DG2L_metRefGam;
  HDG2L DG2L_metRefMuo;
  HDG2L DG2L_metRefJet;
  HDG2L DG2L_metRefSJet;
  HDG2L DG2L_metCellout;
  HDG2L DG2L_mt2;
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
