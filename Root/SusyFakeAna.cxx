#include <iomanip>
#include "SusyWeakProdAna/SusyFakeAna.h"
#include "SusyWeakProdAna/SusyAnaCommon.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// SusyFakeAna Constructor
/*--------------------------------------------------------------------------------*/
SusyFakeAna::SusyFakeAna(SusyHistos* _histos):
  SusyBaseAna(_histos,false,true,true)
{
  cout << "-------------------------------" << endl;
  cout << "    Running Fake Analysis        " << endl;
  cout << "-------------------------------" << endl;

  _random = new TRandom3(2055704344);

  n_pass_SS1j            = 0;
  n_pass_SSEM            = 0;
  n_pass_HFTagProbe      = 0;
  n_pass_MCExtractionEff = 0;
  n_pass_ZTagProbe       = 0;
  n_pass_ZConv           = 0;
  n_pass_ZHFLF           = 0;
}


/*--------------------------------------------------------------------------------*/
// Main process loop function 
/*--------------------------------------------------------------------------------*/
void SusyFakeAna::doAnalysis(unsigned int isys)
{
  reset();
  SYST = isys;

  if(FILL_TOYNT){
    bool metDetails      = false;
    bool dijetBlock      = true; //off ?
    bool OS2LBlock       = false;
    bool SS2LBlock       = true;
    bool ZBalanceBlock   = false;
    bool diversVarsBlock = false;
    bool fakeBlock       = true;

    initializeToyNt(metDetails, dijetBlock, 
		    OS2LBlock, SS2LBlock, ZBalanceBlock,
		    diversVarsBlock,fakeBlock);
  }
  

  if(!CUTFLOW && v_baseLep->size()< 1/*NBASELEPMIN*/) return;
  if(!selectEvent(v_baseLep, v_sigLep, v_sigJet, m_met)) return;
  
  
  return;
}
/*--------------------------------------------------------------------------------*/
// The Terminate() function is the last function to be called
/*--------------------------------------------------------------------------------*/
void SusyFakeAna::end()
{

  
  cout << "------------------------------------------------------------------------" << endl;
  cout << "SusyFakeAna::event counters" <<endl;
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
  cout << "pass tau veto       " << n_pass_tauVeto[0]      << endl;
  cout << "------------------------------------------------------------------------" << endl;
    
  cout << std::setprecision(1) << std::fixed;

  cout << "------------------------------------------------------------------------" << endl;
  cout << "------------------------------------------------------------------------" << endl;
  cout << " Pass SS1j              " <<  n_pass_SS1j            << endl; 
  cout << " Pass SSEM              " <<  n_pass_SSEM            << endl; 
  cout << " Pass HF TagProbe       " <<  n_pass_HFTagProbe      << endl;
  cout << " Pass MC Extraction Eff " <<  n_pass_MCExtractionEff << endl;
  cout << " Pass Z Tag-Probe       " <<  n_pass_ZTagProbe       << endl;
  cout << " Pass Z Conv            " <<  n_pass_ZConv           << endl; 
  cout << " Pass Z HFLF            " <<  n_pass_ZHFLF           << endl; 
  cout << "------------------------------------------------------------------------" << endl;
  cout << "------------------------------------------------------------------------" << endl;

  finish();

}



/*--------------------------------------------------------------------------------*/
// Full event selection
/*--------------------------------------------------------------------------------*/
bool SusyFakeAna::selectEvent(LeptonVector* baseLeps, 
			      LeptonVector* signalLeptons, 
			      const JetVector* signalJets,
			      const Met* met)
{
  //
  //Basic cleaning
  //
  if( !passEventCleaning() ) return false;

  m_ET=0;//fudge needed for tau veto counter
  if( !passTauVeto(v_sigTau) ){
    if(dbg()>10) cout << "Fail Tau veto " << endl;
    return false;
  }

  if(passSelections(baseLeps, signalLeptons, signalJets, met)){
    m_toyNt->_b_isGenuineSS  =  isGenuineSS(baseLeps,nt->evt()->isMC);
    getEventWeights(baseLeps, signalJets, met);
    fillToyNt(SYST, baseLeps, signalJets, met,_ww, _wwBTag, _wwqFlip);
  }

  return true;

}

/*--------------------------------------------------------------------------------*/
// Get event weights
/*--------------------------------------------------------------------------------*/
void SusyFakeAna::getEventWeights(LeptonVector* leptons, 
				  const JetVector* jets,
				  const Met* met)
{
  _ww              = eventWeight(LUMIMODE); 
  float _lepSFW    = getLepSFWeight(leptons);
  float _trigW     = getTriggerWeight(leptons, met->lv().Pt(), jets->size(), nt->evt()->nVtx, SYST);
  _ww             *= _lepSFW * _trigW;  //If we use bTag event, add the BTag weight 
  _wwBTag          = getBTagSF(nt->evt(),v_baseJet,SYST);
  
  float qFlipWeight=1;
  if( nt->evt()->isMC &&  m_ET!=ET_mm && !isGenuineSS(leptons,nt->evt()->isMC)){
    new_met.clear();
    new_met = *met;
    qFlipWeight = getQFlipProb(leptons,&new_met,SYST);
  }
  _wwqFlip = _ww * qFlipWeight;;


}


/*--------------------------------------------------------------------------------*/
// Various CR selection for the fake
/*--------------------------------------------------------------------------------*/
bool SusyFakeAna::passSelections(LeptonVector* baseLeptons, 
				 LeptonVector* leptons, 				 
				 const JetVector* jets,
				 const Met* met)
{
  bool pass_SS1j=false;
  bool pass_SSEM=false;
  bool pass_HFTagProbe=false;
  bool pass_MCExtractionEff=false;
  bool pass_ZTagProbe=false;
  bool pass_ZConv=false;
  bool pass_ZHFLF=false;

  
  if(passZTagProbe(baseLeptons,jets,met)){
    n_pass_ZTagProbe++;
    m_toyNt->_b_pass_ZTP=true;
    pass_ZTagProbe=true;
  }
  
  if(passConv(baseLeptons,jets,met)){
    n_pass_ZConv++;
    m_toyNt->_b_pass_ZConv=true;
    pass_ZConv=true;
  }

   if(passZHFLF(baseLeptons,jets,met)){
    n_pass_ZHFLF++;
    m_toyNt->_b_pass_ZHFLF=true;
    pass_ZHFLF=true;
  }

  if( passSS1j(baseLeptons,jets,met) ){
    n_pass_SS1j++;
    m_toyNt->_b_pass_SS1j=true;
    pass_SS1j=true;
  }

  if( passSSEM(baseLeptons,jets,met) ){
    n_pass_SSEM++;
    m_toyNt->_b_pass_SSEM=true;
    pass_SSEM=true;
  }
  
  if( passHFTagProbe(baseLeptons,jets,met) ){
    n_pass_HFTagProbe++;
    m_toyNt->_b_pass_HFTP=true;
    pass_HFTagProbe=true;
  }
  
  if(passMCExtractionEff(baseLeptons,jets,met)){
    n_pass_MCExtractionEff++;
    m_toyNt->_b_pass_MCEff=true;
    pass_MCExtractionEff=true;
  }
  

  if(pass_SS1j || pass_SSEM || 
     pass_HFTagProbe || 
     pass_MCExtractionEff || 
     pass_ZTagProbe || pass_ZConv || pass_ZHFLF) 
    return true;

  return false;
}


/*--------------------------------------------------------------------------------*/
// SS >=1 jet
// ==2 baseline
// pass dilepton trigger
// >=1 1 central LF
// SS lepton (data or mm) (for ee/em weight event with qFlip
/*--------------------------------------------------------------------------------*/
bool SusyFakeAna::passSS1j(LeptonVector* leptons, 
			   const JetVector* jets,
			   const Met* met)
{
  //Selection cuts
  resetCuts();
  m_nLepMin   = 2;
  m_nLepMax   = 2;
  m_selSS     = true;
  m_minC20    = 1;

  //Selection
  if(!passNLepCut(leptons))                  return false;
  if(!passMll20(leptons))                    return false;
  if(!passTrigger(leptons, m_trigObj, met) ) return false; 
  if(!passLJet(jets))                        return false;

  m_ET = getDiLepEvtType(*leptons);
  if(m_ET==ET_me) m_ET=ET_em;        //Keep EM & ME together  

  if( nt->evt()->isMC &&  m_ET!=ET_mm)  return true; //GenuineSS and OS pass - OS to weight w/ qFlip
  else 
    if(!passQQ(leptons)) return false;
  
  return true;
}

/*--------------------------------------------------------------------------------*/
// SS EM event for Electron SF computation
// ==2 baseline
// pass dilepton trigger
// SS EM event
/*--------------------------------------------------------------------------------*/
bool SusyFakeAna::passSSEM(LeptonVector* leptons, 
			   const JetVector* jets,
			   const Met* met)
{
  //Cut values
  const float cut_mtMax=100;
  resetCuts(); 
  m_nLepMin   = 2;
  m_nLepMax   = 2;
  m_selSS     = true;
  m_selOS     = false;

  /*
  //Selection
  //if(leptons->size()>NBASELEPMAX )           return false;
  if(!passNLepCut(leptons))                  return false;
  if(!passMll20(leptons))                    return false;
  if(!passTrigger(leptons, m_trigObj, met) ) return false; 
  if(!passQQ(leptons))                       return false;

  m_ET = getDiLepEvtType(*leptons);
  if(m_ET==ET_me) m_ET=ET_em;                //Keep EM & ME together  
  if(m_ET != ET_em)                          return false;

  int nProbe = 0;
  float mt = -999;
  int idxTag=-1;
  int idxProbe=-1;
  for(uint i=0; i<leptons->size(); ++i){//After overlap removal
    Susy::Lepton* _l = leptons->at(i);
    if(!_l->isEle()){ //muon - flag it as the tag
      idxTag = i;
    }
    else{ //electron - probe
      mt = Mt(leptons->at(i), met); 
      idxProbe = i;  
      nProbe++;
    }
  } 

  if(nProbe<1) return false;
  
  m_toyNt->_b_SSEM_tagIdx   = idxTag;
  m_toyNt->_b_SSEM_probeIdx = idxProbe;
  */

  LeptonVector candLeps;
  
  if( v_preMu->size()  != 1) return false;
  if( v_baseEle->size()!= 1 ) return false;

  candLeps.push_back( v_preMu->at(0));
  candLeps.push_back( v_baseEle->at(0));

  if(candLeps.size()!=2 ) return false;

  if(!passNLepCut(&candLeps))                  return false;
  if(!passMll20(&candLeps))                    return false;
  if(!passQQ(&candLeps))                       return false;
  m_ET = getDiLepEvtType(candLeps);
  if(m_ET==ET_me) m_ET=ET_em;                //Keep EM & ME together  
  if(m_ET != ET_em)                            return false;
  
  if(candLeps[0]->Pt()<20)                     return false; //muon need pT>20 due to bb MC cut

  uint l0flag(candLeps[0]->trigFlags), l1flag(candLeps[1]->trigFlags); 
  
  bool singleMuTrig( l0flag & TRIG_mu24i_tight && candLeps[0]->Pt()>25 );
  bool diLepTrig (passTrigger( (LeptonVector*) &candLeps, m_trigObj, met)); 

  if( !(singleMuTrig || diLepTrig))            return false; 

  float mt = Mt(candLeps[1], met); 
  if(mt>cut_mtMax)                             return false;
  
  //Get index from baseline lep stored in toy
  for(uint i=0; i<leptons->size(); ++i){
    Susy::Lepton* _l = leptons->at(i);
    if( _l == candLeps[0])  m_toyNt->_b_SSEM_tagIdx = i;
    else if( _l == candLeps[1])  m_toyNt->_b_SSEM_probeIdx = i;
  }
  /*
  cout << "SSEM " << nt->evt()->event <<endl; 
  dumpLeptons(&candLeps);
  if(m_toyNt->_b_SSEM_tagIdx>-1 ) 
    cout << " \t tag "   <<m_toyNt->_b_SSEM_tagIdx << " pT " << leptons->at(m_toyNt->_b_SSEM_tagIdx)->Pt() <<endl;;
  if(m_toyNt->_b_SSEM_probeIdx>-1 ) 
    cout << " \t probe " << m_toyNt->_b_SSEM_probeIdx << " pT " << leptons->at(m_toyNt->_b_SSEM_probeIdx)->Pt() << endl << endl;
  */

  return true;
}


/*--------------------------------------------------------------------------------*/
// HF Tag probe for muon/electron HF SF computation
// pass dilepton trigger
// preMuon in b-jet pT>20
// probe lepton
// if MM event, mll no in Z and >40
// Met <40
// mT(probe,met)<100
/*--------------------------------------------------------------------------------*/
bool SusyFakeAna::passHFTagProbe(LeptonVector* leptons, 
				 const JetVector* jets,
				 const Met* met)
{
  //Cut values
  const float cut_mllMin = 40;
  const float cut_metMax = 60;//40;
  const float cut_mtMax  = 100;
  const float cut_ptMu   = 20;

  resetCuts();
  
  if( v_preMu->size() == 0 ) return false;
  if( jets->size() == 0 )    return false;
  if( leptons->size() == 0 ) return false;

  //Look for a muon inside a b-jet - use preMuons
  Lepton* tag    = NULL;
  int     nTag   = 0;
  int     nBjets = 0;
  float   minDR  = 999;
  for(uint ij=0; ij<jets->size(); ++ij){
    const Jet* jet = jets->at(ij);
    if(!isCentralBJet(jet)) continue;
    nBjets++;
    for(uint im=0; im<v_preMu->size(); ++im){//preMuons 
      Muon* mu = v_preMu->at(im);
      if(fabs(mu->Eta())>2.4) continue;     
      float dR = jet->DeltaR(*mu);
      if(dR<M_J_DR){
	nTag++;
	minDR=dR;
	tag = (Lepton*) v_preMu->at(im);
      }
    }
  }
  if(nTag!=1)               return false;
  if(nBjets!=1)             return false;
  if(tag->Pt() < cut_ptMu)  return false;

  //Look for the probe e or mu. Check that's not the tag
  Lepton* probe   = NULL;
  int nProbe = 0;
  int idxTag=-1;
  int idxProbe=-1;
  for(uint i=0; i<leptons->size(); ++i){
    Susy::Lepton* _l = leptons->at(i);
    if(_l==tag){ //stored tag Idx if pass baseline cuts
      idxTag=i;
      continue; 
    }
    if(_l->isMu() && abs(_l->Eta())>2.4) continue;
    probe = _l;
    idxProbe = i;
    nProbe++;
  }
  if(nProbe!=1) return false;

  LeptonVector candLeps;
  candLeps.push_back( tag );
  candLeps.push_back( probe );
  
  m_ET = getDiLepEvtType(candLeps);
  if(m_ET==ET_me) m_ET=ET_em;                 //Keep EM & ME together  
  if( !(m_ET==ET_em  ||  m_ET==ET_mm))        return false;

  uint l0flag(candLeps[0]->trigFlags), l1flag(candLeps[1]->trigFlags); 
  bool singleMuTrig( l0flag & TRIG_mu18_tight && candLeps[0]->Pt()>20 );
  bool diLepTrig (passTrigger( (LeptonVector*) &candLeps, m_trigObj, met)); 
  if( !(singleMuTrig || diLepTrig))            return false; 

  //Kinematics cuts from MM events
  if(tag->isMu() && probe->isMu()){//MM event
    float mll= Mll(tag,probe);
    if(fabs(mll-MZ)<10) return false;
    if(mll<cut_mllMin) return false;
  }
  
  //Met and mT(probe,met) to reject bkg
  if(met->lv().Pt()>cut_metMax) return false;
  if(Mt(probe, met)>cut_mtMax) return false;
  
  m_toyNt->_b_HFTP_tagIdx=idxTag;
  m_toyNt->_b_HFTP_probeIdx=idxProbe;

  return true;

}

/*--------------------------------------------------------------------------------*/
// Extraction region for real/fake efficiency in MC 
/*--------------------------------------------------------------------------------*/
bool SusyFakeAna::passMCExtractionEff(LeptonVector* leptons, 
			   const JetVector* jets,
			   const Met* met)
{
  //Selection
  resetCuts();
  m_nLepMin   = 2;
  m_nLepMax   = 2;
  m_minC20    = 1;

  if(!passNLepCut(leptons))                  return false;
  if(!passMll20(leptons))                    return false;
  if(!passTrigger(leptons, m_trigObj, met) ) return false; 
  //if(!passLJet(jets)) return false;

  m_ET = getDiLepEvtType(*leptons);
  if(m_ET==ET_me) m_ET=ET_em;        //Keep EM & ME together  

  return true;

}

/*--------------------------------------------------------------------------------*/
// Z tag & probe
// == 2 baseline SF-OS
// Single lepton trigger for tag-tight
// mll within Z
// 
/*--------------------------------------------------------------------------------*/
bool SusyFakeAna::passZTagProbe(LeptonVector* leptons, 
				const JetVector* jets,
				const Met* met)
{
  resetCuts(); 
  m_nLepMin   = 2;
  m_nLepMax   = 2;
  m_selOS     = true;
  m_selSS     = false;
  m_selOF     = false;
  m_selSF     = true;

  //Selection
  if(!passNLepCut(leptons))                 return false;
  if(!passQQ(leptons))                      return false;
  if(!passFlavor(leptons))                  return false;
  
  m_ET = getDiLepEvtType(*leptons);
  if(m_ET==ET_me) m_ET=ET_em;                //Keep EM & ME together  

  //Lepton within Z window
  float mll = Mll(leptons->at(0),leptons->at(1));
  if(fabs(mll-MZ)>10)                       return false;

  //At least one tight
  int nVtx   = nt->evt()->nVtx;
  bool isMC  = nt->evt()->isMC;
  bool l0sig(isSignalLepton(leptons->at(0), *v_baseEle, *v_baseMu, nVtx, isMC));
  bool l1sig(isSignalLepton(leptons->at(1), *v_baseEle, *v_baseMu, nVtx, isMC));
  if( !l0sig && !l1sig )                    return false;

  //Tag pass single lepton trigger
  uint l0flag(leptons->at(0)->trigFlags), l1flag(leptons->at(1)->trigFlags);
  bool l0trig(leptons->at(0)->isEle() ? l0flag & TRIG_e24vhi_medium1 : l0flag & TRIG_mu24i_tight);
  bool l1trig(leptons->at(1)->isEle() ? l1flag & TRIG_e24vhi_medium1 : l1flag & TRIG_mu24i_tight); 

  //Save both combo if both lepton pass trigger and tight selection
  int nProbe = 0;
  if( l0sig && l0trig && leptons->at(0)->Pt() > 25 ){
    m_toyNt->_b_ZTP_tagIdx1=0;
    m_toyNt->_b_ZTP_probeIdx1=1;
    nProbe++;
  }
  if( l1sig && l1trig && leptons->at(1)->Pt() > 25 ){
    m_toyNt->_b_ZTP_tagIdx2=1;
    m_toyNt->_b_ZTP_probeIdx2=0;
    nProbe++;
  }
  
  if(nProbe==0)                            return false;

  return true;
}


/*--------------------------------------------------------------------------------*/
// Conversion CR
// 2 signal muon - OS 
// Muon(s) pass single or dilepton trigger with appropriate threshold
// ==1 baseline electron
// m_mme within Z
// 
/*--------------------------------------------------------------------------------*/
bool SusyFakeAna::passConv(LeptonVector* leptons, 
			   const JetVector* jets,
			   const Met* met)
{
  resetCuts(); 
  m_nLepMin   = 2;
  m_nLepMax   = 3;

  const float cut_metMax = 70;//50 2L
  const float cut_mtMax  = 40;

  if(!passNLepCut(leptons))                  return false;

  //2 signal muons + 1 baseline ele
  if( v_sigMu->size() != 2 )  return false;
  if( v_baseEle->size() != 1) return false;
  if( v_baseLep->size() != 3) return false;

  ElectronVector preElecs; 
  MuonVector     preMuons;
  preMuons.push_back( v_sigMu->at(0) );
  preMuons.push_back( v_sigMu->at(1) );
  preElecs.push_back( v_baseEle->at(0) );
  
  //OS mm
  if( preMuons[0]->q * preMuons[1]->q > 0 )  return false; // Opposite sign
  //m_mm > 20 
  if( (*preMuons[0]+*preMuons[1]).M() < 20 ) return false; // M(mu,mu) > 20

  //trigger
  //One muon passes single muon trigger & match w/ pT threshold
  //Both muon pass the dimuon trigger with matching.
  uint m0flag(preMuons[0]->trigFlags), m1flag(preMuons[1]->trigFlags);
  bool singleMuTrig( (m0flag & TRIG_mu24i_tight && preMuons[0]->Pt()>25 ) || 
		     (m1flag & TRIG_mu24i_tight && preMuons[1]->Pt()>25) );
  bool diMuonTrig (passTrigger( (LeptonVector*) &preMuons, m_trigObj, met)); 
  if( !(singleMuTrig || diMuonTrig)) return false; 

  //Trilepton mass within Z
  float mlll = (*preMuons[0] + *preMuons[1] + *preElecs[0]).M();
  if(fabs(mlll-MZ)>10) return false;

  if(met->lv().Pt()>cut_metMax) return false;
  //if(Mt(preElecs[0], met)>cut_mtMax) return false;

  //Find the index in the baseline !
  for(uint i=0; i<leptons->size(); ++i){
    Susy::Lepton* _l = leptons->at(i);
    if      (_l == preMuons[0]) m_toyNt->_b_ZConv_tagIdx1=i;
    else if (_l == preMuons[1]) m_toyNt->_b_ZConv_tagIdx2=i;
    else if (_l == preElecs[0]) m_toyNt->_b_ZConv_probeIdx=i;
  }


  return true;
}


/*--------------------------------------------------------------------------------*/
//
/*--------------------------------------------------------------------------------*/
bool SusyFakeAna::passZHFLF(LeptonVector* leptons, 
			    const JetVector* jets,
			    const Met* met)
{
  resetCuts(); 
  m_nLepMin   = 2;
  m_nLepMax   = 3;

  //at least 2 signal muons or 2 signal electrons + 1 baseline ele or mu
  if( v_baseLep->size() != 3)                         return false;
  if( !(v_sigMu->size() >1 || v_sigEle->size() >1) )  return false;
  
 

  LeptonVector tagLeptons;
  LeptonVector probeLeptons;
  
  //loop over the signal leptons, pick the 2 that are opposite sign, SF, closest to mZ?
  float best_mZ=999;
  for(uint ilep=0; ilep<v_sigLep->size(); ++ilep){
    for(uint slep=0; slep<v_sigLep->size(); ++slep){
      if(isSFOS(v_sigLep->at(ilep), v_sigLep->at(slep))){
	float mll = Mll(v_sigLep->at(ilep), v_sigLep->at(slep)); 
	for(uint iilep=0; iilep<leptons->size(); ++iilep){
	  Susy::Lepton* _l = leptons->at(iilep);
	  float mlll = (*(v_sigLep->at(ilep)) + *(v_sigLep->at(slep)) + *_l ).M();
	  if(fabs(mlll-MZ)<best_mZ){
	    tagLeptons.clear();
	    tagLeptons.push_back(v_sigLep->at(ilep));
	    tagLeptons.push_back(v_sigLep->at(slep));
	    best_mZ = mlll;
	  }
	}
// 	if( fabs(mll-MZ)<best_mZ){
// 	  tagLeptons.clear();
// 	  tagLeptons.push_back(v_sigLep->at(ilep));
//  	  tagLeptons.push_back(v_sigLep->at(slep));
// 	  best_mZ = mll;
// 	}
      }
    }
  }
  if(tagLeptons.size()<2) return false;
  //m_ll > 20 & within Z peak
  if( (*tagLeptons[0]+*tagLeptons[1]).M() < 20 )         return false; // M(l,l) > 20
  if( fabs((*tagLeptons[0]+*tagLeptons[1]).M()-MZ) >10 ) return false; // M(l,l) not in Z
  
  m_ET = getDiLepEvtType(tagLeptons);
  if(m_ET==ET_me) m_ET=ET_em;                //Keep EM & ME together  

  //Triggers
  uint l0flag(tagLeptons[0]->trigFlags), l1flag(tagLeptons[1]->trigFlags);
  bool singleMuTrig( (l0flag & TRIG_mu24i_tight && tagLeptons[0]->Pt()>25 && !tagLeptons[0]->isEle() ) || 
		     (l1flag & TRIG_mu24i_tight && tagLeptons[1]->Pt()>25 && !tagLeptons[0]->isEle()) );
  bool singleEleTrig((l0flag & TRIG_e24vhi_medium1 && tagLeptons[0]->Pt()>25 && tagLeptons[0]->isEle() ) || 
		     (l1flag & TRIG_e24vhi_medium1 && tagLeptons[1]->Pt()>25 && tagLeptons[0]->isEle()) );
  bool diLeptonTrig (passTrigger( (LeptonVector*) &tagLeptons, m_trigObj, met)); 
  if( !(singleMuTrig || singleEleTrig || diLeptonTrig)) return false; 


  //Find probe
  for(uint ilep=0; ilep<leptons->size(); ++ilep){
    Susy::Lepton* _l = leptons->at(ilep);
    if      (_l == tagLeptons[0]){
      m_toyNt->_b_ZHFLF_tagIdx1=ilep;
      continue;
    }
    else if (_l == tagLeptons[1]){
      m_toyNt->_b_ZHFLF_tagIdx2=ilep;
      continue;
    }
    m_toyNt->_b_ZHFLF_probeIdx=ilep;
    probeLeptons.push_back(leptons->at(ilep));
  }

  //Trilepton mass not within Z
  float mlll = (*tagLeptons[0] + *tagLeptons[1] + *probeLeptons[0]).M();
  if(fabs(mlll-MZ)<10) return false;
  m_toyNt->_b_ZHFLF_mlll = mlll;

  return true; 


}
