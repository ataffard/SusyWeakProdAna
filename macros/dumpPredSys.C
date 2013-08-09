#include "SusyNtuple/TGuiUtils.h"
#include "SusyWeakProdAna/DrawPlots.h"

typedef unsigned uint;

string dir ="histOutputs/";
//string dir ="histos_080813_21fb_n0145_DD_v4/histOutputs/";
//string dir ="histos_030813_21fb_n0127_Moriond_DD_v8/";
//string dir ="";

TGuiUtils* _utils;
DrawPlots* _ana;


int main(int argc, char *argv[]){
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _ana = new DrawPlots(); 
  
  string _pathHisto  = string(getenv("HISTOANA")) + "/SusyAna/" +dir;
  cout << "Using dir " << _pathHisto << endl;
  
  
  vector<string> sample;
  //sample.push_back("histo_TtbarLeptLept.117800_rlep.root");
  //sample.push_back("histo_llnunu_WW.126892_rlep.root");
  
  //sample.push_back("histo_lllnu_WZ.126893_rlep.root");
  //sample.push_back("histo_data12_flep.root");
  //sample.push_back("histo_ZX_AlpgenPythia_rlep.root");
  //sample.push_back("histo_ZX_Sherpa_rlep.root");
  //sample.push_back("histo_WW_Sherpa_rlep.root");
  //sample.push_back("histo_top_MCNLO_rlep.root");
  //sample.push_back("histo_ZTauTaujets_AlpgenPythia_rlep.root");
  //sample.push_back("histo_llnunu_WW.126892_rlep_preSRmT2.root"); 
  //sample.push_back("histo_ttbar.105200_CRWW_rlep.root");
  //sample.push_back("histo_WW_PowHeg_rlep.root");
  //sample.push_back("histo_ZZ_PowHeg_std.root");
  //sample.push_back("histo_WZ_PowHeg_std.root");
  //sample.push_back("histo_Zjets_SherpaAlpgenPythia_rlep.root");
  //sample.push_back("ML_VRWZ_LLL/histo_WZ.129478_ML_VRWZ_LLL_std.root");
  sample.push_back("ML_VRWZ_LLL/histo_WZ.129478_ML_VRWZ_LLL_rlep_NOM_XS_DN.root");

  vector<string> SR;
  //SR.push_back("DG2L_CR2LepOS_");
  //SR.push_back("DG2L_CR2LepSS_");
  //SR.push_back("DG2L_SRSSjveto_");
  //SR.push_back("DG2L_SROSjveto_");
  //SR.push_back("DG2L_ZXCRWW_");
  //SR.push_back("DG2L_CRWW5_");
  //SR.push_back("DG2L_ZXCRWW2_");
  //SR.push_back("DG2L_preSRmT2_");
  //SR.push_back("DG2L_CRTOP_");
  //SR.push_back("DG2L_SRmT2a_")
  SR.push_back("ML_VRWZ_");

  /*
    SR.push_back("DG2L_SR2jets_");
    SR.push_back("DG2L_SRmT2_");
    SR.push_back("DG2L_NTOP_");
    SR.push_back("DG2L_NWW1_");  
    SR.push_back("DG2L_ZXCR1_");  
    SR.push_back("DG2L_ZXCR3_");  
    SR.push_back("DG2L_ZXCR4_");  
    SR.push_back("DG2L_ZXCR5_");  
  */
  
  vector<string> LEP;
  LEP.push_back("LLL");
  /*
  LEP.push_back("EE");
  LEP.push_back("MM");
  LEP.push_back("EM");
  */
  TH2F* _pred = _utils->myTH2F("Pred", "Pred", 
			       DGSys_N,DGSys_EES_Z_UP,DGSys_N-1,
			       3,0,2,"","");
  for(uint ilep=0; ilep<LEP.size(); ilep++){	
    _pred->GetYaxis()->SetBinLabel(ilep+1,LEP[ilep].c_str());
  }
  for(uint isys=DGSys_NOM;isys<DGSys_N; isys++){
    _pred->GetXaxis()->SetBinLabel(isys+1,DGSystNames[isys].c_str());
  }
  _pred->GetXaxis()->LabelsOption("v");

  for(uint i=0; i<sample.size(); i++){
    cout << "Sample " <<sample[i] << endl;
    TFile* _f = new TFile(string(_pathHisto + sample[i]).c_str(),"READ");
    
    for(uint is=0; is<SR.size(); is++){
      cout << " ========= SR/CR " << SR[is] << " =================" << endl;
      for(uint il=0; il<LEP.size(); il++){
	cout << " ******************* " << LEP[il] << " ***************** "  << endl;
	float nom=0;
	for(uint isys=DGSys_NOM; isys<DGSys_N; isys++){
	  //string sHName = SR[is] + LEP[il] + "_DG2L_pred_"+ DG2LSystNames[isys];
	  string sHName = SR[is] + LEP[il] + "_ML_pred_"+ DGSystNames[isys];
	  TH1F* _h = (TH1F*)  _f->Get(sHName.c_str())->Clone();
	  float fracErr = 0;
	  if(isys==DGSys_NOM) nom=_h->Integral(0,-1);
	  else   fracErr = (nom>0 || nom<0) ? (_h->Integral(0,-1) - nom)/nom : 0.;
	  cout << SR[is] << LEP[il] << "_" << DGSystNames[isys]<< "\t";
	  printf("\t\t %3.4f \t %3.4f \n",_h->Integral(0,-1),fracErr); 

	  double val,err;
	  val = _h->IntegralAndError(0,-1,err);
	  _pred->SetBinContent(isys+1, il+1,val);
	  _pred->SetBinError(isys+1, il+1,err);
	  
	}
      }
      cout << "\n\n" << endl;
    }
    
  }


  // TCanvas* _c1  = _utils->myCanvas("TOP CR");
  //_utils->myDraw2d(_pred,_c1,1,"text",false,kBlack,false,20);
  //_pred->Draw("text");


}
