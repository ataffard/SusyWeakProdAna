#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"

/*
.L ../macros/run_Plots.C 
main()

openHist("STD","histo_top_PowHeg","histo_WW_Sherpa","histo_ZX_SherpaLFHF","histo_ZTauTaujets_SherpaLFHF");

openHist("DD","histo_topDil_Sherpa","histo_WW_Sherpa","histo_ZX_Sherpa","histo_ZTauTaujets_Sherpa");


draw("DG2L_CR2LepOS_EE_DG2L_mll",true);

*/



typedef unsigned uint;

TGuiUtils* _utils;
DrawPlots* _ana;


void openHist(string mode="DD",
	      string Top="histo_top_MCNLO",
	      string WW="histo_WW_PowHeg",
	      string Zjets="histo_Zjets_SherpaAlpgenPythia",
	      string ZV="histo_WZ_ZZ_PowHeg",
	      string Fake="histo_data12_fake",
	      string Higgs="histo_Higgs");
		      
void draw(string name, bool logy);


//---------------------------
//---------------------------
// MAIN 
//---------------------------
//---------------------------
int main(int argc, char *argv[]){
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");

  _ana = new DrawPlots(); 
  //openHist();
  openHist("STD");
}

//--------------------------------------------------------------------------------
void openHist(string mode,string Top,string WW,string Zjets,string ZV,string Fake, string Higgs)
{
  _ana->openHistoFiles(mode,Top,WW, Zjets, ZV, Fake,Higgs);
}

//--------------------------------------------------------------------------------
void draw(string name, bool logy)
{
  //_ana->drawPlot(name,logy);   
  _ana->drawPlotErrBand(name,logy,true,true);   
}
