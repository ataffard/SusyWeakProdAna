#include "SusyWeakProdAna/DrawPlots.h"
#include <string>
#include <vector>
#include <iostream>

#include "TSystem.h"
#include "TROOT.h"
#include "TFile.h"

/*
./DrawPlotsExec -pred -p DG2L_SR0_OS_EE_DG2L_mll
./DrawPlotsExec -comp -p sel_pr_m_iso_npv
./DrawPlotsExec -fake "DG2L" "SR1" "E"
./DrawPlotsExec -cutflow "DG2L" "Egamma.periodB"


Alternative via prompt:
.L ../util/run_IsoPlots.C
main();
_ana->drawPlot("DG2L_SR5_EE_DG2L_mu",false);   

*/
int main(int argc, char *argv[]){
  bool merge = false;
  bool comp  = false;
  bool pred  = false;
  bool fakeComp  = false;
  bool FR      = false;
  bool logY    = false;
  bool cutflow = false;
  string ana = ""; //DG2L, ML
  string pName = "";
  string sr    = "";
  string cr    = "";
  string type  = "";				
  string sel   = "";
  string lep   = "";
  string var   = "";

  for(int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-merge") == 0)
      merge = true;
    if (strcmp(argv[i], "-comp") == 0)
      comp = true;
    if (strcmp(argv[i], "-pred") == 0)
      pred = true;
    if (strcmp(argv[i], "-logy") == 0)
      logY = true;
    if (strcmp(argv[i], "-p") == 0)
      pName = argv[++i];
    if (strcmp(argv[i], "-fakeComp") == 0){
      fakeComp = true;
      pName = argv[++i];
      sr    = argv[++i];
      type  = argv[++i];
    }
    if (strcmp(argv[i], "-FR") == 0){
      FR = true;
      pName = argv[++i];
      sel   = argv[++i];
      cr    = argv[++i];
      type  = argv[++i];
      lep   = argv[++i];
      var   = argv[++i];
    }
    if (strcmp(argv[i], "-cutflow") == 0){
      cutflow = true;
      ana = argv[++i];
      pName = argv[++i];
    }
  }
  

  DrawPlots* _ana = new DrawPlots();
   
  //histo files merging
  if(merge) _ana->mergeHistoFiles();
  if(!cutflow)  _ana->openHistoFiles();


  if(comp) _ana->compareShape(pName,logY);
  if(pred) _ana->drawPlot(pName,logY);
  if(fakeComp){
    _ana->getFakeComposition(pName,sr,type);
  }
  if(FR){
    _ana->getFakeRate(pName,sel,cr,type,lep,var);
  }
  if(cutflow){
    TString s(ana);
    if(s.Contains("DG2L")) _ana->dumpCutflow_DG2L(pName);
    if(s.Contains("ML")) _ana->dumpCutflow_ML(pName);
  }
  

}
