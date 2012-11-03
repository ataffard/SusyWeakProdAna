#include <iostream>

#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include "TObject.h"
#include "TH1.h"

TChain* _chain;

void validateHFT()
{
  //TString _pathFile(string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/histos_103112_13fb_n0111_DD_MMtrial8_SYS_HFT/HFTOutputs/");
  TString _pathFile(string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/HFTOutputs/");

  //_chain = new TChain("id_126892");
  //  TString fileName=_pathFile + "NOM_126892.root";

  _chain = new TChain("id_147771");
    TString fileName=_pathFile + "NOM_147771.root";

  //_chain = new TChain("id_117800");
  //  TString fileName=_pathFile + "NOM_117800.root";

  _chain->Add(fileName);

  //Signal region and CR's cut
  TCut SR1("2LnCentralLightJets==0 && 2LnCentralBJets==0 && 2LnForwardJets==0 && abs(2LMll/1000.-91.2)>10. && 2LMETrel>100000. && 2LisOS");
  TCut SRSS("((isEE || isEMU) && 2LisOS) || !2LisOS");
  //TCut SRSS("!2LisOS"); //old HFT

  TCut SR2("2LnCentralLightJets==0 && 2LnCentralBJets==0 && 2LnForwardJets==0 && 2LMETrel>100000");
  TCut SR3("2LnCentralLightJets>1 && 2LnCentralBJets==0 && 2LnForwardJets==0 && 2LMETrel>50000. && 2LisOS && abs(2LMll/1000.-91.2)>10. && 2LTopVeto");
  TCut SR4a("(2LnCentralLightJets==0) && (2LnCentralBJets==0) && (2LnForwardJets==0) && abs(2LMll/1000.-91.2)>10. && 2LMETrel>40000. && 2LisOS && 2LMT2>90000.");
  TCut SR4b("(2LnCentralLightJets==0) && 2LnCentralBJets==0 && 2LnForwardJets==0 && abs(2LMll/1000.-91.2)>10. && 2LMETrel>40000. && 2LisOS && 2LMT2>110000.");
  // Top CRs
  TCut TopCR1("(2LnCentralLightJets + 2LnCentralBJets)>1 &&  2LnCentralBJets>=1 && abs(2LMll/1000.-91.2)>10. && 2LMETrel>100000. && 2LisOS");
  TCut TopCR2("(2LnCentralLightJets + 2LnCentralBJets)>1 &&  2LnCentralBJets>=1 && abs(2LMll/1000.-91.2)>10. && 2LMETrel>50000. && 2LisOS");
  TCut TopCR4("(2LnCentralLightJets + 2LnCentralBJets)>1 &&  2LnCentralBJets>=1 && abs(2LMll/1000.-91.2)>10. && 2LMETrel>40000. && 2LisOS");
  // Z+X
  TCut ZXCR1("(isEE || isMUMU) && 2LnCentralLightJets==0 && 2LnCentralBJets==0 && 2LnForwardJets==0 && abs(2LMll/1000.-91.2)<10. && 2LMETrel>100000. && 2LisOS");
  TCut ZXCR3("(isEE || isMUMU) && 2LnCentralLightJets>1 && 2LnCentralBJets==0 && 2LnForwardJets==0 && 2LMETrel>50000. && 2LisOS && abs(2LMll/1000.-91.2)<10. && 2LTopVeto");
  TCut ZXCR4("(isEE || isMUMU) && 2LnCentralLightJets==0 && 2LnCentralBJets==0 && 2LnForwardJets==0 && abs(2LMll/1000.-91.2)<10. && 2LMETrel>40000. && 2LisOS");
  //NWW1
  TCut WWCR1("2LnCentralLightJets==0 && 2LnCentralBJets==0 && 2LnForwardJets==0 && abs(2LMll/1000.-91.2)>10.&& 2LMETrel>70000 && 2LMETrel<100000 && 2LisOS");

  TCut SR1EE_w( (SR1 && "isEE"  ) * "2LTotalWeight");
  TCut SR1MM_w( (SR1 && "isMUMU") * "2LTotalWeight");
  TCut SR1EM_w( (SR1 && "isEMU" ) * "2LTotalWeight");

  //  TCut SR2EE_w( (SR2 && "isEE"   && SRSS) * "2LTotalWeight"); 
  TCut SR2EE_w( (SR2 && "isEE"   && SRSS) * "2LTotalWeight* 2LqFlipWeight");
  TCut SR2MM_w( (SR2 && "isMUMU" && SRSS) * "2LTotalWeight");
  //  TCut SR2EM_w( (SR2 && "isEMU"  && SRSS) * "2LTotalWeight"); 
  TCut SR2EM_w( (SR2 && "isEMU"  && SRSS) * "2LTotalWeight* 2LqFlipWeight"); 

  TCut SR3EE_w( (SR3 && "isEE"  ) * "2LTotalWeight");
  TCut SR3MM_w( (SR3 && "isMUMU") * "2LTotalWeight");
  TCut SR3EM_w( (SR3 && "isEMU" ) * "2LTotalWeight");

  TCut SR4aEE_w( (SR4a && "isEE"  ) * "2LTotalWeight");
  TCut SR4aMM_w( (SR4a && "isMUMU") * "2LTotalWeight");
  TCut SR4aEM_w( (SR4a && "isEMU" ) * "2LTotalWeight");

  TCut SR4bEE_w( (SR4b && "isEE"  ) * "2LTotalWeight");
  TCut SR4bMM_w( (SR4b && "isMUMU") * "2LTotalWeight");
  TCut SR4bEM_w( (SR4b && "isEMU" ) * "2LTotalWeight");

  TCut TopCR4EE_w( (TopCR4 && "isEE"  ) * "2LTotalWeight");
  TCut TopCR4MM_w( (TopCR4 && "isMUMU") * "2LTotalWeight");
  TCut TopCR4EM_w( (TopCR4 && "isEMU" ) * "2LTotalWeight");

  TCut WWCR1EE_w( (WWCR1 && "isEE"  ) * "2LTotalWeight");
  TCut WWCR1MM_w( (WWCR1 && "isMUMU") * "2LTotalWeight");
  TCut WWCR1EM_w( (WWCR1 && "isEMU" ) * "2LTotalWeight");


  TCut ZXCR1EE_w( (ZXCR1 && "isEE"  ) * "2LTotalWeight");
  TCut ZXCR1MM_w( (ZXCR1 && "isMUMU") * "2LTotalWeight");
  TCut ZXCR1EM_w( (ZXCR1 && "isEMU" ) * "2LTotalWeight");

  TCut ZXCR3EE_w( (ZXCR3 && "isEE"  ) * "2LTotalWeight");
  TCut ZXCR3MM_w( (ZXCR3 && "isMUMU") * "2LTotalWeight");
  TCut ZXCR3EM_w( (ZXCR3 && "isEMU" ) * "2LTotalWeight");

  TCut ZXCR4EE_w( (ZXCR4 && "isEE"  ) * "2LTotalWeight");
  TCut ZXCR4MM_w( (ZXCR4 && "isMUMU") * "2LTotalWeight");
  TCut ZXCR4EM_w( (ZXCR4 && "isEMU" ) * "2LTotalWeight");



  //
  //Get the yields
  //
  _chain->Draw("isEE>>SR1_EE(2,-0.5,1.5)",SR1EE_w,"goff");
  _chain->Draw("isEE>>SR1_MM(2,-0.5,1.5)",SR1MM_w,"goff");
  _chain->Draw("isEE>>SR1_EM(2,-0.5,1.5)",SR1EM_w,"goff"); 
  
  _chain->Draw("isEE>>SR2_EE(2,-0.5,1.5)",SR2EE_w,"goff");
  _chain->Draw("isEE>>SR2_MM(2,-0.5,1.5)",SR2MM_w,"goff");
  _chain->Draw("isEE>>SR2_EM(2,-0.5,1.5)",SR2EM_w,"goff");
  
  _chain->Draw("isEE>>SR3_EE(2,-0.5,1.5)",SR3EE_w,"goff");
  _chain->Draw("isEE>>SR3_MM(2,-0.5,1.5)",SR3MM_w,"goff");
  _chain->Draw("isEE>>SR3_EM(2,-0.5,1.5)",SR3EM_w,"goff");

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

  _chain->Draw("isEE>>ZXCR3_EE(2,-0.5,1.5)",ZXCR3EE_w,"goff");
  _chain->Draw("isEE>>ZXCR3_MM(2,-0.5,1.5)",ZXCR3MM_w,"goff");
  _chain->Draw("isEE>>ZXCR3_EM(2,-0.5,1.5)",ZXCR3EM_w,"goff"); 
  
  _chain->Draw("isEE>>ZXCR4_EE(2,-0.5,1.5)",ZXCR4EE_w,"goff");
  _chain->Draw("isEE>>ZXCR4_MM(2,-0.5,1.5)",ZXCR4MM_w,"goff");
  _chain->Draw("isEE>>ZXCR4_EM(2,-0.5,1.5)",ZXCR4EM_w,"goff"); 


  //
  //Printout
  //
  cout << "SR1 " << endl;
  cout << "  EE " << SR1_EE->Integral(0,-1) 
       << "  MM " << SR1_MM->Integral(0,-1) 
       << "  EM " << SR1_EM->Integral(0,-1) 
       << endl;
  
  cout << "SR2 " << endl;
  cout << "  EE " << SR2_EE->Integral(0,-1) 
       << "  MM " << SR2_MM->Integral(0,-1) 
       << "  EM " << SR2_EM->Integral(0,-1) 
       << endl;
  
  cout << "SR3 " << endl;
  cout << "  EE " << SR3_EE->Integral(0,-1) 
       << "  MM " << SR3_MM->Integral(0,-1) 
       << "  EM " << SR3_EM->Integral(0,-1) 
       << endl;

  cout << "SR4a " << endl;
  cout << "  EE " << SR4a_EE->Integral(0,-1) 
       << "  MM " << SR4a_MM->Integral(0,-1) 
       << "  EM " << SR4a_EM->Integral(0,-1) 
       << endl;

  cout << "SR4b " << endl;
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
  
  cout << "ZXCR1 " << endl;
  cout << "  EE " << ZXCR1_EE->Integral(0,-1) 
       << "  MM " << ZXCR1_MM->Integral(0,-1) 
       << "  EM " << ZXCR1_EM->Integral(0,-1) 
       << endl;

  cout << "ZXCR3 " << endl;
  cout << "  EE " << ZXCR3_EE->Integral(0,-1) 
       << "  MM " << ZXCR3_MM->Integral(0,-1) 
       << "  EM " << ZXCR3_EM->Integral(0,-1) 
       << endl;

  cout << "ZXCR4 " << endl;
  cout << "  EE " << ZXCR4_EE->Integral(0,-1) 
       << "  MM " << ZXCR4_MM->Integral(0,-1) 
       << "  EM " << ZXCR4_EM->Integral(0,-1) 
       << endl;


}
