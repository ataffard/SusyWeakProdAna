#include <iomanip>
#include "SusyWeakProdAna/SusyAnaLooper.h"



using namespace Susy;

/*--------------------------------------------------------------------------------*/
// SusyAnaLooper Constructor
/*--------------------------------------------------------------------------------*/
SusyAnaLooper::SusyAnaLooper():
  _do2LAna(false),
  _do3LAna(false),
  _doFakeAna(false)
{
  _histoDir = new TDirectory("Ana","Ana");
  _susyHistos = new SusyHistos();
}

/*--------------------------------------------------------------------------------*/
// The Begin() function is called at the start of the query.
/*--------------------------------------------------------------------------------*/
void SusyAnaLooper::Begin(TTree* /*tree*/)
{
  SusyNtAna::Begin(0);
  if(dbg()>0) cout << "SusyAnaLooper::Begin" << endl;
  
  _susyHistos->setSample(sampleName());

  if(_doFakeAna){
    _susyFakeAna = new SusyFakeAna(_susyHistos);
    _susyFakeAna->setDebug(dbg());
    _susyFakeAna->hookContainers(&nt,
				 &m_baseElectrons, &m_signalElectrons,
				 &m_baseMuons, &m_signalMuons,
				 &m_baseLeptons, &m_signalLeptons,
				 &m_baseJets, &m_signalJets);
    _susyHistos->BookFakeHistograms(_histoDir);

  }

  if(_do2LAna){
    _susy2LAna = new Susy2LepAna(_susyHistos);
    _susy2LAna->setDebug(dbg());
    _susy2LAna->hookContainers(&nt,
			       &m_baseElectrons, &m_signalElectrons,
			       &m_baseMuons, &m_signalMuons,
			       &m_baseLeptons, &m_signalLeptons,
			       &m_baseJets, &m_signalJets);
    _susyHistos->Book2LHistograms(_histoDir);
  }

  if(_do3LAna){
    _susy3LAna = new Susy3LepAna(_susyHistos);
    _susy3LAna->setDebug(dbg());
    _susy3LAna->hookContainers(&nt,
			       &m_baseElectrons, &m_signalElectrons,
			       &m_baseMuons, &m_signalMuons,
			       &m_baseLeptons, &m_signalLeptons,
			       &m_baseJets, &m_signalJets);
    _susyHistos->Book3LHistograms(_histoDir);
  }

}

/*--------------------------------------------------------------------------------*/
// Main process loop function 
/*--------------------------------------------------------------------------------*/
Bool_t SusyAnaLooper::Process(Long64_t entry)
{
  // Communicate tree entry number to SusyNtObject
  GetEntry(entry);
  clearObjects();

  m_chainEntry++;
  if(dbg()>0 || m_chainEntry%50000==0)
  {
    cout << "**** Processing entry " << setw(6) << m_chainEntry
         << " run " << setw(6) << nt.evt()->run
         << " event " << setw(7) << nt.evt()->event << " ****" << endl;
  }

  //Debug this event - check if should be processed
  if(m_dbgEvt && !processThisEvent(nt.evt()->run, nt.evt()->event)) return kFALSE;


  if(dbg()>0){
    cout<<"-----------------------------------"<<endl;
    cout<<"Run: "<<nt.evt()->run<<" Event "<<nt.evt()->event<<endl;
  }

  // grab base object and select signal objects
  selectObjects(NtSys_NOM);
  

  //perform event analysis
  if(_doFakeAna){
    _susyFakeAna->hookMet(m_met);
    _susyFakeAna->doAnalysis();
  }
  if(_do2LAna){
    _susy2LAna->hookMet(m_met);
    _susy2LAna->doAnalysis();
  }
  if(_do3LAna){
    _susy3LAna->hookMet(m_met);
    _susy3LAna->doAnalysis();
  }

  return kTRUE;
}

/*--------------------------------------------------------------------------------*/
// The Terminate() function is the last function to be called
/*--------------------------------------------------------------------------------*/
void SusyAnaLooper::Terminate()
{
  if(_do2LAna) _susy2LAna->end();
  if(_do3LAna) _susy3LAna->end();

  _susyHistos->SaveHistograms(_histoDir);

  SusyNtAna::Terminate();
  if(dbg()>0) cout << "SusyAnaLooper::Terminate" << endl;

}

