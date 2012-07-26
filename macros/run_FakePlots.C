/*
To run 

.L ../util/run_ FakePlots.C
main();

*/

#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"

TGuiUtils* _utils;
DrawPlots* _ana;

void comp();
void fr(string cr="CR0",string type="hf", string lep="m", string var="pt");

void compFr(string cr="CR0",string type="hf", string lep="m", string var="pt");

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
  _ana->openHistoFiles();  
}


//-----------------------------------------------------------
//
// Compare shape of histo - nromalized
//
void comp()
{
  _ana->compareShape("sel_pr_m_iso_npv");
  
}

//-----------------------------------------------------------
//
// FR -  Eff plots
// FR MC truth Match 
// FR - SF data/MC using bb
// fr("CR0","hf","m","pt")
//
void fr(string cr, string type, string lep, string var)
{
  
  //Draw trm from top & bb, Wjets, Zjets
  std::cout << "Building Truth FR" << std::endl;
  TH1F* _h_top_FR   = _ana->getFakeRate("top","trm",cr,type,lep,var); 
  TH1F* _h_Wjets_FR = _ana->getFakeRate("Wjets","trm",cr,type,lep,var); 
  TH1F* _h_Zjets_FR = _ana->getFakeRate("Zjets","trm",cr,type,lep,var); 
  //  TH1F* _h_bb_FR    = _ana->getFakeRate("BB","trm",cr,type,lep,var); 

  TLegend*  _leg = new TLegend(0.7,0.6,0.8,0.80);
  _utils->legendSetting(_leg); 
  TCanvas* _c0  = _utils->myCanvas("FR Truth match");
  _utils->myDraw1d(_h_top_FR,_c0,1,"e",false,kOrange-2,false,20);
  //  _utils->myDraw1d(_h_bb_FR,_c0,1,"esame",false,kRed+2,false,27);
  _utils->myDraw1d(_h_Wjets_FR,_c0,1,"esame",false,kViolet-7,false,24);
  _utils->myDraw1d(_h_Zjets_FR,_c0,1,"esame",false,kRed+2,false,26);
  _leg->AddEntry(_h_top_FR,"Top","p");
  _leg->AddEntry(_h_Wjets_FR,"Wjets","p");
  _leg->AddEntry(_h_Zjets_FR,"Zjets","p");
  //  _leg->AddEntry(_h_bb_FR,"b#bar b","p");
  _leg->Draw();


  //Draw sel FR - data: EWK bkg remove in getFakeRate
  std::cout << "Building Data/MC SF-FR" << std::endl;
  //  TH1F* _h_bb_FR    = _ana->getFakeRate("BB","sel",cr,type,lep,var); 
  TH1F* _h_data_FR  = _ana->getFakeRate("DATA","sel",cr,type,lep,var); 

  TLegend*  _leg2 = new TLegend(0.7,0.6,0.8,0.80);
  _utils->legendSetting(_leg2); 
  TCanvas* _c1  = _utils->myCanvas("FR selected");
  
  _utils->myDraw1d(_h_data_FR,_c1,1,"e",false,kBlack,false,20);
  //  _utils->myDraw1d(_h_bb_FR,_c1,1,"esame",false,kRed+2,false,27);
  _leg2->AddEntry(_h_data_FR,"Data","p");
  //  _leg2->AddEntry(_h_bb_FR,"b#bar b","p");
  _leg2->Draw();

  //Compte SF
  /*
  TH1F* _fr_SF = (TH1F*) _h_data_FR->Clone();
  _fr_SF->Reset();
  _fr_SF->GetYaxis()->SetTitle("Data/MC");
  _fr_SF->Divide(_h_data_FR,_h_bb_FR,1,1,"B");
  TCanvas* _c2  = _utils->myCanvas("Data/MC SF");
  _utils->myDraw1d(_fr_SF,_c2,1,"e",false,kBlack,false,20);
  */

}

//-----------------------------------------------------------
void compFr(string cr,string type, string lep, string var)
{
  /*
    sel_CR0_hf_m_loose_pt,false,20);
  if(lep=="e")_utils->myDraw1d(_h_fr_0,_c1,1,"esame",false,kViolet,false,20);
  _utils->myDraw1d(_h_fr_1,_c1,1,"esame",false,kBlue,false,20);
  _utils->myDraw1d(_h_fr_2,_c1,1,"esame",false,kRed,false,20);
  _utils->myDraw1d(_h_fr_3,_c1,1,"esame",false,kGreen+3,false,20);
  _leg1->AddEntry(_h_fr_2011,"2011 isolation ","p");
  if(lep=="e") _leg1->AddEntry(_h_fr_0,"TighPP ","p");
  _leg1->AddEntry(_h_fr_1,"TigthPP + pTCone30 ","p");
  _leg1->AddEntry(_h_fr_2,"TigthPP + pTCone30 + IP ","p");
  _leg1->AddEntry(_h_fr_3,"TigthPP + pTCone30+IP+eTCone30 ","p");
  _leg1->Draw();

  if(type=="pr"){
    if(lep=="m") _h_fr_2011->GetYaxis()->SetRangeUser(0.6,1.05);
    if(lep=="e") _h_fr_2011->GetYaxis()->SetRangeUser(0.5,1);
  }
  if(type=="hf") _h_fr_2011->GetYaxis()->SetRangeUser(0,1);


}
