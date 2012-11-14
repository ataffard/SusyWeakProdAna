/*
root -q -l '../macros/pMSSM_Rizzo.C+("SRjveto")'
*/

#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"
#include "SusyWeakProdAna/Histos_Common.h"
#include "SignificanceCalculator/SignificanceCalculator.h"
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
bool useBkgDD =true;

//
// Variables
//
TGuiUtils* _utils;
DrawPlots* _ana;

std::vector<string> LEP;
std::vector<int>    lepColor;


void getBkg(string sReg, vector<float>* nom, vector<float>* err);

void plots(string sVar);

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
  string dir = _ana->_pathHisto + "/pMSSM_Rizzo/";
  string fileName= dir + "SignalYield_" + sReg + ".txt";
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
    string name = "Yield_" + LEP[ilep] + "_" + sReg;
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
  // Plot event yield
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

  string fName= _ana->_pathHisto + "/pMSSM_Rizzo/" + "pMSSM_Rizzo_" + sReg + ".png";
  _c1->SaveAs(fName.c_str());
  

  //
  //Get the bkg for this channel
  //
  vector<float> _nomBkg; //EE,MM, EM,TOT
  vector<float> _errBkg;
  getBkg(sReg, &_nomBkg, &_errBkg);


  //
  //Compute significance. Zllr
  //
  SignificanceCalculator* _sigCalc = new SignificanceCalculator();
  TH1F* _hZn[4];
  TCanvas* _c2[4];

  for(uint ilep=0; ilep<LEP.size(); ilep++){
    _hZn[ilep] = (TH1F*) _h[ilep]->Clone();
    string name = "Zn_" + LEP[ilep] + "_" + sReg;
    _hZn[ilep]->Reset();
    _hZn[ilep]->SetTitle(name.c_str());
    _hZn[ilep]->SetName(name.c_str());
    
    if(verbose)
      cout << LEP[ilep] << " Bkg " << _nomBkg[ilep] << " " << _errBkg[ilep]/_nomBkg[ilep] << endl;
    
    float maxZn=0;
    for(uint ids=1; ids<_DSId.size(); ids++){  
      Double_t _Zn = _sigCalc->Zn(_nom[ilep][ids-1],_nomBkg[ilep],_errBkg[ilep]/_nomBkg[ilep]); //Set S & B - do PE
      _hZn[ilep]->SetBinContent(int(ids),_Zn);
      if(verbose) cout << "\t Signal DS " <<  _DSId[ids-1] << " " << _nom[ilep][ids-1] << " Zn " << _Zn  << endl;
      if(_Zn>maxZn) maxZn=_Zn;
    }
    
    _c2[ilep]  = _utils->myCanvas(string("Zn_"+ LEP[ilep] + "_" + sReg).c_str(),1000,700);
    _hZn[ilep]->SetMinimum(0);
    _hZn[ilep]->SetMaximum(maxZn*1.2);
    _utils->myDraw1d(_hZn[ilep],_c2[ilep],1,"hist",false,kBlack,false,20);

    string pName= _ana->_pathHisto + "/pMSSM_Rizzo/" 
      + "pMSSM_Rizzo_Zn_" + LEP[ilep] + "_" + sReg + ".png";
    _c2[ilep]->SaveAs(pName.c_str());
    

    if(verbose) cout << " \n" << endl;
  }


  delete _sigCalc;


  
}

//_____________________________________________________________________________//
void getBkg(string sReg, vector<float>* nom, vector<float>* err)
{
  //
  // Read txt file
  //
  string fileName= _ana->_pathTables + "/BkgYield_" + sReg + ".txt";
  std::ifstream infile;
  infile.open(fileName.c_str());
  if (!infile.is_open()){
    printf("Problem opening output file .... bailing out \n %s \n",fileName.c_str());
    abort();
  }
  
  //vector<float> _nom[4]; //EE,MM, EM,TOT
  //vector<float> _err[4];
  nom->clear();
  err->clear();

  // Read through file
  std::string currentLine = ""; 
  string sDSId;
  float nomEE, statEE, sysUpEE, sysDnEE;
  float nomMM, statMM, sysUpMM, sysDnMM;
  float nomEM, statEM, sysUpEM, sysDnEM;
  float nomTOT, statTOT, sysUpTOT, sysDnTOT;

  while(infile >> sDSId  
	>> nomEE  >> statEE  >> sysUpEE  >> sysDnEE
	>> nomMM  >> statMM  >> sysUpMM  >> sysDnMM
	>> nomEM  >> statEM  >> sysUpEM  >> sysDnEM
	>> nomTOT >> statTOT >> sysUpTOT >> sysDnTOT){
    if(verbose){
      cout << sDSId << "\t" 
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

    if(sDSId == "ALL"){
      nom->push_back(nomEE);
      err->push_back(sqrt( pow(statEE,2) + pow(sysEE,2) ));
      nom->push_back(nomMM);
      err->push_back(sqrt( pow(statMM,2) + pow(sysMM,2) ));
      nom->push_back(nomEM);
      err->push_back(sqrt( pow(statEM,2) + pow(sysEM,2) ));    
      nom->push_back(nomTOT);
      err->push_back(sqrt( pow(statTOT,2) + pow(sysTOT,2) ));
      
      if(verbose) cout << "Bkg Yield for lep in " << sReg << "\t"
		       << " EE  " << nom->at(0) << " +/- " << err->at(0) << "\t"
		       << " MM  " << nom->at(1) << " +/- " << err->at(1) << "\t" 
		       << " EM  " << nom->at(2) << " +/- " << err->at(2) << "\t"
		       << " ALL " << nom->at(3) << " +/- " << err->at(3) << "\t"
 		       << endl;
    }
  }

  infile.close();

  // Overwrite Bkg estimate w/ DD ones 
  // Use Tables from note 11/13/12
  if(useBkgDD){
    nom->clear();
    err->clear();
    if(sReg=="SRjVeto"){
      nom->push_back(50.58);
      err->push_back(7.08);
      nom->push_back(59.16);
      err->push_back(9.10);
      nom->push_back(70.83);
      err->push_back(6.25);
      nom->push_back(180.56);
      err->push_back(21.07);
    }
    else if(sReg=="SRSSjVeto"){
      nom->push_back(3.58);
      err->push_back(1.46);
      nom->push_back(2.44);
      err->push_back(0.63);
      nom->push_back(6.55);
      err->push_back(1.98);
      nom->push_back(12.58);
      err->push_back(3.31);
    }
    else if(sReg=="SR2jets"){
      nom->push_back(80.17);
      err->push_back(18.73);
      nom->push_back(107.12);
      err->push_back(19.89);
      nom->push_back(0);
      err->push_back(0);
      nom->push_back(187.29);
      err->push_back(36.06);
    }
    else if(sReg=="SRmT2"){
      nom->push_back(17.39);
      err->push_back(4.59);
      nom->push_back(25.80);
      err->push_back(9.49);
      nom->push_back(19.28);
      err->push_back(2.92);
      nom->push_back(62.21);
      err->push_back(15.08);
    }
    else if(sReg=="SRmT2b"){
      nom->push_back(6.94);
      err->push_back(2.65);
      nom->push_back(7.53);
      err->push_back(2.88);
      nom->push_back(4.98);
      err->push_back(0.69);
      nom->push_back(19.40);
      err->push_back(5.36);
    }
    
    

  }

}

//_____________________________________________________________________________//
void plots(string sReg, string sVar){

  /*
    Models with most sensitivity for 2L are
    165868
    165872
    165880
    165883
    165884
    165889

    //lower yields
    165885
    165863
    165866
  */

  vector<string> _DSId;
  _DSId.push_back("165868");
  _DSId.push_back("165872");
  _DSId.push_back("165880");
  _DSId.push_back("165883");
  _DSId.push_back("165884");
  _DSId.push_back("165889");


  vector<int> _dsCol;
  _dsCol.push_back(kRed-3);
  _dsCol.push_back(kMagenta-3);
  _dsCol.push_back(kBlue-6);
  _dsCol.push_back(kAzure+6);
  _dsCol.push_back(kGreen-3);
  _dsCol.push_back(kViolet+1);

  //
  //Load standard stuff
  //
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 
  
  LEP.clear();
  LEP.push_back("EE");
  LEP.push_back("MM");
  LEP.push_back("EM");
  LEP.push_back("ALL");

  //
  // Grab the relevant histos
  //
  vector<TH1F*> _hSig;  //the var histo to plots - sum All channels
  
  for(uint i=0; i<_DSId.size(); i++){
    std::vector<TH1F*> _sigH1[4];//[LEP][SYS]
    TFile* _sigFile = _ana->openFile(_DSId[i]);
    
    for(uint ilep=0; ilep<LEP.size()-1; ilep++){
      _ana->clearHistVect(_sigH1[ilep]);
      string hName = "DG2L_" + sReg + "_" + LEP[ilep] + "_DG2L_" + sVar;
      _sigH1[ilep] = _ana->loadHisto(_sigFile,_DSId[i],hName);
    }
    
    //Add EE+MM+EM
    _sigH1[3].reserve(DGSys_N);
    _sigH1[3] = _ana->sumChannels(_sigH1[0],_sigH1[1],_sigH1[2]);
    
    _hSig.push_back(_sigH1[3][DGSys_NOM]);
  }


  //
  //Plot
  //
  TLegend* _leg = new TLegend(0.6,0.6,0.7,0.8);
  _utils->legendSetting(_leg); 

  string sName = "pMSSM_Rizzo_Kin_" + sReg + "_" + sVar;
  TCanvas* _c1  = _utils->myCanvas(sName.c_str());
  for(uint i=0; i<_DSId.size(); i++){
    string opt = "histsame";
    if(i==0) opt="hist";
    _utils->myDraw1d(_hSig[i],_c1,1,opt.c_str(),false,_dsCol[i],false,20);
    _leg->AddEntry(_hSig[i],_DSId[i].c_str(),"l");
  }
  _leg->Draw();
  _utils->myText(0.1,0.9,kBlack,sReg.c_str(),0.05);

  string fName= _ana->_pathHisto + "/pMSSM_Rizzo/" + sName + ".png";
  _c1->SaveAs(fName.c_str());
 






}
