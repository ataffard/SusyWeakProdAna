#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

typedef unsigned uint;

string mth = "std";
string dir = "";//"histos_102512_13fb_n0107_DD_predSR/";


TGuiUtils* _utils;
DrawPlots* _ana;


int main(int argc, char *argv[]){
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 

  std::vector< std::vector<float> > _skyNum;
  std::vector< std::vector<float> > _AnyNum;
  float run;
  float lepSFprod;
  float btagW;
  float trigW;
  std::vector< float> _line;  

  string _path  = string(getenv("WORKAREA")) + "/SusyWeakProdAna/run/jobLogs/cutflow_v5_102712";
  string _fSky  = _path + "/sky_mm_SR1_sort.log";
  string _fAny  = _path + "/anyes_mm_SR1_sort.log";

  //Load Sky file
  ifstream _fileSky;
  _fileSky.open(_fSky.c_str());
  if (!_fileSky.is_open()){
    cerr << "Failed opening " << _fSky << endl;
    exit(1);
  }
  

  while( _fileSky >> run >> lepSFprod >> btagW >> trigW ){
    _line.clear();
    _line.push_back(run);
    _line.push_back(lepSFprod);
    _line.push_back(btagW);
    _line.push_back(trigW);

    _skyNum.push_back(_line);
  }
  _fileSky.close();

  //Load Anyes file
  ifstream _fileAny;
  _fileAny.open(_fAny.c_str());
  if (!_fileAny.is_open()){
    cerr << "Failed opening " << _fAny << endl;
    exit(1);
  }
  
  while( _fileAny >> run >> lepSFprod >> btagW >> trigW ){
    _line.clear();
    _line.push_back(run);
    _line.push_back(lepSFprod);
    _line.push_back(btagW);
    _line.push_back(trigW);

    _AnyNum.push_back(_line);
  }
  _fileAny.close();


  for(uint iline=0; iline<_skyNum.size(); iline++){
    float diff_run       = _skyNum[iline][0]-_AnyNum[iline][0];
    float diff_lepSFprod = _skyNum[iline][1]-_AnyNum[iline][1];
    float diff_btagW     = _skyNum[iline][2]-_AnyNum[iline][2];
    float diff_trigW     = _skyNum[iline][3]-_AnyNum[iline][3];

    if(fabs(diff_lepSFprod)>0.0001 || fabs(diff_btagW)>0.0001 || fabs(diff_trigW)>0.0001)
      cout << run 
	   << " " << diff_lepSFprod 
	   << " " << diff_btagW 
	   << " " << diff_trigW << endl; 
  }




}
