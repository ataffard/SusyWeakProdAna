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
				bool mcMll, bool isZAlpgenSherpa, 
				string sys1, string sys2)
{
  _hDir->cd();

  string dir =  string(getenv("HISTOANA")) + "/SusyAna";
  gSystem->mkdir(dir.c_str(),kTRUE);

  string fileName  = dir + "/" + "histo_" + _sample;
  if(isZAlpgenSherpa && mcMll){
    fileName = fileName + "_mll60";
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
				       bool mcMll, bool isZAlpgenSherpa, 
				       string sys1, string sys2)
{
  _hDir->cd(); //running dir where all the histos are cached

  string baseDir =  string(getenv("HISTOANA")) + "/SusyAna/histOutputs";
  gSystem->mkdir(baseDir.c_str(),kTRUE);

  TFile *histoFile[DIL_NSR][3];
  std::map<string, std::pair< int,int> > _label;  //eg: CR2LepOS_EE

  //Put  each SR/Lep combo in a dir otherwise too many files to ls
  for(int ilep=0; ilep<3; ilep++){	
    for(int isr=0; isr<DIL_NSR; isr++){ //SR/CR's loop
      string chanSR =  string(DIL_SRNAME[isr]) + "_" + string(DIL_FLAV[ilep]); 
      _label.insert(_label.end(),std::make_pair(string(chanSR), std::make_pair(ilep,isr)));
      string dir =  baseDir + "/" + chanSR; 
      gSystem->mkdir(dir.c_str(),kTRUE);

      string fileName  = dir + "/" + "histo_" + _sample;

      if(isZAlpgenSherpa && mcMll){
	fileName = fileName + "_mll60";
      }

      chanSR =  "_" + chanSR;
      fileName = fileName + chanSR;

      fileName = fileName + "_" + SMETHOD[method];
      if(strlen(sys1.c_str())>0) fileName = fileName + "_" + sys1;
      if(strlen(sys2.c_str())>0) fileName = fileName + "_" + sys2;
      
      fileName = fileName +".root";
      //cout << "File name " << fileName << endl;
      string title     = _sample + "_" + chanSR + " histo file";      
      histoFile[isr][ilep] = new TFile(fileName.c_str(),"RECREATE",title.c_str());
    }
  }

  //Looping over the histo and saving them in there appropriate files

  TIter next(_hDir->GetList()); //Use GetListOfKeys when reading back file
  TObject* obj=0; 
  while((obj=next())) { 
    if (!obj->InheritsFrom("TH1")) continue; //skip anything that is not TH1

    string _name =  string(obj->GetName());
    string _name2 = _name.substr(5); // DG2L_
    uint idx1 = _name2.find("_"); //idx of 1st _
    uint idx2 = _name2.find("_",idx1+1); //idx of 2nd _
    string _subName = _name2.substr(0,idx2);
    //cout << "histName " << _name <<  "\t histo name extracted " << _subName << endl;
      
    std::map<string, std::pair< int,int> >::const_iterator it = _label.find(_subName);
    if( it != _label.end()){
      int ilep = it->second.first;
      int isr = it->second.second;
      histoFile[isr][ilep]->cd();
      obj->Write();
    }
    else cout << "Error cannot find the file to associate " << _subName << endl;

  } 



  //Closing all the files
  for(int ilep=0; ilep<3; ilep++){	
    int count = 0;
    for(int isr=0; isr<DIL_NSR; isr++){ //SR/CR's loop
      count++;
      histoFile[isr][ilep]->Close();
      delete histoFile[isr][ilep];
    }
    cout << "Histograms for " << DIL_FLAV[ilep] << " "  << count << " files saved " << endl;
  }
  
}
