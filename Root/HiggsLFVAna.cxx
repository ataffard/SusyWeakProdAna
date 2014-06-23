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
  cout << "HiggsLFVAna::event counters" <<endl;
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

  cout << "Channels        " << LFV_FLAV[0] << "\t\t " << LFV_FLAV[1] 
       << "\t\t " << LFV_FLAV[2] << "\t\t " << LFV_FLAV[3] <<endl;

  print_line("pass category ",0,3,&n_pass_dil[0]);
  print_line("pass nLep     ",0,3,&n_pass_signalLep[0]);
  print_line("pass tau veto ",0,3,&n_pass_tauVeto[0]);
  print_line("pass trig     ",0,3,&n_pass_trig[0]);
  print_line("pass truth    ",0,3,&n_pass_truth[0]);
  print_base();
  print_SR();
  
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
  float _ww         = w;//eventWeight(LUMIMODE,SYST); 
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

  
  //************************//
  //Loop over SR's & CR's
  //************************//
  for(uint iSR=LFV_base; iSR< LFV_NSR; iSR++){
    int icut =0;
    string sSR=LFV_SRNAME[iSR];
    setSelection(sSR,m_ET);
    SR=iSR;
    if(dbg() > 2 ) cout << "Signal region " << sSR << endl;


    //For data - fake estimate/weight
    if( !nt->evt()->isMC && m_useLooseLep){
      float _metRel = getMetRel(met,*leptons,*signalJets);
      //
      //TO ADD
      //
      //_ww = getFakeWeight(leptons,nt->evt()->nVtx,nt->evt()->isMC,
      //iSR,signalJets->size(),_metRel,SYST);
      if(WEIGHT_COUNT) _inc = _ww;
    }
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);

    //
    // Apply event selection cuts
    // Bailout as soon as one cut fail for a given SR/CR
    //
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    

    if(!passQQ(leptons)) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass QQ " << sSR << endl;
   
    if(!passFlavor(leptons)) continue; 
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass lep flavor " << sSR << endl;
 
    if(!passZVeto(leptons,false)) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass Zveto " << sSR << endl;
    
    //-------------------------------------------------------//
    // Apply bweight only to SR where we use jet count/veto
    //-------------------------------------------------------//
    if(USE_BWEIGHT && nt->evt()->isMC) {
      if(iSR == LFV_SR) _ww *= bTagWeight;
      if(WEIGHT_COUNT) _inc = _ww;
    }

    //JET CUTS
    if(!passFullJetVeto(signalJets)) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass Full-Jet veto " << sSR << endl;

    if(!passFJet(signalJets)) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass F-Jet req " << sSR << endl;
    
    if(!passBJet(signalJets)) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass b-Jet req " << sSR << endl;
    
    if(!passLJet(signalJets)) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passCentralJet(signalJets)) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg()>1 ) cout << "\t Pass central jet " << sSR << endl;

    if(!passLead2JetsPt(signalJets) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass Jet pT " << sSR << endl;
    
    //DILEPTON CUTS
    if(!passLead2LepPt(leptons) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass l0 pT " << sSR << endl;
    
    if(!passMll(leptons) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass Mll " << sSR << endl;
    
    if(!passPtll(leptons) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passPtllBound(leptons) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passdPhill(leptons) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passdEtall(leptons) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passdRll(leptons) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passDPhiMetl1(leptons,met) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);

    //EVT KINEMATICS CUTS
    if(!passMaxMT(leptons,met) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passDPhillJ0(leptons,signalJets) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passDPhillMET(leptons,met) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passMeff(signalJets, met) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passHT(leptons,signalJets, met) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg()>1 ) cout << "\t Pass HT " << sSR << endl;

    if(!passMetMeff(leptons,signalJets, met,true) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passTopTagger(leptons,signalJets,met) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass toptagger " << sSR << endl;
    
    if(!passMETRel(met,leptons,signalJets) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass MetRel " << sSR << endl;
    
    if(!passMET(met) ) continue;
    _hh->H1FILL(_hh->LFV_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >1 ) cout << "\t Pass Met " << sSR << endl;
    
    
    //
    // Fill histos, ToyNt and HFT 
    //
    if(DO_FILL_HISTO ){
      fillHistograms(SR,SYST,leptons, signalJets,met,_ww);
      if(dbg() >10 ) cout << "\t Filled histos " << sSR << endl;
    }
    if(FILL_TOYNT && iSR==TOYNT_iSR && SYST==DGSys_NOM) 
      fillToyNt(SYST,leptons, signalJets,met,_ww, bTagWeight,_ww);
    
  }//SR loop


  return true;
}

/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
void HiggsLFVAna::print_base()
{
  int j= LFV_base;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << LFV_SRNAME[j] <<endl;
  print_line("pass OS ",0,3,j,&n_pass_os[0]);
  print_line("pass OF ",0,3,j,&n_pass_flav[0]);
}
/*--------------------------------------------------------------------------------*/
void HiggsLFVAna::print_SR()
{
  int j= LFV_SR;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << LFV_SRNAME[j] <<endl;
  print_line("pass Jveto         ",0,3,j,&n_pass_FullJveto[0]);
  print_line("pass lepPt         ",0,3,j,&n_pass_leadLepPt[0]);
  print_line("pass dPhi(ll)      ",0,3,j,&n_pass_dPhill[0]);
  print_line("pass dPhi(Met,l1)  ",0,3,j,&n_pass_dPhiMetl1[0]);
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


  if(dbg()>1) cout << "\t Fill histo " << LFV_FLAV[m_ET] << " " 
		   << LFV_SRNAME[iSR] << " " << DGSystNames[iSYS] << " w " << _ww << endl;
  
  _hh->H1FILL(_hh->LFV_pred[iSR][m_ET][iSYS],0.,_ww); 





}
