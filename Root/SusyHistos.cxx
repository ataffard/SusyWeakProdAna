#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "TCanvas.h"
#include "SusyWeakProdAna/SusyHistos.h"

using namespace std;

/*--------------------------------------------------------------------------------*/
// SusyHistos Constructor
/*--------------------------------------------------------------------------------*/
SusyHistos::SusyHistos()
{
}
/*--------------------------------------------------------------------------------*/
// SusyHistos SaveHistograms
/*--------------------------------------------------------------------------------*/
void SusyHistos::SaveHistograms(TDirectory* _hDir)
{
  _hDir->cd();

  string dir =  string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna";
  gSystem->mkdir(dir.c_str(),kTRUE);

  string fileName  = dir + "/" + "histo_" + _sample +  ".root";
  string title     = _sample + " histo file";
  TFile *histoFile = new TFile(fileName.c_str(),"RECREATE",title.c_str());

  TIter next(_hDir->GetList()); //Use GetListOfKeys when reading back file
  TObject* obj=0; 
  while((obj=next())) { 
    //std::cout << obj->GetName() << std::endl; 
    if (!obj->InheritsFrom("TH1")) continue; //skip anything that is not TH1
    obj->Write(); 
  } 
  
  histoFile->Close();
  
  cout << "Histograms saved " << histoFile->GetName() << endl;
  delete histoFile;

}



