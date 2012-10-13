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

#include "SusyNtuple/TGuiUtils.h"


typedef unsigned uint;
using namespace std;

#define DATA8TEV

const bool HIDEDATA=false;//true;

enum MC { FAKE=0, Ztt=1, WW=2, TOP=3, ZX=4,  OTHER=6};
enum SIG {modeANoSlep_15, modeANoSlep_18};
/*My color scheme */
/*
enum MCCOL { C_TOP=kAzure-3, C_DIB=kYellow-7, C_WJETS = kMagenta+4, 
	     C_ZJETS=kOrange+8, C_BB=kRed+2, C_QCD=kGreen+3};
*/
/*DG2L color scheme */
enum MCCOL { C_FAKE=kGray, 
	     C_Ztt=kSpring+1, 
	     C_WW=kAzure+4, 
	     C_TOP=kRed+1, 
	     C_ZX=kOrange-2,
	     C_SIG1=kMagenta-7, C_SIG2=kRed-2};
const int iMarker[9]={20,21,22,23,24,25,26,27,30};

//const char* const SIGFILE[] = {"mAwSl", "mCwSl"};


class DrawPlots {

 public:
  DrawPlots();
  virtual ~DrawPlots(){};

  void openHistoFiles(string mode="STD",
		      string Top="histo_topDil_Sherpa",
		      string WW="histo_WW_Sherpa",
		      string ZX="histo_ZX_Sherpa",		      
		      string Ztt="histo_ZTauTaujets_SherpaLFHF",
		      string Fake="histo_mcFake_Sherpa"
		      );

  TObject* getHisto(string sample, string name, bool moveUnderOver=false);


  /* To compare shape - histo normalysed to 1 */
  void compareShape(string name, bool logy);

  /* To view bkg prediction & data */
  void drawPlot(string name, bool logy);
  
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
  void  drawChannelText(string name, float x=0.2,float y=0.73);
  void  drawLumi(float x=0.18, float y=0.80);
  void  drawATLAS(float x=0.18, float y=0.88);

  /* Math func */
  void binomialError(float Num, float Den, 
		     float& Eff, float& EffErr);
  TH1F* calcEff(TH1F* h, int opt=0); //opt=1: 1-Eff
  TGraphErrors* TGraph2Hist(TH1F* hx, TH1F* hy); //hx,hy hs same number of bins

  TH1F* calcRatio(TH1F* hnum, TH1F* hden, string name="ratio");

  string itos(int i);

  /* Retrun TH1 histo from TH3 */
  TH1F* get1DHisto(TH3* _h, string axis="x", int bin1a=0, int bin2a=-1, int bin1b=0, int bin2b=-1);
  
  /* Make Profile give axes to profile onto, the one to intergral and the one to loop over */
  TH1F* getProfile3D(TH3* _h, string axis="x", string afix="y", string aloop="z");
  
  ClassDef(DrawPlots,1);

 private:

  string       _pathHisto;
  string       _pathPlots;
  string       _pathTables;
  TGuiUtils*   _utils;

  bool _logy;
  bool _moveUO;
  
  vector<string> SFILE;
  vector<string> SIGFILE;


  vector<TFile*> _mcFile;
  vector<string> _mcFileName;
  string         _dataFileName;
  TFile*         _dataFile;

  vector<TFile*> _sigFile;
  vector<string> _sigFileName;
  vector<Color_t> _sigColor;
  vector<string>  _sigName;
  vector<TH1F*>   _sigH1;

  TLegend*       _leg;
  TH1F*          _dataH1;
  THStack*       _mcStack;
  TH1F*          _mcStackH;
  vector<Color_t> _mcColor;
  vector<int>     _mcMarker;
  vector<string>  _mcName;
  vector<TH1F*>   _mcH1;


  /* build histo stack */
  void buildStack(string name);
  void grabHisto(string name, bool quiet=true);
  void setMoveUnderOver(bool b){_moveUO=b;}
  void setLogy(bool b) {_logy=b;}



};
#endif
