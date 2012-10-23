/*

root -l 'ChangeDataTreeName.C+("Egamma.periodA")'

*/

#include <iostream>

#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include "TObject.h"
#include "TH1.h"

void ChangeDataTreeName(std::string suffix){

  // Define names
  TString filename = "", treename = "", histname = "";
  filename.Form("central_%s.root",suffix.c_str());
  treename.Form("id_%s",suffix.c_str());
  histname.Form("sumOfMcWeights_%s",suffix.c_str());

  TString _pathFile(string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/histos_102112_13fb_n0107_DD_predSR/HFTOutputs/");
  filename = _pathFile+filename;
  cout << "open " << filename << endl;

  // Open file
  TFile* file         = new TFile(filename,"UPDATE");
  if(file->IsOpen()==kFALSE){
    cerr << "Cannot open file " << filename << endl;
    abort();
  }
  file->ls();
  TTree* treeOriginal = (TTree*) file->Get(treename);
  TH1D*  histOriginal = (TH1D*)  file->Get(histname);
  TTree* treeNew      = (TTree*) treeOriginal->Clone();
  TH1D*  histNew      = (TH1D*)  histOriginal->Clone();

  // Set-Write-Delete
  histNew->SetName ("sumOfMcWeights_Data" );
  histNew->SetTitle("sumOfMcWeights_Data" );
  histNew->Write   ("",TObject::kOverwrite);
  treeNew->SetName ("id_Data"             );
  treeNew->SetTitle("id_Data"             );
  treeNew->Write   ("",TObject::kOverwrite);
  file->Delete     (histname+";1"         );
  file->Delete     (treename+";1"         );

  // Close file
  file->Close();
}
