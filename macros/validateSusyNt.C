/*
wget http://folk.uio.no/borgeg/atlas/susy/D3PDprod/prod/prod_latest/subperiods_p1181.txt
awk '/group/ {print $18,'\t', $7}' subperiods_p1181.txt > subperiods_p1181.test

*/

#include "SusyNtuple/TGuiUtils.h"
typedef unsigned uint;

string pass = "n0107/";
string dir = "SusyNt_merge/"+pass;

TGuiUtils* _utils;

int main(int argc, char *argv[]){
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");

  string _pathHisto  = string(getenv("WORKAREA")) + "/histoAna/SusyAna/" +dir;

  cout << "Validating " << dir << endl;
  vector<string> stream;
  stream.push_back("Egamma");
  stream.push_back("Muons");
  
  vector<string> prd;
  prd.push_back("A");
  prd.push_back("B");
  prd.push_back("C");
  prd.push_back("D");
  prd.push_back("E1");
  prd.push_back("E2");
  prd.push_back("E3");
  prd.push_back("E4");
  prd.push_back("E5");

  for(uint i=0; i<stream.size(); i++){
    cout << "Stream " <<stream[i] << endl;
    for(uint j=0; j<prd.size(); j++){
     string sFile = stream[i] + ".period" + prd[j] + "_merge.root";
     TFile* _f = new TFile(string(_pathHisto + sFile).c_str(),"READ");
     
     string sHName ="rawCutFlow"; 
     TH1F* _h = (TH1F*)  _f->Get(sHName.c_str())->Clone();
     int nEvt = _h->GetBinContent(1);
     cout << "File " << sFile << " has " << nEvt << endl;
    } 
  }


}
