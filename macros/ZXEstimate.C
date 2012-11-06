#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"
#include "SusyWeakProdAna/Histos_Common.h"
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
vector<string> ZXCR;
vector<string> ZXSR;

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
float get_ZXCR_data(int ilep, int ireg);


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

  ZXCR.push_back("ZXCR1");
  ZXCR.push_back("ZXCR3");
  ZXCR.push_back("ZXCR4");
  ZXCR.push_back("ZXCR5");

  ZXSR.push_back("SRjveto");
  ZXSR.push_back("SR2jets");
  ZXSR.push_back("preSRmT2");
  ZXSR.push_back("NWW1");
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
  
  for(uint il=0; il<LEP.size()-1; il++){//EE && MM only
    cout << "****** "<< LEP[il] << " *******" << endl;
    for(uint ireg=0; ireg<ZXCR.size(); ireg++){
      string hNameCR= "DG2L_" + ZXCR[ireg] + "_" + LEP[il] + "_DG2L_pred"; 
      string hNameSR= "DG2L_" + ZXSR[ireg] + "_" + LEP[il] + "_DG2L_pred"; 

      float ZX_data = get_ZXCR_data(il,ireg);

      TH1F* _hCR;
      TH1F* _hSR;
      TH1F* _hRatio;
      //
      //Obtain central value
      //
      //ZX in CR's
      _ana->grabHisto(hNameCR,true);
      _hCR       = (TH1F*)  _ana->getMcHisto(ZX,DGSys_NOM)->Clone();
      //ZX in SR's
      _ana->grabHisto(hNameSR,true);
      _hSR       = (TH1F*)  _ana->getMcHisto(ZX,DGSys_NOM)->Clone();
      //Calc TF
      _hRatio    = _ana->calcRatio(_hSR,_hCR,"TF",""); //uncorrelated 
      
      Double_t _err=0;
      Double_t _TF=_hRatio->IntegralAndError(0,-1,_err);
      
      //
      //Get the sys err
      //
      float err_up = 0; 
      float err_dn = 0;
      for(uint isys=DGSys_EES_Z_UP; isys<DGSys_BKGMETHOD_UP; isys++){
	//ZX in CR's
	_ana->grabHisto(hNameCR,true);
	_hCR       = (TH1F*)  _ana->getMcHisto(ZX,isys)->Clone();
	//ZX in SR's
	_ana->grabHisto(hNameSR,true);
	_hSR       = (TH1F*)  _ana->getMcHisto(ZX,isys)->Clone();
	//Calc TF
	_hRatio    = _ana->calcRatio(_hSR,_hCR,"TF",""); //uncorrelated 

	Double_t _errSys=0;
	Double_t _TFSys=_hRatio->IntegralAndError(0,-1,_errSys);
	
	if(_TFSys > _TF) err_up += pow(_TFSys-_TF,2);
	else             err_dn += pow(_TFSys-_TF,2);
      }
      err_up = sqrt(err_up);
      err_dn = sqrt(err_dn);

      //
      //Print results
      //
      cout << std::setprecision(3) 
	   << "\t " << ZXSR[ireg] 
	   << "\t TF " << _TF 
	   << " +/- " << _err << " (stat.)"
	   << " + " << err_up << " (sys.)"
	   << " - " << err_dn << " (sys.)"
	   << endl;
	

      cout << "Predicted ZX in SR's " <<
	   << "\t " << ZX_data * _TF
	   << " +/- " << ZX_data * _err << " (stat.)"
	   << " + " <<  ZX_data * err_up << " (sys.)"
	   << " - " <<  ZX_data * err_dn << " (sys.)"
	   << endl;

      _hCR->Delete();
      _hSR->Delete();
      _hRatio->Delete();
      
    }
  }
  
 

}

//_____________________________________________________________________________//
//
// SF of Z+X background obtained from CR's 
//
float get_ZXCR_data(int il, int ireg)
{
  string hNameCR= "DG2L_" + ZXCR[ireg] + "_" + LEP[il] + "_DG2L_pred"; 
  cout << "Grab " << hNameCR << endl;
  _ana->grabHisto(hNameCR,false);
  
  //ZX MC in CR's
  TH1F* _h_ZX_mc      = (TH1F*)  _ana->getMcHisto(ZX,DGSys_NOM)->Clone(); 
  cout << "Name " << _h_ZX_mc->Integral(0,-1) << endl;
  
  //Data in CR's
  TH1F* _h_data       = (TH1F*)  _ana->getDataHisto()->Clone(); 
  
  //Other bkg
  TH1F* _h_fake       = (TH1F*)  _ana->getMcHisto(FAKE,DGSys_NOM)->Clone();       
  TH1F* _h_top        = (TH1F*)  _ana->getMcHisto(TOP,DGSys_NOM)->Clone();       
  TH1F* _h_WW         = (TH1F*)  _ana->getMcHisto(WW,DGSys_NOM)->Clone();       
  TH1F* _h_Ztt        = (TH1F*)  _ana->getMcHisto(Ztt,DGSys_NOM)->Clone();       
  
  TH1F* _h_oBkg = (TH1F*) _h_fake->Clone();
  _h_oBkg->SetTitle("oBkg");  _h_oBkg->SetName("oBkg");
  _h_oBkg->Add(_h_top);
  _h_oBkg->Add(_h_WW);
  _h_oBkg->Add(_h_Ztt);
  
  TH1F* _h_ZX_data = (TH1F*) _h_data->Clone();
  _h_ZX_data->SetTitle("ZX_data"); _h_ZX_data->SetName("ZX_data");
  _h_ZX_data->Add(_h_oBkg,-1);
  
  cout << std::setprecision(5) 
       << ">>> ZX estimate from CR's \n"
       << "\t " << ZXCR[ireg] 
       << "\t Data " << _h_data->Integral(0,-1)
       << "\t oBkg " << _h_oBkg->Integral(0,-1)
       << "\t ZX_mc " << _h_ZX_mc->Integral(0,-1)
       << "\t ZX_data " << _h_ZX_data->Integral(0,-1)
       << endl;
  
  
  _h_ZX_mc->Delete();
  _h_data->Delete();
  _h_fake->Delete();
  _h_top->Delete();
  _h_WW->Delete();
  _h_Ztt->Delete();
  _h_oBkg->Delete();
  _h_ZX_data->Delete();


  return _h_ZX_data->Integral(0,-1);
}
