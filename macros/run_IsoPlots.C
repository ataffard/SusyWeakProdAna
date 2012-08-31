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


bool delCanvas=false;


//Iso use in analysis: ie m:ptCone20  e:ptCone20/pt
void iso_Pileup(string lep, string dep, string iso="ptCone20");
void iso_Pileup2(string lep, string dep, string iso="ptCone20");

void perf_IsoCut(string lep="m", string iso="ptCone20", bool relative=false);
void perf_IsoCutRange(string lep="m", string iso="ptCone20", string dep="npv", bool relative=false);

void make_All(int opt=-1, int type=-1);

void compIso(string lep="m", string iso="ptCone", bool relative=false);

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
//Run e/m in 2 jobs due to mem leak
//-----------------------------------------------------------
void make_All(int opt, int type){

  delCanvas=true;


  //
  //All iso - dependency -vs Npv (add mu)
  //
  if(opt==-1 || opt==1){
    for(int ilep=0; ilep<2; ilep++){
      string sLep;
      if(ilep==0 && (type==-1 || type==0)) sLep="e";
      if(ilep==1 && (type==-1 || type==1)) sLep="m";
      if(type==0 && ilep==1) continue;
      if(type==1 && ilep==0) continue;

      for(int ix=0; ix<2; ix++){
	string sX;      
	if(ix==0) sX="npv";
	if(ix==1) sX="mu";
	std::vector<string> varA;
	varA.push_back("ptCone20");
	varA.push_back("ptCone30");
	varA.push_back("ptCone40");
	varA.push_back("ptCone20Rel");
	varA.push_back("ptCone30Rel");
	varA.push_back("ptCone40Rel");
	varA.push_back("etCone20");
	varA.push_back("etCone30");
	varA.push_back("etCone40");
	varA.push_back("etCone20Rel");
	varA.push_back("etCone30Rel");
	varA.push_back("etCone40Rel");
	if(ilep==0){//Electron only
	  varA.push_back("etConeCorr20");
	  varA.push_back("etConeCorr30");
	  varA.push_back("etConeCorr40");
	  varA.push_back("etConeCorr20Rel");
	  varA.push_back("etConeCorr30Rel");
	  varA.push_back("etConeCorr40Rel");
	  varA.push_back("etConeTopoCorr20");
	  varA.push_back("etConeTopoCorr30");
	  varA.push_back("etConeTopoCorr40");
	  varA.push_back("etConeTopoCorr20Rel");
	  varA.push_back("etConeTopoCorr30Rel");
	  varA.push_back("etConeTopoCorr40Rel");
	}
	
	for(int iVar=0; iVar<varA.size(); iVar++){
	  iso_Pileup2(sLep,sX,varA[iVar]);
	}
	
      }
    }
  }

  //
  //Signal Eff -vs Bkg rejection  //obsolete
  //
  //perf_IsoCut("m","ptCone");
  //perf_IsoCut("m","etCone");
  //perf_IsoCut("e","ptCone");
  //perf_IsoCut("e","etCone");
  ////relative iso
  //perf_IsoCut("m","ptCone",true);
  //perf_IsoCut("m","etCone",true);
  //perf_IsoCut("e","ptCone",true);
  //perf_IsoCut("e","etCone",true);


  //
  //Signal Eff -vs Bkg rejection - split pt bins and Npv/mu
  //
  if(opt==-1 || opt==2){
    for(int ilep=0; ilep<2; ilep++){
      string sLep;
      if(ilep==0 && (type==-1 || type==0)) sLep="e";
      if(ilep==1 && (type==-1 || type==1)) sLep="m";
      if(type==0 && ilep==1) continue;
      if(type==1 && ilep==0) continue;
      for(int ix=1; ix<2/*2*/; ix++){
	string sX;      
	if(ix==0) sX="npv";
	if(ix==1) sX="mu";
	std::vector<string> varA;
	varA.push_back("ptCone");
	varA.push_back("etCone");
	if(ilep==0){//Electron only
	  varA.push_back("etConeCorr");
	  varA.push_back("etConeTopoCorr");
	}
	for(int iVar=0; iVar<varA.size(); iVar++){
	  std::cout << "Isolation ROC curves " << std::endl;
	  perf_IsoCutRange(sLep,varA[iVar],sX); 
	  perf_IsoCutRange(sLep,varA[iVar],sX,true);//relative iso 
	  std::cout << "Compare Iso var Data/MC PR/HF " << std::endl;
	  compIso(sLep,varA[iVar]);
	  compIso(sLep,varA[iVar],true);
	}
      }
    }
  }

  


}

//-----------------------------------------------------------
//
// Plot ana isolation dependency on pileup
// lep:e, m; dep:npv, mu(only data)
//
void iso_Pileup(string lep, string dep, string iso)
{
  TString LEP(lep);
  TString DEP(dep);

  TH1F* _h_top;
  TH1F* _h_diBoson;
  TH1F* _h_Wjets;
  TH1F* _h_Zjets;
  TH1F* _h_BB;
  TH1F* _h_data;

  _pathPlots =  string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/histos_" + 
    DATE + "/Plots/PerfIso";
  gSystem->mkdir(_pathPlots.c_str(),kTRUE);

  string _hName = "trm_pr_" + lep + "_iso_" + dep;
  string _iso = "ptCone20";
  if(LEP.Contains("e")) _iso = "ptCone20/pt";
 
  if(!DEP.Contains("mu")){
    //
    //Signal lepton MC truth 
    //
    _h_top       = (TH1F*) _ana->getHisto("top",_hName);
    _h_diBoson   = (TH1F*) _ana->getHisto("diBoson",_hName);
    _h_Wjets     = (TH1F*) _ana->getHisto("Wjets",_hName);
    _h_Zjets     = (TH1F*) _ana->getHisto("Zjets",_hName);
    
    TLegend*  _leg0 = new TLegend(0.65,0.65,0.8,0.85); _utils->legendSetting(_leg0); 
    TCanvas* _c0  = _utils->myCanvas(string("Truth match Real: Iso -vs- "+dep).c_str());
    _utils->myDraw1d(_h_top,_c0,1,"e",false,kOrange-2,false,20);
    _utils->myDraw1d(_h_diBoson,_c0,1,"esame",false,kAzure-4,false,25);
    _utils->myDraw1d(_h_Wjets,_c0,1,"esame",false,kViolet-7,false,24);
    _utils->myDraw1d(_h_Zjets,_c0,1,"esame",false,kViolet-9,false,26);
    if(LEP.Contains("m"))  _h_top->GetYaxis()->SetRangeUser(0,0.2);
    if(LEP.Contains("e"))  _h_top->GetYaxis()->SetRangeUser(0,0.01);
    _h_top->GetYaxis()->SetTitle(_iso.c_str());
    _leg0->AddEntry(_h_top,"Top","p");
    _leg0->AddEntry(_h_diBoson,"diBoson","p");
    _leg0->AddEntry(_h_Wjets,"Wjets","p");
    _leg0->AddEntry(_h_Zjets,"Zjets","p");
    _leg0->Draw();
    _utils->myText(0.15,0.9,kBlack,"MC TruthMatch. Prompt leptons",0.03);
    gPad->Modified();

    string name = "IsoPileup_MC_prompt_" + lep + "_" + iso + "_" + dep;
    string fName= _pathPlots + "/" + name;
    std::cout << "Saving plot: " << name <<std::endl;
    _c0->SaveAs((fName+".png").c_str());    

    //
    //Fake lepton MC truth  (BB) & HF tag probe 
    //
    _hName = "trm_hf_" + lep + "_iso_" + dep;
    _h_top       = (TH1F*) _ana->getHisto("top",_hName);
    _h_diBoson   = (TH1F*) _ana->getHisto("diBoson",_hName);
    _h_Wjets     = (TH1F*) _ana->getHisto("Wjets",_hName);
    _h_Zjets     = (TH1F*) _ana->getHisto("Zjets",_hName);
    _h_BB        = (TH1F*) _ana->getHisto("BB",_hName);
    
    TLegend*  _leg2 = new TLegend(0.65,0.65,0.8,0.85); _utils->legendSetting(_leg2); 
    TCanvas* _c2  = _utils->myCanvas(string("Truth match Fake: Iso -vs- "+dep).c_str());
    _utils->myDraw1d(_h_BB,_c2,1,"e",false,kRed+2,false,27);
    _utils->myDraw1d(_h_top,_c2,1,"esame",false,kOrange-2,false,20);
    _utils->myDraw1d(_h_diBoson,_c2,1,"esame",false,kAzure-4,false,25);
    _utils->myDraw1d(_h_Wjets,_c2,1,"esame",false,kViolet-7,false,24);
    if(LEP.Contains("m"))  _h_BB->GetYaxis()->SetRangeUser(0,5);
    if(LEP.Contains("e"))  _h_BB->GetYaxis()->SetRangeUser(0,0.5);
    _h_BB->GetYaxis()->SetTitle(_iso.c_str());
    
    _leg2->AddEntry(_h_top,"Top","p");
    _leg2->AddEntry(_h_diBoson,"diBoson","p");
    _leg2->AddEntry(_h_Wjets,"Wjets","p");
    _leg2->AddEntry(_h_BB,"b#bar b","p");
    _leg2->Draw();
    _utils->myText(0.15,0.9,kBlack,"MC TruthMatch. HF leptons",0.03);
    gPad->Modified();

    name = "IsoPileup_MC_HF_" + lep + "_" + iso + "_" + dep;
    fName= _pathPlots + "/" + name;
    std::cout << "Saving plot: " << name <<std::endl;
    _c2->SaveAs((fName+".png").c_str());
  }
  
  //
  //Data Z tag probe
  //
  _hName = "sel_pr_" + lep + "_iso_" + dep;
  _h_data  = (TH1F*) _ana->getHisto("data12_All",_hName);
  _h_Zjets = (TH1F*) _ana->getHisto("Zjets",_hName);
  
  TLegend*  _leg1 = new TLegend(0.65,0.65,0.8,0.85); _utils->legendSetting(_leg1); 
  TCanvas* _c1  = _utils->myCanvas(string("Data Z tag-probe: Iso -vs- "+dep).c_str());
  _utils->myDraw1d(_h_data,_c1,1,"e",false,kBlack,false,20);
  _utils->myDraw1d(_h_Zjets,_c1,1,"esame",false,kViolet-9,false,26);
  
  if(LEP.Contains("m"))  _h_data->GetYaxis()->SetRangeUser(0,0.2);
  if(LEP.Contains("e"))  _h_data->GetYaxis()->SetRangeUser(0,0.01);
  _h_data->GetYaxis()->SetTitle(_iso.c_str());
  _leg1->AddEntry(_h_data,"Data","p");
  _leg1->AddEntry(_h_Zjets,"Zjets","p");
  _leg1->Draw();
  _utils->myText(0.15,0.9,kBlack,"Data MC comparison. Z TP",0.03);
  gPad->Modified();

  string name = "IsoPileup_DataMC_prompt_" + lep + "_" + iso + "_" + dep;
  string fName= _pathPlots + "/" + name;
  std::cout << "Saving plot: " << name <<std::endl;
  _c1->SaveAs((fName+".png").c_str());

  //
  //Data BB HF tag-probe
  //
  _hName = "sel_hf_" + lep + "_iso_" + dep;
  _h_data  = (TH1F*) _ana->getHisto("data12_All",_hName);
  _h_BB= (TH1F*) _ana->getHisto("BB",_hName);
  
  TLegend*  _leg3 = new TLegend(0.65,0.65,0.8,0.85); _utils->legendSetting(_leg3); 
  TCanvas* _c3  = _utils->myCanvas(string("Data HF tag-probe: Iso -vs- "+dep).c_str());
  _utils->myDraw1d(_h_data,_c3,1,"e",false,kBlack,false,20);
  _utils->myDraw1d(_h_BB,_c3,1,"esame",false,kRed+2,false,27);
  _h_data->GetYaxis()->SetRangeUser(0,5);
  if(LEP.Contains("m"))  _h_data->GetYaxis()->SetRangeUser(0,5);
  if(LEP.Contains("e"))  _h_data->GetYaxis()->SetRangeUser(0,0.5);
  _h_data->GetYaxis()->SetTitle(_iso.c_str());
  _leg3->AddEntry(_h_data,"Data","p");
  _leg3->AddEntry(_h_BB,"b#bar b","p");
  _leg3->Draw();
  _utils->myText(0.15,0.9,kBlack,"Data MC comparison. HF TP",0.03);
  gPad->Modified();

  name = "IsoPileup_DataMC_HF_" + lep + "_" + iso + "_" + dep;
  fName= _pathPlots + "/" + name;
  std::cout << "Saving plot: " << name <<std::endl;
  _c3->SaveAs((fName+".png").c_str());

}


//-----------------------------------------------------------
//
// Plot isolation dependency on pileup
// lep:e, m; dep:npv, mu(only data)
//
void iso_Pileup2(string lep, string dep, string iso)
{
  TString LEP(lep);
  TString DEP(dep);
  TString ISO(iso);

  TH3F* _h3_top;
  TH3F* _h3_diBoson;
  TH3F* _h3_Wjets;
  TH3F* _h3_Zjets;
  TH3F* _h3_BB;
  TH3F* _h3_data;

  TH1F* _h_top;
  TH1F* _h_diBoson;
  TH1F* _h_Wjets;
  TH1F* _h_Zjets;
  TH1F* _h_BB;
  TH1F* _h_data;

  float max_mc_pr;
  float max_mc_hf;
  float scale = 1.2;

  _pathPlots =  string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/histos_" + 
    DATE + "/Plots/PerfIso";
  gSystem->mkdir(_pathPlots.c_str(),kTRUE);

  //
  //Iso -vs Npv
  //
  string _hName = "trm_pr_" + lep + "_" + iso + "_" + dep;

  //if(!DEP.Contains("mu")){
    //
    //Signal lepton MC truth 
    //
    _h3_top       = (TH3F*) _ana->getHisto("top",_hName);
    _h3_diBoson   = (TH3F*) _ana->getHisto("diBoson_Herwig",_hName);
    _h3_Wjets     = (TH3F*) _ana->getHisto("Wjets",_hName);
    _h3_Zjets     = (TH3F*) _ana->getHisto("Zjets_Alpgen",_hName);
    

    _h_top     = _ana->getProfile3D(_h3_top,"x","y","z");
    _h_top->SetTitle("top_pr");    _h_top->SetName("top_pr");
    _h_diBoson = _ana->getProfile3D(_h3_diBoson,"x","y","z");
    _h_diBoson->SetTitle("diBoson_pr");    _h_diBoson->SetName("diBoson_pr");
    _h_Wjets   = _ana->getProfile3D(_h3_Wjets,"x","y","z");
    _h_Wjets->SetTitle("Wjets_pr");     _h_Wjets->SetName("Wjets_pr");
    _h_Zjets   = _ana->getProfile3D(_h3_Zjets,"x","y","z");
    _h_Zjets->SetTitle("Zjets_pr");    _h_Zjets->SetName("Zjets_pr");
    max_mc_pr = _h_top->GetMaximum();


    TLegend*  _leg0 = new TLegend(0.2,0.2,0.35,0.35); _utils->legendSetting(_leg0); 
    TCanvas* _c0  = _utils->myCanvas(string("Truth match Real: Iso -vs- "+dep).c_str());
    _utils->myDraw1d(_h_top,_c0,1,"e",false,kOrange-2,false,20);
    _utils->myDraw1d(_h_diBoson,_c0,1,"esame",false,kAzure-4,false,25);
    _utils->myDraw1d(_h_Wjets,_c0,1,"esame",false,kViolet-7,false,24);
    _utils->myDraw1d(_h_Zjets,_c0,1,"esame",false,kViolet-9,false,26);
    _h_top->GetYaxis()->SetTitle(iso.c_str());
    _h_top->SetMaximum(max_mc_pr*scale);
    _leg0->AddEntry(_h_top,"Top","p");
    _leg0->AddEntry(_h_diBoson,"diBoson","p");
    _leg0->AddEntry(_h_Wjets,"Wjets","p");
    _leg0->AddEntry(_h_Zjets,"Zjets","p");
    _leg0->Draw();
    _utils->myText(0.15,0.9,kBlack,"MC TruthMatch. Prompt leptons",0.03);
    gPad->Modified();

    string name = "IsoPileup2_MC_prompt_" + lep + "_" + iso + "_" + dep;
    string fName= _pathPlots + "/" + name;
    std::cout << "Saving plot: " << name <<std::endl;
    _c0->SaveAs((fName+".png").c_str());  
    if(delCanvas) delete _c0;

    //
    //Fake lepton MC truth  (BB) & HF tag probe 
    //
    _hName = "trm_hf_" + lep + "_" + iso +  "_" + dep;

    _h3_top       = (TH3F*) _ana->getHisto("top",_hName);
    _h3_diBoson   = (TH3F*) _ana->getHisto("diBoson_Herwig",_hName);
    _h3_Wjets     = (TH3F*) _ana->getHisto("Wjets",_hName);
    _h3_Zjets     = (TH3F*) _ana->getHisto("Zjets_Alpgen",_hName);
    _h3_BB        = (TH3F*) _ana->getHisto("BB",_hName);
    
    _h_top     = _ana->getProfile3D(_h3_top,"x","y","z");
    _h_top->SetTitle("top_hf");    _h_top->SetName("top_hf");
    _h_diBoson = _ana->getProfile3D(_h3_diBoson,"x","y","z");
    _h_diBoson->SetTitle("diBoson_hf"); _h_diBoson->SetName("diBoson_hf");
    _h_Wjets   = _ana->getProfile3D(_h3_Wjets,"x","y","z");
    _h_Wjets->SetTitle("Wjets_hf");    _h_Wjets->SetName("Wjets_hf");
    _h_Zjets   = _ana->getProfile3D(_h3_Zjets,"x","y","z");
    _h_Zjets->SetTitle("Zjets_hf");    _h_Zjets->SetName("Zjets_hf");
    _h_BB      = _ana->getProfile3D(_h3_BB,"x","y","z");
    _h_BB->SetTitle("BB_hf");    _h_BB->SetName("BB_hf");
    max_mc_hf = _h_top->GetMaximum();

    TLegend*  _leg2 = new TLegend(0.2,0.2,0.35,0.35); _utils->legendSetting(_leg2); 
    TCanvas* _c2  = _utils->myCanvas(string("Truth match Fake: Iso -vs- "+dep).c_str());
    _utils->myDraw1d(_h_BB,_c2,1,"e",false,kRed+2,false,27);
    _utils->myDraw1d(_h_top,_c2,1,"esame",false,kOrange-2,false,20);
    _utils->myDraw1d(_h_diBoson,_c2,1,"esame",false,kAzure-4,false,25);
    _utils->myDraw1d(_h_Wjets,_c2,1,"esame",false,kViolet-7,false,24);
    _h_BB->GetYaxis()->SetTitle(iso.c_str());
    _h_BB->SetMaximum(max_mc_hf*scale);

    _leg2->AddEntry(_h_top,"Top","p");
    _leg2->AddEntry(_h_diBoson,"diBoson","p");
    _leg2->AddEntry(_h_Wjets,"Wjets","p");
    _leg2->AddEntry(_h_BB,"b#bar b","p");
    _leg2->Draw();
    _utils->myText(0.15,0.9,kBlack,"MC TruthMatch. HF leptons",0.03);
    gPad->Modified();

    name = "IsoPileup2_MC_HF_" + lep + "_" + iso + "_" + dep;
    fName= _pathPlots + "/" + name;
    std::cout << "Saving plot: " << name <<std::endl;
    _c2->SaveAs((fName+".png").c_str());
    if(delCanvas) delete _c2;
    //}

  //
  //Data Ztag probe 
  //

  _hName = "sel_pr_" + lep + "_" + iso + "_" + dep;
  
  _h3_data  = (TH3F*) _ana->getHisto("data12",_hName);
  _h3_Zjets = (TH3F*) _ana->getHisto("Zjets_Alpgen",_hName);

  _h_data   = _ana->getProfile3D(_h3_data,"x","y","z");
  _h_data->SetTitle("data_pr");  _h_data->SetName("data_pr");  
  _h_Zjets   = _ana->getProfile3D(_h3_Zjets,"x","y","z");
  _h_Zjets->SetTitle("Zjets_pr");  _h_Zjets->SetName("Zjets_pr");
  max_mc_pr = _h_data->GetMaximum();

  string _sFunc = "pol1";
  if(LEP.CompareTo("m")==0 && ISO.Contains("etCone")) _sFunc="pol2";
  TF1* _f_data = new TF1("f_data",_sFunc.c_str());
  TF1* _f_Zjets = new TF1("f_Zjets",_sFunc.c_str());
  _f_Zjets->SetLineColor(kViolet-9);
  _h_data->Fit("f_data");
  _h_Zjets->Fit("f_Zjets");


  TLegend*  _leg1 = new TLegend(0.2,0.2,0.35,0.35); _utils->legendSetting(_leg1); 
  TCanvas* _c1  = _utils->myCanvas(string("Data Z tag-probe: Iso -vs- "+dep).c_str());
  _utils->myDraw1d(_h_data,_c1,1,"e",false,kBlack,false,20);
  _utils->myDraw1d(_h_Zjets,_c1,1,"esame",false,kViolet-9,false,26);
  _h_data->GetYaxis()->SetTitle(iso.c_str());
  _h_data->SetMaximum(max_mc_pr*scale);
  _leg1->AddEntry(_h_data,"Data","p");
  _leg1->AddEntry(_h_Zjets,"Zjets","p");
  _leg1->Draw();
  _utils->myText(0.15,0.9,kBlack,"Data MC comparison. Z TP",0.03);
  gPad->Modified();
  
  std::cout <<"Data Fit " << std::endl;  
  for(int ip=0; ip< _f_data->GetNpar(); ip++){
    std::cout << "\tp"<<ip << " " << _f_data->GetParameter(ip) << " +/- " << _f_data->GetParError(ip) << std::endl;
  }
  std::cout <<"ZjetsFit " << std::endl;  
  for(int ip=0; ip< _f_Zjets->GetNpar(); ip++){
    std::cout << "\tp"<<ip << " " << _f_Zjets->GetParameter(ip) << " +/- " << _f_Zjets->GetParError(ip) << std::endl;
  }


  
  string name = "IsoPileup2_DataMC_prompt_" + lep + "_" + iso + "_" + dep;
  string fName= _pathPlots + "/" + name;
  std::cout << "Saving plot: " << name <<std::endl;
  _c1->SaveAs((fName+".png").c_str());
  if(delCanvas) delete _c1;

  
  //
  //Data BB HF tag-probe
  //
  _hName = "sel_hf_" + lep + "_" + iso + "_" + dep;
  _h3_data  = (TH3F*) _ana->getHisto("data12",_hName);
  _h3_BB= (TH3F*) _ana->getHisto("BB",_hName);

  _h_data   = _ana->getProfile3D(_h3_data,"x","y","z");
  _h_data->SetTitle("data_hf");  _h_data->SetName("data_hf");  
  _h_BB   = _ana->getProfile3D(_h3_BB,"x","y","z");
  _h_BB->SetTitle("BB_hf");  _h_BB->SetName("BB_hf");
  max_mc_hf = _h_data->GetMaximum();


  TLegend*  _leg3 = new TLegend(0.2,0.2,0.35,0.35); _utils->legendSetting(_leg3); 
  TCanvas* _c3  = _utils->myCanvas(string("Data HF tag-probe: Iso -vs- "+dep).c_str());
  _utils->myDraw1d(_h_data,_c3,1,"e",false,kBlack,false,20);
  _utils->myDraw1d(_h_BB,_c3,1,"esame",false,kRed+2,false,27);
  _h_data->GetYaxis()->SetTitle(iso.c_str());
  _h_data->SetMaximum(max_mc_hf*scale);
  _leg3->AddEntry(_h_data,"Data","p");
  _leg3->AddEntry(_h_BB,"b#bar b","p");
  _leg3->Draw();
  _utils->myText(0.15,0.9,kBlack,"Data MC comparison. HF TP",0.03);
  gPad->Modified();

  name = "IsoPileup2_DataMC_HF_" + lep + "_" + iso + "_" + dep;
  fName= _pathPlots + "/" + name;
  std::cout << "Saving plot: " << name <<std::endl;
  _c3->SaveAs((fName+".png").c_str());
  if(delCanvas) delete _c3;


}


//-----------------------------------------------------------
//
// Plot Eff iso cut for signal against bkg
// lep:e, m; iso:ptCone20 etc....  
// MC truth match real from Z - bkg bbar 
// Data Z tag-probe -vs- HF tag-probe in data
//
// perf_IsoCut("m","ptCone")
//
void perf_IsoCut(string lep, string iso, bool relative)
{
  TString LEP(lep);
  TString ISO(iso);
  string cone[3]={"20", "30", "40"};

  //Legend posisiton
  float lx1=0.25;
  float ly1=0.25;
  float lxo=0.25;
  float lyo=0.25;
  
  if(ISO.CompareTo("etCone")==0 && !relative){
    lx1=0.55;
    ly1=0.55;
  }
  
  _pathPlots =  string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/histos_" +
    DATE +"/Plots/PerfIso";
  gSystem->mkdir(_pathPlots.c_str(),kTRUE);

  //
  //Using MC truth match
  //
  //Make the eff plot (can't use the ones after hadd !!!)
  bool useBB=false;
  TGraphErrors* g0[3];
  TCanvas* _c0  = _utils->myCanvas("Perf Iso - MC");
  TMultiGraph *mg0 = new TMultiGraph();
  TLegend*  _leg0 = new TLegend(lx1,ly1,lx1+lxo,ly1+lyo); _utils->legendSetting(_leg0); 
  for(uint i=0; i<3; i++){
    string _hNameR, _hNameF, _sTitle;
    if(relative){
      _hNameR = "trm_pr_" + lep + "_" + iso + cone[i] + "Rel";
      _hNameF = "trm_hf_" + lep + "_" + iso + cone[i] + "Rel";
      _sTitle = iso + cone[i] + "Rel";
    }
    else{
      _hNameR = "trm_pr_" + lep + "_" + iso + cone[i];
      _hNameF = "trm_hf_" + lep + "_" + iso + cone[i];
      _sTitle = iso + cone[i];
    }

    TH1F* _h_PR     = (TH1F*) _ana->getHisto("Zjets",_hNameR);
    TH1F* _h_HF;
    if(useBB) _h_HF = (TH1F*) _ana->getHisto("BB",_hNameF);
    else      _h_HF = (TH1F*) _ana->getHisto("top",_hNameF);
    TH1F* _eff_PR   = _ana->calcEff(_h_PR);
    TH1F* _rej_HF   = _ana->calcEff(_h_HF,1); //rejection
    
    // Make perf iso cut plot
    g0[i] = _ana->TGraph2Hist(_eff_PR,_rej_HF);
    g0[i]->SetMarkerColor(i+2);
    _leg0->AddEntry(g0[i],_sTitle.c_str(),"p");
    mg0->Add(g0[i]);

    if(LEP.Contains("m") && ISO.CompareTo("ptCone")==0 && i==0){
      float isoCut =1.8;
      int ibin = _h_PR->FindBin(isoCut);
      std::cout << " Muon Iso operating point - Signal Eff " << _eff_PR->GetBinContent(ibin)
		<< " bkg Rejection " << _rej_HF->GetBinContent(ibin) <<std::endl;
    }
  }
  _c0->cd(1); 
  mg0->Draw("AP"); 
  mg0->GetXaxis()->SetTitle("Signal Efficiency");
  mg0->GetYaxis()->SetTitle("Bkg Rejection Efficiency");
  mg0->GetXaxis()->SetRangeUser(0.9,1);
  mg0->GetYaxis()->SetRangeUser(0.15,0.9);
  _leg0->Draw();
  gPad->SetGridx();
  gPad->SetGridy();
  _utils->myText(0.15,0.9,kBlack,"MC TruthMatch. Signal:Z PR; Bkg: BB HF",0.03);
  gPad->Modified();
  
  string name = "PerfIso_MC_" + lep + "_" + iso;
  if(relative) name = name + "Rel";
  string fName= _pathPlots + "/" + name;
  std::cout << "Saving plot: " << name <<std::endl;
  _c0->SaveAs((fName+".png").c_str());
  
  //
  //Using data tag-probe
  //
  //Make the eff plot (can't use the ones after hadd !!!)
  TGraphErrors* g1[3];
  TCanvas* _c1  = _utils->myCanvas("Perf Iso - Data");
  TMultiGraph *mg1 = new TMultiGraph();
  TLegend*  _leg1 = new TLegend(lx1,ly1,lx1+lxo,ly1+lyo); _utils->legendSetting(_leg1); 

  for(uint i=0; i<3; i++){
    string _hNameR, _hNameF, _sTitle;
    if(relative){
      _hNameR = "sel_pr_" + lep + "_" + iso + cone[i] + "Rel";
      _hNameF = "sel_hf_" + lep + "_" + iso + cone[i] + "Rel";
      _sTitle = iso + cone[i] + "Rel";
    }
    else{
      _hNameR = "sel_pr_" + lep + "_" + iso + cone[i];
      _hNameF = "sel_hf_" + lep + "_" + iso + cone[i];
      _sTitle = iso + cone[i];
    }
    TH1F* _h_BB   = (TH1F*) _ana->getHisto("data12_All",_hNameF);
    TH1F* _h_data = (TH1F*) _ana->getHisto("data12_All",_hNameR);

    TH1F* _eff_data = _ana->calcEff(_h_data);
    TH1F* _rej_BB   = _ana->calcEff(_h_BB,1); //rejection
    
    // Make perf iso cut plot
    g1[i] = _ana->TGraph2Hist(_eff_data,_rej_BB);
    g1[i]->SetMarkerColor(i+2);
    _leg1->AddEntry(g1[i],_sTitle.c_str(),"p");
    mg1->Add(g1[i]);
  }
  _c1->cd(1); 
  gPad->SetGridx();
  gPad->SetGridy();
  mg1->Draw("AP"); 
  mg1->GetXaxis()->SetTitle("Signal Efficiency");
  mg1->GetYaxis()->SetTitle("Bkg Rejection Efficiency");
  mg1->GetXaxis()->SetRangeUser(0.9,1);
  mg1->GetYaxis()->SetRangeUser(0.15,0.9);
  _leg1->Draw();
  _utils->myText(0.15,0.9,kBlack,"Data. Signal:Z TP; Bkg: HF TP",0.03);
  gPad->Modified();
  
  name = "PerfIso_Data_" + lep + "_" + iso;
  if(relative) name = name + "Rel";
  fName= _pathPlots + "/" + name;
  std::cout << "Saving plot: " << name <<std::endl;
  _c1->SaveAs((fName+".png").c_str());

}

//-----------------------------------------------------------
//
// Plot Eff iso cut for signal against bkg in pt bins and Npv (mu) bins
// lep:e, m; iso:ptCone20 etc....  
// dep npv, mu
// MC truth match real from Z - bkg bbar 
// Data Z tag-probe -vs- HF tag-probe
//
void perf_IsoCutRange(string lep, string iso, string dep, bool relative)
{
  //PTbins 10-15, 15-20, 20-40, 40-60, >60
  //Npv/mu 0-15, 15-20, >20

  TString LEP(lep);
  TString ISO(iso);
  TString DEP(dep);
  //  string cone[3]={"20", "30", "40"};
  string cone[3]={"30"};
  int hMarker[3]={20,24,27};
  string _hNameR, _hNameF;

  //Legend posisiton
  float lx1=0.15;
  float ly1=0.15;
  float lxo=0.35;
  float lyo=0.35;
 
  /* 
  if(ISO.CompareTo("etCone")==0 && relative){
    lx1=0.55;
    ly1=0.55;
  }
  */

  _pathPlots =  string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/histos_" +
    DATE +"/Plots/PerfIso";
  gSystem->mkdir(_pathPlots.c_str(),kTRUE);
  
  bool useBB=false; //To pick HF from BB or Top samples

  
  for(uint k=0; k<2; k++){//MC-Data
    string sSel;
    string sFile1;
    string sFile2;
    string sType;
    if(k==0){ //MC
      std::cout << ">>>> MC <<<<" << std::endl;
      sSel = "trm";
      sFile1 = "Zjets_Alpgen";
      if(useBB) sFile2 = "BB";
      else      sFile2 = "top";
      sType="MC";
    }
    if(k==1){ //Data
      std::cout << "\n>>>> DATA <<<<" << std::endl;
      sSel = "sel";
      sFile1 = "data12";
      sFile2 = "data12";
      sType="Data";
    }
    
    for(uint ipt=0; ipt<6; ipt++){
      int ipt1, ipt2;
      string sPtBin;
      string sFile;
      if(ipt==0){
	ipt1=0; ipt2=-1;
	sPtBin="Inclusive";
	sFile="_PtAll";
      }
      if(ipt==1){
	ipt1=0; ipt2=1;
	sPtBin="10 < p_{T} < 20 GeV";
	sFile="_Pt10";
      }
      if(ipt==2){
	ipt1=2; ipt2=2;
	sPtBin="20 < p_{T} < 30 GeV";
	sFile="_Pt20";
      }
      if(ipt==3){
	ipt1=3; ipt2=3;
	sPtBin="30 < p_{T} < 40 GeV";
	sFile="_Pt30";
      }
      if(ipt==4){
	ipt1=4; ipt2=5;
	sPtBin="40 < p_{T} < 60 GeV";
	sFile="_Pt40";
      }
      if(ipt==5){
	ipt1=6; ipt2=-1;
	sPtBin="p_{T} > 60 GeV";
	sFile="_Pt60";
      }
      
      TGraphErrors* g0[3][3];
      TMultiGraph *mg0 = new TMultiGraph();
      TLegend*  _leg0 = new TLegend(lx1,ly1,lx1+lxo,ly1+lyo); _utils->legendSetting(_leg0); 
      
      for(uint i=0; i<1; i++){
	string _iTitle = iso + cone[i];
	_hNameR = sSel + "_pr_" + lep + "_" + iso + cone[i] + "_" + dep;
	_hNameF = sSel + "_hf_" + lep + "_" + iso + cone[i] + "_" + dep;
	if(relative){
	  _hNameR = sSel + "_pr_" + lep + "_" + iso + cone[i] + "Rel_" + dep;
	  _hNameF = sSel + "_hf_" + lep + "_" + iso + cone[i] + "Rel_" + dep;
	  _iTitle = iso + cone[i] + "Rel";
	}

	if(i==0) std::cout << "Analysing " << _hNameR << " " << _hNameF << std::endl;
	
	TH3F* _h3_PR     = (TH3F*) _ana->getHisto(sFile1.c_str(),_hNameR);
	TH3F* _h3_HF     = (TH3F*) _ana->getHisto(sFile2.c_str(),_hNameF);
	
	for(uint j=0; j<3; j++){
	  int inpv1, inpv2;
	  if(DEP.CompareTo("mu")==0){
	    if(j==0){inpv1=0; inpv2=15;}
	    if(j==1){inpv1=16; inpv2=20;}
	    if(j==2){inpv1=21; inpv2=31;}
	  }
	  else{
	    if(j==0){inpv1=0; inpv2=7;}
	    if(j==1){inpv1=8; inpv2=15;}
	    if(j==2){inpv1=16; inpv2=30;}
	  }
	  string _jTitle = " (" + _ana->itos(inpv1) + "<" + dep + "<" + _ana->itos(inpv2) + ")";
	  string _sTitle = _iTitle + _jTitle;
	  
	  _h_PR = _ana->get1DHisto(_h3_PR,"x",ipt1,ipt2, inpv1,inpv2);
	  _h_PR->SetTitle("iso_pr_PR");     _h_PR->SetName("iso_pr_PR");
	  _h_HF    = _ana->get1DHisto(_h3_HF,"x",ipt1,ipt2, inpv1,inpv2);
	  _h_HF->SetTitle("iso_hf_HF");    _h_HF->SetName("iso_hf_HF");
	  
	  TH1F* _eff_PR = _ana->calcEff(_h_PR);
	  TH1F* _rej_HF = _ana->calcEff(_h_HF,1); //rejection
	  
	  // Make perf iso cut plot
	  g0[i][j] = _ana->TGraph2Hist(_eff_PR,_rej_HF);
	  g0[i][j]->SetMarkerColor(i+2);
	  g0[i][j]->SetMarkerStyle(hMarker[j]);
	  _leg0->AddEntry(g0[i][j],_sTitle.c_str(),"p");
	  mg0->Add(g0[i][j]);
	  
	  
	  //Operating isolation cut point 2011
	  if(LEP.Contains("m") && (ISO.CompareTo("ptCone")==0 || ISO.CompareTo("etCone")==0 )
	     && i==0){//i==0 cone30
	    float isoCut;
	    if(relative==true) isoCut =0.2;//0.16;
	    else               isoCut =1.8;
	    int ibin = _h_PR->FindBin(isoCut);
	    if(k==0 && j==0) std::cout <<"MC " << sPtBin << std::endl;
	    if(k==1 && j==0) std::cout <<"DATA " << sPtBin << std::endl;
	    std::cout << " Muon Iso cone " << cone[i] << " operating point " << _jTitle << " bin " << ibin 
		      << " isoCut " << isoCut
		      << std::setprecision(4) 
		      << "  - Signal Eff " << _eff_PR->GetBinContent(ibin)*100.
		      << " bkg Rejection " << _rej_HF->GetBinContent(ibin)*100.
		      <<std::endl;
	  }
	  if(LEP.Contains("e") && (ISO.CompareTo("ptCone")==0 || ISO.CompareTo("etConeTopoCorr")==0)
	     && i==0){//i==0 cone30
	    float isoCut;
	    if(relative==true) isoCut =0.2;
	    else               isoCut =2;
	    int ibin = _h_PR->FindBin(isoCut);
	    if(k==0 && j==0) std::cout <<"MC " << sPtBin << std::endl;
	    if(k==1 && j==0) std::cout <<"DATA " << sPtBin << std::endl;
	    std::cout << " Electron Iso cone " << cone[i] << " operating point " << _jTitle << " bin " << ibin
		      << " isoCut " << isoCut
		      << std::setprecision(4) 
		      << "  - Signal Eff " << _eff_PR->GetBinContent(ibin)*100.
		      << " bkg Rejection " << _rej_HF->GetBinContent(ibin)*100.
		      <<std::endl;
	  }
	  
	}//npv loop    
      }//iso cone loop
      
      string sCanvas = "Perf Iso - MC " + sPtBin;
      if(k==0) sCanvas = "MC " + sCanvas;
      if(k==1) sCanvas = "DATA " + sCanvas;
      TCanvas* _c0  = _utils->myCanvas(sCanvas.c_str());
      _utils->myText(0.7,0.9,kBlack,sPtBin.c_str(),0.03);
      
      _c0->cd(1); 
      mg0->Draw("AP"); 
      mg0->GetXaxis()->SetTitle("Signal Efficiency");
      mg0->GetYaxis()->SetTitle("Bkg Rejection Efficiency");
      mg0->GetXaxis()->SetRangeUser(0.9,1);
      mg0->GetYaxis()->SetRangeUser(0.15,0.9);
      if(k==0){
	if(useBB) _utils->myText(0.15,0.9,kBlack,"MC TruthMatch. Signal:Z PR; Bkg(BB): HF",0.03);
	else      _utils->myText(0.15,0.9,kBlack,"MC TruthMatch. Signal:Z PR; Bkg(top): HF",0.03);
      }
      if(k==1) _utils->myText(0.15,0.9,kBlack,"Data. Signal:Z TP; Bkg: HF TP",0.03);
      _leg0->Draw();
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->Modified();
      
      string name = "PerfIso_" + sType + "_" + lep + "_" + iso;
      if(relative) name += "Rel";
      name +=  "_" + dep + sFile;
      string fName= _pathPlots + "/" + name;
      std::cout << "Saving plot: " << name <<std::endl;
      _c0->SaveAs((fName+".png").c_str());
      if(delCanvas){
	if(_leg0) delete _leg0;
	if(mg0) delete mg0;
      }
    }//pt bins 
        
  }//MC - data


  //Clean canvases
  //if(delCanvas) gROOT->GetListOfCanvases()->Delete();
  

}


//-----------------------------------------------------------
// Compare Iso shape PR - HF
//

void compIso(string lep, string iso, bool relative){

  TString LEP(lep);
  TString ISO(iso);
  string cone[3]={"20", "30", "40"};

  _pathPlots =  string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/histos_" +
    DATE +"/Plots/PerfIso";
  gSystem->mkdir(_pathPlots.c_str(),kTRUE);
  
  string type;
  TH1F* _h_pr_iso[2];
  TH1F* _h_hf_bb_iso[2];
  TH1F* _h_hf_top_iso[2];

  for(uint i=0; i<3; i++){//Cone size 
    string sTitle;
    
    for(uint k=0; k<2; k++){//MC-Data
      if(k==0) type="trm_";
      if(k==1) type="sel_";

      string pr_iso, hf_iso;
      sTitle = lep + "_" + iso + cone[i];
      if(relative){
	pr_iso = type + "pr_" + sTitle + "Rel";
	hf_iso = type + "hf_" + sTitle + "Rel";
      }
      else{
	pr_iso = type + "pr_" + sTitle;
	hf_iso = type + "hf_" + sTitle;
      }
      
      
      if(k==0){//MC
	_h_pr_iso[0]     = (TH1F*) _ana->getHisto("Zjets",pr_iso);
	_h_hf_bb_iso[0]  = (TH1F*) _ana->getHisto("BB",hf_iso);
	_h_hf_top_iso[0] = (TH1F*) _ana->getHisto("top",hf_iso);
      }
      else if(k==1){//Data
	string dFile;
	dFile = "data12_All";
	_h_pr_iso[1]    = (TH1F*) _ana->getHisto(dFile,pr_iso);
	_h_hf_bb_iso[1] = (TH1F*) _ana->getHisto(dFile,hf_iso);
      }
      
      _utils->normalize(_h_pr_iso[k],1);
      _utils->normalize(_h_hf_bb_iso[k],1); 
      if(k==0) _utils->normalize(_h_hf_top_iso[k],1);
      
      _h_pr_iso[k]->GetYaxis()->SetTitle("A.U");
      
      string cName = sTitle;
      if(k==0) cName = "MC " + sTitle;
      if(k==1) cName = "Data " + sTitle;
      
      //Compare PR & HF
      TCanvas* _c1  = _utils->myCanvas(cName.c_str());
      _utils->myDraw1d(_h_pr_iso[k],_c1,1,"e",false,kBlue,false,20);
      _utils->myDraw1d(_h_hf_bb_iso[k],_c1,1,"esame",false,kRed,false,20);
      if(k==0) _utils->myDraw1d(_h_hf_top_iso[k],_c1,1,"esame",false,kMagenta,false,20);
      if(i==0) _h_pr_iso[k]->GetXaxis()->SetRangeUser(-5,15);
      
      _utils->myText(0.15,0.9,kBlack,cName.c_str(),0.03);
      
      TLegend*  _leg0 = new TLegend(0.6,0.7,0.9,0.85); 
      _utils->legendSetting(_leg0); 
      _leg0->AddEntry(_h_pr_iso[k],"Prompt", "p");
      _leg0->AddEntry(_h_hf_bb_iso[k],"HF BB", "p");
      if(k==0) _leg0->AddEntry(_h_hf_top_iso[k],"HF Top", "p");
      _leg0->Draw();


      string name = "CompIso_";
      if(k==0) name += "MC_";
      if(k==1) name += "Data_";
      name += sTitle;
      if(relative) name += "Rel";
      string fName= _pathPlots + "/" + name;
      std::cout << "Saving plot: " << name <<std::endl;
      _c1->SaveAs((fName+".png").c_str());
      if(delCanvas) delete _c1;

    }


    //COMPARE PR Data - MC
    string cName2 = "PR Data -vs- MC " + sTitle;
    TCanvas* _c2  = _utils->myCanvas(cName2.c_str());
    _utils->myDraw1d(_h_pr_iso[0],_c2,1,"e",false,kBlue,false,20);
    _utils->myDraw1d(_h_pr_iso[1],_c2,1,"esame",false,kRed,false,20);
    if(i==0) _h_pr_iso[0]->GetXaxis()->SetRangeUser(-5,15);
    TLegend*  _leg2 = new TLegend(0.55,0.7,0.9,0.85); 
    _utils->legendSetting(_leg2); 
    _leg2->AddEntry(_h_pr_iso[0],"Prompt MC", "p");
    _leg2->AddEntry(_h_pr_iso[1],"Prompt Data", "p");
    _leg2->Draw();

    string name = "CompIso_Data_vs_MC_PR_" + sTitle;
    if(relative) name += "Rel";
    string fName= _pathPlots + "/" + name;
    std::cout << "Saving plot: " << name <<std::endl;
    _c2->SaveAs((fName+".png").c_str());
    if(delCanvas) delete _c2;
    


    //COMPARE HF Data - MC
    string cName3 = "HF Data -vs- MC " + sTitle;
    TCanvas* _c3  = _utils->myCanvas(cName3.c_str());
    _utils->myDraw1d(_h_hf_bb_iso[0],_c3,1,"e",false,kBlue,false,20);
    _utils->myDraw1d(_h_hf_top_iso[0],_c3,1,"e",false,kMagenta,false,20);
    _utils->myDraw1d(_h_hf_bb_iso[1],_c3,1,"esame",false,kRed,false,20);
    if(i==0) _h_hf_bb_iso[0]->GetXaxis()->SetRangeUser(-5,15);
    TLegend*  _leg3 = new TLegend(0.55,0.7,0.9,0.85); 
    _utils->legendSetting(_leg3); 
    _leg3->AddEntry(_h_hf_bb_iso[0],"HF MC BB", "p");
    _leg3->AddEntry(_h_hf_top_iso[0],"HF MC top", "p");
    _leg3->AddEntry(_h_hf_bb_iso[1],"HF Data", "p");
    _leg3->Draw();

    name = "CompIso_Data_vs_MC_HF_" + sTitle;
    if(relative) name += "Rel";
    fName= _pathPlots + "/" + name;
    std::cout << "Saving plot: " << name <<std::endl;
    _c3->SaveAs((fName+".png").c_str());
    if(delCanvas) delete _c3;

  }
  
  
    
}
