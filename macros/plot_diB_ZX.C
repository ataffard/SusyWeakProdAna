#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"
#include "SusyWeakProdAna/Histos_Common.h"
#include <vector>
#include <string>
#include "TFile.h"

typedef unsigned uint;

TGuiUtils* _utils;
DrawPlots* _ana;

vector<string> LEP;

void openHist(string mode="DD",
	      string Top="histo_ZZ_Sherpa",
	      string WW="histo_lllnu_WZ.126893",
	      string ZX="histo_Zjets_Sherpa",		      
	      string Ztt="histo_ZTauTaujets_Sherpa",
	      string Fake="histo_dummy");


void plot(string name);

//_____________________________________________________________________________//
int main(int argc, char *argv[]){
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 
  
  LEP.clear();
  LEP.push_back("EE");
  LEP.push_back("MM");
  //LEP.push_back("EM");

  openHist();

  //plot("DG2L_ZXCR7_EE_DG2L_mll");
  plot("DG2L_ZXCR7_MM_DG2L_mll");

}


//--------------------------------------------------------------------------------
void openHist(string mode,string Top,string WW,string ZX,string Ztt,string Fake)
{
  _ana->openHistoFiles(mode,Top,WW, ZX, Ztt, Fake);

}

//--------------------------------------------------------------------------------
void plot(string name){
  int isys=DGSys_NOM;
  
  _ana->grabHisto(name,true);

  TH1F* _h_ZZ    = (TH1F*) _ana->getMcHisto(TOP,isys)->Clone();
  TH1F* _h_WZ    = (TH1F*) _ana->getMcHisto(WW,isys)->Clone();
  TH1F* _h_Zjets = (TH1F*) _ana->getMcHisto(ZX,isys)->Clone();

  _h_ZZ->GetXaxis()->SetRangeUser(70,110);
  _h_WZ->GetXaxis()->SetRangeUser(70,110);
  _h_Zjets->GetXaxis()->SetRangeUser(70,110);


  vector<int> _col;
  _col.push_back(kViolet+1);  //ZZ
  _col.push_back(kAzure+4);   //WZ
  _col.push_back(kOrange-2);  //Zjets

  TLegend* _leg = new TLegend(0.7,0.6,0.9,0.8);
  _utils->legendSetting(_leg); 


  //Stack version
  TCanvas* _c0  = _utils->myCanvas("Stack");
  THStack* _stack;
  _stack = new THStack("ZX","ZX");
  _stack->Clear();
  _utils->addToTHStack(_stack,_h_ZZ,_col[0], "HIST", _leg, "ZZ");
  _utils->addToTHStack(_stack,_h_WZ,_col[1], "HIST", _leg, "WZ");
  _utils->addToTHStack(_stack,_h_Zjets,_col[2], "HIST", _leg, "Zjets");
  _stack->Draw("hist");
  _stack->GetXaxis()->SetRangeUser(70,110);


  TH1F* _h_stack = (TH1F*) _h_ZZ->Clone();
  _h_stack->Add(_h_WZ);
  _h_stack->Add(_h_Zjets);
  _h_stack->GetXaxis()->SetRangeUser(70,110);
  _h_stack->SetMaximum(_h_stack->GetMaximum()*1.2);

  TCanvas* _c1  = _utils->myCanvas("Individual",800,800);
  _c1->Clear();
  _c1->Divide(2,2);
  _c1->cd(1);
  _utils->myDraw1d(_h_ZZ,_c1,1,"e",false,_col[0],false,20);
  _utils->myDraw1d(_h_WZ,_c1,2,"e",false,_col[1],false,20);
  _utils->myDraw1d(_h_Zjets,_c1,3,"e",false,_col[2],false,20);
  _leg->Draw();

  
  
  _c0->Clear();
  _utils->myDraw1d(_h_stack,_c0,1,"e",false,kBlack,false,20);
  
  _stack->Draw("histsame");
  _utils->myDraw1d(_h_stack,_c0,1,"esame",false,kBlack,false,20);
  _leg->Draw();
  


}
