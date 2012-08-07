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
void compIP(string cr="CR0",string type="hf", string lep="m", string var="pt");

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
    sel_CR0_hf_m_loose_pt
    sel_CR0_hf_m_tightNI_pt
    sel_CR0_hf_m_tightNI2_pt
    sel_CR0_hf_m_tightNIIP_pt
  */
  
  //string sel = "sel";
  string sel = "trm";

  //string sFile1 = "data12";
  //string sFile1 = "top";
  string sFile1 = "Zjets_Alpgen";
  //string sFile1 = "SimplifiedModel_wA_slep";
      
  string loose = sel + "_" + cr + "_" + type + "_" + lep + "_loose_" + var;
  string tightPP = sel + "_" + cr + "_" + type + "_" + lep + "_tightPP_" + var; //e tight PP
  string tight = sel + "_" + cr + "_" + type + "_" + lep + "_tight_" + var; //winter 2011 cuts
  string tightNI = sel + "_" + cr + "_" + type + "_" + lep + "_tightNI_" + var;
  string tightNIIP = sel + "_" + cr + "_" + type + "_" + lep + "_tightNIIP_" + var;
  string tightNI2 = sel + "_" +  cr + "_" + type + "_" + lep + "_tightNI2_" + var;




  TH1F* _h_loose   = (TH1F*) _ana->getHisto(sFile1,loose);
  TH1F* _h_tightPP = NULL;
  if(lep=="e") _h_tightPP   = (TH1F*) _ana->getHisto(sFile1,tightPP);
  TH1F* _h_tight     = (TH1F*) _ana->getHisto(sFile1,tight);
  TH1F* _h_tightNI   = (TH1F*) _ana->getHisto(sFile1,tightNI);
  TH1F* _h_tightNIIP = (TH1F*) _ana->getHisto(sFile1,tightNIIP);
  TH1F* _h_tightNI2  = (TH1F*) _ana->getHisto(sFile1,tightNI2);

  TH1F* _h_fr_0 = (TH1F*)  _h_loose->Clone();  //2011
  _h_fr_0->Reset();
  _h_fr_0->GetYaxis()->SetTitle("Efficiency");
  _h_fr_0->Divide(_h_tight,_h_loose,1,1,"B");

  TH1F* _h_fr_4 = NULL;
  if(lep=="e") {
    _h_fr_4 =  (TH1F*)  _h_loose->Clone(); //E only Tight PP
    _h_fr_4->Reset();
    _h_fr_4->GetYaxis()->SetTitle("Efficiency");
    _h_fr_4->Divide(_h_tightPP,_h_loose,1,1,"B");
  }

  TH1F* _h_fr_1 = (TH1F*)  _h_loose->Clone(); //PtCone
  _h_fr_1->Reset();
  _h_fr_1->GetYaxis()->SetTitle("Efficiency");
  _h_fr_1->Divide(_h_tightNI,_h_loose,1,1,"B");

  TH1F* _h_fr_2 = (TH1F*)  _h_loose->Clone();  //PtCone + IP
  _h_fr_2->Reset();
  _h_fr_2->GetYaxis()->SetTitle("Efficiency");
  _h_fr_2->Divide(_h_tightNIIP,_h_loose,1,1,"B");


  TH1F* _h_fr_3 = (TH1F*)  _h_loose->Clone(); //PtCone + IP + EtCone
  _h_fr_3->Reset();
  _h_fr_3->GetYaxis()->SetTitle("Efficiency");
  _h_fr_3->Divide(_h_tightNI2,_h_loose,1,1,"B");



  TLegend*  _leg1;
  if(type=="pr" /*|| type=="lf"*/) _leg1 = new TLegend(0.5,0.3,0.8,0.60);
  else _leg1 = new TLegend(0.5,0.6,0.8,0.80);
  _utils->legendSetting(_leg1,0.03); 
  TCanvas* _c1  = _utils->myCanvas("FR");
  
  _h_fr_0->GetXaxis()->SetRangeUser(0,100);
  _utils->myDraw1d(_h_fr_0,_c1,1,"e",false,kBlack,false,20);
  if(lep=="e") _utils->myDraw1d(_h_fr_4,_c1,1,"esame",false,kCyan-3,false,24);
  _utils->myDraw1d(_h_fr_1,_c1,1,"esame",false,kBlue,false,22);
  _utils->myDraw1d(_h_fr_2,_c1,1,"esame",false,kRed,false,33);
  _utils->myDraw1d(_h_fr_3,_c1,1,"esame",false,kGreen+3,false,27);
  if(type == "pr" /*|| type=="lf"*/) _h_fr_0->GetYaxis()->SetRangeUser(0.6,1.05);
  else _h_fr_0->GetYaxis()->SetRangeUser(0,1.0);
  _c1->Update();

  _leg1->AddEntry(_h_fr_0,"2011 isolation ","p");
  if(lep=="e") _leg1->AddEntry(_h_fr_4,"TightPP ","p");
  _leg1->AddEntry(_h_fr_1,"+ PtCone30","p");
  _leg1->AddEntry(_h_fr_2,"+ IP ","p");
  _leg1->AddEntry(_h_fr_3,"+ EtCone(Topo)30 ","p");
  _leg1->Draw();
  
 

}


//-----------------------------------------------------------
void compIP(string cr,string type, string lep, string var)
{
  //string sel = "sel";
  string sel = "trm";

  //string sFile1 = "data12";
  string sFile1 = "top";
  //string sFile1 = "Zjets_Alpgen";
  //string sFile1 = "SimplifiedModel_wA_slep";  //Use lf for signla - classification pb
      
  string PR = sel + "_" + cr + "_" + "pr" + "_" + lep + "_tightNI_" + var;
  string HF = sel + "_" + cr + "_" + "hf" + "_" + lep + "_tightNI_" + var;

  TH1F* _h_pr   = (TH1F*) _ana->getHisto(sFile1,PR);
  TH1F* _h_hf   = (TH1F*) _ana->getHisto(sFile1,HF);


  //ROC curve - rought
  TH1F* _h_roc_pr_d0S = _utils->myTH1F("h_roc_pr_d0S","h_roc_pr_d0S",50,0,10,"d0S","Eff");
  TH1F* _h_roc_hf_d0S = _utils->myTH1F("h_roc_pr_d0S","h_roc_pr_d0S",50,0,10,"d0S","Eff");
  
  for(int i=1; i<=_h_roc_pr_d0S->GetNbinsX(); i++){
    float _val = _h_roc_pr_d0S->GetBinLowEdge(i);
    int negBin = _h_pr->FindBin(- _val);
    int posBin = _h_pr->FindBin( _val);
    float _den = _h_pr->Integral(0,-1);
    float _num = _h_pr->Integral(negBin,posBin);
    float _eff, _effErr;
    _ana->binomialError(_num, _den, _eff, _effErr);
      _h_roc_pr_d0S->SetBinContent(i,_eff);
    _h_roc_pr_d0S->SetBinError(i,_effErr);
    
    _den = _h_hf->Integral(0,-1);
    _num = _h_hf->Integral(negBin,posBin);
    _ana->binomialError(_num, _den, _eff, _effErr);
    _h_roc_hf_d0S->SetBinContent(i,1-_eff);
    _h_roc_hf_d0S->SetBinError(i,_effErr);
  }

  TCanvas* _c2  = _utils->myCanvas("ROC_d0S");
  _utils->myDraw1d(_h_roc_pr_d0S,_c2,1,"e",false,kBlue,false,20);
  _utils->myDraw1d(_h_roc_hf_d0S,_c2,1,"esame",false,kRed,false,20);
  

  //Compare PR -vs HF 
  _utils->normalize(_h_pr,1);
  _h_pr->SetMaximum(_h_pr->GetMaximum()*11);
  _utils->normalize(_h_hf,1);

  TLegend*  _leg1 = new TLegend(0.7,0.7,0.85,0.85);
  _utils->legendSetting(_leg1,0.03);
  TCanvas* _c1  = _utils->myCanvas("IP");
  _utils->myDraw1d(_h_pr,_c1,1,"e",true,kBlue,false,20);
  _utils->myDraw1d(_h_hf,_c1,1,"esame",true,kRed,false,20);
  _leg1->AddEntry(_h_pr,"Prompt ","p");
  _leg1->AddEntry(_h_hf,"HF ","p");
  _leg1->Draw();
  

  float den=0;
  float num=0;
  float cut=0;
  float effPR=0;
  float effHF=0;
  int lowB, highB;
  if(var=="d0S"){
    if(lep=="e") cut=5;
    if(lep=="m") cut=3;
    lowB = _h_pr->FindBin(-cut);
    highB = _h_pr->FindBin(cut);
  }
  else{
    if(lep=="e") cut=0.4;
    if(lep=="m") cut=0.2;
    lowB = _h_pr->FindBin(-cut);
    highB = _h_pr->FindBin(cut);
  }
  den = _h_pr->Integral(0,-1);
  num = _h_pr->Integral(lowB,highB);
  effPR = (den>0) ? num/den : 0;
  den = _h_hf->Integral(0,-1);
  num = _h_hf->Integral(lowB,highB);
  effHF = (den>0) ? num/den : 0;
  
  cout << "Eff-PR @ " << cut << " = " << effPR << endl;
  cout << "Eff-HF @ " << cut << " = " << effHF << endl;

  

}
