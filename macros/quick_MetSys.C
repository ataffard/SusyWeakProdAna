#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"

typedef unsigned uint;


TGuiUtils* _utils;
DrawPlots* _ana;

int main(int argc, char *argv[]){
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
 _ana = new DrawPlots(); 
 
 string sChan = "CRZ";
 //string sChan = "ZXCR1";
 
 //string name = "DG2L_"+ sChan + "_EE_DG2L_etmiss";
 // string name = "DG2L_"+ sChan + "_EE_DG2L_metrel";
 
 //string name = "DG2L_"+ sChan + "_EE_DG2L_etmiss";
 //string name = "DG2L_"+ sChan + "_EE_DG2L_metrel";

 //Don't have sys for the sub met terms
 //string name = "DG2L_"+ sChan + "_EE_DG2L_metCellout";
 string name = "DG2L_"+ sChan + "_EE_DG2L_metRefSJet";
 //string name = "DG2L_"+ sChan + "_EE_DG2L_metRefJet";

  string _pathHisto  =  string(getenv("WORKAREA")) + "/histoAna/SusyAna/";
  
  string sFile_nom = "histo_Zee.147770_rlep_wqFlip_13fb_newmap.root";
  string sFile_scale_up = "histo_Zee.147770_rlep_SCALEST_UP.root";
  string sFile_scale_dn = "histo_Zee.147770_rlep_SCALEST_DN.root";
  string sFile_resost = "histo_Zee.147770_rlep_RESOST.root";
  
  TFile* _f_nom      = new TFile(string(_pathHisto + sFile_nom).c_str());
  TH1F* _h_nom = (TH1F*)  _f_nom->Get(name.c_str())->Clone();

  TFile* _f_scale_up = new TFile(string(_pathHisto + sFile_scale_up).c_str());
  TH1F* _h_scale_up = (TH1F*)  _f_scale_up->Get(name.c_str())->Clone();

  TFile* _f_scale_dn = new TFile(string(_pathHisto + sFile_scale_dn).c_str());
  TH1F* _h_scale_dn = (TH1F*)  _f_scale_dn->Get(name.c_str())->Clone();

  TFile* _f_resost   = new TFile(string(_pathHisto + sFile_resost).c_str());
  TH1F* _h_resost = (TH1F*)  _f_resost->Get(name.c_str())->Clone();


  TLegend*  _leg = new TLegend(0.5,0.6,0.8,0.80);
  _utils->legendSetting(_leg); 
  TCanvas* _c0  = _utils->myCanvas(name.c_str());

  _utils->myDraw1d(_h_nom,_c0,1,"e",false,kBlack,false,20);
  _utils->myDraw1d(_h_scale_up,_c0,1,"esame",false,kRed,false,20);
  _utils->myDraw1d(_h_scale_dn,_c0,1,"esame",false,kGreen,false,20);
  _utils->myDraw1d(_h_resost,_c0,1,"esame",false,kBlue,false,20);
  
  _leg->AddEntry(_h_nom,"Zee nom","l");
  _leg->AddEntry(_h_scale_up,"Zee SCALE_UP","l");
  _leg->AddEntry(_h_scale_dn,"Zee SCALE_DN","l");
  _leg->AddEntry(_h_resost,"Zee RESOST","l");
  _leg->Draw();
  _utils->myText(0.1,0.9,kBlack,sChan.c_str(),0.05);

  //
  //Ratio plots
  //
  
  TH1F* _r_scale_up = _ana->calcRatio(_h_scale_up,_h_nom,"R_SCALE_UP");
  TH1F* _r_scale_dn = _ana->calcRatio(_h_scale_dn,_h_nom,"R_SCALE_DN");
  TH1F* _r_resost   = _ana->calcRatio(_h_resost,_h_nom,"R_RESOST");

  TLegend*  _leg1 = new TLegend(0.2,0.2,0.4,0.4);
  _utils->legendSetting(_leg1); 
  TCanvas* _c1  = _utils->myCanvas(string(name+"_Ratio").c_str());
  _utils->myDraw1d(_r_scale_up,_c1,1,"e",false,kRed,false,20);
  _utils->myDraw1d(_r_scale_dn,_c1,1,"esame",false,kGreen,false,20);
  _utils->myDraw1d(_r_resost,_c1,1,"esame",false,kBlue,false,20);

  _leg1->AddEntry(_r_scale_up,"Zee SCALE_UP/NOM","l");
  _leg1->AddEntry(_r_scale_dn,"Zee SCALE_DN/NOM","l");
  _leg1->AddEntry(_r_resost,"Zee RESOST/NOM","l");
  _leg1->Draw();
  _utils->myText(0.1,0.9,kBlack,sChan.c_str(),0.05);



}
