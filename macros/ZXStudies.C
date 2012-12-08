#include "SusyNtuple/TGuiUtils.h"
#include <vector>
#include <string>
#include "TROOT.h"
#include "TFile.h"
#include "TLine.h"
#include "TChain.h"
#include "TEfficiency.h"

#include "SusyWeakProdAna/DrawPlots.h"

TGuiUtils* _utils;
DrawPlots* _ana;

TChain* ZAP;  //Z+jets AlpgenPythia
TChain* ZS;   //Z+jets Sherpa
TChain* data;
vector<int> _col;
vector<string> _name;


void study(int type);
void plot(TH1* _hmc1, TH1* _hmc2, TH1* _hdata, string label, bool logy=true);

TH1F* bookHist(string var, string name);

TH1F* ratio(TH1* _hmc, TH1* _hdata);

void effJVF(int ilep);


//_____________________________________________________________________________//
int main(int argc, char *argv[]){
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 

  //string ver = "histos_112812_13fb_n0114_DD_MMtrial9_SYS/";
  string ver = "";
  string dir =  string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/" +  ver + "ToyNtOutputs/";

  ZAP = new TChain("ToyNt");
  ZAP->AddFile(string(dir+"117650_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"117651_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"117652_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"117653_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"117654_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"117655_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"117660_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"117661_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"117662_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"117663_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"117664_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"117665_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"110805_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"110806_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"110807_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"110808_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"110809_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"110810_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"110811_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"110812_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"110817_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"110818_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"110819_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"110820_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"110821_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"110822_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"110823_DIL_CRZ.root").c_str());
  ZAP->AddFile(string(dir+"110824_DIL_CRZ.root").c_str());


  ZS = new TChain("ToyNt");
  ZS->AddFile(string(dir+"147770_DIL_CRZ.root").c_str());
  ZS->AddFile(string(dir+"147771_DIL_CRZ.root").c_str());

  data = new TChain("ToyNt");
  data->AddFile(string(dir+"Egamma.periodA_DATA_DIL_CRZ.root").c_str());
  data->AddFile(string(dir+"Egamma.periodB_DATA_DIL_CRZ.root").c_str());
  data->AddFile(string(dir+"Egamma.periodC_DATA_DIL_CRZ.root").c_str());
  data->AddFile(string(dir+"Egamma.periodD_DATA_DIL_CRZ.root").c_str());
  data->AddFile(string(dir+"Egamma.periodE_DATA_DIL_CRZ.root").c_str());
  data->AddFile(string(dir+"Muons.periodA_DATA_DIL_CRZ.root").c_str());
  data->AddFile(string(dir+"Muons.periodB_DATA_DIL_CRZ.root").c_str());
  data->AddFile(string(dir+"Muons.periodC_DATA_DIL_CRZ.root").c_str());
  data->AddFile(string(dir+"Muons.periodD_DATA_DIL_CRZ.root").c_str());
  data->AddFile(string(dir+"Muons.periodE_DATA_DIL_CRZ.root").c_str());




  _col.push_back(kRed-4);     //ZS
  _col.push_back(kViolet+1);  //ZAP

  _name.push_back("Sherpa");     
  _name.push_back("Alpgen+Pythia6");
  _name.push_back("Data");     

}
//_____________________________________________________________________________//
TH1F* bookHist(string var,string name)
{
  TString VAR(var);
  TH1F* _h=NULL;
  int nbin;
  float xL;
  float xH;
  string sX;
  string sY="Entries";

  if(VAR.Contains("metrel") ||
     VAR.Contains("met") 
     ){
    nbin=40; xL=0; xH=200;
    if(VAR.CompareTo("met")==0)
      sX="#slash{E}_{T} [GeV]";
    else if(VAR.CompareTo("met_refEle")==0)
      sX="#slash{E}_{T}^{refEle} [GeV]";
    else if(VAR.CompareTo("met_refMuo")==0)
      sX="#slash{E}_{T}^{refMuo} [GeV]";
    else if(VAR.CompareTo("met_refJet")==0)
      sX="#slash{E}_{T}^{refJet} [GeV]";
    else if(VAR.CompareTo("met_cellout")==0)
      sX="#slash{E}_{T}^{cellout} [GeV]";
    else
      sX="#slash{E}_{T}^{Rel} [GeV]";
  }
  else if(VAR.CompareTo("pTll")==0){
    nbin=110; xL=0; xH=220;
    sX="p_{T}^{ll} [GeV]";
  }
  else if(VAR.Contains("nOJets")||
	  VAR.Contains("nJets")) {
    nbin=10; xL=-0.5; xH=9.5;
    sX="N_{jets}";
  }
  else if(VAR.CompareTo("j_pt")==0){
    nbin=40; xL=0; xH=200;
    sX="P_{T} [GeV]";
  }
  else if(VAR.Contains("j_eta")){
    nbin=50; xL=-5; xH=5;
    //nbin=25; xL=-2.5; xH=2.5;
    sX="#eta";
  }
  else if(VAR.Contains("j_jvf")){
    nbin=40; xL=-1; xH=1;
    sX="JVF";
  }
  else if(VAR.Contains("dphi_metl") ||
	  VAR.Contains("dphi_ll") ||
	  VAR.Contains("dphi_Zj") 
	  ){
    nbin=32; xL=0; xH=3.2;
    sX="#delta #phi(l,Met) [rad]";
    if(VAR.CompareTo("dphi_ll")==0)
      sX="#delta #phi(ll,Met) [rad]";
    else if(VAR.CompareTo("dphi_Zj")==0)
      sX="#delta #phi(Z,j) [rad]";
  }
  else if(VAR.CompareTo("j_pt/pTll")==0){
    nbin=100; xL=0; xH=2;
    sX="p_{T}^{recoil}/p_{T}^{Z}";
  }
  
  _h = _utils->myTH1F(name.c_str(),name.c_str(),nbin,xL,xH,sX.c_str(),sY.c_str());
  return _h;

}

//_____________________________________________________________________________//
void study(int type){
  string slep="_ee";
  string SLEP="_EE_";
  if(type==1){
    slep = "_mm";
    SLEP = "_MM_";
  }
  string hName ="hZS"+slep;

  TCut EE("llType==0");
  TCut MM("llType==1");

  //
  //start modif
  //
  bool logy=true;
  //TCut SJET(" ((j_pt<30 && j_jvf>0.25) || j_pt>30) && j_isOJ");



  //TCut SEL("nSJets==0 && npvCorr<12");
  //TCut SEL("nSJets==0 && pTll>80 && j_isOJ");
  //TCut SEL("nSJets==0 && pTll<40 && npvCorr>12");
  //  TCut SEL("nSJets==0 && pTll>80 && oj_pt[0]>30");
  //TCut SEL("nSJets==0 && pTll>80 && oj_pt<30");
  //  TCut SEL("nSJets==0 && pTll>80 && oj_jvf[0]<0.25");
  //TCut SEL("nSJets==0 && pTll<40 && oj_jvf>0.25");
  //TCut SEL(!SJET && "nSJets==0");

  //TCut SEL("j_isRecoil");
  //  TCut SEL("j_isRecoil && dphi_Zj>2.5");
  TCut SEL("dphi_Zj>2.5 && !j_isRecoil && !j_isSublead && j_isTruth");


  //string var = "met";
  //string var = "metrel";
  //  string var = "met_refEle";
  //string var = "met_refMuo";
  //string var = "met_refJet";
  //string var = "met_cellout";
  //string var = "pTll";
  //string var = "nJets";
  //string var = "j_pt";
  //string var = "j_eta";
  string var = "j_jvf";
  //  string var = "dphi_metl[1]";
  //string var = "j_pt/pTll";
  //string var = "dphi_Zj";


  //string sName = "CRZrecoil"+ SLEP + var;
  string sName = "CRZrecoil_dphiZJ25"+ SLEP + var;
  //string sName = "CRZjveto"+ SLEP + var;
  //string sName = "CRZjveto_pTll80"+ SLEP + var;
  //string sName = "CRZjveto_pTll80_ojPtGT30"+ SLEP + var;
  //  string sName = "CRZjveto_pTll80_oj1JVFLT025"+ SLEP + var;
  //  string sName = "CRZjveto_pTllLT40"+ SLEP + var;  
  //  string sName = "CRZjveto_pTllLT40_ojJVFGT025"+ SLEP + var;  
  //string sName = "CRZjveto_pTllLT40_npvLT12"+ SLEP + var;  

  //
  //end modif
  //


  //No changes needed below
  TH1F* hZS = bookHist(var,hName);
  if(hZS==NULL) abort();

  TCut CEE( (SEL && EE  ) * "w");
  TCut CMM( (SEL && MM  ) * "w");


  TH1F* hZAP = (TH1F*)  hZS->Clone();
  hZAP->SetTitle(string("hZAP"+slep).c_str());
  hZAP->SetName(string("hZAP"+slep).c_str());

  TH1F* hdata = (TH1F*)  hZS->Clone();
  hdata->SetTitle(string("hdata"+slep).c_str());
  hdata->SetName(string("hdata"+slep).c_str());

  //Fill histo
  if(type==0){
    string cmd1 = var + ">>hZS_ee";
    string cmd2 = var + ">>hZAP_ee";
    string cmd3 = var + ">>hdata_ee";
    ZS->Draw(cmd1.c_str(),CEE,"goff");
    ZAP->Draw(cmd2.c_str(),CEE,"goff");
    data->Draw(cmd3.c_str(),CEE,"goff");
  }
  else{
    string cmd1 = var + ">>hZS_mm";
    string cmd2 = var + ">>hZAP_mm";
    string cmd3 = var + ">>hdata_mm";
    ZS->Draw(cmd1.c_str(),CMM,"goff");
    ZAP->Draw(cmd2.c_str(),CMM,"goff");
    data->Draw(cmd3.c_str(),CMM,"goff");
  }

  plot(hZS, hZAP, hdata,sName,logy);

}


//_____________________________________________________________________________//
void plot(TH1* _hmc1, TH1* _hmc2, TH1* _hdata, string label, bool logy)
{

  const float maxScaleLin=1.2;
  const float maxScaleLog=11;
  float scale=maxScaleLin;
  if(logy)  scale=maxScaleLog;
  _hmc1->SetMaximum(_utils->getMax(_hmc1,_hmc2)*scale);


  //Get ratio data/SM
  TH1F*  _ratioH1=ratio(_hmc1, _hdata);
  TH1F*  _ratioH2=ratio(_hmc2, _hdata);

  TCanvas* _c1  = _utils->myCanvas(label.c_str());
  TPad* _pTop = NULL;
  TPad* _pBot = NULL;
  _pTop = new TPad("pTop","pTop",0,0.3,1,1);
  _pBot = new TPad("pBot","pBot",0,0,1,0.3);
  TVirtualPad* _tv = _c1->cd();


  _pTop->SetTopMargin(0.08);
  _pTop->SetBottomMargin(0.4);
  _pTop->SetRightMargin(0.05);
  _pTop->SetLeftMargin(0.15);
  _pTop->SetNumber(1);
  if(hdata_ee){
    _pTop->SetBottomMargin(0.05);
    _pBot->SetTopMargin(0);
    _pBot->SetBottomMargin(0.4);
    _pBot->SetRightMargin(0.05);
    _pBot->SetLeftMargin(0.15);
    _pBot->SetNumber(2);
  }
  _tv->cd();

  //Top plot
  _pTop->Draw();
  _pTop->cd();

  TLegend*  _leg = new TLegend(0.45,0.55,0.85,0.93);
  _utils->legendSetting(_leg,0.05); 

  _utils->myDraw1d(_hmc1,_pTop,1,"hist",logy,_col[0],false,20);
  _utils->myDraw1d(_hmc2,_pTop,1,"histsame",logy,_col[1],false,20);
  _utils->myDraw1d(_hdata,_pTop,1,"esame",logy,kBlack,false,20);

  _leg->AddEntry(_hmc1,_name[0].c_str(),"l");
  _leg->AddEntry(_hmc2,_name[1].c_str(),"l");
  _leg->AddEntry(_hdata,_name[2].c_str(),"p");
  _leg->Draw();
  _utils->myText(0.1,0.95,kBlack,label.c_str(),0.05);


  _pTop->RedrawAxis();
  _pTop->Update();
  
  //Bottom Ratio
  _tv->cd();
  if(_ratioH1){
    _pBot->Draw();
    _pBot->cd();
     _utils->myDraw1d(_ratioH1,_pBot,1,"pe0",false, _col[0], false,20);
     _utils->myDraw1d(_ratioH2,_pBot,1,"pe0same",false, _col[1], false,20);
    _ratioH1->SetMarkerSize(0.3);
    _ratioH2->SetMarkerSize(0.3);

    TLine* _line = new TLine(_ratioH1->GetBinCenter(1)-_ratioH1->GetBinWidth(1)/2,1,
			     _ratioH1->GetBinCenter(_ratioH1->GetNbinsX())+
			     _ratioH1->GetBinWidth(_ratioH1->GetNbinsX())/2,1);

    float min = _ratioH1->GetMinimum()*1.2;
    float max = _ratioH1->GetMaximum()*1.2;

    _line->SetLineStyle(7);
    _line->SetLineWidth(1);
    _line->SetLineColor(kRed);
    _line->Draw();
    _pBot->RedrawAxis();
    
    if(min<0) min=-1;
    if(max>1) max=2;
    min=0;
    max=2;
    _ratioH1->GetYaxis()->SetRangeUser(min,max);
    _pBot->Update();
  }

  string _sLogy = "";
  if(logy) _sLogy = "_logy";
  string fName= "ZXstudy_" + label + _sLogy;
  _c1->SaveAs((fName+".png").c_str());



}


//_____________________________________________________________________________//
TH1F* ratio(TH1* _hmc, TH1* _hdata){

  TH1F* _ratio= (TH1F*) _hmc->Clone();
  _ratio->Reset();
  _ratio->GetXaxis()->SetTitle(_hmc->GetXaxis()->GetTitle());
  _ratio->GetXaxis()->SetTitleSize(0.15);
  _ratio->GetXaxis()->SetTitleOffset(1.2);
  _ratio->SetLabelSize(0.12,"X");
  _ratio->SetLabelSize(0.12,"Y");
  _ratio->GetYaxis()->SetTitle("Data/SM");
  _ratio->GetYaxis()->SetTitleSize(0.08);
  _ratio->GetYaxis()->SetTitleOffset(0.8);
  _ratio->GetYaxis()->SetNdivisions(205);

  _ratio->Divide(_hdata,_hmc,1,1);

  return _ratio;

}

//_____________________________________________________________________________//
void effJVF(int ilep)
{

  TCut LEP;
  if(ilep==0)  LEP = "llType==0";
  else         LEP = "llType==1";

  TCut SEL("j_isRecoil && dphi_Zj>2.5");
  
  TCut JALL(LEP && SEL);
  TCut J200(JALL && "j_pt>200");
  TCut J100(JALL && "j_pt>100 && j_pt<200");
  TCut J80 (JALL && "j_pt>80 && j_pt<100");
  TCut J50 (JALL && "j_pt>50 && j_pt<80");
  TCut J30 (JALL && "j_pt>30 && j_pt<50");
  TCut J20 (JALL && "j_pt>20 && j_pt<30");


  TH1F* h_ZAP_den = bookHist("j_jvf","h_ZAP_den");
  TH1F* h_ZAP_num200 = bookHist("j_jvf","h_ZAP_num200");
  TH1F* h_ZAP_num100 = bookHist("j_jvf","h_ZAP_num100");
  TH1F* h_ZAP_num80 = bookHist("j_jvf","h_ZAP_num80");
  TH1F* h_ZAP_num50 = bookHist("j_jvf","h_ZAP_num50");
  TH1F* h_ZAP_num30 = bookHist("j_jvf","h_ZAP_num30");
  TH1F* h_ZAP_num20 = bookHist("j_jvf","h_ZAP_num20");

  ZAP->Draw("j_jvf>>h_ZAP_den",JALL,"goff");
  ZAP->Draw("j_jvf>>h_ZAP_num200",J200,"goff");
  ZAP->Draw("j_jvf>>h_ZAP_num100",J100,"goff");
  ZAP->Draw("j_jvf>>h_ZAP_num80",J80,"goff");
  ZAP->Draw("j_jvf>>h_ZAP_num50",J50,"goff");
  ZAP->Draw("j_jvf>>h_ZAP_num30",J30,"goff");
  ZAP->Draw("j_jvf>>h_ZAP_num20",J20,"goff");


  TH1F* ZAP_eff_200   = _ana->calcEff(h_ZAP_num200,1);
  //ZAP_eff_200->GetYaxis()->SetTile("Efficiency");
  TH1F* ZAP_eff_100   = _ana->calcEff(h_ZAP_num100,1);
  //ZAP_eff_100->GetYaxis()->SetTile("Efficiency");
  TH1F* ZAP_eff_80   = _ana->calcEff(h_ZAP_num80,1);
  //ZAP_eff_80->GetYaxis()->SetTile("Efficiency");
  TH1F* ZAP_eff_50   = _ana->calcEff(h_ZAP_num50,1);
  //ZAP_eff_50->GetYaxis()->SetTile("Efficiency");
  TH1F* ZAP_eff_30   = _ana->calcEff(h_ZAP_num30,1);
  //ZAP_eff_30->GetYaxis()->SetTile("Efficiency");
  TH1F* ZAP_eff_20   = _ana->calcEff(h_ZAP_num20,1);
  //ZAP_eff_20->GetYaxis()->SetTile("Efficiency");


  TCanvas* _c1  = _utils->myCanvas("JVF Eff");
  _utils->myDraw1d(ZAP_eff_200,_c1,1,"e",false,kRed,false,20);
  _utils->myDraw1d(ZAP_eff_100,_c1,1,"esame",false,kBlue,false,20);
  _utils->myDraw1d(ZAP_eff_80,_c1,1,"esame",false,kMagenta,false,20);
  _utils->myDraw1d(ZAP_eff_50,_c1,1,"esame",false,kGreen,false,20);
  _utils->myDraw1d(ZAP_eff_30,_c1,1,"esame",false,kViolet,false,20);
  _utils->myDraw1d(ZAP_eff_20,_c1,1,"esame",false,kCyan,false,20);
  




}
