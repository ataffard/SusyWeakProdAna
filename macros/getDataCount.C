#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"

typedef unsigned uint;

string mth = "std";
string dir = "";//"histos_101112_13fb_n0105_DD_blind_MCData/";


TGuiUtils* _utils;
DrawPlots* _ana;

int main(int argc, char *argv[]){
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
 _ana = new DrawPlots(); 

 string _pathHisto  = string(getenv("WORKAREA")) + "/histoAna/SusyAna/" +dir;
 cout << "Using dir " << _pathHisto << endl;

 vector<string> stream;
 stream.push_back("Egamma");
 stream.push_back("Muons");

 vector<string> prd;
 // prd.push_back("A");
 // prd.push_back("B");
 // prd.push_back("C");
 // prd.push_back("D");
 prd.push_back("E1");
 prd.push_back("E2");
 prd.push_back("E3");
 prd.push_back("E4");
 prd.push_back("E5");

 vector<string> SR;
 SR.push_back("DG2L_CR2LepOS_");
 SR.push_back("DG2L_CR2LepSS_");

 //SR.push_back("DG2L_NWW1_");

 vector<string> LEP;
 LEP.push_back("EE");
 LEP.push_back("MM");
 LEP.push_back("EM");


 for(uint i=0; i<stream.size(); i++){
   cout << "Stream " <<stream[i] << endl;

   for(uint j=0; j<prd.size(); j++){
     string sFile = "histo_" + stream[i] + ".period" + prd[j] + "_" +mth + ".root";
     //cout << "File " << sFile << endl;
     cout << "Period " << prd[j] << endl;
     TFile* _f = new TFile(string(_pathHisto + sFile).c_str(),"READ");
     _f.Open(string(_pathHisto + sFile).c_str(),"READ");
     /*
     if(_f.IsOpen()==kFALSE){
       cout << " File " << _pathHisto + sFile <<  " not found " << endl;
       continue;
     }
     */

     for(uint is=0; is<SR.size(); is++){
       for(uint il=0; il<LEP.size(); il++){
	 string sHName = SR[is] + LEP[il] + "_DG2L_pred";
	 //	 cout << "Shist " << sHName << endl;
	 TH1F* _h = (TH1F*)  _f->Get(sHName.c_str())->Clone();
	 cout << SR[is] << "_" << LEP[il] << " ";
	 printf("%3.2f \n",_h->Integral(0,-1)); 
       }
     }
     cout << endl;
   }
   cout << endl;
 }





}
