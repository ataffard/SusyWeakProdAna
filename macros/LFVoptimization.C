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
static const string ver       = "histos_070614_21fb_n0150_DD_LFV_v1/";  
static const string SR        = "_LFV_base"; //Skimmed ToyNt suffix

static const int dbg = 1;

//Options for optimisation
static const int    selDil = 2;  //EE=0, MM=1, EM=2, ME=3 
static const int    selCuts = 4; //0: SR 1: Zpeak, 2: ttbar enriched, 3: EM/ME side bands; 4: WW VR

//LFV grids
static const unsigned int   sigSampleStart = 169670; //TauMu
static const unsigned int   sigSampleEnd   = 169671; //TauEl
static const unsigned int   sigSampleIdx   = 0; //Idx of signal sample to use for optimisation

//Settings for optimisation/plots
static const bool   weightEvt          = true;   //Weight bkg/signal events
static const bool   showData           = true;   //false; // shows Zn below plot

static const bool   skipDetailBkg      = false;  //false: shows each bkg group in cutflow & plots;
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


/* Analyses function */
void runOptimisation(bool useOneSignal=true);          //Run Optimisation

void signal_kin(int iSig=0);                           //Make basic plots of the signal kinematics.
void data_kin(int dil);                                //Plots data to compare between EM and ME

void compareData(bool normalize=false, 
		 bool logy=false);                     //Use output of data_kin to compare EM/ME channel kinematics.


void signalYield();                                    //Print signal yield 

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


string itos(int i);

/* Selections for various ana */
TCut sel_SR(int dilType, bool verbose=true);
TCut sel_Z(int dilType, bool verbose=true);
TCut sel_Top(int dilType, bool verbose=true);
TCut sel_EMSideBands(int dilType, bool verbose=true);
TCut sel_WW(int dilType, bool verbose=true);

//_____________________________________________________________________________//
//_____________________________________________________________________________//
int main(int argc, char *argv[]){

  init();
  loadSamples(); 
  bookHist();
  
  runOptimisation();
  
  /*
  signal_kin(); resetHist();
  //EM
  data_kin(EM); resetHist();
  //ME
  data_kin(ME); resetHist();
  compareData();
  */

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
    cutFlow(_vNtSig[sigSampleIdx],yield,statErr);
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
  cout << "Booking histograms... " << endl;
  string name;
  
  _var.push_back("metrel");
  _var.push_back("met");
  _var.push_back("pTll");
  _var.push_back("mll");
  _var.push_back("mll_collApprox");
  _var.push_back("dR_ll");
  _var.push_back("dphi_ll");
  _var.push_back("mEff"); //Sum lep T & met - overwrite due to bug when nSj=0!
  _var.push_back("ST");
  
  _var.push_back("l_pt[0]");
  _var.push_back("l_pt[1]");
  _var.push_back("mTl[0]");
  _var.push_back("mTl[1]");
  _var.push_back("dphi_metl[0]");
  _var.push_back("dphi_metl[1]");

  _var.push_back("nJets");
  _var.push_back("nCJets");
  _var.push_back("nBJets");
  _var.push_back("nFJets");
  _var.push_back("j_pt[0]");
  _var.push_back("j_pt[1]");
  _var.push_back("j_pt[2]");

  _var.push_back("llType");
  _var.push_back("etcone30Rel_l0");
  _var.push_back("etcone30Rel_l1");
  _var.push_back("min_mTl");
  _var.push_back("HT");
  _var.push_back("dPhiMetll");
  _var.push_back("deta_ll");
  _var.push_back("max_mTl");
  _var.push_back("e_eta");
  _var.push_back("m_eta");

  //LFV specific
  _var.push_back("mcollCorr");
  _var.push_back("metCorr");
  _var.push_back("metrelCorr");
  _var.push_back("dphi_metCorrL0");
  _var.push_back("dphi_metCorrL1");

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

  cout << "Histograms booked for " << _var.size() << " variables " << endl;
  
}
//_____________________________________________________________________________//
TH1F* histList(int ivar, string name)
{
  string sY="Entries";
  TH1F* h = NULL;
  if(ivar==0) h = myBook(name.c_str(),30,0,300,"E_{T}^{miss,rel} [GeV]",sY.c_str());
  if(ivar==1) h = myBook(name.c_str(),30,0,300,"E_{T}^{miss} [GeV]",sY.c_str());
  if(ivar==2) h = myBook(name.c_str(),40,0,400,"p_{T}^{ll} [GeV]",sY.c_str());
  if(ivar==3) h = myBook(name.c_str(),26,20,280,"m_{ll} [GeV]",sY.c_str());
  if(ivar==4) h = myBook(name.c_str(),26,20,280,"m_{ll}^{collApproximation} [GeV]",sY.c_str());
  if(ivar==5) h = myBook(name.c_str(),45,0,4.5,"dR_{ll} ",sY.c_str());
  if(ivar==6) h = myBook(name.c_str(),32,0,3.2,"#delta#phi_{ll} [rad]",sY.c_str());
  if(ivar==7) h = myBook(name.c_str(),80,0,800,"m_{Eff} [GeV]",sY.c_str());
  if(ivar==8) h = myBook(name.c_str(),40,0,400,"S_{T} [GeV]",sY.c_str());
  if(ivar==9) h = myBook(name.c_str(),40,0,200,"p_{T}^{l0} [GeV]",sY.c_str());
  if(ivar==10) h = myBook(name.c_str(),40,0,200,"p_{T}^{l1} [GeV]",sY.c_str());
  if(ivar==11) h = myBook(name.c_str(),40,0,400,"m_{T}^{l0} [GeV]",sY.c_str());
  if(ivar==12) h = myBook(name.c_str(),40,0,400,"m_{T}^{l1} [GeV]",sY.c_str());
  if(ivar==13) h = myBook(name.c_str(),32,0,3.2,"#delta#phi_{l0,ETmiss} [rad]",sY.c_str());
  if(ivar==14) h = myBook(name.c_str(),32,0,3.2,"#delta#phi_{l1,ETmiss} [rad]",sY.c_str());
  if(ivar==15) h = myBook(name.c_str(),8,-0.5,7.5,"Njets",sY.c_str());
  if(ivar==16) h = myBook(name.c_str(),8,-0.5,7.5,"NLjets",sY.c_str());
  if(ivar==17) h = myBook(name.c_str(),8,-0.5,7.5,"NBjets",sY.c_str());
  if(ivar==18) h = myBook(name.c_str(),8,-0.5,7.5,"NFjets",sY.c_str());
  if(ivar==19) h = myBook(name.c_str(),40,0,200,"p_{T}^{j0} [GeV]",sY.c_str());
  if(ivar==20) h = myBook(name.c_str(),40,0,200,"p_{T}^{j1} [GeV]",sY.c_str());
  if(ivar==21) h = myBook(name.c_str(),40,0,200,"p_{T}^{j2} [GeV]",sY.c_str());
  if(ivar==22) h = myBook(name.c_str(),4,-0.5,3.5,"dilType",sY.c_str());
  if(ivar==23) h = myBook(name.c_str(),60,-0.1,0.5,"Etcone30/pt (l0)",sY.c_str());
  if(ivar==24) h = myBook(name.c_str(),60,-0.1,0.5,"Etcone30/pt (l1)",sY.c_str());
  if(ivar==25) h = myBook(name.c_str(),40,0,400,"min(m_{T}^{l0},(m_{T}^{l1}) [GeV]",sY.c_str());
  if(ivar==26) h = myBook(name.c_str(),40,0,800,"HT [GeV]",sY.c_str());
  if(ivar==27) h = myBook(name.c_str(),32,0,3.2,"#delta#phi(met,ll) [rad]",sY.c_str());
  if(ivar==28) h = myBook(name.c_str(),30,0,3,"#delta#eta_{ll} ",sY.c_str());
  if(ivar==29) h = myBook(name.c_str(),40,0,400,"max(m_{T}^{l0},(m_{T}^{l1}) [GeV]",sY.c_str());
  if(ivar==30) h = myBook(name.c_str(),25,-2.5,2.5,"#eta^{e}",sY.c_str());
  if(ivar==31) h = myBook(name.c_str(),25,-2.5,2.5,"#eta^{#mu}",sY.c_str());

  //LFV specific
  if(ivar==32) h = myBook(name.c_str(),30,0,300,"m_{coll-corr} [GeV]",sY.c_str());
  if(ivar==33) h = myBook(name.c_str(),30,0,300,"E_{T}^{miss-corr} [GeV]",sY.c_str());
  if(ivar==34) h = myBook(name.c_str(),30,0,300,"E_{T}^{miss,rel-corr} [GeV]",sY.c_str());
  if(ivar==35) h = myBook(name.c_str(),32,0,3.2,"#delta#phi_{l0,ETmissCorr} [rad]",sY.c_str());
  if(ivar==36) h = myBook(name.c_str(),32,0,3.2,"#delta#phi_{l1,ETmissCorr} [rad]",sY.c_str());

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

  for(uint ivar=0; ivar<_var.size(); ivar++){
    TCut _sel("metrel>=0"); //dummy cut to weight the events
    TCut _selData("");
    if     (ivar==4)  cmd = "mll_collApprox";
    else if(ivar==7)  cmd = "l_pt[0]+l_pt[1]+metCorr";
    else if(ivar==23) cmd = "l_etcone30[0]/l_pt[0]";
    else if(ivar==24) cmd = "l_etcone30[1]/l_pt[1]";
    else if(ivar==25) cmd = "TMath::Min(mTl[0],mTl[1])";
    else if(ivar==26) cmd = "mEff";
    else if(ivar==27) cmd = "acos(cos(phill-metCorrPhi))";
    else if(ivar==28) cmd = "abs(deta_ll)";
    else if(ivar==29) cmd = "TMath::Max(mTl0Corr,mTl1Corr)";
    else if(ivar==30){
      cmd = "l_eta";
      _sel += TCut("l_isEle");
      _selData += TCut("l_isEle");
    }
    else if(ivar==31){
      cmd = "l_eta";
      _sel += TCut("!l_isEle");
      _selData += TCut("!l_isEle");
    }
    else if(ivar==35) cmd = "acos(cos(l_phi[0]-metCorrPhi))";
    else if(ivar==36) cmd = "acos(cos(l_phi[1]-metCorrPhi))";
    else              cmd = _var[ivar];
    
    cmdSig = cmd + ">>sig_" + _var[ivar];
    if(weightEvt) _vNtSig[sigSampleIdx]->Draw(cmdSig.c_str(),_sel*weight,"goff");
    else          _vNtSig[sigSampleIdx]->Draw(cmdSig.c_str(),"","goff");

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

  string fileName="comp_" + itos(sigSampleStart + sigSampleIdx) + SR + ".root";
  TFile* _f = new TFile(fileName.c_str(), "RECREATE");
  
  float nBkg=0;
  Double_t statErrBkg=0;
  float nSig=0;

  _hBkgStack.clear();
  for(uint ivar=0; ivar<_var.size(); ivar++){  
    string label = _var[ivar] + "_Sig_AllBkg";
        
    if(_hSig[ivar]->Integral()<=0. || _hData[ivar]->Integral()<=0. ){
      if(_hData[ivar]->Integral()<=0.) cout << _hData[ivar]->GetTitle() << " is empty in range. Skipping. " << endl;
      if(showData && _hData[ivar]->Integral()<=0.) continue; 
      if(!showData && _hSig[ivar]->Integral()<=0){ 
	cout << _hSig[ivar]->GetTitle() << " is empty in range. Skipping. " << endl;
	continue;
      }
    }
    
    TLegend*  _leg = new TLegend(0.6,0.45,0.8,0.85);
    _utils->legendSetting(_leg,0.05); 
    
    THStack* _hStack = NULL;
    if(!skipDetailBkg){
      _hStack = buildStack(_leg,ivar);
      _hBkgStack.push_back(_hStack);
    }
    
    if(dbg>2) cout << "Plotting " << _var[ivar] 
		   << " " << _hSig[ivar]->Integral(0,-1)
		   << " " << _hBkgTotal->Integral(0,-1)
		   << endl;

    if(ivar==33){//Grab bkg/signal yields`
      nBkg = _hBkgTotal->IntegralAndError(0,-1,statErrBkg);
      nSig = _hSig[ivar]->Integral(0,-1);
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

    //_leg->AddEntry(_hBkgTotal,MCNames[ALLBKG].c_str(), "l");
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
    
    tot = _hCut[icut]->IntegralAndError(0,-1,statErr);

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
    _sel = sel_SR(dilType,verbose);
  }
  else if(isel == 1){
    _sel = sel_Z(dilType,verbose);
  }
  else if(isel == 2){
    _sel = sel_Top(dilType,verbose);
  }
  else if(isel == 3){
    _sel = sel_EMSideBands(dilType,verbose);
  }
  else if(isel == 4){
    _sel = sel_WW(dilType,verbose);
  }

  if(verbose) _sel.Print();

  return _sel;
}

//_____________________________________________________________________________//
TCut sel_SR(int dilType, bool verbose)
{
  _vCut.clear();
  
  //EE
  if(dilType==0)
    { 
      _vCut.push_back(TCut("llType==0"));
      _vCut.push_back(TCut("isOS"));
      _vCut.push_back(TCut("nBJets==0"));
      // _vCut.push_back(TCut("l_pt[0]>20"));
      // _vCut.push_back(TCut("l_pt[1]>15"));
      // _vCut.push_back(TCut("nCJets>=0 && nBJets==0 && nFJets==0"));
    }
  //MM
  else if(dilType==1)
    {
      _vCut.push_back(TCut("llType==1"));
      _vCut.push_back(TCut("isOS"));
      _vCut.push_back(TCut("nBJets==0"));
      // _vCut.push_back(TCut("l_pt[0]>20"));
      // _vCut.push_back(TCut("l_pt[1]>15"));
      // _vCut.push_back(TCut("nCJets>=0 && nBJets==0 && nFJets==0"));
    }
  //EM
  else if(dilType==2)
    {
      //Shikma selection - presentation HSG4 05/21
      /*
      _vCut.push_back(TCut("llType==2"));
      _vCut.push_back(TCut("isOS"));
      _vCut.push_back(TCut("l_pt[0]>30"));
      _vCut.push_back(TCut("l_pt[1]>20"));
      //_vCut.push_back(TCut("nJets==0"));
      _vCut.push_back(TCut("nJets>=1 && j_pt[0]<30 && abs(j_eta[0])<2.5"));

      _vCut.push_back(TCut("abs(dphi_ll)>2.5"));
      _vCut.push_back(TCut("abs(dphi_metl[1])<0.5"));
      */

      _vCut.push_back(TCut("llType==2"));
      _vCut.push_back(TCut("isOS"));
      _vCut.push_back(TCut("l_pt[0]>30"));
      _vCut.push_back(TCut("l_pt[1]>18"));   
      //_vCut.push_back(TCut("nBJets==0"));
      // _vCut.push_back(TCut("nCJets>=0 && nBJets==0 && nFJets==0"));
      //_vCut.push_back(TCut("nCJets==0"));
      //      _vCut.push_back(TCut("abs(dphi_ll)>2.5"));
      //_vCut.push_back(TCut("abs(dphi_metl[1])<0.5"));

      
      
    }
  //ME
  else if(dilType==3)
    {
      //Shikma selection - presentation HSG4 05/21
      /*
      _vCut.push_back(TCut("llType==3"));
      _vCut.push_back(TCut("isOS"));
      _vCut.push_back(TCut("l_pt[0]>30"));
      _vCut.push_back(TCut("l_pt[1]>20"));
      //_vCut.push_back(TCut("nJets==0"));
      _vCut.push_back(TCut("nJets>=1 && j_pt[0]<30 && abs(j_eta[0])<2.5"));

      _vCut.push_back(TCut("abs(dphi_ll)>2.5"));
      _vCut.push_back(TCut("abs(dphi_metl[1])<0.5"));
      */


      _vCut.push_back(TCut("llType==3"));
      _vCut.push_back(TCut("isOS"));
      _vCut.push_back(TCut("l_pt[0]>40"));
      //_vCut.push_back(TCut("l_pt[1]>20"));
      _vCut.push_back(TCut("nBJets==0"));
      _vCut.push_back(TCut("nFJets==0"));

      //      _vCut.push_back(TCut("nCJets==0"));
      _vCut.push_back(TCut("nCJets==1"));
      
      
      _vCut.push_back(TCut("metrelCorr<40")); //signal has little metrel since it's aligned w/ 1 lepton
      _vCut.push_back(TCut("mll>50 && mll<120")); //signal has mll >50 - removes Ztt, mll<120 removes WW/Top
      _vCut.push_back(TCut("TMath::Max(mTl[0],mTl[1])>50"));
      _vCut.push_back(TCut("(l_pt[0]+l_pt[1]+metCorr)>100 && (l_pt[0]+l_pt[1]+metCorr)<200"));  
      _vCut.push_back(TCut("acos(cos(l_phi[1]-metCorrPhi))<0.5"));
      //_vCut.push_back(TCut("pTll>20"));


      //Not use
      //_vCut.push_back(TCut("abs(dphi_ll)>2.5"))
      //_vCut.push_back(TCut("met<80")); //signal has met ~40. WW/Top have higher MET.
      
      /*
	mT l0/ max MT, peaks higher for signal than Ztt
	HT between 100-200 - lower bound reduce Ztt, upper bound WW/top. cut 60% eff on signal- needed considering mcoll?

	In 1j need to suppress firther Ztt
	
       */

    }
 


  //Build the TCut
  TCut _thisSel("");
  for(uint icut=0; icut<_vCut.size(); icut++){
    _thisSel += _vCut[icut];
  }
  
  return _thisSel;
}

//_____________________________________________________________________________//
TCut sel_Z(int dilType, bool verbose)
{
  cout << "Selecting Z validation region " << endl;
  _vCut.clear();
  TCut _thisSel("");

  if(dilType==0) //EE
    {
      _vCut.push_back(TCut("llType==0"));
    }
  else if(dilType==1) //MM
    {
      _vCut.push_back(TCut("llType==1"));
    }
  else return _thisSel;
  
  _vCut.push_back(TCut("abs(mll-91.2)<10"));
  _vCut.push_back(TCut("nBJets==0 && nFJets==0"));
  //_vCut.push_back(TCut("nCJets==0"));
  _vCut.push_back(TCut("nCJets>0"));
  _vCut.push_back(TCut("l_pt[0]>30"));
  _vCut.push_back(TCut("l_pt[1]>18"));

   //Build the TCut
  for(uint icut=0; icut<_vCut.size(); icut++){
    _thisSel += _vCut[icut];
  }
  return _thisSel;
}

//_____________________________________________________________________________//
TCut sel_Top(int dilType, bool verbose)
{
  cout << "Selecting Top validation region " << endl;
  _vCut.clear();
  TCut _thisSel("");

  if(dilType==2)//EM+ME
    {
      _vCut.push_back(TCut("llType==2 || llType==3"));
    }
  else return _thisSel;

  _vCut.push_back(TCut("nBJets>0"));
  _vCut.push_back(TCut("nFJets==0"));
  _vCut.push_back(TCut("l_pt[0]>30"));
  _vCut.push_back(TCut("l_pt[1]>18"));


  //Build the TCut
  for(uint icut=0; icut<_vCut.size(); icut++){
    _thisSel += _vCut[icut];
  }
  
  return _thisSel;
}

//_____________________________________________________________________________//
TCut sel_EMSideBands(int dilType, bool verbose)
{
  cout << "Selecting EM or ME side bands " << endl;
  _vCut.clear();
  TCut _thisSel("");
  
  if(dilType==2)//EM
    {
      _vCut.push_back(TCut("llType==2"));
    }
  else if(dilType==3)//EM
    {
      _vCut.push_back(TCut("llType==3"));
    }
  else return _thisSel;
  
  _vCut.push_back(TCut("mcollCorr<100 || mcollCorr>150"));
  _vCut.push_back(TCut("nBJets==0"));
  _vCut.push_back(TCut("nFJets==0"));
  _vCut.push_back(TCut("l_pt[0]>30"));
  _vCut.push_back(TCut("l_pt[1]>18"));
  _vCut.push_back(TCut("nCJets==0"));
  // _vCut.push_back(TCut("nCJets>0"));


 //Build the TCut
  for(uint icut=0; icut<_vCut.size(); icut++){
    _thisSel += _vCut[icut];
  }

  return _thisSel;
}

//_____________________________________________________________________________//
TCut sel_WW(int dilType, bool verbose)
{
  cout << "Selecting WW validation region " << endl;
  _vCut.clear();
  TCut _thisSel("");

  if(dilType==2)//EM+ME
    {
      _vCut.push_back(TCut("llType==2 || llType==3"));
    }
  else return _thisSel;

  _vCut.push_back(TCut("nBJets==0"));
  _vCut.push_back(TCut("nFJets==0"));
  _vCut.push_back(TCut("l_pt[0]>30"));
  _vCut.push_back(TCut("l_pt[1]>18"));
  //_vCut.push_back(TCut("nCJets==0"));
  _vCut.push_back(TCut("nCJets>0"));
  _vCut.push_back(TCut("mT2>40"));

  //Build the TCut
  for(uint icut=0; icut<_vCut.size(); icut++){
    _thisSel += _vCut[icut];
  }
  
  return _thisSel;
}


//_____________________________________________________________________________//
//_____________________________________________________________________________//
void signal_kin(int iSig)
{
  string filename = "signalKin_" + itos(sigSampleStart + iSig) + ".root";

  _histoDir->cd();
   
  TCut weight("w");
  TCut _sel("metrel>=0");//dummy cut

  _vNtSig[iSig]->Draw("l_pt[0]>>sig_l_pt[0]",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("l_pt[1]>>sig_l_pt[1]",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("nJets>>sig_nJets",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("nCJets>>sig_nCJets",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("nBJets>>sig_nBJets",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("nFJets>>sig_nFJets",_sel*weight,"goff");

  _vNtSig[iSig]->Draw("dphi_ll>>sig_dphi_ll",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("abs(deta_ll)>>sig_deta_ll",_sel*weight,"goff");

  _vNtSig[iSig]->Draw("acos(cos(l_phi[0]-metCorrPhi))>>sig_dphi_metl[0]",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("acos(cos(l_phi[1]-metCorrPhi))>>sig_dphi_metl[1]",_sel*weight,"goff");

  _vNtSig[iSig]->Draw("met>>sig_met",_sel*weight,"goff");
  _vNtSig[iSig]->Draw("metCorr>>sig_metCorr",_sel*weight,"goff");

  _vNtSig[iSig]->Draw("mcollCorr>>sig_mcollCorr",_sel*weight,"goff");

  //Central jet - pT 
  TH1F* _h_pt_C20 = (TH1F*) _hSig[19]->Clone();
  _h_pt_C20->SetTitle("sig_jC20_pt[0]");
  _h_pt_C20->SetName("sig_jC20_pt[0]");

  _sel+= TCut("nBJets==0 && j_isC20[0]");
  _vNtSig[iSig]->Draw("j_pt[0]>>sig_jC20_pt[0]",_sel*weight,"goff");
  
  //Forward jet - pT 
  TH1F* _h_pt_F30 = (TH1F*) _hSig[19]->Clone();
  _h_pt_F30->SetTitle("sig_jF30_pt[0]");
  _h_pt_F30->SetName("sig_jF30_pt[0]");

  _sel = TCut("nBJets==0 && j_isF30[0]");
  _vNtSig[iSig]->Draw("j_pt[0]>>sig_jF30_pt[0]",_sel*weight,"goff");
  

  saveHist(filename);
}



//_____________________________________________________________________________//
//_____________________________________________________________________________//
void data_kin(int dil)
{
  string filename = "dataKin_" + LepN[dil] + ".root";

  _histoDir->cd();

  TCut weight("w");
  TCut _sel("metrel>=0");//dummy cut
  TCut _sel2("");//additional selection cuts

  TCut _sel_EM("llType==2");
  TCut _sel_ME("llType==3");

  if(dil==2   )    _sel += _sel_EM;
  else if (dil==3) _sel += _sel_ME;

  ntData->SetEventList(0);//reset eventList

  string listName = "elist_Data_" + LepN[dil] ;
  string cmd = ">>" + listName ;

  ntData->Draw(cmd.c_str(),_sel,"goff");
  _dataEvtList = (TEventList*)gDirectory->Get(listName.c_str());
  ntData->SetEventList(_dataEvtList);
  cout << "\t Data in " << LepN[dil]  << " events, selected " <<_dataEvtList->GetN() <<endl;

  ntData->Draw("l_pt[0]>>data_l_pt[0]",_sel*weight,"goff");
  ntData->Draw("l_pt[1]>>data_l_pt[1]",_sel*weight,"goff");
  ntData->Draw("nJets>>data_nJets",_sel*weight,"goff");
  ntData->Draw("nCJets>>data_nCJets",_sel*weight,"goff");
  ntData->Draw("nBJets>>data_nBJets",_sel*weight,"goff");
  ntData->Draw("nFJets>>data_nFJets",_sel*weight,"goff");

  ntData->Draw("dphi_ll>>data_dphi_ll",_sel*weight,"goff");
  ntData->Draw("abs(deta_ll)>>data_deta_ll",_sel*weight,"goff");

  ntData->Draw("dphi_metl[0]>>data_dphi_metl[0]",_sel*weight,"goff");
  ntData->Draw("dphi_metl[1]>>data_dphi_metl[1]",_sel*weight,"goff");

  ntData->Draw("met>>data_met",_sel*weight,"goff");
  ntData->Draw("metCorr>>data_metCorr",_sel*weight,"goff");

  ntData->Draw("mcollCorr>>data_mcollCorr",_sel*weight,"goff");

  //Central jet - pT 
  TH1F* _h_pt_C20 = (TH1F*) _hSig[19]->Clone();
  _h_pt_C20->SetTitle("data_jC20_pt[0]");
  _h_pt_C20->SetName("data_jC20_pt[0]");

  _sel2 += _sel + TCut("nBJets==0 && j_isC20[0]");
  ntData->Draw("j_pt[0]>>data_jC20_pt[0]",_sel2*weight,"goff");
  
  //Forward jet - pT 
  TH1F* _h_pt_F30 = (TH1F*) _hSig[19]->Clone();
  _h_pt_F30->SetTitle("data_jF30_pt[0]");
  _h_pt_F30->SetName("data_jF30_pt[0]");

  _sel2.Clear();
  _sel2 = _sel + TCut("nBJets==0 && j_isF30[0]");
  ntData->Draw("j_pt[0]>>data_jF30_pt[0]",_sel2*weight,"goff");

  //With selection cuts
  //lepton pT>20
  _sel2.Clear();
  _sel2 = _sel + TCut("l_pt[0]>20 && l_pt[1]>20");
  TH1F* _h_pt_l0 = (TH1F*) _hSig[9]->Clone();
  _h_pt_l0->SetTitle("data_l0_pt_cut20");
  _h_pt_l0->SetName("data_l0_pt_cut20");
  TH1F* _h_pt_l1 = (TH1F*) _hSig[10]->Clone();
  _h_pt_l1->SetTitle("data_l1_pt_cut20");
  _h_pt_l1->SetName("data_l1_pt_cut20");

  ntData->Draw("l_pt[0]>>data_l0_pt_cut20",_sel2*weight,"goff");
  ntData->Draw("l_pt[1]>>data_l1_pt_cut20",_sel2*weight,"goff");

  TH1F* _h_pt_l0_Lead = (TH1F*) _hSig[9]->Clone();
  _h_pt_l0_Lead->SetTitle("data_l0_pt_cut20_Lead");
  _h_pt_l0_Lead->SetName("data_l0_pt_cut20_Lead");
  TH1F* _h_pt_l1_subLead = (TH1F*) _hSig[10]->Clone();
  _h_pt_l1_subLead->SetTitle("data_l1_pt_cut20_subLead");
  _h_pt_l1_subLead->SetName("data_l1_pt_cut20_subLead");


  if(dil==2   ){//EM
    TCut _sel_lep("l_isEle[0] && !l_isEle[1]");
    ntData->Draw("l_pt[0]>>data_l0_pt_cut20_Lead",(_sel_lep&&_sel2)*weight,"goff");
    ntData->Draw("l_pt[1]>>data_l1_pt_cut20_subLead",(_sel_lep&&_sel2)*weight,"goff");
  }
  else if (dil==3){//MU
    TCut _sel_lep("!l_isEle[0] && l_isEle[1]");
    ntData->Draw("l_pt[0]>>data_l0_pt_cut20_Lead",(_sel_lep&&_sel2)*weight,"goff");
    ntData->Draw("l_pt[1]>>data_l1_pt_cut20_subLead",(_sel_lep&&_sel2)*weight,"goff");
  }

  TH1F* _h_dPhi_ll = (TH1F*) _hSig[6]->Clone();
  _h_dPhi_ll->SetTitle("data_dPhill_cut20");
  _h_dPhi_ll->SetName("data_dPhill_cut20");

  TH1F* _h_dEta_ll = (TH1F*) _hSig[28]->Clone();
  _h_dEta_ll->SetTitle("data_dEtall_cut20");
  _h_dEta_ll->SetName("data_dEtall_cut20");

  TH1F* _h_dPhi_metL1 = (TH1F*) _hSig[14]->Clone();
  _h_dPhi_metL1->SetTitle("data_dPhi_metL1_cut20");
  _h_dPhi_metL1->SetName("data_dPhi_metL1_cut20");

  ntData->Draw("dphi_ll>>data_dPhill_cut20",_sel2*weight,"goff");
  ntData->Draw("abs(deta_ll)>>data_dEtall_cut20",_sel2*weight,"goff");
  ntData->Draw("dphi_metl[1]>>data_dPhi_metL1_cut20",_sel2*weight,"goff");
  
  TH1F* _h_dPhi_metL1_corr = (TH1F*) _hSig[14]->Clone();
  _h_dPhi_metL1_corr->SetTitle("data_dPhi_metL1_corr_cut20");
  _h_dPhi_metL1_corr->SetName("data_dPhi_metL1_corr_cut20");
  ntData->Draw("acos(cos(l_phi[1]-metCorrPhi))>>data_dPhi_metL1_corr_cut20",_sel2*weight,"goff");
  

  //+ Angular cuts
  _sel2.Clear();
  _sel2 = _sel + TCut("l_pt[0]>40 && l_pt[1]>20 && abs(dphi_ll)>2.5 && abs(acos(cos(l_phi[1]-metCorrPhi)))<0.5");
  TH1F* _h_pt_l0_ang = (TH1F*) _hSig[9]->Clone();
  _h_pt_l0_ang->SetTitle("data_l0_pt_cut4020_ang");
  _h_pt_l0_ang->SetName("data_l0_pt_cut4020_ang");
  TH1F* _h_pt_l1_ang = (TH1F*) _hSig[10]->Clone();
  _h_pt_l1_ang->SetTitle("data_l1_pt_cut4020_ang");
  _h_pt_l1_ang->SetName("data_l1_pt_cut4020_ang");

  TH1F* _h_dPhi_ll_ang = (TH1F*) _hSig[6]->Clone();
  _h_dPhi_ll_ang->SetTitle("data_dPhill_cut4020");
  _h_dPhi_ll_ang->SetName("data_dPhill_cut4020");
  TH1F* _h_dPhi_metL1_corr_ang = (TH1F*) _hSig[14]->Clone();
  _h_dPhi_metL1_corr_ang->SetTitle("data_dPhi_metL1_corr_cut4020_ang");
  _h_dPhi_metL1_corr_ang->SetName("data_dPhi_metL1_corr_cut4020_ang");

  ntData->Draw("l_pt[0]>>data_l0_pt_cut4020_ang",_sel2*weight,"goff");
  ntData->Draw("l_pt[1]>>data_l1_pt_cut4020_ang",_sel2*weight,"goff");
  ntData->Draw("dphi_ll>>data_dPhill_cut4020_ang",_sel2*weight,"goff");
  ntData->Draw("acos(cos(l_phi[1]-metCorrPhi))>>data_dPhi_metL1_corr_cut4020_ang",_sel2*weight,"goff");

  TH1F* _h_mcollCorr_ang = (TH1F*) _hSig[32]->Clone();
  _h_mcollCorr_ang->SetTitle("data_mcollCorr_cut4020_ang");
  _h_mcollCorr_ang->SetName("data_mcollCorr_cut4020_ang");
  ntData->Draw("mcoll>>data_mcollCorr_cut4020_ang",_sel2*weight,"goff");

  saveHist(filename);

}
//_____________________________________________________________________________//
void compareData(bool normalize, bool logy)
{
  const float maxEntries =500;
  const float maxScaleLin=1.2;
  const float maxScaleLog=2;
 
  //Input files
  string fName_EM = "dataKin_" + LepN[EM] + ".root";
  string fName_ME = "dataKin_" + LepN[ME] + ".root";
  TFile* f_EM = new TFile(fName_EM.c_str(),"READ");
  TFile* f_ME = new TFile(fName_ME.c_str(),"READ");
  
  init(); //Initialise style
  
  string fileName = "dataKin_comparison_EM_ME.root";
  TFile* _f = new TFile(fileName.c_str(), "RECREATE");
  
  //Loop over all the histogram and make & save canvases.
  TIter next(f_EM->GetListOfKeys());
  TObject* obj=0; 
  while((obj=next())) { 
    std::cout << obj->GetName() << std::endl; 

    //Get histos
    TH1F* h_EM = (TH1F*) f_EM->Get(obj->GetName()); h_EM->Sumw2();
    TH1F* h_ME = (TH1F*) f_ME->Get(obj->GetName()); h_ME->Sumw2();

    if(normalize){ //Normalize integral to 1
      //TO DO fix error bars --- the ratio plots gets garbage errors - not sure why yet.
      _utils->normalize(h_EM,1.);
      _utils->normalize(h_ME,1.);
      h_EM->GetYaxis()->SetTitle("Arbitrary Unit");
      h_ME->GetYaxis()->SetTitle("Arbitrary Unit");
    }
    
    //overwrite logy base on the number of entries
    //if(h_EM->Integral(0,-1) > maxEntries || h_ME->Integral(0,-1) > maxEntries) logy=true;
    //if(h_EM->Integral(0,-1) < maxEntries || h_ME->Integral(0,-1) < maxEntries) logy=false;
    float scale=maxScaleLin;
    if(logy)  scale=maxScaleLog;

    //Set the y-range to the max allowed
    float min =  0;
    if(logy) min = 0.1;
    float max =  _utils->getMax(h_EM,h_ME)*scale;
    if(logy) max =  scale * _utils->getMax(h_EM,h_ME);
    h_EM->SetMaximum(max);
    h_EM->SetMinimum(min);
    h_EM->GetYaxis()->SetRangeUser(min,max);
    
    //Build the ratio plot
    TGraphAsymmErrors* _TG_EM   = _utils->updateDataError(h_EM,true);
    TGraphAsymmErrors* _TG_ME   = _utils->updateDataError(h_ME,true);
    TGraphAsymmErrors* _TGRatio = _utils->myTGraphErrorsDivide(_TG_EM, _TG_ME);
    _TGRatio->SetLineWidth(2);
    _TGRatio->SetMarkerSize(1.0);
    
    TH1F*  _hRatio=NULL; //to ease plotting
    _hRatio= (TH1F*) h_EM->Clone();
    _hRatio->Reset();
    _hRatio->GetXaxis()->SetTitle(h_EM->GetXaxis()->GetTitle());
    _hRatio->GetXaxis()->SetTitleSize(0.12);
    _hRatio->GetXaxis()->SetTitleOffset(1.2);
    _hRatio->SetLabelSize(0.12,"X");
    _hRatio->SetLabelSize(0.12,"Y");
    _hRatio->GetXaxis()->SetLabelOffset(0.02);
    _hRatio->GetYaxis()->SetTitleOffset(0.55);
    _hRatio->GetYaxis()->SetTitle("EM/ ME");
    _hRatio->GetYaxis()->SetTitleSize(0.8);
    _hRatio->GetYaxis()->SetNdivisions(205);
    _ratioTG.push_back(_TGRatio); //Put in vectors so that not to lose when plotting multiple plots
    _ratioH.push_back(_hRatio);
    
    //
    //Plottng canvas
    //
    TLegend*  _leg = new TLegend(0.6,0.6,0.8,0.85);
    _utils->legendSetting(_leg,0.05); 
    
    string c_name = string(obj->GetName()) + " EM-vs-ME";
    TCanvas* _c1  = _utils->myCanvas(c_name.c_str());

    TPad* _pTop = NULL;
    TPad* _pBot = NULL;
    _pTop = new TPad("pTop","pTop",0,0.3,1,1);
    _pBot = new TPad("pBot","pBot",0,0,1,0.3);

    TVirtualPad* _tv = _c1->cd();
    _pTop->SetTopMargin(0.08);
    _pTop->SetBottomMargin(0.4);
    _pTop->SetRightMargin(0.05);
    _pTop->SetLeftMargin(0.15);
    _pTop->SetNumber(1);
    _pTop->SetBottomMargin(0.01);
    _pBot->SetTopMargin(0.07);
    _pBot->SetBottomMargin(0.4);
    _pBot->SetRightMargin(0.05);
    _pBot->SetLeftMargin(0.15);
    _pBot->SetNumber(2);
  
    _tv->cd();
    _pBot->Draw();
    _pTop->Draw();

    _pTop->cd();
    _utils->myDraw1d(h_EM,_pTop,1,"e",logy,C_WW,false,20);
    _utils->myDraw1d(h_ME,_pTop,1,"esame",logy,C_TOP,false,20);
    h_EM->GetYaxis()->SetTitleSize(0.8);

    _leg->AddEntry(h_EM,string("Data " + LepNames[EM]).c_str(), "p");
    _leg->AddEntry(h_ME,string("Data " + LepNames[ME]).c_str(), "p");
    _leg->Draw();
    
    _pTop->Update();    
    _pTop->RedrawAxis();
    _pTop->Update();    

    _tv->cd();
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


    //Save canvas to file
    _f->cd();
    _c1->Write();
  }
  
  _f->Close();
  
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
    TCut SEL = setSelection(selCuts, selDil,true/*false*/);
    cutFlow(nt,dummy,dummy,true/*false*/);
  }
}
