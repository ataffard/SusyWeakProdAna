#ifndef SusyCommon_Histos_LFV_h
#define SusyCommon_Histos_LFV_h

//////////////////////////////////////////////////////////
// Lepton histograms - Higgs LFV studies
//////////////////////////////////////////////////////////

#include "SusyNtuple/TGuiUtils.h"

#include "SusyWeakProdAna/Histos_Common.h"


const char* const LFV_FLAV[] = {"EE", "MM", "EM", "ME"};
const char* const LFV_QQ[] = {"OS"};

enum LFV_SR{LFV_base=0,
	    LFV_SR,
	    LFV_NSR
};

const string LFV_SRNAME[] = {"LFV_base", "LFV_SR"
};

class Histos_LFV
{

 public:

  Histos_LFV(){
    _utils = new TGuiUtils();
    gROOT->SetStyle("ATLAS");
    _utils->atlasStyle->SetOptStat("emr");
    _utils->atlasStyle->SetOptStat("");
  };
  virtual ~Histos_LFV(){};
  
  void BookLFVHistograms(TDirectory* _hDir, bool useSys=true);

  TGuiUtils* _utils;

  typedef TH1F        *HLFV[LFV_NSR][4][DGSys_N]; //3: EE, MM, EM, ME


 
  //2lep DG
  HLFV LFV_pred;
  HLFV LFV_cutflow;

  ClassDef(Histos_LFV,1);

};


#endif
