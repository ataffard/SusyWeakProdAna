#include <iostream>

#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include "TObject.h"
#include "TH1.h"

TChain* _chain;

void validateHFT()
{
  TString _pathFile(string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/histos_102312_13fb_n0107_DD_predSR/HFTOutputs/");

  _chain = new TChain("id_Data");
  TString fileName=_pathFile + "central_*";
  _chain->Add(fileName);

  
  TCut SR1("2LnCentralLightJets==0 && 2LnCentralBJets==0 && 2LnForwardJets==0 && abs(2LMll/1000.-91.2)>10. && 2LMETrel>100000. && 2LisOS");
  TCut SR2("2LnCentralLightJets==0 && 2LnCentralBJets==0 && 2LnForwardJets==0 && 2LMETrel>100000. && !2LisOS");
  TCut SR3("2LnCentralLightJets>1 && 2LnCentralBJets==0 && 2LnForwardJets==0 && 2LMETrel>50000. && 2LisOS && abs(2LMll/1000.-91.2)>10. && 2LTopVeto");
  TCut SR4a("2LnCentralLightJets==0) && (2LnCentralBJets==0) && (2LnForwardJets==0) && abs(2LMll/1000.-91.2)>10. && 2LMETrel>40000. && 2LisOS && 2LMT2>90000.");
  TCut SR4b("2LnCentralLightJets==0 && 2LnCentralBJets==0 && 2LnForwardJets==0 && abs(2LMll/1000.-91.2)>10. && 2LMETrel>40000. && 2LisOS && 2LMT2>110000.");
  // Top CRs
  TCut TopCR1("(2LnCentralLightJets + 2LnCentralBJets)>1 &&  2LnCentralBJets>=1 && abs(2LMll/1000.-91.2)>10. && 2LMETrel>100000. && 2LisOS");
  TCut TopCR2("(2LnCentralLightJets + 2LnCentralBJets)>1 &&  2LnCentralBJets>=1 && abs(2LMll/1000.-91.2)>10. && 2LMETrel>50000. && 2LisOS");
  TCut TopCR4("(2LnCentralLightJets + 2LnCentralBJets)>1 &&  2LnCentralBJets>=1 && abs(2LMll/1000.-91.2)>10. && 2LMETrel>40000. && 2LisOS");
  // Z+X
  TCut ZXCR1("(isEE || isMUMU) && 2LnCentralLightJets==0 && 2LnCentralBJets==0 && 2LnForwardJets==0 && abs(2LMll/1000.-91.2)<10. && 2LMETrel>100000. && 2LisOS");
  TCut ZXCR3("(isEE || isMUMU) && 2LnCentralLightJets>1 && 2LnCentralBJets==0 && 2LnForwardJets==0 && 2LMETrel>50000. && 2LisOS && abs(2LMll/1000.-91.2)<10. && 2LTopVeto");
  TCut ZXCR4("(isEE || isMUMU) && 2LnCentralLightJets==0 && 2LnCentralBJets==0 && 2LnForwardJets==0 && abs(2LMll/1000.-91.2)<10. && 2LMETrel>40000. && 2LisOS");


  _chain->Draw("isEE>>TopCR4_EE(2,-0.5,1.5)",TopCR4&&"isEE","goff");
  _chain->Draw("isEE>>TopCR4_MM(2,-0.5,1.5)",TopCR4&&"isMUMU","goff");
  _chain->Draw("isEE>>TopCR4_EM(2,-0.5,1.5)",TopCR4&&"isEMU","goff");


  cout << "TopCR4 " << endl;
  cout << "  EE " << TopCR4_EE->Integral(0,-1) 
       << "  MM " << TopCR4_MM->Integral(0,-1) 
       << "  EM " << TopCR4_EM->Integral(0,-1) 
       << endl;


}
