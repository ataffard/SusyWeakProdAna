#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"

/*
.L ../macros/testPlotErr
main()

openHist("STD","histo_top_PowHeg","histo_WW_Sherpa","histo_ZX_SherpaLFHF","histo_ZTauTaujets_SherpaLFHF");
openHist("DD","histo_topDil_Sherpa","histo_WW_Sherpa","histo_ZX_Sherpa","histo_ZTauTaujets_Sherpa");

draw("DG2L_CR2LepOS_EE_DG2L_mll",true);

*/

typedef unsigned uint;

TGuiUtils* _utils;
DrawPlots* _ana;

void openHist(string mode="DD",
	      string Top="histo_top_PowHeg",
	      //	      string Top="histo_topDil_Sherpa",
	      string WW="histo_WW_Sherpa",
	      //string WW="histo_WW_Powheg",
	      string ZX="histo_ZX_Sherpa",		      
	      //string ZX="histo_ZX_SherpaLFHF",		      
	      //string ZX="histo_ZX_SherpaPowheg",		      
	      //string ZX="histo_ZX_Alpgen",		      
	      string Ztt="histo_ZTauTaujets_Sherpa",
	      //string Ztt="histo_ZTauTaujets_Alpgen",
	      //string Ztt="histo_ZTauTaujets_SherpaLFHF",
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
  openHist();
}

//--------------------------------------------------------------------------------
void openHist(string mode,string Top,string WW,string ZX,string Ztt,string Fake)
{
  _ana->openHistoFiles(mode,Top,WW, ZX, Ztt, Fake);
  //  _ana->openHistoFiles("DD","histo_dummy","histo_dummy", ZX, "histo_dummy", "histo_dummy");

}

//--------------------------------------------------------------------------------
void draw(string name, bool logy)
{
  _ana->drawPlotErrBand(name,logy,true,false);   
}




