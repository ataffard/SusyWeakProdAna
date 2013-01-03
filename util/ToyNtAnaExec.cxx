
#include <cstdlib>
#include <string>

#include "TChain.h"
#include "TSystem.h"
#include "Cintex/Cintex.h"

#include "SusyWeakProdAna/ToyNt_ZXStudies.h"
#include "SusyNtuple/ChainHelper.h"

using namespace std;

void help()
{
  cout << "  Options:"                          << endl;

  cout << "  -F name of single input file"      << endl;
  cout << "     defaults: ''"                   << endl;

  cout << "  -f name of input filelist"         << endl;
  cout << "     defaults: ''"                   << endl;

  cout << "  -D name of input file dir"         << endl;
  cout << "     defaults: ''"                   << endl;

  cout << "  -s sample name, for naming files"  << endl;
  cout << "     defaults: ntuple sample name"   << endl;

  cout << "  -d debug printout level"           << endl;
  cout << "     defaults: 0 (quiet) "           << endl;

  cout << "  -dbgEvt                         "  << endl;
  cout << "     process events in debugEvents.txt " << endl;

  cout << "  -h print this help"                << endl;
}


int main(int argc, char** argv)
{
  ROOT::Cintex::Cintex::Enable();

  int nEvt    = -1;
  int nSkip   = 0;
  int dbg     = 0;
  bool dbgEvt = false; 
  string sample;
  string file;
  string fileList;
  string fileDir;

  //  string toyNtDir="/Users/anyes/MyWork/Atlas-Ana/SUSYAna_8TeV2012/histoAna/SusyAna/ToyNtOutputs";
  string toyNtDir=string(getenv("WORKAREA")) + "/histoAna/SusyAna/histos_121712_13fb_n0115_DD_MMtrial9_SYS/ToyNtOutputs";
  gSystem->Setenv("TOYNTDIR",toyNtDir.c_str());
  
  /** Read inputs to program */
  for(int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-F") == 0)
      file = argv[++i];
    else if (strcmp(argv[i], "-f") == 0)
      fileList = argv[++i];
    else if (strcmp(argv[i], "-D") == 0)
      fileDir = argv[++i];
    else if (strcmp(argv[i], "-s") == 0)
      sample = argv[++i];
    else if (strcmp(argv[i], "-d") == 0)
      dbg = atoi(argv[++i]);
    else if (strcmp(argv[i], "-dbgEvt") == 0)
      dbgEvt = true;
    else {
      help();
      return 0;
    }
  }

  // If no input specified except sample name, use a standard fileList
  if(file.empty() && fileList.empty() && 
     fileDir.empty() && !sample.empty())
    fileList = "files/" + sample + ".txt";

  cout << "flags:" << endl;
  cout << "  sample    " << sample   << endl;
  cout << "  dbg       " << dbg      << endl;
  cout << "  dbgEvt    " << dbgEvt   << endl;

  if(!file.empty())     cout << "  input   " << file     << endl;
  if(!fileList.empty()) cout << "  input   " << fileList << endl;
  if(!fileDir.empty())  cout << "  input   " << fileDir  << endl;
  cout << endl;

  // Build the input chain
  TChain* chain = new TChain("ToyNt");
  
  ChainHelper::addFile(chain, file);
  ChainHelper::addFileList(chain, fileList);
  ChainHelper::addFileDir(chain, fileDir);
  
  Long64_t nEntries = chain->GetEntries();
  chain->ls();
  
   // Build the TSelector
  ToyNt_ZXStudies* toyAna = new ToyNt_ZXStudies();
  toyAna->setSampleName(sample);
  toyAna->setDebug(dbg);
  if(dbgEvt) toyAna->setEvtDebug();

  // Run the job
  if(nEvt<0) nEvt = nEntries;
  cout << endl;
  cout << "Total entries:   " << nEntries << endl;
  cout << "Process entries: " << nEvt << endl;
  if(nEvt>0) chain->Process(toyAna, sample.c_str(), nEvt, nSkip);


  delete chain;
  return 0;

}
