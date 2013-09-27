/*
 .L ../macros/ZVSys_3L4L.C
main()

plots: do one by one - fix range one some plots

Tables:
scripts/makeLatexTable_v2.sh
Use the SR & the macros bkgYield3L4L.C
The pdf need to be made by hande renaming the .tex file and
 merging All & >=2 J numbers

*/

#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"
#include "SusyWeakProdAna/Histos_Common.h"
#include <vector>
#include <string>
#include "TFile.h"

typedef unsigned uint;

TGuiUtils* _utils;
DrawPlots* _ana;

//
// Variables
//
vector<string> LEP;
vector<string> VR;

bool verbose = false;
bool useSys =true; 

//
// Functions
//
void openHist(string mode="STD",
	      string Top="histo_top_MCNLO",
	      string WW="histo_WW_PowHeg",
	      string Zjets="histo_Zjets_SherpaAlpgenPythia",		      
	      string ZV="histo_WZ_PowHeg", //WZ
	      string Fake="histo_ZZ_PowHeg", //ZZ
	      string Higgs="histo_Higgs");


void  make_Plots(int ivr, string var);



//_____________________________________________________________________________//
//_____________________________________________________________________________//

int main(int argc, char *argv[]){
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 
  
  LEP.clear();
  LEP.push_back("LLL");
  LEP.push_back("LLLL");

  VR.push_back("VRWZ");
  VR.push_back("VRZZ");

  openHist();
 

}



//--------------------------------------------------------------------------------
void openHist(string mode,string Top,string WW,string Zjets,
	      string ZV,string Fake,string Higgs)
{
  _ana->openHistoFiles(mode,Top,WW, Zjets, ZV, Fake,Higgs);
  _ana->fudgeFake(true);
}


//_____________________________________________________________________________//
//
// Make plot for ZX section
//
void  make_Plots(int ivr, string var){
  
  string name;
  static bool const noSys=true;

  //
  // Use Ztt place to put diB,ok since Ztt is 0
  //
  _ana->SFILE[1]="ZZ";
  _ana->SFILE[2]="WZ";
  cout << "size " << _ana->_mcColor.size() << endl;
  _ana->_mcColor[1] = kGreen+3; //Change Fake/ZZ color histo
 

  bool logy=false;
  if(var=="metrel") logy=true;

  if(ivr==0)       name = "ML_VRWZ_LLL_ML_"  + var;
  else if(ivr==1)  name = "ML_VRZZ_LLLL_ML_" + var;
    
  cout << "Plotting " << name << endl;
  _ana->drawPlotErrBand(name,logy,false,noSys);
}
