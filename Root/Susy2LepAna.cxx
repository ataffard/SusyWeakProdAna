#include <iomanip>
#include <iostream>
#include "TCanvas.h"
#include "SusyWeakProdAna/Susy2LepAna.h"
#include "SusyWeakProdAna/PhysicsTools.h"

#include "SusyWeakProdAna/SusyAnaCommon.h"
#include "SusyMatrixMethod/FakeRegions.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// Susy2LepAna Constructor
/*--------------------------------------------------------------------------------*/
Susy2LepAna::Susy2LepAna(SusyHistos* _histos):
  SusyBaseAna(_histos),
  m_writeHFT(false)
{
  cout << "-------------------------------" << endl;
  cout << "    Running 2L Analysis        " << endl;
  cout << "-------------------------------" << endl;
}

/*--------------------------------------------------------------------------------*/
// Main process loop function 
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::doAnalysis(float w, unsigned int isys)
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

  if(FILL_HFT && !m_writeHFT) initializeHistFitterTree();
  if(FILL_TOYNT && !m_writeToyNt && isys==DGSys_NOM){
    bool metDetails      = false;
    bool dijetBlock      = true;
    bool OS2LBlock       = true;
    bool SS2LBlock       = true;
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
void Susy2LepAna::end()
{
  cout << endl;
  cout << endl;
  cout << "**********************************************" << endl;
  cout << "Susy2LepAna::event counters" <<endl;
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

  cout << "Channels        " << DIL_FLAV[0] << "\t " << DIL_FLAV[1] << "\t " << DIL_FLAV[2] <<endl;
  print_line("pass category ",n_pass_dil[0], n_pass_dil[1], n_pass_dil[2]);
  print_line("pass nLep     ",n_pass_signalLep[0], n_pass_signalLep[1], n_pass_signalLep[2]);
  print_line("pass tau veto ",n_pass_tauVeto[0], n_pass_tauVeto[1], n_pass_tauVeto[2]);
  print_line("pass trig     ",n_pass_trig[0],n_pass_trig[1],n_pass_trig[2]);
  print_line("pass truth    ",n_pass_truth[0],n_pass_truth[1],n_pass_truth[2]);
  print_SRmT2();
  print_SRWW();
  print_SRZjets();
  print_SRSSjets();

  print_WWCR();
  print_TOPCR();
  print_ZVCR();
  print_VRSS();

  print_CRZ();
  cout << std::endl << std::endl; 

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
  
  finish();

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
      systName.Form("%s",DGSystNames[i].c_str());
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
	systName.Form("%s",DGSystNames[i].c_str());
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
// Set cut selection
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::setSelection(std::string s, uint dilType)
{
  std::string m_sel = s;
  resetCuts();
 
  //----------------------------//
  // Signal Regions
  //----------------------------//
  if(m_sel      == "SRmT2a"){
    m_selOS     = true;
    if(dilType==ET_ee || dilType==ET_mm){
      m_vetoZ   = true;
    }
    m_vetoJ     = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_mt2Min    = 90;
  }
  else if(m_sel == "SRmT2b"){
    m_selOS     = true;
    if(dilType==ET_ee || dilType==ET_mm){
      m_vetoZ   = true;
    }
    m_vetoJ     = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_mt2Min    = 120;
  }  
  else if(m_sel == "SRmT2c"){
    m_selOS     = true;
    if(dilType==ET_ee || dilType==ET_mm){
      m_vetoZ   = true;
    }
    m_vetoJ     = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_mt2Min    = 150;
  }  
  else if(m_sel == "SRWWa"){
    m_selOS     = true;
    if(dilType==ET_ee|| dilType==ET_mm){
      m_vetoZ   = true;
    }
    m_vetoJ     = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_metRelMin = 80;
    m_pTllMin   = 80;
    m_highMll   = 120;

  }
  else if(m_sel == "SRWWb"){
    m_selOS     = true;
    if(dilType==ET_ee|| dilType==ET_mm){
      m_vetoZ   = true;
    }
    m_vetoJ     = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_highMll   = 170;
    m_mt2Min    = 90;
  }
  else if(m_sel == "SRWWc"){
    m_selOS     = true;
    if(dilType==ET_ee|| dilType==ET_mm){
      m_vetoZ   = true;
    }
    m_vetoJ     = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_mt2Min    = 100;
  }
  else if(m_sel == "SRZjets"){
    m_selOS     = true;
    m_selSF     = true;
    m_selZ      = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_vetoF     = true;
    m_vetoB     = true;
    m_minC20    = 2;
    m_pTj0Min   = 45;
    m_pTj1Min   = 45;
    m_lowMjj    = 50;
    m_highMjj   = 100;
    m_pTllMin   = 80;
    m_dRllMin   = 0.3;
    m_dRllMax   = 1.5;
    m_metRelMin = 80;
  }
  else if(m_sel == "SRZjetsb"){
    m_selOS     = true;
    m_selSF     = true;
    m_selZ      = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_vetoF     = true;
    m_vetoB     = true;
    m_minC20    = 2;
    m_pTj0Min   = 40;
    m_pTj1Min   = 25;
    m_lowMjj    = 50;
    m_highMjj   = 100;

    m_mt2Min    = 100;
    m_metRelMin = 50;
    m_MeffMin   = 250;
 }


  else if(m_sel == "SRSSjets"){
    m_selSS     = true;
    m_vetoB     = true;
    m_vetoF     = true;
    m_minC20    = 1;
    if(dilType==ET_ee){
      m_mllIn     = true;
      m_lowMll    = MZ-10;
      m_highMll   = MZ+10;
      m_highMTWW  = 70; 
      m_metRelMin = 70;
    }
    else if(dilType==ET_mm){
      m_pTl0Min   = 15;
      m_pTl1Min   = 15;
      m_highMTWW  = 50; 
      m_metRelMin = 50;
      m_mt2Max    = 30;
      m_pTllMax   = 20;
      m_pTj0Min   = 25;
    }
    else if(dilType==ET_em){
      m_highMTWW  = 50; 
      m_metRelMin = 50;
    }
  }  


  //----------------------------//
  // Control regions
  //----------------------------//
  else if(m_sel == "CRWWmet"){
    m_selOS     = true;
    m_vetoJ     = true;
    if(dilType==ET_ee || dilType==ET_mm){
      m_vetoZ   = true;
    }
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_metRelMin = 60;
    m_metRelMax = 80;
    m_pTllMin   = 40;
    m_highMll   = 120;
  }
  else if(m_sel == "CRWWmt2"){
    m_selOS     = true;
    m_vetoJ     = true;
    if(dilType==ET_ee || dilType==ET_mm){
      m_vetoZ   = true;
    }
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_mt2Min    = 50;
    m_mt2Max    = 90;
  }
  else if(m_sel == "CRTOPmet"){//emm only used
    m_selOS     = true;
    if(dilType==ET_ee || dilType==ET_mm){
      m_vetoZ   = true;
    }
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_selB      = true;
    m_maxC20    = 0;
    m_vetoF     = true;
    m_metRelMin = 80;
    m_pTllMin   = 80;
    m_highMll   = 120;
  }
  else if(m_sel == "CRTOPmt2"){//em only used
    m_selOS     = true;
    if(dilType==ET_ee || dilType==ET_mm){
      m_vetoZ   = true;
    }
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_selB      = true;
    m_maxC20    = 0;
    m_vetoF     = true;
    m_mt2Min    = 70;
  }
  else if(m_sel == "CRZVmet"){
    m_selOS     = true;
    m_selSF     = true;
    m_selZ      = true;
    m_vetoJ     = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_metRelMin = 80;
    m_pTllMin   = 80;
  }
  else if(m_sel == "CRZVmt2a"){
    m_selOS     = true;
    m_selSF     = true;
    m_selZ      = true;
    m_vetoJ     = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_mt2Min    = 90;
  }
  else if(m_sel == "CRZVmt2b"){
    m_selOS     = true;
    m_selSF     = true;
    m_selZ      = true;
    m_vetoJ     = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_mt2Min    = 100;
  }
  else if(m_sel == "CRZVmt2c"){
    m_selOS     = true;
    m_selSF     = true;
    m_selZ      = true;
    m_vetoJ     = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_mt2Min    = 120;
  }
  else if(m_sel == "CRZVmt2d"){
    m_selOS     = true;
    m_selSF     = true;
    m_selZ      = true;
    m_vetoJ     = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_mt2Min    = 150;
  }
  else if(m_sel == "CRTOPZjets"){
    m_selOS     = true;
    m_selSF     = true;
    m_vetoZ     = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    //m_minCJet   = 2; //B20+C20 Updated 07/11 v3 cutflow
    m_selB      = true;
    m_vetoF     = true; 
    m_pTllMin   = 80;
    m_dRllMin   = 0.3;
    m_dRllMax   = 1.5;
    m_metRelMin = 80;
  }
  else if(m_sel == "CRZVZjets"){
    m_selOS     = true;
    m_selSF     = true;
    m_selZ      = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_vetoF     = true;
    m_vetoB     = true;
    m_minC20    = 2;
    m_pTj0Min   = 35;
    m_pTj1Min   = 25;
    m_dRllMin   = 1.5;
    m_dRllMax   = 4.0;
    m_metRelMin = 80;
    m_pTllBound = true;
  }
  else if(m_sel == "CRZVZjets1"){
    m_selOS     = true;
    m_selSF     = true;
    m_selZ      = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_vetoF     = true;
    m_vetoB     = true;
    m_minC20    = 2;

    m_mt2Min     = 100;
    m_MetMeffMin = 0.45;
    m_metRelMin  = 40;
  }
  else if(m_sel == "CRZVZjets2"){
    m_selOS     = true;
    m_selSF     = true;
    m_selZ      = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_vetoF     = true;
    m_vetoB     = true;
    m_minC20    = 2;

    m_mt2Min     = 100;
    m_metRelMin  = 80;
  }
  else if(m_sel == "CRZVZjets3"){
    m_selOS     = true;
    m_selSF     = true;
    m_selZ      = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_vetoF     = true;
    m_vetoB     = true;
    m_minC20    = 2;

    m_mt2Min      = 100;
    m_metRelMin   = 40;
    m_dPhillJ0Max = 2.0;
  }
  else if(m_sel == "CRZVZjets4"){
    m_selOS     = true;
    m_selSF     = true;
    m_selZ      = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_vetoF     = true;
    m_vetoB     = true;
    m_minC20    = 2;

    m_dRllMin   = 1.5;
    m_dPhillJ0Max = 2.0;
    m_mt2Min      = 90;
  }
 else if(m_sel == "CRZjetsTemplate"){
    m_selOS     = true;
    m_selSF     = true;
    m_selZ      = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_vetoF     = true;
    m_vetoB     = true;
    m_minC20    = 2;
    m_pTj0Min   = 45;
    m_pTj1Min   = 45;
    m_lowMjj    = 50;
    m_highMjj   = 100;
    m_pTllMin   = 80;
    m_dRllMin   = 1.5;
  }
  
  //----------------------------//
  // SS Validation regions  
  //----------------------------//
  else if(m_sel == "VRSS"){
    m_selSS = true;
    if(dilType==ET_ee){
      m_mllIn     = true;
      m_lowMll    = MZ-10;
      m_highMll   = MZ+10;
    }
    m_metRelMin = 40;
  }
  
  //----------------------------//
  // Loose Selections
  //----------------------------//
  else if(m_sel == "CRZ"){
    m_selOS     = true;
    m_selZ      = true;
  }
  else if(m_sel == "CRZjets"){
    m_selOS     = true;
    m_selSF     = true;
    m_selZ      = true;
    m_vetoF     = true;
    m_vetoB     = true;
    m_minC20    = 2;
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
  else if(m_sel == "preSROSjveto"){ 
    m_selOS     = true;
    if(dilType==ET_ee || dilType==ET_mm){
      m_vetoZ   = true;
    }
    m_vetoJ     = true;
  }
  else if(m_sel == "preSRmT2"){ //SRmT2 - no Mt2
    m_selOS      = true;
    if(dilType==ET_ee || dilType==ET_mm){
      m_vetoZ    = true;
    }
    m_vetoJ     = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
    m_metRelMin = 40;
  }
  else if(m_sel == "preSRZjets"){//SRZjets no metRel,pTll,dRll
    m_selOS     = true;
    m_selSF     = true;
    m_selZ      = true;
    m_vetoF     = true;
    m_vetoB     =  true;
    m_minC20    = 2;
    m_pTj0Min   = 45;
    m_pTj1Min   = 45;
    m_lowMjj    = 50;
    m_highMjj   = 100;
  }
  else if(m_sel == "preSRWW"){//SRWW, no metrel, pTll, mll
    m_selOS     = true;
    if(dilType==ET_ee|| dilType==ET_mm){
      m_vetoZ   = true;
    }
    m_vetoJ     = true;
    m_pTl0Min   = 35;
    m_pTl1Min   = 20;
  }
  else if(m_sel == "preSRSS"){ //SRSSjets metRel40, no mTWW,mt2
    m_selSS = true;
    m_vetoB = true;
    m_vetoF = true;
    m_minC20 = 1;
    if(dilType==ET_ee){
      m_mllIn     = true;
      m_lowMll    = MZ-10;
      m_highMll   = MZ+10;
      m_metRelMin = 70;
    }
    else if(dilType==ET_mm){
      m_pTl0Min   = 15;
      m_pTl1Min   = 15;
      m_pTj0Min   = 25;
      m_metRelMin = 50;
    }
    else if(dilType==ET_em){
      m_metRelMin = 50;
    }
  }
  
  
  //----------------------------//
  // Optimisation  Regions
  // Very loose selection for ToyNt making
  //----------------------------//
  if(m_sel == "optimSRZjets"){ //top optim SRZjets
    m_selOS = true;
    m_selSF = true;
    m_selZ  = true;
    m_vetoF = true;
    m_vetoB = true;
    m_minC20 = 2;
  }
  else if(m_sel == "optimSRjets"){//to optim WH >=1 jets, SRZjets, SRjets
    m_selOS = true;
    m_vetoF = true;
    m_vetoB = true;
    m_minC20 = 1;
  }
  else if(m_sel == "optimSRSS"){//top optim SS channels
    m_selSS = true;
  }
  else if(m_sel == "optimSR0jet"){
    m_selOS = true;
    //m_vetoZ = true;
    if(dilType==ET_ee){
      m_mllIn     = true;
      m_lowMll    = MZ-10;
      m_highMll   = MZ+10;
    }
    m_vetoJ = true;
  }
  
}

/*--------------------------------------------------------------------------------*/
// Full event selection
/*--------------------------------------------------------------------------------*/
bool Susy2LepAna::selectEvent(LeptonVector* leptons, 
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
  m_ET =  getDiLepEvtType(*baseLeps);
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
  float _ww      = w;//eventWeight(LUMIMODE); 
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
	 << " SYST " << DGSystNames[SYST]
	 << " lepSF " << _lepSFW
	 << " trigW " << _trigW
	 << " bTag " << bTagWeight
	 << " weight(w/btag) " << _ww*bTagWeight << endl;
  }
 


  //
  //Loop over SR's & CR's
  //
  for(uint iSR=DIL_SRmT2a; iSR<DIL_NSR; iSR++){

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
    }
    
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    
    //Deal with Charge flip estimate - obtain approprate weighting
    if(!USE_QFLIP && !passQQ(leptons)) continue;
    float _ww_qFlip=1;
    if(USE_QFLIP){
      if( nt->evt()->isMC && m_method == RLEP &&  m_ET!=ET_mm &&
	  (iSR==DIL_SRSSjets || iSR==DIL_CR2LepSS || iSR==DIL_CR2LepSS40 || 
	   iSR==DIL_preSRSS  || iSR==DIL_VRSS || iSR==DIL_optimSRSS) ){
	if(isGenuineSS(leptons,nt->evt()->isMC) && SYST==DGSys_NOM )  n_pass_ss[m_ET][SR]+=_inc; //genuine SS - no qFlip
	else{ //OS ee/em event - get the qFlip prob
	  _ww_qFlip = getQFlipProb(leptons,&new_met,DGSys_NOM);
	  _ww *= _ww_qFlip;
	  if(WEIGHT_COUNT) _inc = _ww;
	  if(SYST==DGSys_NOM) n_pass_ss[m_ET][SR]+=_inc;
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
    }

    if(!passFullJetVeto(signalJets)){
      if(dbg()>5 && iSR==PRINT_SR) cout << "Fail Jveto " << nt->evt()->run << " " << nt->evt()->event << endl;
      continue;
    }
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(DUMP_RUNEVT && iSR==PRINT_SR){
      evtDump << DIL_FLAV[m_ET] << " " << nt->evt()->run  
	      << " " << nt->evt()->event 
	      << " nLJets " << numberOfCLJets(*signalJets,m_jvfTool, (SusyNtSys) DGSys_NOM, m_anaType)
	      << " nBJets " << numberOfCBJets(*signalJets) 
	      << " nFJets " << numberOfFJets(*signalJets) 
	      << endl;
    }

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

    if(!passPtllBound(leptons) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passdPhill(leptons) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passdRll(leptons) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passMWWT(leptons,&new_met) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);


    if(!passDPhillJ0(leptons,signalJets) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passMeff(signalJets, &new_met) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passMetMeff(leptons,signalJets, &new_met,false) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(!passTopTagger(leptons,signalJets,&new_met) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass toptagger " << sSR << endl;

    if(!passMETRel(&new_met,leptons,signalJets) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);
    if(dbg() >10 ) cout << "\t Pass MetRel " << sSR << endl;

    if(!passMT2(leptons, &new_met) ) continue;
    _hh->H1FILL(_hh->DG2L_cutflow[SR][m_ET][SYST],icut++,_ww);

    if(dbg() >10 ) cout << "\t Pass All " << sSR << endl;

    //
    // Fill histos, ToyNt and HFT 
    //
    if(DO_FILL_HISTO ){
      fillHistograms(SR,SYST,leptons, signalJets,&new_met,_ww);
      if(dbg() >10 ) cout << "\t Filled histos " << sSR << endl;
    }
    if(FILL_TOYNT && iSR==TOYNT_iSR && SYST==DGSys_NOM) 
      fillToyNt(SYST,leptons, signalJets,&new_met,_ww, bTagWeight,_ww_qFlip);

    if(m_writeHFT && validSystForHFT(SYST) ){
      float wHFT= writeIntoHistFitterTree(iSR,leptons,baseLeps,signalJets,v_baseJet,met);
      
      if(wHFT>0 && nt->evt()->isMC && SYST==DGSys_NOM &&
	 !(iSR==DIL_SRSSjets || iSR==DIL_VRSS)
	 && fabs(_ww-wHFT)/wHFT>0.000001) 
	cout << "WARNING >>> run " << nt->evt()->run  
	     << " event " << nt->evt()->event 
	     << " mismatch weight with HFT " << _ww << " " << wHFT << endl;
    }
    
    
    //
    // Debugging - Dump run event
    //
    //if(DUMP_RUNEVT && (iSR==DIL_CR2LepOS || iSR==DIL_CR2LepSS) ){
    //if(DUMP_RUNEVT && SYST==DGSys_NOM && (iSR==DIL_NTOP && m_ET==ET_ee) ){
    /*
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
    */
  }
  
  if(nt->evt()->isMC) restoreOriginal(*leptons,met);

  return true;
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
  /*
    ANYES 01-17-14  ---- NEED COMPATIBILITY w/ 2 MM packages
  SusyMatrixMethod::FAKE_REGION  frSR = SusyMatrixMethod::FR_SRmT2a;
  switch (iSR){
  case DIL_SRmT2a:
    frSR = SusyMatrixMethod::FR_SRmT2a;
    break;
  case DIL_SRmT2b:
    frSR = SusyMatrixMethod::FR_SRmT2b;
    break;
  case DIL_SRmT2c:
    frSR = SusyMatrixMethod::FR_SRmT2c;
    break;
  case DIL_SRZjets:
    frSR = SusyMatrixMethod::FR_SRZjets; 
    break;
  case DIL_SRZjetsb:
    frSR = SusyMatrixMethod::FR_SRZjets; 
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
  case DIL_SRSSjets:
    frSR = SusyMatrixMethod::FR_SRSSInc;
    break;
    
  case DIL_CRWWmet:
    frSR = SusyMatrixMethod::FR_CRWWMet;
    break;
  case DIL_CRWWmt2:
    frSR = SusyMatrixMethod::FR_CRWWmT2;
    break;
  case DIL_CRTOPmet:
    frSR = SusyMatrixMethod::FR_CRTopMet;
    break;
  case DIL_CRTOPmt2:
    frSR = SusyMatrixMethod::FR_CRTopmT2;
    break;
  case DIL_CRTOPZjets:
    frSR = SusyMatrixMethod::FR_CRTopZjets;
    break;
  case DIL_CRZVmet:
    frSR = SusyMatrixMethod::FR_CRZVMet;
    break;
  case DIL_CRZVmt2a:
    frSR = SusyMatrixMethod::FR_CRZVmT2_90;
    break;
  case DIL_CRZVmt2b:
    frSR = SusyMatrixMethod::FR_CRZVmT2_100;
    break;
  case DIL_CRZVmt2c:
    frSR = SusyMatrixMethod::FR_CRZVmT2_120;
    break;
  case DIL_CRZVmt2d:
    frSR = SusyMatrixMethod::FR_CRZVmT2_150;
    break;
  case DIL_CRZVZjets:
    frSR = SusyMatrixMethod::FR_CRZVMet;
    break;
  case DIL_CRZVZjets1:
    frSR = SusyMatrixMethod::FR_CRZVMet; //TEMP
    break;
  case DIL_CRZVZjets2:
    frSR = SusyMatrixMethod::FR_CRZVMet; //TEMP
    break;
  case DIL_CRZVZjets3:
    frSR = SusyMatrixMethod::FR_CRZVMet; //TEMP
    break;
  case DIL_CRZVZjets4:
    frSR = SusyMatrixMethod::FR_CRZVMet; //TEMP
    break;
  case DIL_VRSS:
    frSR = SusyMatrixMethod::FR_VRSS;
    break;  
    
    //No proper weihted average defined for these
  case DIL_CRZ:
    frSR = SusyMatrixMethod::FR_SRZjets;
    break;        
  case DIL_CRZjets:
    frSR = SusyMatrixMethod::FR_SRZjets;
    break;        
  case DIL_CRZjveto:
    frSR = SusyMatrixMethod::FR_SRmT2a;
    break;        
  case DIL_CR2LepOS:
    frSR = SusyMatrixMethod::FR_SRZjets; 
    break;
  case DIL_CR2LepSS:
    frSR = SusyMatrixMethod::FR_SRSSInc;
    break;
  case DIL_CR2LepSS40:
    frSR = SusyMatrixMethod::FR_VRSS;
    break;
  case DIL_preSROSjveto:
    frSR = SusyMatrixMethod::FR_SRmT2a;
    break;
  case DIL_preSRmT2:
    frSR = SusyMatrixMethod::FR_CRPremT2;
    break;
  case DIL_preSRWW:
    frSR = SusyMatrixMethod::FR_CRPremT2;
    break;
  case DIL_preSRZjets:
    frSR = SusyMatrixMethod::FR_SRZjets; 
    break;
  case DIL_preSRSS:
    frSR = SusyMatrixMethod::FR_VRSS;
    break;
  case DIL_optimSRZjets:
    frSR = SusyMatrixMethod::FR_SRZjets; 
    break;
  case DIL_optimSRjets:
    frSR = SusyMatrixMethod::FR_SRZjets; 
    break;
  case DIL_optimSRSS:
    frSR = SusyMatrixMethod::FR_SRSSInc; 
    break;
  case DIL_optimSR0jet:
    frSR = SusyMatrixMethod::FR_SRmT2a; 
    break;
  }
  */

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

  /*
  _fw = m_matrix_method.getTotalFake(_isSignal[0], _isEle[0], _pt[0],_eta[0],
				     _isSignal[1], _isEle[1], _pt[1],_eta[1],
				     frSR, metrel, 
				     (SusyMatrixMethod::SYSTEMATIC) iiSys);  
  
  if(dbg()>10) cout << "SR " << DIL_SRNAME[iSR] 
		    << " applying Ssys " << SYST 
		    << " " << DGSystNames[SYST] << " fw " << _fw << endl;
  */
  return _fw;
}

/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_SRmT2()
{
  int j= DIL_SRmT2a;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass Zveto  ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass Jveto  ",n_pass_FullJveto[0][j], n_pass_FullJveto[1][j], n_pass_FullJveto[2][j]);
  print_line("pass lepPt  ",n_pass_leadLepPt[0][j], n_pass_leadLepPt[1][j], n_pass_leadLepPt[2][j]);
  print_line("pass MT2(90)",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
  j= DIL_SRmT2b;
  print_line("pass MT2(120)",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
  j= DIL_SRmT2c;
  print_line("pass MT2(150)",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_SRWW()
{
  int j= DIL_SRWWa;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass ZVeto  ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass Jveto  ",n_pass_FullJveto[0][j], n_pass_FullJveto[1][j], n_pass_FullJveto[2][j]);
  print_line("pass lepPt  ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass Mll    ",n_pass_mll[0][j],n_pass_mll[1][j],n_pass_mll[2][j]);
  print_line("pass pTll   ",n_pass_pTll[0][j],n_pass_pTll[1][j],n_pass_pTll[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  
  j= DIL_SRWWb;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass Mll    ",n_pass_mll[0][j],n_pass_mll[1][j],n_pass_mll[2][j]);
  print_line("pass MT2    ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
  
  j= DIL_SRWWc;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass MT2    ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
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
  print_line("pass FVeto  ",n_pass_FJet[0][j], n_pass_FJet[1][j], n_pass_FJet[2][j]);
  print_line("pass bVeto  ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass 2Jets  ",n_pass_CJet[0][j], n_pass_CJet[1][j], n_pass_CJet[2][j]);
  print_line("pass JetsPt ",n_pass_JetPt[0][j],n_pass_JetPt[1][j],n_pass_JetPt[2][j]);
  print_line("pass Mjj    ",n_pass_mjj[0][j],n_pass_mjj[1][j],n_pass_mjj[2][j]);
  print_line("pass lepPt  ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass pTll   ",n_pass_pTll[0][j],n_pass_pTll[1][j],n_pass_pTll[2][j]);
  print_line("pass dRll   ",n_pass_dRll[0][j],n_pass_dRll[1][j],n_pass_dRll[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);

  j= DIL_SRZjetsb;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass 2Jets  ",n_pass_CJet[0][j], n_pass_CJet[1][j], n_pass_CJet[2][j]);
  print_line("pass JetsPt ",n_pass_JetPt[0][j],n_pass_JetPt[1][j],n_pass_JetPt[2][j]);
  print_line("pass Mjj    ",n_pass_mjj[0][j],n_pass_mjj[1][j],n_pass_mjj[2][j]);
  print_line("pass lepPt  ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass MEff   ",n_pass_Meff[0][j], n_pass_Meff[1][j], n_pass_Meff[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass MT2    ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
  
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_SRSSjets()
{
  int j= DIL_SRSSjets;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass SS     ",n_pass_ss[0][j], n_pass_ss[1][j], n_pass_ss[2][j]);
  print_line("pass FVeto  ",n_pass_FJet[0][j], n_pass_FJet[1][j], n_pass_FJet[2][j]);
  print_line("pass bVeto  ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass nJets  ",n_pass_CJet[0][j], n_pass_CJet[1][j], n_pass_CJet[2][j]);
  print_line("pass JetsPt ",n_pass_JetPt[0][j],n_pass_JetPt[1][j],n_pass_JetPt[2][j]);
  print_line("pass Mll    ",n_pass_mll[0][j],  n_pass_mll[1][j],  n_pass_mll[2][j]);
  print_line("pass pTll   ",n_pass_pTll[0][j],n_pass_pTll[1][j],n_pass_pTll[2][j]);
  print_line("pass mWWT   ",n_pass_mWWT[0][j], n_pass_mWWT[1][j], n_pass_mWWT[2][j]);  
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass MT2    ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_ZVCR()
{
  int j= DIL_CRZVmet;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass SF     ",n_pass_flav[0][j], n_pass_flav[1][j], n_pass_flav[2][j]);
  print_line("pass Z      ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass Jveto  ",n_pass_FullJveto[0][j], n_pass_FullJveto[1][j], n_pass_FullJveto[2][j]);
  print_line("pass lepPt  ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass pTll   ",n_pass_pTll[0][j],n_pass_pTll[1][j],n_pass_pTll[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);

  j= DIL_CRZVmt2a;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass lepPt     ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass MT2 (90)  ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
  j= DIL_CRZVmt2b;
  print_line("pass MT2 (100) ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
  j= DIL_CRZVmt2c;
  print_line("pass MT2 (120) ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
  j= DIL_CRZVmt2d;
  print_line("pass MT2 (150) ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);

  j= DIL_CRZVZjets;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass Z      ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass FVeto  ",n_pass_FJet[0][j], n_pass_FJet[1][j], n_pass_FJet[2][j]);
  print_line("pass bVeto  ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass 2Jets  ",n_pass_CJet[0][j], n_pass_CJet[1][j], n_pass_CJet[2][j]);
  print_line("pass JetsPt ",n_pass_JetPt[0][j],n_pass_JetPt[1][j],n_pass_JetPt[2][j]);
  print_line("pass lepPt    ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass pTll (2D) ",n_pass_pTllBound[0][j],n_pass_pTllBound[1][j],n_pass_pTllBound[2][j]);
  print_line("pass dRll   ",n_pass_dRll[0][j],n_pass_dRll[1][j],n_pass_dRll[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  

  j= DIL_CRZVZjets1;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass 2Jets  ",n_pass_CJet[0][j], n_pass_CJet[1][j], n_pass_CJet[2][j]);
  print_line("pass lepPt    ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass Met/MEff   ",n_pass_MetMeff[0][j], n_pass_MetMeff[1][j], n_pass_MetMeff[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass MT2 (100) ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
  
  j= DIL_CRZVZjets2;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass lepPt    ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
  print_line("pass MT2 (100) ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);

  j= DIL_CRZVZjets3;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass lepPt    ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass dPhi(ll,j0) ",n_pass_dPhillJ0[0][j], n_pass_dPhillJ0[1][j], n_pass_dPhillJ0[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);

  j= DIL_CRZVZjets4;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass dRll   ",n_pass_dRll[0][j],n_pass_dRll[1][j],n_pass_dRll[2][j]);
  print_line("pass dPhi(ll,j0) ",n_pass_dPhillJ0[0][j], n_pass_dPhillJ0[1][j], n_pass_dPhillJ0[2][j]);
  print_line("pass MT2 (90) ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);
}

/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_WWCR()
{
  int j= DIL_CRWWmet;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass ZVeto  ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass Jveto  ",n_pass_FullJveto[0][j], n_pass_FullJveto[1][j], n_pass_FullJveto[2][j]);
  print_line("pass lepPt  ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass Mll    ",n_pass_mll[0][j],n_pass_mll[1][j],n_pass_mll[2][j]);
  print_line("pass pTll   ",n_pass_pTll[0][j],n_pass_pTll[1][j],n_pass_pTll[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);

  j= DIL_CRWWmt2;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass lepPt  ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass MT2    ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);

}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_TOPCR()
{
  int j= DIL_CRTOPmet;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass Z      ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass FVeto  ",n_pass_FJet[0][j], n_pass_FJet[1][j], n_pass_FJet[2][j]);
  print_line("pass btag   ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass C20    ",n_pass_LJet[0][j], n_pass_LJet[1][j], n_pass_LJet[2][j]);
  print_line("pass lepPt  ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass Mll    ",n_pass_mll[0][j],  n_pass_mll[1][j],  n_pass_mll[2][j]);
  print_line("pass pTll   ",n_pass_pTll[0][j],n_pass_pTll[1][j],n_pass_pTll[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);

  j= DIL_CRTOPmt2;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass lepPt  ",n_pass_leadLepPt[0][j],n_pass_leadLepPt[1][j],n_pass_leadLepPt[2][j]);
  print_line("pass MT2    ",n_pass_mt2[0][j], n_pass_mt2[1][j], n_pass_mt2[2][j]);

  
  j= DIL_CRTOPZjets;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass OS     ",n_pass_os[0][j], n_pass_os[1][j], n_pass_os[2][j]);
  print_line("pass SF     ",n_pass_flav[0][j], n_pass_flav[1][j], n_pass_flav[2][j]);
  print_line("pass Z      ",n_pass_Z[0][j], n_pass_Z[1][j], n_pass_Z[2][j]);
  print_line("pass FVeto  ",n_pass_FJet[0][j], n_pass_FJet[1][j], n_pass_FJet[2][j]);
  print_line("pass btag   ",n_pass_BJet[0][j], n_pass_BJet[1][j], n_pass_BJet[2][j]);
  print_line("pass C20    ",n_pass_CJet[0][j], n_pass_CJet[1][j], n_pass_CJet[2][j]);
  print_line("pass pTll   ",n_pass_pTll[0][j], n_pass_pTll[1][j], n_pass_pTll[2][j]);
  print_line("pass dRll   ",n_pass_dRll[0][j], n_pass_dRll[1][j], n_pass_dRll[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
}
/*--------------------------------------------------------------------------------*/
void Susy2LepAna::print_VRSS()
{
  int j= DIL_VRSS;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << DIL_SRNAME[j] <<endl;
  print_line("pass SS     ",n_pass_ss[0][j], n_pass_ss[1][j], n_pass_ss[2][j]);
  print_line("pass Mll    ",n_pass_mll[0][j],n_pass_mll[1][j],n_pass_mll[2][j]);
  print_line("pass MetRel ",n_pass_metRel[0][j], n_pass_metRel[1][j], n_pass_metRel[2][j]);
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
  float dRll=999;
  dRll  = leptons->at(0)->DeltaR(*leptons->at(1));
  float dPhilMet=999; //min dPhi (l,MET)
  float mTl1=-999;
  float mTl2=-999;

  for(uint ilep=0; ilep<leptons->size(); ilep++){
    const Susy::Lepton* _l = leptons->at(ilep);
    _ll = _ll + (*_l);
    bool isChargeFlip =  _l->isEle() ? ((Electron*) _l)->isChargeFlip : false; 
    LEP_TYPE lType = getType(_l);
    /*->mcOrigin,
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
  float mT2 = getMT2(*leptons, met);
  float metRel = getMetRel(met,*leptons,*jets);
  float mct     = mCT(*leptons->at(0),*leptons->at(1));
  TLorentzVector recoil = -met->lv() - (*leptons->at(0)) - (*leptons->at(1));
  float mctPerp = mCTperp(*leptons->at(0),*leptons->at(1),recoil) ;

  _hh->H1FILL(_hh->DG2L_mll[iSR][m_ET][iSYS],_ll.M(),_ww); 
  _hh->H1FILL(_hh->DG2L_mllcoarse[iSR][m_ET][iSYS],_ll.M(),_ww); 
  _hh->H1FILL(_hh->DG2L_mllcoarser[iSR][m_ET][iSYS],_ll.M(),_ww); 
  _hh->H1FILL(_hh->DG2L_pTll[iSR][m_ET][iSYS],_ll.Pt(),_ww); 
  _hh->H1FILL(_hh->DG2L_mWWT[iSR][m_ET][iSYS],mWT,_ww); 
  _hh->H1FILL(_hh->DG2L_mTl1[iSR][m_ET][iSYS],mTl1,_ww);
  _hh->H1FILL(_hh->DG2L_mTl2[iSR][m_ET][iSYS],mTl2,_ww); 
  _hh->H1FILL(_hh->DG2L_dPhill[iSR][m_ET][iSYS],fabs(dPhill),_ww); 
  _hh->H1FILL(_hh->DG2L_dRll[iSR][m_ET][iSYS],dRll,_ww); 
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
  _hh->H1FILL(_hh->DG2L_mct[iSR][m_ET][iSYS],mct,_ww);
  _hh->H1FILL(_hh->DG2L_mctPerp[iSR][m_ET][iSYS],mctPerp,_ww); 


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
    if(isCentralLightJet(_j,m_jvfTool, (SusyNtSys) DGSys_NOM, m_anaType)) nSigCJet++;
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
  float metSig = mEff/met->lv().Pt();
  float mEffwLep = mEff + (*leptons->at(0)).Pt() + (*leptons->at(1)).Pt();
  float metSigwLep = mEffwLep/met->lv().Pt();

  dPhiJMet = TVector2::Phi_mpi_pi(dPhiJMet)*TMath::RadToDeg();  

  _hh->H1FILL(_hh->DG2L_ST[iSR][m_ET][iSYS],ST,_ww); 
  _hh->H1FILL(_hh->DG2L_mEff[iSR][m_ET][iSYS],mEff,_ww); 
  _hh->H1FILL(_hh->DG2L_metSig[iSR][m_ET][iSYS],metSig,_ww); 
  _hh->H1FILL(_hh->DG2L_mEffwLep[iSR][m_ET][iSYS],mEffwLep,_ww); 
  _hh->H1FILL(_hh->DG2L_metSigwLep[iSR][m_ET][iSYS],metSigwLep,_ww); 
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
      systName.Form("%s",DGSystNames[i].c_str());
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
	  systName.Form("%s",DGSystNames[i].c_str());
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
  if(iSR==DIL_CRZ || iSR==DIL_CRZjets || iSR==DIL_CRZjveto || 
     iSR==DIL_CR2LepOS || iSR==DIL_CR2LepSS || iSR==DIL_CR2LepSS40 ||
     iSR==DIL_preSROSjveto || iSR==DIL_preSRmT2 || 
     iSR==DIL_preSRZjets || iSR==DIL_preSRWW  || iSR==DIL_preSRSS ||
     iSR==DIL_optimSRZjets || iSR==DIL_optimSRjets || 
     iSR==DIL_optimSRSS || iSR==DIL_optimSR0jet
     )
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
  int   _nC20   = numberOfCLJets(*signalJets,m_jvfTool, (SusyNtSys) DGSys_NOM, m_anaType);
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
    float _xs     = 0;//getCrossSection(nt->evt()); //ANYES UPDATE 01-16-14
    float w_l1_sf = leptons->at(0)->effSF;
    float w_l2_sf = leptons->at(1)->effSF;
    float w_trig  = getTriggerWeight(leptons, _metET, signalJets->size(), nt->evt()->nVtx, DGSys_NOM);
    float w_btag  = getBTagSF(nt->evt(),baseJets, DGSys_NOM);

    //Obtain the weights for each sys variation and take ratio to nominal  // UPDATE Anyes 01-16-14
    _xs_up = 0;//getCrossSection(nt->evt())  / _xs; //ANYES UPDATE 01-16-14
    _xs_dn = 0;//getCrossSection(nt->evt())  / _xs; //ANYES UPDATE 01-16-14
    
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
      if(!isGenuineSS(leptons,nt->evt()->isMC) && (_isEE || _isEM) ){ //Not true SS - use OS * qFlip
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
  /*
  if(iSR==DIL_CRWWa){
    bool _inZ = (fabs(_Mll-MZ)<10) ? true : false;
    if(_isOS && _inZ && (_nC20+_nB20+_nF30)==0 && _metRel>40 && _MT2>90)
      return 0; //event also in ZXCRmt2, don't duplicate
  }
  */

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
  /*
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
  */

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

