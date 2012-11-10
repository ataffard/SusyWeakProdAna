/*

root -l 'ChangeFakeTreeName.C+("Egamma.periodA")'

*/

#include <iostream>
#include <string>

#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include "TObject.h"
#include "TH1.h"

void ChangeFakeTreeName(std::string suffix){

  TString _pathFile(string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/HFT_tmp/");

  vector<string> SYS; 
  SYS.push_back("NOM");  
  /*
  SYS.push_back("FAKE_EL_RE_UP");  
  SYS.push_back("FAKE_EL_RE_DN");
  SYS.push_back("FAKE_EL_FR_UP");
  SYS.push_back("FAKE_EL_FR_DN");
  SYS.push_back("FAKE_MU_RE_UP");
  SYS.push_back("FAKE_MU_RE_DN");
  SYS.push_back("FAKE_MU_FR_UP");
  SYS.push_back("FAKE_MU_FR_DN");
  */
  
  for(unsigned int i=0; i<SYS.size(); i++){
    // Define names
    TString filename = "", treename = "", histname = "";
    filename.Form("%s_%s_FAKE.root",SYS[i].c_str(),suffix.c_str());
    treename.Form("id_%s_FAKE",suffix.c_str());
    histname.Form("sumOfMcWeights_%s_FAKE",suffix.c_str());
    
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
    histNew->SetName ("sumOfMcWeights_Fake" );
    histNew->SetTitle("sumOfMcWeights_Fake" );
    histNew->Write   ("",TObject::kOverwrite);
    treeNew->SetName ("id_Fake"             );
    treeNew->SetTitle("id_Fake"             );
    treeNew->Write   ("",TObject::kOverwrite);
    file->Delete     (histname+";1"         );
    file->Delete     (treename+";1"         );
    
    // Close file
    file->Close();
 }
}

