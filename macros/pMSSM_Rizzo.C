#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"
#include "SusyWeakProdAna/Histos_Common.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <ostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "TFile.h"
#include "TH1.h"

using namespace std;
typedef unsigned uint;

bool verbose = false;

//
// Variables
//
TGuiUtils* _utils;
DrawPlots* _ana;

std::vector<string> LEP;
std::vector<int>    lepColor;

//_____________________________________________________________________________//
void pMSSM_Rizzo(string sReg){

  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 
  
  LEP.clear();
  LEP.push_back("EE");
  LEP.push_back("MM");
  LEP.push_back("EM");
  LEP.push_back("ALL");

  lepColor.clear();
  lepColor.push_back(kOrange-2);
  lepColor.push_back(kAzure+4);
  lepColor.push_back(kRed+1);
  lepColor.push_back(kBlack);

  //
  // Read txt file
  //
  string fileName= "SignalYield_" + sReg + ".txt";
  std::ifstream infile;
  infile.open(fileName.c_str());
  if (!infile.is_open()){
    printf("Problem opening output file .... bailing out \n %s \n",fileName.c_str());
    abort();
  }
  
  vector<int> _DSId;
  vector<float> _nom[4]; //EE,MM, EM,TOT
  vector<float> _err[4];

  // Read through file
  std::string currentLine = ""; 
  int   dsId;
  float nomEE, statEE, sysUpEE, sysDnEE;
  float nomMM, statMM, sysUpMM, sysDnMM;
  float nomEM, statEM, sysUpEM, sysDnEM;
  float nomTOT, statTOT, sysUpTOT, sysDnTOT;

  while(infile >> dsId  
	>> nomEE  >> statEE  >> sysUpEE  >> sysDnEE
	>> nomMM  >> statMM  >> sysUpMM  >> sysDnMM
	>> nomEM  >> statEM  >> sysUpEM  >> sysDnEM
	>> nomTOT >> statTOT >> sysUpTOT >> sysDnTOT){
    if(verbose){
      cout << dsId << "\t" 
	   << nomEE << " " << statEE << " " << sysUpEE << " " << sysDnEE << "\t"
	   << nomMM << " " << statMM << " " << sysUpMM << " " << sysDnMM << "\t"
	   << nomEM << " " << statEM << " " << sysUpEM << " " << sysDnEM << "\t"
	   << nomTOT << " " << statTOT << " " << sysUpTOT << " " << sysDnTOT
	   << endl;
    }
    float sysEE = max(sysUpEE,sysDnEE);
    float sysMM = max(sysUpMM,sysDnMM);
    float sysEM = max(sysUpEM,sysDnEM);
    float sysTOT = max(sysUpTOT,sysDnTOT);

    _DSId.push_back(dsId);
    _nom[0].push_back(nomEE);
    _err[0].push_back(sqrt( pow(statEE,2) + pow(sysEE,2) ));
    _nom[1].push_back(nomMM);
    _err[1].push_back(sqrt( pow(statMM,2) + pow(sysMM,2) ));
    _nom[2].push_back(nomEM);
    _err[2].push_back(sqrt( pow(statEM,2) + pow(sysEM,2) ));    
    _nom[3].push_back(nomTOT);
    _err[3].push_back(sqrt( pow(statTOT,2) + pow(sysTOT,2) ));
  }
  

  //
  // Book histo
  //
  const int nDS = 37;
  TH1F* _h[4];
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    string name = "Yield_" + LEP[ilep];
    _h[ilep] = (TH1F*) _utils->myTH1F(name.c_str(), name.c_str(), nDS,0,nDS-1, "","Events");
    
    for(int i=1; i<_h[ilep]->GetXaxis()->GetNbins(); i++){
      string sDSId;
      ostringstream convert;
      convert << _DSId[i-1]; 
      sDSId=convert.str();
      _h[ilep]->GetXaxis()->SetBinLabel(i,sDSId.c_str());
      _h[ilep]->GetXaxis()->LabelsOption("v");
    }
  }

  //
  // Fill Histo
  //
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    for(uint ids=1; ids<_DSId.size(); ids++){  
      _h[ilep]->SetBinContent(int(ids),_nom[ilep][ids-1]);
      _h[ilep]->SetBinError(int(ids),_err[ilep][ids-1]);
    }
  }
  

  //
  //
  //
  TLegend* _leg = new TLegend(0.4,0.7,0.6,0.8);
  _utils->legendSetting(_leg); 
  
  THStack* _sigStack;
  _sigStack = new THStack(string("Stack_pMSSM_Rizzo_"+sReg).c_str(),
			  string("Stack_pMSSM_Rizzo_"+sReg).c_str());
  _sigStack->Clear();
  
  for(uint ilep=0; ilep<LEP.size()-1; ilep++){
    _utils->addToTHStack(_sigStack,_h[ilep],lepColor[ilep], 
			 "HIST", _leg, LEP[ilep].c_str());
  }
  
  
  TCanvas* _c1  = _utils->myCanvas("Yield",1000,700);
  _h[3]->SetMinimum(0);
  _utils->myDraw1d(_h[3],_c1,1,"e",false,kBlack,false,20);
  _sigStack->Draw("histsame");
  _utils->myDraw1d(_h[3],_c1,1,"esame",false,kBlack,false,20);
  _leg->Draw();

  _utils->myText(0.1,0.9,kBlack,sReg.c_str(),0.05);

  string fName= "pMSSM_Rizzo_" + sReg + ".png";
  _c1->SaveAs(fName.c_str());
  
  
}
