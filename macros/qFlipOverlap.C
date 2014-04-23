{
  /** @file calcOverlap.C
      @brief Calculate the overlap from Anyes's histograms
      @author Brokk Toggerson - brokk.koen.toggerson@cern.ch
      @author University of California, Irvine
      @date First Written: 15 October 2012
      @date Last Modified: 15 October 2012
  */
  
#include "SusyNtuple/TGuiUtils.h"
#include <string>
  
  double appliedFactor = 0.6808;//0.7075;//0.6176;//1;//0.5966; 
  //double newFactor = 0.694367; //+/- 0.0155  //All bkg
  double newFactor = 0.442407; //+/- 0.0154125   // 0.6908; //+/- 0.0154  //Zjets only

  bool applyCorrection=false;//true;

  // Using Zjet
  //    60-110  F = 0.714722 +/- 0.0158234
  //    65-110  F = 0.71089 +/- 0.0157861
  //    70-120  F = 0.710245 +/- 0.0157588  


  const float  mllLow  = 70;
  const float  mllHigh = 110;


  //  string histname = "DGWH_WH_optimSRSS_EE_DGWH_mllcoarse_NOM";
  //string histname = "DGWH_WH_optimSRSS_EE_DGWH_mllShift_NOM";
  string histname = "DGWH_WH_optimSRSS_EE_DGWH_mll_NOM";
  //string histname = "DG2L_CR2LepSS_EE_DG2L_mll_NOM";
  //string histname = "DG2L_CR2LepSS40_EE_DG2L_mll_NOM";
  

  //  string dir = "histos_021414_21fb_n0150_DD_WH_v3/";
  //string dir = "histos_022514_21fb_n0150_DD_WH_v4/"; //comb  map
  string dir = "histos_041614_21fb_n0150_DD_WH_v6/"; //data map
  string _pathHisto  = string(getenv("WORKAREA")) + "/histoAna/SusyAna/" +dir;

  string dFile =  _pathHisto + "histo_data12_std.root"; 
  string ZFile =  _pathHisto + "histo_Zjets_AlpgenPythia_rlep.root";
  //string ZFile =  _pathHisto + "histo_Bkg_Zjets_Alpgen_WZ_ZZ_Sherpa_WW_Sherpa_TopMCNLO_rlep.root";
  //string ZFile =  _pathHisto + "histo_Zee.147770_rlep.root";
  string fFile =  _pathHisto + "histo_data12_flep.root";
  
  cout << dFile << endl;

  double d(0.), dErr(0.); 
  double f(0.), fErr(0.); 
  double Z(0.), ZErr(0.); 
 
  TH1F* hd = 0x0; 
  TH1F* hf = 0x0; 
  TH1F* hZ = 0x0; 

  TFile* fd = new TFile(dFile.c_str()); 
  hd = (TH1F*) fd->Get(histname.c_str());



  int binDn = hd->FindBin(mllLow);
  int binUp = hd->FindBin(mllHigh);
  cout << "Bin range " << binDn << " " << binUp 
       << " corresponding to [" << mllLow << "," << mllHigh << "]"  << endl;
  
 
  d = hd->IntegralAndError(binDn, binUp, dErr); 
  
  TFile* ff = new TFile(fFile.c_str()); 
  hf = (TH1F*) ff->Get(histname.c_str()); 
  f = hf->IntegralAndError(binDn, binUp, fErr); 
  
  TFile* fZ = new TFile(ZFile.c_str()); 
  hZ = (TH1F*) fZ->Get(histname.c_str());
  hZ->Scale(1./appliedFactor);
  if(applyCorrection)  hZ->Scale(1.*newFactor);
  Z = hZ->IntegralAndError(binDn, binUp, ZErr); 
  
  //f + Z = d + o
  //o = f + Z - d
  //factor F = 1 - o/Z
  //F = 1 - (f+Z-d)/Z
  //F = 1 - (f/Z + Z/Z - d/Z)
  //F = 1 - f/Z - 1 + d/Z
  //F = d/Z - f/Z
  //F = (d-f)/Z
  //FErr^2 = (dF/df)^2*fErr^2 + (dF/dd)^2*dErr^2 + (dF/dZ)^2*ZErr^2
  //FErr^2 = (1/Z)^2*fErr^2 + (1/Z)^2*dErr^2 + ((d-f)/Z^2)^2*ZErr^2
  //FErr^2 = (1/Z)^2 * [ ( fErr^2 + dErr^2 ) + F^2*ZErr^2 ]
  
  double F = (d - f) / Z; 
  double FErr = (1./Z)*(1./Z) * ( (fErr*fErr + dErr*dErr) + F*F*ZErr*ZErr ); 
  FErr = sqrt(FErr); 
  
  cout<<"d = "<<d<<" +/- "<<dErr<<endl;
  cout<<"f = "<<f<<" +/- "<<fErr<<endl;
  cout<<"Z = "<<Z<<" +/- "<<ZErr<<endl;
  cout<<"F = "<<F<<" +/- "<<FErr<<endl;
  
  cout<<endl;
  cout<<endl;


  //
  // Make plot
  //


  TGuiUtils* _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");

  bool logy         = true;
  const float scale = 1.3;
  float max         = _utils->getMax(hd,hZ)*scale;
  hd->SetMaximum(max);

  TLegend*  _leg = new TLegend(0.5,0.6,0.8,0.8);
  _utils->legendSetting(_leg,0.04); 
  
  
  THStack* hStack = new THStack("stackBkg_mll","stackBkg_mll");
  _utils->addToTHStack(hStack,hf, kGray, "HIST", _leg, "Fake leptons");
  _utils->addToTHStack(hStack,hZ, kOrange-2, "HIST", _leg, "Z+jets");
  _leg->AddEntry(hd,"Data", "p");
  

  TCanvas* _c1  = _utils->myCanvas("qFlip - Fake overlap",800,800);
 
  hStack->Draw("hist");
  hStack->GetXaxis()->SetRangeUser(40,150);
  hStack->GetXaxis()->SetTitle(hd->GetXaxis()->GetTitle());
  hStack->GetYaxis()->SetTitle(hd->GetYaxis()->GetTitle());
  hStack->GetYaxis()->SetTitleSize(0.05);
  hStack->GetYaxis()->SetTitleOffset(1.2);
  hStack->GetYaxis()->SetLabelSize(0.05);
  hStack->Draw("hist");

  hd->GetXaxis()->SetRangeUser(40,150);
  _utils->myDraw1d(hd,_c1,1,"esame",logy,kBlack,false,20);
  _leg->Draw();
  _c1->RedrawAxis();
  _c1->Update();   
}
