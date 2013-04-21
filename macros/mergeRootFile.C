/*
 * Merge a number of .root files with histograms into one output file.
 * Assume that the TH don't need to be hadd
 *
 * Execute the function like this from the command line:
 *
 * echo outputfile.root input1.root input2.root... | root -b mergeRootFile.C+
 *
 */

#include <string>
#include <iostream>

#include "TList.h"
#include "TFile.h"
#include "TH1.h"
#include "TKey.h"
#include "Riostream.h"

void mergeRootFile()
{
    using namespace std;

    string outfile;
    cin >> outfile;
    
    std::string filename;

    cout << "Opening output file: " << outfile << endl;
    TFile output(outfile.c_str(), "RECREATE");
    

    while(cin >> filename) {
      TFile *f = new TFile(filename.c_str());
      cout << "Add content of file: " << filename << endl;
      
      TIter next(f->GetListOfKeys()); //Use GetListOfKeys when reading back file
      TKey* key=0;
      while( (key = (TKey*)next()) ){
	//      TObject* obj=0; 
	//while((obj=next())) { 
	f->cd();
	TObject* obj=key->ReadObj(); 
	//TH1 *h1 = (TH1*)obj;
	//std::cout << obj->GetName() << " " << obj->InheritsFrom("TH1") << std::endl;
	//std::cout << h1->GetName() << " " << h1->GetEntries() << std::endl; 
	//if (!obj->InheritsFrom("TH1")) continue; //skip anything that is not TH -- DOES NOT WORK ????
	output.cd();
	obj->Write( key->GetName() ); 
      }
      f->Close();
    }
    output.Write();
    output.Close();

}

