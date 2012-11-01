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
  bool comp  = false;
  bool pred  = false;
  bool table = false;
  bool fakeComp  = false;
  bool FR      = false;
  bool logY    = false;
  bool cutflow = false;
  string ana = ""; //DG2L, ML
  string pName = "";

  /*
  string mode = "STD";
  string sTop = "histo_top_Sherpa";
  string sFake = "histo_mcFake_Sherpa";
  */

  string mode = "DD";
  string sTop = "histo_topDil_Sherpa";
  string sFake = "histo_data12_fake";


  //string sTop = "histo_top_PowHeg";
  string sWW = "histo_WW_Sherpa";
  //string sZX = "histo_ZX_SherpaLFHF";
  //string sZtt = "histo_ZTauTaujets_SherpaLFHF";  
  string sZX = "histo_ZX_Sherpa";
  string sZtt = "histo_ZTauTaujets_Sherpa";  
 

  string sr    = "";
  string cr    = "";
  string type  = "";				
  string sel   = "";
  string lep   = "";
  string var   = "";

  for(int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-comp") == 0)
      comp = true;
    if (strcmp(argv[i], "-pred") == 0)
      pred = true;
    if (strcmp(argv[i], "-table") == 0)
      table = true;
    if (strcmp(argv[i], "-logy") == 0)
      logY = true;
    if (strcmp(argv[i], "-p") == 0)
      pName = argv[++i];
    if (strcmp(argv[i], "-Top") == 0)
      sTop = argv[++i];
    if (strcmp(argv[i], "-WW") == 0)
      sWW = argv[++i];
    if (strcmp(argv[i], "-ZX") == 0)
      sZX = argv[++i];
    if (strcmp(argv[i], "-Ztt") == 0)
      sZtt = argv[++i];
    if (strcmp(argv[i], "-Fake") == 0)
      sFake = argv[++i];
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
    if (strcmp(argv[i], "-mode") == 0)
      mode = argv[++i];
  }
  

  DrawPlots* _ana = new DrawPlots();
   
  //histo files merging
  if(!cutflow)  _ana->openHistoFiles(mode,sTop,sWW,sZX,sZtt,sFake);


  if(comp) _ana->compareShape(pName,logY);
  if(pred) _ana->drawPlotErrBand(pName,logY);
  if(table){
    _ana->bkgEstimate_DG2L();
    _ana->bkgEstimate_ML();
  }
  if(fakeComp){
    _ana->getFakeComposition(pName,sr,type);
  }
  if(FR){
    _ana->getFakeRate(pName,sel,cr,type,lep,var);
  }
  if(cutflow){
    TString s(ana);
    if(s.Contains("DG2L")) _ana->dumpCutflow_DG2L(pName);
    //if(s.Contains("ML")) _ana->dumpCutflow_ML(pName);
  }
  

}
