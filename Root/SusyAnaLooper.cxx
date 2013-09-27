#include <iomanip>
#include "SusyWeakProdAna/SusyAnaLooper.h"
#include "SusyWeakProdAna/SusyAnaCommon.h"


using namespace Susy;

/*--------------------------------------------------------------------------------*/
// SusyAnaLooper Constructor
/*--------------------------------------------------------------------------------*/
SusyAnaLooper::SusyAnaLooper(): 
  SusyNtAna(),
  _do2LAna(false), 
  _doWHAna(false),
  _doMll(false),
  _do3LAna(false),
  _doFakeAna(false),
  _useLooseLep(false),
  _method(STD),
  _systematic1(""),
  _systematic2(""),
  _runOneSys(false),
  _runSysRange(false),
  _isZAlpgenSherpa(false),
  nHFOR(0),
  nMllCut(0)
{
  //setAnaType(Ana_2Lep);
  setSelectTaus(true);

 
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

  cout << "--------------------------" << endl;
  cout << "SusyAnaLooper " << endl;
  cout << "  Ana type " << endl;
  cout << "     2LAna " << _do2LAna << endl;
  cout << "     WHAna " << _doWHAna << endl;
  cout << "     3LAna " << _do3LAna << endl;
  cout << "  Select Taus: true " << endl;
  cout << "--------------------------" << endl;



  cout << ">>> Weighting MC to " << pLUMI << " fb^-1 corresponding to " << LUMW << endl;
  cout << " \t output dir " << DATE.c_str() << endl;
  printSettings();

  if(_doMll) cout << " \t Mll cut  toggled for Sherpa and Alpgen " << MLLCUT << endl;

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
			       &m_preElectrons, &m_baseElectrons, &m_signalElectrons,
			       &m_preMuons, &m_baseMuons, &m_signalMuons,
			       &m_baseLeptons, &m_signalLeptons,
			       &m_preJets, &m_baseJets, &m_signalJets2Lep,
			       &m_baseTaus, &m_signalTaus);
    _susyHistos->Book2LHistograms(_histoDir,DO_SYS);
    if(_isSleptonGrid) _susy2LAna->setSleptonSumWs(_sleptonSumWs);
    _susy2LAna->setMCSumWs(getSumwMap());

    if(DO_SYS){
      if(_runOneSys || _runSysRange){
	if(_systematic1.length()>2){
	  int minSys=getSysIndex(_systematic1);
	  if(_runOneSys)   _susy2LAna->setMcSysMinMax(minSys,minSys);
	  else if(_runSysRange){
	    int maxSys=getSysIndex(_systematic2);
	    _susy2LAna->setMcSysMinMax(minSys, maxSys);
	  }
	}
	else {
	  _systematic1="";
	  _systematic2="";
	  _runOneSys=false;
	  _runSysRange=false;
	  _susy2LAna->setMcSysMinMax();
	}
      }
      else _susy2LAna->setMcSysMinMax();
    }
    
  }
  if(_doWHAna){
    _susyWHAna = new SusyWHAna(_susyHistos);
    _susyWHAna->setDebug(dbg());
    _susyWHAna->setUseLooseLep(_useLooseLep);
    _susyWHAna->setMethod(_method);
    _susyWHAna->hookContainers(&nt,
			       &m_preElectrons, &m_baseElectrons, &m_signalElectrons,
			       &m_preMuons, &m_baseMuons, &m_signalMuons,
			       &m_baseLeptons, &m_signalLeptons,
			       &m_preJets, &m_baseJets, &m_signalJets2Lep,
			       &m_baseTaus, &m_signalTaus);
    _susyHistos->BookWHHistograms(_histoDir,DO_SYS);
    _susyWHAna->setMCSumWs(getSumwMap());

    if(DO_SYS){
      if(_runOneSys || _runSysRange){
	if(_systematic1.length()>2){
	  int minSys=getSysIndex(_systematic1);
	  if(_runOneSys)   _susyWHAna->setMcSysMinMax(minSys,minSys);
	  else if(_runSysRange){
	    int maxSys=getSysIndex(_systematic2);
	    _susyWHAna->setMcSysMinMax(minSys, maxSys);
	  }
	}
	else {
	  _systematic1="";
	  _systematic2="";
	  _runOneSys=false;
	  _runSysRange=false;
	  _susyWHAna->setMcSysMinMax();
	}
      }
      else _susyWHAna->setMcSysMinMax();
    }
    
  }
  if(_do3LAna){
    _susy3LAna = new Susy3LepAna(_susyHistos);
    _susy3LAna->setDebug(dbg());
    _susy3LAna->setUseLooseLep(_useLooseLep);
    _susy3LAna->hookContainers(&nt,
			       &m_preElectrons, &m_baseElectrons, &m_signalElectrons,
			       &m_preMuons, &m_baseMuons, &m_signalMuons,
			       &m_baseLeptons, &m_signalLeptons,
			       &m_preJets, &m_baseJets, &m_signalJets2Lep,
			       &m_baseTaus, &m_signalTaus);

    _susyHistos->Book3LHistograms(_histoDir);
    _susy3LAna->setMCSumWs(getSumwMap());


    if(DO_SYS){
      if(_runOneSys || _runSysRange){
	if(_systematic1.length()>2){
	  int minSys=getSysIndex(_systematic1);
	  if(_runOneSys)   _susy3LAna->setMcSysMinMax(minSys,minSys);
	  else if(_runSysRange){
	    int maxSys=getSysIndex(_systematic2);
	    _susy3LAna->setMcSysMinMax(minSys, maxSys);
	  }
	}
	else {
	  _systematic1="";
	  _systematic2="";
	  _runOneSys=false;
	  _runSysRange=false;
	  _susy3LAna->setMcSysMinMax();
	}
      }
      else _susy3LAna->setMcSysMinMax();
    }
    
  }


}

/*--------------------------------------------------------------------------------*/
// Analysis settings/flags
/*--------------------------------------------------------------------------------*/
void SusyAnaLooper::printSettings()
{
  string _path =  "/histos_" + DATE;

  cout << " ================================"       <<endl;
  cout << " SusyAnaCommon Flags " << endl;
  cout << "   Destination       " << _path          << endl;
  cout << "   LUMIMODE          " << LUMIMODE       << endl;
  cout << "   LUMI              " << LUMW           << endl;
  cout << "   pLUMI             " << pLUMI          << endl;
  cout << "   MINRUM            " << MINRUN         << endl; 
  cout << "   MAXRUM            " << MAXRUN         << endl; 
  cout << endl;
  cout << " 2L/WH Settings " << endl;
  cout << "   NBASELEPMIN       " << NBASELEPMIN    << endl;
  cout << "   NBASELEPMAX       " << NBASELEPMAX    << endl;
  if(_doMll)
    cout << "   MLL for Z's       " << MLLCUT         << endl;
  cout << endl;
  cout << "   BLIND_DATA        " << BLIND_DATA     << endl;
  cout << "   BLIND_MC          " << BLIND_MC       << endl;
  cout << endl;
  cout << "   WEIGHT_COUNT      " << WEIGHT_COUNT   << endl;
  cout << "   DO_FILL_HISTO     " << DO_FILL_HISTO  << endl;
  cout << "   USE_DGWEIGHT      " << USE_DGWEIGHT   << endl;
  cout << "   USE_BWEIGHT       " << USE_BWEIGHT    << endl;
  cout << "   USE_LEPSF         " << USE_LEPSF      << endl;
  cout << "   USE_QFLIP         " << USE_QFLIP      << endl;
  cout << "   USE_MCWEIGHT      " << USE_MCWEIGHT   << endl;
  cout << "   USE_MCTRIG        " << USE_MCTRIG     << endl;
  cout << endl;
  cout << "   DO_SYS            " << DO_SYS         << endl;
  cout << "   FILL_HFT          " << FILL_HFT       << endl;
  cout << endl;
  cout << "   FILL_TOYNT        " << FILL_TOYNT     << endl;
  cout << "   TOYNT_SR          " << TOYNT_iSR      << endl;
  cout << endl;
  cout << "   DUMP EVENT        " << DUMP_RUNEVT    << endl;
  cout << "   CUTFLOW           " << CUTFLOW        << endl;
  cout << " ================================"       <<endl;
  cout << endl;

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
  if(m_chainEntry%50000==0) {
    cout << "**** Processing entry " << setw(6) << m_chainEntry
         << " run " << setw(6) << nt.evt()->run
         << " event " << setw(7) << nt.evt()->event << " ****" << endl;
  }

  //Debug this event - check if should be processed
  if(m_dbgEvt && !processThisEvent(nt.evt()->run, nt.evt()->event))  return kFALSE;
  
  if(!nt.evt()->isMC && nt.evt()->run < MINRUN) return kFALSE;
  if(!nt.evt()->isMC && nt.evt()->run > MAXRUN) return kFALSE;

  if(nt.evt()->hfor==4 && 
     !( (nt.evt()->mcChannel >= 164440 && nt.evt()->mcChannel <= 164443) ||
       (nt.evt()->mcChannel >= 164450 && nt.evt()->mcChannel <= 164453) )
     ){
    nHFOR++;
    return kFALSE;
  }

  //
  // Sherpa - Alpgen patch
  //
  if(_doMll){
    //Sherpa - upper cut mll<60
    if((nt.evt()->mcChannel>=147770 && nt.evt()->mcChannel<=147772) || //masselss b/c
       (nt.evt()->mcChannel>=128975 && nt.evt()->mcChannel<=128977) || //Heavy/light
       (nt.evt()->mcChannel>=146820 && nt.evt()->mcChannel<=146822)  
       ){
      _isZAlpgenSherpa=true;
      if(nt.evt()->mllMcTruth > MLLCUT){ nMllCut++; return kFALSE;}
    }
    //AlpgenPythia LF/HF lower cut mll>60
    if((nt.evt()->mcChannel>=110805 && nt.evt()->mcChannel<=110828) || //HF bb/cc
       (nt.evt()->mcChannel>=117650 && nt.evt()->mcChannel<=117675)    //LF
       ){
      _isZAlpgenSherpa=true;
      if(nt.evt()->mllMcTruth < MLLCUT){ nMllCut++; return kFALSE;}
    }
  }
  /*
  if((nt.evt()->mcChannel>=146830 && nt.evt()->mcChannel<=146855)){
    _isAlpgenLowMass=true;
    if(_doMll){ //Reject Alpgen low mass with Mll>40 - To patch w/ Sherpa
      if(!nt.evt()->passMllForAlpgen) return kFALSE;
    }
  }
  */

  //Check Duplicate run:event in data
  if(!nt.evt()->isMC && checkDuplicate()){
    if(isDuplicate(nt.evt()->run, nt.evt()->event))  return kFALSE;
  }

  if(dbg()>1){
    cout<<"-----------------------------------"<<endl;
    cout<<"Run: "<<nt.evt()->run<<" Event "<<nt.evt()->event<<endl;
  }

  // grab base object and select signal objects
  uint iSys=DGSys_NOM;
  if(_do2LAna || _doWHAna ||_do3LAna){
    uint minSys=DGSys_NOM;
    uint maxSys=DGSys_NOM+1;
    if(DO_SYS){
      maxSys=DGSys_N;  
      if(_runOneSys){
	minSys=getSysIndex(_systematic1);
	if(minSys==DGSys_N){
	  cerr << " Requested systematic " << _systematic1 << " not found - Aborting" << endl;
	  abort();
	}
	maxSys=minSys+1;
      }
      if(_runSysRange){
	minSys=getSysIndex(_systematic1);
	maxSys=getSysIndex(_systematic2)+1;
      }
    }
    
    //Speed processing when not dumping cutflow....
    if(!CUTFLOW){
      int nNtLep = nt.ele()->size() + nt.muo()->size();
      int nNtTau = nt.tau()->size();
      if(nNtLep < 2) return false;
      //if(m_nTauMin >= 0 && nNtTau < m_nTauMin) return false;
    }

    for(uint iiSyst=minSys; iiSyst<maxSys; iiSyst++){     //Syst Looper
      if(dbg()>10) cout << "Do sys? " << DGSystNames[iiSyst] <<endl;

      if( !nt.evt()->isMC && iiSyst>DGSys_NOM){
	if(_method==FLEP){
	  if(iiSyst < DGSys_FAKE_EL_RE_UP || iiSyst>DGSys_FAKE_MU_FR_DN){
	    if(dbg()>10) cout << "\tNot fake sys - skip " << DGSystNames[iiSyst] <<endl;
	    continue;   //DD fake  - fake sys only
	  }
	}
	else if(_method!=FLEP) break; // done here
      }
      if(nt.evt()->isMC && iiSyst >= DGSys_FAKE_EL_RE_UP && iiSyst<=DGSys_FAKE_MU_FR_DN) continue; //DD Fake sys

      //Skip spare sys.
      if(iiSyst>= DGSys_GEN) break; // done here
     
      if(dbg()>1) cout << "  Processing Sys " << iiSyst << " " << DGSystNames[iiSyst] <<endl;
      
      clearObjects();
      if(iiSyst<=DGSys_RESOST) //Only for sys up to trigger SF need reload the SusyNt obj
	selectObjects( (SusyNtSys) iiSyst, false, TauID_medium);
      else
	selectObjects((SusyNtSys) DGSys_NOM, false, TauID_medium);
      
      if(dbg()>5) dumpEvent();
      
      if(_do2LAna){
	_susy2LAna->hookMet(m_met);
	_susy2LAna->doAnalysis(iiSyst);
      }
      if(_doWHAna){
	_susyWHAna->hookMet(m_met);
	_susyWHAna->doAnalysis(iiSyst);
      }
      if(_do3LAna){
	_susy3LAna->hookMet(m_met);
	_susy3LAna->doAnalysis(iiSyst);
      }
    }   
   }
  else{
    selectObjects((SusyNtSys) iSys);
    if(dbgEvt()) dumpEvent();

    //perform event analysis
    if(_doFakeAna){
      _susyFakeAna->hookMet(m_met);
      _susyFakeAna->doAnalysis();
    }
   
  }

  return kTRUE;
}

/*--------------------------------------------------------------------------------*/
// The Terminate() function is the last function to be called
/*--------------------------------------------------------------------------------*/
void SusyAnaLooper::Terminate()
{
  if(_do2LAna) _susy2LAna->end();
  if(_doWHAna) _susyWHAna->end();
  if(_do3LAna) _susy3LAna->end();

  TString _SS(sampleName());

  if( _SS.Contains("simplifiedModel") ||
      _SS.Contains("sM_wA") ||
      _SS.Contains("DGemt") ||
      _SS.Contains("DLiSlep") ){
    _susyHistos->SaveHistograms(_histoDir,_method,
				_doMll,_isZAlpgenSherpa,
				_systematic1, _systematic2);
  }
  else{
    if(_do2LAna)
      _susyHistos->SaveSplit2LHistograms(_histoDir,_method,
					 _doMll,_isZAlpgenSherpa,
					 _systematic1, _systematic2);

    if(_doWHAna)
      _susyHistos->SaveSplitWHHistograms(_histoDir,_method,
					 _doMll,_isZAlpgenSherpa,
					 _systematic1, _systematic2);


    if(_do3LAna)
      _susyHistos->SaveSplit3LHistograms(_histoDir,_method,
					 _doMll,_isZAlpgenSherpa,
					 _systematic1, _systematic2);

  }



  SusyNtAna::Terminate();
  if(dbg()>0) cout << "SusyAnaLooper::Terminate" << endl;
  cout << "Number of event rejected by HFOR " << nHFOR <<endl;
  cout << "Number of event rejected by MllCut " << nMllCut <<endl;

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
