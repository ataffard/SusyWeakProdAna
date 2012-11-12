/*

   format/channels  nom stat sysUp sysDn

   root -q -l '../macros/bkgYield.C+("SRjveto")' 


*/


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

bool verbose = true;

//
// Variables
//
TGuiUtils* _utils;
DrawPlots* _ana;

std::vector<TH1F*> _mcFake[4];  //[LEP][SYS]
std::vector<TH1F*> _mcZtt[4];   //[LEP][SYS]
std::vector<TH1F*> _mcWW[4];    //[LEP][SYS]
std::vector<TH1F*> _mcTopDil[4];//[LEP][SYS]
std::vector<TH1F*> _mcZX[4];    //[LEP][SYS]
std::vector<TH1F*> _mcAll[4];   //[LEP][SYS]

std::vector<string> BKG;
std::vector<string> LEP;

//
// Functions
//
//void makeBkgYieldTxt(string sReg);
void loadBkg(string DSId, string sReg,std::vector<TH1F*> *_H1);

//_____________________________________________________________________________//
void  bkgYield(string sReg){

  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 
  
  LEP.clear();
  LEP.push_back("EE");
  LEP.push_back("MM");
  LEP.push_back("EM");
  LEP.push_back("ALL");

  BKG.clear();
  BKG.push_back("FAKE");
  BKG.push_back("Ztt");
  BKG.push_back("WW");
  BKG.push_back("TOP");
  BKG.push_back("ZX");
  BKG.push_back("ALL");


  
  loadBkg("histo_topDil_Sherpa_rlep",sReg,_mcTopDil);
  loadBkg("histo_WW_Sherpa_rlep",sReg,_mcWW);
  loadBkg("histo_ZX_Sherpa_rlep",sReg,_mcZX);
  loadBkg("histo_ZTauTaujets_Sherpa_rlep",sReg,_mcZtt);  
  loadBkg("histo_data12_fake",sReg,_mcFake);
  loadBkg("histo_Bkg_rlep",sReg,_mcAll);


  string fileName= _ana->_pathTables + "/BkgYield_" + sReg + ".txt";
  std::ofstream outFile;
  outFile.open(fileName.c_str());
  if (!outFile.is_open()){
    printf("Problem opening output file .... bailing out \n %s \n",fileName.c_str());
    abort();
  }
  
  //FAKE
  if(verbose) cout << BKG[0] << endl;
  outFile << BKG[0] << "\t";
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    Double_t nom, stat, sysUp, sysDn;
    _ana->getYield(_mcFake[ilep],nom, stat, sysUp, sysDn, verbose);
      outFile << nom << " " << stat << " " << sysUp << " " << sysDn << " \t"; 
  }
  outFile << endl;
  
  //Ztt
  if(verbose) cout << BKG[1] << endl;
  outFile << BKG[1] << "\t";
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    Double_t nom, stat, sysUp, sysDn;
    _ana->getYield(_mcZtt[ilep],nom, stat, sysUp, sysDn, verbose);
      outFile << nom << " " << stat << " " << sysUp << " " << sysDn << " \t"; 
  }
  outFile << endl;
  
  //WW
  if(verbose) cout << BKG[2] << endl;
  outFile << BKG[2] << "\t";
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    Double_t nom, stat, sysUp, sysDn;
    _ana->getYield(_mcWW[ilep],nom, stat, sysUp, sysDn, verbose);
      outFile << nom << " " << stat << " " << sysUp << " " << sysDn << " \t"; 
  }
  outFile << endl;

  //TOP
  if(verbose) cout << BKG[3] << endl;
  outFile << BKG[3] << "\t";
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    Double_t nom, stat, sysUp, sysDn;
    _ana->getYield(_mcTopDil[ilep],nom, stat, sysUp, sysDn, verbose);
      outFile << nom << " " << stat << " " << sysUp << " " << sysDn << " \t"; 
  }
  outFile << endl;

  //ZX
  if(verbose) cout << BKG[4] << endl;
  outFile << BKG[4] << "\t";
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    Double_t nom, stat, sysUp, sysDn;
    _ana->getYield(_mcZX[ilep],nom, stat, sysUp, sysDn, verbose);
      outFile << nom << " " << stat << " " << sysUp << " " << sysDn << " \t"; 
  }
  outFile << endl;

  //ALL
  if(verbose) cout << BKG[5] << endl;
  outFile << BKG[5] << "\t";
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    Double_t nom, stat, sysUp, sysDn;
    _ana->getYieldBkgAll(_mcFake[ilep],
			 _mcZtt[ilep],
			 _mcWW[ilep],
			 _mcTopDil[ilep],
			 _mcZX[ilep],
			 nom, stat, sysUp, sysDn, verbose);
    outFile << nom << " " << stat << " " << sysUp << " " << sysDn << " \t"; 
  }
  outFile << endl;


  outFile.close();

}

//_____________________________________________________________________________//
void loadBkg(string DSId, string sReg,std::vector<TH1F*> *_H1){

  TFile*_bkgFile = _ana->openFile(DSId);

  for(uint ilep=0; ilep<LEP.size()-1; ilep++){
    _ana->clearHistVect(_H1[ilep]);
    string hName = "DG2L_" + sReg + "_" + LEP[ilep] + "_DG2L_pred";
    _H1[ilep] = _ana->loadHisto(_bkgFile,DSId,hName,verbose);
  }

  //Add EE+MM+EM
  _H1[3].reserve(DGSys_N);
  _H1[3] = _ana->sumChannels(_H1[0],_H1[1],_H1[2]);

}


