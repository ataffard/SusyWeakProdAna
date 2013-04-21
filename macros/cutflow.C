/*

Make cutflow Latex table for the note
root -q -l '../macros/cutflow.C+("SRmT2a")' 
root -q -l '../macros/cutflow.C+("SRmT2b")' 

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

std::vector<TH1F*> _mcFake[4];  
std::vector<TH1F*> _mcZV[4];   
std::vector<TH1F*> _mcWW[4];    
std::vector<TH1F*> _mcTopDil[4];
std::vector<TH1F*> _mcZjets[4]; 
std::vector<TH1F*> _mcHiggs[4]; 
std::vector<TH1F*> _mcAll[4];   

std::vector<TH1F*> _slep_1[4]; 
std::vector<TH1F*> _slep_2[4];  
std::vector<TH1F*> _C1C1_1[4]; 
std::vector<TH1F*> _C1C1_2[4];  

std::vector<string> BKG;
std::vector<string> LEP;

void loadBkg(string DSId, string sReg,std::vector<TH1F*> *_H1);
float getYield(std::vector<TH1F*> histV, int cut);

//_____________________________________________________________________________//
void  cutflow(string sReg){

  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 

  LEP.clear();
  LEP.push_back("EE");
  LEP.push_back("MM");
  LEP.push_back("EM");
  LEP.push_back("ALL");

  //Tables
  std::vector<string> sLEPCOL;
  sLEPCOL.clear();
  sLEPCOL.push_back("$ee$");   
  sLEPCOL.push_back("$\\mu\\mu$");   
  sLEPCOL.push_back("$e\\mu$");   
  sLEPCOL.push_back("Total");   

  //Cutflow
  std::vector<int> nCUT;
  nCUT.clear();
  nCUT.push_back(PERCHANNEL);
  nCUT.push_back(QQ);
  nCUT.push_back(FLAVOR);
  nCUT.push_back(ZVETO);
  nCUT.push_back(FULLJVETO);
  nCUT.push_back(METREL);
  nCUT.push_back(MT2);

  //Columns
  std::vector<string> sBKGCOL;
  sBKGCOL.clear();
  //sBKGCOL.push_back("Fake leptons");
  sBKGCOL.push_back("ZV");
  sBKGCOL.push_back("WW");
  sBKGCOL.push_back("Top");
  sBKGCOL.push_back("Z+jets");
  sBKGCOL.push_back("SM Higgs");
  sBKGCOL.push_back("Total");
  sBKGCOL.push_back("\\SL\\SL");
  sBKGCOL.push_back("\\SL\\SL");
  sBKGCOL.push_back("\\CONEP \\CONEM");
  sBKGCOL.push_back("\\CONEP \\CONEM");
  
  std::vector<string> sBKGCOL2;
  sBKGCOL2.clear();
  sBKGCOL2.push_back("");
  sBKGCOL2.push_back("");
  sBKGCOL2.push_back("");
  sBKGCOL2.push_back("");
  sBKGCOL2.push_back("");
  sBKGCOL2.push_back("");
  sBKGCOL2.push_back("(191-90)");
  sBKGCOL2.push_back("(215-10)");
  sBKGCOL2.push_back("(350-0)");
  sBKGCOL2.push_back("(425-75)");


  loadBkg("histo_data12_flep",sReg,_mcFake);
  loadBkg("top_MCNLO_rlep",sReg,_mcTopDil);
  loadBkg("WW_PowHeg_rlep",sReg,_mcWW);
  loadBkg("Zjets_SherpaAlpgenPythia_rlep",sReg,_mcZjets);
  loadBkg("WZ_ZZ_PowHeg_rlep",sReg,_mcZV);  
  loadBkg("Higgs_rlep",sReg,_mcHiggs);
  loadBkg("Bkg_Zjets_SherpaAlpgen_WZ_ZZ_PowHeg_WW_PowHeg_TopMCNLO_rlep",sReg,_mcAll);
  
  loadBkg("Herwigpp_pMSSM_DLiSlep_MSL_191_M1_090.175447_rlep_NOM_XS_DN",sReg,_slep_1);
  loadBkg("Herwigpp_pMSSM_DLiSlep_MSL_251_M1_010.175510_rlep_NOM_XS_DN",sReg,_slep_2);
  loadBkg("Herwigpp_simplifiedModel_wC_slep_noWcascade_20.144917_rlep_NOM_XS_DN",sReg,_C1C1_1);
  loadBkg("Herwigpp_simplifiedModel_wC_slep_noWcascade_25.144922_rlep_NOM_XS_DN",sReg,_C1C1_2);

  /*
  loadBkg("Herwigpp_pMSSM_DLiSlep_MSL_191_M1_090.175447_rlep_unweighted_NOM_XS_DN",sReg,_slep_1);
  loadBkg("Herwigpp_pMSSM_DLiSlep_MSL_251_M1_010.175510_rlep_unweighted_NOM_XS_DN",sReg,_slep_2);
  loadBkg("Herwigpp_simplifiedModel_wC_slep_noWcascade_20.144917_rlep_unweighted_NOM_XS_DN",sReg,_C1C1_1);
  loadBkg("Herwigpp_simplifiedModel_wC_slep_noWcascade_25.144922_rlep_unweighted_NOM_XS_DN",sReg,_C1C1_2);
  */


  for(uint ilep=0; ilep<LEP.size(); ilep++){
    
    string fileName= _ana->_pathTables + "/Cutflow_" + LEP[ilep] + "_" + sReg + ".tex";
    std::ofstream outTEX;
    outTEX.open(fileName.c_str());
    if (!outTEX.is_open()){
      printf("Problem opening output file .... bailing out \n %s \n",fileName.c_str());
      abort();
    }
    
    //Header Tex table
    outTEX << "\\begin{table}[htbp] "<< std::endl;
    outTEX << "\\small "<< std::endl;
    outTEX << "\\begin{center} "<< std::endl;
    outTEX << "\\begin{tabular}{l|" ;
    for(uint iBkg=0; iBkg<sBKGCOL.size()-1; iBkg++) outTEX << "r|";
    outTEX << "r}"<< std::endl;
    outTEX << "\\hline \\hline"<< std::endl;
    //header
    outTEX << "Cuts & ";
    for(uint iBkg=0; iBkg<sBKGCOL.size(); iBkg++){
      outTEX << sBKGCOL[iBkg];
      if(iBkg<sBKGCOL.size()-1) outTEX << " & ";
    }
    outTEX << "\\\\ " << std::endl;
    outTEX << " & ";
    for(uint iBkg=0; iBkg<sBKGCOL2.size(); iBkg++){
      outTEX << sBKGCOL2[iBkg];
      if(iBkg<sBKGCOL2.size()-1) outTEX << " & ";
    }
    outTEX << "\\\\ \\hline" << std::endl;

    int nPrec = 0;    
    for(uint icut=0; icut<nCUT.size(); icut++){
      if(icut>4) nPrec = 2;
      //outTEX.setf( std::ios::fixed, std:: ios::floatfield );
      outTEX << std::fixed;
      outTEX << std::setprecision(nPrec);
      outTEX << DG2LCutNames[nCUT[icut]] 
	//<< " & " << getYield(_mcFake[ilep],nCUT[icut]) 
	     << " & " << getYield(_mcZV[ilep],nCUT[icut]) 
	     << " & " << getYield(_mcWW[ilep],nCUT[icut]) 
	     << " & " << getYield(_mcTopDil[ilep],nCUT[icut]) 
	     << " & " << getYield(_mcZjets[ilep],nCUT[icut]) 
	     << " & " << getYield(_mcHiggs[ilep],nCUT[icut])
	     << " & " << getYield(_mcAll[ilep],nCUT[icut])
	     << " & " << getYield(_slep_1[ilep],nCUT[icut])
	     << " & " << getYield(_slep_2[ilep],nCUT[icut])
	     << " & " << getYield(_C1C1_1[ilep],nCUT[icut])
	     << " & " << getYield(_C1C1_2[ilep],nCUT[icut])
	     << "\\\\" <<std::endl;
    }
    
    //Footer
    outTEX << "\\hline \\hline" <<std::endl;
    outTEX << "\\end{tabular} " << std::endl;
    
    string caption = "\\caption{Cutflow for " + sLEPCOL[ilep] + " " + sReg + " }";
    outTEX << caption << std::endl;
    
    outTEX << "\\end{center}" << std::endl;
    outTEX << "\\end{table}" << std::endl;
    outTEX << std::endl;
    outTEX << std::endl;
    
    outTEX << "\\FloatBarrier" << std::endl;
   
    
    outTEX.close();

  }


}
//_____________________________________________________________________________//
void loadBkg(string DSId, string sReg,std::vector<TH1F*> *_H1){

  TFile*_bkgFile = _ana->openFile(DSId);

  for(uint ilep=0; ilep<LEP.size()-1; ilep++){
    _ana->clearHistVect(_H1[ilep]);
    string hName = "DG2L_" + sReg + "_" + LEP[ilep] + "_DG2L_cutflow";
    _H1[ilep] = _ana->loadHisto(_bkgFile,DSId,hName,verbose);
  }

  //Add EE+MM+EM
  _H1[3].reserve(DGSys_N);
  _H1[3] = _ana->sumChannels(_H1[0],_H1[1],_H1[2]);

}

//_____________________________________________________________________________//
float getYield(std::vector<TH1F*> histV, int cut){
  
  if(histV[DGSys_NOM]==NULL){
    cerr << "Empty hist " << endl;
    abort();
  }

  return histV[DGSys_NOM]->GetBinContent(cut+1);
  

}
