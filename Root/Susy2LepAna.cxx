#include <iomanip>
#include "TCanvas.h"
#include "SusyWeakProdAna/Susy2LepAna.h"
#include "SusyWeakProdAna/PhysicsTools.h"

#include "SusyWeakProdAna/SusyAnaCommon.h"

using namespace std;
using namespace Susy;

//const char* const DIL_FLAV[] = {"EE", "MM", "EM"};
//const char* const DIL_QQ[] = {"OS", "SS"};

/*--------------------------------------------------------------------------------*/
// Susy2LepAna Constructor
/*--------------------------------------------------------------------------------*/
Susy2LepAna::Susy2LepAna(SusyHistos* _histos):
  SusyNtTools(),
  _hh(_histos),
  m_susyXsec(0),
  m_dbg(0),
  m_useLooseLep(false),
  m_writeHFT(false),
  m_writeToyNt(false),
  m_nLepMin(2),
  m_nLepMax(2),
  m_cutNBaseLep(true),
  m_selOS(false),
  m_selSS(false),
  m_selSF    (false),
  m_selOF    (false),
  m_vetoSF   (false),
  m_selZ       (false),
  m_vetoZ      (false),
  m_selB       (false),
  m_vetoB      (false),
  m_vetoF      (false),
  m_vetoJ      (false),
  m_minC20     ( -1  ),
  m_minCJet    ( -1  ),
  m_metMin     ( -1  ),
  m_metMax     ( -1  ),
  m_metRelMin  ( -1  ),
  m_metRelMax  ( -1  ),
  m_topTag     (false),
  m_mt2Min     ( -1  ),
  m_mt2Max     ( -1  ),
  m_lepLeadPtMin( -1  ),
  m_pTl0Min     ( -1  ),
  m_pTl1Min     ( -1  ),
  m_pTllMin     ( -1  ),
  m_pTllMax     ( -1  ),
  m_lowMll      ( -1  ),
  m_highMll     ( -1  ),
  m_mllIn       (false),
  m_dPhillMax   ( -1  ),
  m_lowMjj      ( -1  ),
  m_highMjj     ( -1  ),
  m_lowMTWW     ( -1  ),
  m_highMTWW    ( -1  ),
  m_pTj0Min     ( -1  ),
  m_pTj1Min     ( -1  ),
  m_dPhiMetll   ( -1  ),
  m_dPhiMetl1   ( -1  ),

  m_ET(ET_Unknown)
{
  reset();
  resetCounter();

  _random = new TRandom3(2012300958);

  bool useReweightUtils=false;//true;
  m_trigObj = new DilTrigLogic(LUMW,useReweightUtils);  
  if(USE_MCTRIG && !USE_DGWEIGHT){
    m_trigObj->useMCTrigger();
    cout << " MESSAGE: Using MC trigger decision " << endl;
  }

  // Configure using fake rates file
  // Currently rates are provided as function of pT only, so only use PT as second option
  string _fakeInput  =  string(getenv("WORKAREA")) + 
    "/SusyMatrixMethod/data/pass6_Apr2_2013.root"; //Spring !
  //    "/SusyMatrixMethod/data/pass3_Mar3_2013.root"; //Spring !

  cout << "Loading fake MM " << _fakeInput << endl;
  m_matrix_method.configure(_fakeInput, SusyMatrixMethod::PT);


  //Configure ChargeFlip tool
  string chargeFlipInput =  string(getenv("WORKAREA")) + 
    "/ChargeFlip/data/chargeFlip.root";
  m_chargeFlip = new chargeFlip(chargeFlipInput);
  
  //Open signal cross section file
  string _sSigFile = string(getenv("WORKAREA")) +
    "/SusyWeakProdAna/data/" + "Simplified_CrossSections.txt";
  sigXsfile.open(_sSigFile.c_str());


  // Get the handle on the SleptonXsecReader
  string sleptonXSecFile = string(getenv("ROOTCOREDIR")) +
    "/data/SusyNtuple/DLiSlep_SignalUncertainties_All.root" ;
  m_SleptonXSecReader    = new SleptonXsecReader(sleptonXSecFile.c_str(),"SignalUncertainties"); 
  
  string xsecFileName  = gSystem->ExpandPathName("$ROOTCOREDIR/data/SusyWeakProdAna/susy_crosssections_8TeV.txt");
  m_susyXsec = new SUSY::CrossSectionDB(xsecFileName);


  setAnaType(Ana_2Lep);


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
  if(FILL_TOYNT && !m_writeToyNt && isys==DGSys_NOM) initializeToyNt();

  //Call this here, since have multiple instances of SusyNtTools
  setAnaType(Ana_2Lep);

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

  cout << "read in:            " << n_readin        << endl;
  cout << "pass GRL:           " << n_pass_GRL      << endl;
  cout << "pass LarErr:        " << n_pass_LarErr   << endl;
  cout << "pass TileErr:       " << n_pass_TileErr  << endl;
  cout << "pass TTCVeto:       " << n_pass_TTCVeto  << endl;
  cout << "pass Good Vtx       " << n_pass_GoodVtx  << endl;
  cout << "pass TileTrip:      " << n_pass_TileTrip << endl;
  cout << "pass HotSpot:       " << n_pass_HotSpot << endl;
  cout << "pass BadJet:        " << n_pass_BadJet  << endl;
  cout << "pass BadMu:         " << n_pass_BadMuon << endl;
  cout << "pass Cosmic:        " << n_pass_Cosmic  << endl;
  cout << "pass DeadRegion:    " << n_pass_DeadRegion << endl;
  cout << "pass atleast 2 base " << n_pass_atleast2BaseLep << endl;
  cout << "pass exactly 2 base " << n_pass_exactly2BaseLep << endl;
  cout << "pass mll20          " << n_pass_mll20   << endl;
  
  string v_ET[ET_N] = {"ee","mm","em"};
  cout << "Channels        " << v_ET[0] << "\t " << v_ET[1] << "\t " << v_ET[2] <<endl;
  print_line("pass category ",n_pass_dil[0], n_pass_dil[1], n_pass_dil[2]);
  print_line("pass nLep     ",n_pass_signalLep[0], n_pass_signalLep[1], n_pass_signalLep[2]);
  print_line("pass trig     ",n_pass_trig[0],n_pass_trig[1],n_pass_trig[2]);
  print_line("pass truth    ",n_pass_truth[0],n_pass_truth[1],n_pass_truth[2]);
  print_SROSjveto();
  print_SRmT2();
  print_SR2jets();
  print_SRZjets();
  print_SRSSjets();
  print_SRWW();
  print_CRZ();
  print_ZXCR();
  print_TOPCR();
  print_WWCR();
  print_VRSS();


  cout << "CHECK INTEGRAL " << _tmp << endl;

  if(DUMP_RUNEVT){
    evtDump.close ();
  }
  
  if(m_writeHFT){ //Saving HistFitterTree - yes that's done in delete contructor!
    for(uint i=_sys1; i <= _sys2; i++) {
      if( m_histFitterTrees[i] && validSystForHFT(i)){
	float sumw = nt->evt()->sumw;
	m_histFitterTrees[i]->setSumOfMcWeights(sumw); 
	delete m_histFitterTrees[i];
      }
    }
    moveHFTOutput();
  }
  
  if(m_writeToyNt){
    m_toyNt->setSumOfMcWeights(nt->evt()->sumw); 
    m_toyNt->SaveTree();

    string dir =  string(getenv("HISTOANA")) + "/SusyAna/ToyNtOutputs";
    gSystem->mkdir(dir.c_str(),kTRUE);

    string cmd = "mv " + m_toyNt->getFilename() + " " + dir;
    std::cout << "Moving ToyNt " << cmd << std::endl;
    gSystem->Exec(cmd.c_str());

    delete m_toyNt;
  }

  clearVectors();
  sigXsfile.close();  
}
/*--------------------------------------------------------------------------------*/
// Move output of this HFT to a decent location
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::moveHFTOutput()
{
  string dir =  string(getenv("HISTOANA")) + "/SusyAna/HFTOutputs";
  gSystem->mkdir(dir.c_str(),kTRUE);
  
  if(nt->evt()->isMC){
    TString mcId = "";
    TString systName = "" ;
    mcId.Form("%i",nt->evt()->mcChannel);
    for(uint i=_sys1; i <=_sys2; i++) {
      if(! validSystForHFT(i)) continue;
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
      TString ds(string(_hh->sampleName()+"_FAKE"));
      TString systName = "" ;
      for(uint i=_sys1; i <=_sys2; i++) {
	if(! validSystForHFT(i)) continue;
	systName.Form("%s",DG2LSystNames[i].c_str());
	TString fName = systName+"_"+ds+".root";
	string cmd = "mv " + string(fName.Data()) + " " + dir;
	std::cout << "Moving DATA FAKE HFT file " << cmd << std::endl;
	gSystem->Exec(cmd.c_str());
      }
    }
    else{
      list = "HFTlist_DATA_" + HFTName;
      cmd = "ls -1 central_"+ HFTName + ".root >" +list;
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
  
}

/*--------------------------------------------------------------------------------*/
// Reset 
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::resetCounter()
{
 
  n_readin       = 0;
  n_pass_GRL     = 0;
  n_pass_LarErr  = 0;
  n_pass_TileErr = 0;
  n_pass_TTCVeto = 0;
  n_pass_GoodVtx = 0;
  n_pass_TileTrip= 0;
  n_pass_HotSpot = 0;
  n_pass_BadJet  = 0;
  n_pass_BadMuon = 0;
  n_pass_Cosmic  = 0;
  n_pass_BadFCAL = 0;
  n_pass_DeadRegion  = 0;
  n_pass_atleast2BaseLep = 0;
  n_pass_exactly2BaseLep = 0;
  n_pass_mll20   = 0;

  // The rest are channel specific.
  for(int i=0; i<ET_N; ++i){
    n_pass_signalLep[i]   = 0;
    n_pass_dil[i]    = 0;
    n_pass_trig[i]    = 0;
    n_pass_truth[i]    = 0;

    for(int j=0; j<DIL_NSR; ++j){
      n_pass_ss[i][j]        = 0;
      n_pass_os[i][j]        = 0;
      n_pass_flav[i][j]      = 0;
      n_pass_Z[i][j]         = 0;
      n_pass_FullJveto[i][j] = 0;
      n_pass_FJet[i][j]      = 0;
      n_pass_BJet[i][j]      = 0;
      n_pass_CJet[i][j]      = 0;
      n_pass_JetPt[i][j]     = 0;
      n_pass_mjj[i][j]       = 0;
      n_pass_leadLepPt[i][j] = 0;
      n_pass_mll[i][j]       = 0;
      n_pass_pTll[i][j]      = 0;
      n_pass_dPhill[i][j]    = 0;
      n_pass_mWWT[i][j]      = 0;
      n_pass_topTag[i][j]    = 0;
      n_pass_metRel[i][j]    = 0;
      n_pass_mt2[i][j]       = 0;

      n_pass_met[i][j]       = 0;
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
void Susy2LepAna::setSelection(std::string s, DiLepEvtType dilType)
{
  m_sel = s;

  m_nLepMin = 2;
  m_nLepMax = 2;
  m_cutNBaseLep = true;
  m_selOS    = false;
  m_selSS    = false;
  m_selSF    = false;
  m_selOF    = false;
  m_vetoSF   = false;
  m_selZ     = false;
  m_vetoZ    = false;
  m_selB     = false;
  m_vetoB    = false;
  m_vetoF    = false;
  m_vetoJ    = false;
  m_minC20   =    -1;
  m_minCJet  =    -1;
  m_metMin     =  -1;
  m_metMax     =  -1;
  m_metRelMin  =  -1;
  m_metRelMax  =  -1;
  m_topTag     = false;
  m_mt2Min     =  -1;
  m_mt2Max     =  -1;
  m_lepLeadPtMin = -1;
  m_pTllMin = -1;
  m_pTllMax = -1;
  m_lowMll    =  -1;
  m_highMll   =  -1;
  m_mllIn     = false; 
  m_lowMjj    =  -1;
  m_highMjj   =  -1;
  m_lowMTWW   =  -1;
  m_highMTWW  =  -1;
  m_pTj0Min   =  -1;
  m_pTj1Min   =  -1;
  m_pTl0Min   =  -1;
  m_pTl1Min   =  -1;
  m_dPhillMax =  -1;
  m_dPhiMetll =  -1;
  m_dPhiMetl1 =  -1;


  //----------------------------//
  // Signal Regions
  //----------------------------//
  if(m_sel == "SROSjveto"){
    m_selOS = true;
    m_vetoZ = true;
    m_vetoJ = true;
    m_metRelMin=100;
    m_mt2Max=90;
  }
  else if(m_sel == "SRmT2a"){
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
  }  
  else if(m_sel == "SR2jets"){
    m_selOS = true;
    m_vetoZ = true;
    m_vetoF = true;
    m_vetoB = true;
    m_minC20 = 2;   //C20
    m_topTag = true;
    m_metRelMin=50;
  }
  else if(m_sel == "SRZjets"){
    m_selOS = true;
    m_selSF = true;
    m_selZ  = true;
    m_vetoF = true;
    m_vetoB = true;
    m_minC20 = 2;
    m_pTj0Min = 50;
    m_pTj1Min = 50;
    m_lowMjj  = 50;
    m_highMjj = 100;
    m_topTag = true;
    m_metRelMin=50;
    m_mt2Min  = 70;
  }
  else if(m_sel == "SRSSjets"){
    m_selSS = true;
    m_vetoB = true;
    m_vetoF = true;
    m_minC20 = 1;
    if(dilType==ET_ee){
      m_mllIn     = true;
      m_lowMll    = MZ-10;
      m_highMll   = MZ+10;
      m_highMTWW  = 70; 
      m_metRelMin = 70;
    }
    else if(dilType==ET_mm || dilType==ET_em){
      m_highMTWW  = 50; 
      m_metRelMin = 50;
      if(dilType==ET_mm && BLIND_SSMUON){
	m_mllIn     = true;
	m_lowMll  = 90;
	m_highMll = 120;
      }
    }
  }  
  else if(m_sel == "SRWWa"){
    m_selOS = true;
    m_selOF = true;
    if(dilType==ET_em){
      m_vetoJ     = true;
      m_vetoB     = true;
      m_vetoF     = true;
      m_pTl0Min   = 35;
      m_pTl1Min   = 20;
      m_metRelMin = 70;
      m_pTllMin   = 70;
      m_highMll   = 80;
      m_dPhillMax = 1.8;
    }
  }
  else if(m_sel == "SRWWb"){
    m_selOS = true;
    m_selOF = true;
    if(dilType==ET_em){
      m_vetoJ    = true;
      m_vetoB    = true;
      m_vetoF    = true;
      m_pTl0Min  = 35;
      m_pTl1Min  = 20;
      m_mt2Min   = 90;
      m_pTllMax  = 170;
      m_highMll  = 130;
      m_dPhillMax = 1.8;
    }
  }
  else if(m_sel == "SRWWc"){
    m_selOS = true;
    m_selOF = true;
    if(dilType==ET_em){
      m_vetoJ    = true;
      m_vetoB    = true;
      m_vetoF    = true;
      m_pTl0Min  = 35;
      m_pTl1Min  = 20;
      m_mt2Min   = 100;
      m_pTllMax  = 190;
      m_dPhillMax = 1.8;
    }
  }
  //----------------------------//
  // Control regions
  //----------------------------//
  else if(m_sel == "ZXCRjveto"){
    m_selOS = true;
    m_selZ  = true;
    m_vetoJ = true;
    m_metRelMin=100;
  }
  else if(m_sel == "ZXCR2jets"){
    m_selOS     = true;
    m_selZ      = true;
    m_vetoB     = true;
    m_vetoF     = true;
    m_minC20    = 2;
    m_topTag    = true;
    m_metRelMin = 50;
    m_mt2Max    = 70;
  }
 else if(m_sel == "ZXCRpremT2"){
    m_selOS = true;
    m_selZ  = true;
    m_vetoJ = true;
    m_metRelMin=40;
 }
  else if(m_sel == "ZXCRmT2a"){
    m_selOS = true;
    m_selZ  = true;
    m_vetoJ = true;
    m_metRelMin=40;
    m_mt2Min=90;
  }
  else if(m_sel == "ZXCRmT2b"){
    m_selOS = true;
    m_selZ  = true;
    m_vetoJ = true;
    m_metRelMin=40;
    m_mt2Min=110;
  }
  else if(m_sel == "ZXCRWW"){
    m_selOS = true;
    m_selZ  = true;
    m_vetoJ = true;
    m_metRelMin=70;
    m_metRelMax=100;
    m_mt2Max=90;
  }
  else if(m_sel == "ZXCRWW2"){
    m_selOS = true;
    m_selZ  = true;
    m_vetoJ = true;
    m_metRelMin=70;
    m_metRelMax=100;
    m_mt2Min=50;
    m_mt2Max=90;
  }
  else if(m_sel == "CRTOP"){
    m_selOS = true;
    m_vetoZ =true;
    //m_vetoF = true;  //Don't check fwd jets
    m_selB=true;
    m_minCJet=2;
    m_metRelMin=40;
  }
  else if(m_sel == "CRTOPWWa"){
    m_selOS  = true;    
    m_selOF = true;
    if(dilType==ET_em){
      //m_vetoF  = true;
      m_selB   = true;
      m_minCJet=1;
      m_pTl0Min = 35;
      m_pTl1Min = 20;
      m_metRelMin=70;
      m_pTllMin  = 70;
      m_highMll  = 80;
      m_dPhillMax = 1.8;
    }
  }
  else if(m_sel == "CRTOPWWb"){///<<<<
    m_selOS  = true;
    m_selOF = true;
    if(dilType==ET_em){
      //m_vetoF  = true;
      m_selB = true;
      m_minCJet=1;
      m_pTl0Min = 35;
      m_pTl1Min = 20;
      m_mt2Min= 90;
      m_pTllMax  = 170;
      m_highMll  = 130;
      m_dPhillMax = 1.8;
    }
  }
  else if(m_sel == "CRTOPWWc"){
    m_selOS  = true;
    m_selOF = true;
    if(dilType==ET_em){
      //m_vetoF  = true;
      m_selB = true;
      m_minCJet=1;
      m_pTl0Min = 35;
      m_pTl1Min = 20;
      m_mt2Min= 100;
      m_pTllMax  = 190;
      m_dPhillMax = 1.8;
    }
  }
  else if(m_sel == "CRWW"){//metrel 70-100, MT2<90
    m_selOS = true;
    //m_selSF = true;
    m_vetoZ = true;
    m_vetoJ = true;
    m_metRelMin = 70;
    m_metRelMax = 100;
    m_mt2Max    = 90;
  }
  else if(m_sel == "CRWW2"){//metrel 70-100, 50<mT2<90
    m_selOS = true;
    //m_selSF = true;
    m_vetoZ = true;
    m_vetoJ = true;
    m_metRelMin = 70;
    m_metRelMax = 100;
    m_mt2Min    = 50;
    m_mt2Max    = 90;
  }
  else if(m_sel == "CRWW3"){//metrel 70-100, MT2<90 & pTll>60
    m_selOS = true;
    //m_selSF = true;
    m_vetoZ = true;
    m_vetoJ = true;
    m_metRelMin = 70;
    m_metRelMax = 100;
    m_mt2Max    = 90;
    m_pTllMin   = 60;
  }
  else if(m_sel == "CRWW4"){//metrel 70-100, 50<mT2<90 & pTll>60
    m_selOS = true;
    //m_selSF = true;
    m_vetoZ = true;
    m_vetoJ = true;
    m_metRelMin = 70;
    m_metRelMax = 100;
    m_mt2Min    = 50;
    m_mt2Max    = 90;
    m_pTllMin   = 60;
  }
  else if(m_sel == "CRWW5"){ //preSRmT2 + 50<mT2<90 - For EM only
    m_selOS = true;
    //m_selSF = true;
    m_vetoZ = true;
    m_vetoJ = true;
    m_metRelMin = 40;
    m_mt2Min    = 50;
    m_mt2Max    = 90;
  }
  else if(m_sel == "CRWW6"){ //preSRmT2 + 50<mT2<90 - pTll>60 - For EE/MM only
    m_selOS = true;
    //m_selSF = true;
    m_vetoZ = true;
    m_vetoJ = true;
    m_metRelMin = 40;
    m_mt2Min    = 50;
    m_mt2Max    = 90;
    m_pTllMin   = 60;
  }
  else if(m_sel == "CRWWa"){
    m_selOS    = true;
    m_selOF = true;
    if(dilType==ET_em){
      m_vetoJ = true;
      m_vetoB = true;
      m_vetoF = true;
      m_pTl0Min = 35;
      m_pTl1Min = 20;
      m_metRelMax = 70;
      m_dPhillMax = 1.8;
    }
  }
  else if(m_sel == "CRWWb"){
    m_selOS    = true;
    m_selOF = true;
    if(dilType==ET_em){
      m_vetoJ = true;
      m_vetoB = true;
      m_vetoF = true;
      m_pTl0Min = 35;
      m_pTl1Min = 20;
      m_mt2Max = 90;
      m_dPhillMax = 1.8;
    }
  }
  else if(m_sel == "CRWWc"){ //Not used 
    m_selOS    = true;     
    m_selOF = true;
    if(dilType==ET_em){
      m_vetoJ     = true;
      m_vetoB     = true;
      m_vetoF     = true;
      m_pTl0Min   = 35;
      m_pTl1Min   = 20;
      m_pTllMax   = 190;
      m_dPhillMax = 1.8;
      m_mt2Max    = 90;
    }
  }
  
  //----------------------------//
  // SS Validation regions
  //----------------------------//
  if(m_sel == "VRSS"){
    m_selSS = true;
    m_vetoB = true;
    m_vetoF = true;
    m_minC20 = 1;
    if(dilType==ET_ee){
      m_mllIn     = true;
      m_lowMll    = MZ-10;
      m_highMll   = MZ+10;
      m_metRelMin = 50;
      m_lowMTWW   = 70; //reversed
    }
    else if(dilType==ET_mm || dilType==ET_em){
      m_metRelMin = 40;
      m_lowMTWW   = 50; //reversed
      if(dilType==ET_mm && BLIND_SSMUON){
	m_mllIn     = true;
	m_lowMll  = 90;
	m_highMll = 120;
      }
    }
  }
  if(m_sel == "VRSSbtag"){
    m_selSS  = true;
    m_selB   = true;  //reverse
    m_vetoF  = true;
    m_minC20 = 0;
    if(dilType==ET_ee){
      m_mllIn     = true;
      m_metRelMin = 50;
    }
    else if(dilType==ET_mm || dilType==ET_em){
      m_metRelMin = 50;
      if(dilType==ET_mm && BLIND_SSMUON){
	m_mllIn     = true;
	m_lowMll  = 90;
	m_highMll = 120;
      }
    }
  }
  //----------------------------//
  // Loose Selections
  //----------------------------//
  else if(m_sel == "CRZ"){
    m_selOS = true;
    m_selZ  = true;
  }
  else if(m_sel == "CRZjveto"){
    m_selOS = true;
    m_selZ  = true;
    m_vetoJ = true;
  }
  else if(m_sel == "CR2LepOS"){
    m_selOS=true;
  }
  else if(m_sel == "CR2LepSS"){
    m_selSS=true;
  }
  else if(m_sel == "CR2LepSS40"){
    m_selSS=true;
    m_metRelMin=40;
  }
  //----------------------------//
  // Pre Signal Regions
  //----------------------------//
  if(m_sel == "preSRjveto"){ //SROSjveto - no metRel
    m_selOS = true;
    m_vetoZ = true;
    m_vetoJ = true;
    m_metRelMin=0;
  }
  else if(m_sel == "preSRmT2"){ //SRmT2 - no Mt2
    m_selOS = true;
    m_vetoZ = true;
    m_vetoJ = true;
    m_metRelMin=40;
    m_mt2Min=0;
  }
  else if(m_sel == "preSR2jets"){//SR2jets no metRel
    m_selOS = true;
    m_vetoZ = true;
    m_vetoF = true;
    m_vetoB = true;
    m_minC20 = 2;
    m_topTag = true;
    m_metRelMin=0;
  }
  else if(m_sel == "preSRZjets"){//SRZjets no metRel/mT2
    m_selOS     = true;
    m_selSF     = true;
    m_selZ      = true;
    m_vetoF     = true;
    m_vetoB     =  true;
    m_minC20    = 2;
    m_pTj0Min   = 50;
    m_pTj1Min   = 50;
    m_lowMjj    = 50;
    m_highMjj   = 100;
    m_topTag    = true;
    m_metRelMin = 0;
    m_mt2Min    = 0;
  }
  else if(m_sel == "preSRSS"){ //SRSSjets metRel40, no mTWW
    m_selSS = true;
    m_vetoB = true;
    m_vetoF = true;
    m_minC20 = 1;
    if(dilType==ET_ee){
      m_mllIn     = true;
      m_lowMll    = MZ-10;
      m_highMll   = MZ+10;
      m_metRelMin = 50;
    }
    else if(dilType==ET_mm || dilType==ET_em){
      m_metRelMin = 50;
      if(dilType==ET_mm && BLIND_SSMUON){
	m_mllIn     = true;
	m_lowMll  = 90;
	m_highMll = 120;
      }
    }
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
  //Set increment to mc weight (otherwise confusing w/ sample w/ -1 weight)
  if(nt->evt()->isMC) _inc = nt->evt()->w; 
  else _inc =1;
  if(nt->evt()->isMC && !WEIGHT_COUNT && WEIGHT_ONE) _inc=1;

  if(SYST==DGSys_NOM) n_readin+=_inc;

  if(!passEventCleaning() ){
    if(dbg()>10) cout<<"Fail cleaning" << endl;  
      return false;
    }

  //NEW Moriond
  if( !passDeadRegions(*v_baseJet,met, nt->evt()->run,nt->evt()->isMC)){
     if(dbg()>10) cout<<"Fail Dead Regions" << endl; 
     return false;
  }
  if(SYST==DGSys_NOM) n_pass_DeadRegion+=_inc;


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
  
  if( !passTrigger(leptons, met) ){ 
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
  if(!WEIGHT_COUNT) _ww=1;
  float _lepSFW  = getLepSFWeight(leptons);
  float _trigW   = getTriggerWeight(leptons, 
				    met->lv().Pt(),
				    signalJets->size(),
				    nt->evt()->nVtx,
				    SYST);
  float bTagWeight =  getBTagSF(nt->evt(),v_baseJet,SYST);

  if(WEIGHT_COUNT)  _ww *= _lepSFW * _trigW;
  float _wwSave = _ww;
  saveOriginal(); //Backup Met & leptons  --> newMet if charge flip
    
  if(dbg()>10){ 
    cout << ">>> run " << nt->evt()->run  
	 << " event " << nt->evt()->event 
	 << " SYST " << DG2LSystNames[SYST]
	 << " lepSF " << _lepSFW
	 << " trigW " << _trigW
	 << " bTag " << bTagWeight
	 << " weight(w/btag) " << _ww*bTagWeight << endl;
    // cout<< " myEvtW " << _ww*bTagWeight<<endl;
  }
 


  //
  //Loop over SR's & CR's
  //
  for(uint iSR=DIL_SROSjveto; iSR<DIL_NSR; iSR++){

    //
    //Debug, skip SS - 
    //
    /*
    if(iSR==DIL_SRSSjets || iSR==DIL_VRSS || iSR==DIL_VRSSbtag || 
       iSR==DIL_CR2LepSS || iSR==DIL_CR2LepSS40 || iSR==DIL_preSRSS) continue;
    */

    int icut =0;
    string sSR=DIL_SRNAME[iSR];
    setSelection(sSR,m_ET);
    SR=iSR;
    if(dbg() > 2 ) cout << "Signal region " << sSR << endl;

    //Reset weight in case used btagWeight in previous SR
    _ww=_wwSave; 
    if(WEIGHT_COUNT) _inc = _ww;
    else{
      if(!WEIGHT_ONE) _inc = nt->evt()->w;
    }

    //Only in MC do we alter the Ele pt - restore 
    if(nt->evt()->isMC) restoreOriginal(*leptons,met);

    //For data - fake estimate
    if( !nt->evt()->isMC && m_useLooseLep){
      float _metRel = getMetRel(&new_met,*leptons,*signalJets);
      _ww = getFakeWeight(leptons,nt->evt()->nVtx,nt->evt()->isMC,iSR,_metRel,SYST);
      if(WEIGHT_COUNT) _inc = _ww;
      //std::cout << " SR " << sSR << " Fake weight " << _ww << endl;
    }
    
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    //Deal with Charge flip estimate - obtain approprate weighting
    if(!USE_QFLIP && !passQQ(leptons)) continue;
    if(USE_QFLIP){
      if( nt->evt()->isMC && m_method == RLEP &&  m_ET!=ET_mm &&
	  (iSR==DIL_SRSSjets || iSR==DIL_CR2LepSS || iSR==DIL_CR2LepSS40 
	   || iSR==DIL_preSRSS  || iSR==DIL_VRSS || iSR==DIL_VRSSbtag) ){
	if(isGenuineSS(leptons) && SYST==DGSys_NOM )  n_pass_ss[m_ET][SR]+=_inc; //genuine SS - no qFlip
	else{ //OS ee/em event - get the qFlip prob
	  float _ww_qFlip = getQFlipProb(leptons,&new_met,DGSys_NOM);
	  _ww *= _ww_qFlip;
	  if(WEIGHT_COUNT) _inc = _ww;
	  if(SYST==DGSys_NOM) n_pass_ss[m_ET][SR]+=_inc;
	  //cout << "QFLup " << _ww_qFlip << " " << _wwSave << " " << _ww << endl;
	}
      }
      else
	if(!passQQ(leptons)) continue;
    }

    //
    // Apply event selection cuts
    // Bailout as soon one fail for a given SR/CR
    //
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passFlavor(leptons)){ 
      if(iSR==PRINT_SR) cout << "Fail flavor" << nt->evt()->run << " " << nt->evt()->event << endl; 
      continue; 
    }
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passZVeto(leptons)) {
      if(dbg()>5 && iSR==PRINT_SR) cout << "Fail Zveto " << nt->evt()->run << " " << nt->evt()->event <<endl;
      continue;
    }
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass Zveto " << sSR << endl;
    
    //
    // Apply bweight only to SR where we use jet count/veto
    //
    if(USE_BWEIGHT && nt->evt()->isMC) {
      if( ! ( iSR ==DIL_CR2LepOS || iSR==DIL_CR2LepSS 
	      || iSR==DIL_CR2LepSS40 || iSR==DIL_CRZ) )
	_ww *= bTagWeight;
	if(WEIGHT_COUNT) _inc = _ww;
	//cout << " Wsave " << _wwSave << " btag " <<  bTagWeight << " w " << _ww << endl;
    }

    if(!passFullJetVeto(signalJets)){
      if(dbg()>5 && iSR==PRINT_SR) cout << "Fail Jveto " << nt->evt()->run << " " << nt->evt()->event << endl;
      continue;
    }
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passFJet(signalJets)) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass F-Jet req " << sSR << endl;

    if(!passBJet(signalJets)) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass b-Jet req " << sSR << endl;

    if(!passLJet(signalJets)) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passCentralJet(signalJets)) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
	
    if(!passLead2JetsPt(signalJets) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass Jet pT " << sSR << endl;

    if(!passMjj(signalJets) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass Mjj " << sSR << endl;

    if(!passLead2LepPt(leptons) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass l0 pT " << sSR << endl;

    if(!passMll(leptons) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass Mll " << sSR << endl;


    if(!passPtll(leptons) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passdPhill(leptons) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passMWWT(leptons,&new_met) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passTopTagger(leptons,signalJets,&new_met) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass toptagger " << sSR << endl;

    if(!passMETRel(&new_met,leptons,signalJets) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass MetRel " << sSR << endl;

    if(!passMT2(leptons, &new_met) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    //if(!passDPhiMetll(leptons,&new_met) ) continue;
    //_hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    //if(!passDPhiMetl1(leptons,&new_met) ) continue;
    //_hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);


    if(dbg() >10 ) cout << "\t Pass All " << sSR << endl;

    //Blind data 
    if(!passBlindData(nt->evt()->isMC,iSR, metRel,mT2)) continue;

    //
    // Fill histos, ToyNt and HFT 
    //
    if(DO_FILL_HISTO ) fillHistograms(SR,SYST,leptons, signalJets,&new_met,_ww);
    if(dbg() >10 ) cout << "\t Filled histos " << sSR << endl;
    if(FILL_TOYNT && iSR==TOYNT_iSR && SYST==DGSys_NOM) fillToyNt(SR,SYST,leptons, signalJets,&new_met,_ww);

    if(m_writeHFT && validSystForHFT(SYST) ){
      float wHFT= writeIntoHistFitterTree(iSR,leptons,baseLeps,signalJets,v_baseJet,met);
      if(wHFT>0) _tmp += wHFT;
      
      if(wHFT>0 && nt->evt()->isMC && SYST==DGSys_NOM &&
	 !(iSR==DIL_SRSSjets || iSR==DIL_VRSS || iSR==DIL_VRSSbtag)
	 && fabs(_ww-wHFT)/wHFT>0.000001) 
	cout << "WARNING >>> run " << nt->evt()->run  
	     << " event " << nt->evt()->event 
	     << " mismatch weight with HFT " << _ww << " " << wHFT << endl;
      //if(nt->evt()->isMC) restoreOriginal(*leptons,met);//Because qFlip was have been called
    }
    
    
    //
    // Debugging - Dump run event
    //
    //if(DUMP_RUNEVT && (iSR==DIL_CR2LepOS || iSR==DIL_CR2LepSS) ){
    //if(DUMP_RUNEVT && SYST==DGSys_NOM && (iSR==DIL_NTOP && m_ET==ET_ee) ){
    if(DUMP_RUNEVT && (iSR==DIL_CRWW2) ){
      cout << "==>Run " << nt->evt()->run  << " : " << nt->evt()->event  << endl;
      evtDump << nt->evt()->run 
	      << " " << nt->evt()->event 
	      << " " << sSR 
	      << " " << DIL_FLAV[m_ET] 
	      << " " << _lepSFW
	      << " " << bTagWeight
	      << " " << _trigW
	      << " " << nt->evt()->w
	      << " " << nt->evt()->wPileup
	      << " " << nt->evt()->sumw
	      << " " << nt->evt()->xsec
	      << " " << _ww 
	      << endl;
      //if( nt->evt()->event==435108) dumpEvent();
    }
  }
  
  
  if(nt->evt()->isMC) restoreOriginal(*leptons,met);

  return true;
}
/*--------------------------------------------------------------------------------*/
// Event weight
/*--------------------------------------------------------------------------------*/
float Susy2LepAna::eventWeight(int mode)
{
  bool useSumWMap = true; 
  if( !nt->evt()->isMC) return 1; //Data weight =1

  float _evtW=nt->evt()->w;

  if(mode==NOLUMI) _evtW= nt->evt()->w; //raw weight - generator included!
  else if(mode==LUMI_E){ //Period E only
    if(USE_MCWEIGHT) _evtW =  nt->evt()->w; 
    else _evtW = nt->evt()->w * nt->evt()->wPileupAE * nt->evt()->xsec * pLUMI*1000. / nt->evt()->sumw;
  }
  else if(mode==LUMI_IJL){ //IJL
    //cout << "Weight " << nt->evt()->w  << " " << nt->evt()->wPileupIL << " " 
    //<<  nt->evt()->xsec << " " << pLUMI << " " << nt->evt()->sumw << endl;
    if(USE_MCWEIGHT) _evtW =  nt->evt()->w;
    else _evtW = nt->evt()->w * nt->evt()->wPileupIL * nt->evt()->xsec * pLUMI*1000. / nt->evt()->sumw;
  }
  else if(mode==LUMI13FB){ //HCP dataset
    if(USE_MCWEIGHT) _evtW =  nt->evt()->w;
    else 
      _evtW = nt->evt()->w * nt->evt()->wPileupAE * nt->evt()->xsec * LUMI_A_E / nt->evt()->sumw;
  }
  else if(mode==LUMI21FB){ //Moriond dataset
    if(USE_MCWEIGHT) _evtW =  nt->evt()->w;
    else{
      int id = nt->evt()->mcChannel;
      //      _evtW = getEventWeightFixed(id,nt->evt(),LUMI_A_L);
      _evtW = getEventWeight(nt->evt(),LUMI_A_L,useSumWMap,m_MCSumWs);   //<<<<=======


      //Replace XS for Higgs samples
      /*
      if(nt->evt()->mcChannel == 161005 ||
	 nt->evt()->mcChannel == 161055 ||
	 nt->evt()->mcChannel == 161805 ||
	 nt->evt()->mcChannel == 160155 ||
	 nt->evt()->mcChannel == 160205 ||
	 nt->evt()->mcChannel == 160255 ||
	 nt->evt()->mcChannel == 160305 ||
	 nt->evt()->mcChannel == 160655 ||
	 nt->evt()->mcChannel == 160705 ||
	 nt->evt()->mcChannel == 160755 ||
	 nt->evt()->mcChannel == 160805 ||
	 nt->evt()->mcChannel == 161105 || 
	 nt->evt()->mcChannel == 161155 ||
	 nt->evt()->mcChannel == 160555 || 
	 nt->evt()->mcChannel == 160505 ||
	 (nt->evt()->mcChannel >= 129477 && nt->evt()->mcChannel <= 129494)|| //WZ Powheg update XS MCNLO !!!
	 (nt->evt()->mcChannel >= 126949 && nt->evt()->mcChannel <= 126951)   //ZZ->llvv x3 Xs
	 ){

	if(m_xsecMap.find(id) == m_xsecMap.end()) {
	  m_xsecMap[id] = m_susyXsec->process(id);
	}
	xs = m_xsecMap[id].xsect() * m_xsecMap[id].kfactor() * m_xsecMap[id].efficiency();
      }
      */

      if(id==176322 || id==176325 || id==176480){
	float xs = nt->evt()->xsec;
	float sumw = nt->evt()->sumw;
	//SUSY WW-like
	if(id==176322) xs = 0.425175*0.9;
	if(id==176325) xs = 0.167127*0.9;
	if(id==176480) xs = 0.5420344;//0.616257;
	

	_evtW = nt->evt()->w * nt->evt()->wPileup * xs * LUMI_A_L / sumw;
      }

      if(dbg()>10)
	cout << "Ana W: " << nt->evt()->w 
	     << " pileup " << nt->evt()->wPileup 
	     << " xsec " <<  nt->evt()->xsec 
	     << " lumi " << LUMI_A_L 
	     << " sumw " << nt->evt()->sumw << endl;
    }
  }
  
  // Correct the cross section for DLiSlep
  if(
     (nt->evt()->mcChannel >= 166501 && nt->evt()->mcChannel <= 166658) ||
     (nt->evt()->mcChannel >= 175420 && nt->evt()->mcChannel <= 175583) ||
     (nt->evt()->mcChannel >= 177423 && nt->evt()->mcChannel <= 177496) 
     ) {
    if(m_SleptonXSecReader!=NULL) {
      if( nt->evt()->susyFinalState==201 ) 
	_evtW *= (m_SleptonXSecReader->getCrossSection(nt->evt()->mcChannel,SleptonPoint::kLeftHanded )/nt->evt()->xsec)*(nt->evt()->sumw/m_sleptonSumWs[201]);   
      else if( nt->evt()->susyFinalState==202 )
	_evtW *= (m_SleptonXSecReader->getCrossSection(nt->evt()->mcChannel,SleptonPoint::kRightHanded)/nt->evt()->xsec)*(nt->evt()->sumw/m_sleptonSumWs[202]);   
      else if( nt->evt()->susyFinalState==216 )
	_evtW *= (m_SleptonXSecReader->getCrossSection(nt->evt()->mcChannel,SleptonPoint::kLeftHanded )/nt->evt()->xsec)*(nt->evt()->sumw/m_sleptonSumWs[216]);   
      else if( nt->evt()->susyFinalState==217 )
	_evtW *= (m_SleptonXSecReader->getCrossSection(nt->evt()->mcChannel,SleptonPoint::kRightHanded)/nt->evt()->xsec)*(nt->evt()->sumw/m_sleptonSumWs[217]);   
      else _evtW = 0;  //protection against undefined final state
    }
    else
      cout << "m_SleptonXSecReader is null, won't touch the event weight" << endl;
  }
  

  return _evtW;
}

/*--------------------------------------------------------------------------------*/
// Lepton SF weight
/*--------------------------------------------------------------------------------*/
float Susy2LepAna::getLepSFWeight(const LeptonVector* leptons, uint iSys)
{
  float _wLepSF=1;
  if(USE_LEPSF && nt->evt()->isMC){
    for(uint ilep=0; ilep<leptons->size(); ilep++){
      const Susy::Lepton* _l = leptons->at(ilep);
      //TODO dealing with those syst here 
      _wLepSF *= _l->effSF;
      if(dbg()>10) cout << "lep" << ilep << " SF " << _l->effSF << endl; 
    }
  }
  return _wLepSF;
}
/*--------------------------------------------------------------------------------*/
// Trigger weight
/*--------------------------------------------------------------------------------*/
float  Susy2LepAna::getTriggerWeight(const LeptonVector* leptons, 
				     float met, int nSignalJets, int npv,
				     uint iSys){
  float _wTrig=1;
  
  if(USE_DGWEIGHT && nt->evt()->isMC) {
    uint iiSys = DGSys_NOM;
    if(iSys==DGSys_TRIGSF_EL_UP) iiSys=NtSys_TRIGSF_EL_UP;
    if(iSys==DGSys_TRIGSF_EL_DN) iiSys=NtSys_TRIGSF_EL_DN;
    if(iSys==DGSys_TRIGSF_MU_UP) iiSys=NtSys_TRIGSF_MU_UP;
    if(iSys==DGSys_TRIGSF_MU_DN) iiSys=NtSys_TRIGSF_MU_DN;
    
    _wTrig =  m_trigObj->getTriggerWeight(*leptons,  nt->evt()->isMC, 
					  met, nSignalJets, npv,
					  (SusyNtSys) iiSys);
    if(_wTrig<0 || _wTrig>1) {
      //cout << "WARNING Trigger weight out of bound - set to 0 or 1 " << DIL_FLAV[m_ET]  << " " << _wtrig << endl;
      if(_wTrig<0) _wTrig=0;
      if(_wTrig>0) _wTrig=1;
      
    }
    if(_wTrig != _wTrig){
      float _wTrigNoSys = m_trigObj->getTriggerWeight(*leptons, nt->evt()->isMC, 
						      met, nSignalJets, npv,
						      (SusyNtSys) 0);
      cout << "WARNING OVERWRITE TRIG SYS BECAUSE OF NAN" << endl;
      _wTrig = _wTrigNoSys;
      if(dbg()>10){
	cout << "No sys "<< _wTrigNoSys << " " << iiSys << " trigW " << _wTrig
	     << " pt1 " << leptons->at(0)->Pt() << " pt2 " << leptons->at(1)->Pt() << endl; 
      }
    }
  }
  return _wTrig;
}
/*--------------------------------------------------------------------------------*/
float Susy2LepAna::getBTagSF(const Susy::Event*, const JetVector* jets, uint iSys)
{
  if(!nt->evt()->isMC) return 1;
  if(!USE_BWEIGHT)     return 1;

  JetVector  valJets;
  valJets.clear();
  for(uint i=0; i<jets->size(); ++i){
    Jet* jet = jets->at(i);
    if( jet->Pt() < JET_PT_L20_CUT        ) continue;
    //if( fabs(jet->Eta()) > JET_ETA_CUT_2L ) continue;
    if( fabs(jet->detEta) > JET_ETA_CUT_2L ) continue;
    valJets.push_back(jet);
  }
  
  if(valJets.size()==0) return 1;//safety.

  //Get sys naming convention
  uint _sys = DGSys_NOM;
  if(iSys==DGSys_BJet_DN) _sys=BTag_BJet_DN; 
  if(iSys==DGSys_CJet_DN) _sys=BTag_CJet_DN; 
  if(iSys==DGSys_LJet_DN) _sys=BTag_LJet_DN; 
  if(iSys==DGSys_BJet_UP) _sys=BTag_BJet_UP; 
  if(iSys==DGSys_CJet_UP) _sys=BTag_CJet_UP; 
  if(iSys==DGSys_LJet_UP) _sys=BTag_LJet_UP; 

  return bTagSF(nt->evt(),valJets,USE_NOJVF_bSF, 
		"MV1", "0_3511", MV1_80, (BTagSys) _sys);

}
/*--------------------------------------------------------------------------------*/
// Fake Bkg estimate event weight
/*--------------------------------------------------------------------------------*/
float Susy2LepAna::getFakeWeight(const LeptonVector* leptons, uint nVtx, 
				 bool isMC, int iSR, float metrel,
				 uint iSys)
{
  bool _isSignal[2];
  bool _isEle[2];
  float _pt[2];
  float _eta[2];
  
  if(leptons->size()>2) return 0;

  SusyMatrixMethod::FAKE_REGION  frSR = SusyMatrixMethod::FR_CRZXOSjveto;
  switch (iSR){
  case DIL_SROSjveto:
    frSR = SusyMatrixMethod::FR_SROSjveto;
    break;
  case DIL_SRmT2a:
    frSR = SusyMatrixMethod::FR_SRmt2a;
    break;
  case DIL_SRmT2b:
    frSR = SusyMatrixMethod::FR_SRmt2b;
    break;
  case DIL_SR2jets:
    frSR = SusyMatrixMethod::FR_SR2jets; 
    break;
  case DIL_SRZjets:
    frSR = SusyMatrixMethod::FR_SRZjets; 
    break;
  case DIL_SRSSjets:
    frSR = SusyMatrixMethod::FR_SRSSjets;
    break;
  case DIL_SRWWa:
    frSR = SusyMatrixMethod::FR_SRWWa;
    break;
  case DIL_SRWWb:
    frSR = SusyMatrixMethod::FR_SRWWb;
    break;
  case DIL_SRWWc:
    frSR = SusyMatrixMethod::FR_SRWWc;
    break;
  case DIL_ZXCRjveto:
    frSR = SusyMatrixMethod::FR_CRZXOSjveto;
    break;
  case DIL_ZXCR2jets:
    frSR = SusyMatrixMethod::FR_CRZX2JETS;
    break;
  case DIL_ZXCRpremT2:
    frSR = SusyMatrixMethod::FR_CRZXMT2a;
    break;
  case DIL_ZXCRmT2a:
    frSR = SusyMatrixMethod::FR_CRZXMT2a;
    break;
  case DIL_ZXCRmT2b:
    frSR = SusyMatrixMethod::FR_CRZXMT2b;
    break;
  case DIL_ZXCRWW:
    frSR = SusyMatrixMethod::FR_CRZXWW;
    break;
  case DIL_ZXCRWW2:
    frSR = SusyMatrixMethod::FR_CRZXWW;
    break;
  case DIL_CRTOP:
    frSR = SusyMatrixMethod::FR_CRTOP;
    break;
  case DIL_CRTOPWWa:
    frSR = SusyMatrixMethod::FR_CRTOPWWa;
    break;
  case DIL_CRTOPWWb:
    frSR = SusyMatrixMethod::FR_CRTOPWWb;
    break;
  case DIL_CRTOPWWc:
    frSR = SusyMatrixMethod::FR_CRTOPWWc;
    break;
  case DIL_CRWW:
    frSR = SusyMatrixMethod::FR_CRWW1;
    break;    
  case DIL_CRWW2:
    frSR = SusyMatrixMethod::FR_CRWW1;
    break;    
  case DIL_CRWW3:
    frSR = SusyMatrixMethod::FR_CRWW1;
    break;    
  case DIL_CRWW4:
    frSR = SusyMatrixMethod::FR_CRWW1;
    break;    
  case DIL_CRWW5:
    frSR = SusyMatrixMethod::FR_CRWW3;
    break;    
  case DIL_CRWW6:
    frSR = SusyMatrixMethod::FR_CRWW2;
    break;    
  case DIL_CRWWa:
    frSR = SusyMatrixMethod::FR_CRWWa;
    break;    
  case DIL_CRWWb:
    frSR = SusyMatrixMethod::FR_CRWWb;
    break;    
  case DIL_CRWWc:
    frSR = SusyMatrixMethod::FR_CRWWc;
    break;    
  case DIL_VRSS:
    frSR = SusyMatrixMethod::FR_VRSS;
    break;    
  case DIL_VRSSbtag:
    frSR = SusyMatrixMethod::FR_VRSSbtag;
    break;    
  case DIL_CRZ:
    frSR = SusyMatrixMethod::FR_CRZXOSjveto;
    break;        
  case DIL_CRZjveto:
    frSR = SusyMatrixMethod::FR_CRZXOSjveto;
    break;        
  case DIL_CR2LepOS:
    frSR = SusyMatrixMethod::FR_CRZXOSjveto; 
    break;
  case DIL_CR2LepSS:
    frSR = SusyMatrixMethod::FR_VRSSbtag;
    break;
  case DIL_CR2LepSS40:
    frSR = SusyMatrixMethod::FR_VRSSbtag;
    break;
  case DIL_preSROSjveto:
    frSR = SusyMatrixMethod::FR_SROSjveto;
    break;
  case DIL_preSRmT2:
    frSR = SusyMatrixMethod::FR_PreMt2;
    break;
  case DIL_preSR2jets:
    frSR = SusyMatrixMethod::FR_SR2jets; 
    break;
  case DIL_preSRZjets:
    frSR = SusyMatrixMethod::FR_SRZjets; 
    break;
  case DIL_preSRSS:
    frSR = SusyMatrixMethod::FR_VRSSbtag;
    break;
  }

  for(uint i=0; i<leptons->size(); i++){
    _isEle[i]=leptons->at(i)->isEle();
    _pt[i]= leptons->at(i)->pt*1000;//MeV
    _eta[i]= leptons->at(i)->eta;
    if(_isEle[i])_isSignal[i] = isSignalElectron((Electron*) leptons->at(i),*v_baseEle,*v_baseMu,nVtx,isMC,false);
    else         _isSignal[i] = isSignalMuon((Muon*) leptons->at(i),*v_baseEle,*v_baseMu,nVtx,isMC,false);
  }  

  //Map naming convention  
  float _fw = 0;
  uint iiSys = DGSys_NOM;
  if(iSys==DGSys_FAKE_EL_RE_UP) iiSys=SusyMatrixMethod::SYS_EL_RE_UP;
  if(iSys==DGSys_FAKE_EL_RE_DN) iiSys=SusyMatrixMethod::SYS_EL_RE_DOWN;
  if(iSys==DGSys_FAKE_EL_FR_UP) iiSys=SusyMatrixMethod::SYS_EL_FR_UP;
  if(iSys==DGSys_FAKE_EL_FR_DN) iiSys=SusyMatrixMethod::SYS_EL_FR_DOWN;
  if(iSys==DGSys_FAKE_MU_RE_UP) iiSys=SusyMatrixMethod::SYS_MU_RE_UP;
  if(iSys==DGSys_FAKE_MU_RE_DN) iiSys=SusyMatrixMethod::SYS_MU_RE_DOWN;
  if(iSys==DGSys_FAKE_MU_FR_UP) iiSys=SusyMatrixMethod::SYS_MU_FR_UP;
  if(iSys==DGSys_FAKE_MU_FR_DN) iiSys=SusyMatrixMethod::SYS_MU_FR_DOWN;

  _fw = m_matrix_method.getTotalFake(_isSignal[0], _isEle[0], _pt[0],_eta[0],
				     _isSignal[1], _isEle[1], _pt[1],_eta[1],
				     frSR, metrel, 
				     (SusyMatrixMethod::SYSTEMATIC) iiSys);  

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
  uint iiSys = SYST;
  if(SYST>DGSys_RESOST) iiSys = DGSys_NOM;

  int cutFlag = nt->evt()->cutFlags[iiSys];

  //
  // Done in SusyNt filtering apart for TileTrip
  //
  if(!passGRL(cutFlag)) { 
    if(dbg()>15) cout << "GRL " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_GRL+=_inc;

  if(!passLarErr(cutFlag)) { 
    if(dbg()>15) cout << "LarErr " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_LarErr+=_inc;

  if(!passTileErr(cutFlag)) { 
    if(dbg()>15) cout << "TileErr " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_TileErr+=_inc;

  if(!passTTCVeto(cutFlag)) { 
    if(dbg()>15) cout << "TTCVeto " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_TTCVeto+=_inc;

  if(!passGoodVtx(cutFlag)) { 
    if(dbg()>15) cout << "Good Vtx " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_GoodVtx+=_inc;

  if(!passTileTripCut(cutFlag)) { 
    if(dbg()>15) cout << "TileTrip " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_TileTrip+=_inc;


  if(!passHotSpot(cutFlag)) { //UPDATED MORIOND
    if(dbg()>15) cout << "hot spot " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_HotSpot+=_inc;

  if(!passBadJet (cutFlag)){
    if(dbg()>15) cout << "bad jet " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_BadJet+=_inc;
 
  if(!passBadMuon(cutFlag)) {
    if(dbg()>15) cout << "bad muon " << endl;
    return false;
  }
  if(SYST==DGSys_NOM) n_pass_BadMuon+=_inc;

  if(!passCosmic (cutFlag)){
    if(dbg()>15) cout << "cosmic " << endl;
    return false;
  }
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
bool Susy2LepAna::passTrigger(const LeptonVector* leptons,const Met *met)
{
  if(leptons->size() < 1) return false;
  if( m_trigObj->passDilTrig(*leptons, met->lv().Pt(), nt->evt()) ){
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
      int truthMatch = _l->truthType;
      bool isEle     = _l->isEle();
      
      bool isReal       = isPT(org,type,mcId,truthMatch,isEle,_hh->sampleName());
      bool isChargeFlip =  _l->isEle() ? ((Electron*) _l)->isChargeFlip : false; 
      
      //Anyes 040613 - Hack gg2WW
      if(mcId >= 169471 && mcId <=169479) return true;

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
float Susy2LepAna::getQFlipProb(const LeptonVector* leptons, Met* met, uint iSys)
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
  if(iSys==DGSys_BKGMETHOD_UP) _sys=1;
  if(iSys==DGSys_BKGMETHOD_DN) _sys=-1;

  float cfP = m_chargeFlip->OS2SS(_pdg1, &_l1_tlv, 
				  _pdg2, &_l2_tlv, 
				  &_new_met, _sys);

  cfP*=  m_chargeFlip->overlapFrac().first; // QFLIP_RESCLALE;
  //cfP*= QFLIP_RESCLALE;
  
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
	 << "\t cfP " << cfP << " " << QFLIP_RESCLALE
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
  if(m_selOF && sameFlav) return false;
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
// Signal region cuts  - use all signal jets C20+B20+F30
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passFullJetVeto(const JetVector* jets)
{
  if(m_vetoJ && jets->size()>0) return false;

  if(SYST==DGSys_NOM) n_pass_FullJveto[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passBJet(const JetVector* jets){
  int nBJet= numberOfCBJets(*jets);
  if(m_vetoB && nBJet>0) return false;
  if(m_selB  && nBJet<=0) return false;
  if(SYST==DGSys_NOM) n_pass_BJet[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passFJet(const JetVector* jets){
  int nFJet= numberOfFJets(*jets);
  if(m_vetoF && nFJet>0) return false;
  if(SYST==DGSys_NOM) n_pass_FJet[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passLJet(const JetVector* jets){
  int nLJet= numberOfCLJets(*jets);
  if(m_minC20>-1 && nLJet<m_minC20) return false;
  if(SYST==DGSys_NOM) n_pass_LJet[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passCentralJet(const JetVector* jets){
  int nCJet= numberOfCLJets(*jets)+ numberOfCBJets(*jets);
  if(m_minCJet>-1 && nCJet<m_minCJet) return false;
  if(SYST==DGSys_NOM) n_pass_CJet[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passLead2JetsPt(const JetVector* jets){
  if(m_pTj0Min <0 && m_pTj1Min <0 ) return true; //cut not applied
  if(jets->size()<2) return false;
  if(jets->at(0)->Pt() < m_pTj0Min) return false;
  if(jets->at(1)->Pt() < m_pTj1Min) return false;
  if(SYST==DGSys_NOM) n_pass_JetPt[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passMjj(const JetVector* jets){
  if(m_lowMjj <0 && m_highMjj <0 ) return true; //cut not applied 
  if(jets->size()<2) return false;
  float mjj=Mll(jets->at(0),jets->at(1));
  if(mjj < m_lowMjj) return false;
  if(mjj > m_highMjj) return false;
  if(SYST==DGSys_NOM) n_pass_mjj[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passZVeto(const LeptonVector* leptons, float Zlow, float Zhigh)
{
  if( leptons->size() < 2 ) return false;
  
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
bool Susy2LepAna::passMll(const LeptonVector* leptons)
{
  float mll=Mll(leptons->at(0),leptons->at(1));
  if(m_mllIn){  //reject inside mll window
    if(m_lowMll <0 && m_highMll<0) return true;
    if(mll > m_lowMll && mll < m_highMll) return false;
  }
  else { //upper or lower bound on mll
    if(m_lowMll>-1  && mll < m_lowMll) return false;
    if(m_highMll>-1 && mll > m_highMll) return false;
  }
  if(SYST==DGSys_NOM) n_pass_mll[m_ET][SR]+=_inc;

  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passMETRel(const Met *met, const LeptonVector* leptons, 
			     const JetVector* jets)
{
  metRel = getMetRel(met,*leptons,*jets);
  if(m_metRelMin>=0 &&  metRel < m_metRelMin ) return false;
  if(m_metRelMax>=0 &&  metRel > m_metRelMax ) return false;
  if(SYST==DGSys_NOM) n_pass_metRel[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passLead2LepPt(const LeptonVector* leptons)
{
  if( leptons->size() < 1 ) return false;
  if(m_pTl0Min >-1 && leptons->at(0)->Pt() < m_pTl0Min) return false;
  if(m_pTl1Min >-1 && leptons->at(1)->Pt() < m_pTl1Min) return false;
  if(SYST==DGSys_NOM) n_pass_leadLepPt[m_ET][SR]+=_inc;
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
bool Susy2LepAna::passdPhill(const LeptonVector* leptons){
  if( leptons->size() < 2 ) return false;
  float dPhi = fabs(leptons->at(0)->DeltaPhi(*leptons->at(1)));
  if(m_dPhillMax>-1 && dPhi>m_dPhillMax) return false;
  if(SYST==DGSys_NOM) n_pass_dPhill[m_ET][SR]+=_inc;
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
    /*
    if( (iSR==DIL_SROSjveto || iSR==DIL_SRSSjveto) && metRel>=m_metRelMin) return false;
    if( iSR==DIL_SR2jets && metRel>=m_metRelMin) return false;
    if( (iSR==DIL_SRmT2a ||iSR==DIL_SRmT2b) && mt2>m_mt2Min ) return false;
    */
  }
  return true;
}

/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passMT2(const LeptonVector* leptons, const Met* met)
{
  mT2 = getMT2(*leptons, met);
  if(m_mt2Min>-1 && mT2<m_mt2Min) return false;
  if(m_mt2Max>-1 && mT2>m_mt2Max) return false;
  if(SYST==DGSys_NOM) n_pass_mt2[m_ET][SR]+=_inc;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::passMWWT(const LeptonVector* leptons, const Met* met)
{
  if( leptons->size() < 2 ) return false;
  TLorentzVector _ll = (*leptons->at(0) + *leptons->at(1));
  float mWT = mT(_ll, met->lv());

  if(m_lowMTWW>-1 && mWT<m_lowMTWW) return false;
  if(m_highMTWW>-1 && mWT>m_highMTWW) return false;
  if(SYST==DGSys_NOM) n_pass_mWWT[m_ET][SR]+=_inc;
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
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_SROSjveto()
{
  int j= DIL_SROSjveto;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass Zveto  ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass Jveto  ",n_pass_FullJveto[0][j], n_pass_FullJveto[1][j], n_pass_FullJveto[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass MT2(<90)",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_SRmT2()
{
  int j= DIL_SRmT2a;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass Jveto  ",n_pass_FullJveto[0][j], n_pass_FullJveto[1][j], n_pass_FullJveto[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass MT2(90)",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
  j= DIL_SRmT2b;
  print_line("pass MT2(110)",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
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
  print_line("pass bVeto  ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass 2Jets  ",n_pass_CJet[0][j], n_pass_CJet[1][j], n_pass_CJet[2][j]);
  print_line("pass topTag ",n_pass_topTag[0][j], n_pass_topTag[1][j], n_pass_topTag[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);

}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_SRZjets()
{
  int j= DIL_SRZjets;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass SF     ",n_pass_flav[0][j], n_pass_flav[1][j], n_pass_flav[2][j]);
  print_line("pass Z      ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass bVeto  ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass 2Jets  ",n_pass_CJet[0][j], n_pass_CJet[1][j], n_pass_CJet[2][j]);
  print_line("pass JetsPt ",n_pass_JetPt[0][j],n_pass_JetPt[1][j],n_pass_JetPt[2][j]);
  print_line("pass Mjj    ",n_pass_mjj[0][j],n_pass_mjj[1][j],n_pass_mjj[2][j]);
  print_line("pass topTag ",n_pass_topTag[0][j], n_pass_topTag[1][j], n_pass_topTag[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass MT2(70)",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_SRSSjets()
{
  int j= DIL_SRSSjets;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass SS     ",n_pass_ss[0][j], n_pass_ss[1][j], n_pass_ss[2][j]);
  print_line("pass bVeto  ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass nJets  ",n_pass_CJet[0][j], n_pass_CJet[1][j], n_pass_CJet[2][j]);
  print_line("pass Mll    ",n_pass_mll[0][j],n_pass_mll[1][j],n_pass_mll[2][j]);
  print_line("pass mWWT   ",n_pass_mWWT[0][j], n_pass_mWWT[1][j], n_pass_mWWT[2][j]);  
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_SRWW()
{
  int j= DIL_SRWWa;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass Jveto  ",n_pass_FullJveto[0][j], n_pass_FullJveto[1][j], n_pass_FullJveto[2][j]);
  print_line("pass lepPt  ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass Mll    ",n_pass_mll[0][j],n_pass_mll[1][j],n_pass_mll[2][j]);
  print_line("pass pTll   ",n_pass_pTll[0][j],n_pass_pTll[1][j],n_pass_pTll[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  
  j= DIL_SRWWb;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass Mll    ",n_pass_mll[0][j],n_pass_mll[1][j],n_pass_mll[2][j]);
  print_line("pass pTll   ",n_pass_pTll[0][j],n_pass_pTll[1][j],n_pass_pTll[2][j]);
  print_line("pass MT2    ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
  
  j= DIL_SRWWc;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass pTll   ",n_pass_pTll[0][j],n_pass_pTll[1][j],n_pass_pTll[2][j]);
  print_line("pass MT2    ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
}

/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_CRZ()
{
  int j= DIL_CRZ;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS        ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass Z         ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);

  j= DIL_CRZjveto;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass Jveto  ",n_pass_FullJveto[0][j], n_pass_FullJveto[1][j], n_pass_FullJveto[2][j]);
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_ZXCR()
{
  int j= DIL_ZXCRjveto;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass Z      ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass Jveto  ",n_pass_FullJveto[0][j], n_pass_FullJveto[1][j], n_pass_FullJveto[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);

  j= DIL_ZXCRmT2a;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass MT2(90)",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
  j= DIL_ZXCRmT2b;
  print_line("pass MT2(110)",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);

  j= DIL_ZXCR2jets;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass bVeto  ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass 2Jets  ",n_pass_CJet[0][j], n_pass_CJet[1][j], n_pass_CJet[2][j]);
  print_line("pass topTag ",n_pass_topTag[0][j], n_pass_topTag[1][j], n_pass_topTag[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass MT2(55)",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);

  j= DIL_ZXCRWW;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass MT2(90)",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
  
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_TOPCR()
{
  int j= DIL_CRTOP;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass Z      ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass btag   ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass 2Jets  ",n_pass_CJet[0][j], n_pass_CJet[1][j], n_pass_CJet[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);

  j= DIL_CRTOPWWa;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass btag   ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass MT2    ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);

  j= DIL_CRTOPWWb;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass MT2    ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
  
  j= DIL_CRTOPWWc;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass MT2    ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_WWCR()
{
  int j= DIL_CRWW;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass Z      ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass Jveto  ",n_pass_FullJveto[0][j], n_pass_FullJveto[1][j], n_pass_FullJveto[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass MT2(90)",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
  j= DIL_CRWW3;
  print_line("pass pTll   ",n_pass_pTll[0][j],n_pass_pTll[1][j],n_pass_pTll[2][j]);
  j= DIL_CRWW2;
  print_line("pass MT2(50-90)",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
  j= DIL_CRWW4;
  print_line("pass pTll   ",n_pass_pTll[0][j],n_pass_pTll[1][j],n_pass_pTll[2][j]);
  j= DIL_CRWW5;
  print_line("pass MT2(90)",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);

  j= DIL_CRWWa;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass lepPt  ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass Mll    ",n_pass_mll[0][j],n_pass_mll[1][j],n_pass_mll[2][j]);
  print_line("pass pTll   ",n_pass_pTll[0][j],n_pass_pTll[1][j],n_pass_pTll[2][j]);
  print_line("pass dPhill ",n_pass_dPhill[0][j],n_pass_dPhill[1][j],n_pass_dPhill[2][j]);

  j= DIL_CRWWb;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass Mll    ",n_pass_mll[0][j],n_pass_mll[1][j],n_pass_mll[2][j]);
  print_line("pass pTll   ",n_pass_pTll[0][j],n_pass_pTll[1][j],n_pass_pTll[2][j]);
  print_line("pass dPhill ",n_pass_dPhill[0][j],n_pass_dPhill[1][j],n_pass_dPhill[2][j]);
  print_line("pass MT2    ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);

  j= DIL_CRWWc;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass pTll   ",n_pass_pTll[0][j],n_pass_pTll[1][j],n_pass_pTll[2][j]);
  print_line("pass dPhill ",n_pass_dPhill[0][j],n_pass_dPhill[1][j],n_pass_dPhill[2][j]);
  print_line("pass MT2    ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_VRSS()
{
  int j= DIL_VRSS;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass SS     ",n_pass_ss[0][j], n_pass_ss[1][j], n_pass_ss[2][j]);
  print_line("pass bVeto  ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass nJets  ",n_pass_CJet[0][j], n_pass_CJet[1][j], n_pass_CJet[2][j]);
  print_line("pass Mll    ",n_pass_mll[0][j],n_pass_mll[1][j],n_pass_mll[2][j]);
  print_line("pass mWWT   ",n_pass_mWWT[0][j], n_pass_mWWT[1][j], n_pass_mWWT[2][j]);  
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);

  j= DIL_VRSSbtag;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass SS     ",n_pass_ss[0][j], n_pass_ss[1][j], n_pass_ss[2][j]);
  print_line("pass bVeto  ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass nJets  ",n_pass_CJet[0][j], n_pass_CJet[1][j], n_pass_CJet[2][j]);
  print_line("pass Mll    ",n_pass_mll[0][j],n_pass_mll[1][j],n_pass_mll[2][j]);
  print_line("pass mWWT   ",n_pass_mWWT[0][j], n_pass_mWWT[1][j], n_pass_mWWT[2][j]);  
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);

}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_line(string s, float a, float b, float c)
{
  cout << setprecision(6)  << s << "\t" << a << "\t" << b << "\t" << c << endl;
}

/*--------------------------------------------------------------------------------*/
// Save/restore orignal lepton - to deal with lepton smearing w/ qFlip
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
  std::sort(leptons.begin(), leptons.end(), comparePt);
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
// Fill histograms 
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::fillHistograms(uint iSR,uint iSYS,
				 const LeptonVector* leptons, 
				 const JetVector* jets,
				 const Met* met,
				 float _ww)
{
  if(dbg()>5) cout << "Fill histo " << DIL_FLAV[m_ET] << " " << DIL_SRNAME[iSR] << endl;

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
  float dPhilMet=999; //min dPhi (l,MET)
  float mTl1=-999;
  float mTl2=-999;

  for(uint ilep=0; ilep<leptons->size(); ilep++){
    const Susy::Lepton* _l = leptons->at(ilep);
    _ll = _ll + (*_l);
    bool isChargeFlip =  _l->isEle() ? ((Electron*) _l)->isChargeFlip : false; 
    LEP_TYPE lType = getType(_l->mcOrigin,
			     _l->mcType,
			     _hh->sampleName(),
			     nt->evt()->mcChannel,
			     _l->truthType,
			     _l->isEle(),
			     isChargeFlip);
        
    float _dPhi=fabs(met->lv().DeltaPhi(*_l));
    if(_dPhi<dPhilMet) dPhilMet=_dPhi;

    if(ilep==0){
      _hh->H1FILL(_hh->DG2L_ptl1[iSR][m_ET][iSYS],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etal1[iSR][m_ET][iSYS],_l->Eta(),_ww); 
      _hh->H1FILL(_hh->DG2L_d0Sl1[iSR][m_ET][iSYS],_l->d0/_l->errD0,_ww); 
      _hh->H1FILL(_hh->DG2L_z0sinthetal1[iSR][m_ET][iSYS],_l->z0SinTheta(),_ww); 
      _hh->H1FILL(_hh->DG2L_orgl1[iSR][m_ET][iSYS],lType,_ww); 
      q1=_l->q;
      mTl1 = mT(*_l, met->lv());
    }
    else if(ilep==1){
      _hh->H1FILL(_hh->DG2L_ptl2[iSR][m_ET][iSYS],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etal2[iSR][m_ET][iSYS],_l->Eta(),_ww); 
      _hh->H1FILL(_hh->DG2L_d0Sl2[iSR][m_ET][iSYS],_l->d0/_l->errD0,_ww); 
      _hh->H1FILL(_hh->DG2L_z0sinthetal2[iSR][m_ET][iSYS],_l->z0SinTheta(),_ww); 
      _hh->H1FILL(_hh->DG2L_orgl2[iSR][m_ET][iSYS],lType,_ww); 
      q2=_l->q;
      mTl2 = mT(*_l, met->lv());
    }
    
    if(_l->isEle()){
      _hh->H1FILL(_hh->DG2L_ePt[iSR][m_ET][iSYS],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_eEta[iSR][m_ET][iSYS],_l->Eta(),_ww); 
    }
    else{
      _hh->H1FILL(_hh->DG2L_mPt[iSR][m_ET][iSYS],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_mEta[iSR][m_ET][iSYS],_l->Eta(),_ww); 
    }

  }//leptons 
  dPhilMet = TVector2::Phi_mpi_pi(dPhilMet)*TMath::RadToDeg();  

  if(q1*q2<0){//OS
    qqType = (q1<0 && q2>0) ? 0 : 1;  //-+/+-
  }
  else if(q1*q2>0){//SS
    qqType = (q1<0 && q2<0) ? 0 : 1;  //--/++
  }
  _hh->H1FILL(_hh->DG2L_qq[iSR][m_ET][iSYS],qqType,_ww); 
  
  float mWT = mT(_ll, met->lv());

  _hh->H1FILL(_hh->DG2L_mll[iSR][m_ET][iSYS],_ll.M(),_ww); 
  _hh->H1FILL(_hh->DG2L_mllcoarse[iSR][m_ET][iSYS],_ll.M(),_ww); 
  _hh->H1FILL(_hh->DG2L_mllcoarser[iSR][m_ET][iSYS],_ll.M(),_ww); 
  _hh->H1FILL(_hh->DG2L_pTll[iSR][m_ET][iSYS],_ll.Pt(),_ww); 
  _hh->H1FILL(_hh->DG2L_mWWT[iSR][m_ET][iSYS],mWT,_ww); 
  _hh->H1FILL(_hh->DG2L_mTl1[iSR][m_ET][iSYS],mTl1,_ww);
  _hh->H1FILL(_hh->DG2L_mTl2[iSR][m_ET][iSYS],mTl2,_ww); 
  _hh->H1FILL(_hh->DG2L_dPhill[iSR][m_ET][iSYS],fabs(dPhill),_ww); 
  _hh->H1FILL(_hh->DG2L_dPhilMet[iSR][m_ET][iSYS],fabs(dPhilMet),_ww); 
  _hh->H1FILL(_hh->DG2L_JZBJet[iSR][m_ET][iSYS],JZBJet(v_sigJet,leptons),_ww); 
  _hh->H1FILL(_hh->DG2L_JZBEtmiss[iSR][m_ET][iSYS],JZBEtmiss(met,leptons),_ww); 
  _hh->H1FILL(_hh->DG2L_etmiss[iSR][m_ET][iSYS],met->lv().Pt(),_ww); 
  _hh->H1FILL(_hh->DG2L_etmissPhi[iSR][m_ET][iSYS],met->lv().Phi(),_ww); 
  _hh->H1FILL(_hh->DG2L_metrel[iSR][m_ET][iSYS],metRel,_ww); 
  _hh->H1FILL(_hh->DG2L_metrel1[iSR][m_ET][iSYS],metRel,_ww); 
  _hh->H1FILL(_hh->DG2L_metrel2[iSR][m_ET][iSYS],metRel,_ww); 
  _hh->H1FILL(_hh->DG2L_metrel3[iSR][m_ET][iSYS],metRel,_ww); 
  _hh->H1FILL(_hh->DG2L_metRefEle[iSR][m_ET][iSYS],met->refEle,_ww); 
  _hh->H1FILL(_hh->DG2L_metRefGam[iSR][m_ET][iSYS],met->refGamma,_ww); 
  _hh->H1FILL(_hh->DG2L_metRefMuo[iSR][m_ET][iSYS],met->refMuo,_ww); 
  _hh->H1FILL(_hh->DG2L_metRefJet[iSR][m_ET][iSYS],met->refJet,_ww); 
  _hh->H1FILL(_hh->DG2L_metRefSJet[iSR][m_ET][iSYS],met->softJet,_ww); 
  _hh->H1FILL(_hh->DG2L_metCellout[iSR][m_ET][iSYS],met->refCell,_ww); 
  _hh->H1FILL(_hh->DG2L_mt2[iSR][m_ET][iSYS],mT2,_ww); 
  _hh->H1FILL(_hh->DG2L_mt2b[iSR][m_ET][iSYS],mT2,_ww); 

  float corrNpv = nt->evt()->nVtx;
  if(nt->evt()->isMC) corrNpv = GetNVertexBsCorrected(nt->evt()->nVtx);
  _hh->H1FILL(_hh->DG2L_npv[iSR][m_ET][iSYS],corrNpv,_ww); 
  _hh->H1FILL(_hh->DG2L_mu[iSR][m_ET][iSYS],nt->evt()->avgMu,_ww); 

  int nBJets=0;
  int nSigJet=0;
  int nSigCJet=0;
  int nSigFJet=0;
  float dPhiJMet=999;
  float mEff=0;
  float ST=0;

  for(uint ijet=0; ijet<jets->size(); ijet++){
    const Susy::Jet* _j = jets->at(ijet);
    nSigJet++;
    if(isCentralLightJet(_j)) nSigCJet++;
    if(isForwardJet(_j)) nSigFJet++;
    if(isCentralBJet(_j)){
      nBJets++;
      _hh->H1FILL(_hh->DG2L_ptbj[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etabj[iSR][m_ET][iSYS],_j->Eta(),_ww); 
      _hh->H1FILL(_hh->DG2L_jvfbj[iSR][m_ET][iSYS],_j->jvf,_ww); 
    }

    if(ijet==0){
      _hh->H1FILL(_hh->DG2L_ptj1[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etaj1[iSR][m_ET][iSYS],_j->Eta(),_ww); 
      _hh->H1FILL(_hh->DG2L_jvfj1[iSR][m_ET][iSYS],_j->jvf,_ww); 
    }
    if(ijet==1){
      _hh->H1FILL(_hh->DG2L_ptj2[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etaj2[iSR][m_ET][iSYS],_j->Eta(),_ww);
      _hh->H1FILL(_hh->DG2L_jvfj2[iSR][m_ET][iSYS],_j->jvf,_ww);  
    }
    if(ijet==2){
      _hh->H1FILL(_hh->DG2L_ptj3[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etaj3[iSR][m_ET][iSYS],_j->Eta(),_ww); 
    }
    if(ijet==3){
      _hh->H1FILL(_hh->DG2L_ptj4[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etaj4[iSR][m_ET][iSYS],_j->Eta(),_ww); 
    }

    float _dPhi=fabs(met->lv().DeltaPhi(*_j));
    if(_dPhi<dPhiJMet) dPhiJMet=_dPhi;
    ST += _j->Pt();

  }
  mEff = ST + met->lv().Pt();

  dPhiJMet = TVector2::Phi_mpi_pi(dPhiJMet)*TMath::RadToDeg();  

  _hh->H1FILL(_hh->DG2L_ST[iSR][m_ET][iSYS],ST,_ww); 
  _hh->H1FILL(_hh->DG2L_mEff[iSR][m_ET][iSYS],mEff,_ww); 
  _hh->H1FILL(_hh->DG2L_dPhiJetMet[iSR][m_ET][iSYS],fabs(dPhiJMet),_ww); 
  _hh->H1FILL(_hh->DG2L_nJets[iSR][m_ET][iSYS],nSigJet,_ww); 
  _hh->H1FILL(_hh->DG2L_nCJets[iSR][m_ET][iSYS],nSigCJet,_ww); 
  _hh->H1FILL(_hh->DG2L_nFJets[iSR][m_ET][iSYS],nSigFJet,_ww); 
  _hh->H1FILL(_hh->DG2L_nBJets[iSR][m_ET][iSYS],nBJets,_ww); 


  //Reco mW from jj in Z events w/o btag
  /*
  if(hasZWindow(*leptons,MZ-10, MZ+10) && nSigJet==2 
     &&nBJets==0){
    TLorentzVector _jj = (*jets->at(0)) + (*jets->at(1));
    _hh->H1FILL(_hh->DG2L_mjj[iSR][m_ET][iSYS],_jj.M(),_ww); 
  }
  */
  if(nSigJet>=2){
    TLorentzVector _jj = (*jets->at(0)) + (*jets->at(1));
    _hh->H1FILL(_hh->DG2L_mjj[iSR][m_ET][iSYS],_jj.M(),_ww); 
  }

  int nSoftJets=0;
  for(uint ijet=0; ijet<v_baseJet->size(); ijet++){
    const Susy::Jet* _j = v_baseJet->at(ijet);
    if(isSignalJet2Lep(_j)) continue;
    nSoftJets++;
    if(ijet==0){
      _hh->H1FILL(_hh->DG2L_ptSj1[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etaSj1[iSR][m_ET][iSYS],_j->Eta(),_ww); 
      _hh->H1FILL(_hh->DG2L_jvfSj1[iSR][m_ET][iSYS],_j->jvf,_ww); 
    }
    if(ijet==1){
      _hh->H1FILL(_hh->DG2L_ptSj2[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->DG2L_etaSj2[iSR][m_ET][iSYS],_j->Eta(),_ww); 
      _hh->H1FILL(_hh->DG2L_jvfSj2[iSR][m_ET][iSYS],_j->jvf,_ww); 
    }
  }
  _hh->H1FILL(_hh->DG2L_nSoftJets[iSR][m_ET][iSYS],nSoftJets,_ww); 


}

/*--------------------------------------------------------------------------------*/
// Debuging 
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
  cout << "N_C20 " <<  numberOfCLJets(*v_sigJet) 
       << " N_B20 " << numberOfCBJets(*v_sigJet)
       << " N_F30 " << numberOfFJets(*v_sigJet)
       << endl;
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
      if(validSystForHFT(i)){
	m_histFitterTrees[i] = new HistFitterTree(mcId,systName);
	cout << "Creating histFitterTree " << mcId << " " << systName << endl;
	HFTName = string(mcId.Data());
      }
      else m_histFitterTrees[i] = NULL;
    }
  }
  else{
    if(m_method == FLEP){ //DD Fake lepton
      TString ds(string(_hh->sampleName()+"_FAKE"));
      for(uint i=_sys1; i <= _sys2; i++) {
	if(validSystForHFT(i)){
	  systName.Form("%s",DG2LSystNames[i].c_str());
	  m_histFitterTrees[i] = new HistFitterTree(ds,systName);
	  HFTName = string(ds.Data());
	  cout << "Creating histFitterTree "  << ds << " " << systName << endl;
	}
	else m_histFitterTrees[i] = NULL;
      }
    }
    else{ //Data - 1 tree
      TString ds(_hh->sampleName());
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
float Susy2LepAna::writeIntoHistFitterTree(uint iSR,
					   LeptonVector* leptons, 
					   const LeptonVector* baseLeptons, 
					   const JetVector* signalJets, 
					   const JetVector* baseJets,
					   const Met* met)
{
  //Save only events pass SR's or CR's cuts
  if(iSR==DIL_CRZ || iSR==DIL_CRZjveto || 
     iSR==DIL_CR2LepOS || iSR==DIL_CR2LepSS || iSR==DIL_CR2LepSS40 ||
     iSR==DIL_preSROSjveto || iSR==DIL_preSRmT2 || 
     iSR==DIL_preSR2jets || iSR==DIL_preSRZjets || iSR==DIL_preSRSS)
    return -1;

  // Find which event type
  DiLepEvtType evtType;
  if(NBASELEPMAX==2) evtType = getDiLepEvtType(*baseLeptons);
  else               evtType = getDiLepEvtType(*leptons);
    
  // Form the Z-candidate
  TLorentzVector Zcandidate = *leptons->at(0);
  Zcandidate += *leptons->at(1);
  
  float histFitWeight = 1; //pileup, mc event weight , nomalized to sample size (ie 1/pb)
  float totalWeight   = 1; //full event weight

  bool  _isData = false;
  bool  _isOS   = leptons->at(0)->q * leptons->at(1)->q < 0 ? true:false;
  bool  _isEE   =  evtType==ET_ee ? true:false;
  bool  _isEM   = (evtType==ET_em || evtType==ET_me) ? true:false;
  bool  _isMM   = evtType==ET_mm ? true:false;
  int   _nC20   = numberOfCLJets(*signalJets);
  int   _nB20   = numberOfCBJets(*signalJets);
  int   _nF30   = numberOfFJets(*signalJets);
  float _ptl1   = leptons->at(0)->Pt();
  float _ptl2   = leptons->at(1)->Pt();
  float _ZPt    = Zcandidate.Pt();
  float _metET  = met->lv().Pt();
  float _metRel = getMetRel(met,*leptons,*signalJets);
  float _Mll    = Mll(leptons->at(0), leptons->at(1));
  float _MT2    = getMT2(*leptons, met);
  bool _topTag  = passTopTag(*leptons,*signalJets,met);
  float _dPhill = leptons->at(0)->DeltaPhi(*leptons->at(1));
  float _mWWT   = mT(Zcandidate, met->lv());
  float _j0Pt   = (signalJets->size()>0) ? signalJets->at(0)->Pt() : 0.;
  float _j1Pt   = (signalJets->size()>1) ? signalJets->at(1)->Pt() : 0.;
  float _mjj    = (signalJets->size()>1) ? Mll(signalJets->at(0),signalJets->at(1)) : 0;

  //
  // Various weights for systematics saved with NOM syst
  //
  // All these are the ratio new_sys/nom_sys
  float _bJet_up       = 1; 
  float _bJet_dn       = 1;  
  float _cJet_up       = 1; 
  float _cJet_dn       = 1;  
  float _lJet_up       = 1; 
  float _lJet_dn       = 1;  
  float _pdf_up        = 1; 
  float _pdf_dn        = 1;  
  float _xs_up         = 1;
  float _xs_dn         = 1;
  float _eTrig_up      = 1;  
  float _eTrig_dn      = 1; 
  float _mTrig_up      = 1;  
  float _mTrig_dn      = 1;  
  float _bkg_up        = 1;
  float _bkg_dn        = 1;
  float _fake_el_re_up = 1;
  float _fake_el_re_dn = 1;
  float _fake_el_fr_up = 1;
  float _fake_el_fr_dn = 1;
  float _fake_mu_re_up = 1;
  float _fake_mu_re_dn = 1;
  float _fake_mu_fr_up = 1;
  float _fake_mu_fr_dn = 1;
  float _el_SF_up      = 1;
  float _el_SF_dn      = 1;
  float _mu_EFF_up     = 1;
  float _mu_EFF_dn     = 1;
  //Weight qFlip - not ratio !!
  float _qFlipWeight   = 1;


  if(nt->evt()->isMC){
    histFitWeight  = nt->evt()->w * nt->evt()->wPileup / nt->evt()->sumw;
    //Nominal values
    float _xs     = getCrossSection(sigXsfile,nt->evt()->mcChannel,nt->evt()->xsec,DGSys_NOM);
    float w_l1_sf = leptons->at(0)->effSF;
    float w_l2_sf = leptons->at(1)->effSF;
    float w_trig  = getTriggerWeight(leptons, _metET, signalJets->size(), nt->evt()->nVtx, DGSys_NOM);
    float w_btag  = getBTagSF(nt->evt(),baseJets, DGSys_NOM);

    //Obtain the weights for each sys variation and take ratio to nominal
    _xs_up = getCrossSection(sigXsfile,nt->evt()->mcChannel,nt->evt()->xsec,1)  / _xs;
    _xs_dn = getCrossSection(sigXsfile,nt->evt()->mcChannel,nt->evt()->xsec,-1) / _xs;
    
    _eTrig_up = getTriggerWeight(leptons, _metET, signalJets->size(), nt->evt()->nVtx, DGSys_TRIGSF_EL_UP) / w_trig;
    _eTrig_dn = getTriggerWeight(leptons, _metET, signalJets->size(), nt->evt()->nVtx, DGSys_TRIGSF_EL_DN) / w_trig;
    _mTrig_up = getTriggerWeight(leptons, _metET, signalJets->size(), nt->evt()->nVtx, DGSys_TRIGSF_MU_UP) / w_trig;
    _mTrig_dn = getTriggerWeight(leptons, _metET, signalJets->size(), nt->evt()->nVtx, DGSys_TRIGSF_MU_DN) / w_trig;

    _bJet_up = getBTagSF(nt->evt(),baseJets, DGSys_BJet_UP) / w_btag;
    _bJet_dn = getBTagSF(nt->evt(),baseJets, DGSys_BJet_DN) / w_btag;
    _cJet_up = getBTagSF(nt->evt(),baseJets, DGSys_CJet_UP) / w_btag;
    _cJet_dn = getBTagSF(nt->evt(),baseJets, DGSys_CJet_DN) / w_btag;
    _lJet_up = getBTagSF(nt->evt(),baseJets, DGSys_LJet_UP) / w_btag;
    _lJet_dn = getBTagSF(nt->evt(),baseJets, DGSys_LJet_DN) / w_btag;

    //TO DO add lep SF error;

    totalWeight   = histFitWeight * w_l1_sf * w_l2_sf * w_trig * w_btag * _xs * pLUMI*1000.;

    if(dbg()>10){
      cout << "HFTw " << nt->evt()->w 
	   << " pileup  " << nt->evt()->wPileup 
	   << " sumw " << nt->evt()->sumw<<endl;
      cout << " l SF " << w_l1_sf * w_l2_sf 
	   << " trig " << w_trig 
	   << " btag " << w_btag 
	   << " xs " <<  _xs 
	   << " lumi " << pLUMI*1000.
	   << " totW " << totalWeight << "\n" << endl;
    }
    _isData=false;

    
    //Get the QFlip weight. Not the event kinematics won't be smeared.
    if(m_method == RLEP &&  m_ET!=ET_mm){
      if(!isGenuineSS(leptons) && (_isEE || _isEM) ){ //Not true SS - use OS * qFlip
	_qFlipWeight = getQFlipProb(leptons,&new_met,DGSys_NOM); //lept pt/Met changed !!!!
	restoreOriginal(*leptons,met);                           //Because qFlip was have been called
	_bkg_up = getQFlipProb(leptons,&new_met,DGSys_BKGMETHOD_UP) / _qFlipWeight; //lept pt/Met changed !!!!
	restoreOriginal(*leptons,met);                           //Because qFlip was have been called
	_bkg_dn = getQFlipProb(leptons,&new_met,DGSys_BKGMETHOD_DN) / _qFlipWeight; //lept pt/Met changed !!!!
	restoreOriginal(*leptons,met);                           //Because qFlip was have been called
      }
    }
  }
  else{
    _isData=true;
    float w_fake = getFakeWeight(leptons,nt->evt()->nVtx,nt->evt()->isMC,iSR,_metRel,DGSys_NOM);
    
    _fake_el_re_up = getFakeWeight(leptons,nt->evt()->nVtx,nt->evt()->isMC,iSR,_metRel,DGSys_FAKE_EL_RE_UP) / w_fake;
    _fake_el_re_dn = getFakeWeight(leptons,nt->evt()->nVtx,nt->evt()->isMC,iSR,_metRel,DGSys_FAKE_EL_RE_DN) / w_fake;
    _fake_el_fr_up = getFakeWeight(leptons,nt->evt()->nVtx,nt->evt()->isMC,iSR,_metRel,DGSys_FAKE_EL_FR_UP) / w_fake;
    _fake_el_fr_dn = getFakeWeight(leptons,nt->evt()->nVtx,nt->evt()->isMC,iSR,_metRel,DGSys_FAKE_EL_FR_DN) / w_fake;
    _fake_mu_re_up = getFakeWeight(leptons,nt->evt()->nVtx,nt->evt()->isMC,iSR,_metRel,DGSys_FAKE_MU_RE_UP) / w_fake;
    _fake_mu_re_dn = getFakeWeight(leptons,nt->evt()->nVtx,nt->evt()->isMC,iSR,_metRel,DGSys_FAKE_MU_RE_DN) / w_fake;
    _fake_mu_fr_up = getFakeWeight(leptons,nt->evt()->nVtx,nt->evt()->isMC,iSR,_metRel,DGSys_FAKE_MU_FR_UP) / w_fake;
    _fake_mu_fr_dn = getFakeWeight(leptons,nt->evt()->nVtx,nt->evt()->isMC,iSR,_metRel,DGSys_FAKE_MU_FR_DN) / w_fake;
    
    totalWeight = histFitWeight = w_fake;
  }


  //Resolve overlap CRWWa overlap with ZXCRmt2
  if(iSR==DIL_CRWWa){
    bool _inZ = (fabs(_Mll-MZ)<10) ? true : false;
    if(_isOS && _inZ && (_nC20+_nB20+_nF30)==0 && _metRel>40 && _MT2>90)
      return 0; //event also in ZXCRmt2, don't duplicate
  }
  

  // Store everything in MeV
  static const float MEV = 1000; 
  m_histFitterTrees[SYST]->lept1Pt             = _ptl1*MEV;
  m_histFitterTrees[SYST]->lept2Pt             = _ptl2*MEV;
  m_histFitterTrees[SYST]->MET                 = _metET*MEV;
  m_histFitterTrees[SYST]->nSigLep             =  2;
  m_histFitterTrees[SYST]->L2nCentralLightJets = _nC20;
  m_histFitterTrees[SYST]->L2nForwardJets      = _nF30;
  m_histFitterTrees[SYST]->L2nCentralBJets     = _nB20;
  m_histFitterTrees[SYST]->L2METrel            = _metRel*MEV;
  m_histFitterTrees[SYST]->L2Mll               = _Mll*MEV;
  m_histFitterTrees[SYST]->MT2                 = _MT2*MEV; 
  m_histFitterTrees[SYST]->L2dileptonpt        = _ZPt*MEV;
  m_histFitterTrees[SYST]->isOS                = _isOS;
  m_histFitterTrees[SYST]->L2TopVeto           = _topTag;
  m_histFitterTrees[SYST]->isEMU               = _isEM;
  m_histFitterTrees[SYST]->isMUMU              = _isMM;
  m_histFitterTrees[SYST]->isEE                = _isEE;
  m_histFitterTrees[SYST]->L2dPhiLL            = _dPhill;
  m_histFitterTrees[SYST]->L2mTmetLL           = _mWWT*MEV;
  m_histFitterTrees[SYST]->L2cjet1pt           = _j0Pt*MEV;
  m_histFitterTrees[SYST]->L2cjet2pt           = _j1Pt*MEV;
  m_histFitterTrees[SYST]->L2mJJ               = _mjj*MEV;

  m_histFitterTrees[SYST]->L2isData            = _isData;

  m_histFitterTrees[SYST]->syst_ETRIGREWUP     = _eTrig_up;
  m_histFitterTrees[SYST]->syst_ETRIGREWDOWN   = _eTrig_dn;
  m_histFitterTrees[SYST]->syst_MTRIGREWUP     = _mTrig_up;
  m_histFitterTrees[SYST]->syst_MTRIGREWDOWN   = _mTrig_dn;
  m_histFitterTrees[SYST]->syst_ESFUP          = _el_SF_up;
  m_histFitterTrees[SYST]->syst_ESFDOWN        = _el_SF_dn;
  m_histFitterTrees[SYST]->syst_MEFFUP         = _mu_EFF_up;
  m_histFitterTrees[SYST]->syst_MEFFDOWN       = _mu_EFF_dn;
  m_histFitterTrees[SYST]->syst_BJETUP         = _bJet_up;
  m_histFitterTrees[SYST]->syst_BJETDOWN       = _bJet_dn;
  m_histFitterTrees[SYST]->syst_CJETUP         = _cJet_up;
  m_histFitterTrees[SYST]->syst_CJETDOWN       = _cJet_dn;
  m_histFitterTrees[SYST]->syst_BMISTAGUP      = _lJet_up;
  m_histFitterTrees[SYST]->syst_BMISTAGDOWN    = _lJet_dn;
  m_histFitterTrees[SYST]->syst_XSUP           = _xs_up;
  m_histFitterTrees[SYST]->syst_XSDOWN         = _xs_dn;
  m_histFitterTrees[SYST]->syst_PDFERRUP       = _pdf_up;
  m_histFitterTrees[SYST]->syst_PDFERRDOWN     = _pdf_dn;
  m_histFitterTrees[SYST]->syst_BKGMETHODUP    = _bkg_up;
  m_histFitterTrees[SYST]->syst_BKGMETHODDOWN  = _bkg_dn;
  m_histFitterTrees[SYST]->syst_ELFAKE_RE_UP   = _fake_el_re_up;  
  m_histFitterTrees[SYST]->syst_ELFAKE_RE_DOWN = _fake_el_re_dn;  
  m_histFitterTrees[SYST]->syst_ELFAKE_FR_UP   = _fake_el_fr_up;  
  m_histFitterTrees[SYST]->syst_ELFAKE_FR_DOWN = _fake_el_fr_dn;  
  m_histFitterTrees[SYST]->syst_MUFAKE_RE_UP   = _fake_mu_re_up;  
  m_histFitterTrees[SYST]->syst_MUFAKE_RE_DOWN = _fake_mu_re_dn;  
  m_histFitterTrees[SYST]->syst_MUFAKE_FR_UP   = _fake_mu_fr_up;  
  m_histFitterTrees[SYST]->syst_MUFAKE_FR_DOWN = _fake_mu_fr_dn;  

  m_histFitterTrees[SYST]->L2finalState        = nt->evt()->susyFinalState;
  m_histFitterTrees[SYST]->L2qFlipWeight       = _qFlipWeight;
  m_histFitterTrees[SYST]->eventweight         = histFitWeight;
  m_histFitterTrees[SYST]->L2TotalWeight       = totalWeight;
  
  // Write the tree
  m_histFitterTrees[SYST]->WriteTree();
  
  return totalWeight;
}
/*--------------------------------------------------------------------------------*/
// Check if this looping over a syst that get save into a new HFT
// All the ones below, get saved as weight.
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::validSystForHFT(uint iSR)
{
  if( (iSR==DGSys_BJet_DN ||iSR==DGSys_CJet_DN ||iSR==DGSys_LJet_DN ||
       iSR==DGSys_BJet_UP ||iSR==DGSys_CJet_UP ||iSR==DGSys_LJet_UP ||
       iSR==DGSys_PDF_UP || iSR==DGSys_PDF_DN ||
       iSR==DGSys_XS_UP || iSR==DGSys_XS_DN ||
       iSR==DGSys_TRIGSF_EL_UP || iSR==DGSys_TRIGSF_EL_DN ||
       iSR==DGSys_TRIGSF_MU_UP || iSR==DGSys_TRIGSF_MU_DN ||
       iSR==DGSys_BKGMETHOD_UP || iSR==DGSys_BKGMETHOD_DN ||
       iSR==DGSys_FAKE_EL_RE_UP || iSR==DGSys_FAKE_EL_RE_DN ||
       iSR==DGSys_FAKE_EL_FR_UP || iSR==DGSys_FAKE_EL_FR_DN ||
       iSR==DGSys_FAKE_MU_RE_UP || iSR==DGSys_FAKE_MU_RE_DN ||
       iSR==DGSys_FAKE_MU_FR_UP || iSR==DGSys_FAKE_MU_FR_DN)) return false;

    return true;
}

/*--------------------------------------------------------------------------------*/
// Initialize ToyNt
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::initializeToyNt()
{
  if(m_writeToyNt==true) return; //Already initialized

  // Set the writeToyNt flag
  m_writeToyNt=true;
  cout << " ===> Initialising ToyNt Outputs " << endl;
  
  if(nt->evt()->isMC){
    // Read MC ID, only run over one unique DS while using this option!!!
    TString mcId = "";
    mcId.Form("%i",nt->evt()->mcChannel);
    m_toyNt = new ToyNt(mcId,TString(TOYNT_SR));
  }
  else{
    if(m_method == FLEP){ //DD Fake lepton
      TString ds(string(_hh->sampleName()+"_FAKE"));
      m_toyNt = new ToyNt(ds,TString(TOYNT_SR));
    }
    else{ //Data 
      TString ds(string(_hh->sampleName()+"_DATA"));
      m_toyNt = new ToyNt(ds,TString(TOYNT_SR));
    }
  }
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::fillToyNt(uint iSR,uint iSYS,
				 const LeptonVector* leptons, 
				 const JetVector* jets,
				 const Met* met,
				 float _ww)
{
  float corrNpv = nt->evt()->nVtx;
  if(nt->evt()->isMC) corrNpv = GetNVertexBsCorrected(nt->evt()->nVtx);
  
  m_toyNt->FillTreeEvent(nt->evt()->run,
			 nt->evt()->event,
			 nt->evt()->nVtx,
			 corrNpv,
			 iSR,
			 m_ET,
			 _ww);	 
  m_toyNt->FillTreeLeptons(leptons,*v_baseEle,*v_baseMu,met,nt->evt()->nVtx,nt->evt()->isMC);
  m_toyNt->FillTreeMet(met,metRel,mT2);
  m_toyNt->FillTreeSignalJets(jets,leptons,met);
  m_toyNt->FillTreeOtherJets(v_baseJet,leptons,met);
  
  //Write entry to TTree
  m_toyNt->WriteTree();

}
