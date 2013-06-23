#ifndef SROptimization_h
#define SROptimization_h

#include <iomanip>

#include "SignificanceCalculator/SignificanceCalculator.h"
#include "SusyXSReader/XSReader.h"
#include "SusyNtuple/TGuiUtils.h"

#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TGraph2D.h"
#include "TMath.h"
#include "TLatex.h"

static const float SYS_ERR = 0.15;//30% systematic on Bkg

//------------------------------------//
// Enum for process
//------------------------------------//
enum SusyProcess
{
  SP_slepton = 0,
  SP_modeC,
  SP_modeA_slep,
  SP_modeA_noslep_hadW,
  SP_N
};
const string SPNames[] = 
  {
    "slepton", 
    "modeC", 
    "wA_slep", 
    "wA_noslep_hadW"
  };

//------------------------------------//
// Enum for runtime options
//------------------------------------//
enum RegionOption
{
  RO_yield = 0,
  R1_yield,
  R2_yield,
  R3_yield,
  R4_yield,
  R5_yield,
  R6_yield,
  R7_yield,
  RO_N
};

const string RegionNames[] = 
  {
    "Base",
    "Sarah",
    "Andreas",
    "AnyesSRZjetsOpt1",
    "AnyesSRZjetsOpt2",
    "AnyesSRZjetsOpt3",
    "AnyesSRZjetsOpt4",
    "AnyesSRSS_Opt1",
  };

//------------------------------------//
// Enum for SM bkg
//------------------------------------//
enum MC 
{
  HIGGS=0, 
  FAKE=1, 
  ZV=2, 
  Zjets=3,  
  TOP=4, 
  WW=5, 
  ALLBKG=6,
  N_MC=7
};

const std::string MCNames[] =
  {
    "Higgs",
    "Fake leptons",
    "ZV",
    "Z+jets",
    "Top",
    "WW",
    "SM Bkg"
  };

enum LEP
  {
    EE=0,
    MM,
    EM,
    ALL,
    N_LEP
  };

const std::string LepN[] =
  {
    "EE",
    "MM",
    "EM",
    "ALL"
  };
const std::string LepNames[] =
  {
    "$ee$",
    "$\\mu\\mu$",
    "$e\\mu$",
    "Total"
  };


//------------------------------------//
// File object 
// slepton or chargino
//------------------------------------//
class sl_File : public TObject
{

 public:
  sl_File(int id, float m_susy, float m_neutralino, 
	  string f_name, float xs=0, float xsSys=0){
    dsid = id;
    m_spart = m_susy; //slepton, Chargino etc...
    m_neut = m_neutralino;
    fname  = f_name;
  };
  ~sl_File(){};

  int    getId(){ return dsid; };
  float  getSusyMass(){ return m_spart; };
  float  getNeutralinoMass(){ return m_neut; };
  string getFName(){ return fname; };
  float  getXS(){ return m_xs; };
  float  getXSsys(){ return m_xsSys; };

 private:
  int dsid;
  float m_spart;
  float m_neut;
  string fname;
  float m_xs;
  float m_xsSys;
  

};



//------------------------------------//
// Class to make plots
//------------------------------------//
class SROptimization
{

 public:
  
  SROptimization(RegionOption option, SusyProcess sp, string skim, string dilType="ALL");
  virtual ~SROptimization();

  // Set debug
  void setDebug(int d){ m_dbg = d; };
  void init();

 // Plot Significance for one cut value
  void PlotSig(); 
  
  // Plot max significance for given variable
  void PlotMaxSig();

  
  // Dump background/Signal prediction in steps
  void dumpBkgTable();
  void dumpSignalTable();

  // Combine in quad the Zn plots in ZnList.txt
  void combineZn();

 private: 
  
  string itos(int i);

  // Load all the file names and file information
  void loadSleptonFiles();
  void loadModeCFiles(bool wSlepton=false);
  void loadModeAFiles(bool wSlepton=false);

  void loadBkgHistos();

  // Get Significance for particular cut
  TGraph2D* buildSigCut(vector<sl_File> files, 
			float err,
			TH1F* bkg, 
			string name,
			float cut=-1, float upper=-1
			);
  // Get Numbers per grid points
  TH2D* buildNumbers(vector<sl_File> files,
		     string name,
		     int nbinsx, float minx, float maxx,
		     int nbinsy, float miny, float maxy,
		     float cut=-1, float upper=-1);


  //Get stat error integrating over bins
  double getStatError(TH1* hist, 
		      int lowBin=-1, int highBin = -1);

  //Combine Zn in quad
  TGraph2D* ZnCombo(vector<TGraph2D*> _ZnGraph);


  // Make Contour
  TGraph* getContourGraph(TGraph2D* signif);

  // Set TGraph Attributes
  void setAttributes(TGraph2D* graph, string xtitle, string ytitle, string ztitle,
		     TCanvas* &canvas);

  // Add lumi and stuff
  void addDecorations(TCanvas* &can, int metRel = -999);
  void  drawLumi(float x=0.18, float y=0.75);


  int m_dbg;
  TGuiUtils*   _utils;
  SignificanceCalculator* m_sigCalc;

  string m_dirInput;
  string m_dirHist;
  vector<sl_File> m_susyFiles;
  TFile* m_bkgFile;

  string m_var;         //variable use to do significance
  string m_selection;   //selection cuts  
  string m_lep;         //lepton type
  string m_histName;    //histogram name

  SusyProcess m_procId; //Susy Grid
  string m_process;
  string m_skim;

  string m_xtitle;     //Axis labels - depends on susy grid
  string m_ytitle;

  vector<string> _mcHistFileName; //Bkg histo filename  
  vector<TH1F*>  _mcH1[N_LEP];    //Bkg histograms



};


#endif
