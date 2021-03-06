#include <iomanip>
#include <iostream>
#include "TCanvas.h"
#include "SusyWeakProdAna/SusyWHAna.h"
#include "SusyWeakProdAna/PhysicsTools.h"

#include "SusyWeakProdAna/SusyAnaCommon.h"
//#include "SusyWeakProdAna/DsidGroups.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// SusyWHAna Constructor
/*--------------------------------------------------------------------------------*/
SusyWHAna::SusyWHAna(SusyHistos* _histos):
  SusyBaseAna(_histos,false,true,true)
{
  cout << "-------------------------------" << endl;
  cout << "    Running WH Analysis        " << endl;
  cout << "-------------------------------" << endl;
}

/*--------------------------------------------------------------------------------*/
// Main process loop function 
/*--------------------------------------------------------------------------------*/
void SusyWHAna::doAnalysis(float w, unsigned int isys)
{
  reset();
  SYST = isys;

  float evtW = w;
  if((isys == DGSys_XS_UP || isys == DGSys_XS_DN) 
     && !isSimplifiedModelGrid(nt->evt()->mcChannel) ){ //Get Xs uncertainty from local implementation
    float uncert = getXsUncert(nt->evt()->mcChannel);
    if(isys == DGSys_XS_UP) evtW *= 1 + uncert;
    if(isys == DGSys_XS_DN) evtW *= 1 - uncert;
  }
  // Get uncertainty on WZ 
  if(nt->evt()->isMC && (SYST == DGSys_GEN_UP || SYST == DGSys_GEN_DN) ){
    int nCJets = numberOfCLJets(*v_sigJet,m_jvfTool, (SusyNtSys) DGSys_NOM, m_anaType);   
    float uncert = getWZUncertainty(nt->evt()->mcChannel,nCJets);
    if(SYST == DGSys_GEN_UP) evtW *= 1 + uncert;
    if(SYST == DGSys_GEN_DN) evtW *= 1 - uncert;
  }

  if(FILL_TOYNT && isys==DGSys_NOM){
    bool metDetails      = false;
    bool dijetBlock      = true;
    bool OS2LBlock       = true;
    bool SS2LBlock       = true;
    bool ZBalanceBlock   = false;
    bool diversVarsBlock = false;

    initializeToyNt(metDetails, dijetBlock, 
		    OS2LBlock, SS2LBlock,
		    ZBalanceBlock, diversVarsBlock);
  }

  //Do selection for SR/CR/N-reg & fill plots
  if(m_useLooseLep){  //use baseline leptons - for fake MM estimate
    if(!CUTFLOW && v_baseLep->size()<2) return;
    if(!selectEvent(v_baseLep, v_baseLep, v_sigJet, m_met, evtW)) return;
  }
  else{
    if(!CUTFLOW && v_sigLep->size()<2) return;
    if(!selectEvent(v_sigLep, v_baseLep, v_sigJet, m_met, evtW)) return;
  }
  return;
}

/*--------------------------------------------------------------------------------*/
// End of job
/*--------------------------------------------------------------------------------*/
void SusyWHAna::end()
{
  cout << endl;
  cout << endl;
  cout << "**********************************************" << endl;
  cout << "SusyWHAna::event counters" <<endl;
  cout << std::setprecision(0) << std::fixed;
  cout << "read in:            " << n_readin        << endl;
  cout << "pass SUSY grid:     " << n_pass_SUSYGrid << endl;
  cout << "pass GRL:           " << n_pass_GRL      << endl;
  cout << "pass TileTrip:      " << n_pass_TileTrip << endl;
  cout << "pass LarErr:        " << n_pass_LarErr   << endl;
  cout << "pass BadJet:        " << n_pass_BadJet  << endl;
  cout << "pass DeadRegion:    " << n_pass_DeadRegion << endl;
  cout << "pass BadMu:         " << n_pass_BadMuon << endl;
  cout << "pass Cosmic:        " << n_pass_Cosmic  << endl;
  cout << "pass HotSpot:       " << n_pass_HotSpot << endl;
  cout << "pass TTCVeto:       " << n_pass_TTCVeto  << endl;
  cout << "pass Good Vtx       " << n_pass_GoodVtx  << endl;

  cout << "pass atleast 2 base " << n_pass_atleast2BaseLep << endl;
  cout << "pass exactly 2 base " << n_pass_exactly2BaseLep << endl;
  cout << "pass mll20          " << n_pass_mll20   << endl;
  
  cout << std::setprecision(1) << std::fixed;

  cout << "Channels        " << WH_FLAV[0] << "\t " << WH_FLAV[1] << "\t " << WH_FLAV[2] <<endl;
  print_line("pass category ",n_pass_dil[0], n_pass_dil[1], n_pass_dil[2]);
  print_line("pass nLep     ",n_pass_signalLep[0], n_pass_signalLep[1], n_pass_signalLep[2]);
  print_line("pass tau veto ",n_pass_tauVeto[0], n_pass_tauVeto[1], n_pass_tauVeto[2]);
  print_line("pass trig     ",n_pass_trig[0],n_pass_trig[1],n_pass_trig[2]);
  print_line("pass truth    ",n_pass_truth[0],n_pass_truth[1],n_pass_truth[2]);


  print_SRSS();
  print_CRSS();
  //  print_SROSOF2jets();
  //  print_SRZb();
  print_optimSR();


  cout << std::endl << std::endl; 

 
  finish();

}


/*--------------------------------------------------------------------------------*/
// Set cut selection
/*--------------------------------------------------------------------------------*/
void SusyWHAna::setSelection(std::string s, uint dilType)
{
  TString m_sel(s);
  resetCuts();
 
  m_nLepMin=2;
  m_nLepMax=2;
  
  //----------------------------//
  // SS Signal Regions
  //----------------------------//
  if(m_sel.Contains("WH_SRSS")){
    m_selSS     = true;
    m_vetoLooseSFOSinZ = true;
    m_vetoB     = true;
    m_vetoF     = true;

    m_pTl0Min   = 30;
    m_pTl1Min   = 20;//30 EM

    if(m_sel.Contains("WH_SRSS1j")){
      m_minC20    = 1;
      m_maxC20    = 1;

      if(dilType==ET_ee){
	m_mllIn      = true;
	m_lowMll     = MZ-10;//80;
	m_highMll    = MZ+10;//100;
	m_metRelMin  = 55;
	m_highMljj   = 90;
	m_HTMin      = 200;
      }
      else if(dilType==ET_mm){
	m_dEtallMax  = 1.5;
	m_mtMaxLow   = 110;
	m_highMljj   = 90;
	m_HTMin      = 200;
      }
      else if(dilType==ET_em){
	m_pTl1Min    = 30;
	m_dEtallMax  = 1.5;
	m_highMljj   = 90;
	m_mtMaxLow   = 110;
	m_HTMin      = 200;
	//m_lowMTWW    = 120;//110;
      }
    }
    if(m_sel.Contains("WH_SRSS23j")){
      m_minC20    = 2;
      m_maxC20    = 3;

      if(dilType==ET_ee){
	m_mllIn      = true;
	m_lowMll     = MZ-10;//80;
	m_highMll    = MZ+10;//100;
	m_metRelMin  = 30;//55;
	m_highMljj   = 120;
	m_mtMaxLow   = 110;
      }
      else if(dilType==ET_mm){
	m_dEtallMax  = 1.5;
	m_pTl1Min    = 30;
	m_highMljj   = 120;
	m_HTMin      = 200;
      }
      else if(dilType==ET_em){
	m_pTl1Min    = 30;
	m_dEtallMax  = 1.5;
	m_highMljj   = 120;
	m_mtMaxLow   = 110;
	m_HTMin      = 200;
	//m_lowMTWW    = 110;
      }
    }
  }

  //----------------------------//
  // SS Control Regions
  //----------------------------//
  if(m_sel.Contains("WH_CRSS")){
    m_selSS     = true;
    m_vetoLooseSFOSinZ = true;
    m_vetoB     = true;
    m_vetoF     = true;


    if(m_sel.Contains("WH_CRSSZVFAKE")){
      if(dilType==ET_ee){
	m_mllIn      = true;
	m_pTl0Min    = 20;
	m_pTl1Min    = 20;
	m_lowMll     = MZ-10;
	m_highMll    = MZ+10;
	m_metRelMin  = 40;
	m_lowMljj    = 90;//120   //Cut depends on nC20J, raised to 120 fror 2+3j !!!
      }
      else if(dilType==ET_mm){///ZV enriched !
	m_pTl0Min    = 30;
	m_pTl1Min    = 30;
	m_lowMljj    = 90;//120   //Cut depends on nC20J, raised to 120 fror 2+3j  !!!
      }
      else if(dilType==ET_em){
	m_pTl0Min    = 30;
	m_pTl1Min    = 30;
	m_lowMljj    = 90;//120   //Cut depends on nC20J, raised to 120 fror 2+3j  !!!
      }
    }
    if(m_sel.Contains("WH_CRSSFAKE")){
      if(dilType==ET_ee){//NOT DEFINE
      }
      else if(dilType==ET_mm){
	m_pTl0Min    = 20;
	m_pTl1Min    = 20;
	m_pTl1Max    = 30;
	//m_lowMljj    = 90;//120   //Cut depends on nC20J, raised to 120 fror 2+3j  !!! --diverge --- from Davide/Suneet cuts.
      }
      else if(dilType==ET_em){
	m_pTl0Min    = 20;// --- Diverge --- from Davide/Suneet cuts.
	m_pTl1Min    = 20;
	m_pTl1Max    = 30;
	m_lowMljj    = 90;//120   //Cut depends on nC20J, raised to 120 fror 2+3j  !!!
      }
    }

  }




  /*
  else if(m_sel.Contains("SROSOF2jets")){  
    m_selOS     = true;
    m_vetoB     = true;
    m_vetoF     = true;
    m_minC20    = 2;
    m_pTl0Min   = 30;  
    m_pTl1Min   = 30;  

    m_dPhillMetMin = 1.5;
    m_mtMin     = 60;
    m_dRllMax   = 1.5;  
    m_lowMjj    = 50;
    m_highMjj   = 100;
    m_metMin    = 80;
  }
  else if(m_sel.EqualTo("WH_SRZb")){  
    m_selOS     = true;
    m_selSF     = true;
    m_lowMll    = MZ-15;
    m_highMll   = MZ+15;
    m_pTl0Min   = 20;
    m_pTl1Min   = 20;
    m_selB      = true;
    m_minB20    = 1;
    m_maxB20    = 1;
  }
  else if(m_sel.EqualTo("WH_SRZbb")){ 
    m_selOS     = true;
    m_selSF     = true;
    m_lowMll    = MZ-15;
    m_highMll   = MZ+15;
    m_pTl0Min   = 20;
    m_pTl1Min   = 20;
    m_selB      = true;
    m_minB20    = 2;
    m_maxB20    = 999; 
    //ADD metSig cut
  }
  */

  //----------------------------//
  // Optimisation  Regions
  // Very loose selection for ToyNt making
  //----------------------------//
  if(m_sel.Contains("WH_optimSRSS")){//top optim SS channels
    m_selSS = true;
    m_vetoLooseSFOSinZ = true;
    m_vetoB     = true;
    m_vetoF     = true;
    m_minC20    = 1;
  }

  //----------------------------//
  // Lea's validation regions
  //----------------------------//
  if(m_sel.Contains("WH_HighMll")){
    m_selSS      = true;
    m_minC20     = 1;
    m_vetoB      = true;
    m_metRelMin  = 40;      //metRel>40
    m_dPhillMin  = 1.3;     //dPhill>1.3
    m_pTllMax    = 20;      //pTll<20
    m_vetoZ      = false;
    m_lowMll     = 60;      //mll>60
    m_mllIn      = false;
    if(dilType==ET_ee)  m_vetoZ = true;
  }
  if(m_sel.Contains("WH_HighPtll")){
    m_selSS      = true;
    m_minC20     = 1;
    m_vetoB      = true;
    m_metRelMin  = 40;      //metRel>40
    m_dPhillMin  = 1.3;     //dPhill>1.3
    m_pTllMin    = 20;      //pTll>20
    m_vetoZ      = false;
    m_highMll    = 60;      //mll<60
    m_mllIn      = false;
    if(dilType==ET_ee)  m_vetoZ = true;
  }
  if(m_sel.Contains("WH_lowMET")){
    m_selSS      = true;
    m_minC20     = 1;
    m_vetoB      = true;
    m_metRelMin  = -1;
    m_metRelMax  = 40;      //metRel<40
    m_dPhillMin  = 1.3;     //dPhill>1.3
    m_pTllMax    = 20;      //pTll<20
    m_vetoZ      = false;
    m_highMll    = 60;      //mll<60 
    m_mllIn      = false;
    if(dilType==ET_ee)  m_vetoZ = true;
  }
  if(m_sel.Contains("WH_BTag")){
    m_selSS      = true;
    m_vetoB      = false;
    m_selB       = true;
    m_vetoZ      = false;
    if(dilType==ET_ee)  m_vetoZ = true;
  }
  
  
}

/*--------------------------------------------------------------------------------*/
// Full event selection
/*--------------------------------------------------------------------------------*/
bool SusyWHAna::selectEvent(LeptonVector* leptons, 
			    LeptonVector* baseLeps, 
			    const JetVector* signalJets,
			    const Met* met,
			    float w)
{
  //Set increment to mc weight (otherwise confusing w/ sample w/ -1 weight)
  if(nt->evt()->isMC) _inc = nt->evt()->w; 
  else _inc =1;
  if(nt->evt()->isMC && !WEIGHT_COUNT && WEIGHT_ONE) _inc=1;

  if(SYST==DGSys_NOM) n_readin+=_inc;
  
  if(!passEventCleaning() ){
    if(dbg()>10) cout<<"Fail cleaning" << endl;  
    return false;
  }
  
  if( v_baseLep->size() < NBASELEPMIN ){ 
    if(dbg()>10) cout<<"Fail baselepMIN " << endl;  
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_atleast2BaseLep+=_inc;
  if(v_baseLep->size()>NBASELEPMAX ){ 
    if(dbg()>10) cout<<"Fail baselepMAX " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_exactly2BaseLep+=_inc;
  
  if(! passMll20(baseLeps)){ 
    if(dbg()>10) cout<<"Fail Mll20 " << endl; 
      return false;
    }
  if(SYST==DGSys_NOM) n_pass_mll20+=_inc;

  //
  // Get Event Type to continue cutflow
  //
  m_ET = getDiLepEvtType(*baseLeps);
  if(m_ET==ET_me) m_ET=ET_em; //Keep EM & ME together
  
  if(SYST==DGSys_NOM) n_pass_dil[m_ET]+=_inc;

  if( !passNLepCut(leptons) ){ 
    if(dbg()>10) cout<<"Fail Nlep " << endl; 
    return false;
  }

  if( !passTauVeto(v_sigTau) ){
    if(dbg()>10) cout << "Fail Tau veto " << endl;
    return false;
  }
  
  if( !passTrigger(leptons, m_trigObj, met) ){ 
    if(dbg()>10) cout<<"Fail Trig " << endl;  
    return false; 
  }

  if( !passIsPromptLepton(leptons,m_method, nt->evt()->isMC)){
    if(dbg()>10) cout<<"Fail Prompt " << endl; 
    return false; 
  }
  
  //
  //set _ww to the appropriate weighting
  //
  float _ww      = w;//eventWeight(LUMIMODE,SYST); 
  float _ww_tmp  = 1;//eventWeight(LUMIMODE,SYST); 
  //_ww = _ww_tmp;//check can reproduce old number
  
  
  if(!WEIGHT_COUNT) _ww=1;
  float _lepSFW  = getLepSFWeight(leptons,SYST);
  float _trigW   = getTriggerWeight(leptons, 
				    met->lv().Pt(),
				    signalJets->size(),
				    nt->evt()->nVtx,
				    SYST);
  float bTagWeight =  getBTagSF(nt->evt(),v_baseJet,SYST);

  if(WEIGHT_COUNT){
    _ww *= _lepSFW * _trigW;
    _ww_tmp *= _lepSFW * _trigW;
  }
  float _wwSave = _ww;
  saveOriginal(); //Backup Met & leptons  --> newMet if charge flip
  
  if(dbg()>1){ 
    cout << ">>> run " << nt->evt()->run  
	 << " event " << nt->evt()->event 
	 << " SYST " << DGSystNames[SYST]
	 << " lepSF " << _lepSFW
	 << " trigW " << _trigW
	 << " bTag " << bTagWeight
	 << " weight(w/btag) " << _ww*bTagWeight 
	 << " wwSave " << _wwSave 
	 << " event weight ww " << _ww 
	 << endl;
  }
 


  //
  //Loop over SR's & CR's
  //
  for(uint iSR=WH_SRSS1j; iSR< WH_NSR; iSR++){
    int icut =0;
    string sSR=WH_SRNAME[iSR];
    setSelection(sSR,m_ET);
    SR=iSR;
    if(dbg() > 2 ) cout << "Signal region " << sSR << endl;

    //    if(dbg()>-1) cout << "\t " << sSR << " weight check 0 " << _ww << endl;
    //Reset weight in case used btagWeight in previous SR
    _ww=_wwSave; 
    //    if(dbg()>-1) cout << "\t " << sSR << "  weight check 1 " << _ww << endl;
    if(WEIGHT_COUNT) _inc = _ww;
    else{
      if(!WEIGHT_ONE) _inc = nt->evt()->w;
    }

    //Only in MC do we alter the Ele pt - restore 
    if(nt->evt()->isMC) restoreOriginal(*leptons,met);

    //For data - fake estimate
    if( !nt->evt()->isMC && m_useLooseLep){
      float _metRel = getMetRel(&new_met,*leptons,*signalJets);
      _ww = getFakeWeight(leptons,nt->evt()->nVtx,nt->evt()->isMC,iSR,signalJets->size(),_metRel,SYST);
      if(WEIGHT_COUNT) _inc = _ww;
    }
    //    if(dbg()>-1) cout << "\t " << sSR << "  weight check 2 " << _ww << endl;

    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    //Deal with Charge flip estimate - obtain approprate weighting
    if(!USE_QFLIP && !passQQ(leptons)) continue;
    float _ww_qFlip=1;
    if(USE_QFLIP){
      if( nt->evt()->isMC && m_method == RLEP &&  m_ET!=ET_mm &&
	  (iSR==WH_SRSS1j || iSR==WH_SRSS23j || 
	   iSR==WH_CRSSZVFAKE || iSR==WH_CRSSFAKE ||
	   iSR==WH_optimSRSS ||
	   iSR==WH_HighMll || iSR==WH_HighPtll || iSR==WH_lowMET || iSR==WH_BTag  ) ){
	if(isGenuineSS(leptons,nt->evt()->isMC) && SYST==DGSys_NOM )  n_pass_ss[m_ET][SR]+=_inc; //genuine SS - no qFlip
	if(!isGenuineSS(leptons,nt->evt()->isMC)){ //OS ee/em event - get the qFlip prob
	  _ww_qFlip = getQFlipProb(leptons,&new_met,SYST);
	  _ww *= _ww_qFlip;
	  if(WEIGHT_COUNT) _inc = _ww;
	  if(SYST==DGSys_NOM) n_pass_ss[m_ET][SR]+=_inc;
	}
      }
      else
	if(!passQQ(leptons)) continue;
    }

    //    if(dbg()>-1) cout << "\t " << sSR << "  weight check last " << _ww << endl;

    //
    // Apply event selection cuts
    // Bailout as soon one fail for a given SR/CR
    //
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passFlavor(leptons)){ 
      //if(iSR==PRINT_SR) cout << "Fail flavor" << nt->evt()->run << " " << nt->evt()->event << endl; 
      continue; 
    }
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);

    
    if(!passSFOSLooseLepton(nt,*leptons) ){
      if(dbg()>10) 
	cout << WH_FLAV[m_ET] << " " << nt->evt()->run  
	     << " " << nt->evt()->event 
	     << " nJets " << numberOfCLJets(*signalJets,m_jvfTool, (SusyNtSys) DGSys_NOM, m_anaType) << " fail 3rd loose lep veto" << endl;
	
       /*
      if(DUMP_RUNEVT && iSR==PRINT_SR){
	
	evtDump << WH_FLAV[m_ET] << " " << nt->evt()->run  
	<< " " << nt->evt()->event 
	<< " nJets " << numberOfCLJets(*signalJets) << endl;
	}
      */
      continue;
    }
    else {
      if(dbg()>10)
	cout << WH_FLAV[m_ET] << " " << nt->evt()->run  
	     << " " << nt->evt()->event 
	     << " nJets " << numberOfCLJets(*signalJets,m_jvfTool, (SusyNtSys) DGSys_NOM, m_anaType) << " pass 3rd loose lep veto" << endl;
    }
    if(dbg()>1 ) cout << "\t Pass 3rd lepton veto " << sSR << endl;

    if(!passZVeto(leptons,false)) {//applied to OS or SS
      if(dbg()>5 && iSR==PRINT_SR) cout << "Fail Zveto " << nt->evt()->run << " " << nt->evt()->event <<endl;
      continue;
    }
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass Zveto " << sSR << endl;
    
    //
    // Apply bweight only to SR where we use jet count/veto
    //
    if(USE_BWEIGHT && nt->evt()->isMC) {
      /*if( ! ( iSR ==DIL_CR2LepOS || iSR==DIL_CR2LepSS 
	|| iSR==DIL_CR2LepSS40 || iSR==DIL_CRZ) )*/
	_ww *= bTagWeight;
	_ww_tmp *= bTagWeight;
	if(WEIGHT_COUNT) _inc = _ww;
    }

    if(!passFullJetVeto(signalJets)){
      if(dbg()>1 && iSR==PRINT_SR) cout << "Fail Jveto " << nt->evt()->run << " " << nt->evt()->event << endl;
      continue;
    }
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass Full-Jet veto " << sSR << endl;

    if(!passFJet(signalJets)) {
      if(dbg() >1 ) cout << "\t Fail F-Jet req " << sSR << endl;
      continue;
    }
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass F-Jet req " << sSR << endl;
    
    if(!passBJet(signalJets)) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass b-Jet req " << sSR << endl;
    
    if(!passLJet(signalJets)) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passCentralJet(signalJets)) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg()>1 ) cout << "\t Pass central jet " << sSR << endl;

    if(!passNBJet(signalJets)) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passLead2JetsPt(signalJets) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass Jet pT " << sSR << endl;
    
    if(!passMjj(signalJets) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass Mjj " << sSR << endl;

    if(!passLead2LepPt(leptons) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass l0 pT " << sSR << endl;
    
    if(!passMll(leptons) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass Mll " << sSR << endl;
    
    if(!passPtll(leptons) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passPtllBound(leptons) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passdPhill(leptons) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passdEtall(leptons) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passdRll(leptons) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passMaxMT(leptons,&new_met) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    //Special for the CR - raise mljj cut in 2+3 (save histo booking!)
    if((iSR == WH_CRSSZVFAKE || iSR == WH_CRSSFAKE) && signalJets->size()>1){
      m_lowMljj = 120;
      if(m_ET==ET_mm)  m_lowMljj = 0;
    }


    if(!passMljj(leptons,signalJets)) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg()>1 ) cout << "\t Pass mljj cut value " << m_lowMljj << sSR << endl;

    if(!passDPhillJ0(leptons,signalJets) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passDPhillMET(leptons,&new_met) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passMeff(signalJets, &new_met) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passHT(leptons,signalJets, &new_met) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg()>1 ) cout << "\t Pass HT " << sSR << endl;

    if(!passMetMeff(leptons,signalJets, &new_met,true) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passTopTagger(leptons,signalJets,&new_met) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass toptagger " << sSR << endl;
    
    if(!passMETRel(&new_met,leptons,signalJets) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass MetRel " << sSR << endl;
    
    if(!passMET(&new_met) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass Met " << sSR << endl;
    
    if(!passMT2(leptons, &new_met) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    

    if(!passMWWT(leptons,&new_met) ) continue;
    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg()>1 ) cout << "\t Pass MWWT" << sSR << endl;


    
    //
    // Debugging - Dump run event
    //
    if(DUMP_RUNEVT && iSR==PRINT_SR ){
      
      evtDump << WH_FLAV[m_ET] << " " << nt->evt()->run  << " " << nt->evt()->event
	      << " nJets " <<  numberOfCLJets(*signalJets,m_jvfTool, (SusyNtSys) DGSys_NOM, m_anaType) 
	      << " SR " << WH_SRNAME[iSR] 
	      << " w " << _ww  << " _wOld " << _ww_tmp << endl;
      //cout << ">>> run " << nt->evt()->run  
      //<< " event " << nt->evt()->event <<endl;
      //dumpEvent();
      cout << ">>> run " << nt->evt()->run  
	   << " event " << nt->evt()->event 
	   << " SYST " << DGSystNames[SYST]
	   << " evtWeight " << w
	   << " lepSF " << _lepSFW
	   << " trigW " << _trigW
	   << " bTag " << bTagWeight
	   << " qFlip " <<  _ww_qFlip
	   << " ww " << _ww 
	   << " wwSave " << _wwSave 
	   << " old way " << _ww_tmp 
	   << endl;
    }
    /*
    if(DUMP_RUNEVT && iSR==PRINT_SR){
      
      evtDump << WH_FLAV[m_ET] << " " << nt->evt()->run  << " " << nt->evt()->event 
	      << " nJets " << numberOfCLJets(*signalJets) << endl;
      //cout << ">>> run " << nt->evt()->run  
      //<< " event " << nt->evt()->event <<endl;
      //dumpEvent();
      
    }
    */

    _hh->H1FILL(_hh->DGWH_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(dbg() >1 ) cout << "\t Pass All " << sSR << endl;

    //
    // Fill histos, ToyNt and HFT 
    //
    if(DO_FILL_HISTO ){
      fillHistograms(SR,SYST,leptons, signalJets,&new_met,_ww);
      if(dbg() >10 ) cout << "\t Filled histos " << sSR << endl;
    }
    if(FILL_TOYNT && iSR==TOYNT_iSR && SYST==DGSys_NOM) 
      fillToyNt(SYST,leptons, signalJets,&new_met,_ww, bTagWeight,_ww_qFlip,_trigW);
       
  }
  
  if(nt->evt()->isMC) restoreOriginal(*leptons,met);

  return true;
}

/*--------------------------------------------------------------------------------*/
// Fake Bkg estimate event weight
/*--------------------------------------------------------------------------------*/
float SusyWHAna::getFakeWeight(const LeptonVector* leptons, uint nVtx, 
			       bool isMC, int iSR, int nJet, float metrel,
				 uint iSys)
{
  bool _isSignal[2];
  bool _isEle[2];
  float _pt[2];
  float _eta[2];
  
  if(leptons->size()>2) return 0;

  const std::string regionName = "CR_SSInc1j";
  size_t iRegion = m_matrix_methodWH.getIndexRegion(regionName);
  
  for(uint i=0; i<leptons->size(); i++){
    _isEle[i]=leptons->at(i)->isEle();
    _pt[i]= leptons->at(i)->pt;
    _eta[i]= leptons->at(i)->eta;
    if(_isEle[i])_isSignal[i] = isSignalElectron((Electron*) leptons->at(i),*v_baseEle,*v_baseMu,nVtx,isMC,false);
    else         _isSignal[i] = isSignalMuon((Muon*) leptons->at(i),*v_baseEle,*v_baseMu,nVtx,isMC,false);
  }  

  //Map naming convention  
  float _fw = 0;
  Systematic::Value iiSys = Systematic::SYS_NOM;
  if(iSys==DGSys_FAKE_EL_RE_UP) iiSys=Systematic::SYS_EL_RE_UP;
  if(iSys==DGSys_FAKE_EL_RE_DN) iiSys=Systematic::SYS_EL_RE_DOWN;
  if(iSys==DGSys_FAKE_EL_FR_UP) iiSys=Systematic::SYS_EL_FR_UP;
  if(iSys==DGSys_FAKE_EL_FR_DN) iiSys=Systematic::SYS_EL_FR_DOWN;
  if(iSys==DGSys_FAKE_MU_RE_UP) iiSys=Systematic::SYS_MU_RE_UP;
  if(iSys==DGSys_FAKE_MU_RE_DN) iiSys=Systematic::SYS_MU_RE_DOWN;
  if(iSys==DGSys_FAKE_MU_FR_UP) iiSys=Systematic::SYS_MU_FR_UP;
  if(iSys==DGSys_FAKE_MU_FR_DN) iiSys=Systematic::SYS_MU_FR_DOWN;

  sf::Lepton l0(_isSignal[0], _isEle[0], _pt[0], _eta[0]);
  sf::Lepton l1(_isSignal[1], _isEle[1], _pt[1], _eta[1]);
  _fw = m_matrix_methodWH.getTotalFake(l0, l1, iRegion, metrel, iiSys); 

  if(dbg()>10) cout << "SR " << WH_SRNAME[iSR] 
		     << " applying Ssys " << SYST 
		     << " " << DGSystNames[SYST] 
		     << " l0 pT " << leptons->at(0)->pt << " eta " << leptons->at(0)->eta << " isEle " << leptons->at(0)->isEle()
		     << " l1 pT " << leptons->at(1)->pt << " eta " << leptons->at(1)->eta << " isEle " << leptons->at(1)->isEle()
		     << " 2D fw: " << _fw 
		     << endl;
  
  return _fw;
}

/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
void SusyWHAna::print_SRSS()
{
  int j= WH_SRSS1j;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << WH_SRNAME[j] <<endl;
  print_line("pass SS     ",n_pass_ss[0][j], n_pass_ss[1][j], n_pass_ss[2][j]);
  print_line("pass 3rd lep ",n_pass_looseSFOSinZ[0][j], n_pass_looseSFOSinZ[1][j], n_pass_looseSFOSinZ[2][j]);
  print_line("pass FVeto   ",n_pass_FJet[0][j], n_pass_FJet[1][j], n_pass_FJet[2][j]);
  print_line("pass bVeto   ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass =1 J    ",n_pass_CJet[0][j], n_pass_CJet[1][j], n_pass_CJet[2][j]);

  print_line("pass lepPt   ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass Mll     ",n_pass_mll[0][j],  n_pass_mll[1][j],  n_pass_mll[2][j]);
  print_line("pass dEtall  ",n_pass_dEtall[0][j],  n_pass_dEtall[1][j],  n_pass_dEtall[2][j]);


  print_line("pass max MT ",n_pass_maxMt[0][j], n_pass_maxMt[1][j], n_pass_maxMt[2][j]);
  print_line("pass mljj   ",n_pass_mljj[0][j], n_pass_mljj[1][j], n_pass_mljj[2][j]);  
  print_line("pass HT     ",n_pass_HT[0][j], n_pass_HT[1][j], n_pass_HT[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass mWWT   ",n_pass_mWWT[0][j], n_pass_mWWT[1][j], n_pass_mWWT[2][j]);  


  j= WH_SRSS23j;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << WH_SRNAME[j] <<endl;
  print_line("pass =2/3 J ",n_pass_CJet[0][j], n_pass_CJet[1][j], n_pass_CJet[2][j]);
  print_line("pass lepPt  ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass Mll    ",n_pass_mll[0][j],  n_pass_mll[1][j],  n_pass_mll[2][j]);
  print_line("pass dEtall ",n_pass_dEtall[0][j],  n_pass_dEtall[1][j],  n_pass_dEtall[2][j]);
  print_line("pass max MT ",n_pass_maxMt[0][j], n_pass_maxMt[1][j], n_pass_maxMt[2][j]);
  print_line("pass mljj   ",n_pass_mljj[0][j], n_pass_mljj[1][j], n_pass_mljj[2][j]);  
  print_line("pass HT     ",n_pass_HT[0][j], n_pass_HT[1][j], n_pass_HT[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass mWWT   ",n_pass_mWWT[0][j], n_pass_mWWT[1][j], n_pass_mWWT[2][j]);  

}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
void SusyWHAna::print_CRSS()
{
  int j= WH_CRSSZVFAKE;

  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << WH_SRNAME[j] <<endl;
  print_line("pass SS     ",n_pass_ss[0][j], n_pass_ss[1][j], n_pass_ss[2][j]);
  print_line("pass 3rd lep ",n_pass_looseSFOSinZ[0][j], n_pass_looseSFOSinZ[1][j], n_pass_looseSFOSinZ[2][j]);
  print_line("pass FVeto   ",n_pass_FJet[0][j], n_pass_FJet[1][j], n_pass_FJet[2][j]);
  print_line("pass bVeto   ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);

  print_line("pass lepPt   ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass Mll     ",n_pass_mll[0][j],  n_pass_mll[1][j],  n_pass_mll[2][j]);
  print_line("pass mljj   ",n_pass_mljj[0][j], n_pass_mljj[1][j], n_pass_mljj[2][j]);  
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);

  j= WH_CRSSFAKE;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << WH_SRNAME[j] <<endl;
  print_line("pass lepPt   ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass mljj   ",n_pass_mljj[0][j], n_pass_mljj[1][j], n_pass_mljj[2][j]);  


}


/*--------------------------------------------------------------------------------*/
void SusyWHAna::print_SROSOF2jets()
{
  /*
  int j= WH_SROSOF2jets;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << WH_SRNAME[j] <<endl;
  print_line("pass OS            ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass Iso           ",n_pass_MuIso[0][j],n_pass_MuIso[1][j],n_pass_MuIso[2][j]);
  print_line("pass FVeto         ",n_pass_FJet[0][j], n_pass_FJet[1][j], n_pass_FJet[2][j]);
  print_line("pass bVeto         ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass >=2 J         ",n_pass_CJet[0][j], n_pass_CJet[1][j], n_pass_CJet[2][j]);
  print_line("pass Mjj           ",n_pass_mjj[0][j],n_pass_mjj[1][j],n_pass_mjj[2][j]);
  print_line("pass lepPt         ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass dRll          ",n_pass_dRll[0][j],n_pass_dRll[1][j],n_pass_dRll[2][j]);
  print_line("pass min(mT1,mT2)  ",n_pass_minMt[0][j], n_pass_minMt[1][j], n_pass_minMt[2][j]);
  print_line("pass dPhi(ll,Met)  ",n_pass_dPhillMet[0][j], n_pass_dPhillMet[1][j], n_pass_dPhillMet[2][j]);
  print_line("pass Met           ",n_pass_met[0][j], n_pass_met[1][j], n_pass_met[2][j]);
  */
}   

/*--------------------------------------------------------------------------------*/
void SusyWHAna::print_SRZb()
{
  /*
  int j= WH_SRZb;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << WH_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass SF     ",n_pass_flav[0][j], n_pass_flav[1][j], n_pass_flav[2][j]);
  print_line("pass btag   ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass ==1b   ",n_pass_NBJet[0][j], n_pass_NBJet[1][j], n_pass_NBJet[2][j]);
  print_line("pass lepPt  ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass Mll    ",n_pass_mll[0][j],  n_pass_mll[1][j],  n_pass_mll[2][j]);
   
  j= WH_SRZbb;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << WH_SRNAME[j] <<endl;
  print_line("pass btag   ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass >=2b   ",n_pass_NBJet[0][j], n_pass_NBJet[1][j], n_pass_NBJet[2][j]);
  print_line("pass lepPt  ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass Mll    ",n_pass_mll[0][j],  n_pass_mll[1][j],  n_pass_mll[2][j]);
  */
}

/*--------------------------------------------------------------------------------*/
void SusyWHAna::print_optimSR()
{
  int j= WH_optimSRSS;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << WH_SRNAME[j] <<endl;
  print_line("pass SS     ",n_pass_ss[0][j], n_pass_ss[1][j], n_pass_ss[2][j]);
  print_line("pass 3rd lep ",n_pass_looseSFOSinZ[0][j], n_pass_looseSFOSinZ[1][j], n_pass_looseSFOSinZ[2][j]);
  print_line("pass FVeto         ",n_pass_FJet[0][j], n_pass_FJet[1][j], n_pass_FJet[2][j]);
  print_line("pass bVeto         ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass >=1 J         ",n_pass_CJet[0][j], n_pass_CJet[1][j], n_pass_CJet[2][j]);

}
/*--------------------------------------------------------------------------------*/
// Fill histograms 
/*--------------------------------------------------------------------------------*/
void SusyWHAna::fillHistograms(uint iSR,uint iSYS,
				 const LeptonVector* leptons, 
				 const JetVector* jets,
				 const Met* met,
				 float _ww)
{
  if(dbg()>1) cout << "\t Fill histo " << WH_FLAV[m_ET] << " " 
		     << WH_SRNAME[iSR] << " " << DGSystNames[iSYS] << " w " << _ww << endl;

  _hh->H1FILL(_hh->DGWH_pred[iSR][m_ET][iSYS],0.,_ww); 

  int q1=0;
  int q2=0;
  int qqType=0;
  TLorentzVector _ll;
  float dPhill= leptons->at(0)->DeltaPhi(*leptons->at(1));;
  dPhill=TVector2::Phi_mpi_pi(dPhill)*TMath::RadToDeg(); 
  float dRll=999;
  dRll  = leptons->at(0)->DeltaR(*leptons->at(1));
  float dEtall=999;
  dEtall= leptons->at(0)->Eta()-leptons->at(1)->Eta();
  float dPhilMet=999; //min dPhi (l,MET)
  float mTl1=-999;
  float mTl2=-999;

  for(uint ilep=0; ilep<leptons->size(); ilep++){
    const Susy::Lepton* _l = leptons->at(ilep);
    _ll = _ll + (*_l);
    bool isChargeFlip =  _l->isEle() ? ((Electron*) _l)->isChargeFlip : false; 
    LEP_TYPE lType = getType(_l);
    /*
    LEP_TYPE lType = getType(_l->mcOrigin,
			     _l->mcType,
			     _hh->sampleName(),
			     nt->evt()->mcChannel,
			     _l->truthType,
			     _l->isEle(),
			     isChargeFlip);
    */
        
    float _dPhi=fabs(met->lv().DeltaPhi(*_l));
    if(_dPhi<dPhilMet) dPhilMet=_dPhi;

    if(ilep==0){
      _hh->H1FILL(_hh->DGWH_ptl1[iSR][m_ET][iSYS],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->DGWH_etal1[iSR][m_ET][iSYS],_l->Eta(),_ww); 
      _hh->H1FILL(_hh->DGWH_d0Sl1[iSR][m_ET][iSYS],_l->d0/_l->errD0,_ww); 
      _hh->H1FILL(_hh->DGWH_z0sinthetal1[iSR][m_ET][iSYS],_l->z0SinTheta(),_ww); 
      _hh->H1FILL(_hh->DGWH_orgl1[iSR][m_ET][iSYS],lType,_ww); 
      q1=_l->q;
      mTl1 = mT(*_l, met->lv());
    }
    else if(ilep==1){
      _hh->H1FILL(_hh->DGWH_ptl2[iSR][m_ET][iSYS],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->DGWH_etal2[iSR][m_ET][iSYS],_l->Eta(),_ww); 
      _hh->H1FILL(_hh->DGWH_d0Sl2[iSR][m_ET][iSYS],_l->d0/_l->errD0,_ww); 
      _hh->H1FILL(_hh->DGWH_z0sinthetal2[iSR][m_ET][iSYS],_l->z0SinTheta(),_ww); 
      _hh->H1FILL(_hh->DGWH_orgl2[iSR][m_ET][iSYS],lType,_ww); 
      q2=_l->q;
      mTl2 = mT(*_l, met->lv());
    }
    
    if(_l->isEle()){
      _hh->H1FILL(_hh->DGWH_ePt[iSR][m_ET][iSYS],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->DGWH_eEta[iSR][m_ET][iSYS],_l->Eta(),_ww); 
    }
    else{
      _hh->H1FILL(_hh->DGWH_mPt[iSR][m_ET][iSYS],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->DGWH_mEta[iSR][m_ET][iSYS],_l->Eta(),_ww); 
    }

  }//leptons 
  dPhilMet = TVector2::Phi_mpi_pi(dPhilMet)*TMath::RadToDeg();  

  if(q1*q2<0){//OS
    qqType = (q1<0 && q2>0) ? 0 : 1;  //-+/+-
  }
  else if(q1*q2>0){//SS
    qqType = (q1<0 && q2<0) ? 0 : 1;  //--/++
  }
  _hh->H1FILL(_hh->DGWH_qq[iSR][m_ET][iSYS],qqType,_ww); 
  
  float mWT = mT(_ll, met->lv());
  float mT2 = getMT2(*leptons, met);
  float metRel = getMetRel(met,*leptons,*jets);
  float HT = Meff(*leptons,*jets,met);
  if(dbg()>1){
    cout << "\t met " <<met->lv().Pt()
	 << " metPhi " << met->lv().Phi()
	 << " metrel " << metRel 
	 << " mWWT " << mWT
	 << " mT2 " << mT2 
	 << " HT " << HT
	 << endl;
  }

  _hh->H1FILL(_hh->DGWH_mll[iSR][m_ET][iSYS],_ll.M(),_ww); 
  _hh->H1FILL(_hh->DGWH_mllShift[iSR][m_ET][iSYS],m_chargeFlip->mllShift(_ll.M()),_ww); 
  _hh->H1FILL(_hh->DGWH_mllcoarse[iSR][m_ET][iSYS],_ll.M(),_ww); 
  _hh->H1FILL(_hh->DGWH_mllcoarser[iSR][m_ET][iSYS],_ll.M(),_ww); 
  _hh->H1FILL(_hh->DGWH_pTll[iSR][m_ET][iSYS],_ll.Pt(),_ww); 
  _hh->H1FILL(_hh->DGWH_mWWT[iSR][m_ET][iSYS],mWT,_ww); 
  _hh->H1FILL(_hh->DGWH_mTl1[iSR][m_ET][iSYS],mTl1,_ww);
  _hh->H1FILL(_hh->DGWH_mTl2[iSR][m_ET][iSYS],mTl2,_ww); 
  _hh->H1FILL(_hh->DGWH_max_mT[iSR][m_ET][iSYS],TMath::Max(mTl1,mTl2),_ww); 
  _hh->H1FILL(_hh->DGWH_dPhill[iSR][m_ET][iSYS],fabs(dPhill),_ww); 
  _hh->H1FILL(_hh->DGWH_dEtall[iSR][m_ET][iSYS],fabs(dEtall),_ww); 
  _hh->H1FILL(_hh->DGWH_dRll[iSR][m_ET][iSYS],fabs(dRll),_ww); 
  _hh->H1FILL(_hh->DGWH_dPhilMet[iSR][m_ET][iSYS],fabs(dPhilMet),_ww); 
  _hh->H1FILL(_hh->DGWH_JZBJet[iSR][m_ET][iSYS],JZBJet(v_sigJet,leptons),_ww); 
  _hh->H1FILL(_hh->DGWH_JZBEtmiss[iSR][m_ET][iSYS],JZBEtmiss(met,leptons),_ww); 
  _hh->H1FILL(_hh->DGWH_etmiss[iSR][m_ET][iSYS],met->lv().Pt(),_ww); 
  _hh->H1FILL(_hh->DGWH_etmissPhi[iSR][m_ET][iSYS],TVector2::Phi_mpi_pi(met->lv().Phi())*TMath::RadToDeg(),_ww); 
  _hh->H1FILL(_hh->DGWH_metrel[iSR][m_ET][iSYS],metRel,_ww); 
  _hh->H1FILL(_hh->DGWH_metrel1[iSR][m_ET][iSYS],metRel,_ww); 
  _hh->H1FILL(_hh->DGWH_metrel2[iSR][m_ET][iSYS],metRel,_ww); 
  _hh->H1FILL(_hh->DGWH_metrel3[iSR][m_ET][iSYS],metRel,_ww); 
  _hh->H1FILL(_hh->DGWH_metRefEle[iSR][m_ET][iSYS],met->refEle,_ww); 
  _hh->H1FILL(_hh->DGWH_metRefGam[iSR][m_ET][iSYS],met->refGamma,_ww); 
  _hh->H1FILL(_hh->DGWH_metRefMuo[iSR][m_ET][iSYS],met->refMuo,_ww); 
  _hh->H1FILL(_hh->DGWH_metRefJet[iSR][m_ET][iSYS],met->refJet,_ww); 
  _hh->H1FILL(_hh->DGWH_metRefSJet[iSR][m_ET][iSYS],met->softJet,_ww); 
  _hh->H1FILL(_hh->DGWH_metCellout[iSR][m_ET][iSYS],met->refCell,_ww); 
  _hh->H1FILL(_hh->DGWH_mt2[iSR][m_ET][iSYS],mT2,_ww); 
  _hh->H1FILL(_hh->DGWH_mt2b[iSR][m_ET][iSYS],mT2,_ww); 

  float corrNpv = nt->evt()->nVtx;
  if(nt->evt()->isMC) corrNpv = GetNVertexBsCorrected(nt->evt()->nVtx);
  _hh->H1FILL(_hh->DGWH_npv[iSR][m_ET][iSYS],corrNpv,_ww); 
  _hh->H1FILL(_hh->DGWH_mu[iSR][m_ET][iSYS],nt->evt()->avgMu,_ww); 

  int nBJets=0;
  int nSigJet=0;
  int nSigCJet=0;
  int nSigFJet=0;
  float dPhiJMet=999;
  float mEff=0;
  float ST=0;
  float mt2J=-999;
  float Mljj = mljj(*leptons,*jets);

  for(uint ijet=0; ijet<jets->size(); ijet++){
    const Susy::Jet* _j = jets->at(ijet);
    nSigJet++;
    if(isCentralLightJet(_j,m_jvfTool, (SusyNtSys) DGSys_NOM, m_anaType)) nSigCJet++;
    if(isForwardJet(_j)) nSigFJet++;
    if(isCentralBJet(_j)){
      nBJets++;
      _hh->H1FILL(_hh->DGWH_ptbj[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DGWH_etabj[iSR][m_ET][iSYS],_j->Eta(),_ww); 
      _hh->H1FILL(_hh->DGWH_jvfbj[iSR][m_ET][iSYS],_j->jvf,_ww); 
    }

    if(ijet==0){
      _hh->H1FILL(_hh->DGWH_ptj1[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DGWH_etaj1[iSR][m_ET][iSYS],_j->Eta(),_ww); 
      _hh->H1FILL(_hh->DGWH_jvfj1[iSR][m_ET][iSYS],_j->jvf,_ww); 
    }
    if(ijet==1){
      _hh->H1FILL(_hh->DGWH_ptj2[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DGWH_etaj2[iSR][m_ET][iSYS],_j->Eta(),_ww);
      _hh->H1FILL(_hh->DGWH_jvfj2[iSR][m_ET][iSYS],_j->jvf,_ww);  
    }
    if(ijet==2){
      _hh->H1FILL(_hh->DGWH_ptj3[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DGWH_etaj3[iSR][m_ET][iSYS],_j->Eta(),_ww); 
    }
    if(ijet==3){
      _hh->H1FILL(_hh->DGWH_ptj4[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DGWH_etaj4[iSR][m_ET][iSYS],_j->Eta(),_ww); 
    }

    float _dPhi=fabs(met->lv().DeltaPhi(*_j));
    if(_dPhi<dPhiJMet) dPhiJMet=_dPhi;
    ST += _j->Pt();

  }

  if(jets->size()>=2){
    TLorentzVector j0 = *jets->at(0);
    TLorentzVector j1 = *jets->at(1);
    TLorentzVector l0 = *leptons->at(0);
    TLorentzVector l1 = *leptons->at(1);
    
    TLorentzVector l0j0 = l0+j0;
    TLorentzVector l1j1 = l1+j1;
    TLorentzVector l0j1 = l0+j1;
    TLorentzVector l1j0 = l1+j0;
    float mt2_a = getMT2(&l0j0,&l1j1,met,false);
    float mt2_b = getMT2(&l0j1,&l1j0,met,false);
    mt2J = min(mt2_a, mt2_b);
  }
  _hh->H1FILL(_hh->DGWH_mt2j[iSR][m_ET][iSYS],mt2J,_ww); 
  if(jets->size()==1)  
    _hh->H1FILL(_hh->DGWH_mlj[iSR][m_ET][iSYS],Mljj,_ww); 
  else
    _hh->H1FILL(_hh->DGWH_mljj[iSR][m_ET][iSYS],Mljj,_ww); 


  mEff = Meff(*leptons,*jets,met,JET_PT_CUT); //ST + met->lv().Pt();

  float metSig = mEff/met->lv().Pt();
  _hh->H1FILL(_hh->DGWH_MetSig[iSR][m_ET][iSYS],metSig,_ww); 

  dPhiJMet = TVector2::Phi_mpi_pi(dPhiJMet)*TMath::RadToDeg();  

  _hh->H1FILL(_hh->DGWH_ST[iSR][m_ET][iSYS],ST,_ww); 
  _hh->H1FILL(_hh->DGWH_mEff[iSR][m_ET][iSYS],mEff,_ww); 
  _hh->H1FILL(_hh->DGWH_dPhiJetMet[iSR][m_ET][iSYS],fabs(dPhiJMet),_ww); 
  _hh->H1FILL(_hh->DGWH_nJets[iSR][m_ET][iSYS],nSigJet,_ww); 
  _hh->H1FILL(_hh->DGWH_nCJets[iSR][m_ET][iSYS],nSigCJet,_ww); 
  _hh->H1FILL(_hh->DGWH_nFJets[iSR][m_ET][iSYS],nSigFJet,_ww); 
  _hh->H1FILL(_hh->DGWH_nBJets[iSR][m_ET][iSYS],nBJets,_ww); 


  if(nSigJet>=2){
    TLorentzVector _jj = (*jets->at(0)) + (*jets->at(1));
    _hh->H1FILL(_hh->DGWH_mjj[iSR][m_ET][iSYS],_jj.M(),_ww); 
  }

  int nSoftJets=0;
  for(uint ijet=0; ijet<v_baseJet->size(); ijet++){
    const Susy::Jet* _j = v_baseJet->at(ijet);
    if(isSignalJet2Lep(_j)) continue;
    nSoftJets++;
    if(ijet==0){
      _hh->H1FILL(_hh->DGWH_ptSj1[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DGWH_etaSj1[iSR][m_ET][iSYS],_j->Eta(),_ww); 
      _hh->H1FILL(_hh->DGWH_jvfSj1[iSR][m_ET][iSYS],_j->jvf,_ww); 
    }
    if(ijet==1){
      _hh->H1FILL(_hh->DGWH_ptSj2[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DGWH_etaSj2[iSR][m_ET][iSYS],_j->Eta(),_ww); 
      _hh->H1FILL(_hh->DGWH_jvfSj2[iSR][m_ET][iSYS],_j->jvf,_ww); 
    }
  }
  _hh->H1FILL(_hh->DGWH_nSoftJets[iSR][m_ET][iSYS],nSoftJets,_ww); 


}


/*--------------------------------------------------------------------------------*/
// Uncertainty on WZ
/*--------------------------------------------------------------------------------*/
float SusyWHAna::getWZUncertainty(uint dsid, int nJet)
{
  static const float uncert_1j  = 0.166;//0.174;
  static const float uncert_23j = 0.368;//0.43;

  XsecUncertainty::McGroup group = XsecUncertainty::kUnknown;
  if(dsidInArray(dsid, kDsidForkWz, kNdsidForkWz)) group = XsecUncertainty::kWz;
  if(group == XsecUncertainty::kUnknown) return 0.0; //Not WZ - 0 uncert


  if(nJet==1) return uncert_1j;
  else if(nJet>1 && nJet<4) return  uncert_23j;
  else{
    if(dbg()>15) cout << "SusyWHAna::getWZUncertainty. Sample is WZ, but number of jet incompatible " << nJet << endl;
    return 0;
  }
  

}
