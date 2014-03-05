#ifndef SusyCommon_Histos_WH_h
#define SusyCommon_Histos_WH_h

//////////////////////////////////////////////////////////
// Lepton histograms - DGWH studies
//////////////////////////////////////////////////////////

#include "SusyNtuple/TGuiUtils.h"

#include "SusyWeakProdAna/Histos_Common.h"
#include <map>

const char* const WH_FLAV[] = {"EE", "MM", "EM"};
const char* const WH_QQ[] = {"OS", "SS"};


enum WH_SR{WH_SRSS1j=0, WH_SRSS23j, 
	   WH_optimSRSS,
	   WH_CRSSZVFAKE, WH_CRSSFAKE,
	   //	   WH_SRZb, WH_SRZbb,
	   WH_HighMll, WH_HighPtll, WH_lowMET, WH_BTag,
	   WH_NSR
};

const string WH_SRNAME[] = { "WH_SRSS1j", "WH_SRSS23j", 
			     "WH_optimSRSS",
			     "WH_CRSSZVFAKE", "WH_CRSSFAKE",
			     "WH_HighMll", "WH_HighPtll", "WH_lowMET", "WH_BTag",
			     //"WH_SRZb", "WH_SRZbb",
};


class Histos_WH
{

 public:

  Histos_WH(){
    _utils = new TGuiUtils();
    gROOT->SetStyle("ATLAS");
    _utils->atlasStyle->SetOptStat("emr");
    _utils->atlasStyle->SetOptStat("");
  };
  virtual ~Histos_WH(){};
  
  void BookWHHistograms(TDirectory* _hDir, bool useSys=true);

  TGuiUtils* _utils;

  typedef TH1F        *HDGWH[WH_NSR][3][DGSys_N]; //3: EE, MM, EM
  
  //WH DG
  HDGWH DGWH_pred;
  HDGWH DGWH_cutflow;
  HDGWH DGWH_nJets;
  HDGWH DGWH_nCJets;
  HDGWH DGWH_nFJets;
  HDGWH DGWH_nBJets;
  HDGWH DGWH_nSoftJets; //Not WH jets
  HDGWH DGWH_qq; //SS: --/++; OS: -+/+-
  HDGWH DGWH_mll;
  HDGWH DGWH_mllcoarse;
  HDGWH DGWH_mllcoarser;
  HDGWH DGWH_mllShift; //mll after smearing.
  HDGWH DGWH_mjj;
  HDGWH DGWH_pTll;
  HDGWH DGWH_mWWT;
  HDGWH DGWH_dPhill;
  HDGWH DGWH_dRll;
  HDGWH DGWH_dEtall;
  HDGWH DGWH_dPhilMet; //New
  HDGWH DGWH_dPhiJetMet; //New
  HDGWH DGWH_mTl1; //Mt using leading lepton 
  HDGWH DGWH_mTl2; //Mt using subleading lepton 
  HDGWH DGWH_max_mT; //max Mt 
  HDGWH DGWH_JZBJet;
  HDGWH DGWH_JZBEtmiss;
  HDGWH DGWH_etmiss;
  HDGWH DGWH_etmissPhi;
  HDGWH DGWH_metrel;
  HDGWH DGWH_metrel1;
  HDGWH DGWH_metrel2;
  HDGWH DGWH_metrel3;
  HDGWH DGWH_metRefEle;
  HDGWH DGWH_metRefGam;
  HDGWH DGWH_metRefMuo;
  HDGWH DGWH_metRefJet;
  HDGWH DGWH_metRefSJet;
  HDGWH DGWH_metCellout;
  HDGWH DGWH_mt2;
  HDGWH DGWH_mt2b;
  HDGWH DGWH_mt2j;
  HDGWH DGWH_mlj;
  HDGWH DGWH_mljj;
  HDGWH DGWH_mEff; //New Jets, leptons
  HDGWH DGWH_ST;   //New
  HDGWH DGWH_MetSig;  //New mEff/Etmiss
  HDGWH DGWH_npv;
  HDGWH DGWH_mu;
  HDGWH DGWH_ptl1;
  HDGWH DGWH_ptl2;
  HDGWH DGWH_etal1;
  HDGWH DGWH_etal2;
  HDGWH DGWH_ePt; //new
  HDGWH DGWH_mPt; //new
  HDGWH DGWH_eEta;//new
  HDGWH DGWH_mEta;//new
  HDGWH DGWH_d0Sl1;
  HDGWH DGWH_d0Sl2;
  HDGWH DGWH_z0sinthetal1;
  HDGWH DGWH_z0sinthetal2;
  HDGWH DGWH_orgl1;
  HDGWH DGWH_orgl2;
  HDGWH DGWH_ptj1;
  HDGWH DGWH_ptj2;
  HDGWH DGWH_ptj3;
  HDGWH DGWH_ptj4;
  HDGWH DGWH_etaj1;
  HDGWH DGWH_etaj2;
  HDGWH DGWH_etaj3;
  HDGWH DGWH_etaj4;
  HDGWH DGWH_jvfj1;
  HDGWH DGWH_jvfj2;
  HDGWH DGWH_ptbj;
  HDGWH DGWH_etabj;
  HDGWH DGWH_jvfbj;
  HDGWH DGWH_ptSj1; //softJet
  HDGWH DGWH_ptSj2; //softJet
  HDGWH DGWH_etaSj1; //softJet
  HDGWH DGWH_etaSj2; //softJet
  HDGWH DGWH_jvfSj1; //softJet
  HDGWH DGWH_jvfSj2; //softJet

  ClassDef(Histos_WH,1);

};


#endif
