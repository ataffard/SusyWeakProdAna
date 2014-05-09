//_____________________________________________________________________________//
//_____________________________________________________________________________//
//   .L ../macros/fakeStudies.C++
//   fakeStudies(opt value)
//_____________________________________________________________________________//
//_____________________________________________________________________________//


/*
Davide's code
https://github.com/gerbaudo/SusyTest0/blob/44055a89c17a9fa33aed9beaab46e0da0483877c/Root/IterativeFakeCorrection.cxx
https://github.com/gerbaudo/SusyTest0/blob/sf-eta-dep/Root/MeasureFakeRate2.cxx
https://github.com/gerbaudo/SusyTest0/blob/master/run/python/iterativeCorrection.py
*/


#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <map>
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

using namespace std;

//_____________________________________________________________________________//
static const string ver       = "histos_030614_21fb_n0150_DD_FAKE_v1/";
static const string SR        = "_WH_optimSRSS";

static const bool useQFLIP=false; //Cannot really use qFlip here... too complicated, sample dependent


static const int dbg = 1;


//Settings for optimisation/plots
static const bool   weightEvt          = true;   //Weight bkg/signal events
static const bool   showData           = true;   //false: shows Zn below plot

//_____________________________________________________________________________//

const float MZ = 91.2;
const string HF_kfrac = "5.166"; //Determine be scaling the HF to match the data in the loose sample SSEM

enum MC 
{
  HF=0, 
  WJETS=1, 
  Zjets=2,  
  TOP=3, 
  ZV=4, 
  WW=5, 
  N_MC=6
};

enum MCCOL { C_HF=kMagenta-2,
	     C_Wjets=kOrange+8, 
	     C_Zjets=kOrange-2,
	     C_TOP=kRed+1, 
	     C_ZV=kSpring+1, 
	     C_WW=kAzure+4, 
	     C_ALL=kBlack
};

const std::string MCLabel[] ={
  "HF",
  "Wjets",
  "Zjets",
  "TOP",
  "ZV",
  "WW",
};

const std::string MCNames[] ={
  "HF",
  "Wjets",
  "Z+jets",
  "Top",
  "ZV",
  "WW",
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


const std::string LepOrg[] = {
  "PR",
  "CONV",
  "HF",
  "LF",
  "Undef"
};

//_____________________________________________________________________________//

TGuiUtils* _utils;
TDirectory* _histoDir;

TChain* ntBkg[N_MC];
TChain* ntData;

vector<string> _type;                   //Bkg or data
vector<string> _lepKind;                //Tag or probe
vector<string> _lepType;                //Loose or type
vector<string> _lepCatg;                //Any, Fake, Real
vector<string> _selIter;                //mT40 mT100

vector< pair<string,string> > _varLep;  //var name - ntuple var
vector< pair<string,string> > _varEvt;  //var name - ntuple var



vector<string>             _bkgFileNames;
//Bkg and data histo, split for leptons and event vars
vector<TH1F*>              _hBkgLep[N_MC][3][2][2]; //[N_MC], [Any/Fake/Real], [Tag/Probe], [Loose/Tight]
vector<TH1F*>              _hDataLep[2][2];
vector<TH1F*>              _hBkgEvt[N_MC]; //[N_MC], [Tag/Probe], [Loose/Tight]
vector<TH1F*>              _hDataEvt;
//Used for iterative correction
vector<TH1F*>              _hBkgProbe[N_MC][2][2]; //[MTlow,MThigh] [Loose,Tight]
vector<TH1F*>              _hDataProbe[2][2];      //[MTlow,MThigh] [Loose,Tight]


vector<TCut>               _vEvtCut;
vector<TCut>               _vProbeCut;
vector<TCut>               _vTagCut;

TCut                       evtSel;
TCut                       tagSel;
TCut                       probeSel;
TCut                       finalSel;


TH1F*                      _hBkgTotal;


vector<TEventList*> _bkgEvtList;
TEventList*         _dataEvtList;


//
// Opt=0 fill hist. Opt=1 compute FR & SF. Opt=2 draw plots
//
void fakeStudies(int opt=0);

void init();
//load Bkg, data and signals 
void loadSamples();      

//Setup the event and lepton selections 
void setSelection(string SRegion, int dilType, int probeFlavor, bool verbose=true);


//Specific analysis selections
void do_SSEM(int dilType, int probeFlavor, bool verbose);

//Select the events
void selectEvent(TCut _cut);   

/* Functions for plotting */
void     bookHist();
void     fillHist(string sRegion);                    //fill all histos using event List
void     saveHistograms(string sRegion);
void     dumpEventYield(string sRegion);
void     plotHist(string sRegion, bool logy=true);    //plot & save plots to file

//Compute FR & SF using MC SM correction
void     getFRSF_mcCorr(string sRegion, string vName);
void     plotFRSF_mcCorr(string sRegion, string vName, bool logy=true);

//Compute FR &Sf using iterative correction
void     getFRSF_iterCorr(string sRegion, string vName);
void     plotFRSF_iterCorr(string sRegion, string vName, bool logy=true);

void     correctRate(TH1F* &rate, TH1F* data, TH1F* mc,
		     vector<double> corrections);
vector<double> getC(TH1F* rate, TH1F* data_num, TH1F* data_den, TH1F* mc, bool tight);
double         getFake(float r, float f, float nL, float nT, bool tight);

TH1F*    histList(string hName, string vName);
TH1F*    myBook(string hName, int nbin, float  xlow, float  xhigh, string xaxis, string yaxis);
TH1F*    myBook(string hName, int nbin, const Double_t* nX, string xaxis, string yaxis);
THStack* buildStack(TLegend* _l, vector<TH1F*>* hList, string vName);

//_____________________________________________________________________________//
// 
//  Perform Fake studies 
//_____________________________________________________________________________//
void fakeStudies(int opt)
{
  
  init();
  
  if(opt==0 || opt==1){
    loadSamples(); 

    setSelection("SSEM",2,0,true);
    selectEvent(finalSel);
    
    bookHist();
    
    fillHist("SSEM");
    dumpEventYield("SSEM");
    saveHistograms("SSEM");
    plotHist("SSEM");
  }
  
  if(opt==0 || opt==2){
    //getFRSF_mcCorr("SSEM","Pt");
    getFRSF_iterCorr("SSEM","Pt");
  }
  
  if(opt==0 || opt==3){
    plotFRSF_mcCorr("SSEM","Pt");
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

  _type.push_back("data_");
  _type.push_back("bkg_");
 
  _lepKind.push_back("Tag_");
  _lepKind.push_back("Probe_");

  _lepType.push_back("Loose_");
  _lepType.push_back("Tight_");

  _lepCatg.push_back("Any_");
  _lepCatg.push_back("Fake_");
  _lepCatg.push_back("Real_");

  _selIter.push_back("mT40_");
  _selIter.push_back("mT100_");

  //
  //Histograms Var 
  //
  
  //Lepton vars
  _varLep.push_back(make_pair("Pt","l_pt"));
  _varLep.push_back(make_pair("Eta","l_eta"));
  _varLep.push_back(make_pair("org","l_org"));  //MC only var
  _varLep.push_back(make_pair("mT","mTl"));
  
  //Event vars
  _varEvt.push_back(make_pair("metRel","metrel"));
  _varEvt.push_back(make_pair("Met","met"));
  _varEvt.push_back(make_pair("dPhi_ll","dphi_ll"));
  
  
}

//_____________________________________________________________________________//
void loadSamples()
{
  string dir =  string(getenv("HISTOANA")) + "/SusyAna/" +  ver ;
  string subDir = "ToyNtOutputs/";

  if(dbg>0){
    cout << "Directory " << dir << endl;
    cout << "Loading ToyNt skim " << SR << endl << endl;
  } 

  //Load BKG
  _bkgFileNames.clear();
  _bkgFileNames.push_back(string("toyNt_HF" + SR + "_flep.root").c_str()); //FAKE
  _bkgFileNames.push_back(string("toyNt_WjetsAlpgenPythia" + SR + "_flep.root").c_str());
  _bkgFileNames.push_back(string("toyNt_Zjets_AlpgenPythia" + SR + "_flep.root").c_str());
  _bkgFileNames.push_back(string("toyNt_top_MCNLO_fake" + SR + "_flep.root").c_str());
  _bkgFileNames.push_back(string("toyNt_WZ_ZZ_Sherpa" + SR + "_flep.root").c_str());
  _bkgFileNames.push_back(string("toyNt_WW_Sherpa" + SR + "_flep.root").c_str());


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

}


//_____________________________________________________________________________//
void setSelection(string SRegion, int dilType, int probeFlavor, bool verbose)
{

  _vEvtCut.clear();
  _vTagCut.clear();
  _vProbeCut.clear();

  if(SRegion == "SSEM"){
    do_SSEM(dilType, probeFlavor, verbose);
  }

}

//_____________________________________________________________________________//
void do_SSEM(int dilType, int probeFlavor, bool verbose)
{
  _vEvtCut.push_back(TCut("pass_SSEM"));

  if(useQFLIP)
    _vEvtCut.push_back(TCut("(mcId<1 && !isOS) || \
(mcId>1 && ( (llType==1 && !isOS) || (llType!=1 && isOS)) )")); 
  else
    _vEvtCut.push_back(TCut("!isOS"));   //Use reco SS event for all - Change event weight accordingly

  _vEvtCut.push_back(TCut("llType==2"));   //implicit from selection

  _vEvtCut.push_back(TCut("SSEM_tagIdx>-1 && SSEM_probeIdx>-1"));   //For safety

  
  _vEvtCut.push_back(TCut("nCJets>=1 && nBJets==0 && nFJets==0"));   //Veto bjet and >=1 jets


  //muon is the tag
  _vTagCut.push_back(TCut("!l_isEle[SSEM_tagIdx]"));    
  
  //Electron is the probe    
  _vProbeCut.push_back(TCut("l_isEle[SSEM_probeIdx]"));  
  _vProbeCut.push_back(TCut("mTl[SSEM_probeIdx]<100"));  

  
  //Combine the cuts
  evtSel.Clear();
  tagSel.Clear();
  probeSel.Clear();
  finalSel.Clear();
  for(uint icut=0; icut<_vEvtCut.size(); icut++){
    evtSel += _vEvtCut[icut];
  }
  evtSel.Print();
  for(uint icut=0; icut<_vTagCut.size(); icut++){
    tagSel += _vTagCut[icut];
  }
  tagSel.Print();
  for(uint icut=0; icut<_vProbeCut.size(); icut++){
    probeSel += _vProbeCut[icut];
  }
  probeSel.Print();

  finalSel = evtSel + tagSel + probeSel;
  finalSel.Print();

}

//_____________________________________________________________________________//
void selectEvent(TCut _cut)
{
  cout << "Generating eventlists " << endl;
 
  for(uint ibkg=0; ibkg<N_MC; ibkg++){
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
void bookHist()
{
  cout << "Booking histograms " << endl;
  _histoDir->cd();
 
  for(uint iK=0; iK<_lepKind.size(); iK++){ //Tag-Probe
    for(uint iT=0; iT<_lepType.size(); iT++){ //Loose-Tight
      for(uint ibkg=0; ibkg<N_MC; ibkg++){
	for(uint iC=0; iC<_lepCatg.size(); iC++){ //Any-Fake-Real
	  _hBkgLep[ibkg][iC][iK][iT].clear();
	}
      }
      _hDataLep[iK][iT].clear();      
    }
  }
  for(uint ibkg=0; ibkg<N_MC; ibkg++){
    _hBkgEvt[ibkg].clear();
  }
  _hDataEvt.clear();      


  uint iTypeMax = _type.size();
  if(!showData) iTypeMax--;

  cout << "Booking histo for lepton vars: " << _varLep.size() << " variables " << endl;
  for(uint itype=0; itype<iTypeMax; itype++){ //Data or Bkg
    for(uint iK=0; iK<_lepKind.size(); iK++){ //Tag-Probe
      for(uint iT=0; iT<_lepType.size(); iT++){ //Loose-Tight
	for(uint ivar=0; ivar<_varLep.size(); ivar++){
	  TH1F* hh = NULL;
	  string prefix = _lepKind[iK] + _lepType[iT];
	  string varName  = _varLep[ivar].second;

	  if(itype==0){ //Data
	    string histName = _type[itype] + prefix + _varLep[ivar].first;
	    hh = histList(histName,varName);
	    if(dbg>5) cout << "Data book : " << histName << endl;
	    _hDataLep[iK][iT].push_back(hh);
	  }
	  else if(itype==1){ //Bkg
	    for(uint ibkg=0; ibkg<N_MC; ibkg++){
	      for(uint iC=0; iC<_lepCatg.size(); iC++){ //Any-Fake-Real
		string histName = _type[itype] + MCLabel[ibkg] + "_" + _lepCatg[iC] + prefix + _varLep[ivar].first;
		hh = histList(histName,varName);
		if(dbg>5) cout << "Bkg book : " << histName << endl;
		_hBkgLep[ibkg][iC][iK][iT].push_back(hh); 
	      }
	    }
	  }
	  
	}
      }
    }
  }

  cout << "Booking histo for event vars: " << _varEvt.size() << " variables " << endl;
  for(uint itype=0; itype<iTypeMax; itype++){ //Data or Bkg
    for(uint ivar=0; ivar<_varEvt.size(); ivar++){
      TH1F* hh = NULL;
      string varName  = _varEvt[ivar].second;
      
      if(itype==0){ //Data
	string histName = _type[itype] + _varEvt[ivar].first;
	hh = histList(histName,varName);
	if(dbg>5) cout << "Data book : " << histName << endl;
	_hDataEvt.push_back(hh);
      }
      else if(itype==1){ //Bkg
	for(uint ibkg=0; ibkg<N_MC; ibkg++){
	  string histName = _type[itype] + MCLabel[ibkg] + "_" + _varEvt[ivar].first;
	  hh = histList(histName,varName);
	  if(dbg>5) cout << "Bkg book : " << histName << endl;
	  _hBkgEvt[ibkg].push_back(hh); 
	}
      }

    }
    
  }

  //Book histo for iterative correction
  cout << "Booking histo for iterative correction: " << _varLep.size() << " variables " << endl;
  for(uint itype=0; itype<iTypeMax; itype++){ //Data or Bkg
    for(uint iS=0; iS<_selIter.size(); iS++){ //Selection criteria
      for(uint iT=0; iT<_lepType.size(); iT++){ //Loose-Tight
	for(uint ivar=0; ivar<_varLep.size(); ivar++){
	  TH1F* hh = NULL;
	  int iK= 1;//Probe only
	  string prefix = _lepKind[iK] + _lepType[iT] + _selIter[iS];
	  string varName  = _varLep[ivar].second;
	  
	  if(itype==0){ //Data
	    string histName = _type[itype] + prefix + _varLep[ivar].first;
	    hh = histList(histName,varName);
	    if(dbg>5) cout << "Data book : " << histName << endl;
	    _hDataProbe[iS][iT].push_back(hh);
	  }
	  
	  else if(itype==1){ //Bkg
	    for(uint ibkg=0; ibkg<N_MC; ibkg++){
	      string histName = _type[itype] + MCLabel[ibkg] + "_" + prefix + _varLep[ivar].first;
	      hh = histList(histName,varName);
	      if(dbg>5) cout << "Bkg book : " << histName << endl;
	      _hBkgProbe[ibkg][iS][iT].push_back(hh); 
	    }
	  }
	  
	}
      }
    }
  }



  
}

//_____________________________________________________________________________//
void fillHist(string sRegion)
{
  cout << "Fill histograms " << endl;
  TCut weight("");
  TCut kfrac(HF_kfrac.c_str());

  if(useQFLIP) weight += TCut("wqflip");
  else         weight += TCut("w");
  
  string probeIdx;
  string tagIdx;
  if(sRegion=="SSEM"){
    tagIdx     = "SSEM_tagIdx";
    probeIdx   = "SSEM_probeIdx";
  }
  
  //
  //Leptons kinematics
  //
  for(uint ivar=0; ivar<_varLep.size(); ivar++){
    for(uint iK=0; iK<_lepKind.size(); iK++){ //Tag-Probe
      for(uint iT=0; iT<_lepType.size(); iT++){ //Loose-Tight
	TCut isTight("");
	TCut isLoose("");
	string cmd;
	if( iK==0){
	  cmd = _varLep[ivar].second + "[" + tagIdx +"]";
	  isTight += TCut(string("l_isT[" + tagIdx +"]").c_str());
	  isLoose += TCut(string("!l_isT[" + tagIdx +"]").c_str());
	}
	if( iK==1){
	  cmd = _varLep[ivar].second + "[" + probeIdx +"]";
	  isTight += TCut(string("l_isT[" + probeIdx +"]").c_str());
	  isLoose += TCut(string("!l_isT[" + probeIdx +"]").c_str());
	}
    
	string prefix = _lepKind[iK] + _lepType[iT];
	TCut _sel("mTl[SSEM_probeIdx]<40");
	
	if(iT==0)      _sel += isLoose;
	else if(iT==1) _sel += isTight;

	_sel += TCut("metrel>=0");
	
	if(dbg>5){
	  cout << "fill histo data with selection iK " << iK << " iT " << iT << endl;
	  _sel.Print();
	}
	
	//Data
	string cmdData = cmd + ">>" + _type[0] + prefix + _varLep[ivar].first;
	if(dbg>5) cout << "data cmd " << cmdData << endl;
	if(_varLep[ivar].second != "l_org")
	   ntData->Draw(cmdData.c_str(),_sel,"goff");


	for(uint ibkg=0; ibkg<N_MC; ibkg++){
	  for(uint iC=0; iC<_lepCatg.size(); iC++){ //Any-Fake-Real
	    TCut lCatg("");
	    if(iC==1){
	      if(iK==0) lCatg += TCut(string("l_org[" + tagIdx +"]>0").c_str());
	      if(iK==1) lCatg += TCut(string("l_org[" + probeIdx +"]>0").c_str());
	    }
	    if(iC==2){
	      if(iK==0) lCatg += TCut(string("l_org[" + tagIdx +"]==0").c_str());
	      if(iK==1) lCatg += TCut(string("l_org[" + probeIdx +"]==0").c_str());
	    }
	    TCut _sel2 = _sel + lCatg;
	    
	    string cmdBkg = cmd + ">>" + _type[1] + MCLabel[ibkg] + "_" + _lepCatg[iC] + prefix + _varLep[ivar].first;
	    if(dbg>5) cout << "Bkg cmd " << cmdBkg << endl;
	    if(weightEvt){
	      if(ibkg==0) ntBkg[ibkg]->Draw(cmdBkg.c_str(),_sel2*weight*kfrac,"goff");
	      else   	  ntBkg[ibkg]->Draw(cmdBkg.c_str(),_sel2*weight,"goff");
	    }
	    else          ntBkg[ibkg]->Draw(cmdBkg.c_str(),_sel2 ,"goff");
	  }
	}
	
      }
    }
    
  }

  //
  //Event kinematics
  //
  for(uint ivar=0; ivar<_varEvt.size(); ivar++){
    string cmd = _varEvt[ivar].second;
    TCut _sel("metrel>=0 && mTl[SSEM_probeIdx]<40");
    
    //Data
    string cmdData = cmd + ">>" + _type[0] +  _varEvt[ivar].first;
    if(dbg>5) cout << "data cmd " << cmdData << endl;
    ntData->Draw(cmdData.c_str(),_sel,"goff");
    
    
    for(uint ibkg=0; ibkg<N_MC; ibkg++){
      string cmdBkg = cmd + ">>" + _type[1] + MCLabel[ibkg] + "_" + _varEvt[ivar].first;
      if(dbg>5) cout << "Bkg cmd " << cmdBkg << endl;
      if(weightEvt){
	if(ibkg==0) ntBkg[ibkg]->Draw(cmdBkg.c_str(),_sel*weight*kfrac,"goff"); //no weight for HF
	else        ntBkg[ibkg]->Draw(cmdBkg.c_str(),_sel*weight,"goff");
      }
      else          ntBkg[ibkg]->Draw(cmdBkg.c_str(),_sel,"goff");

    }
    
  }


  //
  //Probe - histo for iteration 
  //
  for(uint ivar=0; ivar<_varLep.size(); ivar++){
    int iK=1; //Probe histo pnly
    for(uint iT=0; iT<_lepType.size(); iT++){ //Loose-Tight
      TCut isTight("");
      TCut isLoose("");
      string cmd;
      
      cmd = _varLep[ivar].second + "[" + probeIdx +"]";
      isTight += TCut(string("l_isT[" + probeIdx +"]").c_str());
      isLoose += TCut(string("!l_isT[" + probeIdx +"]").c_str());

      for(uint iS=0; iS<_selIter.size(); iS++){ //Selection criteria
	string prefix = _lepKind[iK] + _lepType[iT] + _selIter[iS];
	TCut _sel("");
	if(iS==0)  _sel += TCut(string("mTl[SSEM_probeIdx]<40").c_str());
	if(iS==1)  _sel += TCut(string("mTl[SSEM_probeIdx]<100").c_str());
	
	if(iT==0)      _sel += isLoose;
	else if(iT==1) _sel += isTight;

	//Data
	string cmdData = cmd + ">>" + _type[0] + prefix + _varLep[ivar].first;
	if(dbg>5) cout << "data cmd " << cmdData << endl;
	if(_varLep[ivar].second != "l_org")
	  ntData->Draw(cmdData.c_str(),_sel,"goff");

	//Bkg
	for(uint ibkg=0; ibkg<N_MC; ibkg++){
	  string cmdBkg = cmd + ">>" + _type[1] + MCLabel[ibkg] + "_" + prefix + _varLep[ivar].first;
	  if(dbg>5) cout << "Bkg cmd " << cmdBkg << endl;
	  if(weightEvt){
	    if(ibkg==0) ntBkg[ibkg]->Draw(cmdBkg.c_str(),_sel*weight*kfrac,"goff");
	    else        ntBkg[ibkg]->Draw(cmdBkg.c_str(),_sel*weight,"goff");
	  }
	  else          ntBkg[ibkg]->Draw(cmdBkg.c_str(),_sel ,"goff");
	}
	
      }
      
    }
  }





}

//_____________________________________________________________________________//
void saveHistograms(string sRegion)
{

  _histoDir->cd();

  string fileName="fake_" + sRegion + "_histos.root";
  TFile* _f = new TFile(fileName.c_str(), "RECREATE");


  TIter next(_histoDir->GetList()); //Use GetListOfKeys when reading back file
  TObject* obj=0; 
  while((obj=next())) { 
    //std::cout << obj->GetName() << std::endl; 
    if (!obj->InheritsFrom("TH1")) continue; //skip anything that is not TH1
    obj->Write(); 
  } 
  
  _f->Close();
  
  cout << "Histograms saved " << _f->GetName() << endl;
  delete _f;



}


//_____________________________________________________________________________//
void dumpEventYield(string sRegion)
{
  int ivar=0; //use 1st event var declared
  string label = _varEvt[ivar].first;

  cout << "--------------------------------------" << endl;
  cout << "--------------------------------------" << endl;
  for(uint ibkg=0; ibkg<N_MC; ibkg++){
    Double_t statErrBkg;
    float nBkg = _hBkgEvt[ibkg][ivar]->IntegralAndError(0,-1,statErrBkg);
    cout << MCNames[ibkg] << "\t\t" << nBkg << " +/- " << statErrBkg << endl;
  }
  cout << "--------------------------------------" << endl;
  Double_t statErrData=0;
  float nData = _hDataEvt[ivar]->IntegralAndError(0,-1,statErrData);
  cout << "Data   \t\t" << nData << " +/- " << statErrData << endl;
  cout << "--------------------------------------" << endl;
  cout << "--------------------------------------" << endl;




}

//_____________________________________________________________________________//
void plotHist(string sRegion, bool logy)
{
  cout << "Plot histograms " << endl;
  
  THStack*                   _bkgStack = NULL;
  vector<TH1F*>              _hBkg4Stack;
  
  const float maxScaleLin=1.3;
  const float maxScaleLog=500;
  float scale=maxScaleLin;
  if(logy)  scale=maxScaleLog;
  
  
  string fileName="fake_" + sRegion + "_plots.root";
  TFile* _f = new TFile(fileName.c_str(), "RECREATE");


  //
  //Lepton kinematics
  //
  for(uint ivar=0; ivar<_varLep.size(); ivar++){
    for(uint iK=0; iK<_lepKind.size(); iK++){ //Tag-Probe
      for(uint iT=0; iT<_lepType.size(); iT++){ //Loose-Tight
	string prefix = _lepKind[iK] + _lepType[iT];
	string label = prefix + _varLep[ivar].first;
	int iC=0;//
	
	if(_hBkgLep[TOP][iC][iK][iT][ivar]->Integral()<=0.){
	  cout << _hBkgLep[TOP][iC][iK][iT][ivar]->GetTitle() << " is empty in range. Skipping. " << endl;
	  continue; 
	}

	TLegend*  _leg = new TLegend(0.6,0.6,0.8,0.8);
	_utils->legendSetting(_leg,0.03); 

	_hBkg4Stack.clear();
	for(uint ibkg=0; ibkg<N_MC; ibkg++){
	  _hBkg4Stack.push_back(_hBkgLep[ibkg][iC][iK][iT][ivar]);
	}
	_bkgStack = buildStack(_leg,&_hBkg4Stack,label);
	
	//Plot the canvas
	float min =  0;
	if(logy) min = 0.1;
	float max =  _utils->getMax(_hBkgTotal,_hDataLep[iK][iT][ivar])*scale;
	_hBkgTotal->SetMaximum(max); _hBkgTotal->SetMinimum(min);
	_hBkgTotal->GetYaxis()->SetRangeUser(min,max);
	
	 TCanvas* _c1  = _utils->myCanvas(label.c_str());
	 _utils->myDraw1d(_hBkgTotal,_c1,1,"hist",logy,C_ALL,false,20);
	 _bkgStack->Draw("histsame");
	 _bkgStack->GetXaxis()->SetTitle(_hBkgLep[0][iC][iK][iT][ivar]->GetXaxis()->GetTitle());
	 _bkgStack->GetYaxis()->SetTitle(_hBkgLep[0][iC][iK][iT][ivar]->GetYaxis()->GetTitle());
	 _bkgStack->GetYaxis()->SetTitleSize(0.05);
	 _bkgStack->GetYaxis()->SetTitleOffset(1.2);
	 _bkgStack->GetYaxis()->SetLabelSize(0.05);
	 _utils->myDraw1d(_hBkgTotal,_c1,1,"histsame",logy,C_ALL,false,20);
	 
	 if(showData){//Data
	   _utils->myDraw1d(_hDataLep[iK][iT][ivar],_c1,1,"esame",logy,kBlack,false,20);
	   _leg->AddEntry(_hDataLep[iK][iT][ivar],"Data", "p");
	 }
	 _leg->Draw();

	 _f->cd();
	 _c1->Write();

      }
    }
  }


  //
  //Event kinematics
  //
  for(uint ivar=0; ivar<_varEvt.size(); ivar++){
    string label = _varEvt[ivar].first;

    if(_hBkgEvt[TOP][ivar]->Integral()<=0.){
      cout << _hBkgEvt[TOP][ivar]->GetTitle() << " is empty in range. Skipping. " << endl;
      continue; 
    }

    TLegend*  _leg = new TLegend(0.6,0.6,0.8,0.8);
    _utils->legendSetting(_leg,0.03); 
    
    _hBkg4Stack.clear();
    for(uint ibkg=0; ibkg<N_MC; ibkg++){
      _hBkg4Stack.push_back(_hBkgEvt[ibkg][ivar]);
    }
    _bkgStack = buildStack(_leg,&_hBkg4Stack,label);

    //Plot the canvas
    float min =  0;
    if(logy) min = 0.1;
    float max =  _utils->getMax(_hBkgTotal,_hDataEvt[ivar])*scale;
    _hBkgTotal->SetMaximum(max); _hBkgTotal->SetMinimum(min);
    _hBkgTotal->GetYaxis()->SetRangeUser(min,max);
    
    TCanvas* _c1  = _utils->myCanvas(label.c_str());
    _utils->myDraw1d(_hBkgTotal,_c1,1,"hist",logy,C_ALL,false,20);
    _bkgStack->Draw("histsame");
    _bkgStack->GetXaxis()->SetTitle(_hBkgEvt[0][ivar]->GetXaxis()->GetTitle());
    _bkgStack->GetYaxis()->SetTitle(_hBkgEvt[0][ivar]->GetYaxis()->GetTitle());
    _bkgStack->GetYaxis()->SetTitleSize(0.05);
    _bkgStack->GetYaxis()->SetTitleOffset(1.2);
    _bkgStack->GetYaxis()->SetLabelSize(0.05);
    _utils->myDraw1d(_hBkgTotal,_c1,1,"histsame",logy,C_ALL,false,20);
	 
    if(showData){//Data
      _utils->myDraw1d(_hDataEvt[ivar],_c1,1,"esame",logy,kBlack,false,20);
      _leg->AddEntry(_hDataEvt[ivar],"Data", "p");
    }
    _leg->Draw();
    
    _f->cd();
    _c1->Write();
    
  }
  

  _f->Close();
  _histoDir->cd();


}

//_____________________________________________________________________________//
void getFRSF_mcCorr(string sRegion, string vName)
{
  string fileName="fake_" + sRegion + "_histos.root";
  TFile* _f = new TFile(fileName.c_str(), "READ");
  cout << "Opening histo file " << fileName << endl;

  string histName; 
  int ivar=0;
  if(vName == "Pt")  ivar=0;
  else if(vName == "Eta") ivar=1;
  else {
    cout << "Could not find histo for requested var. Aborting." << endl;
    abort();
  }
  string varName  = _varLep[ivar].first;

  int iK = 1; //Probe

  //
  // Retrieve data histo 
  // Compute FR
  //
  TH1F* h_data_loose = NULL;
  TH1F* h_data_tight = NULL;

  for(uint iT=0; iT<_lepType.size(); iT++){ //Loose-Tight
    string prefix = _type[0] +_lepKind[iK] + _lepType[iT];
    histName =  prefix + varName;
    if(iT==0) h_data_loose = (TH1F*) _f->Get(histName.c_str());
    if(iT==1) h_data_tight = (TH1F*) _f->Get(histName.c_str());
  }
  cout << "Got histos " << endl;

  TH1F* h_data_loose_raw = (TH1F*) h_data_loose->Clone();
  h_data_loose_raw->SetTitle(string(string(h_data_loose->GetTitle()) + "_noCorr").c_str());
  h_data_loose_raw->SetName(string(string(h_data_loose->GetName()) + "_noCorr").c_str());
  TH1F* h_data_tight_raw = (TH1F*) h_data_tight->Clone();
  h_data_tight_raw->SetTitle(string(string(h_data_tight->GetTitle()) + "_noCorr").c_str());
  h_data_tight_raw->SetName(string(string(h_data_tight->GetName()) + "_noCorr").c_str());



  //
  // Get the REAL SM from MC and subtract to the data histo
  //
  //Add bkg subtraction from PR lepton.
  int lepSource=2; //real at truth level

  TH1F* h_mc_real_loose = (TH1F*) h_data_loose->Clone();
  h_mc_real_loose->Reset();
  histName = _type[1] + "MC_" + _lepKind[iK] + _lepCatg[lepSource] + _lepKind[1] + _lepType[0] + varName;
  h_mc_real_loose->SetTitle(histName.c_str());
  h_mc_real_loose->SetName(histName.c_str());

  TH1F* h_mc_real_tight = (TH1F*) h_data_tight->Clone();;
  h_mc_real_tight->Reset();
  histName = _type[1] + "MC_" + _lepKind[iK] + _lepCatg[lepSource] + _lepKind[1] + _lepType[1] + varName;
  h_mc_real_tight->SetTitle(histName.c_str());
  h_mc_real_tight->SetName(histName.c_str());

  for(uint iT=0; iT<_lepType.size(); iT++){ //Loose-Tight
    string prefix =  _lepKind[iK] + _lepType[iT];
    for(uint ibkg=0; ibkg<N_MC; ibkg++){
      histName =  _type[1] + MCLabel[ibkg] + "_" + _lepCatg[lepSource] +  prefix + varName;
      TH1F* htmp = (TH1F*) _f->Get(histName.c_str());
      if(iT==0)	h_mc_real_loose->Add(htmp); 
      if(iT==1) h_mc_real_tight->Add(htmp);
    }
  }



  h_data_loose->Add(h_mc_real_loose,-1);
  h_data_tight->Add(h_mc_real_tight,-1);


  TH1F* h_FR_data = (TH1F*)  h_data_loose->Clone();
  h_FR_data->Reset();
  histName = _type[0] + "FR_" + _lepKind[iK] + varName;
  h_FR_data->SetTitle(histName.c_str());
  h_FR_data->SetName(histName.c_str());
  
  
  h_FR_data->Divide(h_data_tight,h_data_loose,1,1,"B");



  //
  // Retrieve MC fake histo 
  // Compute FR
  //
  lepSource=1; //Fake at truth level

  TH1F* h_mc_loose = (TH1F*) h_data_loose->Clone();
  h_mc_loose->Reset();
  histName = _type[1] + "MC_" + _lepKind[iK] + _lepCatg[lepSource] + _lepKind[1] + _lepType[0] + varName;
  h_mc_loose->SetTitle(histName.c_str());
  h_mc_loose->SetName(histName.c_str());

  TH1F* h_mc_tight = (TH1F*) h_data_tight->Clone();;
  h_mc_tight->Reset();
  histName = _type[1] + "MC_" + _lepKind[iK] + _lepCatg[lepSource] + _lepKind[1] + _lepType[1] + varName;
  h_mc_tight->SetTitle(histName.c_str());
  h_mc_tight->SetName(histName.c_str());

  for(uint iT=0; iT<_lepType.size(); iT++){ //Loose-Tight
    string prefix =  _lepKind[iK] + _lepType[iT];
    for(uint ibkg=0; ibkg<N_MC; ibkg++){
      histName =  _type[1] + MCLabel[ibkg] + "_" + _lepCatg[lepSource] +  prefix + varName;
      TH1F* htmp = (TH1F*) _f->Get(histName.c_str());
      if(iT==0)	h_mc_loose->Add(htmp); 
      if(iT==1) h_mc_tight->Add(htmp);
    }
  }


  TH1F* h_FR_mc = (TH1F*)  h_mc_loose->Clone();
  h_FR_mc->Reset();
  histName = _type[1] + "FR_" + _lepKind[iK] + varName;
  h_FR_mc->SetTitle(histName.c_str());
  h_FR_mc->SetName(histName.c_str());

  h_FR_mc->Divide(h_mc_tight,h_mc_loose,1,1,"B");
  //h_FR_mc->Draw();


  //
  // Compute the SF Data/MC
  //
  TH1F* h_SF = (TH1F*)  h_mc_loose->Clone();
  h_SF->Reset();
  histName = "SF_" + _lepKind[iK] + varName;
  h_SF->SetTitle(histName.c_str());
  h_SF->SetName(histName.c_str());

  h_SF->Divide(h_FR_data, h_FR_mc,1,1,"B");
  h_SF->Draw();



  //
  // Save histos to file
  //
  fileName="fake_" + sRegion + "_FR_SF_" + vName +".root";
  TFile* _ff = new TFile(fileName.c_str(), "RECREATE");

  h_data_loose->Write();
  h_data_tight->Write();
  h_data_loose_raw->Write();
  h_data_tight_raw->Write();
  h_mc_real_loose->Write();
  h_mc_real_tight->Write(); 
  h_FR_data->Write();

  h_mc_loose->Write();
  h_mc_tight->Write(); 
  h_FR_mc->Write();

  h_SF->Write();
  
  cout << "FR/SF histograms saved " << _ff->GetName() << endl;

  _ff->Close();
  _f->Close();

  delete _f;
  delete _ff;

}

//_____________________________________________________________________________//
void plotFRSF_mcCorr(string sRegion, string vName, bool logy){

  const float maxScaleLin=1.3;
  const float maxScaleLog=500;
  float scale=maxScaleLin;
  if(logy)  scale=maxScaleLog;

  string fileName="fake_" + sRegion + "_FR_SF_" + vName +".root";
  TFile* _f = new TFile(fileName.c_str(), "READ");

  //
  //Data plots
  //

  TCanvas* _c1  = _utils->myCanvas("Data Loose before-after Bkg subtraction");
  TLegend*  _leg1 = new TLegend(0.2,0.2,0.6,0.3);
  _utils->legendSetting(_leg1,0.03); 
  TH1F* _hData_L1 = (TH1F*) _f->Get(string("data_Probe_Loose_" + vName + "_noCorr").c_str());
  TH1F* _hData_L2 = (TH1F*) _f->Get(string("data_Probe_Loose_" + vName).c_str());
  _utils->myDraw1d(_hData_L1,_c1,1,"e",logy,kBlue,false,20);
  _utils->myDraw1d(_hData_L2,_c1,1,"esame",logy,kRed,false,20);
  _leg1->AddEntry(_hData_L1,"Data Loose lepton (raw)", "p");
  _leg1->AddEntry(_hData_L2,"Data Loose lepton (corrected)", "p");
  _leg1->Draw();


  TCanvas* _c2  = _utils->myCanvas("Data Tight before-after Bkg subtraction");
  TLegend*  _leg2 = new TLegend(0.2,0.2,0.6,0.3);
  _utils->legendSetting(_leg2,0.03); 
  TH1F* _hData_T1 = (TH1F*) _f->Get(string("data_Probe_Tight_" + vName + "_noCorr").c_str());
  TH1F* _hData_T2 = (TH1F*) _f->Get(string("data_Probe_Tight_" + vName).c_str());
  _utils->myDraw1d(_hData_T1,_c2,1,"e",logy,kBlue,false,20);
  _utils->myDraw1d(_hData_T2,_c2,1,"esame",logy,kRed,false,20);
  _leg2->AddEntry(_hData_T1,"Data Tight lepton (raw)", "p");
  _leg2->AddEntry(_hData_T2,"Data Tight lepton (corrected)", "p");
  _leg2->Draw();


  //
  // MC plots
  //
  TCanvas* _c3  = _utils->myCanvas("MC Loose Tight");
  TLegend*  _leg3 = new TLegend(0.2,0.2,0.6,0.3);
  _utils->legendSetting(_leg3,0.03); 
  TH1F* _hMC_L = (TH1F*) _f->Get(string("bkg_MC_Probe_Fake_Probe_Loose_"+ vName).c_str());
  TH1F* _hMC_T = (TH1F*) _f->Get(string("bkg_MC_Probe_Fake_Probe_Tight_" + vName).c_str());
  _utils->myDraw1d(_hMC_L,_c3,1,"e",logy,kBlue,false,20);
  _utils->myDraw1d(_hMC_T,_c3,1,"esame",logy,kRed,false,20);
  _leg3->AddEntry(_hMC_L,"MC Loose lepton", "p");
  _leg3->AddEntry(_hMC_T,"MC Tight lepton", "p");
  _leg3->Draw();

  
  //
  // FR & SF plots
  //
  TCanvas* _c4   = _utils->myCanvas("FR and SF");
  TPad*    _pTop = new TPad("pTop","pTop",0,0.3,1,1);
  TPad*    _pBot = new TPad("pBot","pBot",0,0,1,0.3);

  TVirtualPad* _tv = _c4->cd();
  _pTop->SetTopMargin(0.08);
  _pTop->SetBottomMargin(0.01);
  _pTop->SetRightMargin(0.05);
  _pTop->SetLeftMargin(0.15);
  _pTop->SetNumber(1);
  _pBot->SetTopMargin(0.07);
  _pBot->SetBottomMargin(0.4);
  _pBot->SetRightMargin(0.05);
  _pBot->SetLeftMargin(0.15);
  _pBot->SetNumber(2);
 
  _tv->cd();
  _pBot->Draw();
  _pTop->Draw();

  //Top canvas
  _pTop->cd();
  TLegend*  _leg4 = new TLegend(0.6,0.6,0.8,0.8);
  _utils->legendSetting(_leg4,0.03); 
  TH1F* _hData_FR = (TH1F*) _f->Get(string("data_FR_Probe_" + vName).c_str());
  TH1F* _hMC_FR = (TH1F*) _f->Get(string("bkg_FR_Probe_" + vName).c_str());
  _utils->myDraw1d(_hData_FR,_pTop,1,"e",false,kBlue,false,20);
  _utils->myDraw1d(_hMC_FR,_pTop,1,"esame",false,kRed,false,20);
  _hData_FR->GetYaxis()->SetRangeUser(0,0.8);
  _hData_FR->GetYaxis()->SetTitle("#epsilon(T|L)");
  _leg4->AddEntry(_hData_FR,"Data FR", "p");
  _leg4->AddEntry(_hMC_FR,"MC FR", "p");
  _leg4->Draw();
  _pTop->RedrawAxis();
  _pTop->Update();    
  
  //Bottom Canvas
  _pBot->cd();
  TH1F* _h_SF = (TH1F*) _f->Get(string("SF_Probe_" + vName).c_str());
  _h_SF->GetYaxis()->SetRangeUser(0,2.5);
  _h_SF->GetYaxis()->SetTitle("SF");
  _h_SF->GetXaxis()->SetTitleSize(0.1);
  _h_SF->GetXaxis()->SetTitleOffset(1.2);
  _h_SF->GetXaxis()->SetLabelSize(0.1);
  _h_SF->GetYaxis()->SetTitleSize(0.1);
  _h_SF->GetYaxis()->SetTitleOffset(0.55);
  _h_SF->GetYaxis()->SetLabelSize(0.1);
  _utils->myDraw1d(_h_SF,_pBot,1,"e",false,kBlack,false,20);
  TLine* _line = new TLine( _h_SF->GetBinCenter(1)-_h_SF->GetBinWidth(1)/2,1,
			   _h_SF->GetBinCenter(_h_SF->GetNbinsX())+
			   _h_SF->GetBinWidth(_h_SF->GetNbinsX())/2,1);
  _line->SetLineStyle(7);
  _line->SetLineWidth(1);
  _line->SetLineColor(kRed);
  _line->Draw();
  
  _pBot->SetGridy();
  _pBot->Modified();
  _pBot->Update();



}


//_____________________________________________________________________________//
void getFRSF_iterCorr(string sRegion, string vName)
{

  //https://github.com/gerbaudo/SusyTest0/blob/44055a89c17a9fa33aed9beaab46e0da0483877c/Root/IterativeFakeCorrection.cxx
  string fileName="fake_" + sRegion + "_histos.root";
  TFile* _f = new TFile(fileName.c_str(), "READ");
  cout << "Opening histo file " << fileName << endl;
  cout << "Got histos " << endl;


  string histName; 
  int ivar=0;
  if(vName == "Pt")  ivar=0;
  else if(vName == "Eta") ivar=1;
  else {
    cout << "Could not find histo for requested var. Aborting." << endl;
    abort();
  }
  string varName  = _varLep[ivar].first;

  int iK = 1; //Probe

  //
  // Retrieve data/MC histo 
  //
  TH1F* h_data_loose[2]; //[low,high mT]
  TH1F* h_data_tight[2];

  TH1F* h_mc_loose[2]; //[low,high mT]
  TH1F* h_mc_tight[2];

  //
  //Data histos
  //
  for(uint iT=0; iT<_lepType.size(); iT++){ //Loose-Tight
    for(uint iS=0; iS<_selIter.size(); iS++){ //Selection criteria
      string prefix = _type[0] +_lepKind[iK] + _lepType[iT] + _selIter[iS];
      histName =  prefix + varName;
      cout << "Data " << histName << endl;
      if(iT==0) h_data_loose[iS] = (TH1F*) _f->Get(histName.c_str());
      if(iT==1) h_data_tight[iS] = (TH1F*) _f->Get(histName.c_str());
    }
  }
  
  //
  //MC histos
  // MC should not include HF
  //
  for(uint iS=0; iS<2; iS++){ //mt40 mt100
    h_mc_loose[iS] = (TH1F*) h_data_loose[iS]->Clone();
    h_mc_loose[iS]->Reset();
    h_mc_loose[iS]->SetTitle(string("MC_Probe_Loose_"+_selIter[iS] + varName).c_str());
    h_mc_loose[iS]->SetName(string("MC_Probe_Loose_"+_selIter[iS] + varName).c_str());

    h_mc_tight[iS] = (TH1F*) h_data_loose[iS]->Clone();
    h_mc_tight[iS]->Reset();
    h_mc_tight[iS]->SetTitle(string("MC_Probe_Tight_"+_selIter[iS] + varName).c_str());
    h_mc_tight[iS]->SetName(string("MC_Probe_Tight_"+_selIter[iS] + varName).c_str());
  }
  
  for(uint iT=0; iT<_lepType.size(); iT++){ //Loose-Tight
    for(uint ibkg=1; ibkg<N_MC; ibkg++){ //BKG  w/o HF
      for(uint iS=0; iS<2; iS++){ //mt40 mt100
	string prefix = _lepKind[iK] + _lepType[iT] + _selIter[iS];
	histName = _type[1] + MCLabel[ibkg] + "_" + prefix + varName;
	cout << "MC " << histName << endl;

	TH1F* htmp = (TH1F*) _f->Get(histName.c_str());
	if(iT==0) h_mc_loose[iS]->Add(htmp);
	if(iT==1) h_mc_tight[iS]->Add(htmp);
      }
    }
  }

  //
  //Do the iterative correction
  //
  static const unsigned int nIter =10;

  //Iter 0 - set to data
  TH1F* corrected[2];
  
  corrected[0] = (TH1F*) h_data_tight[0]->Clone("Corrected_num");//Tight - low Mt
  corrected[1] = (TH1F*) h_data_loose[0]->Clone("Corrected_den");//Loose - low Mt
  for(uint iIter=0; iIter<nIter; iIter++){
    cout << "--------------------------------" << endl;
    cout << "Iteration: "<< iIter << endl;

    // Dump the rate
    for(int bin=1; bin<=corrected[0]->GetNbinsX(); ++bin){
      float num = corrected[0]->GetBinContent(bin);
      float den = corrected[1]->GetBinContent(bin);
      cout<<"Bin: "<<bin<<" num: "<<num<<" den: "<<den<<" rate: "<<num/den<<endl;
    }
    
    // Make temporary rate
    TH1F* rate = (TH1F*) corrected[0]->Clone("rate");
    rate->Reset();
    rate->Divide(corrected[0], corrected[1],1,1,"B");
    
    vector<double> C_T = getC(rate, h_data_tight[1], h_data_loose[1], h_mc_tight[1], true);
    vector<double> C_L = getC(rate, h_data_tight[1], h_data_loose[1], h_mc_loose[1], false);

    // Correct the rates
    correctRate(corrected[0], h_data_tight[0], h_mc_tight[0], C_T);
    correctRate(corrected[1], h_data_loose[0], h_mc_loose[0], C_L);
    
    delete rate;
    
  }

  //Data FR
  TH1F* h_FR_data = (TH1F*)  h_data_loose[0]->Clone();
  h_FR_data->Reset();
  histName = _type[0] + "FR_" + _lepKind[iK] + varName;
  h_FR_data->SetTitle(histName.c_str());
  h_FR_data->SetName(histName.c_str());
  h_FR_data->Divide(h_data_tight[0],h_data_loose[1],1,1,"B");

  //MC FR
  TH1F* h_FR_mc = (TH1F*)  h_mc_loose[0]->Clone();
  h_FR_mc->Reset();
  histName = _type[1] + "FR_" + _lepKind[iK] + varName;
  h_FR_mc->SetTitle(histName.c_str());
  h_FR_mc->SetName(histName.c_str());

  h_FR_mc->Divide(h_mc_tight[0],h_mc_loose[0],1,1,"B");

  //
  // Compute the SF Data/MC
  //
  TH1F* h_SF = (TH1F*)  h_mc_loose[0]->Clone();
  h_SF->Reset();
  histName = "SF_" + _lepKind[iK] + varName;
  h_SF->SetTitle(histName.c_str());
  h_SF->SetName(histName.c_str());

  h_SF->Divide(h_FR_data, h_FR_mc,1,1,"B");
  h_SF->Draw();


  //
  // Save histos to file
  //
  fileName="fake_" + sRegion + "_FR_SF_iterCorr_" + vName +".root";
  TFile* _ff = new TFile(fileName.c_str(), "RECREATE");

  
  for(uint iS=0; iS<2; iS++){
    h_data_loose[iS]->Write();
    h_data_tight[iS]->Write();

    h_mc_loose[iS]->Write();
    h_mc_tight[iS]->Write();
  }

  h_FR_data->Write();
  h_FR_mc->Write();
  h_SF->Write();


  cout << "FR/SF histograms saved " << _ff->GetName() << endl;

  _ff->Close();
  _f->Close();

  delete _f;
  delete _ff;



}
//-----------------------------------------------//
// Correct rate
//-----------------------------------------------//
void correctRate(TH1F* &rate, TH1F* data, TH1F* mc, vector<double> corrections)
{
  // Loop and correct each bin.
  int nbins = rate->GetNbinsX();
  for(int bin=1; bin<=nbins; ++bin){
    float d_bc = data->GetBinContent(bin);
    float d_be = data->GetBinError(bin);
    float m_bc = mc->GetBinContent(bin);
    float m_be = mc->GetBinError(bin);
    float corr = corrections.at(bin-1);
    cout<<"\t\tbin: "<<bin<<" data: "<<d_bc<<" mc: "<<m_bc<<" Corrected: "<<d_bc - corr * m_bc<<endl;
    rate->SetBinContent(bin, d_bc - corr * m_bc);
    rate->SetBinError(bin, sqrt(pow(d_be,2) + pow(corr*m_be,2)) );
  }

}

//-----------------------------------------------//
// Get correction
//-----------------------------------------------//
vector<double> getC(TH1F* rate, TH1F* data_num, TH1F* data_den, TH1F* mc, bool tight)
{
  // Correction factor:
  // C = (N^(data,high) - N^(fake pred, high))/N^(MC,high)

  cout<<"Getting corrections for tight? "<<tight<<endl;
  vector<double> corrections;

  int nbins = data_num->GetNbinsX();
  for(int bin=1; bin<=nbins; ++bin){
    // Set N loose and tight for 1-D MM
    float nLoose = data_den->GetBinContent(bin);
    float nTight = data_num->GetBinContent(bin); //tight ? data_num->GetBinContent(bin) : 0.;

    // Get r and f
    float r = 1;//m_real->GetBinContent(bin);  //Real Eff --- need the histos !!!!
    float f = rate->GetBinContent(bin);
    //f = r-f <= 0.3 ? r-0.3 : f;

    // Set the values for C
    double nData = tight ? nTight : nLoose;
    double nMC   = mc->GetBinContent(bin);
    double nFake = getFake(r, f, nLoose, nTight, tight);
    cout<<"\tCorrection: "<<nData<<" "<<nFake<<" "<<nMC<<" === "<<(nData-nFake)/nMC<<endl;
    corrections.push_back( (nData - nFake)/nMC );

  }// end loop over bins

  return corrections;

}
//-----------------------------------------------//
// Get the fake contribution
//-----------------------------------------------//
double getFake(float r, float f, float nL, float nT, bool tight)
{
  cout<<"\t\t\tr: "<<r<<" f: "<<f<<" nL: "<<nL<<" nT: "<<nT<<endl;
  if(tight) return f/(r-f) * (nL*r -nT);
  else      return 1/(r-f) * (nL*r -nT);
}


//_____________________________________________________________________________//
void plotFRSF_iterCorr(string sRegion, string vName, bool logy){

  const float maxScaleLin=1.3;
  const float maxScaleLog=500;
  float scale=maxScaleLin;
  if(logy)  scale=maxScaleLog;


}




//_____________________________________________________________________________//
//_____________________________________________________________________________//
TH1F* histList(string hName, string vName)
{
  string sY="Entries";
  TH1F* h = NULL;

  const Double_t Ptbins[]           = {10,20,35,100};
  const Double_t CoarsePtbins[]     = {10,20,35,100};
  const Double_t Etabins[]          = {0,1.37,1.52,2.1,2.5};
  const Double_t CoarseEtabins[]    = {0,1.37,2.5};
  const Double_t Metbins[]          = {0, 20, 30, 40, 50, 60, 100, 200};
  const Double_t Jetbins[]          = {-0.5, 0.5, 1.5, 2.5, 3.5};
  const int   nPtbins            = sizeof(Ptbins)           / sizeof(Ptbins[0])       - 1;
  const int   nCoarsePtbins      = sizeof(CoarsePtbins)     / sizeof(CoarsePtbins[0]) - 1;
  const int   nEtabins           = sizeof(Etabins)          / sizeof(Etabins[0])          - 1;
  const int   nCoarseEtabins     = sizeof(CoarseEtabins)    / sizeof(CoarseEtabins[0])    - 1;
  const int   nMetbins           = sizeof(Metbins)          / sizeof(Metbins[0])          - 1;
  const int   nJetbins           = sizeof(Jetbins)          / sizeof(Jetbins[0])          - 1;


  //Leptons 
  if(vName=="l_pt")      h = myBook(hName.c_str(),nCoarsePtbins,CoarsePtbins,"p_{T} [GeV]",sY.c_str());
    //h = myBook(hName.c_str(),20,0,100,"p_{T} [GeV]",sY.c_str());
  if(vName=="l_eta")     h = myBook(hName.c_str(),nCoarseEtabins,CoarseEtabins,"#eta",sY.c_str());
    //h = myBook(hName.c_str(),25,-2.5,2.5,"#eta",sY.c_str());
  if(vName=="l_org"){
    h = myBook(hName.c_str(),5,-0.5,4.5,"Origin",sY.c_str());
    for(int ibin=1; ibin<=h->GetNbinsX(); ibin++){
      h->GetXaxis()->SetBinLabel(ibin,LepOrg[ibin-1].c_str());
    }
  }
  if(vName == "mTl")     h = myBook(hName.c_str(),20,0,100,"m_{T} [GeV]",sY.c_str());

  if(vName == "metrel")  h = myBook(hName.c_str(),30,0,300,"E_{T}^{miss,rel} [GeV]",sY.c_str());
  if(vName == "met")     h = myBook(hName.c_str(),30,0,300,"E_{T}^{miss} [GeV]",sY.c_str());
  if(vName == "dphi_ll") h = myBook(hName.c_str(),32,0,3.2,"d#phi_{ll} [rad]",sY.c_str()); 

  if(h==NULL){
    cout << "Booking histo for var " << vName << " not found. Aborting" << endl;
    abort();
  }

  return h;
}


//_____________________________________________________________________________//
TH1F* myBook(string hName, int nbin, float xlow, float xhigh, string xaxis, string yaxis)
{
  return _utils->myTH1F(hName.c_str(),hName.c_str(),
			nbin,xlow, xhigh,
			xaxis.c_str(),yaxis.c_str());
}

//_____________________________________________________________________________//
TH1F* myBook(string hName, int nbin, const Double_t* nX, string xaxis, string yaxis)
{
  return _utils->myTH1F(hName.c_str(),hName.c_str(),
			nbin,nX,
			xaxis.c_str(),yaxis.c_str());
}

//_____________________________________________________________________________//
THStack* buildStack(TLegend* _l, vector<TH1F*>* hList, string vName)
{
  _histoDir->cd();

  THStack* hStack = new THStack(string("stackBkg_"+vName).c_str(),
				string("stackBkg_"+vName).c_str());
  
  _hBkgTotal = (TH1F*) hList->at(0)->Clone();
  _hBkgTotal->Reset();
  _hBkgTotal->SetTitle(string("Stack_"+vName).c_str());
  _hBkgTotal->SetName(string("Stack_"+vName).c_str());

  for(uint ibkg=0; ibkg<hList->size(); ibkg++){
    int icol=0;
    if(ibkg==0) icol = C_HF;
    if(ibkg==1) icol = C_Wjets;
    if(ibkg==2) icol = C_Zjets;
    if(ibkg==3) icol = C_TOP;
    if(ibkg==4) icol = C_ZV;
    if(ibkg==5) icol = C_WW;
    _utils->addToTHStack(hStack,hList->at(ibkg),icol,"HIST",_l, MCNames[ibkg].c_str());
    _hBkgTotal->Add(hList->at(ibkg),1);
  }
  return hStack;

}
