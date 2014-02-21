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
  bool noSys =true;

  /*
  string mode = "STD";
  string sTop = "histo_top_Sherpa";Pl
  string sFake = "histo_mcFake_Sherpa";
  */

  string mode = "DD";

  string sFake = "histo_data12";

  //  string sTop = "histo_top_PowHeg";
  //  string sTop = "histo_top_Alpgen";
  string sTop = "histo_top_MCNLO";
  //string sTop = "histo_topDil_Sherpa";ll 
  
  string sWW = "histo_WW_Sherpa";
  //  string sWW = "histo_WW_PowHeg";
  /*
    string sZjets = "histo_Zjets_SherpaLFHF";
    string sZV = "histo_ZTauTaujets_SherpaLFHF";  
  */
  
  string sZjets = "histo_Zjets_AlpgenPythia";
  //  string sZjets = "histo_Zjets_SherpaAlpgenPythia";
  //string sZjets = "histo_Zjets_AlpgenPythia_WZ_ZZ_PowHeg";
  //string sZjets = "histo_Zjets_Sherpa_WZ_ZZ_PowHeg";
  
  
  
  //string sZjets = "histo_Zjets_AlpgenPythia";
  //string sZjets  = "histo_Zjets_AlpgenPythia_diBPowHeg";

  //string sZV = "histo_WZ_ZZ_PowHeg";  
  string sZV = "histo_WZ_ZZ_Sherpa";  
  
  string sHiggs = "histo_Higgs";
  //string sHiggs = "histo_dummy";

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
    if (strcmp(argv[i], "-Zjets") == 0)
      sZjets = argv[++i];
    if (strcmp(argv[i], "-ZV") == 0)
      sZV = argv[++i];
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
    if (strcmp(argv[i], "-noSys") == 0)
      noSys = false;
  }
  

  DrawPlots* _ana = new DrawPlots();
   
  //histo files merging
  if(!cutflow)  _ana->openHistoFiles(mode,sTop,sWW,sZjets,sZV,sFake,sHiggs);


  if(comp) _ana->compareShape(pName,logY);
  if(pred) _ana->drawPlotErrBand(pName,logY,true,noSys);
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
