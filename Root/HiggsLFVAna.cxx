#include <iomanip>
#include <iostream>
#include "TCanvas.h"
#include "SusyWeakProdAna/HiggsLFVAna.h"

#include "SusyWeakProdAna/SusyAnaCommon.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// HiggsLFVAna Constructor
/*--------------------------------------------------------------------------------*/
HiggsLFVAna::HiggsLFVAna(SusyHistos* _histos):
  SusyBaseAna(_histos,false,true,true)
{
  cout << "----------------------------------" << endl;
  cout << "    Running Higgs-LFV Analysis    " << endl;
  cout << "----------------------------------" << endl;
}

/*--------------------------------------------------------------------------------*/
// Main process loop function 
/*--------------------------------------------------------------------------------*/
void HiggsLFVAna::doAnalysis(float w, unsigned int isys)
{
  reset();
  SYST = isys;
  float evtW = w;
  
  if(FILL_TOYNT && isys==DGSys_NOM){
    bool metDetails      = false;
    bool dijetBlock      = false;
    bool OS2LBlock       = true;
    bool SS2LBlock       = false;
    bool ZBalanceBlock   = false;
    bool diversVarsBlock = false;

    initializeToyNt(metDetails, dijetBlock, 
		    OS2LBlock, SS2LBlock, ZBalanceBlock, diversVarsBlock);
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
void HiggsLFVAna::end()
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

  print_preOS();

  
  cout << std::endl << std::endl; 

 
  finish();

}


/*--------------------------------------------------------------------------------*/
// Set cut selection
/*--------------------------------------------------------------------------------*/
void HiggsLFVAna::setSelection(std::string s, uint dilType)
{
  TString m_sel(s);
  resetCuts();
 
  m_nLepMin=2;
  m_nLepMax=2;

  //----------------------------//
  // OS Signal Regions EM/ME
  //----------------------------//
  if(m_sel.Contains("LFV_SR")){
     m_selOS     = true;
     m_selOF     = true;

     m_pTl0Min   = 40;
     m_pTl1Min   = 20;
     m_vetoJ     = true;
     m_dPhillMin = 2.5;
     m_dPhiMetl1 = 0.5;
  }

  if(m_sel.Contains("LFV_base")){
     m_selOS     = true;
     m_selOF     = true;
  }


}

/*--------------------------------------------------------------------------------*/
// Full event selection
/*--------------------------------------------------------------------------------*/
bool HiggsLFVAna::selectEvent(LeptonVector* leptons, 
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
  
  //
  // Get Event Type to continue cutflow
  //
  m_ET = getDiLepEvtType(*baseLeps);


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

  if(!WEIGHT_COUNT) _ww=1;
  float _lepSFW  = getLepSFWeight(leptons,SYST);
  float _trigW   = getTriggerWeight(leptons, 
				    met->lv().Pt(),
				    signalJets->size(),
				    nt->evt()->nVtx,
				    SYST);
  float bTagWeight =  getBTagSF(nt->evt(),v_baseJet,SYST);

  if(WEIGHT_COUNT)  _ww *= _lepSFW * _trigW;
  float _wwSave = _ww;

  
  //
  //Loop over SR's & CR's
  //
  for(uint iSR=LFV_base; iSR< LFV_NSR; iSR++){
    int icut =0;
    string sSR=LFV_SRNAME[iSR];
    setSelection(sSR,m_ET);
    SR=iSR;
    if(dbg() > 2 ) cout << "Signal region " << sSR << endl;


  }


  return true;
}

/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
void HiggsLFVAna::print_preOS()
{


}

/*--------------------------------------------------------------------------------*/
// Fill histograms 
/*--------------------------------------------------------------------------------*/
void HiggsLFVAna::fillHistograms(uint iSR,uint iSYS,
				 const LeptonVector* leptons, 
				 const JetVector* jets,
				 const Met* met,
				 float _ww)
{

}
