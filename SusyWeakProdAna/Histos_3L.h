#ifndef SusyCommon_Histos_3L_h
#define SusyCommon_Histos_3L_h

//////////////////////////////////////////////////////////
// Lepton histograms - DG3L studies
//////////////////////////////////////////////////////////

#include "SusyNtuple/TGuiUtils.h"

#include "SusyWeakProdAna/Histos_Common.h"

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

  void Book3LHistograms(TDirectory* _hDir);

  TGuiUtils* _utils;

  typedef TH1F        *HML[nHSR_ML];
  
  HML   ML_cutflow;
  HML   ML_evtCatgUnOrdered;
  HML   ML_evtCatgOSpair;
  HML   ML_nLep;
  HML   ML_nJets;
  HML   ML_nBJets;
  HML   ML_SFOSMll; //may have multiple entry per event
  HML   ML_SFOSMlll;
  HML   ML_AllMll;
  HML   ML_AllMlll;
  HML   ML_SFOSMT;
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
  inline void H1sML_Fill(HML &h, int idx1,  float x, float w){
    h[idx1]->Fill(x,w);
  };
  
  ClassDef(Histos_3L,1);

};


#endif
