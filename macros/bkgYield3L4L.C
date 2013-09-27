/*

format/channels  nom stat sysUp sysDn		
Produce the tex tables as well

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

static const bool blindData=false;//true;
bool verbose = true;

//
// Variables
//
TGuiUtils* _utils;
DrawPlots* _ana;

std::vector<TH1F*> _mcWZ[2]; 
std::vector<TH1F*> _mcZZ[2]; 
std::vector<TH1F*> _mcWW[2]; 
std::vector<TH1F*> _mcTopDil[2];
std::vector<TH1F*> _mcZjets[2]; 
std::vector<TH1F*> _mcHiggs[2]; 
std::vector<TH1F*> _mcAll[2];   
std::vector<TH1F*> _data[2];    

std::vector<string> BKG;
std::vector<string> LEP;



//
// Functions
//
void loadBkg(string DSId, string sReg,std::vector<TH1F*> *_H1);
bool isSR(string sReg);

//_____________________________________________________________________________//
void  bkgYield3L4L(string sReg){

  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 
  
  int nPrec = 2;
 

  LEP.clear();
  LEP.push_back("LLL");
  LEP.push_back("LLLL");

  BKG.clear();
  BKG.push_back("WZ");
  BKG.push_back("ZZ");
  BKG.push_back("WW");
  BKG.push_back("TOP");
  BKG.push_back("Zjets");
  BKG.push_back("HIGGS");
  BKG.push_back("ALL");

  //Column
  std::vector<string> sLEPCOL;
  sLEPCOL.clear();
  sLEPCOL.push_back("$lll$");   
  sLEPCOL.push_back("$llll$");   
  

  std::vector<string> sLEP;
  sLEP.clear();
  sLEP.push_back("LLL");   
  sLEP.push_back("LLLL");   

  //Raws
  std::vector<string> sBKGCOL;
  sBKGCOL.clear();
  sBKGCOL.push_back("WZ");
  sBKGCOL.push_back("ZZ");
  sBKGCOL.push_back("WW");
  sBKGCOL.push_back("Top");
  sBKGCOL.push_back("Z+jets");
  sBKGCOL.push_back("SM Higgs");
  sBKGCOL.push_back("Total");

  std::vector<string> sBKG;
  sBKG.clear();
  sBKG.push_back("WZ");
  sBKG.push_back("ZZ");
  sBKG.push_back("WW");
  sBKG.push_back("TOP");
  sBKG.push_back("Zjets");
  sBKG.push_back("HIGGS");
  sBKG.push_back("TOTAL");

  std::vector<string> sDATA;
  sDATA.clear();
  sDATA.push_back("Data");



  loadBkg("histo_data12_std",sReg,_data);

  loadBkg("WZ_PowHeg_std",sReg,_mcWZ);  
  loadBkg("ZZ_PowHeg_std",sReg,_mcZZ);  
  loadBkg("top_MCNLO_std",sReg,_mcTopDil);
  loadBkg("WW_PowHeg_std",sReg,_mcWW);
  loadBkg("Zjets_SherpaAlpgenPythia_std",sReg,_mcZjets);
  loadBkg("Higgs_std",sReg,_mcHiggs);
  loadBkg("Bkg_Zjets_SherpaAlpgen_WZ_ZZ_PowHeg_WW_PowHeg_TopMCNLO_std",sReg,_mcAll);


  string fileName= _ana->_pathTables + "/BkgYield_" + sReg + ".txt";
  std::ofstream outFile;
  outFile.open(fileName.c_str());
  if (!outFile.is_open()){
    printf("Problem opening output file .... bailing out \n %s \n",fileName.c_str());
    abort();
  }
  

  fileName= _ana->_pathTables + "/BkgYield_" + sReg + ".tex";
  std::ofstream outTEX;
  outTEX.open(fileName.c_str());
  if (!outTEX.is_open()){
    printf("Problem opening output file .... bailing out \n %s \n",fileName.c_str());
    abort();
  }

  //Header Tex table
  outTEX << "\\begin{table}[htbp] "<< std::endl;
  outTEX << "\\begin{center} "<< std::endl;
  outTEX << "\\begin{tabular}{l|" ;
  for(uint ilep=0; ilep<sLEP.size()-1; ilep++) outTEX << "r|";
  outTEX << "r}"<< std::endl;
  outTEX << "\\hline \\hline"<< std::endl;
  //header
  outTEX << std::setw(17) << " & ";
  for(uint ilep=0; ilep<sLEP.size(); ilep++){
    outTEX << sLEPCOL[ilep];
    if(ilep<sLEP.size()-1) outTEX << " & ";
  }
  outTEX << "\\\\ \\hline" << std::endl;
    

  //WZ
  if(verbose) cout << BKG[0] << endl;
  outFile << BKG[0] << "\t";
  outTEX << sBKGCOL[0] << " & ";
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    Double_t nom, stat, sysUp, sysDn;
    _ana->getYield(_mcWZ[ilep],nom, stat, sysUp, sysDn, verbose);
    outFile << nom << " " << stat << " " << sysUp << " " << sysDn << " \t"; 
    outTEX << std::setprecision(nPrec) << std::fixed << nom << " $\\pm$ " << stat 
	   << " $^{+"<< sysUp << "}_{-" << sysDn << "}$ ";
    if(ilep<sLEP.size()-1) outTEX << " & ";
  }
  outTEX << "\\\\" <<std::endl;
  outFile << endl;
  
  //ZZ
  if(verbose) cout << BKG[1] << endl;
  outFile << BKG[1] << "\t";
  outTEX << sBKGCOL[1] << " & ";
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    Double_t nom, stat, sysUp, sysDn;
    _ana->getYield(_mcZZ[ilep],nom, stat, sysUp, sysDn, verbose);
    outFile << nom << " " << stat << " " << sysUp << " " << sysDn << " \t"; 
    outTEX << std::setprecision(nPrec) << std::fixed << nom << " $\\pm$ " << stat 
	   << " $^{+"<< sysUp << "}_{-" << sysDn << "}$ ";
    if(ilep<sLEP.size()-1) outTEX << " & ";
  }
  outTEX << "\\\\" <<std::endl;
  outFile << endl;
  
  //WW
  if(verbose) cout << BKG[2] << endl;
  outTEX << sBKGCOL[2] << " & ";
  outFile << BKG[2] << "\t";
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    Double_t nom, stat, sysUp, sysDn;
    _ana->getYield(_mcWW[ilep],nom, stat, sysUp, sysDn, verbose);
    outFile << nom << " " << stat << " " << sysUp << " " << sysDn << " \t"; 
    outTEX << std::setprecision(nPrec) << std::fixed << nom << " $\\pm$ " << stat 
	   << " $^{+"<< sysUp << "}_{-" << sysDn << "}$ ";
    if(ilep<sLEP.size()-1) outTEX << " & ";
  }
  outTEX << "\\\\" <<std::endl;
  outFile << endl;

  //TOP
  if(verbose) cout << BKG[3] << endl;
  outTEX << sBKGCOL[3] << " & ";
  outFile << BKG[3] << "\t";
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    Double_t nom, stat, sysUp, sysDn;
    _ana->getYield(_mcTopDil[ilep],nom, stat, sysUp, sysDn, verbose);
    outFile << nom << " " << stat << " " << sysUp << " " << sysDn << " \t"; 
    outTEX << std::setprecision(nPrec) << std::fixed << nom << " $\\pm$ " << stat 
	   << " $^{+"<< sysUp << "}_{-" << sysDn << "}$ ";
    if(ilep<sLEP.size()-1) outTEX << " & ";
  }
  outTEX << "\\\\" <<std::endl;
  outFile << endl;

  //Zjets
  if(verbose) cout << BKG[4] << endl;
  outTEX << sBKGCOL[4] << " & ";
  outFile << BKG[4] << "\t";
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    Double_t nom, stat, sysUp, sysDn;
    _ana->getYield(_mcZjets[ilep],nom, stat, sysUp, sysDn, verbose);
    outFile << nom << " " << stat << " " << sysUp << " " << sysDn << " \t"; 
    outTEX << std::setprecision(nPrec) << std::fixed << nom << " $\\pm$ " << stat 
	   << " $^{+"<< sysUp << "}_{-" << sysDn << "}$ ";
    if(ilep<sLEP.size()-1) outTEX << " & ";
  }
  outTEX << "\\\\ " <<std::endl;
  outFile << endl;


  //HIGGS
  if(verbose) cout << BKG[5] << endl;
  outTEX << sBKGCOL[5] << " & ";
  outFile << BKG[5] << "\t";
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    Double_t nom, stat, sysUp, sysDn;
    _ana->getYield(_mcHiggs[ilep],nom, stat, sysUp, sysDn, verbose);
    outFile << nom << " " << stat << " " << sysUp << " " << sysDn << " \t"; 
    outTEX << std::setprecision(nPrec) << std::fixed << nom << " $\\pm$ " << stat 
	   << " $^{+"<< sysUp << "}_{-" << sysDn << "}$ ";
    if(ilep<sLEP.size()-1) outTEX << " & ";
  }
  outTEX << "\\\\ \\hline" <<std::endl;
  outFile << endl;


  //ALL
  if(verbose) cout << BKG[6] << endl;
  outTEX << sBKGCOL[6] << " & ";
  outFile << BKG[6] << "\t";
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    Double_t nom, stat, sysUp, sysDn;
    _ana->getYieldBkgAll(_mcWZ[ilep],
			 _mcZZ[ilep],
			 _mcWW[ilep],
			 _mcTopDil[ilep],
			 _mcZjets[ilep],
			 _mcHiggs[ilep],
			 nom, stat, sysUp, sysDn, verbose);
    outFile << nom << " " << stat << " " << sysUp << " " << sysDn << " \t"; 
    outTEX << std::setprecision(nPrec) << std::fixed << nom << " $\\pm$ " << stat 
	   << " $^{+"<< sysUp << "}_{-" << sysDn << "}$ ";
    if(ilep<sLEP.size()-1) outTEX << " & ";
  }
  outTEX << "\\\\ \\hline" <<std::endl;
  outFile << endl;

  //
  //Add data
  //
  if(verbose) cout << "DATA" << endl;
  outTEX << "Data & ";
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    Double_t nom, stat, sysUp, sysDn;
    _ana->getYield(_data[ilep],nom, stat, sysUp, sysDn, verbose);
    if(!(isSR(sReg) && blindData))
      outTEX << std::setprecision(0) << std::fixed << nom;
    if(ilep<sLEP.size()-1) outTEX << " & ";
  }
  
  //Footer
  outTEX << "\\\\ \\hline \\hline" <<std::endl;
  outTEX << "\\end{tabular} " << std::endl;
  
  string caption = "\\caption{Background estimate for " + sReg + " }";
  outTEX << caption << std::endl;
  
  outTEX << "\\end{center}" << std::endl;
  outTEX << "\\end{table}" << std::endl;
  outTEX << std::endl;
  outTEX << std::endl;
  
  outTEX << "\\FloatBarrier" << std::endl;
  

  outFile.close();
  outTEX.close();

}

//_____________________________________________________________________________//
void loadBkg(string DSId, string sReg,std::vector<TH1F*> *_H1){

  TFile*_bkgFile = _ana->openFile(DSId);

  for(uint ilep=0; ilep<LEP.size(); ilep++){
    _ana->clearHistVect(_H1[ilep]);
    string hName = "ML_" + sReg + "_" + LEP[ilep] + "_ML_pred";
    //string hName = "ML_" + sReg + "_" + LEP[ilep] + "_ML_predGe2J";
    cout << "loading " << hName << endl;
    _H1[ilep] = _ana->loadHisto(_bkgFile,DSId,hName,verbose);
  }

}


//_____________________________________________________________________________//
bool isSR(string sReg){
  
 return false;

}
