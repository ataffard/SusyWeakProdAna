#include <iomanip>
#include "TCanvas.h"
#include "SusyWeakProdAna/Susy2LepAna.h"
#include "SusyWeakProdAna/PhysicsTools.h"

#include "SusyWeakProdAna/SusyAnaCommon.h"

using namespace std;
using namespace Susy;

const char* const DIL_FLAV[] = {"EE", "MM", "EM"};
const char* const DIL_QQ[] = {"OS", "SS"};

const string DIL_SRNAME[] = {"SRjveto", "SRSSjveto", "SR2jets", "SRmT2",
			     "SRmT2b",
			     "CRZ", "NTOP", "NWW1", "NWW2", "NWW3",
			     "ZXCR1", "ZXCR3", "ZXCR4",
			     "CR2LepOS", "CR2LepSS"
};

/*--------------------------------------------------------------------------------*/
// Susy2LepAna Constructor
/*--------------------------------------------------------------------------------*/
Susy2LepAna::Susy2LepAna(SusyHistos* _histos):
  _hh(_histos),
  m_dbg(0),
  m_useLooseLep(false),
  m_nLepMin(2),
  m_nLepMax(2),
  m_cutNBaseLep(true),
  m_selOS(false),
  m_selSS(false),
  m_selSF    (false),
  m_vetoSF   (false),
  m_selZ       (false),
  m_vetoZ      (false),
  m_selB       (false),
  m_vetoB      (false),
  m_vetoJ      (false),
  m_sel2J      (false),
  m_metMin     ( -1  ),
  m_metMax     ( -1  ),
  m_metRelMin  ( -1  ),
  m_metRelMax  ( -1  ),
  m_topTag     (false),
  m_mt2Min     ( -1  ),
  m_mtMin      ( -1  ),
  m_mtMax      ( -1  ),
  m_mllMin     ( -1  ),
  m_lepLeadPtMin( -1  ),
  m_pTllMin     ( -1  ),
  m_pTllMax     ( -1  ),
  m_dPhiMetll( -1  ),
  m_dPhiMetl1( -1  ),
  m_ET(ET_Unknown)
{
  n_readin       = 0;
  n_pass_HotSpot = 0;
  n_pass_BadJet  = 0;
  n_pass_BadMuon = 0;
  n_pass_Cosmic  = 0;
  n_pass_BadFCAL = 0;
  n_pass_atleast2BaseLep = 0;
  n_pass_exactly2BaseLep = 0;

  // The rest are channel specific.
  for(int i=0; i<ET_N; ++i){
    n_pass_dil[i]    = 0;
    n_pass_signalLep[i]   = 0;
    n_pass_trig[i]    = 0;
    n_pass_truth[i]    = 0;

    for(int j=0; j<DIL_NSR; ++j){
      n_pass_ss[i][j]        = 0;
      n_pass_os[i][j]        = 0;
      n_pass_flav[i][j]      = 0;
      n_pass_Z[i][j]         = 0;
      n_pass_Jveto[i][j]     = 0;
      n_pass_2Jet[i][j]      = 0;
      n_pass_bJet[i][j]      = 0;
      n_pass_topTag[i][j]    = 0;
      n_pass_metRel[i][j]    = 0;
      n_pass_met[i][j]       = 0;
      n_pass_mt2[i][j]       = 0;
      n_pass_mll[i][j]       = 0;
      n_pass_pTll[i][j]      = 0;
      n_pass_leadLepPt[i][j] = 0;
      n_pass_sumLepPt[i][j]  = 0;
      n_pass_dPhiMetll[i][j] = 0;
      n_pass_dPhiMetl1[i][j] = 0;
    }
  }

  _random = new TRandom3(2012300958);
  

  m_trigObj = new DilTrigLogic(LUMW);
  

  // Configure using fake rates file
  // Currently rates are provided as function of pT only, so only use PT as second option
  string _fakeInput  =  string(getenv("WORKAREA")) + 
    "/SusyMatrixMethod/data/fakeRate_trial6_Oct2.root"; //e SF=1
  cout << "Loading fake MM " << _fakeInput << endl;
  m_matrix_method.configure(_fakeInput, SusyMatrixMethod::PT);


  //Configure ChargeFlip tool
  string chargeFlipInput =  string(getenv("WORKAREA")) + 
    "/ChargeFlip/data/chargeFlip.root";
  m_chargeFlip = new chargeFlip(chargeFlipInput);
  

  m_ET = ET_Unknown;


  _tmp=0;
}

/*--------------------------------------------------------------------------------*/
// Hook the containers
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::hookContainers(Susy::SusyNtObject* _ntPtr,
				 ElectronVector* _baseEleA, ElectronVector* _sigEleA,
				 MuonVector*     _baseMuoA, MuonVector*     _sigMuoA,
				 LeptonVector*   _baseLepA, LeptonVector*   _sigLepA,
				 JetVector*      _baseJetA, JetVector*      _sigJetA)
{
  nt         = _ntPtr;
  v_baseEle  = _baseEleA;
  v_sigEle   = _sigEleA;
  v_baseMu   = _baseMuoA;
  v_sigMu    = _sigMuoA;
  v_baseLep  = _baseLepA;
  v_sigLep   = _sigLepA;
  v_baseJet  = _baseJetA;
  v_sigJet   = _sigJetA;
}

/*--------------------------------------------------------------------------------*/
// Main process loop function 
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::doAnalysis()
{
  reset();

  //NB: Sept 19: signal Jet def: L25+B20+F30
  //Do selection for SR/CR/N-reg & fill plots
  if(m_useLooseLep){  //use baseline leptons - for fake MM estimate
    if(!selectEvent(v_baseLep, v_baseLep, v_sigJet, m_met)) return;
  }
  else{
    if(!selectEvent(v_sigLep, v_baseLep, v_sigJet, m_met)) return;
  }
  return;
}

/*--------------------------------------------------------------------------------*/
// End of job
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::end()
{
  cout << endl;
  cout << endl;
  cout << "**********************************************" << endl;
  cout << "Susy2LepAna::event counters" <<endl;

  cout << "read in:            " << n_readin       << endl;
  cout << "pass HotSpot:       " << n_pass_HotSpot << endl;
  cout << "pass BadJet:        " << n_pass_BadJet  << endl;
  cout << "pass BadMu:         " << n_pass_BadMuon << endl;
  cout << "pass Cosmic:        " << n_pass_Cosmic  << endl;
  cout << "pass Bad FCAL:      " << n_pass_BadFCAL << endl;
  cout << "pass atleast 2 base " << n_pass_atleast2BaseLep << endl;
  cout << "pass exactly 2 base " << n_pass_exactly2BaseLep << endl;
  
  string v_ET[ET_N] = {"ee","mm","em"};
  cout << "Channels        " << v_ET[0] << "\t " << v_ET[1] << "\t " << v_ET[2] <<endl;
  print_line("pass category ",n_pass_dil[0], n_pass_dil[1], n_pass_dil[2]);
  print_line("pass nLep     ",n_pass_signalLep[0], n_pass_signalLep[1], n_pass_signalLep[2]);
  print_line("pass trig     ",n_pass_trig[0],n_pass_trig[1],n_pass_trig[2]);
  print_line("pass truth    ",n_pass_truth[0],n_pass_truth[1],n_pass_truth[2]);
  print_SRjveto();
  print_SRSSjveto();
  print_SR2jets();
  print_SRmT2();
  print_CRZ();
  print_NTOP();
  print_NWW();
  print_NZX();


  cout << "CHECK INTEGRAL " << _tmp << endl;
}

/*--------------------------------------------------------------------------------*/
// Reset
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::reset()
{
  //_ww  = 0;
  _inc = 1;
  SR=DIL_NSR;
  m_ET = ET_Unknown;
  metRel = 0;
  mT2    = 0;
  mCT    = 0;
}
/*--------------------------------------------------------------------------------*/
// Set cut selection
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::setSelection(std::string s)
{
  m_sel = s;

  m_nLepMin = 2;
  m_nLepMax = 2;
  m_cutNBaseLep = true;
  m_selOS    = false;
  m_selSS    = false;
  m_selSF    = false;
  m_vetoSF   = false;
  m_selZ     = false;
  m_vetoZ    = false;
  m_selB     = false;
  m_vetoB    = false;
  m_vetoJ    = false;
  m_sel2J      = false;
  m_metMin     =  -1;
  m_metMax     =  -1;
  m_metRelMin  =  -1;
  m_metRelMax  =  -1;
  m_topTag     = false;
  m_mt2Min     =  -1;
  m_mtMin      =  -1;
  m_mtMax      =  -1;
  m_mllMin     =  -1;
  m_lepLeadPtMin = -1;
  m_pTllMin = -1;
  m_pTllMax = -1;
  m_dPhiMetll =  -1;
  m_dPhiMetl1 =  -1;

  if(m_sel == "SRjveto"){
    m_selOS = true;
    m_vetoZ = true;
    m_vetoJ = true;
    if(SKIP_METCUT_SR) m_metRelMin=0;
    else m_metRelMin=100;
  }
  else if(m_sel == "SRSSjveto"){
    m_selSS=true;
    m_vetoJ = true;
    if(SKIP_METCUT_SR) m_metRelMin=0;
    else m_metRelMin=100;
  }
  else if(m_sel == "SR2jets"){
    m_selOS = true;
    m_selSF = true;
    m_vetoZ = true;
    m_sel2J = true;
    m_vetoB = true;
    m_topTag = true;
    if(SKIP_METCUT_SR) m_metRelMin=0;
    else m_metRelMin=50;
  }
  else if(m_sel == "SRmT2"){
    m_selOS = true;
    m_vetoZ = true;
    m_vetoJ = true;
    m_metRelMin=40;
    if(SKIP_METCUT_SR) m_mt2Min=0;
    else  m_mt2Min=90;
  }
  else if(m_sel == "SRmT2b"){
    m_selOS = true;
    m_vetoZ = true;
    m_vetoJ = true;
    m_metRelMin=40;
    if(SKIP_METCUT_SR) m_mt2Min=0;
    else  m_mt2Min=110;
    /*    m_selOS = true;
	  m_vetoZ = true;
	  m_vetoJ = true;
	  if(SKIP_METCUT_SR) m_metRelMin=0;
	  else m_metRelMin=40;
	  m_lepLeadPtMin=50;
	  m_pTllMin =100;
	  m_dPhiMetll=2.5;
	  m_dPhiMetl1=0.5;
    */
  }
  else if(m_sel == "CRZ"){
    m_selOS = true;
    m_selZ  = true;
  }
  else if(m_sel == "NTOP"){
    m_selOS = true;
    m_vetoZ =true;
    m_sel2J=true;
    m_selB=true;
    m_metRelMin=40;
  }
  else if(m_sel == "NWW1"){
    m_selOS = true;
    m_vetoZ =true;
    m_vetoJ = true;
    m_metRelMin=70;
    m_metRelMax=100;
  }
  else if(m_sel == "NWW2"){
    m_selOS = true;
    m_vetoZ =true;
    m_vetoJ = true;
    m_metRelMin=70;
    m_metRelMax=100;
    m_mllMin=40;
  }
  else if(m_sel == "NWW3"){
    m_selOS = true;
    m_vetoZ =true;
    m_vetoJ = true;
    m_metRelMin=70;
    m_metRelMax=100;
    m_mllMin=40;
    m_pTllMin=40;
    m_pTllMax=150;
  }
  else if(m_sel == "ZXCR1"){
    m_selOS = true;
    m_selZ  = true;
    m_vetoJ = true;
    m_metRelMin=100;
  }
  else if(m_sel == "ZXCR3"){
    m_selOS = true;
    m_selZ  = true;
    m_topTag = true;
    m_metRelMin=50;
    m_sel2J = true;
    m_vetoB = true;
  }
  else if(m_sel == "ZXCR4"){
    m_selOS = true;
    m_selZ  = true;
    m_metRelMin=40;
    m_sel2J = true;
    m_vetoB = true;
  }
  else if(m_sel == "CR2LepOS"){
    m_selOS=true;
  }
  else if(m_sel == "CR2LepSS"){
    m_selSS=true;
  }
  
}

/*--------------------------------------------------------------------------------*/
// Full event selection
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::selectEvent(const LeptonVector* leptons, 
			      const LeptonVector* baseLeps, 
			      const JetVector* signalJets,
			      const Met* met)
{
  //Other apply Zveto applied to EM !!!!

  n_readin+=_inc;

  if(!passEventCleaning() ) return false;
  if(!passBadFCAL(v_baseJet,nt->evt()->run,nt->evt()->isMC)) return false;
  if( v_baseLep->size() < 2 )       return false;
  n_pass_atleast2BaseLep+=_inc;
  if( v_baseLep->size() != 2 )      return false;
  n_pass_exactly2BaseLep+=_inc;

  // Get Event Type to continue cutflow
  m_ET = getDiLepEvtType(*baseLeps);
  if(m_ET==ET_me) m_ET=ET_em; //Keep EM & ME together
  
  n_pass_dil[m_ET]+=_inc;

  if( !passNLepCut(leptons) )     return false;
  if( !passTrigger(leptons) )     return false; 
  if( !passIsPromptLepton(leptons,nt->evt()->isMC)) return false; 
  
  //
  //set _ww to the appropriate weighting
  //
  float _ww = eventWeight(LUMIMODE); 
  if(WEIGHT_COUNT) _inc = _ww;
  else _inc = nt->evt()->w;
  
  float bTagWeight =  getBTagSF(nt->evt(),v_baseJet);
  float _wwSave = _ww;

  //
  //Loop over SR's & CR's
  //
  for(uint iSR=DIL_SRjveto; iSR<DIL_NSR; iSR++){
    string sSR=DIL_SRNAME[iSR];
    setSelection(sSR);
    SR=iSR;
    int icut =0;
    _ww=_wwSave; //Reset weight in case used btagWeight in previous SR
    if(dbg() >10 ) cout << "Signal region " << sSR << endl;
    
    if(iSR==0 && dbg()>2){
      cout << "DG2L wPileup: " << nt->evt()->wPileupAB3
	   << " XS " << nt->evt()->xsec 
	   << " lumi A-B3 " << LUMI_A_B3 
	   << " sumw " << nt->evt()->sumw;
      cout << " lep SF " << v_sigLep->at(0)->effSF
	   << " " << v_sigLep->at(1)->effSF << endl;
    }

    // Apply bweight only to SR where we use jet count/veto
    if(USE_BWEIGHT && nt->evt()->isMC) {
      if( ! ( iSR ==DIL_CR2LepOS || iSR==DIL_CR2LepSS) )
	_ww = _wwSave * bTagWeight;
    }

    //For data - fake estimate
    if( !nt->evt()->isMC && m_useLooseLep){
      float _metRel = getMetRel(met,*leptons,*signalJets);
      _ww = getFakeWeight(leptons,nt->evt()->nVtx,nt->evt()->isMC,iSR,_metRel);
      //std::cout << " SR " << sSR << " Fake weight " << _ww << endl;
    }
    
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],icut++,_ww);

    /*
    if(!USE_QFLIP && !passQQ(leptons)) continue;
    if(USE_QFLIP && 
       (iSR==DIL_SRSSjveto || iSR==DIL_CR2LepSS) &&
       !m_ET==ET_mm){
      //Use OS event & charge flip prob fro EE & EM events
    }
    */

    if(!passQQ(leptons)) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],icut++,_ww);

    if(iSR==DIL_CR2LepOS && m_ET==ET_ee) _tmp += _ww;

    if(!passFlavor(leptons)) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],icut++,_ww);

    if(!passZVeto(leptons)) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass Zveto " << sSR << endl;

    if(!passJetVeto(signalJets)) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],icut++,_ww);

    if(!passge2CJet(signalJets)) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],icut++,_ww);

    if(!passbJet(signalJets)) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],icut++,_ww);
    
    if(!passTopTagger(leptons,signalJets,met) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass toptagger " << sSR << endl;

    if(!passMETRel(met,leptons,signalJets) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass MetRel " << sSR << endl;

    if(!passMT2(leptons, met) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],icut++,_ww);

    if(!passMll(leptons) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],icut++,_ww);

    if(!passPtll(leptons) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],icut++,_ww);

    if(!passLeadLepPt(leptons) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],icut++,_ww);

    if(!passSumLepPt(leptons) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],icut++,_ww);
    
    if(!passDPhiMetll(leptons,met) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],icut++,_ww);

    if(!passDPhiMetl1(leptons,met) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET],icut++,_ww);

    if(dbg() >10 ) cout << "\t Pass All " << sSR << endl;

    if(!passBlindData(nt->evt()->isMC,iSR, metRel,mT2)) continue;

    if(DO_FILL_HISTO ) fillHistograms(SR,leptons, signalJets,met,_ww);
    if(dbg() >10 ) cout << "\t Filled histos " << sSR << endl;

  }

  return true;
}
/*--------------------------------------------------------------------------------*/
// Event weight
/*--------------------------------------------------------------------------------*/
float Susy2LepAna::eventWeight(int mode)
{
  float _evtW=nt->evt()->w;

  if(mode==NOLUMI) _evtW= nt->evt()->w; //raw weight - generator included!
  else if(mode==LUMI1FB){
    _evtW=getEventWeightAB3(nt->evt());
    //_evtW=  nt->evt()->w ;
    //*  nt->evt()->wPileupAB3;
    //  *  nt->evt()->xsec 
    //  * LUMI_A_B3 
    //  / nt->evt()->sumw;
  }
  else if(mode==LUMI5FB){
    _evtW=getEventWeightAB(nt->evt());
  }
  else if(mode==LUMI13FB){
    _evtW=getEventWeight(nt->evt());
  }
  
  if(USE_LEPSF && nt->evt()->isMC){
    for(uint ilep=0; ilep<v_sigLep->size(); ilep++){
      const Susy::Lepton* _l = v_sigLep->at(ilep);
      _evtW *= _l->effSF;
    }
  }

  if(USE_DGWEIGHT && nt->evt()->isMC) 
    _evtW *= m_trigObj->getTriggerWeight(*v_sigLep,  nt->evt()->isMC, NtSys_NOM);

  //move this out of here so not to apply to Inc OS/SS
  //if(USE_BWEIGHT) _ww *= getBTagSF(nt->evt(),v_baseJet);

  return _evtW;
}

/*--------------------------------------------------------------------------------*/
// Fake Bkg estimate event weight
/*--------------------------------------------------------------------------------*/
float Susy2LepAna::getFakeWeight(const LeptonVector* leptons, uint nVtx, 
				 bool isMC, int iSR, float metrel)
{
  bool _isSignal[2];
  bool _isEle[2];
  float _pt[2];
  float _eta[2];
  
  if(leptons->size()>2) return 0;

  SusyMatrixMethod::FAKE_REGION  frSR = SusyMatrixMethod::FR_SRNONE;
  switch (iSR){
  case DIL_SRjveto:
    frSR = SusyMatrixMethod::FR_SR1;
    break;
  case DIL_SRSSjveto:
    frSR = SusyMatrixMethod::FR_SR2;
    break;
  case DIL_SR2jets:
    frSR = SusyMatrixMethod::FR_SR3;
    break;
  case DIL_SRmT2:
    frSR = SusyMatrixMethod::FR_SR4;
    break;
  case DIL_SRmT2b:
    frSR = SusyMatrixMethod::FR_SR4;
    break;
  case DIL_CRZ:
    frSR = SusyMatrixMethod::FR_SR1;
    break;
  case DIL_NTOP:
    frSR = SusyMatrixMethod::FR_TOPCR;
    break;
  case DIL_NWW1:
    frSR = SusyMatrixMethod::FR_WWCR1;
    break;
  case DIL_NWW2:
    frSR = SusyMatrixMethod::FR_WWCR2;
    break;
  case DIL_NWW3:
    frSR = SusyMatrixMethod::FR_WWCR3;
    break;
  case DIL_ZXCR1:
    frSR = SusyMatrixMethod::FR_SR1;
    break;
  case DIL_ZXCR3:
    frSR = SusyMatrixMethod::FR_SR1;
    break;
  case DIL_ZXCR4:
    frSR = SusyMatrixMethod::FR_SR1;
    break;
  case DIL_CR2LepOS:
    frSR = SusyMatrixMethod::FR_SR1; //FR_SRNONE
    break;
  case DIL_CR2LepSS:
    frSR = SusyMatrixMethod::FR_SR2;
    break;
  }

  for(uint i=0; i<leptons->size(); i++){
    _isEle[i]=leptons->at(i)->isEle();
    _pt[i]= leptons->at(i)->pt*1000;//MeV
    _eta[i]= leptons->at(i)->eta;
    if(_isEle[i])_isSignal[i] = isSignalElectron((Electron*) leptons->at(i),nVtx,isMC);
    else         _isSignal[i] = isSignalMuon((Muon*) leptons->at(i),nVtx,isMC);
    //cout << "\t Lepton isEle " << _isEle[i] << " isSignal " << _isSignal[i] 
    //<< " pt " << _pt[i] << " eta " << _eta[i] << " SR " << frSR << endl;  
  }  
  
  float _fw = 0;
  _fw = m_matrix_method.getTotalFake(_isSignal[0], _isEle[0], _pt[0],_eta[0],
				     _isSignal[1], _isEle[1], _pt[1],_eta[1],
				     frSR,       // What region should we get rates from?
				     SusyMatrixMethod::NONE);           // What systematics are to be applied?
  return _fw;
}

/*--------------------------------------------------------------------------------*/
// To determine is have true OS event
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::isTrueOS(const LeptonVector* leptons){
  //TO implement for charge flip estimate
  //leptons are Prompt and opposite sign and !chargeFlip
  return true;
}

/*--------------------------------------------------------------------------------*/
// Event cleaning
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passEventCleaning()
{
  int cutFlag = nt->evt()->evtFlag[NtSys_NOM];
  //  int cutFlag = nt->evt()->cutFlags[NtSys_NOM];

  if(!passHotSpot(cutFlag)) return false;
  n_pass_HotSpot+=_inc;

  if(!passBadJet (cutFlag)) return false;
  n_pass_BadJet+=_inc;

  if(!passBadMuon(cutFlag)) return false;
  n_pass_BadMuon+=_inc;

  if(!passCosmic (cutFlag)) return false;
  n_pass_Cosmic+=_inc;

  return true;
}

/*--------------------------------------------------------------------------------*/
// FCAL bad C1-C8 run206248-207332
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passBadFCAL(const JetVector* jets, int run, bool isMC)
{
  bool inBadFCAL=hasJetInBadFCAL(*jets);

  if(!isMC){ //Veto data C1-C8
    if(run>=BAD_FCAL_RUN1 && 
       run <=BAD_FCAL_RUN2 && 
       inBadFCAL) return false;
  }
  /*
  (isMC && LUMIMODE==LUMI13FB){
    float x = _random->Rndm();
    if(x < (BAD_FCAL_LUMI/LUMI_A_E) &&
       inBadFCAL)  return false;
  }
  */
  n_pass_BadFCAL+=_inc;
  return true;
}

/*--------------------------------------------------------------------------------*/
// Generic cuts
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passNLepCut(const LeptonVector* leptons)
{
  uint nLep = leptons->size();
  if(m_nLepMin>=0 && nLep < m_nLepMin) return false;
  if(m_nLepMax>=0 && nLep > m_nLepMax) return false;
  n_pass_signalLep[m_ET]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passTrigger(const LeptonVector* leptons)
{
  if(leptons->size() < 1) return false;
  if( m_trigObj->passDilTrig(*leptons, nt->evt()) ){
    n_pass_trig[m_ET]+=_inc;
    return true;
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passIsPromptLepton(const LeptonVector* leptons, bool isMC)
{
  if(leptons->size() < 1) return false;
  
  if(isMC && m_method == RLEP){
    for(uint ilep=0; ilep<leptons->size(); ilep++){
      const Susy::Lepton* _l = leptons->at(ilep);
      int org        = _l->mcOrigin;
      int type       = _l->mcType;
      int mcId       = nt->evt()->mcChannel;
      int truthMatch = _l->truthMatchType;
      bool isEle     = _l->isEle();
      
      bool isReal       = isPT(org,type,mcId,truthMatch,isEle);
      bool isChargeFlip =  _l->isEle() ? ((Electron*) _l)->isChargeFlip : false; 
      bool isConversion = isConv(org,type,truthMatch,isEle,isChargeFlip);

      
      //W/o including charge flip
      //    if( !( isReal && !isChargeFlip) ) return false;
      //To include the charge flip
      if( !( isReal  || isChargeFlip) ) return false;
    }
  }
  n_pass_truth[m_ET]+=_inc;
  return true;
}

/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passFlavor(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;
  bool sameFlav = isSameFlav(leptons->at(0), leptons->at(1));
  if(m_selSF && !sameFlav) return false;
  n_pass_flav[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passQQ(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;  
  float qq = leptons->at(0)->q * leptons->at(1)->q;
  if(m_selOS && qq >0 ) return false;
  if(m_selSS && qq <0 ) return false;
  if(m_selOS) n_pass_os[m_ET][SR]+=_inc;
  if(m_selSS) n_pass_ss[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
// Signal region cuts  - use all signal jets L25+B20+F30
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passJetVeto(const JetVector* jets)
{
  if(m_vetoJ && jets->size()>0) return false;

  n_pass_Jveto[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passbJet(const JetVector* jets,float cutVal){
  int nBJet= numberOfCBJets(*jets);
  if(m_vetoB && nBJet>0) return false;
  if(m_selB && nBJet<=0) return false;
  n_pass_bJet[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
float Susy2LepAna::getBTagSF(const Susy::Event*, const JetVector* jets)
{
  JetVector  valJets;
  valJets.clear();
  for(uint i=0; i<jets->size(); ++i){
    const Jet* jet = jets->at(i);
    if(fabs(jet->Eta()) <= JET_ETA_CUT) valJets.push_back(jet);
  }
  
  if(valJets.size()==0) return 1;//safety.
  return bTagSF(nt->evt(),valJets,USE_NOJVF_bSF);
}

/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passge2CJet(const JetVector* jets)
{
  int nCJet  = numberOfCLJets(*jets) + numberOfCBJets(*jets);
  int nFJet  = numberOfFJets(*jets);
  if(m_sel2J && nCJet < 2) return false;
  if(m_sel2J && nFJet >0) return false;
  n_pass_2Jet[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passZVeto(const LeptonVector* leptons, float Zlow, float Zhigh)
{
  if( leptons->size() < 2 ) return false;
  //bool hasz = hasZWindow(*leptons,Zlow, Zhigh); 
  
  //Z veto on EE/MM/EM of OS
  bool hasz=false;
  bool OS = (leptons->at(0)->q*leptons->at(1)->q < 0) ? 1 : 0;
  float mll=Mll(leptons->at(0),leptons->at(1));
  if(mll>Zlow && mll<Zhigh && OS) hasz=true;
    
  if(m_vetoZ && hasz) return false;
  if(m_selZ){
    hasz = hasZWindow(*leptons,Zlow, Zhigh); 
    if( !hasz) return false;
  }
  
  n_pass_Z[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passMETRel(const Met *met, const LeptonVector* leptons, const JetVector* jets)
{
  metRel = getMetRel(met,*leptons,*jets);
  if(m_metRelMin>=0 &&  metRel < m_metRelMin ) return false;
  if(m_metRelMax>=0 &&  metRel > m_metRelMax ) return false;
  n_pass_metRel[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passMET(const Met *met)
{
  float etmiss = met->lv().Pt();
  if(m_metMin>=0 &&  etmiss < m_metMin ) return false;
  if(m_metMax>=0 &&  etmiss > m_metMax ) return false;
  n_pass_met[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passLeadLepPt(const LeptonVector* leptons)
{
  if( leptons->size() < 1 ) return false;
  float pt0 = leptons->at(0)->Pt();
  if( m_lepLeadPtMin >-1 && pt0< m_lepLeadPtMin) return false;
  n_pass_leadLepPt[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passSumLepPt(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;
  float sumpt = leptons->at(0)->Pt() + leptons->at(1)->Pt();
  if( m_pTllMin >-1 && sumpt< m_pTllMin) return false;
   n_pass_sumLepPt[m_ET][SR]+=_inc;
  return true;
}

/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passPtll(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;
  TLorentzVector ll = (*leptons->at(0) + *leptons->at(1));
  if( m_pTllMin >-1 && ll.Pt() < m_pTllMin) return false;
  if( m_pTllMax >-1 && ll.Pt() > m_pTllMax) return false;
   n_pass_pTll[m_ET][SR]+=_inc;
  return true;
}

/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passDPhiMetll(const LeptonVector* leptons, const Met* met)
{
  TLorentzVector metlv = met->lv();
  TLorentzVector ll = (*leptons->at(0) + *leptons->at(1));
  if( m_dPhiMetll>-1 && !passdPhi(metlv, ll, m_dPhiMetll) ) return false;
  n_pass_dPhiMetll[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passDPhiMetl1(const LeptonVector* leptons, const Met* met)
{
  TLorentzVector metlv = met->lv();
  TLorentzVector ll = *leptons->at(1);
  if( m_dPhiMetl1>-1 && !passdPhi(metlv, ll, m_dPhiMetl1) ) return false;
  n_pass_dPhiMetl1[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passTopTagger(const LeptonVector* leptons, const JetVector* jets, const Met* met)
{
  if(m_topTag && !passTopTag(*leptons,*jets,met)) return false;
  n_pass_topTag[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passdPhi(TLorentzVector v0, TLorentzVector v1, float cut)
{
  return v0.DeltaPhi(v1) > cut;
}

/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passBlindData(bool isMC, int iSR, float metRel, float mt2){
  if(!isMC && BLIND_DATA){
    if( (iSR==DIL_SRjveto || iSR==DIL_SRSSjveto) && metRel>=m_metRelMin) return false;
    if( iSR==DIL_SR2jets && metRel>=m_metRelMin) return false;
    if( (iSR==DIL_SRmT2 ||iSR==DIL_SRmT2b) && mt2>m_mt2Min ) return false;
  }
  return true;
}

/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passMT2(const LeptonVector* leptons, const Met* met)
{
  mT2 = getMT2(*leptons, met);
  if(m_mt2Min>-1 && mT2<m_mt2Min) return false;
  n_pass_mt2[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passMll(const LeptonVector* leptons)
{
  float mll=Mll(leptons->at(0),leptons->at(1));
  if(m_mllMin>-1 && mll < m_mllMin) return false;
  n_pass_mll[m_ET][SR]+=_inc;
  return true;
}

/*--------------------------------------------------------------------------------*/
float Susy2LepAna::JZBJet(const JetVector* jets, const LeptonVector* leptons)
{
  float sumPtJet=0;
  float zPt=Mll(leptons->at(0),leptons->at(1));

  for(uint i=0; i<jets->size(); ++i){
   const Jet* jet = jets->at(i);
   sumPtJet+= jet->Pt();
  }

  return fabs(-sumPtJet)-fabs(zPt);
}
/*--------------------------------------------------------------------------------*/
float Susy2LepAna::JZBEtmiss(const Met *met, const LeptonVector* leptons)
{
  float etmiss=met->lv().Pt();
  float zPt=Mll(leptons->at(0),leptons->at(1));

  return fabs(-etmiss-zPt)-fabs(zPt);

}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_SRjveto()
{
  int j= DIL_SRjveto;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass Zveto  ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass Jveto  ",n_pass_Jveto[0][j], n_pass_Jveto[1][j], n_pass_Jveto[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_SRSSjveto()
{
  int j= DIL_SRSSjveto;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass SS     ",n_pass_ss[0][j], n_pass_ss[1][j], n_pass_ss[2][j]);
  print_line("pass Jveto  ",n_pass_Jveto[0][j], n_pass_Jveto[1][j], n_pass_Jveto[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_SR2jets()
{
  int j= DIL_SR2jets;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass SF     ",n_pass_flav[0][j], n_pass_flav[1][j], n_pass_flav[2][j]);
  print_line("pass ZVeto  ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass 2Jets  ",n_pass_2Jet[0][j], n_pass_2Jet[1][j], n_pass_2Jet[2][j]);
  print_line("pass bVeto  ",n_pass_bJet[0][j], n_pass_bJet[1][j], n_pass_bJet[2][j]);
  print_line("pass topTag ",n_pass_topTag[0][j], n_pass_topTag[1][j], n_pass_topTag[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_SRmT2()
{
  int j= DIL_SRmT2;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass ZVeto  ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass Jveto  ",n_pass_Jveto[0][j], n_pass_Jveto[1][j], n_pass_Jveto[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass MT2(90)",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
  j= DIL_SRmT2b;
  print_line("pass MT2(110)",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
}
/*--------------------------------------------------------------------------------*/
/*
void Susy2LepAna::print_SRmT2b()
{
  int j= DIL_SRmT2b;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS        ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass ZVeto     ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass Jveto     ",n_pass_Jveto[0][j], n_pass_Jveto[1][j], n_pass_Jveto[2][j]);
  print_line("pass MetRel    ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass LeadPt    ",n_pass_leadLepPt[0][j], n_pass_leadLepPt[1][j], n_pass_leadLepPt[2][j]);
  print_line("pass SumPt     ",n_pass_sumLepPt[0][j], n_pass_sumLepPt[1][j], n_pass_sumLepPt[2][j]);
  print_line("pass dPhiMEtll ",n_pass_dPhiMetll[0][j], n_pass_dPhiMetll[1][j], n_pass_dPhiMetll[2][j]);
  print_line("pass dPhiMetl1 ",n_pass_dPhiMetl1[0][j], n_pass_dPhiMetl1[1][j], n_pass_dPhiMetl1[2][j]);
}
*/
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_CRZ()
{
  int j= DIL_CRZ;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS        ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass Z         ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_NTOP()
{
 int j= DIL_NTOP;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass Z      ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass 2Jets  ",n_pass_2Jet[0][j], n_pass_2Jet[1][j], n_pass_2Jet[2][j]);
  print_line("pass btag   ",n_pass_bJet[0][j], n_pass_bJet[1][j], n_pass_bJet[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);

}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_NWW()
{
  int j= DIL_NWW1;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass Z      ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass Jveto  ",n_pass_Jveto[0][j], n_pass_Jveto[1][j], n_pass_Jveto[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  j= DIL_NWW2;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass Mll ",n_pass_mll[0][j], n_pass_mll[1][j], n_pass_mll[2][j]);
  j= DIL_NWW3;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass pTll ",n_pass_pTll[0][j], n_pass_pTll[1][j], n_pass_pTll[2][j]);


}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_NZX()
{
  int j= DIL_ZXCR1;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass Z      ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass Jveto  ",n_pass_Jveto[0][j], n_pass_Jveto[1][j], n_pass_Jveto[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  j= DIL_ZXCR3;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass 2Jets  ",n_pass_2Jet[0][j], n_pass_2Jet[1][j], n_pass_2Jet[2][j]);
  print_line("pass bVeto  ",n_pass_bJet[0][j], n_pass_bJet[1][j], n_pass_bJet[2][j]);
  print_line("pass topTag ",n_pass_topTag[0][j], n_pass_topTag[1][j], n_pass_topTag[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  j= DIL_ZXCR4;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  
}

/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_line(string s, float a, float b, float c)
{
  cout << setprecision(6)  << s << "\t" << a << "\t" << b << "\t" << c << endl;
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::fillHistograms(uint iSR,
				 const LeptonVector* leptons, 
				 const JetVector* jets,
				 const Met* met,
				 float _ww)
{
  _hh->H1FILL(_hh->DG2L_pred[iSR][m_ET],0.,_ww); 


  int q1=0;
  int q2=0;
  int qqType=0;
  TLorentzVector _ll;
  float dPhill=999;
  dPhill=leptons->at(0)->DeltaPhi(*leptons->at(1));
  dPhill=TVector2::Phi_mpi_pi(dPhill)*TMath::RadToDeg(); 
  for(uint ilep=0; ilep<leptons->size(); ilep++){
    const Susy::Lepton* _l = leptons->at(ilep);
    _ll = _ll + (*_l);
    bool isChargeFlip =  _l->isEle() ? ((Electron*) _l)->isChargeFlip : false; 
    LEP_TYPE lType = getType(_l->mcOrigin,
			     _l->mcType,
			     _hh->sampleName(),
			     nt->evt()->mcChannel,
			     _l->truthMatchType,
			     _l->isEle(),
			     isChargeFlip);
    if(ilep==0){
      _hh->H1FILL(_hh->DG2L_ptl1[iSR][m_ET],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etal1[iSR][m_ET],_l->Eta(),_ww); 
      _hh->H1FILL(_hh->DG2L_d0Sl1[iSR][m_ET],_l->d0/_l->errD0,_ww); 
      _hh->H1FILL(_hh->DG2L_z0sinthetal1[iSR][m_ET],_l->z0SinTheta(),_ww); 
      _hh->H1FILL(_hh->DG2L_orgl1[iSR][m_ET],lType,_ww); 
      q1=_l->q;
    }
    else if(ilep==1){
      _hh->H1FILL(_hh->DG2L_ptl2[iSR][m_ET],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etal2[iSR][m_ET],_l->Eta(),_ww); 
      _hh->H1FILL(_hh->DG2L_d0Sl2[iSR][m_ET],_l->d0/_l->errD0,_ww); 
      _hh->H1FILL(_hh->DG2L_z0sinthetal2[iSR][m_ET],_l->z0SinTheta(),_ww); 
      _hh->H1FILL(_hh->DG2L_orgl2[iSR][m_ET],lType,_ww); 
      q2=_l->q;
    }
    
  }//leptons 
  
  if(q1*q2<0){//OS
    qqType = (q1<0 && q2>0) ? 0 : 1;  //-+/+-
  }
  else if(q1*q2>0){//SS
    qqType = (q1<0 && q2<0) ? 0 : 1;  //--/++
  }
  _hh->H1FILL(_hh->DG2L_qq[iSR][m_ET],qqType,_ww); 
  
  float mWT = mT(_ll, met->lv());

  _hh->H1FILL(_hh->DG2L_mll[iSR][m_ET],_ll.M(),_ww); 
  _hh->H1FILL(_hh->DG2L_pTll[iSR][m_ET],_ll.Pt(),_ww); 
  _hh->H1FILL(_hh->DG2L_mWWT[iSR][m_ET],mWT,_ww); 
  _hh->H1FILL(_hh->DG2L_dPhill[iSR][m_ET],dPhill,_ww); 
  _hh->H1FILL(_hh->DG2L_JZBJet[iSR][m_ET],JZBJet(v_sigJet,leptons),_ww); 
  _hh->H1FILL(_hh->DG2L_JZBEtmiss[iSR][m_ET],JZBEtmiss(met,leptons),_ww); 
  _hh->H1FILL(_hh->DG2L_etmiss[iSR][m_ET],met->lv().Pt(),_ww); 
  _hh->H1FILL(_hh->DG2L_metrel[iSR][m_ET],metRel,_ww); 
  _hh->H1FILL(_hh->DG2L_metRefEle[iSR][m_ET],met->refEle,_ww); 
  _hh->H1FILL(_hh->DG2L_metRefGam[iSR][m_ET],met->refGamma,_ww); 
  _hh->H1FILL(_hh->DG2L_metRefMuo[iSR][m_ET],met->refMuo,_ww); 
  _hh->H1FILL(_hh->DG2L_metRefJet[iSR][m_ET],met->refJet,_ww); 
  _hh->H1FILL(_hh->DG2L_metRefSJet[iSR][m_ET],met->softJet,_ww); 
  _hh->H1FILL(_hh->DG2L_metCellout[iSR][m_ET],met->refCell,_ww); 
  _hh->H1FILL(_hh->DG2L_mt2[iSR][m_ET],mT2,_ww); 

  float corrNpv = nt->evt()->nVtx;
  if(nt->evt()->isMC) corrNpv = GetNVertexBsCorrected(nt->evt()->nVtx);
  _hh->H1FILL(_hh->DG2L_npv[iSR][m_ET],corrNpv,_ww); 
  _hh->H1FILL(_hh->DG2L_mu[iSR][m_ET],nt->evt()->avgMu,_ww); 

  int nBJets=0;
  int nSigJet=0;
  int nSigCJet=0;
  int nSigFJet=0;
  for(uint ijet=0; ijet<jets->size(); ijet++){
    const Susy::Jet* _j = jets->at(ijet);
    nSigJet++;
    if(isCentralLightJet(_j)) nSigCJet++;
    if(isForwardJet(_j)) nSigFJet++;
    if(isCentralBJet(_j)){
      nBJets++;
      _hh->H1FILL(_hh->DG2L_ptbj[iSR][m_ET],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etabj[iSR][m_ET],_j->Eta(),_ww); 
    }

    if(ijet==0){
      _hh->H1FILL(_hh->DG2L_ptj1[iSR][m_ET],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etaj1[iSR][m_ET],_j->Eta(),_ww); 
    }
    if(ijet==1){
      _hh->H1FILL(_hh->DG2L_ptj2[iSR][m_ET],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etaj2[iSR][m_ET],_j->Eta(),_ww); 
    }
    if(ijet==2){
      _hh->H1FILL(_hh->DG2L_ptj3[iSR][m_ET],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etaj3[iSR][m_ET],_j->Eta(),_ww); 
    }
    if(ijet==3){
      _hh->H1FILL(_hh->DG2L_ptj4[iSR][m_ET],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etaj4[iSR][m_ET],_j->Eta(),_ww); 
    }
  }
  _hh->H1FILL(_hh->DG2L_nJets[iSR][m_ET],nSigJet,_ww); 
  _hh->H1FILL(_hh->DG2L_nCJets[iSR][m_ET],nSigCJet,_ww); 
  _hh->H1FILL(_hh->DG2L_nFJets[iSR][m_ET],nSigFJet,_ww); 
  _hh->H1FILL(_hh->DG2L_nBJets[iSR][m_ET],nBJets,_ww); 


  //Reco mW from jj in Z events w/o btag
  if(hasZWindow(*leptons,MZ-10, MZ+10) && nSigJet==2 
     &&nBJets==0){
    TLorentzVector _jj = (*jets->at(0)) + (*jets->at(1));
    _hh->H1FILL(_hh->DG2L_mjj[iSR][m_ET],_jj.M(),_ww); 
  }



}
