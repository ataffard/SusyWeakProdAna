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
void SusyHistos::SaveHistograms(TDirectory* _hDir,
				int method,
				bool mcMll, bool isAlpgen, 
				string sys1, string sys2)
{
  _hDir->cd();

  string dir =  string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna";
  gSystem->mkdir(dir.c_str(),kTRUE);

  string fileName  = dir + "/" + "histo_" + _sample;
  if(isAlpgen){
    if(mcMll) fileName = fileName + "_mll40";
    else  fileName = fileName + "_mll60";
  }
  fileName = fileName + "_" + SMETHOD[method];
  if(strlen(sys1.c_str())>0) fileName = fileName + "_" + sys1;
  if(strlen(sys2.c_str())>0) fileName = fileName + "_" + sys2;
  
  fileName = fileName +".root";

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



/*--------------------------------------------------------------------------------*/
// SusyHistos SaveHistograms
// Split ouput file as 
/*--------------------------------------------------------------------------------*/
void SusyHistos::SaveSplit2LHistograms(TDirectory* _hDir,
				       int method,
				       bool mcMll, bool isAlpgen, 
				       string sys1, string sys2)
{
  _hDir->cd();

  string dir =  string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/temp_histOutputs";
  gSystem->mkdir(dir.c_str(),kTRUE);




  string fileName  = dir + "/" + "histo_" + _sample;
  if(isAlpgen){
    if(mcMll) fileName = fileName + "_mll40";
    else  fileName = fileName + "_mll60";
  }
  fileName = fileName + "_" + SMETHOD[method];
  if(strlen(sys1.c_str())>0) fileName = fileName + "_" + sys1;
  if(strlen(sys2.c_str())>0) fileName = fileName + "_" + sys2;
  
  fileName = fileName +".root";

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
