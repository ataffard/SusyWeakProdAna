#ifndef SusyCommon_Histos_2L_h
#define SusyCommon_Histos_2L_h

//////////////////////////////////////////////////////////
// Lepton histograms - DG2L studies
//////////////////////////////////////////////////////////

#include "SusyNtuple/TGuiUtils.h"

#include "SusyWeakProdAna/Histos_Common.h"
#include <map>

enum DIL_SR{DIL_SRjveto=0, DIL_SR2jets, DIL_SRmT2, DIL_SRmT2b,
	    DIL_CRZ, DIL_CRZjveto, DIL_NTOP, DIL_NWW1, DIL_NWW2, DIL_NWW3,
	    DIL_ZXCR1, DIL_ZXCR3, DIL_ZXCR4, DIL_ZXCR5, DIL_ZXCR6,DIL_ZXCR7,
	    DIL_preSRjveto, DIL_preSR2jets, DIL_preSRmT2,
	    DIL_CR2LepOS, DIL_CR2LepOS40,
	    DIL_SRjveto2, DIL_VRjveto, DIL_VRjveto2,
	    DIL_preSRSSjveto, DIL_SRSSjveto, DIL_SRSS1j,DIL_SRSS1jcut,
	    DIL_CR2LepSS, DIL_CR2LepSS40,DIL_VR1SS, 
	    DIL_NSR
};

//111712: reorder SR/CR and put SS at the end to resolve the lept pt prb
const string DIL_SRNAME[] = {"SRjveto", "SR2jets", "SRmT2", "SRmT2b",
			     "CRZ", "CRZjveto", "NTOP", "NWW1", "NWW2", "NWW3",
			     "ZXCR1", "ZXCR3", "ZXCR4","ZXCR5","ZXCR6","ZXCR7",
			     "preSRjveto", "preSR2jets", "preSRmT2",
			     "CR2LepOS", "CR2LepOS40",
			     "SRjveto2", "VRjveto", "VRjveto2",
			     "preSRSSjveto","SRSSjveto", "SRSS1j","SRSS1jcut",
			     "CR2LepSS", "CR2LepSS40", "VR1SS" 		     
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
