#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"
typedef unsigned uint;

static const float sigLumi=0.039;

string dir = "histos_020413_13fb_n0124_HCP_DD/";
string sFile = "histo_data12_std.root";

TGuiUtils* _utils;
DrawPlots* _ana;

TH1F* ratio(TH1F* hnum, TH1F* hden, string name);


int main(int argc, char *argv[]){
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 

  //Get the Z count per run
  string _pathHisto  = string(getenv("WORKAREA")) + "/histoAna/SusyAna/" +dir;
  TFile* _f = new TFile(string(_pathHisto + sFile).c_str(),"READ");
  
  string sHName ="DG2L_CRZ_EE_DG2L_Zcount_NOM"; 
  TH1F* _h_Zee = (TH1F*)  _f->Get(sHName.c_str())->Clone();
  
  sHName ="DG2L_CRZ_MM_DG2L_Zcount_NOM"; 
  TH1F* _h_Zmm = (TH1F*)  _f->Get(sHName.c_str())->Clone();
 
  //Set the errors
  for(uint i=0; i<_h_Zmm->GetXaxis()->GetNbins(); i++){
    float err =  sqrt(_h_Zee->GetBinContent(i+1));
    _h_Zee->SetBinError(i+1, err);

    err =  sqrt(_h_Zmm->GetBinContent(i+1));
    _h_Zmm->SetBinError(i+1, err);
  }


  // TCanvas* _c1  = _utils->myCanvas("Zee count");
  // _utils->myDraw1d(_h_Zee,_c1,1,"e",false,kBlack,false,20);

  // TCanvas* _c2  = _utils->myCanvas("Zmm count");
  //_utils->myDraw1d(_h_Zmm,_c2,1,"e",false,kBlack,false,20);

  //Prep lumi histo
  TH1F* _h_lumi = (TH1F*)  _f->Get(sHName.c_str())->Clone();
  _h_lumi->Reset();
  _h_lumi->SetTitle("LumiPerRun");
  _h_lumi->SetName("LumiPerRun");


  //Open the run - lumi file
  string path =  string(getenv("WORKAREA")) + "/SusyWeakProdAna/data";
  string fileName  = path + "/" + "Moriond_lumi_GRL_v58.txt";

  FILE* eventsFile=fopen(fileName.c_str(),"r");
  int run;
  float lumi;

  int ibin=1;
  while(fscanf(eventsFile,"%i %f \n",&run, &lumi) != EOF){
    //cout << "Adding run-event " << run << " " << lumi << endl; 
    _h_lumi->SetBinContent(ibin,lumi);
    _h_lumi->SetBinError(ibin++,lumi*sigLumi);
  }

  //  TCanvas* _c3  = _utils->myCanvas("Lumi per run");
  //  _utils->myDraw1d(_h_lumi,_c3,1,"e",false,kBlack,false,20);

  
  //Do Z yield
  TH1F* _h_Zee_yield = ratio(_h_Zee, _h_lumi, "Zee_yield");
  TH1F* _h_Zmm_yield = ratio(_h_Zmm, _h_lumi, "Zmm_yield");
  _h_Zee_yield->GetXaxis()->LabelsOption("v");
  _h_Zmm_yield->GetXaxis()->LabelsOption("v");

  TCanvas* _c4  = _utils->myCanvas("Zee yield");
  _utils->myDraw1d(_h_Zee_yield,_c4,1,"e",false,kBlack,false,20);

  TCanvas* _c5  = _utils->myCanvas("Zmm yield");
  _utils->myDraw1d(_h_Zmm_yield,_c5,1,"e",false,kBlack,false,20);


}


TH1F* ratio(TH1F* hnum, TH1F* hden, string name)
{
  TH1F* _h_R = (TH1F*) hnum->Clone();
  _h_R->Reset();
  //_h_R->Sumw2()
  _h_R->SetName(name.c_str()); 
  _h_R->SetTitle(name.c_str());
  _h_R->GetYaxis()->SetTitle("Z Yield");
  _h_R->Divide(hnum,hden,1,1);

  for(uint i=0; i<hnum->GetXaxis()->GetNbins(); i++){
    float num = hnum->GetBinContent(i+1);
    float numErr = hnum->GetBinError(i+1);
    
    float den = hden->GetBinContent(i+1);
    float denErr = hden->GetBinError(i+1);

    float err = sqrt(pow(numErr/num,2) + pow(denErr/den,2));
    _h_R->SetBinError(i+1, err);
  }

  return _h_R;
}
