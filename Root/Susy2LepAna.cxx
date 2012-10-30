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
			     "ZXCR1", "ZXCR3", "ZXCR4","ZXCR5",
			     "CR2LepOS", "CR2LepSS",
			     "preSRjveto", "preSRSSjveto", "preSR2jets", "preSRmT2",
			     "VR1SS"
};

/*--------------------------------------------------------------------------------*/
// Susy2LepAna Constructor
/*--------------------------------------------------------------------------------*/
Susy2LepAna::Susy2LepAna(SusyHistos* _histos):
  _hh(_histos),
  m_dbg(0),
  m_useLooseLep(false),
  m_writeHFT(false),

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
 
  reset();
  resetCounter();

  _random = new TRandom3(2012300958);

  m_trigObj = new DilTrigLogic(LUMW);  
  if(USE_MCTRIG && !USE_DGWEIGHT){
    m_trigObj->useMCTrigger();
    cout << " MESSAGE: Using MC trigger decision " << endl;
  }

  // Configure using fake rates file
  // Currently rates are provided as function of pT only, so only use PT as second option
  string _fakeInput  =  string(getenv("WORKAREA")) + 
    "/SusyMatrixMethod/data/fakeRate_trial7_Oct12.root"; 
  cout << "Loading fake MM " << _fakeInput << endl;
  m_matrix_method.configure(_fakeInput, SusyMatrixMethod::PT);


  //Configure ChargeFlip tool
  string chargeFlipInput =  string(getenv("WORKAREA")) + 
    "/ChargeFlip/data/chargeFlip.root";
  m_chargeFlip = new chargeFlip(chargeFlipInput);
  
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

  if(DUMP_RUNEVT){
    string dumpName = _hh->sampleName()+ "_dump.txt";
    evtDump.open (dumpName.c_str());
  }

}

/*--------------------------------------------------------------------------------*/
// Main process loop function 
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::doAnalysis(unsigned int isys)
{
  reset();
  SYST = isys;

  if(FILL_HFT && !m_writeHFT) initializeHistFitterTree();

  //cout << "Analysing event for Sys " << DG2LSystNames[SYST] <<endl;

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
  cout << "pass mll20          " << n_pass_mll20   << endl;
  
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

  if(DUMP_RUNEVT){
    evtDump.close ();
  }
  
  if(m_writeHFT){ //Saving HistFitterTree - yes that's done in delete contructor!
    for(uint i=DGSys_NOM; i < DGSys_GEN ; i++) {
      if( m_histFitterTrees[i]){
	float sumw = nt->evt()->sumw;
	if(nt->evt()->mcChannel==147771){
	  sumw = 2.35447863214e+13;
	  cout << "Warning overwritting sumW in " << nt->evt()->mcChannel << " with " << sumw << endl;
	}
	if(nt->evt()->mcChannel==147772){
	  sumw = 1.23966712709e+13;
	  cout << "Warning overwritting sumW in " << nt->evt()->mcChannel << " with " << sumw << endl;
	  }
	m_histFitterTrees[i]->setSumOfMcWeights(sumw); 
	delete m_histFitterTrees[i];
      }
    }
    moveHFTOutput();
  }

  clearVectors();
}
/*--------------------------------------------------------------------------------*/
// Move output of this HFT to a decent location
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::moveHFTOutput()
{
  string dir =  string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/HFTOutputs";
  gSystem->mkdir(dir.c_str(),kTRUE);
  
  if(nt->evt()->isMC){
    TString mcId = "";
    TString systName = "" ;
    mcId.Form("%i",nt->evt()->mcChannel);
    for(uint i=_sys1; i <=_sys2; i++) {
      systName.Form("%s",DG2LSystNames[i].c_str());
      TString fName = systName+"_"+mcId+".root";
      string cmd = "mv " + string(fName.Data()) + " " + dir;
      std::cout << "Moving MC HFT file " << cmd << std::endl;
      gSystem->Exec(cmd.c_str());
    }
  }
  else{
    string cmd="";
    string list ="";
    if(m_method == FLEP){
      list = "HFTlist_FAKE_" + HFTName;
      cmd = "ls -1 *"+ HFTName + ".root >" +list;
    }
    else{
      list = "HFTlist_FAKE_" + HFTName;
      cmd = "ls -1 central_"+ HFTName + ".root >" +list;
    }
    gSystem->Exec(cmd.c_str());
    
    FILE* fInput;
    if ((fInput = fopen(list.c_str(),"r")) == NULL) {
      cout << "File " << list << " could not be opened. Exit" << endl;;
      abort();
    }
    char _name[200];
    while (!feof( fInput )) {
      if (fscanf(fInput, "%s\n",&_name[0])){
	string cmd2 = "mv " + string(_name) + " " + dir;
	std::cout << "Move " << cmd2 << std::endl;
	gSystem->Exec(cmd2.c_str());
      }
    }
    cmd = "rm -f " + list;
    gSystem->Exec(cmd.c_str());
  }

}

/*--------------------------------------------------------------------------------*/
// Reset 
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::resetCounter()
{
 
  n_readin       = 0;
  n_pass_HotSpot = 0;
  n_pass_BadJet  = 0;
  n_pass_BadMuon = 0;
  n_pass_Cosmic  = 0;
  n_pass_BadFCAL = 0;
  n_pass_atleast2BaseLep = 0;
  n_pass_exactly2BaseLep = 0;
  n_pass_mll20   = 0;

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

}
/*--------------------------------------------------------------------------------*/
// Reset  per event
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::reset()
{
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
    m_metRelMin=100;
  }
  if(m_sel == "preSRjveto"){
    m_selOS = true;
    m_vetoZ = true;
    m_vetoJ = true;
    m_metRelMin=0;
  }
  else if(m_sel == "SRSSjveto"){
    m_selSS=true;
    m_vetoJ = true;
    m_metRelMin=100;
  }
  else if(m_sel == "preSRSSjveto"){
    m_selSS=true;
    m_vetoJ = true;
    m_metRelMin=0;
  }
  else if(m_sel == "VR1SS"){
    m_selSS=true;
    //Matt applies Zveto in ee - will not to get the qFlip
    m_metRelMin=40;
    m_metRelMax=100;
  }
  else if(m_sel == "SR2jets"){
    m_selOS = true;
    m_selSF = true;
    m_vetoZ = true;
    m_sel2J = true;
    m_vetoB = true;
    m_topTag = true;
    m_metRelMin=50;
  }
  else if(m_sel == "preSR2jets"){
    m_selOS = true;
    m_selSF = true;
    m_vetoZ = true;
    m_sel2J = true;
    m_vetoB = true;
    m_topTag = true;
    m_metRelMin=0;
  }
  else if(m_sel == "preSRmT2"){
    m_selOS = true;
    m_vetoZ = true;
    m_vetoJ = true;
    m_metRelMin=40;
    m_mt2Min=0;
  }
  else if(m_sel == "SRmT2"){
    m_selOS = true;
    m_vetoZ = true;
    m_vetoJ = true;
    m_metRelMin=40;
    m_mt2Min=90;
  }
  else if(m_sel == "SRmT2b"){
    m_selOS = true;
    m_vetoZ = true;
    m_vetoJ = true;
    m_metRelMin=40;
    m_mt2Min=110;
    /*    m_selOS = true;
	  m_vetoZ = true;
	  m_vetoJ = true;
	  if(METRELCUT_SR) m_metRelMin=0;
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
    m_mllMin=30;
  }
  else if(m_sel == "NWW3"){
    m_selOS = true;
    m_vetoZ =true;
    m_vetoJ = true;
    m_metRelMin=70;
    m_metRelMax=100;
    m_mllMin=40;
    //    m_pTllMin=40;
    //    m_pTllMax=150;
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
    m_vetoJ = true;
    m_metRelMin=40;
  }
  else if(m_sel == "ZXCR5"){
    m_selOS = true;
    m_selZ  = true;
    m_vetoJ = true;
    m_metRelMin=70;
    m_metRelMax=100;
  }
  else if(m_sel == "CR2LepOS"){
    m_selOS=true;
    if(METREL40) m_metRelMin=40;
  }
  else if(m_sel == "CR2LepSS"){
    m_selSS=true;
    if(METREL40) m_metRelMin=40;
  }
  
}

/*--------------------------------------------------------------------------------*/
// Full event selection
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::selectEvent(LeptonVector* leptons, 
			      LeptonVector* baseLeps, 
			      const JetVector* signalJets,
			      const Met* met)
{
  if(dbg()>10){ 
    cout << "\n >>> run " << nt->evt()->run  
	 << " event " << nt->evt()->event 
	 << " SYST " << DG2LSystNames[SYST]
	 << endl;
    dumpEvent();
  }

  if(SYST==DGSys_NOM) n_readin+=_inc;

  if(!passEventCleaning() ){
    if(dbg()>10) cout<<"Fail cleaning" << endl;  
      return false;
    }
  if(!passBadFCAL(v_baseJet,
		  nt->evt()->run,
		  nt->evt()->isMC)){
    if(dbg()>10) cout<<"Fail FCAL" << endl; 
      return false;
    }
  if( v_baseLep->size() < 2 ){ 
    if(dbg()>10) cout<<"Fail baselep1 " << endl;  
      return false;
    }
  if(SYST==DGSys_NOM) n_pass_atleast2BaseLep+=_inc;
  if( v_baseLep->size() != 2 ){ 
    if(dbg()>10) cout<<"Fail baselep2 " << endl;
      return false;
    }
  if(SYST==DGSys_NOM) n_pass_exactly2BaseLep+=_inc;
  if(! passMll20(baseLeps)){ 
    if(dbg()>10) cout<<"Fail Mll20 " << endl; 
      return false;
    }
  if(SYST==DGSys_NOM) n_pass_mll20+=_inc;

  // Get Event Type to continue cutflow
  m_ET = getDiLepEvtType(*baseLeps);
  if(m_ET==ET_me) m_ET=ET_em; //Keep EM & ME together
  
  if(SYST==DGSys_NOM) n_pass_dil[m_ET]+=_inc;

  if( !passNLepCut(leptons) ){ 
    if(dbg()>10) cout<<"Fail Nlep " << endl; 
      return false;
    }
  if( !passTrigger(leptons) ){ 
    if(dbg()>10) cout<<"Fail Trig " << endl;  
      return false; 
    }
  if( !passIsPromptLepton(leptons,nt->evt()->isMC)){
    if(dbg()>10) cout<<"Fail Prompt " << endl; 
      return false; 
    }
  
  //
  //set _ww to the appropriate weighting
  //
  float _ww      = eventWeight(LUMIMODE); 
  float _lepSFW  = getLepSFWeight(leptons);
  float _trigW   = getTriggerWeight(leptons, SYST);
  _ww           *= _lepSFW * _trigW;
  
  float bTagWeight =  getBTagSF(nt->evt(),v_baseJet);
  float _wwSave = _ww;

  if(WEIGHT_COUNT) _inc = _ww;
  else _inc = nt->evt()->w;


  //
  //Write HistFitterTree here
  //
  if(m_writeHFT){
    writeIntoHistFitterTree(leptons,baseLeps,signalJets,v_baseJet,met);
  }
   
  if(dbg()>10){ 
    cout << "\n >>> run " << nt->evt()->run  
	 << " event " << nt->evt()->event 
	 << " SYST " << DG2LSystNames[SYST]
	 << " lepSF " << _lepSFW
	 << " trigW " << _trigW
	 << " bTag " << bTagWeight
	 << " weight " << _ww << endl;
    //    dumpEvent();
  }
 
  saveOriginal(); //Backup Met & leptons

  //
  //Loop over SR's & CR's
  //
  for(uint iSR=DIL_SRjveto; iSR<DIL_NSR; iSR++){
    string sSR=DIL_SRNAME[iSR];
    setSelection(sSR);
    SR=iSR;
    if(dbg() > 2 ) cout << "Signal region " << sSR << endl;

    int icut =0;
    _ww=_wwSave; //Reset weight in case used btagWeight in previous SR
    //Only in MC do we alter the Ele pt 
    if(nt->evt()->isMC) restoreOriginal(*leptons,met);

    //For data - fake estimate
    if( !nt->evt()->isMC && m_useLooseLep){
      float _metRel = getMetRel(&new_met,*leptons,*signalJets);
      _ww = getFakeWeight(leptons,nt->evt()->nVtx,nt->evt()->isMC,iSR,_metRel);
      if(WEIGHT_COUNT) _inc = _ww;
      //std::cout << " SR " << sSR << " Fake weight " << _ww << endl;
    }
    
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    bool allowZVeto=true; //To prevent applying ZVeto to OS events used for SS estimate
    if(!USE_QFLIP && !passQQ(leptons)) continue;
    if(USE_QFLIP){
      if( nt->evt()->isMC && m_method == RLEP &&  m_ET!=ET_mm &&
	  (iSR==DIL_SRSSjveto || iSR==DIL_CR2LepSS) ){
	if(isGenuineSS(leptons) && SYST==DGSys_NOM )  n_pass_ss[m_ET][SR]+=_inc; //genuine SS - no qFlip
	else{ //OS ee/em event - get the qFlip prob
	  float _ww_qFlip = getQFlipProb(leptons,&new_met);
	  _ww *= _ww_qFlip;
	  _inc = _ww;
	  if(SYST==DGSys_NOM) n_pass_ss[m_ET][SR]+=_inc;
	  allowZVeto=false;
	}
      }
      else
	if(!passQQ(leptons)) continue;
    }

    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passFlavor(leptons)) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passZVeto(leptons) && allowZVeto ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass Zveto " << sSR << endl;
    
    // Apply bweight only to SR where we use jet count/veto
    if(USE_BWEIGHT && nt->evt()->isMC) {
      if( ! ( iSR ==DIL_CR2LepOS || iSR==DIL_CR2LepSS || iSR==DIL_CRZ) )
	_ww = _wwSave * bTagWeight;
	if(WEIGHT_COUNT) _inc = _ww;
	//cout << " Wsave " << _wwSave << " btag " <<  bTagWeight << " w " << _ww << endl;
    }

    if(!passJetVeto(signalJets)) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passge2Jet(signalJets, iSR)) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passbJet(signalJets)) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passTopTagger(leptons,signalJets,&new_met) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass toptagger " << sSR << endl;

    if(!passMETRel(&new_met,leptons,signalJets) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass MetRel " << sSR << endl;

    if(!passMT2(leptons, &new_met) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passMll(leptons) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passPtll(leptons) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passLeadLepPt(leptons) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passSumLepPt(leptons) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    if(!passDPhiMetll(leptons,&new_met) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passDPhiMetl1(leptons,&new_met) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(dbg() >10 ) cout << "\t Pass All " << sSR << endl;

    if(!passBlindData(nt->evt()->isMC,iSR, metRel,mT2)) continue;

    if(DO_FILL_HISTO ) fillHistograms(SR,SYST,leptons, signalJets,&new_met,_ww);
    if(dbg() >10 ) cout << "\t Filled histos " << sSR << endl;

    //Dump run event
    //    if(DUMP_RUNEVT && (iSR==DIL_CR2LepOS || iSR==DIL_CR2LepSS) ){
    //if(DUMP_RUNEVT && SYST==DGSys_NOM && (iSR==DIL_NTOP && m_ET==ET_ee) ){
    if(DUMP_RUNEVT && (iSR==DIL_SRjveto) ){
      float wl1 = leptons->at(0)->effSF;
      float wl2 = leptons->at(1)->effSF;

      evtDump << nt->evt()->run 
	      << " " << nt->evt()->event 
	      << " " << sSR 
	      << " " << DIL_FLAV[m_ET] 
	      << " " << _lepSFW
	      << " " << bTagWeight
	      << " " << _trigW
	      << " " << _ww 
	      << endl;
    }
  }

  if(nt->evt()->isMC) restoreOriginal(*leptons,met);
  // clearVectors();

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
    if(USE_MCWEIGHT) _evtW =  nt->evt()->w;
    else _evtW=getEventWeightFixed(nt->evt()->mcChannel,nt->evt(),LUMI_A_B3);
  }
  else if(mode==LUMI5FB){
    //_evtW=getEventWeightAB(nt->evt());
    if(USE_MCWEIGHT) _evtW =  nt->evt()->w;
    else _evtW=getEventWeightFixed(nt->evt()->mcChannel,nt->evt(),LUMI_A_B14);
  }
  else if(mode==LUMI13FB){
    if(USE_MCWEIGHT) _evtW =  nt->evt()->w;
    else _evtW=getEventWeightFixed(nt->evt()->mcChannel,nt->evt(),LUMI_A_E);
    //Fixes Sherpa Zmumu weight 
    //_evtW=getEventWeight(nt->evt());
  }
  return _evtW;
}

/*--------------------------------------------------------------------------------*/
// Lepton SF weight
/*--------------------------------------------------------------------------------*/
float Susy2LepAna::getLepSFWeight(const LeptonVector* leptons)
{
  float _wLepSF=1;
  if(USE_LEPSF && nt->evt()->isMC){
    for(uint ilep=0; ilep<leptons->size(); ilep++){
      const Susy::Lepton* _l = leptons->at(ilep);
      _wLepSF *= _l->effSF;
      if(dbg()>10) cout << "lep" << ilep << " SF " << _l->effSF << endl; 
    }
  }
  return _wLepSF;
}
/*--------------------------------------------------------------------------------*/
// Trigger weight
/*--------------------------------------------------------------------------------*/
float  Susy2LepAna::getTriggerWeight(const LeptonVector* leptons, uint iSys){
  float _wTrig=1;
  
  if(USE_DGWEIGHT && nt->evt()->isMC) {
    uint iiSys = DGSys_NOM;
    if(iSys==DGSys_TRIGSF_EL_UP) iiSys=NtSys_TRIGSF_EL_UP;
    if(iSys==DGSys_TRIGSF_EL_DN) iiSys=NtSys_TRIGSF_EL_DN;
    if(iSys==DGSys_TRIGSF_MU_UP) iiSys=NtSys_TRIGSF_MU_UP;
    if(iSys==DGSys_TRIGSF_MU_DN) iiSys=NtSys_TRIGSF_MU_DN;
    
    float _wTrigNoSys =  m_trigObj->getTriggerWeight(*leptons,  nt->evt()->isMC, (SusyNtSys) 0);
    _wTrig =  m_trigObj->getTriggerWeight(*leptons,  nt->evt()->isMC, (SusyNtSys) iiSys);
    if(_wTrig<0 || _wTrig>1) {
      //cout << "WARNING Trigger weight out of bound - set to 0 or 1 " << DIL_FLAV[m_ET]  << " " << _wtrig << endl;
      if(_wTrig<0) _wTrig=0;
      if(_wTrig>0) _wTrig=1;
    }
    if(dbg()>-10) cout << "No sys "<< _wTrigNoSys << " " << iiSys << " trigW " << _wTrig << endl; 
  }
  return _wTrig;
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
    frSR = SusyMatrixMethod::FR_SRNONE;
    break;
  case DIL_ZXCR3:
    frSR = SusyMatrixMethod::FR_SRNONE;
    break;
  case DIL_ZXCR4:
    frSR = SusyMatrixMethod::FR_SRNONE;
    break;
  case DIL_ZXCR5:
    frSR = SusyMatrixMethod::FR_SRNONE;
    break;
  case DIL_CR2LepOS:
    frSR = SusyMatrixMethod::FR_SRNONE; //FR_SRNONE
    break;
  case DIL_CR2LepSS:
    frSR = SusyMatrixMethod::FR_VR1;
    break;
  }

  for(uint i=0; i<leptons->size(); i++){
    _isEle[i]=leptons->at(i)->isEle();
    _pt[i]= leptons->at(i)->pt*1000;//MeV
    _eta[i]= leptons->at(i)->eta;
    if(_isEle[i])_isSignal[i] = isSignalElectron((Electron*) leptons->at(i),nVtx,isMC);
    else         _isSignal[i] = isSignalMuon((Muon*) leptons->at(i),nVtx,isMC);
  }  
  
  float _fw = 0;
  uint iiSys = DGSys_NOM;
  if(SYST==DGSys_FAKE_EL_RE_UP) iiSys=SusyMatrixMethod::SYS_EL_RE_UP;
  if(SYST==DGSys_FAKE_EL_RE_DN) iiSys=SusyMatrixMethod::SYS_EL_RE_DOWN;
  if(SYST==DGSys_FAKE_EL_FR_UP) iiSys=SusyMatrixMethod::SYS_EL_FR_UP;
  if(SYST==DGSys_FAKE_EL_FR_DN) iiSys=SusyMatrixMethod::SYS_EL_FR_DOWN;
  if(SYST==DGSys_FAKE_MU_RE_UP) iiSys=SusyMatrixMethod::SYS_MU_RE_UP;
  if(SYST==DGSys_FAKE_MU_RE_DN) iiSys=SusyMatrixMethod::SYS_MU_RE_DOWN;
  if(SYST==DGSys_FAKE_MU_FR_UP) iiSys=SusyMatrixMethod::SYS_MU_FR_UP;
  if(SYST==DGSys_FAKE_MU_FR_DN) iiSys=SusyMatrixMethod::SYS_MU_FR_DOWN;

  _fw = m_matrix_method.getTotalFake(_isSignal[0], _isEle[0], _pt[0],_eta[0],
				     _isSignal[1], _isEle[1], _pt[1],_eta[1],
				     frSR,                       // What region should we get rates from?
				     metrel, 
				     (SusyMatrixMethod::SYSTEMATIC) iiSys);  // What systematics are to be applied?

  if(dbg()>10) cout << "SR " << DIL_SRNAME[iSR] 
		    << " applying Ssys " << SYST 
		    << " " << DG2LSystNames[SYST] << " fw " << _fw << endl;
  
  return _fw;
}


/*--------------------------------------------------------------------------------*/
// Event cleaning
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passEventCleaning()
{
  //int cutFlag = nt->evt()->evtFlag[NtSys_NOM];
  uint iiSys = SYST;
  if(SYST>DGSys_RESOST) iiSys = DGSys_NOM;

  int cutFlag = nt->evt()->cutFlags[iiSys];
  //cout << "CutFlag " << hex << cutFlag << dec << endl;

  if(!passHotSpot(cutFlag)) return false;
  if(SYST==DGSys_NOM) n_pass_HotSpot+=_inc;

  if(!passBadJet (cutFlag)) return false;
  if(SYST==DGSys_NOM) n_pass_BadJet+=_inc;

  if(!passBadMuon(cutFlag)) return false;
  if(SYST==DGSys_NOM) n_pass_BadMuon+=_inc;

  if(!passCosmic (cutFlag)) return false;
  if(SYST==DGSys_NOM) n_pass_Cosmic+=_inc;

  return true;
}

/*--------------------------------------------------------------------------------*/
// FCAL bad C1-C8 run206248-207332
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passBadFCAL(const JetVector* jets, int run, bool isMC)
{
  bool inBadFCAL=hasJetInBadFCAL(*jets,run,isMC);

  if(!isMC){ //Veto data C1-C8
    if(run>=BAD_FCAL_RUN1 && 
       run <=BAD_FCAL_RUN2 && 
       inBadFCAL) return false;
  }
  if(SYST==DGSys_NOM) n_pass_BadFCAL+=_inc;
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
  if(SYST==DGSys_NOM) n_pass_signalLep[m_ET]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passTrigger(const LeptonVector* leptons)
{
  if(leptons->size() < 1) return false;
  if( m_trigObj->passDilTrig(*leptons, nt->evt()) ){
    if(SYST==DGSys_NOM) n_pass_trig[m_ET]+=_inc;
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
      
      bool isReal       = isPT(org,type,mcId,truthMatch,isEle,_hh->sampleName());
      bool isChargeFlip =  _l->isEle() ? ((Electron*) _l)->isChargeFlip : false; 
      
      if( !( isReal && !isChargeFlip) ) return false; //W/o including charge flip
      //if( !( isReal  || isChargeFlip) ) return false; //To include the charge flip
    }
  }
  if(SYST==DGSys_NOM) n_pass_truth[m_ET]+=_inc;
  return true;
}

/*--------------------------------------------------------------------------------*/
// To determine if have true OS event - ie neither e has qFlip
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::hasQFlip(const LeptonVector* leptons){
  if(leptons->size() < 1) return false;

  const Susy::Lepton* _l1 = leptons->at(0);
  const Susy::Lepton* _l2 = leptons->at(1);
  bool l1_isQFlip = _l1->isEle() ? ((Electron*) _l1)->isChargeFlip : false;
  bool l2_isQFlip = _l2->isEle() ? ((Electron*) _l2)->isChargeFlip : false;
  
  if(l1_isQFlip || l2_isQFlip) return true;

  return false;
}

/*--------------------------------------------------------------------------------*/
float Susy2LepAna::getQFlipProb(const LeptonVector* leptons, Met* met)
{
  if(leptons->size() < 1) return 1;
  Susy::Lepton* _l1 = leptons->at(0);
  Susy::Lepton* _l2 = leptons->at(1);
  
  TVector2 _new_met(met->lv().Px(),met->lv().Py());
  TLorentzVector _l1_tlv(*_l1);
  TLorentzVector _l2_tlv(*_l2);
  int _pdg1 = _l1->isEle() ? 11 : 13; _pdg1 *= _l1->q; //right convention???
  int _pdg2 = _l2->isEle() ? 11 : 13; _pdg2 *= _l2->q;
  
  int _sys = 0;
  if(SYST==DGSys_BKGMETHOD_UP) _sys=1;
  if(SYST==DGSys_BKGMETHOD_DN) _sys=-1;

  float cfP = m_chargeFlip->OS2SS(_pdg1, &_l1_tlv, 
				  _pdg2, &_l2_tlv, 
				  &_new_met, _sys);

  cfP*=  m_chargeFlip->overlapFrac().first; // QFLIP_RESCLALE;
  
  if(dbg()>5){
    cout << "qFlip - original " <<endl;
    cout << "\t l1 org_pt " << _l1->Pt() 
	 << "\t l2 org_pt " << _l2->Pt() 
	 << "\t org met   " << met->lv().Pt() 
	 << endl;
  }
  
  float _new_met_Et = sqrt(pow(_new_met.Px(),2) + pow(_new_met.Py(),2)); 
  met->Et = _new_met_Et;
  _l1->pt = _l1_tlv.Pt(); _l1->resetTLV();
  _l2->pt = _l2_tlv.Pt(); _l2->resetTLV();

  if(dbg()>5){
    cout << "qFlip - smear " <<endl;
    cout << "\t l1 new_pt " << _l1->Pt() << " " << _l1_tlv.Pt()
	 << "\t l2 new_pt " << _l2->Pt() << " " << _l2_tlv.Pt()
	 << "\t new met   " << met->lv().Pt() 
	 << endl;
  }

  return cfP;  
}

/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passFlavor(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;
  bool sameFlav = isSameFlav(leptons->at(0), leptons->at(1));
  if(m_selSF && !sameFlav) return false;
  if(SYST==DGSys_NOM) n_pass_flav[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passQQ(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;  
  float qq = leptons->at(0)->q * leptons->at(1)->q;
  if(m_selOS && qq >0 ) return false;
  if(m_selSS && qq <0 ) return false;
  if(m_selOS && SYST==DGSys_NOM) n_pass_os[m_ET][SR]+=_inc;
  if(m_selSS && SYST==DGSys_NOM) n_pass_ss[m_ET][SR]+=_inc;
  return true;
}

/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::isGenuineSS(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;  
  float qq = leptons->at(0)->q * leptons->at(1)->q;
  if(hasQFlip(leptons)) return false;
  if(qq <0 ) return false;
  return true;
}

/*--------------------------------------------------------------------------------*/
// Signal region cuts  - use all signal jets L25+B20+F30
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passJetVeto(const JetVector* jets)
{
  if(m_vetoJ && jets->size()>0) return false;

  if(SYST==DGSys_NOM) n_pass_Jveto[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passbJet(const JetVector* jets,float cutVal){
  int nBJet= numberOfCBJets(*jets);
  if(m_vetoB && nBJet>0) return false;
  if(m_selB && nBJet<=0) return false;
  if(SYST==DGSys_NOM) n_pass_bJet[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
float Susy2LepAna::getBTagSF(const Susy::Event*, const JetVector* jets)
{
  if(!nt->evt()->isMC) return 1;
  if(!USE_BWEIGHT)     return 1;
  //if(!( m_selB || m_vetoB || m_vetoJ || m_sel2J || m_topTag)) return 1;

  JetVector  valJets;
  valJets.clear();
  for(uint i=0; i<jets->size(); ++i){
    Jet* jet = jets->at(i);
    if(fabs(jet->Eta()) <= JET_ETA_CUT) valJets.push_back(jet);
  }
  
  if(valJets.size()==0) return 1;//safety.

  uint _sys = DGSys_NOM;
  if(SYST==DGSys_BJet_DN) _sys=BTag_BJet_DN; 
  if(SYST==DGSys_CJet_DN) _sys=BTag_CJet_DN; 
  if(SYST==DGSys_LJet_DN) _sys=BTag_LJet_DN; 
  if(SYST==DGSys_BJet_UP) _sys=BTag_BJet_UP; 
  if(SYST==DGSys_CJet_UP) _sys=BTag_CJet_UP; 
  if(SYST==DGSys_LJet_UP) _sys=BTag_LJet_UP; 

  return bTagSF(nt->evt(),valJets,USE_NOJVF_bSF, 
		"MV1", "0_122", MV1_85, (BTagSys) _sys);
  
}

/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passge2Jet(const JetVector* jets, int iSR)
{
  int nJet  = numberOfCLJets(*jets) + numberOfCBJets(*jets);
  int nFJet  = numberOfFJets(*jets);
  if(m_sel2J && nJet < 2) return false;
  if(iSR!=DIL_NTOP && m_sel2J && nFJet >0) return false;
  if(SYST==DGSys_NOM)  n_pass_2Jet[m_ET][SR]+=_inc;
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
    //hasz = hasZWindow(*leptons,Zlow, Zhigh); //has SFOS requirement
    if( !hasz) return false;
  }
  
  if(SYST==DGSys_NOM) n_pass_Z[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passMETRel(const Met *met, const LeptonVector* leptons, const JetVector* jets)
{
  metRel = getMetRel(met,*leptons,*jets);
  if(m_metRelMin>=0 &&  metRel < m_metRelMin ) return false;
  if(m_metRelMax>=0 &&  metRel > m_metRelMax ) return false;
  if(SYST==DGSys_NOM) n_pass_metRel[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passMET(const Met *met)
{
  float etmiss = met->lv().Pt();
  if(m_metMin>=0 &&  etmiss < m_metMin ) return false;
  if(m_metMax>=0 &&  etmiss > m_metMax ) return false;
  if(SYST==DGSys_NOM) n_pass_met[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passLeadLepPt(const LeptonVector* leptons)
{
  if( leptons->size() < 1 ) return false;
  float pt0 = leptons->at(0)->Pt();
  if( m_lepLeadPtMin >-1 && pt0< m_lepLeadPtMin) return false;
  if(SYST==DGSys_NOM) n_pass_leadLepPt[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passSumLepPt(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;
  float sumpt = leptons->at(0)->Pt() + leptons->at(1)->Pt();
  if( m_pTllMin >-1 && sumpt< m_pTllMin) return false;
  if(SYST==DGSys_NOM) n_pass_sumLepPt[m_ET][SR]+=_inc;
  return true;
}

/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passPtll(const LeptonVector* leptons)
{
  if( leptons->size() < 2 ) return false;
  TLorentzVector ll = (*leptons->at(0) + *leptons->at(1));
  if( m_pTllMin >-1 && ll.Pt() < m_pTllMin) return false;
  if( m_pTllMax >-1 && ll.Pt() > m_pTllMax) return false;
  if(SYST==DGSys_NOM) n_pass_pTll[m_ET][SR]+=_inc;
  return true;
}

/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passDPhiMetll(const LeptonVector* leptons, const Met* met)
{
  TLorentzVector metlv = met->lv();
  TLorentzVector ll = (*leptons->at(0) + *leptons->at(1));
  if( m_dPhiMetll>-1 && !passdPhi(metlv, ll, m_dPhiMetll) ) return false;
  if(SYST==DGSys_NOM) n_pass_dPhiMetll[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passDPhiMetl1(const LeptonVector* leptons, const Met* met)
{
  TLorentzVector metlv = met->lv();
  TLorentzVector ll = *leptons->at(1);
  if( m_dPhiMetl1>-1 && !passdPhi(metlv, ll, m_dPhiMetl1) ) return false;
  if(SYST==DGSys_NOM) n_pass_dPhiMetl1[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passTopTagger(const LeptonVector* leptons, const JetVector* jets, const Met* met)
{
  if(m_topTag && !passTopTag(*leptons,*jets,met)) return false;
  if(SYST==DGSys_NOM) n_pass_topTag[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passdPhi(TLorentzVector v0, TLorentzVector v1, float cut)
{
  return v0.DeltaPhi(v1) > cut;
}

/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passBlindData(bool isMC, int iSR, float metRel, float mt2){
  if( (!isMC && BLIND_DATA) || ( isMC && BLIND_MC)){
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
  if(SYST==DGSys_NOM) n_pass_mt2[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passMll20(const LeptonVector* leptons)
{
  float mll=Mll(leptons->at(0),leptons->at(1));
  if(mll<20) return false;
  return true;
}


/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passMll(const LeptonVector* leptons)
{
  float mll=Mll(leptons->at(0),leptons->at(1));
  if(m_mllMin>-1 && mll < m_mllMin) return false;
  if(SYST==DGSys_NOM) n_pass_mll[m_ET][SR]+=_inc;
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
  print_line("pass Mll20 ",n_pass_mll[0][j], n_pass_mll[1][j], n_pass_mll[2][j]);
  j= DIL_NWW3;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass Mll30 ",n_pass_pTll[0][j], n_pass_pTll[1][j], n_pass_pTll[2][j]);


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
  j= DIL_ZXCR5;
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
void Susy2LepAna::saveOriginal()
{
  //Make copy of Met
  new_met.clear();
  new_met = *m_met;

  //Make copy of Electrons/Muons/Leptons
  clearVectors();//for safety

  for(uint iEle=0; iEle<v_sigEle->size(); iEle++){
    Susy::Electron* _e = new Electron(*(v_sigEle->at(iEle)));
    v_save_sigEle.push_back(_e);
  }
  for(uint iMuo=0; iMuo<v_sigMu->size(); iMuo++){
    Susy::Muon* _m = new Muon(*(v_sigMu->at(iMuo)));
    v_save_sigMu.push_back(_m);
  }
  buildLeptons(v_save_sigLep, v_save_sigEle, v_save_sigMu);

  if(dbg()>10){
    cout << ">>> Original Lep" <<endl;
    for(uint iLep=0; iLep<v_save_sigLep.size(); iLep++){
      Susy::Lepton* _l = v_save_sigLep.at(iLep);
      cout << "\t ";  _l->print();
    }
  }
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::restoreOriginal(LeptonVector& leptons, const Met *met)
{
  new_met.clear();
  new_met = *met;

  //Not pretty - but essentially the ana change the v_sigEle !!!
  for(uint iEle=0; iEle<v_sigEle->size(); iEle++){
    Susy::Electron* _e = v_sigEle->at(iEle);
    _e->clear();
    _e->operator=(*(v_save_sigEle.at(iEle)));
  }
  
  leptons.clear();
  buildLeptons(leptons, *v_sigEle, *v_sigMu);

  if(dbg()>10){
    cout << "Restored Lep " << leptons.size() <<endl;
    for(uint iLep=0; iLep<leptons.size(); iLep++){
      Susy::Lepton* _l = leptons.at(iLep);
    cout << "\t "; _l->print();
    }
  }
}

/*--------------------------------------------------------------------------------*/
void Susy2LepAna::clearVectors()
{
  for(uint i=0; i<v_save_sigEle.size(); i++){
    delete v_save_sigEle[i];
  }
  for(uint i=0; i<v_save_sigMu.size(); i++){
    delete v_save_sigMu[i];
  }
  v_save_sigEle.clear();
  v_save_sigMu.clear();
  v_save_sigLep.clear();
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::fillHistograms(uint iSR,uint iSYS,
				 const LeptonVector* leptons, 
				 const JetVector* jets,
				 const Met* met,
				 float _ww)
{
  _hh->H1FILL(_hh->DG2L_pred[iSR][m_ET][iSYS],0.,_ww); 

  if(!nt->evt()->isMC){
    int run = nt->evt()->run;
    int ibin=0;
    if(!_hh->runBins.empty()){
      map<int,int>::const_iterator it = _hh->runBins.find(run);
      if(it !=  _hh->runBins.end()) ibin = it->second;
    }
    _hh->DG2L_Zcount[iSR][m_ET][iSYS]->AddBinContent(ibin); 
  }

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
      _hh->H1FILL(_hh->DG2L_ptl1[iSR][m_ET][iSYS],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etal1[iSR][m_ET][iSYS],_l->Eta(),_ww); 
      _hh->H1FILL(_hh->DG2L_d0Sl1[iSR][m_ET][iSYS],_l->d0/_l->errD0,_ww); 
      _hh->H1FILL(_hh->DG2L_z0sinthetal1[iSR][m_ET][iSYS],_l->z0SinTheta(),_ww); 
      _hh->H1FILL(_hh->DG2L_orgl1[iSR][m_ET][iSYS],lType,_ww); 
      q1=_l->q;
    }
    else if(ilep==1){
      _hh->H1FILL(_hh->DG2L_ptl2[iSR][m_ET][iSYS],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etal2[iSR][m_ET][iSYS],_l->Eta(),_ww); 
      _hh->H1FILL(_hh->DG2L_d0Sl2[iSR][m_ET][iSYS],_l->d0/_l->errD0,_ww); 
      _hh->H1FILL(_hh->DG2L_z0sinthetal2[iSR][m_ET][iSYS],_l->z0SinTheta(),_ww); 
      _hh->H1FILL(_hh->DG2L_orgl2[iSR][m_ET][iSYS],lType,_ww); 
      q2=_l->q;
    }
    
  }//leptons 
  
  if(q1*q2<0){//OS
    qqType = (q1<0 && q2>0) ? 0 : 1;  //-+/+-
  }
  else if(q1*q2>0){//SS
    qqType = (q1<0 && q2<0) ? 0 : 1;  //--/++
  }
  _hh->H1FILL(_hh->DG2L_qq[iSR][m_ET][iSYS],qqType,_ww); 
  
  float mWT = mT(_ll, met->lv());

  _hh->H1FILL(_hh->DG2L_mll[iSR][m_ET][iSYS],_ll.M(),_ww); 
  _hh->H1FILL(_hh->DG2L_pTll[iSR][m_ET][iSYS],_ll.Pt(),_ww); 
  _hh->H1FILL(_hh->DG2L_mWWT[iSR][m_ET][iSYS],mWT,_ww); 
  _hh->H1FILL(_hh->DG2L_dPhill[iSR][m_ET][iSYS],dPhill,_ww); 
  _hh->H1FILL(_hh->DG2L_JZBJet[iSR][m_ET][iSYS],JZBJet(v_sigJet,leptons),_ww); 
  _hh->H1FILL(_hh->DG2L_JZBEtmiss[iSR][m_ET][iSYS],JZBEtmiss(met,leptons),_ww); 
  _hh->H1FILL(_hh->DG2L_etmiss[iSR][m_ET][iSYS],met->lv().Pt(),_ww); 
  _hh->H1FILL(_hh->DG2L_metrel[iSR][m_ET][iSYS],metRel,_ww); 
  _hh->H1FILL(_hh->DG2L_metRefEle[iSR][m_ET][iSYS],met->refEle,_ww); 
  _hh->H1FILL(_hh->DG2L_metRefGam[iSR][m_ET][iSYS],met->refGamma,_ww); 
  _hh->H1FILL(_hh->DG2L_metRefMuo[iSR][m_ET][iSYS],met->refMuo,_ww); 
  _hh->H1FILL(_hh->DG2L_metRefJet[iSR][m_ET][iSYS],met->refJet,_ww); 
  _hh->H1FILL(_hh->DG2L_metRefSJet[iSR][m_ET][iSYS],met->softJet,_ww); 
  _hh->H1FILL(_hh->DG2L_metCellout[iSR][m_ET][iSYS],met->refCell,_ww); 
  _hh->H1FILL(_hh->DG2L_mt2[iSR][m_ET][iSYS],mT2,_ww); 

  float corrNpv = nt->evt()->nVtx;
  if(nt->evt()->isMC) corrNpv = GetNVertexBsCorrected(nt->evt()->nVtx);
  _hh->H1FILL(_hh->DG2L_npv[iSR][m_ET][iSYS],corrNpv,_ww); 
  _hh->H1FILL(_hh->DG2L_mu[iSR][m_ET][iSYS],nt->evt()->avgMu,_ww); 

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
      _hh->H1FILL(_hh->DG2L_ptbj[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etabj[iSR][m_ET][iSYS],_j->Eta(),_ww); 
    }

    if(ijet==0){
      _hh->H1FILL(_hh->DG2L_ptj1[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etaj1[iSR][m_ET][iSYS],_j->Eta(),_ww); 
    }
    if(ijet==1){
      _hh->H1FILL(_hh->DG2L_ptj2[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etaj2[iSR][m_ET][iSYS],_j->Eta(),_ww); 
    }
    if(ijet==2){
      _hh->H1FILL(_hh->DG2L_ptj3[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etaj3[iSR][m_ET][iSYS],_j->Eta(),_ww); 
    }
    if(ijet==3){
      _hh->H1FILL(_hh->DG2L_ptj4[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etaj4[iSR][m_ET][iSYS],_j->Eta(),_ww); 
    }
  }
  _hh->H1FILL(_hh->DG2L_nJets[iSR][m_ET][iSYS],nSigJet,_ww); 
  _hh->H1FILL(_hh->DG2L_nCJets[iSR][m_ET][iSYS],nSigCJet,_ww); 
  _hh->H1FILL(_hh->DG2L_nFJets[iSR][m_ET][iSYS],nSigFJet,_ww); 
  _hh->H1FILL(_hh->DG2L_nBJets[iSR][m_ET][iSYS],nBJets,_ww); 


  //Reco mW from jj in Z events w/o btag
  if(hasZWindow(*leptons,MZ-10, MZ+10) && nSigJet==2 
     &&nBJets==0){
    TLorentzVector _jj = (*jets->at(0)) + (*jets->at(1));
    _hh->H1FILL(_hh->DG2L_mjj[iSR][m_ET][iSYS],_jj.M(),_ww); 
  }
}


/*--------------------------------------------------------------------------------*/
void Susy2LepAna::dumpEvent()
{
  cout << ">>> ";  nt->evt()->print();
  
  cout << "Baseline objects" << endl;
  for(uint iEl=0; iEl < v_baseEle->size(); iEl++){
    cout << "  ";
    v_baseEle->at(iEl)->print();
  }
  for(uint iMu=0; iMu < v_baseMu->size(); iMu++){
    cout << "  ";
    v_baseMu->at(iMu)->print();
  }
  for(uint iJ=0; iJ < v_baseJet->size(); iJ++){
    cout << "  ";
    v_baseJet->at(iJ)->print();
  }

  cout << "Signal objects" << endl;
  for(uint iEl=0; iEl < v_sigEle->size(); iEl++){
    cout << "  ";
    v_sigEle->at(iEl)->print();
  }
  for(uint iMu=0; iMu < v_sigMu->size(); iMu++){
    cout << "  ";
    v_sigMu->at(iMu)->print();
  }
  for(uint iJ=0; iJ < v_sigJet->size(); iJ++){
    cout << "  ";
    v_sigJet->at(iJ)->print();
  }
}



/*--------------------------------------------------------------------------------*/
// Initialize HistFitterTree
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::initializeHistFitterTree()
{
  // Set the writeHFT flag
  m_writeHFT=true;
  cout << " ===> Initialising HistFitterTree Outputs " << endl;

  TString systName = "" ;
  
  if(nt->evt()->isMC){
  // Read MC ID, only run over one unique DS while using this option!!!
    TString mcId = "";
    mcId.Form("%i",nt->evt()->mcChannel);
    for(uint i=_sys1; i <=_sys2; i++) {
      systName.Form("%s",DG2LSystNames[i].c_str());
      m_histFitterTrees[i] = new HistFitterTree(mcId,systName);
      cout << "Creating histFitterTree " << mcId << " " << systName << endl;
      HFTName = string(mcId.Data());
    }
  }
  else{
    if(m_method == FLEP){ //DD Fake lepton
      TString ds(string(_hh->sampleName()+"_FAKE"));
      //Nominal fake tree
      systName.Form("%s",DG2LSystNames[DGSys_NOM].c_str());
      m_histFitterTrees[DGSys_NOM] = new HistFitterTree(ds,systName);
      HFTName = string(ds.Data());
      cout << "Creating histFitterTree " << ds << " " << systName << endl;

      for(uint i=DGSys_FAKE_EL_RE_UP; i <= DGSys_FAKE_MU_FR_DN ; i++) {
	systName.Form("%s",DG2LSystNames[i].c_str());
	m_histFitterTrees[i] = new HistFitterTree(ds,systName);
	cout << "Creating histFitterTree " << i << " " << ds << " " << systName << endl;
      }
    }
    else{ //Data - 1 tree
      TString ds(_hh->sampleName());
      //systName.Form("%s",DG2LSystNames[DGSys_NOM].c_str());
      systName.Form("%s","central");
      m_histFitterTrees[DGSys_NOM] = new HistFitterTree(ds,systName);
      cout << "Creating histFitterTree " << ds << " " << systName << endl;
      HFTName = string(ds.Data());
    }

  }
  
}

/*--------------------------------------------------------------------------------*/
// Write into HistFitter Tree
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::writeIntoHistFitterTree( const LeptonVector* leptons, 
					   const LeptonVector* baseLeptons, 
					   const JetVector* signalJets, 
					   const JetVector* baseJets,
					   const Met* met)
{
  // Find which event type
  DiLepEvtType evtType = getDiLepEvtType(*baseLeptons);
  
  // Form the Z-candidate
  TLorentzVector Zcandidate = *leptons->at(0);
  Zcandidate += *leptons->at(1);
  
  // Get HistFitterTree weight
  // Includes: 1) pile-up
  //           2) lepton SF
  //           3) trigger
  //           4) b-tagging
  
  float histFitWeight = 1;
  float totalWeight=1;

  if(nt->evt()->isMC){
    histFitWeight  = nt->evt()->w * nt->evt()->wPileup
                     *leptons->at(0)->effSF*leptons->at(1)->effSF;
    if(USE_DGWEIGHT){
      uint iiSys = DGSys_NOM;
      if(SYST==DGSys_TRIGSF_EL_UP) iiSys=NtSys_TRIGSF_EL_UP;
      if(SYST==DGSys_TRIGSF_EL_DN) iiSys=NtSys_TRIGSF_EL_DN;
      if(SYST==DGSys_TRIGSF_MU_UP) iiSys=NtSys_TRIGSF_MU_UP;
      if(SYST==DGSys_TRIGSF_MU_DN) iiSys=NtSys_TRIGSF_MU_DN;
      histFitWeight *= m_trigObj->getTriggerWeight(*leptons, nt->evt()->isMC, (SusyNtSys) iiSys);
    }
    histFitWeight *= getBTagSF(nt->evt(),baseJets);

    float _sumW = nt->evt()->sumw;
    //Hack for broken sumW in n0107 !!!
    if(nt->evt()->mcChannel==147771) _sumW = 2.35447863214e+13;
    if(nt->evt()->mcChannel==147772) _sumW = 1.23966712709e+13;
    totalWeight   = histFitWeight * nt->evt()->xsec * LUMI_A_E / _sumW;
  }
  else{
    if(m_useLooseLep && ( SYST==DGSys_NOM ||
			  (SYST>=DGSys_FAKE_EL_RE_UP && SYST<=DGSys_FAKE_MU_FR_DN) ) ){
      
      bool  _isOS   = leptons->at(0)->q * leptons->at(1)->q < 0 ? true:false;
      bool  _isEE   =  evtType==ET_ee ? true:false;
      bool  _isEM   = (evtType==ET_em || evtType==ET_me) ? true:false;
      bool  _isMM   = evtType==ET_mm ? true:false;
      bool _topTag  = passTopTag(*leptons,*signalJets,met);
      int   _nC25   = numberOfCLJets(*signalJets);
      int   _nB20   = numberOfCBJets(*signalJets);
      int   _nF30   = numberOfFJets(*signalJets);
      float _ptl1   = leptons->at(0)->Pt();
      float _ptl2   = leptons->at(1)->Pt();
      float _ZPt    = Zcandidate.Pt();
      float _metET  = met->lv().Pt();
      float _metRel = getMetRel(met,*leptons,*signalJets);
      float _Mll    = Mll(leptons->at(0), leptons->at(1));
      float _MT2    = getMT2(*leptons, met);

      // NEED TO DETERMINE WHICH SR to get the appropriate weight
      int iSR = findSRCR(_isOS, _isEE, _isMM, _isEM, _topTag, 
			 _nC25, _nB20, _nF30, 
			 _ptl1, _ptl2, _ZPt, 
			 _metET, _metRel, _Mll, _MT2);
      histFitWeight = getFakeWeight(leptons,nt->evt()->nVtx,nt->evt()->isMC,iSR,_metRel);
      totalWeight = histFitWeight;
    }
  }

  // Fill the tree
  // Store everything in MeV
  m_histFitterTrees[SYST]->Fill2LTreeEventInfo(leptons->at(0)->Pt()*1000.,
					       leptons->at(1)->Pt()*1000.,
					       met->lv().Pt()*1000.,
					       numberOfCLJets(*signalJets),
					       numberOfCBJets(*signalJets),
					       numberOfFJets(*signalJets),
					       getMetRel(met,*leptons,*signalJets)*1000.,
					       Mll(leptons->at(0), leptons->at(1))*1000.,
					       getMT2(*leptons, met)*1000.,
					       Zcandidate.Pt()*1000.,
					       leptons->at(0)->q * leptons->at(1)->q < 0 ? true:false,                                       
					       passTopTag(*leptons,*signalJets,met),
					       histFitWeight,
					       (evtType==ET_em || evtType==ET_me) ? true:false,
					       evtType==ET_mm ? true:false,
					       evtType==ET_ee ? true:false,
					       totalWeight
					       );
  
  // Write the tree
  m_histFitterTrees[SYST]->WriteTree();
  
}

/*--------------------------------------------------------------------------------*/
// Write into HistFitter Tree
/*--------------------------------------------------------------------------------*/
int Susy2LepAna::findSRCR(bool isOS, bool isEE, bool isMM, bool isEM, bool topTag,
			  int nC25, int nB20, int nF30, 
			  float ptl1, float ptl2, float Zpt,
			  float met, float metrel, float mll, float mt2)
{
  int iSR=DIL_NSR;
  bool _inZ = (fabs(mll-MZ)<10) ? true : false;

  //
  //Signal regions
  //
  //Order matters 
  if(isOS && !_inZ && metrel>40 && 
	  (nC25+nB20+nF30)==0 && mt2>110)                          iSR=DIL_SRmT2b;
  else if(isOS && !_inZ && metrel>40 && 
	  (nC25+nB20+nF30)==0 && mt2>90)                           iSR=DIL_SRmT2;
  else if(isOS && !_inZ && metrel>100 && (nC25+nB20+nF30)==0 )     iSR=DIL_SRjveto;
  else if(!isOS && metrel>100 && (nC25+nB20+nF30)==0)              iSR=DIL_SRSSjveto;
  else if(isOS && !isEM && !_inZ && metrel>50 &&
	  !topTag && nC25>=2 && nB20==0 && nF30==0)                iSR=DIL_SR2jets;

  //
  //Control regions
  //
  //WW
  else if(isOS && metrel>70 && metrel<100 && 
	  (nC25+nB20+nF30)==0 && mll>30)                           iSR=DIL_NWW2;
  else if(isOS && metrel>70 && metrel<100 &&
	  (nC25+nB20+nF30)==0)                                     iSR=DIL_NWW1;
  //ZX
  else if(isOS && _inZ)                                            iSR=DIL_CRZ;
  else if(isOS && _inZ && metrel>100 && (nC25+nB20+nF30)==0)       iSR=DIL_ZXCR1;
  else if(isOS && _inZ && metrel>40 && 
	  (nC25+nB20+nF30)==0 && mT2>90)                           iSR=DIL_ZXCR4;
  else if(isOS && _inZ && metrel>50 && 
	  !topTag && nC25>=2 && nB20==0 && nF30==0)                iSR=DIL_ZXCR3;
  else if(isOS && !_inZ && metrel>40 
	  && (nC25+nB20+nF30)>=2 && nB20>=1)                       iSR=DIL_NTOP;
  
  else if(isOS)                                                    iSR=DIL_CR2LepOS;
  else if(!isOS)                                                   iSR=DIL_CR2LepSS;


  return iSR;
}
