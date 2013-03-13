#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"

#include <vector>
#include <string>
#include "TFile.h"

typedef unsigned uint;

TGuiUtils* _utils;
DrawPlots* _ana;

//
// Constants
//
//string _pathHisto  =  string(getenv("WORKAREA")) + "/histoAna/SusyAna/";
//string _dir= "histos_101312_13fb_n0105_DD_blindData_noMetRel/";


//
// Variables
//
vector<string> LEP;
vector<string> WWCR;
vector<string> WWSR;

bool verbose = false;
bool useSys =true; //false;

//
// Functions
//
void openHist(string mode="DD",
	      string Top="histo_top_MCNLO",
	      string WW="histo_WW_Sherpa",
	      string ZX="histo_ZX_AlpgenPythia",		      
	      string Ztt="histo_ZTauTaujets_AlpgenPythia",
	      string Fake="histo_data12_flep",
	      string Higgs="histo_Higgs_rlep");

void get_WWCR_data(int ilep, int ireg, int isys,
		    Double_t &est, Double_t &err, 
		    Double_t &SF, Double_t &SFerr);
//void get_WWCR_WWmc();
//void get_WWCR_oBkg();

//void get_WWSR_mc();


//_____________________________________________________________________________//
int main(int argc, char *argv[]){
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 
  openHist();
  
  LEP.clear();
  LEP.push_back("EE");
  LEP.push_back("MM");
  LEP.push_back("EM");

  WWCR.push_back("CRWW");

  WWSR.push_back("SRmT2a");
  WWSR.push_back("SRmT2b");



  
}
//--------------------------------------------------------------------------------
void openHist(string mode,string Top,string WW,string ZX,string Ztt,string Fake)
{
  _ana->openHistoFiles(mode,Top,WW, ZX, Ztt, Fake, Higgs);
}

//_____________________________________________________________________________//
void get_WWCR_data(int il, int ireg, int isys,
		   Double_t &est, Double_t &err, 
		   Double_t &SF, Double_t &SFerr)
{
  
  est =0;
  err =0;
  
  //CR Histos
  string hNameCR= "DG2L_" + WWCR[ireg] + "_" + LEP[il] + "_DG2L_pred"; 
  _ana->grabHisto(hNameCR,true,useSys);

  
  //WW MC in CR's
  TH1F* _h_WW_mc      = (TH1F*)  _ana->getMcHisto(WW,isys)->Clone(); 
  
  //Data in CR's
  TH1F* _h_data       = (TH1F*)  _ana->getDataHisto()->Clone(); 
  
  //Other bkg
  TH1F* _h_fake       = (TH1F*)  _ana->getMcHisto(FAKE,isys)->Clone();       
  TH1F* _h_top        = (TH1F*)  _ana->getMcHisto(TOP,isys)->Clone();       
  TH1F* _h_ZX         = (TH1F*)  _ana->getMcHisto(ZX,isys)->Clone();       
  TH1F* _h_Ztt        = (TH1F*)  _ana->getMcHisto(Ztt,isys)->Clone();       
  
  TH1F* _h_oBkg = (TH1F*) _h_fake->Clone();
  _h_oBkg->SetTitle("oBkg");  _h_oBkg->SetName("oBkg");
  _h_oBkg->Add(_h_top);
  _h_oBkg->Add(_h_WW);
  _h_oBkg->Add(_h_Ztt);
  
  Double_t oBkg_err;
  Double_t oBkg = _h_oBkg->IntegralAndError(0,-1,oBkg_err);

}
