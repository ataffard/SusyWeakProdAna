/*
 .L ../macros/ZXEstimate.C
 main()
get_ZX_Est(); //uncommented

plots: do one by one - fix range one some plots

Then run 
.L ../macros/make_ZX_texTables.C++
 make_ZX_texTables("TF");

*/

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

bool verbose = false;

//
// Functions
//
void openHist(string mode="DD",
	      string Top="histo_topDil_Sherpa",
	      string WW="histo_WW_Sherpa",
	      //string ZX="histo_ZX_Sherpa",		      
	      string ZX="histo_ZX_SherpaPowheg",
	      string Ztt="histo_ZTauTaujets_Sherpa",
	      string Fake="histo_data12_fake");

void  get_ZX_Est();
void  get_ZXCR_data(int ilep, int ireg, int isys,
		    Double_t &est, Double_t &err, 
		    Double_t &SF, Double_t &SFerr);
void  get_ZXSR_mc(int ilep, int ireg, int isys,
		    Double_t &est, Double_t &err);
void  get_ZX_TF(int ilep, int ireg, int isys, 
		Double_t &est, Double_t &err);
void  get_MT2Eff(int ilep, string ireg, int isys,  
		 Double_t &est, Double_t &err);
void  get_ZX_SF_SR(int ilep, int ireg, int isys, 
		   Double_t pred, Double_t predErr, 
		   Double_t &SF, Double_t &SFerr);

void  get_ZX_BkgErr(int ilep, int ireg, 
		    Double_t &bkgSysUp, Double_t &bkgSysDn, 
		    Double_t &ZX_mc_bkgUp, Double_t &ZX_mc_bkgDn, 
		    Double_t &SF_up, Double_t &SF_dn);

void  make_ZXPlots();


//_____________________________________________________________________________//
int main(int argc, char *argv[]){
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 
  
  LEP.clear();
  LEP.push_back("EE");
  //  LEP.push_back("MM");
  //LEP.push_back("EM");

  ZXCR.push_back("ZXCR1"); //SRjveto
  ZXCR.push_back("ZXCR3"); //SR2jets
  ZXCR.push_back("ZXCR4"); //MT2 via preMt2 and Mt2Eff ->mT2a
  ZXCR.push_back("ZXCR4"); //MT2 via preMt2 and Mt2Eff ->mT2b
  ZXCR.push_back("ZXCR5"); //NWW1
  ZXCR.push_back("ZXCR5"); //NWW2
  ZXCR.push_back("ZXCR6"); //SRmT2a
  ZXCR.push_back("ZXCR7"); //SRmT2b

  ZXSR.push_back("SRjveto");
  ZXSR.push_back("SR2jets");
  ZXSR.push_back("preSRmT2"); // ->mT2a
  ZXSR.push_back("preSRmT2"); // ->mT2b
  ZXSR.push_back("NWW1");
  ZXSR.push_back("NWW2");
  ZXSR.push_back("SRmT2");
  ZXSR.push_back("SRmT2b");

  get_ZX_Est();

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
  openHist();

  for(uint il=0; il<LEP.size(); il++){//EE && MM only
    int iMT2=0;
    if(verbose) cout << "****** "<< LEP[il] << " *******" << endl;
    for(uint ireg=0; ireg<ZXCR.size(); ireg++){
      cout << "Get ZX est for " << LEP[il] << " " << ZXSR[ireg] << endl;
      
      string sReg = ZXSR[ireg];
      if(ZXCR[ireg]=="ZXCR4"){
	if(iMT2==0) sReg=ZXSR[ireg]+"a";
	else        sReg=ZXSR[ireg]+"b";
      }

      //
      //Output SF file
      //
      string fileName= "ZX_SF_" + LEP[il] + "_" + sReg + ".txt";
      std::ofstream txt(fileName.c_str());
      std::ostream & outSF = txt;
      if (!txt.is_open()){
	printf("Problem opening output file .... bailing out \n %s \n",fileName.c_str());
	return;
      }
      
      //
      //Output TF file
      //
      fileName= "ZX_TF_" + LEP[il] + "_" + sReg + ".txt";
      std::ofstream txt2(fileName.c_str());
      std::ostream & outTF = txt2;
      if (!txt2.is_open()){
	printf("Problem opening output file .... bailing out \n %s \n",fileName.c_str());
	return;
      }


      //
      //Output ZX est data file
      //
      fileName= "ZX_EST_" + LEP[il] + "_" + sReg + ".txt";
      std::ofstream txt3(fileName.c_str());
      std::ostream & outEST = txt3;
      if (!txt3.is_open()){
	printf("Problem opening output file .... bailing out \n %s \n",fileName.c_str());
	return;
      }

      //
      //Output MT2 Eff files
      //
      if(ZXCR[ireg]=="ZXCR4"){
	fileName= "ZX_MT2aEff_" + LEP[il] + "_" + sReg + ".txt";
	std::ofstream txt4(fileName.c_str());
	std::ostream & outMt2aEff = txt4;
	if (!txt4.is_open()){
	  printf("Problem opening output file .... bailing out \n %s \n",fileName.c_str());
	  return;
	}
	
	fileName= "ZX_MT2bEff_" + LEP[il] + "_" + sReg + ".txt";
	std::ofstream txt5(fileName.c_str());
	std::ostream & outMt2bEff = txt5;
	if (!txt5.is_open()){
	  printf("Problem opening output file .... bailing out \n %s \n",fileName.c_str());
	  return;
	}
      }

      //
      //Output ZX SR mc file
      //
      fileName= "ZX_SREST_" + LEP[il] + "_" + sReg + ".txt";
      std::ofstream txt6(fileName.c_str());
      std::ostream & outZXSR = txt6;
      if (!txt6.is_open()){
	printf("Problem opening output file .... bailing out \n %s \n",fileName.c_str());
	return;
      }

      //
      // Loop over sys
      //
      for(uint isys=DGSys_NOM; isys<DGSys_BKGMETHOD_UP; isys++){
	if(verbose) cout << " SYS " << DG2LSystNames[isys] << endl;
	

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
	
	//ZX MC in SR
	Double_t ZX_SR_mc, ZX_SR_mc_err; 
	get_ZXSR_mc(il,ireg,isys, ZX_SR_mc, ZX_SR_mc_err);


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

	  //_SF_SR     *= eff_mt2;
	  //_SF_SR_err = _SF_SR * _combErr_SF;
	  
	  if(iMT2==0){
	    sReg = "SRmT2a"; 
	    //
	    //Dump outout Eff  to file
	    //
	    outMt2aEff << DG2LSystNames[isys] << "\t" << eff_mt2 << endl;
	    if(isys==DGSys_NOM){
	      float CR_stat_up = eff_mt2 + eff_err_mt2;
	      float CR_stat_dn = eff_mt2 - eff_err_mt2;
	      outMt2aEff << "CRSTAT_UP " << "\t"  << CR_stat_up << endl;
	      outMt2aEff << "CRSTAT_DN " << "\t"  << CR_stat_dn << endl;
	    }
	  }
	  else{
	    sReg = "SRmT2b"; 
	    //
	    //Dump outout Eff  to file
	    //
	    outMt2bEff << DG2LSystNames[isys] << "\t" << eff_mt2 << endl;
	    if(isys==DGSys_NOM){
	      float CR_stat_up = eff_mt2 + eff_err_mt2;
	      float CR_stat_dn = eff_mt2 - eff_err_mt2;
	      outMt2bEff << "CRSTAT_UP " << "\t"  << CR_stat_up << endl;
	      outMt2bEff << "CRSTAT_DN " << "\t"  << CR_stat_dn << endl;
	    }
	  }

	}
	
	//Get ZX SF in SR (cross check - should be the same as in CR's
	//Double_t _SF, _SF_err;
	//get_ZX_SF_SR(il,ireg,isys, _ZX_pred,_ZX_pred_err,_SF, _SF_err);
	
	
	//Print results
	if(verbose) {
	  cout << "\t ZX " << sReg
	       << "\t "   << _ZX_pred 
	       << " +/- " << _ZX_pred_err << " (stat.)"
	       << endl;
	  cout << "\t SF ZX " << sReg
	       << "\t "   << _SF_SR 
	       << " +/- " << _SF_SR_err << " (stat.)"
	       << endl;
	}
	
	//
	//Dump output SF to file
	//
	outSF << DG2LSystNames[isys] << "\t" << _SF_SR << endl;
	if(isys==DGSys_NOM){
	  float CR_stat_up = _SF_SR + _SF_SR_err;
	  float CR_stat_dn = _SF_SR - _SF_SR_err;
	  outSF << "CRSTAT_UP " << "\t"  << CR_stat_up << endl;
	  outSF << "CRSTAT_DN " << "\t"  << CR_stat_dn << endl;
	}


	//
	//Dump output TF to file
	//
	outTF << DG2LSystNames[isys] << "\t" << _TF << endl;
	if(isys==DGSys_NOM){
	  float CR_stat_up = _TF + _TF_err;
	  float CR_stat_dn = _TF - _TF_err;
	  outTF << "CRSTAT_UP " << "\t"  << CR_stat_up << endl;
	  outTF << "CRSTAT_DN " << "\t"  << CR_stat_dn << endl;
	}

	//
	//Dump ZX EST to file
	//
	outEST << DG2LSystNames[isys] << "\t" << _ZX_pred << endl;
	if(isys==DGSys_NOM){
	  float CR_stat_up = _ZX_pred + _ZX_pred_err;
	  float CR_stat_dn = _ZX_pred - _ZX_pred_err;
	  outEST << "CRSTAT_UP " << "\t"  << CR_stat_up << endl;
	  outEST << "CRSTAT_DN " << "\t"  << CR_stat_dn << endl;
	}
	
	//
	//Dump ZX SR mcto file
	//
	outZXSR << DG2LSystNames[isys] << "\t" << ZX_SR_mc << endl;
	if(isys==DGSys_NOM){
	  float CR_stat_up = ZX_SR_mc + ZX_SR_mc_err;
	  float CR_stat_dn = ZX_SR_mc - ZX_SR_mc_err;
	  outZXSR << "CRSTAT_UP " << "\t"  << CR_stat_up << endl;
	  outZXSR << "CRSTAT_DN " << "\t"  << CR_stat_dn << endl;
	}
	


      }//sys

      //
      //Get the extra sys for data/MC disagreement in CR's
      //
      Double_t BKG_sysUp, BKG_sysDn;
      Double_t ZXmc_BKG_UP, ZXmc_BKG_DN;
      Double_t SF_BKG_UP, SF_BKG_DN;
      get_ZX_BkgErr(il,ireg, 
		    BKG_sysUp, BKG_sysDn,
		    ZXmc_BKG_UP, ZXmc_BKG_DN,
		    SF_BKG_UP, SF_BKG_DN);

      outSF << "BKGMETHOD_UP " << "\t"  << SF_BKG_UP << endl;
      outSF << "BKGMETHOD_DN " << "\t"  << SF_BKG_DN << endl;

      outTF << "BKGMETHOD_UP " << "\t"  << _TF * (1 + BKG_sysUp) << endl;
      outTF << "BKGMETHOD_DN " << "\t"  << _TF * (1 - BKG_sysDn) << endl;

      outEST << "BKGMETHOD_UP " << "\t"  << _ZX_pred * (1 + BKG_sysDn) << endl; //swap since not ratio
      outEST << "BKGMETHOD_DN " << "\t"  << _ZX_pred * (1 - BKG_sysUp) << endl;

      outZXSR << "BKGMETHOD_UP " << "\t"  << ZX_SR_mc * (1 + BKG_sysDn) << endl; //swap since not ratio
      outZXSR << "BKGMETHOD_DN " << "\t"  << ZX_SR_mc * (1 - BKG_sysUp) << endl;


      txt.close();
      txt2.close();
      txt3.close();
      if(ZXCR[ireg]=="ZXCR4"){
	txt4.close();
	txt5.close();
      }
      txt6.close();

      if(ZXCR[ireg]=="ZXCR4") iMT2++;
    }//Region

    if(verbose) cout << endl;
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
  if(verbose) {
    cout << std::setprecision(5) 
	 << ">>> ZX estimate in " << ZXCR[ireg] << "\n"
	 << "\t Data \t\t" << _h_data->Integral(0,-1)
	 << "\t oBkg " << _h_oBkg->Integral(0,-1)
	 << "\t ZX_mc " << _h_ZX_mc->Integral(0,-1)
	 << "\t ZX_data " << est  << " +/- " << err
	 << "\t ZX_CR SF " << SF << " +/- " << SFerr
	 << endl;
  }
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
//
// SF of Z+X background in CR's 
//
void get_ZXSR_mc(int il, int ireg, int isys,
		   Double_t &est, Double_t &err)
{
  est =0;
  err =0;
  
  //SR Histos
  string hNameSR= "DG2L_" + ZXSR[ireg] + "_" + LEP[il] + "_DG2L_pred"; 
  _ana->grabHisto(hNameSR,true);

  //ZX MC in CR's
  TH1F* _h_ZX_mc      = (TH1F*)  _ana->getMcHisto(ZX,isys)->Clone(); 
  est = _h_ZX_mc->IntegralAndError(0,-1,err);

  //Print
  if(verbose) {
    cout << std::setprecision(5) 
	 << ">>> ZX " << ZXSR[ireg] << "\n"
	 << "\t ZX_mc " << est  << " +/- " << err
	 << endl;
  }
  _h_ZX_mc->Delete();
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
  
  if(verbose) {
    cout << std::setprecision(5) 
	 << "\t ZX CR \t\t"  << _hCR->Integral(0,-1)
	 << "\t ZX SR "  << _hSR->Integral(0,-1)
	 << " TF "       << est 
	 << " +/- "      << err
	 << endl;
  }

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
  if(verbose){
    cout << std::setprecision(5) 
	 << "\t " << sreg 
	 << "\t Eff \t" << est 
	 << " +/- " << err << " (stat.)"
	 << endl;
  }
  
  _h_ZX_SR->Delete();
  _h_ZX_preSR->Delete();
  _hRatio->Delete();
  
}



//_____________________________________________________________________________//
//
// Compute additional error needed to cover for data/MC disagreement in CR.
//
void  get_ZX_BkgErr(int ilep, int ireg,
		    Double_t &bkgSysUp, Double_t &bkgSysDn, 
		    Double_t &ZX_mc_bkgUp, Double_t &ZX_mc_bkgDn,
		    Double_t &SF_up, Double_t &SF_dn){

  
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
  //Compute extra sys needed
  //
  Double_t sigma_b_up = 0;
  Double_t sigma_b_dn = 0;

  //Taking the full difference as sys (i think that's an overestimate)
  Double_t _sys = fabs(_nom - ZX_data)/_nom;

  //Taking only the diff not already covered by the sys
  if(_nom -ZX_data >0)  _sys = fabs( _nom - _sys_dn - ZX_data)/(_nom-_sys_dn);
  else                  _sys = fabs( _nom + _sys_up - ZX_data)/(_nom+_sys_up);

  if(_nom> ZX_data)  sigma_b_up = _sys;  //MC overpredict 
  else               sigma_b_dn = _sys;

  //
  // Get corresponding SF
  //
  SF_up = ZX_data /_nom * (1 + sigma_b_up);
  SF_dn = ZX_data /_nom * (1 - sigma_b_dn);
  bkgSysUp = sigma_b_up;
  bkgSysDn = sigma_b_dn;
  ZX_mc_bkgUp = _nom * (1 + sigma_b_dn); //swap since not ratio
  ZX_mc_bkgDn = _nom * (1 - sigma_b_up);
  
  if(verbose) {
    cout << " Additional error needed for ZX_mc \n" 
	 << " ZX_mc "     <<  _nom << " +" << _sys_up << " - " << _sys_dn << "\n" 
	 << " ZX_data "   << ZX_data << "\n"
	 << " Extra sys + " << bkgSysUp << " - " << bkgSysDn << "\n"
	 << " SF " << ZX_data /_nom << " + " <<  SF_up << " - " << SF_dn << "\n"
	 << " ZX_mc " << _nom << " + " <<  ZX_mc_bkgUp << " - " << ZX_mc_bkgDn 
	 << endl;
  }

  _h_data->Delete();
  _h_fake->Delete();
  _h_top->Delete();
  _h_WW->Delete();
  _h_Ztt->Delete();
  _h_oBkg->Delete();
  _h_ZX_data->Delete();


}

//_____________________________________________________________________________//
//
// Make plot for ZX section
//
void  make_ZXPlots(int ilep, int ireg){
  
  string name;

  //
  // Use Ztt place to put diB,ok since Ztt is 0
  //
  _ana->SFILE[1]="ZZ,WZ";
  _ana->SFILE[4]="Z(ee,#mu#mu)+jets";

  openHist("DD",
	   "histo_topDil_Sherpa",
	   "histo_WW_Sherpa",
	   "histo_Zjets_Sherpa",		      
	   "histo_diBZX_Sherpa",
	   "histo_data12_fake");
    
  bool logy=false;

  if(ireg==1){       // ZXCR1
    if(ilep==0) name = "DG2L_ZXCR1_EE_DG2L_metrel";
    if(ilep==1) name = "DG2L_ZXCR1_MM_DG2L_metrel";
  }
  else if(ireg==3){  // ZXCR3
    if(ilep==0) name = "DG2L_ZXCR3_EE_DG2L_metrel";
    if(ilep==1) name = "DG2L_ZXCR3_MM_DG2L_metrel";
  }
  else if(ireg==4){  // ZXCR4
    logy=true;
    if(ilep==0) name = "DG2L_ZXCR4_EE_DG2L_metrel";
    if(ilep==1) name = "DG2L_ZXCR4_MM_DG2L_metrel";
  }
  else if(ireg==6){  // ZXCR6
   if(ilep==0) name = "DG2L_ZXCR6_EE_DG2L_metrel";
   if(ilep==1) name = "DG2L_ZXCR6_MM_DG2L_metrel";
  }
  else if(ireg==7){  // ZXCR7
   if(ilep==0) name = "DG2L_ZXCR7_EE_DG2L_metrel";
   if(ilep==1) name = "DG2L_ZXCR7_MM_DG2L_metrel";
  }
  else if(ireg==5){  // ZXCR5
   if(ilep==0) name = "DG2L_ZXCR5_EE_DG2L_mt2";
   if(ilep==1) name = "DG2L_ZXCR5_MM_DG2L_mt2";
  }
  
  _ana->drawPlotErrBand(name,logy,false);
}
