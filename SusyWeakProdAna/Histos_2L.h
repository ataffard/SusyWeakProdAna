#ifndef SusyCommon_Histos_2L_h
#define SusyCommon_Histos_2L_h

//////////////////////////////////////////////////////////
// Lepton histograms - DG2L studies
//////////////////////////////////////////////////////////

#include "SusyNtuple/TGuiUtils.h"

#include "SusyWeakProdAna/Histos_Common.h"

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
  
  void Book2LHistograms(TDirectory* _hDir);

  TGuiUtils* _utils;

  typedef TH1F        *HDG2L[nHSR_DG2L][3]; //3: EE, MM, EM
  
  //2lep DG
  HDG2L DG2L_pred;
  HDG2L DG2L_cutflow;
  HDG2L DG2L_nJets;
  HDG2L DG2L_nBJets;
  HDG2L DG2L_qq; //SS: --/++; OS: -+/+-
  HDG2L DG2L_mll;
  HDG2L DG2L_mjj;
  HDG2L DG2L_pTll;
  HDG2L DG2L_mWWT;
  HDG2L DG2L_dPhill;
  HDG2L DG2L_JZBJet;
  HDG2L DG2L_JZBEtmiss;
  HDG2L DG2L_etmiss;
  HDG2L DG2L_metrel;
  HDG2L DG2L_metRefEle;
  HDG2L DG2L_metRefGam;
  HDG2L DG2L_metRefMuo;
  HDG2L DG2L_metRefJet;
  HDG2L DG2L_metRefSJet;
  HDG2L DG2L_metCellout;
  HDG2L DG2L_mt2;
  HDG2L DG2L_npv;
  HDG2L DG2L_mu;
  HDG2L DG2L_ptl1;
  HDG2L DG2L_ptl2;
  HDG2L DG2L_etal1;
  HDG2L DG2L_etal2;
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
  HDG2L DG2L_ptbj;
  HDG2L DG2L_etabj;

  /** Fill histo given SR for DG2L*/
  void H1sDG_Fill(HDG2L &h, int idx1, int idx2,  float x, float w){
    h[idx1][idx2]->Fill(x,w);
  };

  ClassDef(Histos_2L,1);

};


#endif
