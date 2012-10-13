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
	      string Top="histo_topDil_Sherpa",
	      string WW="histo_WW_Sherpa",
	      string ZX="histo_ZX_Sherpa",		      
	      string Ztt="histo_ZTauTaujets_Sherpa",
	      string Fake="histo_data12_fake");
		      
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
}

//--------------------------------------------------------------------------------
void openHist(string mode,string Top,string WW,string ZX,string Ztt,string Fake)
{
  _ana->openHistoFiles(mode,Top,WW, ZX, Ztt, Fake);
}

//--------------------------------------------------------------------------------
void draw(string name, bool logy)
{
  _ana->drawPlot(name,logy);   
}
