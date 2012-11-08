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

void  get_ZX_Est();
void  get_ZXCR_data(int ilep, int ireg, int isys,
		    Double_t &est, Double_t &err, 
		    Double_t &SF, Double_t &SFerr);
void  get_ZX_TF(int ilep, int ireg, int isys, 
		Double_t &est, Double_t &err);
void  get_MT2Eff(int ilep, string ireg, int isys,  
		 Double_t &est, Double_t &err);
void  get_ZX_SF_SR(int ilep, int ireg, int isys, 
		   Double_t pred, Double_t predErr, 
		   Double_t &SF, Double_t &SFerr);


void  get_ZX_BkgErr(int ilep, int ireg, Double_t &SF_up, Double_t &SF_dn);


//_____________________________________________________________________________//
int main(int argc, char *argv[]){
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 
  openHist();
  
  LEP.clear();
  //LEP.push_back("EE");
  LEP.push_back("MM");
  //  LEP.push_back("EM");

  ZXCR.push_back("ZXCR1"); //SRjveto
  /*
  ZXCR.push_back("ZXCR3"); //SR2jets
  ZXCR.push_back("ZXCR4"); //MT2 via preMt2 and Mt2Eff ->mT2a
  ZXCR.push_back("ZXCR4"); //MT2 via preMt2 and Mt2Eff ->mT2b
  ZXCR.push_back("ZXCR5"); //NWW1
  ZXCR.push_back("ZXCR6"); //SRmT2a
  ZXCR.push_back("ZXCR7"); //SRmT2b
  */


  ZXSR.push_back("SRjveto");
  /*
  ZXSR.push_back("SR2jets");
  ZXSR.push_back("preSRmT2"); // ->mT2a
  ZXSR.push_back("preSRmT2"); // ->mT2b
  ZXSR.push_back("NWW1");
  ZXSR.push_back("SRmT2");
  ZXSR.push_back("SRmT2b");
  */
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
void get_ZX_Est()
{
  int iMT2=0;

  for(uint il=0; il<LEP.size(); il++){//EE && MM only
    cout << "****** "<< LEP[il] << " *******" << endl;
    for(uint ireg=0; ireg<ZXCR.size(); ireg++){
      
      string fileName = "ZX_SF_" + LEP[il] + "_" +ZXSR[ireg] + ".txt";
      std::ofstream txt(fileName.c_str());
      std::ostream & out = txt;
      if (!txt.is_open()){
	printf("Problem opening output file .... bailing out \n %s \n",fileName.c_str());
	return;
      }
      else printf("Output file opened: %s \n",fileName.c_str());
      
      for(uint isys=DGSys_NOM; isys<DGSys_BKGMETHOD_UP; isys++){
	cout << " SYS " << DG2LSystNames[isys] << endl;
	

	//Get ZX SF in CR's
	Double_t ZX_CR_data, ZX_CR_data_err; 
	Double_t _SF_CR, _SF_CR_err;
	get_ZXCR_data(il,ireg,isys, ZX_CR_data, ZX_CR_data_err,_SF_CR, _SF_CR_err);
	
	Double_t _SF_SR = _SF_CR;
	Double_t _SF_SR_err = _SF_CR_err;

	//Get TF = SR/CR for ZX
	Double_t _TF, _TF_err;
	get_ZX_TF(il,ireg,isys, _TF, _TF_err);
	
	//ZX prediction
	float _frac1 = ZX_CR_data_err/ZX_CR_data;
	float _frac2 = _TF_err/_TF;
	float _combErr = sqrt(_frac1*_frac1 + _frac2*_frac2);
	
	Double_t _ZX_pred     = ZX_CR_data * _TF; 
	Double_t _ZX_pred_err = ZX_CR_data * _TF *_combErr; 
	
	string sReg = ZXSR[ireg];
	//
	//For MT2 a/b need eff (old way)
	//
	if(ZXCR[ireg]=="ZXCR4"){
	  Double_t eff_mt2, eff_err_mt2;
	  if(iMT2==0) get_MT2Eff(il, "SRmT2", isys, eff_mt2, eff_err_mt2);
	  else        get_MT2Eff(il, "SRmT2b",isys, eff_mt2, eff_err_mt2);
	  
	  float _frac3 = eff_err_mt2/eff_mt2;
	  float _combErr_mt2 = sqrt(_frac1*_frac1 + _frac2*_frac2 + _frac3*_frac3);
	  
	  _ZX_pred *= eff_mt2;
	  _ZX_pred_err = _ZX_pred * _combErr_mt2;

	  float _frac4 =  _SF_CR_err/_SF_CR;
	  float _combErr_SF = sqrt(_frac3*_frac3 + _frac4*_frac4);

	  _SF_SR     *= eff_mt2;
	  _SF_SR_err = _SF_SR * _combErr_SF;
	  
	  if(iMT2==0) sReg = "SRmT2a"; 
	  else        sReg = "SRmT2b"; 
	  iMT2++;
	}
	
	//Get ZX SF in SR (cross check - should be the same as in CR's
	Double_t _SF, _SF_err;
	get_ZX_SF_SR(il,ireg,isys, _ZX_pred,_ZX_pred_err,_SF, _SF_err);
	
	
	//Print results
	cout << "\t ZX " << sReg
	     << "\t "   << _ZX_pred 
	     << " +/- " << _ZX_pred_err << " (stat.)"
	     << endl;
	cout << "\t SF ZX " << sReg
	     << "\t "   << _SF_SR 
	     << " +/- " << _SF_SR_err << " (stat.)"
	     << endl;
	

	//
	//Dump output to file
	//
	out << DG2LSystNames[isys] << "\t" << _SF_SR << endl;
	if(isys==DGSys_NOM){
	  float CR_stat_up = _SF_SR + _SF_SR_err;
	  float CR_stat_dn = _SF_SR - _SF_SR_err;
	  out << "CRSTAT_UP " << "\t"  << CR_stat_up << endl;
	  out << "CRSTAT_DN " << "\t"  << CR_stat_dn << endl;
	}
	
      }//sys
      Double_t BKG_UP, BKG_DN;
      get_ZX_BkgErr(il,ireg, BKG_UP, BKG_DN);
      out << "BKGMETHOD_UP " << "\t"  << BKG_UP << endl;
      out << "BKGMETHOD_DN " << "\t"  << BKG_DN << endl;
      
      txt.close();
	
    }//Region
    cout << endl;
  } //Lepton
  
 

}

//_____________________________________________________________________________//
//
// SF of Z+X background in CR's 
//
void get_ZXCR_data(int il, int ireg, int isys,
		   Double_t &est, Double_t &err, 
		   Double_t &SF, Double_t &SFerr)
{
  est =0;
  err =0;
  
  //CR Histos
  string hNameCR= "DG2L_" + ZXCR[ireg] + "_" + LEP[il] + "_DG2L_pred"; 
  _ana->grabHisto(hNameCR,true);

  //ZX MC in CR's
  TH1F* _h_ZX_mc      = (TH1F*)  _ana->getMcHisto(ZX,isys)->Clone(); 
  
  //Data in CR's
  TH1F* _h_data       = (TH1F*)  _ana->getDataHisto()->Clone(); 
  
  //Other bkg
  TH1F* _h_fake       = (TH1F*)  _ana->getMcHisto(FAKE,isys)->Clone();       
  TH1F* _h_top        = (TH1F*)  _ana->getMcHisto(TOP,isys)->Clone();       
  TH1F* _h_WW         = (TH1F*)  _ana->getMcHisto(WW,isys)->Clone();       
  TH1F* _h_Ztt        = (TH1F*)  _ana->getMcHisto(Ztt,isys)->Clone();       
  
  TH1F* _h_oBkg = (TH1F*) _h_fake->Clone();
  _h_oBkg->SetTitle("oBkg");  _h_oBkg->SetName("oBkg");
  _h_oBkg->Add(_h_top);
  _h_oBkg->Add(_h_WW);
  _h_oBkg->Add(_h_Ztt);
  
  //ZX_data = data - oBkg
  TH1F* _h_ZX_data = (TH1F*) _h_data->Clone();
  _h_ZX_data->SetTitle("ZX_data"); _h_ZX_data->SetName("ZX_data");
  _h_ZX_data->Add(_h_oBkg,-1);
  est = _h_ZX_data->IntegralAndError(0,-1,err);
  
  //ZX SF in CR = ZX_data/ZX_mc
  TH1F* _h_ZX_CR_SF = _ana->calcRatio(_h_ZX_data,_h_ZX_mc,"SF",""); //uncorrelated
  SF = _h_ZX_CR_SF->IntegralAndError(0,-1,SFerr);

  //Print
  cout << std::setprecision(5) 
       << ">>> ZX estimate in " << ZXCR[ireg] << "\n"
       << "\t Data \t\t" << _h_data->Integral(0,-1)
       << "\t oBkg " << _h_oBkg->Integral(0,-1)
       << "\t ZX_mc " << _h_ZX_mc->Integral(0,-1)
       << "\t ZX_data " << est  << " +/- " << err
       << "\t ZX_CR SF " << SF << " +/- " << SFerr
       << endl;
  
  _h_ZX_mc->Delete();
  _h_data->Delete();
  _h_fake->Delete();
  _h_top->Delete();
  _h_WW->Delete();
  _h_Ztt->Delete();
  _h_oBkg->Delete();
  _h_ZX_data->Delete();
  _h_ZX_CR_SF->Delete();
}
//_____________________________________________________________________________//
// TF of ZX SR/CR
//
void  get_ZX_TF(int ilep, int ireg, int isys, Double_t &est, Double_t &err)
{
  est = 1;
  err = 0;
  
  string hNameCR= "DG2L_" + ZXCR[ireg] + "_" + LEP[ilep] + "_DG2L_pred"; 
  string hNameSR= "DG2L_" + ZXSR[ireg] + "_" + LEP[ilep] + "_DG2L_pred"; 

  //ZX in CR's
  _ana->grabHisto(hNameCR,true);
  TH1F* _hCR       = (TH1F*)  _ana->getMcHisto(ZX,isys)->Clone();
  //ZX in SR's
  _ana->grabHisto(hNameSR,true);
  TH1F* _hSR       = (TH1F*)  _ana->getMcHisto(ZX,isys)->Clone();

  //Calc TF
  TH1F* _hRatio    = _ana->calcRatio(_hSR,_hCR,"TF",""); //uncorrelated 
  est =_hRatio->IntegralAndError(0,-1,err);
  
  cout << std::setprecision(5) 
       << "\t ZX CR \t\t"  << _hCR->Integral(0,-1)
       << "\t ZX SR "  << _hSR->Integral(0,-1)
       << " TF "       << est 
       << " +/- "      << err
       << endl;

  _hCR->Delete();
  _hSR->Delete();
  _hRatio->Delete();
}

//_____________________________________________________________________________//
//
// SF pred/MC
//
void  get_ZX_SF_SR(int ilep, int ireg, int isys,  
		   Double_t pred, Double_t predErr, 
		   Double_t &SF, Double_t &SFerr)
{
  // ZX in SR
  string hNameSR= "DG2L_" + ZXSR[ireg] + "_" + LEP[ilep] + "_DG2L_pred"; 
  _ana->grabHisto(hNameSR,true);

  //ZX MC in SR's
  TH1F* _h_ZX_mc      = (TH1F*)  _ana->getMcHisto(ZX,isys)->Clone(); 
  
  TH1F* _h_ZX_pred = (TH1F*) _h_ZX_mc->Clone();
  _h_ZX_pred->Reset();
  _h_ZX_pred->SetName("ZX_pred");  _h_ZX_pred->SetTitle("ZX_pred");
  _h_ZX_pred->SetBinContent(1,pred);
  _h_ZX_pred->SetBinError(1,predErr);

  //Calc SF
  TH1F* _hRatio    = _ana->calcRatio(_h_ZX_pred,_h_ZX_mc,"SF",""); //uncorrelated 
  SF =_hRatio->IntegralAndError(0,-1,SFerr);

  _h_ZX_mc->Delete();
  _h_ZX_pred->Delete();
  _hRatio->Delete();
  
}

//_____________________________________________________________________________//
//
// MT2 Eff SRMt2a/b - preMT2
//
void  get_MT2Eff(int ilep, string sreg, int isys,  
		 Double_t &est, Double_t &err)
{
  est=1;
  err=0;
  
  //ZX MC in SR's
  string hNameCR= "DG2L_" + sreg + "_" + LEP[ilep] + "_DG2L_pred"; 
  _ana->grabHisto(hNameCR,true);
  TH1F* _h_ZX_SR      = (TH1F*)  _ana->getMcHisto(ZX,isys)->Clone(); 
  
  //ZX MC in preMT2
  hNameCR= "DG2L_preSRmT2_" + LEP[ilep] + "_DG2L_pred"; 
  _ana->grabHisto(hNameCR,true);
  TH1F* _h_ZX_preSR   = (TH1F*)  _ana->getMcHisto(ZX,isys)->Clone(); 
  
  //Efficiency
  TH1F* _hRatio =  _ana->calcRatio(_h_ZX_SR,_h_ZX_preSR,"EFF",""); //uncorrelated 
  
  est=_hRatio->IntegralAndError(0,-1,err);
  
  //
  //Print results
  //
  cout << std::setprecision(5) 
       << "\t " << sreg 
       << "\t Eff " << est 
       << " +/- " << err << " (stat.)"
       << endl;
  
  _h_ZX_SR->Delete();
  _h_ZX_preSR->Delete();
  _hRatio->Delete();
  
}



//_____________________________________________________________________________//
//
// Compute additional error needed to cover for data/MC disagreement in CR.
//
void  get_ZX_BkgErr(int ilep, int ireg, Double_t &SF_up, Double_t &SF_dn){

  
  //CR Histos
  string hNameCR= "DG2L_" + ZXCR[ireg] + "_" + LEP[ilep] + "_DG2L_pred"; 
  _ana->grabHisto(hNameCR,true);

  //
  //Get ZX data estimate
  //

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
  
  //ZX_data = data - oBkg
  TH1F* _h_ZX_data = (TH1F*) _h_data->Clone();
  _h_ZX_data->SetTitle("ZX_data"); _h_ZX_data->SetName("ZX_data");
  _h_ZX_data->Add(_h_oBkg,-1);
  
  Double_t ZX_data_err;
  Double_t ZX_data = _h_ZX_data->IntegralAndError(0,-1,ZX_data_err);
  
  //
  // Get ZX MC prediction and combined stat+sys error
  //
  Double_t _nom=0;
  Double_t _sys_up=0;
  Double_t _sys_dn=0;
  
  for(uint isys=DGSys_NOM; isys<DGSys_BKGMETHOD_UP; isys++){
    //ZX MC in CR's
    TH1F* _h_ZX_mc      = (TH1F*)  _ana->getMcHisto(ZX,isys)->Clone(); 
    
    if(isys==DGSys_NOM){
      Double_t _stat;
      _nom = _h_ZX_mc->IntegralAndError(0,-1,_stat);
      _sys_up = _stat;
      _sys_dn = _stat;
    }
    else{
      Double_t _shift = _h_ZX_mc->Integral(0,-1);
      if(_shift > _nom ) _sys_up += pow(_shift-_nom,2);
      else               _sys_dn += pow(_shift-_nom,2);
    }
    _h_ZX_mc->Delete();
  }
  _sys_up = sqrt(_sys_up);
  _sys_dn = sqrt(_sys_dn);


  //
  // Compute additional error sigma_b such that cover the whole gap
  //
  // [ZX_data - ZX_mc] = sqrt(sigma_a^2 + sigma_b^2)  
  //

  Double_t sigma_tot;
  if(ZX_data > _nom ) sigma_tot = (ZX_data - ZX_data_err) - _nom;
  else                sigma_tot = (ZX_data + ZX_data_err) - _nom;

  Double_t sigma_b_up = 0;
  Double_t sigma_b_dn = 0;
  if(sigma_tot >0){ //use sys_up
    sigma_b_up = pow(sigma_tot,2) - pow(_sys_up,2);
  }
  else { //use sys_dn
    sigma_b_dn = pow(sigma_tot,2) - pow(_sys_dn,2);
  }
  if(sigma_b_up>0) sigma_b_up = sqrt(sigma_b_up);
  if(sigma_b_dn>0) sigma_b_dn = sqrt(sigma_b_dn);

  cout << " Additional error needed for ZX \n" 
       << _nom 
       << " + " << _sys_up 
       << " - " << _sys_dn 
       << " extra + " << sigma_b_up 
       << " extra - " << sigma_b_dn 
       << endl;


  //
  // Get corresponding SF
  //
  SF_up = ZX_data /(_nom + sigma_b_up);
  SF_dn = ZX_data /(_nom - sigma_b_dn);
  
  
  _h_data->Delete();
  _h_fake->Delete();
  _h_top->Delete();
  _h_WW->Delete();
  _h_Ztt->Delete();
  _h_oBkg->Delete();
  _h_ZX_data->Delete();


}
