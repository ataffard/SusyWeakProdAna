#include <stdlib.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

#include "TROOT.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TLine.h"
#include "TChain.h"
#include "TCut.h"
#include "THStack.h"
#include "TEventList.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TGraph2D.h"
#include "RooStats/NumberCountingUtils.h"

#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/XSObject.h"

//_____________________________________________________________________________//
static const string ver       = "histos_070614_21fb_n0150_DD_LFV_v1/";  
static const string SR        = "_LFV_base"; //Skimmed ToyNt suffix - inclusive OS

static const int dbg = 1;

//Options for optimisation
static const int    selDil    = 0;  //EE=0, MM=1, EM+ME=2 
static const int    selCuts   = 0;  //0: SR

//C1C1 grids
static const int    sigSampleId    = 144917; //Id of signal sample to use for optimisation
static const float  scaleSignal    = 1;      //Set to one if want no scaling
static const string SignalSampleDB = "modeC_lightslep_MC1eqMN2_DiagonalMatrix.txt";
static const int    nSignalPts     = 60; //to be update to fit file

//Settings for optimisation/plots
static const bool   weightEvt          = true;   //Weight bkg/signal events
static const bool   showData           = false;  //true; // shows Zn below plot

static const bool   skipDetailBkg      = false;  //false: shows each bkg group;
static const bool   showCutflow        = true;   //dump cutflow yield
static const bool   showCutflowDetail  = true;   //dump yield last cut
static const bool   showCutflowDetail2 = false;  //dump cutflow each cut
static const bool   logPlot            = false;  //true;   

static const float SYS_ERR = 0.3;                //30% systematic on Bkg  - Note: Zn add stat err on tot bkg

//_____________________________________________________________________________//

const float MZ = 91.2;

enum MC 
{
  HIGGS=0, 
  FAKE=1, 
  ZV=2, 
  Zjets=3,  
  TOP=4, 
  WW=5, 
  //  ALLBKG=6,
  N_MC=6//7
};

const std::string MCLabel[] ={
  "Higgs",
  "FAKE",
  "ZV",
  "Zjets",
  "TOP",
  "WW",
  //  "ALL"
};

const std::string MCNames[] ={
  "Higgs",
  "Fake leptons",
  "ZV",
  "Z+jets",
  "Top",
  "WW",
  //  "SM Bkg"
};

enum LEP {
  EE=0,
  MM,
  EM,
  ME,
  ALL,
  N_LEP
};

const std::string LepN[] = {
  "EE",
  "MM",
  "EM",
  "ME",
  "ALL"
};
const std::string LepNames[] = {
  "$ee$",
  "$\\mu\\mu$",
  "$e\\mu$",
  "$\\mu e$",
  "Total"
};

enum MCCOL { C_HIGGS=kYellow-9,
	     C_FAKE=kGray, 
	     C_ZV=kSpring+1, 
	     C_Zjets=kOrange-2,
	     C_TOP=kRed+1, 
	     C_WW=kAzure+4, 
	     C_ALL=kBlack,
	     C_SIG1=kMagenta-7, C_SIG2=kRed-2
};

//_____________________________________________________________________________//
TGuiUtils* _utils;
TDirectory* _histoDir;

TChain* ntBkg[N_MC];
TChain* ntData;
vector<TChain*> _vNtSig;

vector<int>    _col;
vector<string> _type;
vector<string> _var;

vector<string>             _bkgFileNames;
vector<TH1F*>              _hBkg[N_MC];
vector<TH1F*>              _hSig;
vector<TH1F*>              _hData;
vector<TCut>               _vCut;
vector<TH1F*>              _hSignificanceUp;
vector<TH1F*>              _hSignificanceDn;
vector<THStack*>           _hBkgStack;
vector<TGraphAsymmErrors*> _ratioTG;
vector<TH1F*>              _ratioH;
TH1F*                      _hBkgTotal;

vector<TEventList*> _bkgEvtList;
vector<TEventList*> _sigEvtList;
TEventList*         _dataEvtList;

typedef std::map<int,XSObject>  xsObj_map;//map DSID with signal mass and cross section
xsObj_map xsmap;                          //[DSID][XSObject] map is sorted by DSID

void OS2Loptimisation();

/* Analyses function - run from OS2LOptimisation() */
void runOptimisation(bool useOneSignal=true);          //Run Optimisation
void signalKin(int iSigId=sigSampleId);                //Make basic plots of the signal kinematics.

/*Standalone functions */
void compareSignal(int iSig1, int iSig2, 
		   int iSig3, bool normalize=true);    //Compare shape 3 signal points

void signalYield();                                    //Print signal yield for all the signal samples
void dumpCutflow(int mcId=0, string filename="");      //Dump cutflow for a given sample. 
                                                       //either give mcId=110001 or filename toyNt_top_MCNLO
void make_ZnPlots();                                   //Make Zn plots and combined Zn based on a selection


/* Base functions */
void init();
//load Bkg, data and signals 
void loadSamples();                                     
//Set TCut to apply
TCut setSelection(int isel, int dilType=3, bool verbose=true); 
//Generate event lists using TCut configured
void selectEvent(TCut _cut,bool useOneSignal=true);    
//Apply 1 cut at a time and show cutflow for bkg/signal
//return yield at last cut
void cutFlow(TChain* nt, Double_t &tot, Double_t &statErr, bool detail=true);            


/* Functions for plotting */
void     bookHist();
TH1F*    myBook(string name, int nbin, float  xlow, float  xhigh, string xaxis, string yaxis);
TH1F*    histList(int ivar, string name);
void     fillHist();                           //fill all histos using event List & save plots to file
void     plotHist(bool logy=false);            //compare signal and bkg
TH1F*    ZnHistos(TH1* _hBkg, TH1* _hSig, bool upper=true);
THStack* buildStack(TLegend* _l, int ivar);

void     saveHist(string fileName);
void     resetHist();

/* Utilities */
string   itos(int i);

int      getSignalSampleIdx(int dsid);
//Get the DB info - DSID -vs- masses/Xs. fills
void     getSignalSamples(string file=SignalSampleDB);

/* Selections for various ana */
TCut sel_SR(int dilType);


//_____________________________________________________________________________//
//_____________________________________________________________________________//
//int main(int argc, char *argv[]){
void OS2Loptimisation(){

  init();
  loadSamples(); 
  bookHist();
  
  runOptimisation();
   
  //signalKin(sigSampleId); resetHist();
}

//_____________________________________________________________________________//
void runOptimisation(bool useOneSignal)
{
 
  Double_t yield=0;
  Double_t statErr=0;
  Double_t totYield=0;
  Double_t totYieldStatErr=0;

  TCut SEL = setSelection(selCuts,selDil);
  if(showCutflow){
    cutFlow(_vNtSig[getSignalSampleIdx(sigSampleId)],yield,statErr);
    cout << "---------------------------------------------" << endl;
    cout << "---------------------------------------------" << endl;
    if(!skipDetailBkg && showCutflowDetail){
      cutFlow(ntBkg[FAKE],yield,statErr);  totYield+=yield; totYieldStatErr+= pow(statErr,2);
      cutFlow(ntBkg[ZV],yield,statErr);    totYield+=yield; totYieldStatErr+= pow(statErr,2);
      cutFlow(ntBkg[WW],yield,statErr);    totYield+=yield; totYieldStatErr+= pow(statErr,2);
      cutFlow(ntBkg[TOP],yield,statErr);   totYield+=yield; totYieldStatErr+= pow(statErr,2);
      cutFlow(ntBkg[Zjets],yield,statErr); totYield+=yield; totYieldStatErr+= pow(statErr,2);
      cutFlow(ntBkg[HIGGS],yield,statErr); totYield+=yield; totYieldStatErr+= pow(statErr,2);
      cout << "---------------------------------------------" << endl;
      totYieldStatErr=sqrt(totYieldStatErr);
      cout << "Total Bkg \t\t\t"<<  totYield << " +/- " << totYieldStatErr << endl;
      cout << "---------------------------------------------" << endl;
      cout << "---------------------------------------------" << endl;
      if(showData){
	cutFlow(ntData,yield,statErr);
	cout << "---------------------------------------------" << endl;
	cout << "---------------------------------------------" << endl;
      }

    }
  }
  selectEvent(SEL,useOneSignal);

  fillHist();
  plotHist(logPlot);

}


//_____________________________________________________________________________//
//_____________________________________________________________________________//
//_____________________________________________________________________________//
void init()
{
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");

  _histoDir = new TDirectory("Ana","Ana");
  _histoDir->cd();

}
//_____________________________________________________________________________//
string itos(int i){
  string s;
  std::stringstream out;
  out << i;
  s = out.str();
  return s;
}

//_____________________________________________________________________________//
void loadSamples()
{
  _type.push_back("sig_");
  _type.push_back("bkg_");
  _type.push_back("data_");

  string dir =  string(getenv("HISTOANA")) + "/SusyAna/" +  ver ;
  string subDir = "ToyNtOutputs/";

  if(dbg>0){
    cout << "Directory " << dir << endl;
    cout << "Loading ToyNt skim " << SR << endl << endl;
  } 

  //Load BKG
  _bkgFileNames.clear();
  _bkgFileNames.push_back(string("toyNt_Higgs" + SR + "_rlep.root").c_str());
  //_bkgFileNames.push_back(string("toyNt_dataFake" + SR + "_flep.root").c_str()); //FAKE
  _bkgFileNames.push_back(string("toyNt_dummy" + SR + "_rlep.root").c_str()); //DUMMY
  _bkgFileNames.push_back(string("toyNt_WZ_ZZ_Sherpa" + SR + "_rlep.root").c_str());
  _bkgFileNames.push_back(string("toyNt_Zjets_AlpgenPythia" + SR + "_rlep.root").c_str());
  _bkgFileNames.push_back(string("toyNt_top_MCNLO" + SR + "_rlep.root").c_str());
  _bkgFileNames.push_back(string("toyNt_WW_Sherpa" + SR + "_rlep.root").c_str());
 
  //Create the TChain for BKG
  for(uint ibkg=0; ibkg<N_MC; ibkg++){
    ntBkg[ibkg] = new TChain("ToyNt",string("Bkg_"+MCLabel[ibkg]).c_str());
    ntBkg[ibkg]->Add( string(dir+_bkgFileNames[ibkg]).c_str() );
    //cout << "Adding file " << _bkgFileNames[ibkg] << endl;
    if(dbg>0) cout << MCLabel[ibkg ] << " nEntries " << ntBkg[ibkg]->GetEntries() << endl;
  }
  
  //Load and create TChain for Data
  ntData = new TChain("ToyNt",string("Data_").c_str());
  ntData->Add( string(dir+"toyNt_data12" + SR + "_std.root").c_str() );
  if(dbg>0)  cout << "Data nEntries " << ntData->GetEntries() << endl;
  
  //Load Signals
  _vNtSig.erase (_vNtSig.begin(),_vNtSig.end());
  _col.push_back(kRed-4);  //Signal  
  getSignalSamples();      //load the DB of signal samples
  
  for(std::map<int,XSObject>::const_iterator it=xsmap.begin(); it != xsmap.end(); ++it) {
    int iSig = it->first;
    string sigFileName = itos(iSig)  + SR + ".root"; 
    TChain* _nt = new TChain("ToyNt", string("Signal_"+itos(iSig)).c_str());
    _nt->Add( string(dir+subDir + sigFileName).c_str() );
    _vNtSig.push_back(_nt);
    if(dbg>0)  cout << "Signal " << iSig << " nEntries " 
		    << _nt->GetEntries() << " @ " << _vNtSig.size()-1 << endl;
    
  }

}


//_____________________________________________________________________________//
void selectEvent(TCut _cut, bool useOneSignal)
{
  cout << "Generating eventlists for cuts" << endl;
  _cut.Print();

  //Select the events and put them in some event lists for Bkg & Signal
  //Set the event list for plotting
  for (std::map<int,XSObject>::const_iterator it=xsmap.begin(); it != xsmap.end(); ++it) {
    int iSig = it->first;
    int iSigIdx = getSignalSampleIdx(iSig);
    if(useOneSignal && iSig != sigSampleId){
      _sigEvtList.push_back(NULL);
      continue;
    }
    if(_vNtSig[iSigIdx]->GetEntries()==0){ //protect against missing samples
      _sigEvtList.push_back(NULL);
      cout << "\tSignal sample " << iSig << " missing. Skipping" << endl;
      continue;
    }

    string cmd = "elist_Sig_" +  itos(iSig);
    _vNtSig[iSigIdx]->Draw(string(">>"+cmd).c_str(),_cut,"goff");
    _sigEvtList.push_back((TEventList*)gDirectory->Get(cmd.c_str()));
    if(dbg>0) cout << "\t Signal evtList for " << iSig
		   << " selected " <<_sigEvtList[iSigIdx]->GetN() <<endl;
    _vNtSig[iSigIdx]->SetEventList(_sigEvtList[iSigIdx]);
    
  }

  for(uint ibkg=0; ibkg<N_MC; ibkg++){
    //if(skipDetailBkg && ibkg<ALLBKG) continue; 
    string evtName = "elist_Bkg_" + MCLabel[ibkg];
    ntBkg[ibkg]->Draw(string(">>" + evtName).c_str(),_cut,"goff");
    TEventList *evtListBkg = (TEventList*)gDirectory->Get(evtName.c_str());
    _bkgEvtList.push_back(evtListBkg);
    ntBkg[ibkg]->SetEventList(evtListBkg);
    if(dbg>0 && evtListBkg) cout << "\t EvtList for " << evtName 
				 << " selected " << (_bkgEvtList.back())->GetN() << endl;
  }
  
  if(showData){
    ntData->Draw(">>elist_Data",_cut,"goff");
    _dataEvtList = (TEventList*)gDirectory->Get("elist_Data");
    ntData->SetEventList(_dataEvtList);
    if(dbg>0) cout << "\t Data selected " <<_dataEvtList->GetN() <<endl;
  }

}
//_____________________________________________________________________________//
TH1F* myBook(string name, int nbin, float xlow, float xhigh, string xaxis, string yaxis)
{
  return _utils->myTH1F(name.c_str(),name.c_str(),
			nbin,xlow, xhigh,
			xaxis.c_str(),yaxis.c_str());
}
//_____________________________________________________________________________//
void bookHist()
{
  cout << "Booking histograms " << endl;
  string name;
    
  //leptons
  _var.push_back("l_pt[0]");
  _var.push_back("l_pt[1]");
  _var.push_back("e_eta");
  _var.push_back("m_eta");

  //jets
  _var.push_back("nJets");
  _var.push_back("nCJets");
  _var.push_back("nBJets");
  _var.push_back("nFJets");
  _var.push_back("j_pt[0]");
  _var.push_back("j_pt[1]");
  _var.push_back("j_pt[2]");

  //met
  _var.push_back("metrel");
  _var.push_back("met");

  //dilepton
  _var.push_back("llType");
  _var.push_back("dR_ll");
  _var.push_back("dphi_ll");
  _var.push_back("deta_ll");
  _var.push_back("mll");
  _var.push_back("pTll");


  //lepton(s) - met
  _var.push_back("mTl[0]");
  _var.push_back("mTl[1]");
  _var.push_back("max_mTl");
  _var.push_back("min_mTl");
  _var.push_back("dphi_metl[0]");
  _var.push_back("dphi_metl[1]");
  _var.push_back("dPhiMetll");

  //event kinematics
  _var.push_back("HT");  //Sum lep T & met - overwrite due to bug when nSj=0!
  _var.push_back("mEff");
  _var.push_back("ST");
  _var.push_back("mll_collApprox");

  //mT2 - mCT
  _var.push_back("mT2");
  _var.push_back("mT2J");
  _var.push_back("mT2jj");
  _var.push_back("mct");
  _var.push_back("mctPerp");
  _var.push_back("mctPara");

  //Add razor
  

  cout << "Booking histo for " << _var.size() << " variables " << endl;

  uint iTypeMax = _type.size();
  if(!showData) iTypeMax--;
  for(uint itype=0; itype<iTypeMax; itype++){
    for(uint ivar=0; ivar<_var.size(); ivar++){
      TH1F* hh = NULL;
      //Signal or  data
      name = _type[itype] + _var[ivar];
      if(itype==0 || itype==2){
	hh = histList(ivar,name);
	if(itype==0)      _hSig.push_back(hh);
	else if(itype==2) _hData.push_back(hh);
      }
      else if(itype==1){
	for(uint ibkg=0; ibkg<N_MC; ibkg++){
	  name = _type[1] + MCLabel[ibkg] + "_"  + _var[ivar]; 
	  hh = histList(ivar,name);
	  _hBkg[ibkg].push_back(hh); 
	}
      }
    }
  }
  
}
//_____________________________________________________________________________//
TH1F* histList(int ivar, string name)
{
  string sY="Entries";
  TH1F* h = NULL;

  //leptons
  if(ivar==0) h = myBook(name.c_str(),40,0,200,"p_{T}^{l0} [GeV]",sY.c_str());
  if(ivar==1) h = myBook(name.c_str(),40,0,200,"p_{T}^{l1} [GeV]",sY.c_str());
  if(ivar==2) h = myBook(name.c_str(),25,-2.5,2.5,"#eta^{e}",sY.c_str());
  if(ivar==3) h = myBook(name.c_str(),25,-2.5,2.5,"#eta^{#mu}",sY.c_str());

  //jets
  if(ivar==4) h = myBook(name.c_str(),8,-0.5,7.5,"Njets",sY.c_str());
  if(ivar==5) h = myBook(name.c_str(),8,-0.5,7.5,"NLjets",sY.c_str());
  if(ivar==6) h = myBook(name.c_str(),8,-0.5,7.5,"NBjets",sY.c_str());
  if(ivar==7) h = myBook(name.c_str(),8,-0.5,7.5,"NFjets",sY.c_str());
  if(ivar==8) h = myBook(name.c_str(),40,0,200,"p_{T}^{j0} [GeV]",sY.c_str());
  if(ivar==9) h = myBook(name.c_str(),40,0,200,"p_{T}^{j1} [GeV]",sY.c_str());
  if(ivar==10) h = myBook(name.c_str(),40,0,200,"p_{T}^{j2} [GeV]",sY.c_str());

  //met
  if(ivar==11) h = myBook(name.c_str(),30,0,300,"E_{T}^{miss,rel} [GeV]",sY.c_str());
  if(ivar==12) h = myBook(name.c_str(),30,0,300,"E_{T}^{miss} [GeV]",sY.c_str());

  //dilepton
  if(ivar==13) h = myBook(name.c_str(),4,-0.5,3.5,"dilType",sY.c_str());
  if(ivar==14) h = myBook(name.c_str(),45,0,4.5,"dR_{ll} ",sY.c_str());
  if(ivar==15) h = myBook(name.c_str(),32,0,3.2,"#delta#phi_{ll} [rad]",sY.c_str());
  if(ivar==16) h = myBook(name.c_str(),30,0,3,"#delta#eta_{ll} ",sY.c_str());
  if(ivar==17) h = myBook(name.c_str(),26,20,280,"m_{ll} [GeV]",sY.c_str());
  if(ivar==18) h = myBook(name.c_str(),40,0,400,"p_{T}^{ll} [GeV]",sY.c_str());

  //lepton(s) - met
  if(ivar==19) h = myBook(name.c_str(),40,0,400,"m_{T}^{l0} [GeV]",sY.c_str());
  if(ivar==20) h = myBook(name.c_str(),40,0,400,"m_{T}^{l1} [GeV]",sY.c_str());
  if(ivar==21) h = myBook(name.c_str(),40,0,400,"max(m_{T}^{l0},(m_{T}^{l1}) [GeV]",sY.c_str());
  if(ivar==22) h = myBook(name.c_str(),40,0,400,"min(m_{T}^{l0},(m_{T}^{l1}) [GeV]",sY.c_str());
  if(ivar==23) h = myBook(name.c_str(),32,0,3.2,"#delta#phi_{l0,ETmiss} [rad]",sY.c_str());
  if(ivar==24) h = myBook(name.c_str(),32,0,3.2,"#delta#phi_{l1,ETmiss} [rad]",sY.c_str());
  if(ivar==25) h = myBook(name.c_str(),32,0,3.2,"#delta#phi(met,ll) [rad]",sY.c_str());

  //event kinematics
  if(ivar==26) h = myBook(name.c_str(),40,0,800,"HT [GeV]",sY.c_str());
  if(ivar==27) h = myBook(name.c_str(),80,0,800,"m_{Eff} [GeV]",sY.c_str());
  if(ivar==28) h = myBook(name.c_str(),40,0,400,"S_{T} [GeV]",sY.c_str());
  if(ivar==29) h = myBook(name.c_str(),20,0,200,"m_{ll}^{CollApprox} [GeV]",sY.c_str());

  //mT2 - mCT
  if(ivar==30) h = myBook(name.c_str(),40,0,400,"m_{T2} [GeV]",sY.c_str());
  if(ivar==31) h = myBook(name.c_str(),40,0,400,"m_{T2}^{J} [GeV]",sY.c_str());
  if(ivar==32) h = myBook(name.c_str(),40,0,400,"m_{T2,jj} [GeV]",sY.c_str());
  if(ivar==33) h = myBook(name.c_str(),40,0,400,"m_{CT} [GeV]",sY.c_str());
  if(ivar==34) h = myBook(name.c_str(),40,0,400,"m_{CT}^{Perp} [GeV]",sY.c_str());
  if(ivar==35) h = myBook(name.c_str(),40,0,400,"m_{CT}^{Para} [GeV]",sY.c_str());


  return h;
}
//_____________________________________________________________________________//
void fillHist()
{
  cout << "Fill histograms " << endl;

  string cmd;
  string cmdBkg;
  string cmdSig;
  string cmdData;

  TCut weight("w");
  char* str = new char[30]; 
  sprintf(str, "%.4g", scaleSignal);
  TCut scale(str);

  int signalIdx = getSignalSampleIdx(sigSampleId);
  for(uint ivar=0; ivar<_var.size(); ivar++){
    TCut _sel("metrel>=0"); //dummy cut to weight the events
    TCut _selData("");

    if(ivar==2){
      cmd = "l_eta";
      _sel += TCut("l_isEle");    _selData += TCut("l_isEle");
    }
    else if(ivar==3){
      cmd = "l_eta";
      _sel += TCut("!l_isEle");   _selData += TCut("!l_isEle");
    }
    else if(ivar==16) cmd = "abs(deta_ll)";
    else if(ivar==21) cmd = "TMath::Max(mTl[0],mTl[1])";
    else if(ivar==22) cmd = "TMath::Min(mTl[0],mTl[1])";
    else if(ivar==25) cmd = "acos(cos(phill-met_phi))";
    else if(ivar==26) cmd = "l_pt[0]+l_pt[1]+met";
    else              cmd = _var[ivar];
    
    if(dbg>1) cout << "Filling histo for " << _var[ivar] << endl;

    cmdSig = cmd + ">>sig_" + _var[ivar];
    if(weightEvt) _vNtSig[signalIdx]->Draw(cmdSig.c_str(),_sel*weight*scale,"goff");
    else          _vNtSig[signalIdx]->Draw(cmdSig.c_str(),"","goff");

    for(uint ibkg=0; ibkg<N_MC; ibkg++){
      //if(skipDetailBkg && ibkg <= ALLBKG) continue;
      cmdBkg = cmd + ">>bkg_" + MCLabel[ibkg] + "_" + _var[ivar];
      if(weightEvt) ntBkg[ibkg]->Draw(cmdBkg.c_str(),_sel*weight,"goff");
      else          ntBkg[ibkg]->Draw(cmdBkg.c_str(),"","goff");
    }
    
    cmdData = cmd + ">>data_" + _var[ivar];
    if(weightEvt) ntData->Draw(cmdData.c_str(),_selData,"goff");
    else          ntData->Draw(cmdData.c_str(),_selData,"goff");
    
  }
}

//_____________________________________________________________________________//
void plotHist(bool logy)
{
  cout << "Plot histograms " << endl;

  const float maxScaleLin=1.3;
  const float maxScaleLog=500;
  float scale=maxScaleLin;
  if(logy)  scale=maxScaleLog;

  string fileName="comp_" + itos(sigSampleId) + SR + ".root";
  TFile* _f = new TFile(fileName.c_str(), "RECREATE");
  
  float nBkg=0;
  Double_t statErrBkg=0;
  float nSig=0;

  _hBkgStack.clear();
  for(uint ivar=0; ivar<_var.size(); ivar++){  
    string label = _var[ivar] + "_Sig_AllBkg";
        
    if(_hSig[ivar]->Integral()<=0.){
      cout << _hSig[ivar]->GetTitle() << " is empty in range. Skipping. " << endl;
      continue; 
    }
    
    TLegend*  _leg = new TLegend(0.6,0.45,0.8,0.85);
    _utils->legendSetting(_leg,0.05); 
    
    THStack* _hStack = NULL;
    if(!skipDetailBkg){
      _hStack = buildStack(_leg,ivar);
      _hBkgStack.push_back(_hStack);
    }
    
    if(dbg>1) cout << "Plotting " << _var[ivar] 
		   << " " << _hSig[ivar]->Integral(0,-1)
		   << " " << _hBkgTotal->Integral(0,-1)
		   << endl;

    if(ivar==33){//Grab bkg/signal yields`
      nBkg = _hBkgTotal->IntegralAndError(0,-1,statErrBkg);
      nSig = _hSig[ivar]->Integral(0,-1)/scaleSignal;
    }

    //Prep what to plot on the bottom canvas
    _histoDir->cd();
    TH1F* _hSTempUP = NULL;
    TH1F* _hSTempDN = NULL;
    if(!weightEvt){ //normalize to 1
      float totBkg = _hBkgTotal->Integral(0,-1);
      _utils->normalize(_hBkgTotal,1);
      _utils->normalize(_hSig[ivar],1);
      if(!skipDetailBkg){
	for(uint ibkg=0; ibkg<N_MC-1; ibkg++){//normalize each bkg to their fraction of total bkg
	  float frac = 0;
	  if(totBkg>0.) frac = _hBkg[ibkg][ivar]->Integral(0,-1)/totBkg;
	  _utils->normalize(_hBkg[ibkg][ivar],frac);
	}
      }
    }
    else{//Zn vs cut values 
      _hSTempUP = ZnHistos(_hBkgTotal,_hSig[ivar],true);
      _hSTempDN = ZnHistos(_hBkgTotal,_hSig[ivar],false);
    }
    _hSignificanceUp.push_back(_hSTempUP);
    _hSignificanceDn.push_back(_hSTempDN);

    //Data MC ratio 
    TGraphAsymmErrors* _TGRatio = NULL;
    if(showData){
      TGraphAsymmErrors* _dataTG = _utils->updateDataError(_hData[ivar],true);
      TGraphAsymmErrors* _bkgTG = _utils->TH1TOTGraphAsymErrors(_hBkgTotal);
      _TGRatio = _utils->myTGraphErrorsDivide(_dataTG, _bkgTG);
      _TGRatio->SetLineWidth(2);
      _TGRatio->SetMarkerSize(1.0);

      TH1F*  _hRatio=NULL;
      _hRatio= (TH1F*) _hData[ivar]->Clone();
      _hRatio->Reset();
      _hRatio->GetXaxis()->SetTitle(_hData[ivar]->GetXaxis()->GetTitle());
      _hRatio->GetXaxis()->SetTitleSize(0.15);
      _hRatio->GetXaxis()->SetTitleOffset(1.2);
      _hRatio->SetLabelSize(0.12,"X");
      _hRatio->SetLabelSize(0.12,"Y");
      _hRatio->GetXaxis()->SetLabelOffset(0.02);
      _hRatio->GetYaxis()->SetTitleOffset(0.55);
      _hRatio->GetYaxis()->SetTitle("Data / SM");
      _hRatio->GetYaxis()->SetTitleSize(0.1);
      _hRatio->GetYaxis()->SetNdivisions(205);
      
      _ratioTG.push_back(_TGRatio);
      _ratioH.push_back(_hRatio);
    }

    //Plot the canvases
    float min =  0;
    if(logy) min = 0.1;
    float max =  _utils->getMax(_hBkgTotal,_hSig[ivar])*scale;
    _hBkgTotal->SetMaximum(max);
    _hBkgTotal->SetMinimum(min);
    _hBkgTotal->GetYaxis()->SetRangeUser(min,max);
      
    TCanvas* _c1  = _utils->myCanvas(label.c_str());
    TPad* _pTop = NULL;
    TPad* _pBot = NULL;
    if(weightEvt){
      _pTop = new TPad("pTop","pTop",0,0.3,1,1);
      _pBot = new TPad("pBot","pBot",0,0,1,0.3);
    }
    else   _pTop = new TPad("pTop","pTop",0,0,1,1);

    TVirtualPad* _tv = _c1->cd();
    _pTop->SetTopMargin(0.08);
    _pTop->SetBottomMargin(0.4);
    _pTop->SetRightMargin(0.05);
    _pTop->SetLeftMargin(0.15);
    _pTop->SetNumber(1);
    if(weightEvt){
      _pTop->SetBottomMargin(0.01);
      _pBot->SetTopMargin(0.07);
      _pBot->SetBottomMargin(0.4);
      _pBot->SetRightMargin(0.05);
      _pBot->SetLeftMargin(0.15);
      _pBot->SetNumber(2);
    }
    else _pTop->SetBottomMargin(0.2);
    _tv->cd();
    if(weightEvt)  _pBot->Draw();
    _pTop->Draw();

    //Top plot
    _pTop->cd();
    if(!skipDetailBkg){//Bkg
      _utils->myDraw1d(_hBkgTotal,_pTop,1,"hist",logy,C_ALL,false,20);
      _hBkgStack.back()->Draw("histsame");
      _hBkgStack.back()->GetXaxis()->SetTitle(_hBkg[0][ivar]->GetXaxis()->GetTitle());
      _hBkgStack.back()->GetYaxis()->SetTitle(_hBkg[0][ivar]->GetYaxis()->GetTitle());
      _hBkgStack.back()->GetYaxis()->SetTitleSize(0.05);
      _hBkgStack.back()->GetYaxis()->SetTitleOffset(1.2);
      _hBkgStack.back()->GetYaxis()->SetLabelSize(0.05);
      
      _utils->myDraw1d(_hBkgTotal,_pTop,1,"histsame",logy,C_ALL,false,20);
    }
    else {
      _utils->myDraw1d(_hBkgTotal,_pTop,1,"hist",logy,C_ALL,false,20);
    }

    //Signal
    _utils->myDraw1d(_hSig[ivar],_pTop,1,"histsame",logy,C_SIG2,false,20);

    if(showData){//Data
      _utils->myDraw1d(_hData[ivar],_pTop,1,"esame",logy,kBlack,false,20);
      _leg->AddEntry(_hData[ivar],"Data", "p");
    }

    if(scaleSignal>1){
      char* str = new char[30]; 
      sprintf(str, "%i (x %.4g)", sigSampleId,scaleSignal);
      _leg->AddEntry(_hSig[ivar],str, "l");
    }
    else
      _leg->AddEntry(_hSig[ivar],itos(sigSampleId).c_str(), "l");
    _leg->Draw();
    if(!weightEvt) _utils->myText(0.7,0.90,kBlack,"Normalized",0.03);

    _pTop->Update();    
    _pTop->RedrawAxis();
    _pTop->Update();    
    

    _tv->cd();
    //Bottom max significance 
    if(_hSignificanceUp[0] && weightEvt && !showData){
      _pBot->cd();
      min = 0.5; 
      max = 2.5;
      _hSignificanceUp.back()->SetMarkerSize(0.5);
      _hSignificanceDn.back()->SetMarkerSize(0.5);
      _utils->myDraw1d(_hSignificanceUp.back(),_pBot,1,"pe0",false, kRed, false,20);
      _utils->myDraw1d(_hSignificanceDn.back(),_pBot,1,"pe0+same",false, kBlue, false,20);
      TLine* _line = new TLine(_hSignificanceUp.back()->GetBinCenter(1)-_hSignificanceUp.back()->GetBinWidth(1)/2,1.645,
			       _hSignificanceUp.back()->GetBinCenter(_hSignificanceUp.back()->GetNbinsX())+
			       _hSignificanceUp.back()->GetBinWidth(_hSignificanceUp.back()->GetNbinsX())/2,1.645);
      _line->SetLineStyle(7);
      _line->SetLineWidth(1);
      _line->SetLineColor(kRed);
      _line->Draw();
      _hSignificanceUp.back()->GetYaxis()->SetRangeUser(min,max);
      _hSignificanceDn.back()->GetYaxis()->SetRangeUser(min,max);
      _pBot->RedrawAxis();
      _pBot->Modified();
      _pBot->Update();
    }
    //Plot Data/SM ratio
    if(showData){
      _pBot->cd();
      _ratioH.back()->GetYaxis()->SetRangeUser(0,2);
      _utils->myDraw1d(_ratioH.back(),_pBot,1,"X0 && e",false, kBlack, false,20);
      _ratioTG.back()->Draw("SAME && P");
      
      TLine* _line = new TLine(_ratioH.back()->GetBinCenter(1)-_ratioH.back()->GetBinWidth(1)/2,1,
			       _ratioH.back()->GetBinCenter(_ratioH.back()->GetNbinsX())+
			       _ratioH.back()->GetBinWidth(_ratioH.back()->GetNbinsX())/2,1);
      _line->SetLineStyle(7);
      _line->SetLineWidth(1);
      _line->SetLineColor(kRed);
      _line->Draw();
      
      _pBot->SetGridy();
      _pBot->Modified();
      _pBot->Update();
    }

    _f->cd();
    _c1->Write();
  }
  
  _f->Close();
  _histoDir->cd();

  float totErr = sqrt(pow(statErrBkg/nBkg,2)+pow(SYS_ERR,2));
  float ZnVal= RooStats::NumberCountingUtils::BinomialExpZ(nSig, nBkg, totErr);
  cout << "S: " << nSig << " B: "<< nBkg <<  " +/- " << statErrBkg << " +/- " << nBkg*SYS_ERR 
       << "\t Zn: " << ZnVal << endl;

}
//_____________________________________________________________________________//
THStack* buildStack(TLegend* _l,int ivar)
{
  _histoDir->cd();
  THStack* hStack = new THStack(string("stackBkg_"+_var[ivar]).c_str(),
				string("stackBkg_"+_var[ivar]).c_str());
  
  _hBkgTotal = (TH1F*) _hBkg[0][ivar]->Clone();
  _hBkgTotal->Reset();
  _hBkgTotal->SetTitle(string("Stack_"+_var[ivar]).c_str());
  _hBkgTotal->SetName(string("Stack_"+_var[ivar]).c_str());

  for(uint ibkg=0; ibkg<N_MC; ibkg++){
    int icol=0;
    if(ibkg==0) icol = C_HIGGS;
    if(ibkg==1) icol = C_FAKE;
    if(ibkg==2) icol = C_ZV;
    if(ibkg==3) icol = C_Zjets;
    if(ibkg==4) icol = C_TOP;
    if(ibkg==5) icol = C_WW;
    _utils->addToTHStack(hStack,_hBkg[ibkg][ivar],icol, 
			 "HIST", _l, MCNames[ibkg].c_str());
    _hBkgTotal->Add(_hBkg[ibkg][ivar],1);
  }
  return hStack;
}
//_____________________________________________________________________________//
TH1F* ZnHistos(TH1* _histBkg, TH1* _histSig, bool upper)
{
  _histoDir->cd();

  string ss = "_DN";
  if(upper) ss="_UP";

  TH1F* _hZn = (TH1F*) _histBkg->Clone();
  _hZn->Reset();
  _hZn->SetTitle(string("Zn_"+ string(_histSig->GetTitle())+ss).c_str());
  _hZn->SetName(string("Zn_"+ string(_histSig->GetTitle())+ss).c_str());
  _hZn->GetXaxis()->SetTitle(_histBkg->GetXaxis()->GetTitle());
  _hZn->GetXaxis()->SetTitleSize(0.15);
  _hZn->GetXaxis()->SetTitleOffset(1.2);
  _hZn->SetLabelSize(0.12,"X");
  _hZn->SetLabelSize(0.12,"Y");
  _hZn->GetYaxis()->SetTitle("Zn");
  _hZn->GetYaxis()->SetTitleSize(0.08);
  _hZn->GetYaxis()->SetTitleOffset(0.8);
  _hZn->GetYaxis()->SetNdivisions(205);
  
  for(int bin=0; bin<=_hZn->GetNbinsX(); ++bin){
    Double_t nBkg=0, nSig=0;
    if(upper){
      nBkg = _histBkg->Integral(bin, -1);
      nSig = _histSig->Integral(bin, -1)/scaleSignal;
    }
    else{
      nBkg = _histBkg->Integral(0, bin);
      nSig = _histSig->Integral(0, bin)/scaleSignal;
    }
    Double_t zn = 0;
    if(nBkg >0 && nSig >0) zn = RooStats::NumberCountingUtils::BinomialExpZ(nSig, nBkg, SYS_ERR);
    if( zn < 0 ) zn = 0.001;
    _hZn->SetBinContent(bin, zn);
  }

  return _hZn;

}

//_____________________________________________________________________________//
void resetHist()
{
  _histoDir->cd();
  
  TIter next(_histoDir->GetList()); //Use GetListOfKeys when reading back file
  TObject* obj=0; 
  while((obj=next())) { 
    if (!obj->InheritsFrom("TH1")) continue; //skip anything that is not TH1
    ((TH1F*) obj)->Reset(); 
  } 

}

//_____________________________________________________________________________//
void saveHist(string fileName)
{
  _histoDir->cd();

  TFile* _f = new TFile(fileName.c_str(), "RECREATE");

  TIter next(_histoDir->GetList()); //Use GetListOfKeys when reading back file
  TObject* obj=0; 
  while((obj=next())) { 
    //std::cout << obj->GetName() << std::endl; 
    if (!obj->InheritsFrom("TH1")) continue; //skip anything that is not TH1
    if ( ((TH1F*) obj)->Integral()<=0.) continue; //skip empty
    obj->Write(); 
  } 
  _f->Close();
  cout << "Saving histogram file " << fileName << endl;

}

//_____________________________________________________________________________//
int getSignalSampleIdx(int dsid)
{
  int idx=0;
  for (std::map<int,XSObject>::const_iterator it=xsmap.begin(); it != xsmap.end(); ++it) {
    int iSig = it->first;
    //cout << "Searching for " << dsid << " found " << iSig << " " << idx << endl;
    if(iSig==dsid) return idx;
    idx++;
  }
  cerr << "Do not have signal DSID " << dsid << endl;
  abort();
}

//_____________________________________________________________________________//
void getSignalSamples(string file)
{
  string dir = string(getenv("WORKAREA")) + "/SusyXSReader/data/";
  
  ifstream input;
  input.open(string(dir+file).c_str(), ifstream::in);
  if(!(input.is_open())){
    cout << "Fail to open " << dir+file << endl;
    abort();
  }
  cout << "Getting signal sample for " << file << endl;

  string line;
  xsmap.clear();
  while(getline(input,line)){
    if(line.find("DS",0)!=std::string::npos) continue;     //skip header
    if(line.size()==0) continue;     //skip empty line
    //grab the interesting info
    vector<float> tokens;
    std::istringstream format(line);
    float val;
    while(format >> val)  tokens.push_back(val);
    if(tokens.size()<5) {
      cout << "Error reading expectd line format " << endl;
      abort();
    }
    int dsid = int(tokens[0]);
    XSObject dsDB(tokens[1],tokens[3],tokens[4],tokens[5]);
    if(dbg>=2) cout << "Read " << dsid 
		    << " " << dsDB.getMC1N2() << " " << dsDB.getMN1() 
		    << " " << dsDB.getXS()<< " "  << dsDB.getXSSys() << endl;
    xsmap.insert(std::make_pair(dsid,dsDB));
  }
  cout << "Loaded " << xsmap.size() << " samples from DB file" << endl;
}

//_____________________________________________________________________________//
void cutFlow(TChain* nt, Double_t &tot, Double_t &statErr,bool detail)
{
  vector<TH1F*> _hCut;
  string cmd;
  TCut _sel("");
  TCut weight("w");

  statErr=0;
  tot=0;

  if(detail) cout << nt->GetTitle() << "\t ";
  for(unsigned int icut=0; icut<_vCut.size(); icut++){
    char hName[200];
    sprintf(hName,"cut_%02d",icut);
    TH1F* _h = myBook(hName,4,-0.5,3.5,"dilType","Entries");

    _sel += _vCut[icut];
    cmd = "llType>>" + string(_h->GetName());
    if(weightEvt) nt->Draw(cmd.c_str(),_sel*weight,"goff");
    else          nt->Draw(cmd.c_str(),_sel,"goff");
    _hCut.push_back(_h);
    
    tot = _hCut.back()->IntegralAndError(0,-1,statErr);

    if(detail){
      if(showCutflowDetail2 && icut<_vCut.size()) 
	cout << "\n" <<_vCut[icut].GetTitle() 
	     <<  std::setprecision(2) << std::fixed <<"\t\t\t" << tot << " +/- " << statErr;
      if(icut==_vCut.size()-1){
	if(showCutflowDetail2) cout << endl;
	cout << "\t\t\t " << std::setprecision(2) << std::fixed << tot << " +/- " << statErr ;
      }
    }
  }
  if(detail) cout << endl;
}

//_____________________________________________________________________________//
//_____________________________________________________________________________//
TCut setSelection(int isel, int dilType,bool verbose)
{
  TCut _sel("");
  if(isel == 0 ){
    _sel=sel_SR(dilType);
  }
  
  if(verbose) _sel.Print();

  return _sel;
}

//_____________________________________________________________________________//
TCut sel_SR(int dilType)
{
  _vCut.clear();
  TCut _thisSel("");


  //EE or MM  - same flavor
  if(dilType==0 || dilType==1)
    { 
      if(dilType==0) //EE
	{
	  _vCut.push_back(TCut("llType==0"));
	}
      else if(dilType==1) //MM
	{
	  _vCut.push_back(TCut("llType==1"));
	}
      else if(dilType==-1) //EE+MM
	{
	  _vCut.push_back(TCut("llType==0 || llType ==1"));
	}
      else return _thisSel;
      
      
      /*
      _vCut.push_back(TCut("abs(mll-91.2)>10"));
      _vCut.push_back(TCut("nSJets==0"));
      _vCut.push_back(TCut("l_pt[0]>35 && l_pt[1]>20"));
      _vCut.push_back(TCut("mT2>90"));
      */

      //_vCut.push_back(TCut("llType==1 "));
      //_vCut.push_back(TCut("llType==0 || llType==1"));
      //_vCut.push_back(TCut("abs(mll-91.2)>10"));
      //_vCut.push_back(TCut("mll<75 || mll>105"));
      //_vCut.push_back(TCut("nBJets==0 && nFJets==0"));
      //_vCut.push_back(TCut("nSJets==0"));
      //_vCut.push_back(TCut("(nJets-nBJets-nFJets)==0"));
      //_vCut.push_back(TCut("l_pt[0]>35 && l_pt[1]>20"));
      //_vCut.push_back(TCut("mT2>90"));
      //_vCut.push_back(TCut("pTll>10"));

      /*
      _vCut.push_back(TCut("abs(mll-91.2)>10"));
      _vCut.push_back(TCut("nBJets==0"));
      _vCut.push_back(TCut("nFJets==0"));
      _vCut.push_back(TCut("nCJets==0"));
      _vCut.push_back(TCut("metrel>40"));//50
      _vCut.push_back(TCut("pTll>50"));//40
      _vCut.push_back(TCut("TMath::Max(mTl[0],mTl[1])>80"));
      */
      //_vCut.push_back(TCut("mTl[0]>40"));//50-60 or use max at 50
      //_vCut.push_back(TCut("mTl[1]>20"));
      //_vCut.push_back(TCut("l_pt[0]>35"));

    }
 
  //EM
  else if(dilType==2)
    {
      _vCut.push_back(TCut("llType>=2"));
      //_vCut.push_back(TCut("nSJets==0"));
      //_vCut.push_back(TCut("nCJets==0 && nBJets==0 && nFJets==0"));
      //_vCut.push_back(TCut("l_pt[0]>35 && l_pt[1]>20"));
      //_vCut.push_back(TCut("mT2>90"));

      _vCut.push_back(TCut("nBJets==0"));
      _vCut.push_back(TCut("nFJets==0"));
      _vCut.push_back(TCut("nCJets<2"));
      _vCut.push_back(TCut("pTll>30"));//30
      _vCut.push_back(TCut("mTl[0]>40"));
      _vCut.push_back(TCut("mTl[1]>20"));
      _vCut.push_back(TCut("l_pt[0]>35"));
      //_vCut.push_back(TCut("TMath::Max(mTl[0],mTl[1])>40"));
      //dPhi(l1,MET)>0.3-0.5
      
      /*
      _vCut.push_back(TCut("nCJets==0 && nBJets==0 && nFJets==0"));
      _vCut.push_back(TCut("l_pt[0]>35 && l_pt[1]>20"));
      _vCut.push_back(TCut("mT2>90"));
      */
    }
  
  
  for(uint icut=0; icut<_vCut.size(); icut++){
    _thisSel += _vCut[icut];
  }

  return _thisSel;
}


//_____________________________________________________________________________//
//_____________________________________________________________________________//
void signalKin(int iSigId)
{
  string filename = "signalKin_" + itos(iSigId) + ".root";
  _histoDir->cd();
   
  TCut weight("w");
  TCut _sel("metrel>=0");//dummy cut
  int iSig = getSignalSampleIdx(iSigId);


  _vNtSig[iSig]->Draw("l_pt[0]>>sig_l_pt[0]",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("l_pt[1]>>sig_l_pt[1]",_sel*weight,"goff");

  _vNtSig[iSig]->Draw("nJets>>sig_nJets",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("nCJets>>sig_nCJets",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("nBJets>>sig_nBJets",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("nFJets>>sig_nFJets",_sel*weight,"goff");

  _vNtSig[iSig]->Draw("dR_ll>>sig_dR_ll",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("dphi_ll>>sig_dphi_ll",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("abs(deta_ll)>>sig_deta_ll",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("pTll>>sig_pTll",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("mll>>sig_mll",_sel*weight,"goff");

  _vNtSig[iSig]->Draw("mTl[0]>>sig_mTl[0]",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("mTl[1]>>sig_mTl[1]",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("dphi_metl[0]>>sig_dphi_metl[0]",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("dphi_metl[1]>>sig_dphi_metl[1]",_sel*weight,"goff");

  _vNtSig[iSig]->Draw("met>>sig_met",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("metrel>>sig_metrel",_sel*weight,"goff");

  _vNtSig[iSig]->Draw("mT2>>sig_mT2",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("mctPerp>>sig_mctPerp",_sel*weight,"goff");

  //Central jet - pT 
  TH1F* _h_pt_C20 = (TH1F*) _hSig[19]->Clone();
  _h_pt_C20->SetTitle("sig_jC20_pt[0]");
  _h_pt_C20->SetName("sig_jC20_pt[0]");

  _sel+= TCut("(nBJets==0 && nFJets==0 ) && j_isC20[0]");
  _vNtSig[iSig]->Draw("j_pt[0]>>sig_jC20_pt[0]",_sel*weight,"goff");
    
  saveHist(filename);
}





//_____________________________________________________________________________//
void signalYield()
{
  init();
  getSignalSamples();

  string dir =  string(getenv("HISTOANA")) + "/SusyAna/" +  ver ;
  string subDir = "ToyNtOutputs/";
  
  Double_t dummy;

  for (std::map<int,XSObject>::const_iterator it=xsmap.begin(); it != xsmap.end(); ++it) {
    int iSig = it->first;
    string sigFileName = itos(iSig)  + SR + ".root"; 
    TChain* nt = new TChain("ToyNt",itos(iSig).c_str());
    nt->Add( string(dir+subDir + sigFileName).c_str());
    if(nt->GetEntries()<10) continue;
    TCut SEL = setSelection(selCuts, selDil,true/*false*/);
    cutFlow(nt,dummy,dummy,true/*false*/);
  }
}

//_____________________________________________________________________________//
void dumpCutflow(int mcId, string filename)
{
  init();
  
  string dir =  string(getenv("HISTOANA")) + "/SusyAna/" +  ver ;
  string subDir = "ToyNtOutputs/";
  
  Double_t dummy;
  string sigFileName;

  if(mcId!=0){
    char DSID[200];
    sprintf(DSID,"%i",mcId);
    sigFileName = DSID + SR + ".root"; 
  }
  else{
    sigFileName = filename  + SR + "_rlep.root";
  }

  TChain* nt = new TChain("ToyNt");
  nt->Add( string(dir+subDir + sigFileName).c_str());

  if(nt->GetEntries()<10) return;
  TCut SEL = setSelection(selCuts, selDil,true);
  if(dbg>0) cout << mcId << " nEntries " << nt->GetEntries() << endl;

  cutFlow(nt,dummy,dummy,true);

}
//_____________________________________________________________________________//
void compareSignal(int iSig1, int iSig2, int iSig3, bool normalize)
{
  bool logy=false;

  //Input files
  string fName_1 = "signalKin_" + itos(iSig1) + ".root";
  string fName_2 = "signalKin_" + itos(iSig2) + ".root";
  string fName_3 = "signalKin_" + itos(iSig3) + ".root";
  TFile* f1 = new TFile(fName_1.c_str(),"READ");
  TFile* f2 = new TFile(fName_2.c_str(),"READ");
  TFile* f3 = new TFile(fName_3.c_str(),"READ");

  init(); //Initialise style

  string fileName = "signalKin_comparison_" + itos(iSig1) 
    + "_"  + itos(iSig2) + "_"  + itos(iSig3)  + ".root";
  TFile* _f = new TFile(fileName.c_str(), "RECREATE");

  //Loop over all the histograms and make & save canvases.
  TIter next(f1->GetListOfKeys());
  TObject* obj=0; 
  while((obj=next())) { 
    if(dbg>0) std::cout << obj->GetName() << std::endl; 

    //Get histos
    TH1F* h1 = (TH1F*) f1->Get(obj->GetName()); h1->Sumw2();
    TH1F* h2 = (TH1F*) f2->Get(obj->GetName()); h2->Sumw2();
    TH1F* h3 = (TH1F*) f3->Get(obj->GetName()); h3->Sumw2();
    
    if(normalize){ //Normalize integral to 1
      //TO DO fix error bars --- the ratio plots gets garbage errors - not sure why yet.
      _utils->normalize(h1,1.);
      _utils->normalize(h2,1.);
      _utils->normalize(h3,1.);
      h1->GetYaxis()->SetTitle("Arbitrary Unit");
      h2->GetYaxis()->SetTitle("Arbitrary Unit");
      h3->GetYaxis()->SetTitle("Arbitrary Unit");
    }

    //Set scale
    float max = h1->GetMaximum();
    max = (h2->GetMaximum()>max) ? h2->GetMaximum() : max;
    max = (h3->GetMaximum()>max) ? h3->GetMaximum() : max;
    h1->SetMaximum(max);
    h1->GetYaxis()->SetRangeUser(0,max);
    
    
    TLegend*  _leg = new TLegend(0.6,0.6,0.8,0.85);
    _utils->legendSetting(_leg,0.05); 
    
    string c_name = string("Signal Kin ") +  string(obj->GetName());
    TCanvas* _c1  = _utils->myCanvas(c_name.c_str());
    
    _utils->myDraw1d(h1,_c1,1,"e",logy,C_WW,false,20);
    _utils->myDraw1d(h2,_c1,1,"esame",logy,C_TOP,false,20);
    _utils->myDraw1d(h3,_c1,1,"esame",logy,C_Zjets,false,20);
    h1->GetYaxis()->SetTitleSize(0.8);
    
    _leg->AddEntry(h1,itos(iSig1).c_str(), "p");
    _leg->AddEntry(h2,itos(iSig2).c_str(), "p");
    _leg->AddEntry(h3,itos(iSig3).c_str(), "p");
    _leg->Draw();
    
    /*
    _utils->myText(0.6,0.50,C_WW,"#Delta m = 100GeV",0.03);
    _utils->myText(0.6,0.45,C_TOP,"#Delta m = 65GeV",0.03);
    _utils->myText(0.6,0.40,C_Zjets,"#Delta m = 35GeV",0.03);
    */
    _utils->myText(0.6,0.50,kBlack,"#Delta m = 100GeV",0.03);

    _f->cd();
    _c1->Write();
  }

  _f->Close();
  _histoDir->cd();
  cout << "Saving signal comparison in " << fileName << endl;

}


//_____________________________________________________________________________//
void make_ZnPlots()
{
  init();
  loadSamples(); 
      
  Double_t yield=0;
  Double_t statErr=0;
  Double_t totB_Yield=0;
  Double_t totB_StatErr=0;

  int ilep = 2; //Add loop
  TCut SEL = sel_SR(ilep);
  
  //Get the background yield and stat error
  /*
  cutFlow(ntBkg[FAKE],yield,statErr);      totB_Yield+=yield; totB_StatErr+= pow(statErr,2);
  cutFlow(ntBkg[ZV],yield,statErr);        totB_Yield+=yield; totB_StatErr+= pow(statErr,2);
  cutFlow(ntBkg[WW],yield,statErr);        totB_Yield+=yield; totB_StatErr+= pow(statErr,2);
  cutFlow(ntBkg[TOP],yield,statErr);       totB_Yield+=yield; totB_StatErr+= pow(statErr,2);
  cutFlow(ntBkg[Zjets],yield,statErr);     totB_Yield+=yield; totB_StatErr+= pow(statErr,2);
  cutFlow(ntBkg[HIGGS],yield,statErr);     totB_Yield+=yield; totB_StatErr+= pow(statErr,2);
  cout << "---------------------------------------------" << endl;
  totB_StatErr=sqrt(totB_StatErr);
  */
  
  totB_Yield = 5600.65;
  totB_StatErr = 38.51;
 
  cout << "Total Bkg \t\t\t"<<  totB_Yield << " +/- " 
       << totB_StatErr << " +/- " << totB_Yield*SYS_ERR  << endl;

  //Get the signal yields and stat errors and compute Zn
  std::map<int, std::pair< float,float> > signalYield;
  std::map<int, float> Zn;
  /*
  for(std::map<int,XSObject>::const_iterator it=xsmap.begin(); it != xsmap.end(); ++it) {
    int iSig = it->first;
    int iSigIdx = getSignalSampleIdx(iSig);
    cutFlow(_vNtSig[iSigIdx],yield,statErr,false);
    signalYield.insert(std::make_pair(iSig, std::make_pair(yield,statErr) ) );

    float znVal=0;
    float totErr = sqrt(pow(totB_StatErr/totB_Yield,2)+pow(SYS_ERR,2));
    if(totB_Yield >0 && yield >0) 
      znVal = RooStats::NumberCountingUtils::BinomialExpZ(yield, totB_Yield, totErr);
    if( znVal < 0 ) znVal = 0.001;
    if(dbg>2) cout << "\t\t DSID " << iSig << " Zn: " << znVal << endl;
    Zn.insert(std::make_pair(iSig,znVal));
  }
  */
  /*
    Fill TGraph2D
    3 or more Zn plots for each independent channel. Make combine Zn plot adding in quad
  */
  

  float DS_pts[nSignalPts]={0};
  float MN1_pts[nSignalPts]={0};
  float MSP_pts[nSignalPts]={0};
  float Sig_pts[nSignalPts]={0};
  float Zn_pts[nSignalPts]={0};

  TGraph2D* g_Yield = new TGraph2D();
  //Fudge: use (0,0) to 0
  int npts   = 0;
  g_Yield->SetPoint(npts, MSP_pts[npts], MN1_pts[npts], DS_pts[npts]);
  npts++;

  for(std::map<int,XSObject>::const_iterator it=xsmap.begin(); it != xsmap.end(); ++it) {
    int iSig = it->first;
    XSObject dsDB = it->second;
    std::map<int, std::pair< float,float> >::const_iterator itYield = signalYield.find(iSig);
    std::map<int, float>::const_iterator itZn = Zn.find(iSig);

    DS_pts[npts] = iSig;
    MN1_pts[npts] = dsDB.getMN1();
    MSP_pts[npts] = dsDB.getMC1N2();
    Sig_pts[npts] = itYield->second.first;  
    Zn_pts[npts]  = itZn->second;  
    cout << std::setprecision(1) << std::fixed  
	 << "Signal " <<  DS_pts[npts] 
	 << " mSP " <<  MSP_pts[npts]  << " mN1 " << MN1_pts[npts]
	 << " yield " <<  Sig_pts[npts] << " Zn " << Zn_pts[npts] << endl;
    //g_Yield->SetPoint(npts, MSP_pts[npts], MN1_pts[npts], DS_pts[npts]);
    npts++;
  }
  cout << "filled " << npts << " points " << endl;

  gStyle->SetPaintTextFormat("4.0f");

  TGraph2D* g_Yield = new TGraph2D(npts, MSP_pts, MN1_pts, Sig_pts);
  g_Yield->SetTitle("SignalYield");
  g_Yield->SetName("SignalYield");
  cout << "Graph " << g_Yield->GetN()<< endl;

  TCanvas* _c1  = _utils->myCanvas("Yield");		    
  //g_Yield->Draw("colz");
  g_Yield->Draw("text");


  /*
  TGraph2D* g_Zn = new TGraph2D(npts, MSP_pts, MN1_pts, Zn_pts);
  g_Zn->SetTitle("SignalZn");
  g_Zn->SetName("SignalZn");
  */



}

