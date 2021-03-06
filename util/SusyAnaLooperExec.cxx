
#include <cstdlib>
#include <string>

#include "TChain.h"
#include "Cintex/Cintex.h"

#include "SusyWeakProdAna/SusyAnaLooper.h"
#include "SusyNtuple/ChainHelper.h"

using namespace std;

/*

SusyAnaLooper - run analyses

To run data/MC as is use -method std
To run fake prediction use -method flep -useLoose
To run MC on prompt lep to combine w/ DD use -method rlep

*/

void help()
{
  cout << "  Options:"                          << endl;
  cout << "  -n number of events to process"    << endl;
  cout << "     defaults: -1 (all events)"      << endl;

  cout << "  -k number of events to skip"       << endl;
  cout << "     defaults: 0"                    << endl;

  cout << "  -d debug printout level"           << endl;
  cout << "     defaults: 0 (quiet) "           << endl;

  cout << "  -F name of single input file"      << endl;
  cout << "     defaults: ''"                   << endl;

  cout << "  -f name of input filelist"         << endl;
  cout << "     defaults: ''"                   << endl;

  cout << "  -D name of input file dir"         << endl;
  cout << "     defaults: ''"                   << endl;

  cout << "  -s sample name, for naming files"  << endl;
  cout << "     defaults: ntuple sample name"   << endl;

  cout << "  -dbgEvt                         "  << endl;
  cout << "     process events in debugEvents.txt " << endl;

  cout << "  -do2L                           "  << endl;
  cout << "     run 2L ana                    " << endl;

  cout << "  -doWH                            "  << endl;
  cout << "     run WH ana                    " << endl;

  cout << "  -do3L                           "  << endl;
  cout << "     run 3L ana                    " << endl;

  cout << "  -doFake                         "  << endl;
  cout << "     run Fake ana                  " << endl;

  cout << "  -doLFV                            "  << endl;
  cout << "     run LFV ana                    " << endl;

  cout << "  -useLoose                       "  << endl;
  cout << "     run 2L 3L using loose lepton " << endl;

  cout << "  -doAll                           "  << endl;
  cout << "     run All ana                   " << endl;

  cout << "  -doMll <true/false>             "  << endl;
  cout << "     run low mass alpgen - Mll<40 " << endl;

  cout << "  -sys1 <name1> -sys2  <name2>    "  << endl;
  cout << "     run  given sys1 and optionally sys2 " << endl;


  cout << "  -method                          "  << endl;
  cout << "     ana method: std: rlep, flep   " << endl;
  

  cout << "  -h print this help"                << endl;
}

int main(int argc, char** argv)
{
  ROOT::Cintex::Cintex::Enable();

  int nEvt    = -1;
  int nSkip   = 0;
  int dbg     = 0;
  bool dbgEvt = false; 
  bool do2L   = false;
  bool doWH   = false;
  bool do3L   = false;
  bool doFake = false;
  bool doLFV   = false;
  bool doMll  = false;
  bool useLoose = false;
  int method    = 0; //see SusyHisto.h 0:STD; 1:RLEP, 2:FLEP
  string smethod;
  string sys1="";
  string sys2="";
  string sample;
  string file;
  string fileList;
  string fileDir;
  bool eos=false;//true;
  

  cout << "SusyAnaLooperExec" << endl;
  cout << endl;

  /** Read inputs to program */
  for(int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-n") == 0)
      nEvt = atoi(argv[++i]);
    else if (strcmp(argv[i], "-k") == 0)
      nSkip = atoi(argv[++i]);
    else if (strcmp(argv[i], "-d") == 0)
      dbg = atoi(argv[++i]);
    else if (strcmp(argv[i], "-F") == 0)
      file = argv[++i];
    else if (strcmp(argv[i], "-f") == 0)
      fileList = argv[++i];
    else if (strcmp(argv[i], "-D") == 0)
      fileDir = argv[++i];
    else if (strcmp(argv[i], "-s") == 0)
      sample = argv[++i];
    else if (strcmp(argv[i], "-dbgEvt") == 0)
      dbgEvt = true;
    else if (strcmp(argv[i], "-do2L") == 0)
      do2L = true;
    else if (strcmp(argv[i], "-doWH") == 0)
      doWH = true;
    else if (strcmp(argv[i], "-do3L") == 0)
      do3L = true;
    else if (strcmp(argv[i], "-doFake") == 0)
      doFake = true;
    else if (strcmp(argv[i], "-doLFV") == 0)
      doLFV = true;
    else if (strcmp(argv[i], "-useLoose") == 0)
      useLoose = true;
    else if (strcmp(argv[i], "-doMll") == 0){
      string tmp = argv[++i];
      if(strcmp(tmp.c_str(),"false") ==0) doMll = false;
      if(strcmp(tmp.c_str(),"true")  ==0) doMll = true;
    }
    else if (strcmp(argv[i], "-doAll") == 0){
      do2L = true;
      doWH = true;
      do3L = true;
      doFake = true;
      doLFV = true;
    }
    else if (strcmp(argv[i], "-method") == 0){
      smethod = argv[++i];
      if(strcmp(smethod.c_str(),"std")==0) method = 0;
      if(strcmp(smethod.c_str(),"rlep")==0) method = 1;
      if(strcmp(smethod.c_str(),"flep")==0) {method = 2; useLoose=true;}
    }
    else if (strcmp(argv[i], "-sys1") == 0){
      sys1 = argv[++i];
    }
    else if (strcmp(argv[i], "-sys2") == 0){
      sys2 = argv[++i];
    }
    else
    {
      help();
      return 0;
    }
  }

  if(eos){
    string cmd =  string(getenv("WORKAREA")) + "/SusyWeakProdAna" + "/scripts/getInputFileListEOS.sh " 
      + sample + " " + fileDir + " " + smethod;
    gSystem->Exec(cmd.c_str());
  }


  // If no input specified except sample name, use a standard fileList
  if(file.empty() && fileList.empty() && fileDir.empty() && !sample.empty())
    fileList = "files/" + sample + ".txt";

  cout << "flags:" << endl;
  cout << "  sample    " << sample   << endl;
  cout << "  nEvt      " << nEvt     << endl;
  cout << "  nSkip     " << nSkip    << endl;
  cout << "  dbg       " << dbg      << endl;
  cout << "  dbgEvt    " << dbgEvt   << endl;
  cout << "  do2L      " << do2L     << endl;
  cout << "  doWH      " << doWH     << endl;
  cout << "  do3L      " << do3L     << endl;
  cout << "  doFake    " << doFake   << endl;
  cout << "  doLFV     " << doWH     << endl;
  cout << "  doMll     " << doMll    << endl;
  cout << "  useLoose  " << useLoose << endl;
  cout << "  method    " << method   << endl;
  cout << "  sys1      " << sys1     << endl;
  cout << "  sys2      " << sys2     << endl;

  if(!file.empty())     cout << "  input   " << file     << endl;
  if(!fileList.empty()) cout << "  input   " << fileList << endl;
  if(!fileDir.empty())  cout << "  input   " << fileDir  << endl;
  cout << endl;


  // Build the input chain
  TChain* chain = new TChain("susyNt");
  if(!eos){
    ChainHelper::addFile(chain, file);
    ChainHelper::addFileList(chain, fileList);
    ChainHelper::addFileDir(chain, fileDir);
  }
  else{
    //CERN EOS
    string eosFileList =  string(getenv("WORKAREA")) + "/SusyWeakProdAna" + "/scripts/EOSFiles/" 
      + sample + "_" + smethod + ".txt";
    cout << "Using EOS inputFileList " << eosFileList << endl; 
    ChainHelper::addFileList(chain, eosFileList);
  }

  Long64_t nEntries = chain->GetEntries();
  chain->ls();

  // Build the TSelector
  SusyAnaLooper* susyAna = new SusyAnaLooper(do2L, do3L, doWH, doFake, doLFV);
  susyAna->setDebug(dbg);
  susyAna->toggleCheckDuplicates(true);
  susyAna->setSampleName(sample);
  if(dbgEvt) susyAna->setEvtDebug();
  susyAna->doMll(doMll);
  susyAna->useLooseLep(useLoose);
  susyAna->setMethod(method);
  if(strcmp(sys2.c_str(),"")!=0)
    susyAna->doSysRange(sys1,sys2);
  else if(strlen(sys1.c_str())>0) 
    susyAna->setSystematic(sys1);

   //Grab sumW for the samples
   if(sample.find("period") == string::npos) {
     bool isSimplifiedModel = (sample.find("wA") != string::npos || sample.find("wC") != string::npos) ? true:false; 
     SumwMap mcSumOfWeights = susyAna->SusyNtTools::buildSumwMap(chain,isSimplifiedModel); 
     cout << "Using SumW map for simplifiedModel " << isSimplifiedModel << endl;
     susyAna->setSumwMap(mcSumOfWeights);
   }

  // Run the job
  if(nEvt<0) nEvt = nEntries;
  cout << endl;
  cout << "Total entries:   " << nEntries << endl;
  cout << "Process entries: " << nEvt << endl;

  //Try caching ahead
  /*
  Int_t cachesize = 100000000;          //100 MBytes
  chain->SetCacheSize(cachesize); 
  chain->AddBranchToCache("*",kTRUE);   //add all branches to the cache
  */

  if(nEvt>0) chain->Process(susyAna, sample.c_str(), nEvt, nSkip);


  delete chain;
  return 0;
}
