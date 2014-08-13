#include <string>
#include <iostream>


#include "TChain.h"
#include "Riostream.h"

void mergeToyNtFile()
{
    using namespace std;

    string outfile;
    cin >> outfile;

    std::string filename;

    TChain* nt = new TChain("ToyNt");
    
    string fileList;
    cin >> fileList;

    FILE* fPtr;
    fPtr = fopen(fileList.c_str(),"r"); 
    if( fPtr == NULL ){
      cout << "Cannot open file " << fileList.c_str() << ". Exitting" << endl;
      exit(1);
    }
    
    TChain* ntTmp = new TChain("ToyNt");

    while( !feof(fPtr)){
      char _name[200];
      fscanf(fPtr,"%s ",&_name[0]);
      string _ss = _name;
      uint   idx = _ss.find("ToyNtOutputs");
      string _subName = _ss.substr(idx+strlen("ToyNtOutputs/"));
      
      ntTmp->Add(_ss.c_str());
      cout << "\t Adding file " << _subName << " " << ntTmp->GetEntries() << endl;
      nt->Add(_ss.c_str());
      ntTmp->Reset();
    }

    
    cout << "Merged TChain saved in " << outfile << " total entries " << nt->GetEntries() << endl;
    nt->Merge(outfile.c_str());

}

