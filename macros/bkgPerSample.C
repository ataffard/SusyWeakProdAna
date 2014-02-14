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
#include "SusyNtuple/TGuiUtils.h"



//_____________________________________________________________________________//
static const string ver       = "histos_011114_21fb_n0145_DD_WH_v1/ToyNtOutputs/";  //ToyNt filtered SS, 3rd lep veto, B/F-veto >=1 C20 jet
static const string SR        = "_WH_optimSRSS";
static const int    dbg       = 0;

TGuiUtils*      _utils;
vector<TChain*> _vNt;
vector<string>  _vSampleNames;
vector<TCut>    _vCut;
vector<TH1F*>   _vHCut;


//_____________________________________________________________________________//
void init();
void selectSamples();
void getNtuples();
void setSelection();
void getYield();

TH1F* myBook(string name, int nbin, float  xlow, float  xhigh, string xaxis, string yaxis);


//_____________________________________________________________________________//
//_____________________________________________________________________________//
int main(int argc, char *argv[]){
  init();
  selectSamples();
  getNtuples();
  setSelection();
  getYield();
}

//_____________________________________________________________________________//
void init()
{
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
}

//_____________________________________________________________________________//
void selectSamples()
{
  _vSampleNames.clear();

  //WZ_PowHeg.txt
  _vSampleNames.push_back("129477");
  _vSampleNames.push_back("129478");
  _vSampleNames.push_back("129479");
  _vSampleNames.push_back("129480");
  _vSampleNames.push_back("129481");
  _vSampleNames.push_back("129482");
  _vSampleNames.push_back("129483");
  _vSampleNames.push_back("129484");
  _vSampleNames.push_back("129485");
  _vSampleNames.push_back("129486");
  _vSampleNames.push_back("129487");
  _vSampleNames.push_back("129488");
  _vSampleNames.push_back("129489");
  _vSampleNames.push_back("129490");
  _vSampleNames.push_back("129491");
  _vSampleNames.push_back("129492");
  _vSampleNames.push_back("129493");
  _vSampleNames.push_back("129494");
  _vSampleNames.push_back("157814");
  _vSampleNames.push_back("157815");
  _vSampleNames.push_back("157816");
  _vSampleNames.push_back("167007");
  
}

//_____________________________________________________________________________//
void getNtuples()
{
  string dir =  string(getenv("HISTOANA")) + "/SusyAna/" +  ver ;

  for(uint iS=0; iS<_vSampleNames.size(); iS++){
    TChain* _nt = new TChain("ToyNt",_vSampleNames[iS].c_str());
    _nt->Add( string(dir+_vSampleNames[iS]+SR+".root").c_str());
    _vNt.push_back(_nt);
    if(dbg>0) cout << _vSampleNames[iS] << " nEntries " << _nt->GetEntries() << endl;
  }

}

//_____________________________________________________________________________//
void setSelection()
{
  _vCut.clear();


  
  TCut MM_1j("llType==1 && nCJets==1 && l_pt[0]>30 && l_pt[1]>20 && abs(deta_ll)<1.5 && TMath::Max(mTl[0],mTl[1])>100 && mEff>200 && mlj<90");
  _vCut.push_back(MM_1j);

  TCut MM_23j("llType==1 && nCJets>1 && nCJets<4 && l_pt[0]>30 && l_pt[1]>30 && abs(deta_ll)<1.5 && mEff>220 && mljj<120");
  _vCut.push_back(MM_23j);


  TCut EM_1j("llType==2 && nCJets==1 && l_pt[0]>30 && l_pt[1]>30 && abs(deta_ll)<1.5 && TMath::Max(mTl[0],mTl[1])>110 && mlj<90 && mWWT>110");
  _vCut.push_back(EM_1j);

  TCut EM_23j("llType==2 && nCJets>1 && nCJets<4 && l_pt[0]>30 && l_pt[1]>30 && abs(deta_ll)<1.5 && mljj<120 && mWWT>110");
  _vCut.push_back(EM_23j);


}

//_____________________________________________________________________________//
void getYield()
{
  _vHCut.clear();
  for(uint iC=0; iC<_vCut.size(); iC++){
    char hName[200];
    sprintf(hName,"cut_%02d",iC);
    _h = myBook(hName,20,0,200,"ETmiss","Entries");
    _vHCut.push_back(_h);
    
    cout << "Selection " << endl;
    _vCut[iC].Print();

    TCut weight("w");
    for(uint iS=0; iS<_vSampleNames.size(); iS++){
      string cmd = "metrel>>" +string(_vHCut[iC]->GetName());
      _vNt[iS]->Draw(cmd.c_str(),_vCut[iC]*weight,"goff");

      float stat_err=0;
      float nom = _vHCut[iC]->IntegralAndError(0,-1,stat_err);
      cout << "\t Sample " << _vSampleNames[iS] << " " << nom << " +/- " << stat_err << endl;
    }
    cout << endl;

  }

}

//_____________________________________________________________________________//
TH1F* myBook(string name, int nbin, float xlow, float xhigh, string xaxis, string yaxis)
{
  return _utils->myTH1F(name.c_str(),name.c_str(),
			nbin,xlow, xhigh,
			xaxis.c_str(),yaxis.c_str());
  
}
