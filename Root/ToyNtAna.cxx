#define ToyNtAna_cxx
// The class definition in ToyNtAna.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("ToyNtAna.C")
// Root > T->Process("ToyNtAna.C","some options")
// Root > T->Process("ToyNtAna.C+")
//

#include "SusyWeakProdAna/ToyNtAna.h"
#include <TH2.h>
#include <TStyle.h>
#include <iomanip>
#include <iostream>

using namespace std;

/*--------------------------------------------------------------------------------*/
// SusyNtAna Constructor
/*--------------------------------------------------------------------------------*/
ToyNtAna::ToyNtAna(TTree * /*tree*/) : 
  fChain(0),
  m_entry(0), 
  m_dbg(0),
  m_dbgEvt(false)
{
  _histoDir = new TDirectory("Ana","Ana");

}

void ToyNtAna::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   m_chainEntry = -1;

   if(m_dbgEvt) loadEventList();

}

void ToyNtAna::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t ToyNtAna::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either ToyNtAna::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

  //Debug this event - check if should be processed
  if(m_dbgEvt && !processThisEvent(run, event)) return kFALSE;

   return kTRUE;
}

void ToyNtAna::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void ToyNtAna::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}

/*--------------------------------------------------------------------------------*/
// Load Event list of run/event to process. Use to debug events
/*--------------------------------------------------------------------------------*/
void ToyNtAna::loadEventList()
{
  int run, event;
  FILE* eventsFile=fopen("debugEvents.txt","r");
  int nEvtDbg=0;
  while(fscanf(eventsFile,"%i %i \n",&run, &event) != EOF){
    cout << "Adding run-event " << run << " " << event << endl; 
    addRunEvent(m_eventList, run, event);
    nEvtDbg++;
  }
  std::cout << " >>> Debuging " << nEvtDbg << " events " << std::endl;
}
/*--------------------------------------------------------------------------------*/
// Process selected events only
/*--------------------------------------------------------------------------------*/
bool ToyNtAna::processThisEvent(unsigned int run, unsigned int event)
{
  if(m_eventList.size()==0) return true;
  return checkRunEvent(m_eventList, run, event);
}
bool ToyNtAna::checkRunEvent(const RunEventMap &runEventMap, unsigned int run, unsigned int event)
{
  RunEventMap::const_iterator eventSetIter = runEventMap.find(run);
  return eventSetIter != runEventMap.end() && 
    eventSetIter->second->find(event) != eventSetIter->second->end();
}
bool ToyNtAna::checkAndAddRunEvent(RunEventMap &runEventMap, unsigned int run, unsigned int event)
{
  set<unsigned int> *&eventSet = runEventMap[run];
  if ( !eventSet )
    eventSet = new set<unsigned int>();
  return !eventSet->insert(event).second;
}
