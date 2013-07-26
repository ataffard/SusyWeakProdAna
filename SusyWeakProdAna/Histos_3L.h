#ifndef SusyCommon_Histos_3L_h
#define SusyCommon_Histos_3L_h

//////////////////////////////////////////////////////////
// Lepton histograms - DG3L studies
//////////////////////////////////////////////////////////

#include "SusyNtuple/TGuiUtils.h"

#include "SusyWeakProdAna/Histos_Common.h"

static const int ML_N=11; //5+6=11
const char* const ML_FLAV[] = {"EEE", "EEM", "EMM", "MMM", "LLL",  //5
			       "EEEE","EEEM", "EEMM", "EMMM", "MMMM", "LLLL" //6
};

enum MultiLepType
  {
    ET_eee = 0,
    ET_eem,
    ET_emm,
    ET_mmm,
    ET_lll,

    ET_eeee,
    ET_eeem,
    ET_eemm,
    ET_emmm,
    ET_mmmm,
    ET_llll,
    
    ET_Ukn
  };


enum ML_SR{ML_CF3L, ML_VRWZ, ML_VRZZ,
	   ML_NSR};

const string ML_SRNAME[] = {"CF3L", "VRWZ", "VRZZ"
};


class Histos_3L
{

 public:

  Histos_3L(){
    _utils = new TGuiUtils();
    gROOT->SetStyle("ATLAS");
    _utils->atlasStyle->SetOptStat("emr");
    _utils->atlasStyle->SetOptStat("");
  };
  virtual ~Histos_3L(){};

  void Book3LHistograms(TDirectory* _hDir, bool useSys=true);
  void Sum3LHistograms();

  TGuiUtils* _utils;

  typedef TH1F        *HML[ML_NSR][ML_N][DGSys_N]; 
  
  HML   ML_pred;
  HML   ML_cutflow;
  HML   ML_evtCatgUnOrdered;
  HML   ML_evtCatgOSpair;
  HML   ML_evtCatgSSpair;
  HML   ML_nLep;
  HML   ML_nJets;
  HML   ML_nBJets;
  HML   ML_SFOSMll; //may have multiple entry per event
  HML   ML_SFOSMlll;
  HML   ML_AllMll;
  HML   ML_AllMlll;
  HML   ML_SFOSMZZ;
  HML   ML_SFOSMT;
  HML   ML_etmiss;
  HML   ML_metRefEle;
  HML   ML_metRefGam;
  HML   ML_metRefMuo;
  HML   ML_metRefJet;
  HML   ML_metRefSJet;
  HML   ML_metCellout;
  HML   ML_ptl1;
  HML   ML_ptl2;
  HML   ML_ptl3;
  HML   ML_ptl4;
  HML   ML_etal1;
  HML   ML_etal2;
  HML   ML_etal3;
  HML   ML_etal4;
  HML   ML_d0Sl1;
  HML   ML_d0Sl2;
  HML   ML_d0Sl3;
  HML   ML_d0Sl4;
  HML   ML_z0sinthetal1;
  HML   ML_z0sinthetal2;
  HML   ML_z0sinthetal3;
  HML   ML_z0sinthetal4;
  HML   ML_orgl1;
  HML   ML_orgl2;
  HML   ML_orgl3;
  HML   ML_orgl4;
  HML   ML_ptj1;
  HML   ML_ptj2;
  HML   ML_ptj3;
  HML   ML_ptj4;
  HML   ML_etaj1;
  HML   ML_etaj2;
  HML   ML_etaj3;
  HML   ML_etaj4;
  HML   ML_ptbj;
  HML   ML_etabj;
  
  /** Fill histo given SR for ML*/
  // inline void H1sML_Fill(HML &h, int idx1,  float x, float w){
  //  h[idx1]->Fill(x,w);
  // };
  
  ClassDef(Histos_3L,1);

};


#endif
