#ifndef DRAWPLOTS_H
#define DRAWPLOTS_H

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
#include <stdlib.h>

#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "THStack.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"

#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/Histos_Common.h"

typedef unsigned uint;
using namespace std;

#define DATA8TEV

const bool HIDEDATA=false;//true;
const bool USESF=true;


enum MC {HIGGS=0, FAKE=1, ZV=2, Zjets=3,  TOP=4, WW=5, OTHER=6};

const std::string MCNames[] ={
  "Higgs",
  "FAKE",
  "ZV",
  "Zjets",
  "TOP",
  "WW"
};

/*My color scheme */
/*
enum MCCOL { C_TOP=kAzure-3, C_DIB=kYellow-7, C_WJETS = kMagenta+4, 
	     C_ZJETS=kOrange+8, C_BB=kRed+2, C_QCD=kGreen+3};
*/
/*DG2L color scheme */
enum MCCOL { C_HIGGS=kYellow-9,
	     C_FAKE=kGray, 
	     C_ZV=kSpring+1, 
	     C_Zjets=kOrange-2,
	     C_TOP=kRed+1, 
	     C_WW=kAzure+4, 
	     C_SIG1=kMagenta-7, C_SIG2=kRed-2};

const int iMarker[9]={20,21,22,23,24,25,26,27,30};

enum SIG {Slepton_255_80, modeCSlep_375_125};


class DrawPlots { 

 public:
  DrawPlots();
  virtual ~DrawPlots(){};

  void openHistoFiles(string mode="STD",
		      string Top="histo_topDil_Sherpa",
		      string WW="histo_WW_Sherpa",
		      string Zjets="histo_Zjets_SherpaAlpgenPythia",		      
		      string ZV="histo_WZ_ZZ_Sherpa",
		      string Fake="histo_mcFake_Sherpa",
		      string Higgs="histo_Higgs"
		      );

  TObject* getHisto(string sample, string name, bool moveUnderOver=false);
  TObject* retreiveHisto(int iSample, string name, bool moveUnderOver=false);


  /* To compare shape - histo normalysed to 1 */
  void compareShape(string name, bool logy);

  /* To view bkg prediction & data */
  void drawPlot(string name, bool logy);
  /* To view bkg prediction & data with error bands */
  void drawPlotErrBand(string name, bool logy=true, bool wSig=true, bool sysBand=true);
  void dumpBinContent(TH1* _dataH, TH1* _mcH, TGraphAsymmErrors* _sysBand);


  TGraphAsymmErrors* getSysErrorBand(TH1F* _hist, bool sysBand=true);
  
  void getFakeSys(vector<TH1F*> &sys);
  /* Toggle this to true to use a Mc sample instead of data driven fake 
     Default is false
  */
  void fudgeFake(bool fudge=false){
    disableFake=fudge;
    std::cout << "Fake will be disable " << fudge << endl;
  };

  /*  Fake composition */
  void  getFakeComposition(string sAna="DG2L", string sSR="SR0_OS", string lep="E");
  TH1F* getFakeRate(string sample, string sel, string cr, string type, string lep, string var);


  /* Dump the cutflow to txt file */
  void dumpCutflow_DG2L(string sample);
  void dumpCutflow_ML(string sample);

  /* Dump bkg estimate and data */
  void bkgEstimate_DG2L();
  void bkgEstimate_ML();


  /* Drawing function */
  void  drawChannelText(string name, float x=0.2,float y=0.73, bool desc=true);
  void  drawLumi(float x=0.18, float y=0.75);
  void  drawATLAS(float x=0.18, float y=0.85);

  /* Math func */
  void binomialError(float Num, float Den, 
		     float& Eff, float& EffErr);
  TH1F* calcEff(TH1F* h, int opt=0); //opt=1: 1-Eff
  TGraphErrors* TGraph2Hist(TH1F* hx, TH1F* hy); //hx,hy hs same number of bins

  TH1F* calcRatio(TH1F* hnum, TH1F* hden, string name="ratio", string opt="B");

  string itos(int i);

  /* Retrun TH1 histo from TH3 */
  TH1F* get1DHisto(TH3* _h, string axis="x", int bin1a=0, int bin2a=-1, int bin1b=0, int bin2b=-1);
  
  /* Make Profile give axes to profile onto, the one to intergral and the one to loop over */
  TH1F* getProfile3D(TH3* _h, string axis="x", string afix="y", string aloop="z");

  void grabHisto(string name, bool quiet=true, bool sysHistos=true);

  TH1F* getMcHisto(int imc, int isys){return _mcH1[imc][isys];}
  TH1F* getDataHisto() {return _dataH1;}

  string getFileName(string DSId);
  TFile* openFile(string DSId);

  std::vector<TH1F*> loadHisto(TFile* file, string DSId, 
			       string name, bool verbose=false);
  
  void clearHistVect(std::vector<TH1F*> _histVec);

  void   getYield(std::vector<TH1F*> histV, 
		  Double_t &nom,
		  Double_t &stat_err,
		  Double_t &sysUp, Double_t &sysDn,
		  bool verbose=false);
  

   void   getYieldBkgAll(std::vector<TH1F*> histFakeV, 
			 std::vector<TH1F*> histZttV, 
			 std::vector<TH1F*> histWWV, 
			 std::vector<TH1F*> histTopV, 
			 std::vector<TH1F*> histZXV, 
			 std::vector<TH1F*> histHiggs, 
			 Double_t &nom,
			 Double_t &stat_err,
			 Double_t &sysUp, Double_t &sysDn,
			 bool verbose=false);
   

  std::vector<TH1F*> sumChannels(std::vector<TH1F*> _histEE,
				 std::vector<TH1F*> _histMM,
				 std::vector<TH1F*> _histEM);

  float getBkgSF(string name, int BkgType);
  void  blindMT2(string name, TH1F* h);

  void  setGenSys(string name, int BkgType, TH1F* hNom, TH1F* h);

  void  setStackOrder(string name);
  
  
  string       _pathHisto;
  string       _pathPlots;
  string       _pathTables;

  string         _dataFileName;
  TFile*         _dataFile;
  vector<string> SFILE;
  vector<string> SIGFILE;
  vector<TFile*> _mcFile;
  vector<string> _mcFileName;
  vector<TFile*> _sigFile;
  vector<string> _sigFileName;

  vector<int>    _stackOrder;

  TH1F*             _dataH1;
  THStack*          _mcStack;
  TH1F*             _mcStackH;
  vector< vector<TH1F*> >   _mcH1;
  vector<Color_t> _mcColor;

  ClassDef(DrawPlots,1);

 private:


  TGuiUtils*   _utils;

  bool _logy;
  bool _moveUO;

  
 
  vector<int>     _mcMarker;

  vector<Color_t> _sigColor;
  vector<string>  _sigName;
  vector<TH1F*>   _sigH1;

  /* build histo stack */
  void buildStack(string name, TLegend* _l);
  void blindDataSR();
  void setMoveUnderOver(bool b){_moveUO=b;}
  void setLogy(bool b) {_logy=b;}

  bool disableFake;

};
#endif
