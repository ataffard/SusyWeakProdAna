{
  /** @file calcOverlap.C
      @brief Calculate the overlap from Anyes's histograms
      @author Brokk Toggerson - brokk.koen.toggerson@cern.ch
      @author University of California, Irvine
      @date First Written: 15 October 2012
      @date Last Modified: 15 October 2012
  */
  
#include <string>
  
  double appliedFactor = 0.6176;//1;//0.5966; 
  //string histname = "DG2L_CR2LepSS_EE_DG2L_mll_NOM";
  string histname = "DG2L_preSRSSjveto_EE_DG2L_mll_NOM";
  //string histname = "DG2L_VR1SS_EE_DG2L_mll_NOM";

  int binDn = 39;
  int binUp = 50;

  string dir = "histos_110812_13fb_n0111_DD_MMtrial9_SYS_HFT/";
  string _pathHisto  = string(getenv("WORKAREA")) + "/histoAna/SusyAna/" +dir;

  string dFile =  _pathHisto + "histo_data12_std.root"; 
  string ZFile =  _pathHisto + "histo_ZX_Sherpa_rlep.root";
  //string ZFile =  _pathHisto + "histo_Zee.147770_rlep.root";
  string fFile =  _pathHisto + "histo_data12_fake.root";
  
  cout << dFile << endl;

  double d(0.), dErr(0.); 
  double f(0.), fErr(0.); 
  double Z(0.), ZErr(0.); 
 
  TH1F* hd = 0x0; 
  TH1F* hf = 0x0; 
  TH1F* hZ = 0x0; 

  TFile* fd = new TFile(dFile.c_str()); 
  hd = (TH1F*) fd->Get(histname.c_str()); 
  d = hd->IntegralAndError(binDn, binUp, dErr); 
  
  TFile* ff = new TFile(fFile.c_str()); 
  hf = (TH1F*) ff->Get(histname.c_str()); 
  f = hf->IntegralAndError(binDn, binUp, fErr); 
  
  TFile* fZ = new TFile(ZFile.c_str()); 
  hZ = (TH1F*) fZ->Get(histname.c_str());
  hZ->Scale(1./appliedFactor);
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

}
