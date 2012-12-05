#define ToyNt_cxx

#include "SusyWeakProdAna/ToyNt.h"
#include "TParameter.h"

using namespace std;


//-----------------------------------------------------------------------------------------------------------
ToyNt::ToyNt(TString MCID, TString suffix) {

  TString filename = MCID + "_" + suffix + ".root";
  
  file= TFile::Open(filename,"RECREATE");
  tree = new TTree("ToyNt","ToyNt");
  (tree)->SetAutoSave(10000000);
  TTree::SetBranchStyle(1);
  (tree)->SetDirectory(file);
  

  tree->Branch("mcid",&_b_mcid);
  tree->Branch("run",&_b_run);
  tree->Branch("event",&_b_event);
  tree->Branch("npv",&_b_npv);
  tree->Branch("npvCorr",&_b_npvCorr);

  tree->Branch("nlep",&_b_nlep);
  tree->Branch("l_pt",_b_l_pt);

  clearOutputBranches();
  return;

}
//-----------------------------------------------------------------------------------------------------------
void ToyNt::clearOutputBranches(void) {

  return;
}

//-----------------------------------------------------------------------------------------------------------
void ToyNt::SaveTree(){
  
  // Write out the output tree and close the output file
  file->cd();
  tree->SetDirectory(file);
  tree->Write();
  tree->SetDirectory(0);
  delete tree;
  file->Close();
  delete file;
}

//-----------------------------------------------------------------------------------------------------------
void ToyNt::WriteTree(){
  tree->Fill();
  clearOutputBranches();
} 

//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTreeEvent(TString mcid, int run, int event, int npv, int npvCorr)
{

}
//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTreeLeptons()
{

}
//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTreeSignalJets()
{

}
//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTreeOtherJets()
{

}
//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTreeKin()
{

}
//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTreeMet()
{

}

//-----------------------------------------------------------------------------------------------------------
void ToyNt::setSumOfMcWeights(double sumOfMcWeights) {
  // Define histogram
  TH1D *sumwhist = new TH1D("sumOfMcWeights_"+_b_mcid,"sumOfMcWeights_"+_b_mcid,1,0.,1.);
  
  // Fill histogram
  sumwhist -> Fill( 0. , sumOfMcWeights ) ;
  
  // Write intLumi to file
  file->cd();
  sumwhist->SetDirectory(file);
  sumwhist->Write();
  sumwhist->SetDirectory(0);
  
  delete sumwhist;
  
  return;
}
