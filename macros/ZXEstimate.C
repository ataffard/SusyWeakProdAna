#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"

#include <vector>
#include <string>
#include "TFile.h"

typedef unsigned uint;

TGuiUtils* _utils;
DrawPlots* _ana;

//
// Variables
//
vector<string> LEP;


//
// Functions
//
void openHist(string mode="DD",
	      string Top="histo_topDil_Sherpa",
	      string WW="histo_WW_Sherpa",
	      string ZX="histo_ZX_Sherpa",		      
	      string Ztt="histo_ZTauTaujets_Sherpa",
	      string Fake="histo_data12_fake");

void getZX_TF();


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
//
// TF of Z+X background from CR's to SR's
//
void getZX_TF()
{
  vector<string> ZXCR;
  ZXCR.push_back("ZXCR1");
  ZXCR.push_back("ZXCR3");
  ZXCR.push_back("ZXCR4");

  vector<string> ZXSR;
  ZXSR.push_back("SRjveto");
  ZXSR.push_back("SR2jets");
  ZXSR.push_back("SRmT2");//pre mt2!!!

  

  for(uint il=0; il<LEP.size(); il++){
    cout << "****** "<< LEP[il] << " *******" << endl;
    for(uint ireg=0; ireg<ZXCR.size(); ireg++){
      string hNameCR= "DG2L_" + ZXCR[ireg] + "_" + LEP[il] + "_DG2L_pred"; 
      string hNameSR= "DG2L_" + ZXSR[ireg] + "_" + LEP[il] + "_DG2L_pred"; 
      
      _ana->grabHisto(hNameCR,false);
      TH1F*              _hCR = (TH1F*)  _ana->_mcH1[ZX][DGSys_NOM]->Clone();
      TGraphAsymmErrors* _sysErr_CR = getSysErrorBand(_hCR);
      
      _ana->grabHisto(hNameSR,false);
      TH1F*              _hSR = (TH1F*)  _ana->_mcH1[ZX][DGSys_NOM]->Clone();
      TGraphAsymmErrors* _sysErr_SR = getSysErrorBand(_hSR);
      
      //      TH1F* _hCR = (TH1F*) _ana->getHisto("ZX_Sherpa_rlep",hNameCR);
      //      TH1F* _hSR = (TH1F*) _ana->getHisto("ZX_Sherpa_rlep",hNameSR);

      TH1F* _hRatio = _utils->calcRatio(_hSR,_hCR,"TF",""); //uncorrelated 
      
      Double_t _err=0;
      Double_t _TF=_hRatio->IntegralAndError(0,-1,_err);
      cout << "\t " << ZXSR[ireg] 
	   << " TF " << _TF 
	   << "+/- " << _err << " (stat.)"
	   << endl;
	
    }
  }
  

}
