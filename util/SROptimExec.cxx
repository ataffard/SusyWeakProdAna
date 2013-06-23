#include <cstdlib>
#include <string>

/*
  //Make Zn plot
  ./SROptimExec -d 1 -r 5 -p 3 -skim DIL_optimSRjets -sig -tabB -tabS

  ./SROptimExec -d 1 -r 7 -p 2 -dil MM -skim DIL_optimSRSS -sig -tabB -tabS


  //Combine Zn
   ./SROptimExec -d 1 -r 5 -p 3 -skim DIL_optimSRjets -combZn


Make sure to update the allBkg.txt file link 
*/


#include "Cintex/Cintex.h"

#include "SusyWeakProdAna/SROptimization.h"

using namespace std;

void help()
{
  cout << "  Options:"                            << endl;

  cout << "  -d sets the debug level"             << endl;
  cout << "     default is 0 (off)"               << endl;

  cout << "  -r Specify Runtime option "          << endl;
  cout << "     0 -- Maximum sig scanning yield"  << endl;
  cout << "     default: 0 "                      << endl;
  cout << "     See RegionOption "                << endl;

  cout << "  -skim Specify ToyNt skim used "      << endl;
  cout << "     eg: DIL_optimSRjets "             << endl;

  cout << "  -dil Specify dil type to use  "      << endl;
  cout << "   ALL, EE, MM, EM "                   << endl;

  cout << "  -p Specify the process to look at "  << endl;
  cout << "     0 -- Slepton grid"                << endl;
  cout << "     1 -- Mode C grid"                 << endl;
  cout << "     2 -- Mode A w/ slepton grid"      << endl;
  cout << "     3 -- Mode A no slep hadW grid"    << endl;
  cout << "     default is 0       "              << endl;

  cout << "  -sig Calculate significance"         << endl;
  cout << "  -maxsig Calculate Max sig for var"   << endl;

  cout << "  -tabS Dump Signal Table "            << endl;
  cout << "  -tabB Dump Bkg Table "               << endl;

  cout << "  -combZn Combine significance plots " << endl;
  cout << "    listed in ZnList.txt             " << endl;

  cout << "  -h print this help"                << endl;
}

int main(int argc, char** argv)
{
  ROOT::Cintex::Cintex::Enable();

  int dbg = 0;
  RegionOption opt = RO_yield;
  SusyProcess proc = SP_slepton;
  string skim = "";
  string dilType = "ALL";
  bool plotSig = false;
  bool maxSig = false;
  bool makeTableSignal = false;
  bool makeTableBkg = false;
  bool combZn = false;

  /** Read inputs to program */
  for(int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-d") == 0)
      dbg = atoi(argv[++i]);
    else if (strcmp(argv[i], "-r") == 0)
      opt = (RegionOption) atoi(argv[++i]);
    else if (strcmp(argv[i], "-p") == 0)
      proc = (SusyProcess) atoi(argv[++i]);
    else if (strcmp(argv[i], "-skim") == 0)
      skim = argv[++i];
    else if (strcmp(argv[i], "-dil") == 0)
      dilType = argv[++i];
    else if (strcmp(argv[i], "-sig") == 0)
      plotSig = true;
    else if (strcmp(argv[i], "-maxsig") == 0)
      maxSig = true;
    else if (strcmp(argv[i], "-tabS") == 0)
      makeTableSignal = true;
    else if (strcmp(argv[i], "-tabB") == 0)
      makeTableBkg= true;
    else if (strcmp(argv[i], "-combZn") == 0)
      combZn = true;
    else{
      help();
      return 0;
    }
  }

  // Options
  cout << "-------------------------------------------" << endl;
  cout << "flags:" << endl;
  cout << "  dbg                      " << dbg      << endl;
  cout << "  Region                   " << opt      << endl;
  cout << "  Skim                     " << skim     << endl;
  cout << "  Dil type                 " << dilType  << endl;
  cout << "  Susy grid                " << proc     << endl;
  cout << "  plot sig                 " << plotSig  << endl;
  cout << "  plot max sign            " << maxSig   << endl;
  cout << "  Signal Table             " << makeTableSignal << endl;
  cout << "  Bkg Table                " << makeTableBkg << endl;
  cout << endl;
  cout << "-------------------------------------------" << endl;  
  
  // Create instance of the class:
  SROptimization* plot = new SROptimization(opt, proc, skim, dilType);
  plot->setDebug(dbg);
  plot->init();  

  if(plotSig)         plot->PlotSig();
  if(maxSig)          plot->PlotMaxSig();
  if(makeTableSignal) plot->dumpSignalTable();
  if(makeTableBkg)    plot->dumpBkgTable();
  if(combZn)          plot->combineZn();

  cout << endl;
  cout << "Plotting job done" << endl;

  return 0;
}
