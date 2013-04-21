#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"

#include "TSystem.h"

typedef unsigned uint;

string mth = "std";
//string dir = "histos_041413_21fb_n0139_Moriond_DD_v2/histOutputs/";
string dir = "histos_040613_21fb_n0138_Moriond_DD_v1/histOutputs/";

string sampleSet = "data12.txt";
//string sampleSet = "Zjets_SherpaAlpgenPythia.txt";
//string sampleSet = "WZ_ZZ_PowHeg.txt";
//string sampleSet = "WW_PowHeg_oldgg2WW.txt";
//string sampleSet = "Higgs.txt";
//string sampleSet = "top_MCNLO.txt";

TGuiUtils* _utils;
DrawPlots* _ana;


int main(int argc, char *argv[]){
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
 _ana = new DrawPlots(); 

 string _pathHisto  = string(getenv("HISTOANA")) + "/SusyAna/" +dir;
 cout << "Using dir " << _pathHisto << endl;


 vector<string> LEP;
 LEP.push_back("EE");
 LEP.push_back("MM");
 LEP.push_back("EM");

 vector<string> SR;
 //SR.push_back("CRWW");
 //SR.push_back("CRWW2");
 SR.push_back("SRmT2a");
 SR.push_back("SRmT2b");
 // SR.push_back("ZXCRWW");


 vector<string> SAMPLES;
 string _dirList = string(getenv("WORKAREA")) + "/SusyWeakProdAna/scripts/inputHistoList/";
 string fileName = _dirList + sampleSet;

 ifstream _file;
 _file.open(fileName.c_str());
 if (!_file.is_open()){
   cerr << "Failed opening " << fileName << endl;
   exit(1);
 }
 
 string _sName;
 while( _file >> _sName){
   SAMPLES.push_back(_sName);
   //cout << "Read " << _sName << endl;
 }
 
 
 
 for(uint iSR=0; iSR<SR.size(); iSR++){
   for(uint iLEP=0; iLEP<LEP.size(); iLEP++){
     float sum=0;
     cout << "----- " << SR[iSR]+"_"+LEP[iLEP] <<  " ----- " << endl;;
         
     for(uint iS=0; iS<SAMPLES.size(); iS++){
       string _hName  = "DG2L_" + SR[iSR] + "_" + LEP[iLEP] + "_DG2L_pred_NOM";
       
       string _subDir = SR[iSR] + "_" + LEP[iLEP] + "/";
       string _fHist;
       
       TString _sSample(SAMPLES[iS]);
       if(_sSample.Contains("Egamma") || _sSample.Contains("Muons")){
	 _fHist = "histo_" + SAMPLES[iS] + "_" + SR[iSR] + "_" + LEP[iLEP] + "_std.root";
       }
       else{
	 _fHist = "histo_" + SAMPLES[iS] + "_" + SR[iSR] + "_" + LEP[iLEP] + "_rlep.root";
       }

       const char* test = gSystem->FindFile((_pathHisto+ _subDir).c_str(),TString(_fHist));
       
       TFile* _f;
       if(test==0){
	 _fHist  = "histo_" + SAMPLES[iS] + "_" + SR[iSR] + "_" + LEP[iLEP] + "_rlep_NOM_EES_Z_UP.root";	 
	 _f = new TFile( (_pathHisto+ _subDir +_fHist).c_str(),"READ");
       }
       else{
	 _f = new TFile( (_pathHisto+ _subDir +_fHist).c_str(),"READ");
       }

       /*
       cout << "subDir " << _subDir << endl;
       cout << "fhist " << _fHist << endl;
       cout << "histo " << _hName << endl;
       */

       if(_f->IsOpen() == kFALSE){
	 cerr << "Failed opening " << _fHist << endl;
	 exit(1);
       }

       TH1F* _h = (TH1F*)  _f->Get(_hName.c_str())->Clone();
       printf("%s \t %3.2f \n",SAMPLES[iS].c_str(),_h->Integral(0,-1)); 
       sum+= _h->Integral(0,-1);
     }
     cout << " ===>> Total " << sum  << endl;
   }
   cout << endl;
 }






}
