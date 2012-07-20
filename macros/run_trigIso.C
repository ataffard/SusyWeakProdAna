/*
To run 

.L ../macros/run_IsoPlots.C
main();

*/

#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"

typedef unsigned uint;

TGuiUtils* _utils;
DrawPlots* _ana;

string _date="051812";

void trigEff(string lep, string var, string dep);

void make_All();

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
// Make all plots
//-----------------------------------------------------------
void make_All(){

  trigEff("m","pt", "Npv");
  trigEff("m","ptCone20Rel", "Npv");

  trigEff("e","pt", "Npv");
  trigEff("e","ptCone20Rel", "Npv");

}


//-----------------------------------------------------------
// trigger Eff -vs- pt
// trigEff("m","pt", "Npv")
// trigEff("m","ptCone20Rel", "Npv")
// trigEff("m","pt_wiso", "Npv")
//-----------------------------------------------------------
void trigEff(string lep, string var, string dep)
{
  //Last 2 are 2012 triggers
  std::vector<string> eTRIG;
  eTRIG.push_back("e20MED");
  eTRIG.push_back("e22MED");
  eTRIG.push_back("e22VHMED1");
  //  eTRIG.push_back("e24MVHIED1");
  //  eTRIG.push_back("620MED1");

  std::vector<string> mTRIG;
  mTRIG.push_back("mu18");
  mTRIG.push_back("mu18MED");
  //  mTRIG.push_back("mu24i");
  //  mTRIG.push_back("mu40");

  std::vector<string> lTRIG;
  lTRIG.clear();

  TString LEP(lep);
  TString DEP(dep);
  
  _pathPlots =  string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/histos_" + 
    _date + "/Plots/PerfIso";
  gSystem->mkdir(_pathPlots.c_str(),kTRUE);


  if(LEP.Contains("e")){
    for(uint i=0; i< eTRIG.size(); i++)
      lTRIG.push_back(eTRIG[i]);
  }
  if(LEP.Contains("m")){
    for(uint i=0; i< mTRIG.size(); i++)
      lTRIG.push_back(mTRIG.at(i));
  }
  
  for(uint i=0; i< lTRIG.size(); i++){
    string _hName, _hNameP;
    _hName  = "sel_pr_" + lep + "_" + var + "_" + dep + "_TG" + _ana->itos(i+1);
    _hNameP = "sel_pr_" + lep + "_" + var + "_" + dep + "_pTrig_TG" + _ana->itos(i+1);
    //    _hName  = "sel_pr_" + lep + "_pt_" + dep + "_iso" + "_TG" + _ana->itos(i+1);
    //    _hNameP = "sel_pr_" + lep + "_pt_" + dep + "_iso" + "_pTrig_TG" + _ana->itos(i+1);

    TH2F* _h2_den   = (TH2F*) _ana->getHisto("data11_All",_hName);
    TH2F* _h2_num   = (TH2F*) _ana->getHisto("data11_All",_hNameP);
    
    string sCanvas = "Trigger Eff " + lTRIG[i];
    TCanvas* _c0  = _utils->myCanvas(sCanvas.c_str());

    TLegend*  _leg0 = new TLegend(0.5,0.35,0.75,0.55); 
    _utils->legendSetting(_leg0); 

    TH1F* _h_ratio[3];
    for(uint j=0; j<2/*3*/; j++){
      int inpv1, inpv2;
      if(j==0){inpv1=0; inpv2=10;}
      if(j==1){inpv1=11; inpv2=20;}
      if(j==2){inpv1=21; inpv2=31;}
      string _jTitle = _ana->itos(inpv1) + "<" + dep + "<" + _ana->itos(inpv2);
      string _sName = "Eff_Trig_Pt_" + _ana->itos(inpv1) + "_" + _ana->itos(inpv2);
      
      TH1F* _h_den = (TH1F*) _h2_den->ProjectionX("den_x",inpv1,inpv2);
      TH1F* _h_num = (TH1F*) _h2_num->ProjectionX("num_x",inpv1,inpv2);
      
      _h_ratio[j] = _ana->calcRatio(_h_num, _h_den,_sName.c_str());
      
      string opt="esame";
      if(j==0) opt="e";
      _utils->myDraw1d(_h_ratio[j],_c0,1,opt.c_str(),false,j+2,false,20);
      _leg0->AddEntry(_h_ratio[j],_jTitle.c_str(),"p");
    }
    _leg0->Draw();
    _utils->myText(0.2,0.9,kBlack,lTRIG[i].c_str(),0.03);
    gPad->Modified();

    string name = "trigIso_Data_pr_" + lep + "_" + var + "_" + dep + "_" + lTRIG[i];
    string fName= _pathPlots + "/" + name;
    std::cout << "Saving plot: " << name <<std::endl;
    _c0->SaveAs((fName+".png").c_str());    

  }//loop triggers

}
