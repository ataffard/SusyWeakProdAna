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

bool verbose = false;// true;

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
std::vector<TH1F*> _data[4];    //[LEP][SYS]

std::vector<string> BKG;
std::vector<string> LEP;

static const bool blindData=false;//true;

//
// Functions
//
//void makeBkgYieldTxt(string sReg);
void loadBkg(string DSId, string sReg,std::vector<TH1F*> *_H1);
bool isSR(string sReg);

//_____________________________________________________________________________//
void  bkgYield(string sReg){

  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 
  
  int nPrec = 2;
  if(sReg=="CRZ" ||
     sReg=="CR2LepOS" ||
     sReg=="CR2LepSS" ||
     sReg=="preSRjveto" ||
     sReg=="preSROSjveto"
     ) nPrec=0;


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

  //Column
  std::vector<string> sLEPCOL;
  sLEPCOL.clear();
  sLEPCOL.push_back("$ee$");   
  sLEPCOL.push_back("$\\mu\\mu$");   
  sLEPCOL.push_back("$e\\mu$");   
  sLEPCOL.push_back("Total");   

  std::vector<string> sLEP;
  sLEP.clear();
  sLEP.push_back("EE");   
  sLEP.push_back("MM");   
  sLEP.push_back("EM");   
  sLEP.push_back("ALL");   

  //Raws
  std::vector<string> sBKGCOL;
  sBKGCOL.clear();
  sBKGCOL.push_back("Fake leptons");
  sBKGCOL.push_back("$Z \\to \\tau\\tau$");
  sBKGCOL.push_back("WW");
  sBKGCOL.push_back("Top");
  sBKGCOL.push_back("$Z(ee,\\mu\\mu)$+jets + ZV)");
  sBKGCOL.push_back("Total");

  std::vector<string> sBKG;
  sBKG.clear();
  sBKG.push_back("FAKE");
  sBKG.push_back("Ztt");
  sBKG.push_back("WW");
  sBKG.push_back("TOP");
  sBKG.push_back("ZX");
  sBKG.push_back("TOTAL");

  std::vector<string> sDATA;
  sDATA.clear();
  sDATA.push_back("Data");



  loadBkg("histo_data12_std",sReg,_data);
  loadBkg("histo_data12_flep",sReg,_mcFake);

  //  loadBkg("histo_topDil_Sherpa_rlep",sReg,_mcTopDil);
  //  loadBkg("histo_top_PowHeg_rlep",sReg,_mcTopDil);
  //  loadBkg("histo_top_Alpgen_rlep",sReg,_mcTopDil);
  loadBkg("top_MCNLO_rlep",sReg,_mcTopDil);
  loadBkg("WW_Sherpa_rlep",sReg,_mcWW);

  /*
  loadBkg("histo_ZX_Sherpa_rlep",sReg,_mcZX);
  loadBkg("histo_ZTauTaujets_Sherpa_rlep",sReg,_mcZtt);  
  loadBkg("histo_BkgSherpa_rlep",sReg,_mcAll);
  */

  loadBkg("ZX_AlpgenPythia_rlep",sReg,_mcZX);
  loadBkg("ZTauTaujets_AlpgenPythia_rlep",sReg,_mcZtt);  
  //  loadBkg("histo_BkgAlpgen_rlep",sReg,_mcAll);

  //  loadBkg("histo_BkgZTopAlpgen_rlep",sReg,_mcAll);
  loadBkg("Bkg_ZXAlpgen_TopMCNLO_rlep",sReg,_mcAll);

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
    

  //FAKE
  if(verbose) cout << BKG[0] << endl;
  outFile << BKG[0] << "\t";
  outTEX << sBKGCOL[0] << " & ";
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    Double_t nom, stat, sysUp, sysDn;
    _ana->getYield(_mcFake[ilep],nom, stat, sysUp, sysDn, verbose);
    outFile << nom << " " << stat << " " << sysUp << " " << sysDn << " \t"; 
    outTEX << std::setprecision(nPrec) << std::fixed << nom << " $\\pm$ " << stat 
	   << " $^{+"<< sysUp << "}_{-" << sysDn << "}$ ";
    if(ilep<sLEP.size()-1) outTEX << " & ";
  }
  outTEX << "\\\\" <<std::endl;
  outFile << endl;

  
  //Ztt
  if(verbose) cout << BKG[1] << endl;
  outFile << BKG[1] << "\t";
  outTEX << sBKGCOL[1] << " & ";
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    Double_t nom, stat, sysUp, sysDn;
    _ana->getYield(_mcZtt[ilep],nom, stat, sysUp, sysDn, verbose);
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

  //ZX
  if(verbose) cout << BKG[4] << endl;
  outTEX << sBKGCOL[4] << " & ";
  outFile << BKG[4] << "\t";
  for(uint ilep=0; ilep<LEP.size(); ilep++){
    Double_t nom, stat, sysUp, sysDn;
    _ana->getYield(_mcZX[ilep],nom, stat, sysUp, sysDn, verbose);
    outFile << nom << " " << stat << " " << sysUp << " " << sysDn << " \t"; 
    outTEX << std::setprecision(nPrec) << std::fixed << nom << " $\\pm$ " << stat 
	   << " $^{+"<< sysUp << "}_{-" << sysDn << "}$ ";
    if(ilep<sLEP.size()-1) outTEX << " & ";
  }
  outTEX << "\\\\ \\hline" <<std::endl;
  outFile << endl;

  //ALL
  if(verbose) cout << BKG[5] << endl;
  outTEX << sBKGCOL[5] << " & ";
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
    if(!isSR(sReg))
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

  for(uint ilep=0; ilep<LEP.size()-1; ilep++){
    _ana->clearHistVect(_H1[ilep]);
    string hName = "DG2L_" + sReg + "_" + LEP[ilep] + "_DG2L_pred";
    _H1[ilep] = _ana->loadHisto(_bkgFile,DSId,hName,verbose);
  }

  //Add EE+MM+EM
  _H1[3].reserve(DGSys_N);
  _H1[3] = _ana->sumChannels(_H1[0],_H1[1],_H1[2]);

}


//_____________________________________________________________________________//
bool isSR(string sReg){
  
  if(sReg=="SROSjveto" ||
     sReg=="SRmT2a" ||
     sReg=="SRmT2b" ||
     sReg=="SR2jets" ||
     sReg=="SRZjets" ||
     sReg=="SRSSjets" ||
     sReg=="SRWWa" ||
     sReg=="SRWWb" ||
     sReg=="SRWWc" 
   ) return true;

 return false;

}
