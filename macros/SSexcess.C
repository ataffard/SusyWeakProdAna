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

TChain* c_data;
TChain* c_fake;
TChain* c_WW;
TChain* c_WZ;
TChain* c_ZZ;
TChain* c_TOP;
TChain* c_ZJET;



vector<int> _col;
vector<string> _name;

void study(int type=1, bool wBkg=false);

TH1F* bookHist(string var, string name);
void plot(TH1* _hdata, string label, bool logy=true);
void plot(TH1* _hdata, TH1* _hFake, TH1* _hWW,  TH1* _hWZ, TH1* _hZZ, TH1* _hTOP, TH1* _hZJET,
	  string label, bool logy);
TH1F* ratio(TH1* _hmc, TH1* _hdata);

//_____________________________________________________________________________//
int main(int argc, char *argv[]){
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 

  //string ver = "histos_121012_13fb_n0115_DD_MMtrial9_SYS/";
  string ver = "";
  //string test = "ToyNtOutputs_SSexcess_org/";
  //  string test = "ToyNtOutputs_SSexcess_v1/"; //n114
  //string test = "ToyNtOutputs_SSexcess_v2/"; //n115 - temporary
  string test = "ToyNtOutputs_n115_SS/"; //n115
  string dir =  string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/" +  ver + test;


  c_data = new TChain("ToyNt");
  c_data->AddFile(string(dir+"Egamma.periodA_DATA_DIL_CR2LepSS.root").c_str());
  c_data->AddFile(string(dir+"Egamma.periodB_DATA_DIL_CR2LepSS.root").c_str());
  c_data->AddFile(string(dir+"Egamma.periodC_DATA_DIL_CR2LepSS.root").c_str());
  c_data->AddFile(string(dir+"Egamma.periodD_DATA_DIL_CR2LepSS.root").c_str());
  c_data->AddFile(string(dir+"Egamma.periodE_DATA_DIL_CR2LepSS.root").c_str());
  
  c_data->AddFile(string(dir+"Muons.periodA_DATA_DIL_CR2LepSS.root").c_str());
  c_data->AddFile(string(dir+"Muons.periodB_DATA_DIL_CR2LepSS.root").c_str());
  c_data->AddFile(string(dir+"Muons.periodC_DATA_DIL_CR2LepSS.root").c_str());
  c_data->AddFile(string(dir+"Muons.periodD_DATA_DIL_CR2LepSS.root").c_str());
  c_data->AddFile(string(dir+"Muons.periodE_DATA_DIL_CR2LepSS.root").c_str());

  c_fake = new TChain("ToyNt");
  c_fake->AddFile(string(dir+"Muons.periodA_FAKE_DIL_CR2LepSS.root").c_str());
  c_fake->AddFile(string(dir+"Muons.periodB_FAKE_DIL_CR2LepSS.root").c_str());
  c_fake->AddFile(string(dir+"Muons.periodC_FAKE_DIL_CR2LepSS.root").c_str());
  c_fake->AddFile(string(dir+"Muons.periodD_FAKE_DIL_CR2LepSS.root").c_str());
  c_fake->AddFile(string(dir+"Muons.periodE_FAKE_DIL_CR2LepSS.root").c_str());

  c_WW = new TChain("ToyNt");
  c_WW->AddFile(string(dir+"126892_DIL_CR2LepSS.root").c_str());
  c_WW->AddFile(string(dir+"106011_DIL_CR2LepSS.root").c_str());
  c_WW->AddFile(string(dir+"106012_DIL_CR2LepSS.root").c_str());
  c_WW->AddFile(string(dir+"106013_DIL_CR2LepSS.root").c_str());
  c_WW->AddFile(string(dir+"106014_DIL_CR2LepSS.root").c_str());
  c_WW->AddFile(string(dir+"106015_DIL_CR2LepSS.root").c_str());
  c_WW->AddFile(string(dir+"106016_DIL_CR2LepSS.root").c_str());
  c_WW->AddFile(string(dir+"106017_DIL_CR2LepSS.root").c_str());
  c_WW->AddFile(string(dir+"106018_DIL_CR2LepSS.root").c_str());
  c_WW->AddFile(string(dir+"106019_DIL_CR2LepSS.root").c_str());
  c_WW->AddFile(string(dir+"126988_DIL_CR2LepSS.root").c_str());
  c_WW->AddFile(string(dir+"126989_DIL_CR2LepSS.root").c_str());

  c_WZ = new TChain("ToyNt");
  c_WZ->AddFile(string(dir+"126893_DIL_CR2LepSS.root").c_str());

  c_ZZ = new TChain("ToyNt");
  c_ZZ->AddFile(string(dir+"126894_DIL_CR2LepSS.root").c_str());
  c_ZZ->AddFile(string(dir+"126895_DIL_CR2LepSS.root").c_str());
  c_ZZ->AddFile(string(dir+"116600_DIL_CR2LepSS.root").c_str());
  c_ZZ->AddFile(string(dir+"116601_DIL_CR2LepSS.root").c_str());
  c_ZZ->AddFile(string(dir+"116602_DIL_CR2LepSS.root").c_str());
  c_ZZ->AddFile(string(dir+"116603_DIL_CR2LepSS.root").c_str());

  c_TOP = new TChain("ToyNt");
  c_TOP->AddFile(string(dir+"117800_DIL_CR2LepSS.root").c_str());
  c_TOP->AddFile(string(dir+"117801_DIL_CR2LepSS.root").c_str());
  c_TOP->AddFile(string(dir+"117802_DIL_CR2LepSS.root").c_str());
  c_TOP->AddFile(string(dir+"108346_DIL_CR2LepSS.root").c_str());
  c_TOP->AddFile(string(dir+"119353_DIL_CR2LepSS.root").c_str());
  c_TOP->AddFile(string(dir+"119354_DIL_CR2LepSS.root").c_str());
  c_TOP->AddFile(string(dir+"119355_DIL_CR2LepSS.root").c_str());
  c_TOP->AddFile(string(dir+"119356_DIL_CR2LepSS.root").c_str());
  c_TOP->AddFile(string(dir+"119583_DIL_CR2LepSS.root").c_str());

  //Z+jets and Z->tautau give no event in mm
  c_ZJET= new TChain("ToyNt");
  c_ZJET->AddFile(string(dir+"147770_DIL_CR2LepSS.root").c_str());
  c_ZJET->AddFile(string(dir+"147771_DIL_CR2LepSS.root").c_str());
  c_ZJET->AddFile(string(dir+"147772_DIL_CR2LepSS.root").c_str());


  _col.push_back(kBlack); //data
  _col.push_back(kGray);  //fake        
  _col.push_back(kAzure+4); //WW    
  _col.push_back(kMagenta-3);  //WZ
  _col.push_back(kGreen+2);   //ZZ       
  _col.push_back(kRed+1);  //TOP   
  _col.push_back(kOrange-4);     
  _col.push_back(kGreen+3);     

  _name.push_back("Data");     
  _name.push_back("Fake");     
  _name.push_back("WW");     
  _name.push_back("WZ");     
  _name.push_back("ZZ");     
  _name.push_back("TOP"); 
  _name.push_back("ZJET"); 

  


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

  if(VAR.Contains("sqrt")){
    nbin=15; xL=0; xH=150;
    sX="m^{W}_{T} [GeV]";
  }
  else if(VAR.CompareTo("metrel")==0 ||
	  VAR.CompareTo("met")==0 ||
	  VAR.Contains("ref")    ||
	  VAR.Contains("cellout")    
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
  else if(VAR.CompareTo("mll")==0){
    //nbin=20; xL=0; xH=200; //10GeV
    nbin=40; xL=0; xH=200; //5GeV
    //nbin=30; xL=0; xH=300; ///10GeV
    //    nbin=25; xL=50; xH=150; //4GeV - Kanisha binning
    sX="m_{ll} [GeV]";
  }
  else if(VAR.Contains("pt")){
    nbin=20; xL=0; xH=100;
    sX="P_{T} [GeV]";
  }
  else if(VAR.Contains("eta")){
    nbin=25; xL=-2.5; xH=2.5;
    sX="#eta";
  }
  else if(VAR.Contains("phi")){
    nbin=16; xL=-3.2; xH=3.2;
    sX="#phi [rad]";
  }
  else if(VAR.Contains("l_q")){
    nbin=5; xL=-2.5; xH=2.5;
    sX="P_{T} [GeV]";
  }
  else if(VAR.Contains("nSJets")) {
    nbin=10; xL=-0.5; xH=9.5;
    sX="N_{jets}";
  }
  else if(VAR.CompareTo("pTll")==0){
    nbin=20; xL=0; xH=100;
    sX="p_{T}^{ll} [GeV]";
  }

  _h = _utils->myTH1F(name.c_str(),name.c_str(),nbin,xL,xH,sX.c_str(),sY.c_str());
  return _h;
  
}



//_____________________________________________________________________________//
void study(int type, bool wBkg){
  string slep="_ee";
  string SLEP="_EE_";
  if(type==1){
    slep = "_mm";
    SLEP = "_MM_";
  }
  if(type==2){
    slep = "_em";
    SLEP = "_EM_";
  }

  TCut EE("llType==0");
  TCut MM("llType==1");
  TCut EM("llType==2");


  //
  //start modif
  //
  bool logy=false;//true;

  TCut SJ("j_isC25 ||j_isB20 ||j_isF30");

  TCut SEL("metrel>40 ");
  //TCut SEL(SJ && "nSJets==1 && (mll>100 & mll<110) && l_pt[0]>30 ");
  //TCut SEL("nSJets==1 && metrel>40 ");
  //TCut SEL("nSJets==1 && met>40 && l_pt[0]>30");
  //TCut SEL("nSJets==1 && metrel>40");
  //TCut SEL("nSJets==1 && metrel>40 && l_pt[0]>30");
  //  TCut SEL(SJ && "nSJets==1 && metrel>40 && l_pt[0]>30 && (mll>100 & mll<110)");
  //TCut SEL(SJ && "nSJets==1 && l_pt[0]>30 && (mll>100 & mll<110)");
  //TCut SEL("nSJets==1 && metrel>40 && mll>90 & mll<120 && l_pt[0]");
  //TCut SEL("nSJets<2 && metrel>40 ");

  //string var = "l_pt[1]";
  //string var = "l_q[0]+l_q[1]";
  //string var = "sqrt(2*l_pt[0]*met * (1- cos(acos(cos(l_phi[0]-met_phi)))))";
  string var = "mll";
  //string var = "pTll";
  //string var = "met";
  //string var = "metrel";
  //string var = "met_refEle";
  //string var = "met_refMuo";
  //string var = "met_refJet";
  //string var = "met_cellout";
  //  string var = "acos(cos(l_phi[1]-met_phi))";
  //string var = "nSJets";
  //string var = "j_pt";
  //string var = "j_eta";
  //string var = "j_jvf";

  string sName = "CR2LepSS_VR1SS"+ SLEP + var;
  //string sName = "CR2LepSS_Jveto"+ SLEP + var;
  //  string sName = "CR2LepSS_1J"+ SLEP + var;
  //string sName = "CR2LepSS_1Jptl030Mll100-110"+ SLEP + var;
  //string sName = "CR2LepSS_1JMetrel40"+ SLEP + var;
  //string sName = "CR2LepSS_Metrel40Mll90-120"+ SLEP + var;
  //string sName = "CR2LepSS_1JMet40ptl030"+ SLEP + var;
  //  string sName = "CR2LepSS_1JMetrel40"+ SLEP + var;
  //string sName = "CR2LepSS_1JMetrel40ptl030"+ SLEP + var;
  //string sName = "CR2LepSS_LE1JMetrel40"+ SLEP + var;
  //string sName = "CR2LepSS_1JMetrel40Mll90-120"+ SLEP + var;
  //  string sName = "CR2LepSS_1JMetrel40ptl030mll102-106"+ SLEP + var;
  //string sName = "CR2LepSS_1JMetrel40ptl030mll100-110"+ SLEP + var;

  //string sName = "CR2LepSS_1Jptl030IN"+ SLEP + var;
  //string sName = "CR2LepSS_1JMetrel40ptl030IN"+ SLEP + var;
  //string sName = "CR2LepSS_1JMetrel40ptl030OUT"+ SLEP + var;

  //
  //end modif
  //


  //No changes needed below
  string hName ="hdata"+slep;

  TH1F* hdata = bookHist(var,hName);
  if(hdata==NULL) abort();

  TCut CEE( (SEL && EE  ) * "w");
  TCut CMM( (SEL && MM  ) * "w");
  TCut CEM( (SEL && EM  ) * "w");

  if(wBkg){
    TH1F* hFake = (TH1F*)  hdata->Clone();
    hFake->SetTitle(string("hFake"+slep).c_str());
    hFake->SetName(string("hFake"+slep).c_str());

    TH1F* hWW = (TH1F*)  hdata->Clone();
    hWW->SetTitle(string("hWW"+slep).c_str());
    hWW->SetName(string("hWW"+slep).c_str());

    TH1F* hWZ = (TH1F*)  hdata->Clone();
    hWZ->SetTitle(string("hWZ"+slep).c_str());
    hWZ->SetName(string("hWZ"+slep).c_str());

    TH1F* hZZ = (TH1F*)  hdata->Clone();
    hZZ->SetTitle(string("hZZ"+slep).c_str());
    hZZ->SetName(string("hZZ"+slep).c_str());

    TH1F* hTOP = (TH1F*)  hdata->Clone();
    hTOP->SetTitle(string("hTOP"+slep).c_str());
    hTOP->SetName(string("hTOP"+slep).c_str());

    TH1F* hZJET = (TH1F*)  hdata->Clone();
    hZJET->SetTitle(string("hZJET"+slep).c_str());
    hZJET->SetName(string("hZJET"+slep).c_str());
  }


  //Fill histo
  if(type==0){
    string cmd1 = var + ">>hdata_ee";
    c_data->Draw(cmd1.c_str(),CEE,"goff");
    if(wBkg){
      cmd1 = var + ">>hFake_ee";
      c_fake->Draw(cmd1.c_str(),CEE,"goff");
      cmd1 = var + ">>hWW_ee";
      c_WW->Draw(cmd1.c_str(),CEE,"goff");
      cmd1 = var + ">>hWZ_ee";
      c_WZ->Draw(cmd1.c_str(),CEE,"goff");
      cmd1 = var + ">>hZZ_ee";
      c_ZZ->Draw(cmd1.c_str(),CEE,"goff");
      cmd1 = var + ">>hTOP_ee";
      c_TOP->Draw(cmd1.c_str(),CEE,"goff");
      cmd1 = var + ">>hZJET_ee";
      c_ZJET->Draw(cmd1.c_str(),CEE,"goff");
    }
  }
  else if (type==1) {
    string cmd1 = var + ">>hdata_mm";
    c_data->Draw(cmd1.c_str(),CMM,"goff");
    if(wBkg){
      cmd1 = var + ">>hFake_mm";
      c_fake->Draw(cmd1.c_str(),CMM,"goff");
      cmd1 = var + ">>hWW_mm";
      c_WW->Draw(cmd1.c_str(),CMM,"goff");
      cmd1 = var + ">>hWZ_mm";
      c_WZ->Draw(cmd1.c_str(),CMM,"goff");
      cmd1 = var + ">>hZZ_mm";
      c_ZZ->Draw(cmd1.c_str(),CMM,"goff");
      cmd1 = var + ">>hTOP_mm";
      c_TOP->Draw(cmd1.c_str(),CMM,"goff");
      cmd1 = var + ">>hZJET_mm";
      c_ZJET->Draw(cmd1.c_str(),CMM,"goff");
    }
  }
 else if (type==2) {
    string cmd1 = var + ">>hdata_em";
    c_data->Draw(cmd1.c_str(),CEM,"goff");
    if(wBkg){
      cmd1 = var + ">>hFake_em";
      c_fake->Draw(cmd1.c_str(),CEM,"goff");
      cmd1 = var + ">>hWW_em";
      c_WW->Draw(cmd1.c_str(),CEM,"goff");
      cmd1 = var + ">>hWZ_em";
      c_WZ->Draw(cmd1.c_str(),CEM,"goff");
      cmd1 = var + ">>hZZ_em";
      c_ZZ->Draw(cmd1.c_str(),CEM,"goff");
      cmd1 = var + ">>hTOP_em";
      c_TOP->Draw(cmd1.c_str(),CEM,"goff");
      cmd1 = var + ">>hZJET_em";
      c_ZJET->Draw(cmd1.c_str(),CEM,"goff");
    }
  }

  if(wBkg)   plot(hdata,hFake,hWW,hWZ,hZZ,hTOP,hZJET, sName,logy);
  else       plot(hdata,sName,logy);

}

//_____________________________________________________________________________//
void plot(TH1* _hdata, string label, bool logy)
{
  const float maxScaleLin=1.2;
  const float maxScaleLog=11;
  float scale=maxScaleLin;
  if(logy)  scale=maxScaleLog;
  _hdata->SetMaximum(_hdata->GetMaximum()*scale);


  TCanvas* _c1  = _utils->myCanvas(label.c_str());

  TLegend*  _leg = new TLegend(0.45,0.55,0.85,0.93);
  _utils->legendSetting(_leg,0.05); 

  _utils->myDraw1d(_hdata,_c1,1,"e",logy,kBlack,false,20);
  _leg->AddEntry(_hdata,_name[0].c_str(),"p");
  _leg->Draw();
  _utils->myText(0.1,0.95,kBlack,label.c_str(),0.05);
 
  string _sLogy = "";
  if(logy) _sLogy = "_logy";
  string fName= "SSexcess_" + label + _sLogy;
  _c1->SaveAs((fName+".png").c_str());

}


//_____________________________________________________________________________//
void plot(TH1* _hdata, TH1* _hFake, TH1* _hWW, TH1* _hWZ, TH1* _hZZ, TH1* _hTOP,  TH1* _hZJET,
	  string label, bool logy)
{

  const float maxScaleLin=1.2;
  const float maxScaleLog=11;
  float scale=maxScaleLin;
  if(logy)  scale=maxScaleLog;

  TLegend*  _leg = new TLegend(0.7,0.6,0.9,0.9);
  _utils->legendSetting(_leg,0.05); 


  //Build Stack
  TH1F* _hBkgTot = (TH1F*) _hdata->Clone();
  _hBkgTot->Reset();
  _hBkgTot->SetTitle(string("hStackH_"+label).c_str());
  _hBkgTot->SetName(string("hStackH_"+label).c_str());

  THStack* _hBkgTotS = new THStack(string("hStack_"+label).c_str(),
				   string("hStack_"+label).c_str());
  
  _hBkgTot->Add(_hFake);
  _hBkgTot->Add(_hWW);
  _hBkgTot->Add(_hWZ);
  _hBkgTot->Add(_hZZ);
  _hBkgTot->Add(_hTOP);
  _hBkgTot->Add(_hZJET);

  _utils->addToTHStack(_hBkgTotS,(TH1F*) _hFake,_col[1],"HIST",_leg,_name[1].c_str());
  _utils->addToTHStack(_hBkgTotS,(TH1F*) _hWW,_col[2],"HIST",_leg,_name[2].c_str());
  _utils->addToTHStack(_hBkgTotS,(TH1F*) _hWZ,_col[3],"HIST",_leg,_name[3].c_str());
  _utils->addToTHStack(_hBkgTotS,(TH1F*) _hZZ,_col[4],"HIST",_leg,_name[4].c_str());
  _utils->addToTHStack(_hBkgTotS,(TH1F*) _hTOP,_col[5],"HIST",_leg,_name[5].c_str());
  _utils->addToTHStack(_hBkgTotS,(TH1F*) _hZJET,_col[6],"HIST",_leg,_name[6].c_str());

  _hdata->SetMaximum(_utils->getMax(_hBkgTot,_hdata)*scale);

  //Get ratio data/SM
  TH1F*  _ratioH=ratio(_hBkgTot, _hdata);


  TCanvas* _c1  = _utils->myCanvas(label.c_str());
  TPad* _pTop = NULL;
  TPad* _pBot = NULL;
  _pTop = new TPad("pTop","pTop",0,0.3,1,1);
  _pBot = new TPad("pBot","pBot",0,0,1,0.3);
  TVirtualPad* _tv = _c1->cd();

  //Top plot
  _pTop->SetTopMargin(0.08);
  _pTop->SetBottomMargin(0.4);
  _pTop->SetRightMargin(0.05);
  _pTop->SetLeftMargin(0.15);
  _pTop->SetNumber(1);
   if(_hdata){
    _pTop->SetBottomMargin(0.05);
    _pBot->SetTopMargin(0);
    _pBot->SetBottomMargin(0.4);
    _pBot->SetRightMargin(0.05);
    _pBot->SetLeftMargin(0.15);
    _pBot->SetNumber(2);
  }
  _tv->cd();
  _pTop->Draw();
  _pTop->cd();

  
  _utils->myDraw1d(_hdata,_pTop,1,"e",logy,kBlack,false,20);
  _hBkgTotS->Draw("histsame");
  _utils->myDraw1d(_hdata,_pTop,1,"esame",logy,kBlack,false,20);
  _leg->Draw();
  _utils->myText(0.1,0.95,kBlack,label.c_str(),0.05);

  _pTop->RedrawAxis();
  _pTop->Update();
  
  //Bottom Ratio
  _tv->cd();
  if(_ratioH){
    _pBot->Draw();
    _pBot->cd();
     _utils->myDraw1d(_ratioH,_pBot,1,"pe0",false, _col[0], false,20);
    _ratioH->SetMarkerSize(0.3);

    TLine* _line = new TLine(_ratioH->GetBinCenter(1)-_ratioH->GetBinWidth(1)/2,1,
			     _ratioH->GetBinCenter(_ratioH->GetNbinsX())+
			     _ratioH->GetBinWidth(_ratioH->GetNbinsX())/2,1);

    float min = _ratioH->GetMinimum()*1.2;
    float max = _ratioH->GetMaximum()*1.2;

    _line->SetLineStyle(7);
    _line->SetLineWidth(1);
    _line->SetLineColor(kRed);
    _line->Draw();
    _pBot->RedrawAxis();
    
    if(min<0) min=-1;
    if(max>1) max=2;
    min=0;
    max=2;
    _ratioH->GetYaxis()->SetRangeUser(min,max);
    _pBot->Update();
  }

  string _sLogy = "";
  if(logy) _sLogy = "_logy";
  string fName= "SSexcess_stack_" + label + _sLogy;
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
