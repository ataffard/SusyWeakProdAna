#include <iostream>
#include <string>
#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include "TObject.h"
#include "TH1.h"

TChain* _chain;

void validateHFT()
{
  //TString _pathFile(string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/histos_110312_13fb_n0111_DD_MMtrial9_SYS_HFT/HFTOutputs/");
  TString _pathFile(string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/HFTOutputs/");

  vector<TString> sample; 
  
  
  _chain = new TChain("id_126893");
  sample.push_back("NOM_126893.root");
  
  
  for(unsigned int i=0; i<sample.size(); i++){
    TString fileName=_pathFile + sample[i]; 
    _chain->Add(fileName);
  }

  //Signal region and CR's cut
  TCut SR1("L2nCentralLightJets==0 && L2nCentralBJets==0 && L2nForwardJets==0 && abs(L2Mll/1000.-91.2)>10. && L2METrel>100000. && MT2<90000. && isOS");

  TCut SR3("L2nCentralLightJets>1 && L2nCentralBJets==0 && L2nForwardJets==0 && L2METrel>50000. && isOS && abs(L2Mll/1000.-91.2)>10. && L2TopVeto");
  TCut SR3b("L2nCentralLightJets>1 && L2nCentralBJets==0 && L2nForwardJets==0 && L2METrel>50000. && isOS && abs(L2Mll/1000.-91.2)<10. && L2TopVeto && L2cjet1pt>50000. && L2cjet2pt>50000. && L2mJJ>50 && L2mJJ<100 && MT2>70");
  TCut SR4a("(L2nCentralLightJets==0) && (L2nCentralBJets==0) && (L2nForwardJets==0) && abs(L2Mll/1000.-91.2)>10. && L2METrel>40000. && isOS && MT2>90000.");
  TCut SR4b("(L2nCentralLightJets==0) && L2nCentralBJets==0 && L2nForwardJets==0 && abs(L2Mll/1000.-91.2)>10. && L2METrel>40000. && isOS && MT2>110000.");

  // Top CRs
  TCut TopCR1("(L2nCentralLightJets + L2nCentralBJets)>1 &&  L2nCentralBJets>=1 && abs(L2Mll/1000.-91.2)>10. && L2METrel>100000. && isOS");
  TCut TopCR2("(L2nCentralLightJets + L2nCentralBJets)>1 &&  L2nCentralBJets>=1 && abs(L2Mll/1000.-91.2)>10. && L2METrel>50000. && isOS");
  TCut TopCR4("(L2nCentralLightJets + L2nCentralBJets)>1 &&  L2nCentralBJets>=1 && abs(L2Mll/1000.-91.2)>10. && L2METrel>40000. && isOS");
  // Z+X
  TCut ZXCR1("(isEE || isMUMU) && L2nCentralLightJets==0 && L2nCentralBJets==0 && L2nForwardJets==0 && abs(L2Mll/1000.-91.2)<10. && L2METrel>100000. && isOS");
  TCut ZXCR2("(isEE || isMUMU) && L2nCentralLightJets>1  && L2nCentralBJets==0 && L2nForwardJets==0 && abs(L2Mll/1000.-91.2)<10. && L2METrel>50000. && isOS && L2TopVeto");
  TCut ZXCR3("(isEE || isMUMU) && L2nCentralLightJets==0 && L2nCentralBJets==0 && L2nForwardJets==0 && L2METrel>40000. && MT2<90000.  && isOS && abs(L2Mll/1000.-91.2)<10.");
  TCut ZXCR4("(isEE || isMUMU) && L2nCentralLightJets==0 && L2nCentralBJets==0 && L2nForwardJets==0 && L2METrel>40000. && MT2<110000. && isOS && abs(L2Mll/1000.-91.2)<10.");
  //NWW1
  TCut WWCR1("L2nCentralLightJets==0 && L2nCentralBJets==0 && L2nForwardJets==0 && abs(L2Mll/1000.-91.2)>10.&& L2METrel>70000. && L2METrel<100000. && MT2<90000. && isOS");

  TCut SR1EE_w( (SR1 && "isEE"  ) * "L2TotalWeight");
  TCut SR1MM_w( (SR1 && "isMUMU") * "L2TotalWeight");
  TCut SR1EM_w( (SR1 && "isEMU" ) * "L2TotalWeight");

  TCut SR3EE_w( (SR3 && "isEE"  ) * "L2TotalWeight");
  TCut SR3MM_w( (SR3 && "isMUMU") * "L2TotalWeight");
  TCut SR3EM_w( (SR3 && "isEMU" ) * "L2TotalWeight");

  TCut SR3bEE_w( (SR3b && "isEE"  ) * "L2TotalWeight");
  TCut SR3bMM_w( (SR3b && "isMUMU") * "L2TotalWeight");
  TCut SR3bEM_w( (SR3b && "isEMU" ) * "L2TotalWeight");

  TCut SR4aEE_w( (SR4a && "isEE"  ) * "L2TotalWeight");
  TCut SR4aMM_w( (SR4a && "isMUMU") * "L2TotalWeight");
  TCut SR4aEM_w( (SR4a && "isEMU" ) * "L2TotalWeight");

  TCut SR4bEE_w( (SR4b && "isEE"  ) * "L2TotalWeight");
  TCut SR4bMM_w( (SR4b && "isMUMU") * "L2TotalWeight");
  TCut SR4bEM_w( (SR4b && "isEMU" ) * "L2TotalWeight");

  TCut TopCR4EE_w( (TopCR4 && "isEE"  ) * "L2TotalWeight");
  TCut TopCR4MM_w( (TopCR4 && "isMUMU") * "L2TotalWeight");
  TCut TopCR4EM_w( (TopCR4 && "isEMU" ) * "L2TotalWeight");

  TCut WWCR1EE_w( (WWCR1 && "isEE"  ) * "L2TotalWeight");
  TCut WWCR1MM_w( (WWCR1 && "isMUMU") * "L2TotalWeight");
  TCut WWCR1EM_w( (WWCR1 && "isEMU" ) * "L2TotalWeight");


  TCut ZXCR1EE_w( (ZXCR1 && "isEE"  ) * "L2TotalWeight");
  TCut ZXCR1MM_w( (ZXCR1 && "isMUMU") * "L2TotalWeight");
  TCut ZXCR1EM_w( (ZXCR1 && "isEMU" ) * "L2TotalWeight");

  TCut ZXCR2EE_w( (ZXCR2 && "isEE"  ) * "L2TotalWeight");
  TCut ZXCR2MM_w( (ZXCR2 && "isMUMU") * "L2TotalWeight");
  TCut ZXCR2EM_w( (ZXCR2 && "isEMU" ) * "L2TotalWeight");

  TCut ZXCR3EE_w( (ZXCR3 && "isEE"  ) * "L2TotalWeight");
  TCut ZXCR3MM_w( (ZXCR3 && "isMUMU") * "L2TotalWeight");
  TCut ZXCR3EM_w( (ZXCR3 && "isEMU" ) * "L2TotalWeight");

  TCut ZXCR4EE_w( (ZXCR4 && "isEE"  ) * "L2TotalWeight");
  TCut ZXCR4MM_w( (ZXCR4 && "isMUMU") * "L2TotalWeight");
  TCut ZXCR4EM_w( (ZXCR4 && "isEMU" ) * "L2TotalWeight");



  //
  //Get the yields
  //
  _chain->Draw("isEE>>SR1_EE(2,-0.5,1.5)",SR1EE_w,"goff");
  _chain->Draw("isEE>>SR1_MM(2,-0.5,1.5)",SR1MM_w,"goff");
  _chain->Draw("isEE>>SR1_EM(2,-0.5,1.5)",SR1EM_w,"goff"); 
  
  _chain->Draw("isEE>>SR3_EE(2,-0.5,1.5)",SR3EE_w,"goff");
  _chain->Draw("isEE>>SR3_MM(2,-0.5,1.5)",SR3MM_w,"goff");
  _chain->Draw("isEE>>SR3_EM(2,-0.5,1.5)",SR3EM_w,"goff");

  _chain->Draw("isEE>>SR3b_EE(2,-0.5,1.5)",SR3bEE_w,"goff");
  _chain->Draw("isEE>>SR3b_MM(2,-0.5,1.5)",SR3bMM_w,"goff");
  _chain->Draw("isEE>>SR3b_EM(2,-0.5,1.5)",SR3bEM_w,"goff");

  _chain->Draw("isEE>>SR4a_EE(2,-0.5,1.5)",SR4aEE_w,"goff");
  _chain->Draw("isEE>>SR4a_MM(2,-0.5,1.5)",SR4aMM_w,"goff");
  _chain->Draw("isEE>>SR4a_EM(2,-0.5,1.5)",SR4aEM_w,"goff");

  _chain->Draw("isEE>>SR4b_EE(2,-0.5,1.5)",SR4bEE_w,"goff");
  _chain->Draw("isEE>>SR4b_MM(2,-0.5,1.5)",SR4bMM_w,"goff");
  _chain->Draw("isEE>>SR4b_EM(2,-0.5,1.5)",SR4bEM_w,"goff");

  _chain->Draw("isEE>>TopCR4_EE(2,-0.5,1.5)",TopCR4EE_w,"goff");
  _chain->Draw("isEE>>TopCR4_MM(2,-0.5,1.5)",TopCR4MM_w,"goff");
  _chain->Draw("isEE>>TopCR4_EM(2,-0.5,1.5)",TopCR4EM_w,"goff");

  _chain->Draw("isEE>>WWCR1_EE(2,-0.5,1.5)",WWCR1EE_w,"goff");
  _chain->Draw("isEE>>WWCR1_MM(2,-0.5,1.5)",WWCR1MM_w,"goff");
  _chain->Draw("isEE>>WWCR1_EM(2,-0.5,1.5)",WWCR1EM_w,"goff"); 

  _chain->Draw("isEE>>ZXCR1_EE(2,-0.5,1.5)",ZXCR1EE_w,"goff");
  _chain->Draw("isEE>>ZXCR1_MM(2,-0.5,1.5)",ZXCR1MM_w,"goff");
  _chain->Draw("isEE>>ZXCR1_EM(2,-0.5,1.5)",ZXCR1EM_w,"goff"); 

  _chain->Draw("isEE>>ZXCR2_EE(2,-0.5,1.5)",ZXCR2EE_w,"goff");
  _chain->Draw("isEE>>ZXCR2_MM(2,-0.5,1.5)",ZXCR2MM_w,"goff");
  _chain->Draw("isEE>>ZXCR2_EM(2,-0.5,1.5)",ZXCR2EM_w,"goff"); 

  _chain->Draw("isEE>>ZXCR3_EE(2,-0.5,1.5)",ZXCR3EE_w,"goff");
  _chain->Draw("isEE>>ZXCR3_MM(2,-0.5,1.5)",ZXCR3MM_w,"goff");
  _chain->Draw("isEE>>ZXCR3_EM(2,-0.5,1.5)",ZXCR3EM_w,"goff"); 
  
  _chain->Draw("isEE>>ZXCR4_EE(2,-0.5,1.5)",ZXCR4EE_w,"goff");
  _chain->Draw("isEE>>ZXCR4_MM(2,-0.5,1.5)",ZXCR4MM_w,"goff");
  _chain->Draw("isEE>>ZXCR4_EM(2,-0.5,1.5)",ZXCR4EM_w,"goff"); 


  //
  //Printout
  //
  cout << "SR1 SROSjveto" << endl;
  cout << "  EE " << SR1_EE->Integral(0,-1) 
       << "  MM " << SR1_MM->Integral(0,-1) 
       << "  EM " << SR1_EM->Integral(0,-1) 
       << endl;
  
  cout << "SR3 SR2jet" << endl;
  cout << "  EE " << SR3_EE->Integral(0,-1) 
       << "  MM " << SR3_MM->Integral(0,-1) 
       << "  EM " << SR3_EM->Integral(0,-1) 
       << endl;

  cout << "SR3b SRZjets" << endl;
  cout << "  EE " << SR3b_EE->Integral(0,-1) 
       << "  MM " << SR3b_MM->Integral(0,-1) 
       << "  EM " << SR3b_EM->Integral(0,-1) 
       << endl;

  cout << "SR4a SRmT2a" << endl;
  cout << "  EE " << SR4a_EE->Integral(0,-1) 
       << "  MM " << SR4a_MM->Integral(0,-1) 
       << "  EM " << SR4a_EM->Integral(0,-1) 
       << endl;

  cout << "SR4b SRmT2b" << endl;
  cout << "  EE " << SR4b_EE->Integral(0,-1) 
       << "  MM " << SR4b_MM->Integral(0,-1) 
       << "  EM " << SR4b_EM->Integral(0,-1) 
       << endl;

  cout << "TopCR4 " << endl;
  cout << "  EE " << TopCR4_EE->Integral(0,-1) 
       << "  MM " << TopCR4_MM->Integral(0,-1) 
       << "  EM " << TopCR4_EM->Integral(0,-1) 
       << endl;

  cout << "WWCR1 " << endl;
  cout << "  EE " << WWCR1_EE->Integral(0,-1) 
       << "  MM " << WWCR1_MM->Integral(0,-1) 
       << "  EM " << WWCR1_EM->Integral(0,-1) 
       << endl;
  
  cout << "ZXCR1 OSjveto" << endl;
  cout << "  EE " << ZXCR1_EE->Integral(0,-1) 
       << "  MM " << ZXCR1_MM->Integral(0,-1) 
       << "  EM " << ZXCR1_EM->Integral(0,-1) 
       << endl;

  cout << "ZXCR2 2jets" << endl;
  cout << "  EE " << ZXCR2_EE->Integral(0,-1) 
       << "  MM " << ZXCR2_MM->Integral(0,-1) 
       << "  EM " << ZXCR2_EM->Integral(0,-1) 
       << endl;

  cout << "ZXCR3 mt2a" << endl;
  cout << "  EE " << ZXCR3_EE->Integral(0,-1) 
       << "  MM " << ZXCR3_MM->Integral(0,-1) 
       << "  EM " << ZXCR3_EM->Integral(0,-1) 
       << endl;

  cout << "ZXCR4 mt2b" << endl;
  cout << "  EE " << ZXCR4_EE->Integral(0,-1) 
       << "  MM " << ZXCR4_MM->Integral(0,-1) 
       << "  EM " << ZXCR4_EM->Integral(0,-1) 
       << endl;


}
