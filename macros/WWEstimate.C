#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"

#include <vector>
#include <string>
#include "TFile.h"

typedef unsigned uint;

TGuiUtils* _utils;
DrawPlots* _ana;

//
// Constants
//
//string _pathHisto  =  string(getenv("WORKAREA")) + "/histoAna/SusyAna/";
//string _dir= "histos_101312_13fb_n0105_DD_blindData_noMetRel/";


//
// Variables
//
vector<string> LEP;
TH1F* hMC[5];


//
// Functions
//
void openHist(string mode="DD",
	      string Top="histo_topDil_Sherpa",
	      string WW="histo_WW_Sherpa",
	      string ZX="histo_ZX_Sherpa",		      
	      string Ztt="histo_ZTauTaujets_Sherpa",
	      string Fake="histo_data12_fake");

void getZX_SF_WWCR();


//_____________________________________________________________________________//
int main(int argc, char *argv[]){
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 
  openHist();
  
  LEP.clear();
  LEP.push_back("EE");
  LEP.push_back("MM");
  LEP.push_back("EM");
}
//--------------------------------------------------------------------------------
void openHist(string mode,string Top,string WW,string ZX,string Ztt,string Fake)
{
  _ana->openHistoFiles(mode,Top,WW, ZX, Ztt, Fake);
}

//_____________________________________________________________________________//
void getZX_SF_WWCR()
{
  const string sSR  = "ZXCR1";
  const string sVar = "metrel";
  const float  metRel_low  = 0;
  const float  metRel_high = 10000;

  float _data[3];
  float _errData[3];
  float _predMC[3][5];
  float _errMC[3][5];

  //Get the intergrals
  for(uint il=0; il<LEP.size(); il++){
    string hName= "DG2L_" + sSR + "_" + LEP[il] + "_DG2L_" + sVar; 
    cout << " Channel " << LEP[il] << endl;
    TH1F* _hd = (TH1F*) _ana->getHisto("data12_std",hName);
    int bLow  = _hd->FindBin(metRel_low);
    int bHigh = _hd->FindBin(metRel_high);
    Double_t _sig=0;
    _data[il] = _hd->IntegralAndError(bLow,bHigh,_sig); 
    _errData[il]=_sig;
    cout << "\t Data: " << _data[il] 
	   << " +/- " << _errData[il] << endl;
    
    for(int imc=0; imc<OTHER; imc++){
      TH1F* _h  = (TH1F*) _ana->retreiveHisto(imc,hName);
      _predMC[il][imc] = _h->IntegralAndError(bLow,bHigh,_sig);
      _errMC[il][imc]  = _sig;
      cout << "\t MC " << _ana->SFILE[imc] << ": " << _predMC[il][imc] 
	   << " +/- " << _errMC[il][imc] << endl;
    }
  }

  //Compute Data/MC SF
  
  for(uint il=0; il<LEP.size(); il++){
    float otherB = _predMC[il][FAKE] 
                 + _predMC[il][Ztt]
                 + _predMC[il][WW] 
                 + _predMC[il][TOP];
    float _errOtherB = pow(_errMC[il][FAKE],2) 
                     + pow(_errMC[il][Ztt],2) 
                     + pow(_errMC[il][WW],2) 
                     + pow(_errMC[il][TOP],2) ;
    _errOtherB = sqrt(_errOtherB);

    
    float ZX_data = _data[il] - otherB;
    float ZX_data_Err = pow(_errOtherB,2) + pow(_errData[il],2);
    ZX_data_Err = sqrt(ZX_data_Err);
    
    float SF     = ZX_data / _predMC[il][ZX];
    float SF_err = pow(ZX_data_Err/ZX_data,2) + pow(_errMC[il][ZX]/_predMC[il][ZX],2);
    SF_err = sqrt(SF_err);

    cout << "SF: " << SF << " +/- " << SF_err << endl;
    

    
  }



}
