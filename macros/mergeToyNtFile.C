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

    while( !feof(fPtr)){
      char _name[200];
      fscanf(fPtr,"%s ",&_name[0]);
      string _ss = _name;
      uint   idx = _ss.find("ToyNtOutputs");
      string _subName = _ss.substr(idx+strlen("ToyNtOutputs/"));
      
      cout << "\t Adding file " << _subName << endl;
      nt->Add(_ss.c_str());
    }

    
    cout << "Merged TChain saved in " << outfile << endl;
    nt->Merge(outfile.c_str());

}

