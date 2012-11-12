/*

given a list of signal (DSId) & SR's output
txt file with 
   EE, MM, EM, TOT event yield with stat +sys.

   format/channels  nom stat sysUp sysDn


.L ../macros/signalYield.C
main(); makeSignalYieldTxt("pMSSM_Rizzo.txt","CR2LepOS")  


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

bool verbose = false;


//
// Variables
//
TGuiUtils* _utils;
DrawPlots* _ana;

TFile* _sigFile;

std::vector<TH1F*> _sigH1[4];//[LEP][SYS]
std::vector<string> LEP;


//
// Functions
//

void   makeSignalYieldTxt(string filename, string sReg);
std::vector<string>  readSignalList(string filename);
void   loadSignal(string DSId, string sReg);





//_____________________________________________________________________________//
int main(int argc, char *argv[]){
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 
  
  LEP.clear();
  LEP.push_back("EE");
  LEP.push_back("MM");
  LEP.push_back("EM");
  LEP.push_back("ALL");


}

//_____________________________________________________________________________//
void   makeSignalYieldTxt(string filename, string sReg){
  
  vector<string> DSId_list = readSignalList(filename);
  
  string fileName= "SignalYield_" + sReg + ".txt";
  std::ofstream outFile;
  outFile.open(fileName.c_str());
  if (!outFile.is_open()){
    printf("Problem opening output file .... bailing out \n %s \n",fileName.c_str());
    abort();
  }
  
  for(uint i=0; i<DSId_list.size(); i++){
    string DSId = DSId_list[i];
    loadSignal(DSId,sReg);
    outFile << DSId << "\t";
    for(uint ilep=0; ilep<LEP.size(); ilep++){
      Double_t nom, stat, sysUp, sysDn;
      _ana->getYield(_sigH1[ilep],nom, stat, sysUp, sysDn, verbose);
      outFile << nom << " " << stat << " " << sysUp << " " << sysDn << " \t"; 
    }
    outFile << endl;
  }

  outFile.close();
}

//_____________________________________________________________________________//
std::vector<string> readSignalList(string filename){
  //Open signal list file
  ifstream _file;
  string _name = string(getenv("WORKAREA"))+"/SusyWeakProdAna/data/"+filename;
  _file.open(_name.c_str());
  
  // Clear the seekg so that we can read the file multiple times
  _file.clear();
  _file.seekg(0,std::ios::beg); 
  
  // Check if can read the file
  if(_file.fail()) {
    cerr << "ERROR Cannot open file " << _name << std::endl;
    abort();
  }
  
  vector<string> _dsIdList;
  
  // Read through file
  std::string currentLine = ""; 
  while(!_file.eof()){
    // Get the line
    char _ss[200];
    std::getline(_file,currentLine);
    sscanf(currentLine.c_str(),"%s",&_ss[0]);
    _dsIdList.push_back(string(_ss));
  }  
  
  return _dsIdList;
  
}



//_____________________________________________________________________________//
void loadSignal(string DSId, string sReg){
  _sigFile = _ana->openFile(DSId);

  for(uint ilep=0; ilep<LEP.size()-1; ilep++){
    _ana->clearHistVect(_sigH1[ilep]);
    string hName = "DG2L_" + sReg + "_" + LEP[ilep] + "_DG2L_pred";
    _sigH1[ilep] = _ana->loadHisto(_sigFile,DSId,hName);
  }

  //Add EE+MM+EM
  _sigH1[3].reserve(DGSys_N);
  _sigH1[3] = _ana->sumChannels(_sigH1[0],_sigH1[1],_sigH1[2]);

}







