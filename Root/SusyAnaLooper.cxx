#include <iomanip>
#include "SusyWeakProdAna/SusyAnaLooper.h"
#include "SusyWeakProdAna/SusyAnaCommon.h"


using namespace Susy;

/*--------------------------------------------------------------------------------*/
// SusyAnaLooper Constructor
/*--------------------------------------------------------------------------------*/
SusyAnaLooper::SusyAnaLooper():
  _do2LAna(false),
  _doMll(false),
  _do3LAna(false),
  _doFakeAna(false),
  _useLooseLep(false),
  _method(STD),
  _isAlpgenLowMass(false),
  nHFOR(0)
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

  cout << ">>> Weighting MC to " << pLUMI << " fb^-1 corresponding to " << LUMW << endl;
  cout << " \t output dir " << DATE.c_str() << endl;

  if(_doMll) cout << " \t Mll40 toggled for lowMass Alpgen " << endl;

  _susyHistos->setSample(sampleName());

  if(_doFakeAna){
    _susyFakeAna = new SusyFakeAna(_susyHistos);
    _susyFakeAna->setDebug(dbg());
    _susyFakeAna->hookContainers(&nt,
				 &m_baseElectrons, &m_signalElectrons,
				 &m_baseMuons, &m_signalMuons,
				 &m_baseLeptons, &m_signalLeptons,
				 &m_baseJets, &m_signalJets2Lep);
    _susyHistos->BookFakeHistograms(_histoDir);
  }

  if(_do2LAna){
    _susy2LAna = new Susy2LepAna(_susyHistos);
    _susy2LAna->setDebug(dbg());
    _susy2LAna->setUseLooseLep(_useLooseLep);
    _susy2LAna->setMethod(_method);
    _susy2LAna->hookContainers(&nt,
			       &m_baseElectrons, &m_signalElectrons,
			       &m_baseMuons, &m_signalMuons,
			       &m_baseLeptons, &m_signalLeptons,
			       &m_baseJets, &m_signalJets2Lep);
    _susyHistos->Book2LHistograms(_histoDir);
  }

  if(_do3LAna){
    _susy3LAna = new Susy3LepAna(_susyHistos);
    _susy3LAna->setDebug(dbg());
    _susy3LAna->setUseLooseLep(_useLooseLep);
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
  if(m_chainEntry%50000==0)
  {
    cout << "**** Processing entry " << setw(6) << m_chainEntry
         << " run " << setw(6) << nt.evt()->run
         << " event " << setw(7) << nt.evt()->event << " ****" << endl;
  }

  //Debug this event - check if should be processed
  if(m_dbgEvt && !processThisEvent(nt.evt()->run, nt.evt()->event))  return kFALSE;
  
  if(!nt.evt()->isMC && nt.evt()->run > MAXRUN) return kFALSE;

  if(nt.evt()->hfor==4){
    nHFOR++;
    return kFALSE;
  }

  //TO DO Add HF bb/cc w/ mll cut!!!
  if((nt.evt()->mcChannel>=146830 && nt.evt()->mcChannel<=146855) 
    //|| //low mass
    //     (nt.evt()->mcChannel>=109300 && nt.evt()->mcChannel<=109313) || //Zeebb Zmmbb Zttbb
    //(nt.evt()->mcChannel>=126414 && nt.evt()->mcChannel<=126421) || //Zeecc Zmmcc
    //     (nt.evt()->mcChannel>=117706 && nt.evt()->mcChannel<=117709)    //Zttcc 
     ){
    _isAlpgenLowMass=true;
    if(_doMll){ //Reject Alpgen low mass with Mll>40 - To patch w/ Sherpa
      if(!nt.evt()->passMllForAlpgen) return kFALSE;
    }
  }

  
  if(dbg()>0){
    cout<<"-----------------------------------"<<endl;
    cout<<"Run: "<<nt.evt()->run<<" Event "<<nt.evt()->event<<endl;
  }

  // grab base object and select signal objects
  SusyNtSys iSys=NtSys_NOM;
  //SusyNtSys iSys=NtSys_RESOST;
  selectObjects(iSys);
  
  if(dbgEvt()) dumpEvent();

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

  _susyHistos->SaveHistograms(_histoDir,_method,_doMll,_isAlpgenLowMass);

  SusyNtAna::Terminate();
  if(dbg()>0) cout << "SusyAnaLooper::Terminate" << endl;
  cout << "Number of event rejected by HFOR " << nHFOR <<endl;

}

/*--------------------------------------------------------------------------------*/
void SusyAnaLooper::dumpEvent()
{
  cout << ">>> ";  nt.evt()->print();
  
  cout << "Baseline objects" << endl;
  for(uint iEl=0; iEl < m_baseElectrons.size(); iEl++){
    cout << "  ";
    m_baseElectrons[iEl]->print();
  }
  for(uint iMu=0; iMu < m_baseMuons.size(); iMu++){
    cout << "  ";
    m_baseMuons[iMu]->print();
  }
  for(uint iJ=0; iJ < m_baseJets.size(); iJ++){
    cout << "  ";
    m_baseJets[iJ]->print();
  }

  cout << "Signal objects" << endl;
  for(uint iEl=0; iEl < m_signalElectrons.size(); iEl++){
    cout << "  ";
    m_signalElectrons[iEl]->print();
  }
  for(uint iMu=0; iMu < m_signalMuons.size(); iMu++){
    cout << "  ";
    m_signalMuons[iMu]->print();
  }
  for(uint iJ=0; iJ < m_signalJets.size(); iJ++){
    cout << "  ";
    m_signalJets[iJ]->print();
  }
}
