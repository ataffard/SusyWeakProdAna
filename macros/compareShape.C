
#include <stdlib.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>

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
#include "RooStats/NumberCountingUtils.h"

#include "SusyNtuple/TGuiUtils.h"

//_____________________________________________________________________________//
static const string ver       = "histos_011814_21fb_n0145_DD_WH_v2/";  //ToyNt filtered SS, 3rd lep veto, B/F-veto >=1 C20 jet
static const string SR        = "_WH_optimSRSS";

static const int dbg = 1;

//Options for optimisation
static const int    selDil    = 2;  //EE, MM, EM for SS and C1C1 grids
static const int    selCuts   = 1;  //WH 1 or 2+3jets

//wA WH grids
static const unsigned int   sigSampleStart = 177501;
static const unsigned int   sigSampleEnd   = 177527;
//static const unsigned int   sigSampleStart = 126893;
//static const unsigned int   sigSampleEnd   = 126893;
static const unsigned int   sigSampleIdx   = 0; //Idx of signal sample to use for optimisation

//Settings for optimisation/plots
static const bool   weightEvt          = true;   //Weight bkg/signal events
static const bool   showData           = false;   //false: shows Zn below plot

static const bool   skipDetailBkg      = false;  //false: shows each bkg group;
static const bool   showCutflow        = true;   //dump cutflow yield
static const bool   showCutflowDetail  = true;   //dump yield last cut
static const bool   showCutflowDetail2 = false;  //dump cutflow each cut
static const bool   logPlot            = true;   

static const float SYS_ERR = 0.3; //30% systematic on Bkg  - Note: Zn add stat err on tot bkg

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
  ALLBKG=6,
  N_MC=7
};

const std::string MCLabel[] ={
  "Higgs",
  "FAKE",
  "ZV",
  "Zjets",
  "TOP",
  "WW",
  "ALL"
};

const std::string MCNames[] ={
  "Higgs",
  "Fake leptons",
  "ZV",
  "Z+jets",
  "Top",
  "WW",
  "SM Bkg"
};

enum LEP {
  EE=0,
  MM,
  EM,
  ALL,
  N_LEP
};

const std::string LepN[] = {
  "EE",
  "MM",
  "EM",
  "ALL"
};
const std::string LepNames[] = {
  "$ee$",
  "$\\mu\\mu$",
  "$e\\mu$",
  "Total"
};

enum MCCOL { C_HIGGS=kYellow-9,
	     C_FAKE=kGray, 
	     C_ZV=kSpring+1, 
	     C_Zjets=kOrange-2,
	     C_TOP=kRed+1, 
	     C_WW=kAzure+4, 
	     C_ALL=kBlue,
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

vector<TEventList*> _bkgEvtList;
vector<TEventList*> _sigEvtList;
TEventList*         _dataEvtList;

void runOptimisation(bool useOneSignal=true);          //Run Optimisation
void signalYield();                                    //Print signal yield 
void getZnForGrid(int idil, int iSR);                                    //Make Zn plots given a selection


void init();
//load Bkg, data and signals 
void loadSamples();                                     
//Set TCut to apply
TCut setSelection(string SRegion, int isel, int dilType=3, bool verbose=true); 
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
TH1F*    ZnHistos(TH1F* _hBkg, TH1F* _hSig, bool upper=true);
THStack* buildStack(TLegend* _l, int ivar);

/* Selections for various ana */
TCut sel_AnyesSRSS(int opt, int dilType, bool verbose=true);
TCut sel_AnyesWH(int opt, int dilType=3, bool verbose=true);


string itos(int i);

//_____________________________________________________________________________//
//_____________________________________________________________________________//
int main(int argc, char *argv[]){
  
  runOptimisation();

}

//_____________________________________________________________________________//
void runOptimisation(bool useOneSignal)
{
 
  init();
  loadSamples(); 

  Double_t yield=0;
  Double_t statErr=0;

  TCut SEL = setSelection(SR, selCuts,selDil);
  if(showCutflow){
    cutFlow(_vNtSig[sigSampleIdx],yield,statErr);
    cout << "---------------------------------------------" << endl;
    cout << "---------------------------------------------" << endl;
    if(!skipDetailBkg && showCutflowDetail){
      cutFlow(ntBkg[ZV],yield,statErr);
      cutFlow(ntBkg[Zjets],yield,statErr);
      cutFlow(ntBkg[WW],yield,statErr);
      cutFlow(ntBkg[TOP],yield,statErr);
      cutFlow(ntBkg[HIGGS],yield,statErr);
      cutFlow(ntBkg[FAKE],yield,statErr);
      cout << "---------------------------------------------" << endl;
      cutFlow(ntBkg[ALLBKG],yield,statErr);
      cout << "---------------------------------------------" << endl;
      cout << "---------------------------------------------" << endl;
    }
  }
  selectEvent(SEL,useOneSignal);

  bookHist();
  fillHist();
  plotHist(logPlot);

}

//_____________________________________________________________________________//
void signalYield()
{
  init();

  string dir =  string(getenv("HISTOANA")) + "/SusyAna/" +  ver ;
  string subDir = "ToyNtOutputs/";
  
  Double_t dummy;

  for(uint iS=sigSampleStart; iS<= sigSampleEnd; iS++){
    char DSID[200];
    sprintf(DSID,"%i",iS);
    string sigFileName = DSID + SR + ".root"; 
    TChain* nt = new TChain("ToyNt",DSID);
    nt->Add( string(dir+subDir + sigFileName).c_str());
    if(nt->GetEntries()<10) continue;
    TCut SEL = setSelection(SR, selCuts, selDil,false);
    cutFlow(nt,dummy,dummy,true/*false*/);
  }
}

//_____________________________________________________________________________//
void getZnForGrid(int idil, int iSR)
{

  init();
  loadSamples();
      
  TCut SEL = setSelection(SR,iSR,idil,true /*false*/);
  selectEvent(SEL,false);
  SEL.Print();
  
  Double_t nBkg, bkgStatErr;
  cutFlow(ntBkg[ALLBKG],nBkg,bkgStatErr);
  
  vector<Double_t> nSig;
  vector<Double_t> sigStatErr;
  vector<Double_t> ZnValues;
  uint nSignalSamples = sigSampleEnd - sigSampleStart +1;
  for(uint iSig=0; iSig<nSignalSamples; iSig++){
    Double_t tot=0, err=0;
    if(_vNtSig[iSig]->GetEntries()>0){ //protect against missing samples
      cutFlow(_vNtSig[iSig],tot,err,false);      
    }
    nSig.push_back(tot);
    sigStatErr.push_back(err);
    
    float totErr = sqrt(pow(bkgStatErr/nBkg,2)+pow(SYS_ERR,2));
    float ZnVal= RooStats::NumberCountingUtils::BinomialExpZ(tot, nBkg, totErr);
    cout << "\t " << sigSampleStart+iSig << " S: " << tot 
	 << "\t B: "<< nBkg <<  " +/- " << bkgStatErr << " +/- " << nBkg*SYS_ERR 
	 << "\t\t Zn: " << ZnVal << endl;
  }
  

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
  _bkgFileNames.push_back(string("toyNt_dataFake" + SR + "_flep.root").c_str()); //FAKE
  _bkgFileNames.push_back(string("toyNt_WZ_ZZ_PowHeg" + SR + "_rlep.root").c_str());
  _bkgFileNames.push_back(string("toyNt_Zjets_SherpaAlpgenPythia" + SR + "_rlep.root").c_str());
  _bkgFileNames.push_back(string("toyNt_top_MCNLO" + SR + "_rlep.root").c_str());
  _bkgFileNames.push_back(string("toyNt_WW_PowHeg" + SR + "_rlep.root").c_str());
  if(SR=="_WH_optimSRSS")
    _bkgFileNames.push_back(string("toyNt_Bkg_Zjets_SherpaAlpgen_WZ_ZZ_PowHeg_WW_PowHeg_TopMCNLO_FAKE" + SR + ".root").c_str());
  else 
    _bkgFileNames.push_back(string("toyNt_Bkg_Zjets_SherpaAlpgen_WZ_ZZ_PowHeg_WW_PowHeg_TopMCNLO" + SR + "_rlep.root").c_str());
  
  //Create the TChain for BKG
  for(uint ibkg=0; ibkg<N_MC; ibkg++){
    ntBkg[ibkg] = new TChain("ToyNt",string("Bkg_"+MCLabel[ibkg]).c_str());
    ntBkg[ibkg]->Add( string(dir+_bkgFileNames[ibkg]).c_str() );
    if(dbg>0) cout << MCLabel[ibkg ] << " nEntries " << ntBkg[ibkg]->GetEntries() << endl;
  }
  
  //Load and create TChain for Data
  ntData = new TChain("ToyNt",string("Data_").c_str());
  ntData->Add( string(dir+"toyNt_data12" + SR + "_std.root").c_str() );
  if(dbg>0)  cout << "Data nEntries " << ntData->GetEntries() << endl;
  
  //Load Signals
  _vNtSig.erase (_vNtSig.begin(),_vNtSig.end());
  _col.push_back(kRed-4);  //Signal  
  uint nSignalSamples = sigSampleEnd - sigSampleStart +1;
  if(dbg>0) cout << "Loading signal from " << sigSampleStart << " -> " << sigSampleEnd << endl;

  for(uint iSig=0; iSig<nSignalSamples; iSig++){
    string sigFileName = itos(sigSampleStart+iSig)  + SR + ".root"; 
    TChain* _nt = new TChain("ToyNt", string("Signal_"+itos(sigSampleStart+iSig)).c_str());
    _nt->Add( string(dir+subDir + sigFileName).c_str() );
    if(dbg>1)  cout << "Signal " << sigSampleStart+iSig << " nEntries " << _nt->GetEntries() << endl;
    _vNtSig.push_back(_nt);
  }

}


//_____________________________________________________________________________//
void selectEvent(TCut _cut, bool useOneSignal)
{
  cout << "Generating eventlists " << endl;

  //Select the events and put them in some event lists for Bkg & Signal
  //Set the event list for plotting

  uint nSignalSamples = sigSampleEnd - sigSampleStart +1;
  for(uint iSig=0; iSig<nSignalSamples; iSig++){
    if(useOneSignal && iSig!= sigSampleIdx){
      _sigEvtList.push_back(NULL);
      continue;
    }
    if(_vNtSig[iSig]->GetEntries()==0){ //protect against missing samples
      _sigEvtList.push_back(NULL);
      cout << "\tSignal sample " << sigSampleStart+iSig << " missing. Skipping" << endl;
      continue;
    }
    string sigName = itos(sigSampleStart+iSig);
    string cmd = "elist_Sig_" + sigName;
    _vNtSig[iSig]->Draw(string(">>"+cmd).c_str(),_cut,"goff");
    _sigEvtList.push_back((TEventList*)gDirectory->Get(cmd.c_str()));
    if(dbg>0) cout << "\t Signal selected " <<_sigEvtList[iSig]->GetN() <<endl;
    _vNtSig[iSig]->SetEventList(_sigEvtList[iSig]);
  }

  for(uint ibkg=0; ibkg<N_MC; ibkg++){
    if(skipDetailBkg && ibkg<ALLBKG) continue; 
    string evtName = "elist_Bkg_" + MCLabel[ibkg];
    ntBkg[ibkg]->Draw(string(">>" + evtName).c_str(),_cut,"goff");
    TEventList *evtListBkg = (TEventList*)gDirectory->Get(evtName.c_str());
    _bkgEvtList.push_back(evtListBkg);
    ntBkg[ibkg]->SetEventList(evtListBkg);
    if(dbg>0) cout << "\t EvtList for " << evtName 
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
  
  _var.push_back("metrel");
  _var.push_back("met");
  _var.push_back("mWWT");
  _var.push_back("mT2");
  _var.push_back("mct");
  _var.push_back("mctPerp");
  _var.push_back("mctPara");
  _var.push_back("pTll");
  _var.push_back("mll");
  _var.push_back("mll_collApprox");
  _var.push_back("dR_ll");
  _var.push_back("dphi_ll");
  _var.push_back("JZBjets");
  _var.push_back("mEff");
  _var.push_back("ST");
  _var.push_back("mjj");
  
  _var.push_back("l_pt[0]");
  _var.push_back("l_pt[1]");
  _var.push_back("mTl[0]");
  _var.push_back("mTl[1]");
  _var.push_back("dphi_metl[0]");
  _var.push_back("dphi_metl[1]");

  _var.push_back("nJets");
  _var.push_back("j_pt[0]");
  _var.push_back("j_pt[1]");
  _var.push_back("j_pt[2]");

  _var.push_back("topTag");
  _var.push_back("metomEff");
  _var.push_back("mT2jj");
  _var.push_back("sphericity");
  _var.push_back("sphericityTrans");
  _var.push_back("llAcoplanarity");
  _var.push_back("jjAcoplanarity");
  _var.push_back("llType");
  _var.push_back("etcone30Rel_l0");
  _var.push_back("etcone30Rel_l1");
  _var.push_back("min_mTl");
  _var.push_back("HT");
  _var.push_back("Smv1");
  _var.push_back("dPhiMetll");
  _var.push_back("mT2J");
  _var.push_back("mlj");
  _var.push_back("mljj");
  _var.push_back("dEtajj");
  _var.push_back("deta_ll");
  _var.push_back("max_mTl");

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
  if(ivar==0) h = myBook(name.c_str(),30,0,300,"E_{T}^{miss,rel} [GeV]",sY.c_str());
  if(ivar==1) h = myBook(name.c_str(),30,0,300,"E_{T}^{miss} [GeV]",sY.c_str());
  if(ivar==2) h = myBook(name.c_str(),30,0,600,"m_{T}^{WW} [GeV]",sY.c_str());
  if(ivar==3) h = myBook(name.c_str(),40,0,400,"m_{T2} [GeV]",sY.c_str());
  if(ivar==4) h = myBook(name.c_str(),40,0,400,"m_{CT} [GeV]",sY.c_str());
  if(ivar==5) h = myBook(name.c_str(),40,0,400,"m_{CT}^{Perp} [GeV]",sY.c_str());
  if(ivar==6) h = myBook(name.c_str(),40,0,400,"m_{CT}^{Para} [GeV]",sY.c_str());
  if(ivar==7) h = myBook(name.c_str(),40,0,400,"p_{T}^{ll} [GeV]",sY.c_str());
  if(ivar==8) h = myBook(name.c_str(),26,20,280,"m_{ll} [GeV]",sY.c_str());
  if(ivar==9) h = myBook(name.c_str(),26,20,280,"m_{ll}^{collApproximation}-91.2 [GeV]",sY.c_str());
  if(ivar==10) h = myBook(name.c_str(),45,0,4.5,"dR_{ll} ",sY.c_str());
  if(ivar==11) h = myBook(name.c_str(),32,0,3.2,"d#phi_{ll} [rad]",sY.c_str());
  if(ivar==12) h = myBook(name.c_str(),30,0,300,"|JZB| [GeV]",sY.c_str());
  if(ivar==13) h = myBook(name.c_str(),80,0,800,"m_{Eff} [GeV]",sY.c_str());
  if(ivar==14) h = myBook(name.c_str(),40,0,400,"S_{T} [GeV]",sY.c_str());
  if(ivar==15) h = myBook(name.c_str(),40,0,200,"m_{jj} [GeV]",sY.c_str());
  if(ivar==16) h = myBook(name.c_str(),40,0,200,"p_{T}^{l0} [GeV]",sY.c_str());
  if(ivar==17) h = myBook(name.c_str(),40,0,200,"p_{T}^{l1} [GeV]",sY.c_str());
  if(ivar==18) h = myBook(name.c_str(),40,0,400,"m_{T}^{l0} [GeV]",sY.c_str());
  if(ivar==19) h = myBook(name.c_str(),40,0,400,"m_{T}^{l1} [GeV]",sY.c_str());
  if(ivar==20) h = myBook(name.c_str(),32,0,3.2,"d#phi_{l0,ETmiss} [rad]",sY.c_str());
  if(ivar==21) h = myBook(name.c_str(),32,0,3.2,"d#phi_{l0,ETmiss} [rad]",sY.c_str());
  if(ivar==22) h = myBook(name.c_str(),8,-0.5,7.5,"Njets",sY.c_str());
  if(ivar==23) h = myBook(name.c_str(),40,0,200,"p_{T}^{j0} [GeV]",sY.c_str());
  if(ivar==24) h = myBook(name.c_str(),40,0,200,"p_{T}^{j1} [GeV]",sY.c_str());
  if(ivar==25) h = myBook(name.c_str(),40,0,200,"p_{T}^{j2} [GeV]",sY.c_str());
  if(ivar==26) h = myBook(name.c_str(),2,0,2,"topTag",sY.c_str());
  if(ivar==27) h = myBook(name.c_str(),50,0,1,"met/m_{Eff}",sY.c_str());
  if(ivar==28) h = myBook(name.c_str(),40,0,400,"m_{T2,jj} [GeV]",sY.c_str());
  if(ivar==29) h = myBook(name.c_str(),50,0,1,"Sphericity",sY.c_str());
  if(ivar==30) h = myBook(name.c_str(),50,0,1,"Sphericity_{Transv}",sY.c_str());
  if(ivar==31) h = myBook(name.c_str(),60,0,6,"Acoplanarity_{ll}",sY.c_str());
  if(ivar==32) h = myBook(name.c_str(),60,0,6,"Acoplanarity_{jj}",sY.c_str());
  if(ivar==33) h = myBook(name.c_str(),3,-0.5,2.5,"dilType",sY.c_str());
  if(ivar==34) h = myBook(name.c_str(),60,-0.1,0.5,"Etcone30/pt (l0)",sY.c_str());
  if(ivar==35) h = myBook(name.c_str(),60,-0.1,0.5,"Etcone30/pt (l1)",sY.c_str());
  if(ivar==36) h = myBook(name.c_str(),40,0,400,"min(m_{T}^{l0},(m_{T}^{l1}) [GeV]",sY.c_str());
  if(ivar==37) h = myBook(name.c_str(),40,0,800,"HT [GeV]",sY.c_str());
  if(ivar==38) h = myBook(name.c_str(),50,0,1,"MV1_j0+MV1_j1",sY.c_str());
  if(ivar==39) h = myBook(name.c_str(),32,0,3.2,"dPhi(met,ll) [rad]",sY.c_str());
  if(ivar==40) h = myBook(name.c_str(),40,0,400,"m_{T2}^{J} [GeV]",sY.c_str());
  if(ivar==41) h = myBook(name.c_str(),40,0,400,"m_{lj} [GeV]",sY.c_str());
  if(ivar==42) h = myBook(name.c_str(),40,0,400,"m_{ljj} [GeV]",sY.c_str());
  if(ivar==43) h = myBook(name.c_str(),40,0,4,"#delta#eta_{jj} ",sY.c_str());
  if(ivar==44) h = myBook(name.c_str(),30,0,3,"#delta#eta_{ll} ",sY.c_str());
  if(ivar==45) h = myBook(name.c_str(),40,0,400,"max(m_{T}^{l0},(m_{T}^{l1}) [GeV]",sY.c_str());

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

  TCut _sel("metrel>=0"); //dummy cut to weight the events
  TCut weight("w");

  for(uint ivar=0; ivar<_var.size(); ivar++){
    if     (ivar==9)  cmd = "abs(mll_collApprox-91.2)";
    else if(ivar==27) cmd = "met/mEff";
    else if(ivar==31) cmd = "abs(llAcoplanarity+3.1415)";
    else if(ivar==32) cmd = "abs(jjAcoplanarity+3.1415)";
    else if(ivar==34) cmd = "l_etcone30[0]/l_pt[0]";
    else if(ivar==35) cmd = "l_etcone30[1]/l_pt[1]";
    else if(ivar==36) cmd = "TMath::Min(mTl[0],mTl[1])";
    else if(ivar==37) cmd = "mEff";
    else if(ivar==38) cmd = "j_mv1[0]+j_mv1[1]";
    else if(ivar==39) cmd = "acos(cos(phill-met_phi))";
    else if(ivar==43) cmd = "abs(dEtajj)";
    else if(ivar==44) cmd = "abs(deta_ll)";
    else if(ivar==45) cmd = "TMath::Max(mTl[0],mTl[1])";
    else              cmd = _var[ivar];
    
    
    cmdSig = cmd + ">>sig_" + _var[ivar];
    if(weightEvt) _vNtSig[sigSampleIdx]->Draw(cmdSig.c_str(),_sel*weight,"goff");
    else          _vNtSig[sigSampleIdx]->Draw(cmdSig.c_str(),"","goff");

    for(uint ibkg=0; ibkg<N_MC; ibkg++){
      if(skipDetailBkg && ibkg <= ALLBKG) continue;
      cmdBkg = cmd + ">>bkg_" + MCLabel[ibkg] + "_" + _var[ivar];
      if(weightEvt) ntBkg[ibkg]->Draw(cmdBkg.c_str(),_sel*weight,"goff");
      else          ntBkg[ibkg]->Draw(cmdBkg.c_str(),"","goff");
    }
    
    cmdData = cmd + ">>data_" + _var[ivar];
    if(weightEvt) ntData->Draw(cmdData.c_str(),"","goff");
    else          ntData->Draw(cmdData.c_str(),"","goff");
    
  }
}

//_____________________________________________________________________________//
void plotHist(bool logy)
{
  cout << "Plot histograms " << endl;

  const float maxScaleLin=1.4;
  const float maxScaleLog=500;
  float scale=maxScaleLin;
  if(logy)  scale=maxScaleLog;

  string fileName="comp_" + itos(sigSampleStart + sigSampleIdx) + SR + ".root";
  TFile* _f = new TFile(fileName.c_str(), "RECREATE");
  
  float nBkg=0;
  Double_t statErrBkg=0;
  float nSig=0;

  _hBkgStack.clear();
  for(uint ivar=0; ivar<_var.size(); ivar++){  
    string label = _var[ivar] + "_Sig_AllBkg";

    if(dbg>2) cout << "Plotting " << _var[ivar] 
		   << " " << _hSig[ivar]->Integral(0,-1)
		   << " " << _hBkg[ALLBKG][ivar]->Integral(0,-1)
		   << endl;
    
    if(_hSig[ivar]->Integral()<=0.){
      cout << _hSig[ivar]->GetTitle() << " is empty in range. Skipping. " << endl;
      continue; 
    }
    
    if(ivar==33){//Grab bkg/signal yields
      nBkg = _hBkg[ALLBKG][ivar]->IntegralAndError(0,-1,statErrBkg);
      nSig = _hSig[ivar]->Integral(0,-1);
    }

    //Prep what to plot on the bottom canvas
    _histoDir->cd();
    TH1F* _hSTempUP = NULL;
    TH1F* _hSTempDN = NULL;
    if(!weightEvt){ //normalize to 1
      float totBkg = _hBkg[ALLBKG][ivar]->Integral(0,-1);
      _utils->normalize(_hBkg[ALLBKG][ivar],1);
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
      _hSTempUP = ZnHistos(_hBkg[ALLBKG][ivar],_hSig[ivar],true);
      _hSTempDN = ZnHistos(_hBkg[ALLBKG][ivar],_hSig[ivar],false);
    }
    _hSignificanceUp.push_back(_hSTempUP);
    _hSignificanceDn.push_back(_hSTempDN);

    //Data MC ratio 
    TGraphAsymmErrors* _TGRatio = NULL;
    if(showData){
      TGraphAsymmErrors* _dataTG = _utils->updateDataError(_hData[ivar],true);
      TGraphAsymmErrors* _bkgTG = _utils->TH1TOTGraphAsymErrors(_hBkg[ALLBKG][ivar]);
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
    float max =  _utils->getMax(_hBkg[ALLBKG][ivar],_hSig[ivar])*scale;
    _hBkg[ALLBKG][ivar]->SetMaximum(max);
    _hBkg[ALLBKG][ivar]->SetMinimum(min);
    _hBkg[ALLBKG][ivar]->GetYaxis()->SetRangeUser(min,max);
      
    TLegend*  _leg = new TLegend(0.6,0.45,0.8,0.85);
    _utils->legendSetting(_leg,0.05); 

    THStack* _hStack = NULL;
    if(!skipDetailBkg){
      _hStack = buildStack(_leg,ivar);
      _hBkgStack.push_back(_hStack);
    }

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
      _utils->myDraw1d(_hBkg[ALLBKG][ivar],_pTop,1,"hist",logy,C_ALL,false,20);
      _hBkgStack.back()->Draw("histsame");
      _hBkgStack.back()->GetXaxis()->SetTitle(_hBkg[0][ivar]->GetXaxis()->GetTitle());
      _hBkgStack.back()->GetYaxis()->SetTitle(_hBkg[0][ivar]->GetYaxis()->GetTitle());
      _hBkgStack.back()->GetYaxis()->SetTitleSize(0.05);
      _hBkgStack.back()->GetYaxis()->SetTitleOffset(1.2);
      _hBkgStack.back()->GetYaxis()->SetLabelSize(0.05);
      
      _utils->myDraw1d(_hBkg[ALLBKG][ivar],_pTop,1,"histsame",logy,C_ALL,false,20);
    }
    else {
      _utils->myDraw1d(_hBkg[ALLBKG][ivar],_pTop,1,"hist",logy,C_ALL,false,20);
    }

    //Signal
    _utils->myDraw1d(_hSig[ivar],_pTop,1,"histsame",logy,C_SIG2,false,20);

    if(showData){//Data
      _utils->myDraw1d(_hData[ivar],_pTop,1,"esame",logy,kBlack,false,20);
      _leg->AddEntry(_hData[ivar],"Data", "p");
    }

    _leg->AddEntry(_hBkg[ALLBKG][ivar],MCNames[ALLBKG].c_str(), "l");
    _leg->AddEntry(_hSig[ivar],itos(sigSampleStart+sigSampleIdx).c_str(), "l");
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
THStack* buildStack(TLegend* _l, int ivar)
{
  _histoDir->cd();
  THStack* hStack = new THStack(string("stackBkg_"+_var[ivar]).c_str(),
				string("stackBkg_"+_var[ivar]).c_str());
  
  for(uint ibkg=0; ibkg<N_MC-1; ibkg++){
    int icol=0;
    if(ibkg==0) icol = C_HIGGS;
    if(ibkg==1) icol = C_FAKE;
    if(ibkg==2) icol = C_ZV;
    if(ibkg==3) icol = C_Zjets;
    if(ibkg==4) icol = C_TOP;
    if(ibkg==5) icol = C_WW;
    _utils->addToTHStack(hStack,_hBkg[ibkg][ivar],icol, 
			 "HIST", _l, MCNames[ibkg].c_str());
  }
  return hStack;
}
//_____________________________________________________________________________//
TH1F* ZnHistos(TH1F* _histBkg, TH1F* _histSig, bool upper)
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
      nSig = _histSig->Integral(bin, -1);
    }
    else{
      nBkg = _histBkg->Integral(0, bin);
      nSig = _histSig->Integral(0, bin);
    }
    Double_t zn = 0;
    if(nBkg >0 && nSig >0) zn = RooStats::NumberCountingUtils::BinomialExpZ(nSig, nBkg, SYS_ERR);
    if( zn < 0 ) zn = 0.001;
    _hZn->SetBinContent(bin, zn);
  }

  return _hZn;

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
    TH1F* _h = myBook(hName,3,-0.5,2.5,"dilType","Entries");

    _sel += _vCut[icut];
    cmd = "llType>>" + string(_h->GetName());
    if(weightEvt) nt->Draw(cmd.c_str(),_sel*weight,"goff");
    else          nt->Draw(cmd.c_str(),_sel,"goff");
    _hCut.push_back(_h);
    
    tot = _hCut[icut]->IntegralAndError(0,-1,statErr);

    if(detail){
      if(showCutflowDetail2 && icut<_vCut.size()-1) 
	cout << "\n" <<_vCut[icut].GetTitle() 
	     <<  std::setprecision(2) << std::fixed <<"\t\t\t" << tot << " +/- " << statErr;
      else if(icut==_vCut.size()-1){
	if(showCutflowDetail2) cout << endl;
	cout << "\t\t\t " << std::setprecision(2) << std::fixed << tot << " +/- " << statErr ;
      }
    }
  }
  if(detail) cout << endl;
}
//_____________________________________________________________________________//
//_____________________________________________________________________________//
TCut setSelection(string SRegion, int isel, int dilType,bool verbose)
{
  TCut _sel("");
  if(SRegion == "_DIL_optimSRSS" ){
    _sel=sel_AnyesSRSS(isel,dilType,verbose);
  }
  else if(SRegion == "_WH_optimSRSS" ){
    _sel = sel_AnyesWH(isel, dilType,verbose);
  }
  
  if(verbose) _sel.Print();

  return _sel;
}

//_____________________________________________________________________________//
TCut sel_AnyesSRSS(int opt, int dilType, bool verbose)
{
  _vCut.clear();
  
  _vCut.push_back(TCut("llType>=0"));

  if(dilType==0){//EE
    _vCut.push_back(TCut("llType==0"));
    _vCut.push_back(TCut("abs(mll-91.2)>10"));
    _vCut.push_back(TCut("nCJets>=0 && nBJets==0 && nFJets==0"));
    _vCut.push_back(TCut("l_pt[0]>20 && l_pt[1]>20"));
    //_vCut.push_back(TCut("abs(l_d0[0])<3 && abs(l_d0[1])<3")); //does nothing
    _vCut.push_back(TCut("metrel>40"));
    _vCut.push_back(TCut("pTll<40")); //remove DiB
    _vCut.push_back(TCut("mWWT>40")); //remove Zjets
    _vCut.push_back(TCut("dphi_ll<2.5"));//remove Zjets
    _vCut.push_back(TCut("nCJets>=1"));//improve S/B if want to beleive. Not enough signal stat - 5 raw.

  }
  else if(dilType==1){//MM
    _vCut.push_back(TCut("llType==1"));
    _vCut.push_back(TCut("nCJets>=0 && nBJets==0 && nFJets==0"));
    _vCut.push_back(TCut("l_etcone30[1]/l_pt[1]<0.1"));
    _vCut.push_back(TCut("l_etcone30[0]/l_pt[0]<0.1"));
    _vCut.push_back(TCut("l_pt[0]>20"));
    _vCut.push_back(TCut("l_pt[1]>15"));
    _vCut.push_back(TCut("metrel>20"));
    _vCut.push_back(TCut("mct<40"));
    _vCut.push_back(TCut("mll<100"));   
    _vCut.push_back(TCut("pTll<20"));

    //_vCut.push_back(TCut("nCJets>0"));
  }
  else if(dilType==2){//EM
    _vCut.push_back(TCut("llType==2"));
    _vCut.push_back(TCut("nCJets>=0 && nBJets==0 && nFJets==0 "));
    _vCut.push_back(TCut("l_etcone30[1]/l_pt[1]<0.1 && !l_isEle[1]"));
    _vCut.push_back(TCut("l_etcone30[0]/l_pt[0]<0.1 && !l_isEle[0]"));
    _vCut.push_back(TCut("l_pt[0]>20 && l_pt[1]>20"));
    _vCut.push_back(TCut("metrel>40"));
    _vCut.push_back(TCut("mll_collApprox>100"));
    _vCut.push_back(TCut("nCJets>=1"));
    _vCut.push_back(TCut("pTll<40"));
    //_vCut.push_back(TCut("met/mEff>0.3"));   
  }
  
  TCut _thisSel("");
  for(uint icut=0; icut<_vCut.size(); icut++){
    _thisSel += _vCut[icut];
  }
  
  return _thisSel;
}

//_____________________________________________________________________________//
TCut sel_AnyesWH(int opt, int dilType, bool verbose)
{
  _vCut.clear();
  //
  // ==1 jet
  //
  if(opt==1){ //SS==1 jets
    if(dilType==0){ //EE
      if(verbose) cout << " \t SRWH SS-EE 1j" << endl;
      _vCut.push_back(TCut("llType==0"));

      //SR
      /*
      _vCut.push_back(TCut("nCJets==1"));
      _vCut.push_back(TCut("l_pt[0]>30"));
      _vCut.push_back(TCut("l_pt[1]>20"));
      //_vCut.push_back(TCut("mll<70 || mll>100"));
      //_vCut.push_back(TCut("mll<80 || mll>100"));
      _vCut.push_back(TCut("abs(mll-91.2)>10"));
      _vCut.push_back(TCut("mlj<90")); 
      _vCut.push_back(TCut("mEff>200"));
      _vCut.push_back(TCut("metrel>55"));
      */

      //VR-fake
      _vCut.push_back(TCut("abs(mll-91.2)>10"));
      //_vCut.push_back(TCut("mll<70 || mll>100"));
      _vCut.push_back(TCut("metrel>40"));
      _vCut.push_back(TCut("(mlj>90 && nCJets==1) || (mljj>120 && nCJets>1)")); 

      //test
      //_vCut.push_back(TCut("metrel>80"));
      //_vCut.push_back(TCut("mWWT>130"));

      //_vCut.push_back(TCut("abs(deta_ll)<1.5"));
      //_vCut.push_back(TCut("TMath::Max(mTl[0],mTl[1])>110"));
      //_vCut.push_back(TCut("abs(mll-91.2)>10"));

    }
    else if(dilType==1){ //MM
      if(verbose) cout << " \t SRWH SS-MM 1j" << endl;
      _vCut.push_back(TCut("llType==1 && !isOS"));
      

      //SR
      _vCut.push_back(TCut("nCJets==1"));
      _vCut.push_back(TCut("l_pt[0]>30"));
      _vCut.push_back(TCut("l_pt[1]>20"));
      _vCut.push_back(TCut("abs(deta_ll)<1.5"));
      _vCut.push_back(TCut("TMath::Max(mTl[0],mTl[1])>100"));
      _vCut.push_back(TCut("mlj<90")); 
      _vCut.push_back(TCut("mEff>200"));
      //_vCut.push_back(TCut("mEff>230"));

      //CR-ZV
      /*
      _vCut.push_back(TCut("l_pt[0]>30"));
      _vCut.push_back(TCut("l_pt[1]>30"));
      _vCut.push_back(TCut("(mlj>90 && nCJets==1) || (mljj>120 && nCJets>1)")); 
      */
      
      //CR-fake
      /*
      _vCut.push_back(TCut("l_pt[1]<30"));
      _vCut.push_back(TCut("(mlj>90 && nCJets==1) || (mljj>120 && nCJets>1)")); 
      */


      ////_vCut.push_back(TCut("metrel>40"));
      ////_vCut.push_back(TCut("TMath::Min(mTl[0],mTl[1])>80"));

    }
    else if(dilType==2){ //EM
      if(verbose) cout << " \t SRWH SS-EM 1j" << endl;
      _vCut.push_back(TCut("llType==2"));

      
      //SR
      /*
      _vCut.push_back(TCut("nCJets==1"));
      _vCut.push_back(TCut("l_pt[0]>30"));
      _vCut.push_back(TCut("l_pt[1]>30"));
      _vCut.push_back(TCut("abs(deta_ll)<1.5"));
      _vCut.push_back(TCut("TMath::Max(mTl[0],mTl[1])>110"));
      _vCut.push_back(TCut("mlj<90")); 
      _vCut.push_back(TCut("mWWT>120"));
      */

      //CR FAKE+ZV
      //_vCut.push_back(TCut("l_pt[0]>30"));
      //_vCut.push_back(TCut("l_pt[1]>30"));
      //_vCut.push_back(TCut("(mlj>90 && nCJets==1) || (mljj>120 && nCJets>1)")); 

      //CR FAKE
      
      _vCut.push_back(TCut("l_pt[0]>30"));
      _vCut.push_back(TCut("l_pt[1]<30"));
      _vCut.push_back(TCut("(mlj>90 && nCJets==1) || (mljj>120 && nCJets>1)")); 
      
      
    }
  }
  //
  // >=2 jets
  //
  else if(opt==2){ // SS->=2 jets
    if(dilType==0){ //EE
      if(verbose) cout << " \t SRWH  SS-EE 2-3 j" << endl;
      _vCut.push_back(TCut("llType==0"));
      _vCut.push_back(TCut("nCJets>1 && nCJets<4"));
      _vCut.push_back(TCut("l_pt[0]>30"));
      _vCut.push_back(TCut("l_pt[1]>20"));
      //_vCut.push_back(TCut("mll<70 || mll>100"));
      //_vCut.push_back(TCut("mll<80 || mll>100"));
      _vCut.push_back(TCut("abs(mll-91.2)>10"));
      _vCut.push_back(TCut("TMath::Max(mTl[0],mTl[1])>100"));
      _vCut.push_back(TCut("mljj<120"));
      //_vCut.push_back(TCut("metrel>55"));
      _vCut.push_back(TCut("metrel>30"));

    }
    else if(dilType==1){ //MM
      if(verbose) cout << " \t SRWH SS-MM 2-3j" << endl;
      _vCut.push_back(TCut("llType==1 && !isOS"));
      _vCut.push_back(TCut("nCJets>1 && nCJets<4"));
      _vCut.push_back(TCut("l_pt[0]>30"));
      _vCut.push_back(TCut("l_pt[1]>30"));
      _vCut.push_back(TCut("abs(deta_ll)<1.5"));
      _vCut.push_back(TCut("mljj<120"));
      //_vCut.push_back(TCut("mEff>220"));
      _vCut.push_back(TCut("mEff>260"));

      //_vCut.push_back(TCut("TMath::Max(mTl[0],mTl[1])>80"));
      //_vCut.push_back(TCut("metrel>40")); //remove as much S than B
    }
    else if(dilType==2){ //EM
      if(verbose) cout << " \t SRWH SS-EM 2-3j" << endl;
      _vCut.push_back(TCut("llType==2"));
      _vCut.push_back(TCut("nCJets>1 && nCJets<4"));
      _vCut.push_back(TCut("l_pt[0]>30"));
      _vCut.push_back(TCut("l_pt[1]>30"));
      _vCut.push_back(TCut("abs(deta_ll)<1.5"));
      //_vCut.push_back(TCut("TMath::Max(mTl[0],mTl[1])>120"));
      _vCut.push_back(TCut("mljj<120"));

      _vCut.push_back(TCut("mWWT>110"));
      //_vCut.push_back(TCut("TMath::Max(mTl[0],mTl[1])>120"));

    }
  }

  
  TCut _thisSel("");
  for(uint icut=0; icut<_vCut.size(); icut++){
    _thisSel += _vCut[icut];
  }

  return _thisSel;
}
