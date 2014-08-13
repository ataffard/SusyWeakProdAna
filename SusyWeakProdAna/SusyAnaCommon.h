///
///  SusyAnaCommon
///  Contains default flags for running analyses
///

#ifndef SusyWeakProdAna_SusyAnaCommon_h
#define SusyWeakProdAna_SusyAnaCommon_h

#include <string>

#include "SusyWeakProdAna/Histos_2L.h"
#include "SusyWeakProdAna/Histos_WH.h"
#include "SusyWeakProdAna/Histos_LFV.h"

enum LUMISET {NOLUMI=0, LUMI13FB, LUMI_IJL, LUMI_E, LUMI21FB };

#define LUMI_21FB  //All 2012
const std::string TAG  = "070614";
const std::string SUFF = "_n0150_DD_LFV_v1"; 

const bool n0150BugFix=true;

//
//Normal mode - all these are true
//
//Pred Mode
//const unsigned int  NBASELEPMAX  = 3;  //number of base lepton allowed - 2 for std analysis
const unsigned int  NBASELEPMAX    = 2;  //number of base lepton allowed - 2 for std analysis
const unsigned int  NBASELEPMIN    = 2;  //number of base lepton allowed - 2 for std analysis

const bool   FILL_TOYNT        = true;         //Write ToyNt
const string TOYNT_SR          ="LFV_base";    //SR used to fill ToyNt
const unsigned int TOYNT_iSR   = LFV_base;     //SR used to fill ToyNt

const bool DO_SYS         = false;   //Toggles systematics looper
const bool FILL_HFT       = false;  //Write the HistFitterTree

const bool CUTFLOW        = false;  // require at least 2 tight/base lep SPEED ANA

const bool WEIGHT_COUNT   = true;   //Apply weight to dump count table - NLO weight always applied
const bool WEIGHT_ONE     = false; //Ignore evt->w use 1
const bool DO_FILL_HISTO  = true;

const bool USE_DGWEIGHT   = true;
const bool USE_BWEIGHT    = true;
const bool USE_LEPSF      = true;
const bool USE_QFLIP      = true;    //Toggled charge flip prob

const bool USE_MCWEIGHT   = false;   //Use this to apply only the mcWeight and then one another weight at a time
const bool USE_MCTRIG     = false;   //Use MC trigger. Set DGWeight = false

const bool BLIND_MC       = false;

const bool DUMP_RUNEVT    =false;   //Dump run:event info
const unsigned int  PRINT_SR = DIL_SRmT2a;//1;

const float MLLCUT = 60;

const bool BLIND_SSMUON  = true; //Blind  90-110 SS muon

//
// Should not need to change anything below
//

#ifdef LUMI_RAW
const int LUMIMODE =NOLUMI;
const string LUMW  = "A-B3"; //!!!!! A->B3 DEFAULT
const std::string DATE=TAG+"_raw"+ SUFF;
const float pLUMI = 999.0;
const unsigned int   MAXRUN = 9999999;
const bool BLIND_DATA=false;
#endif




#ifdef LUMI_21FB
const int LUMIMODE =LUMI21FB;
const string LUMW  = "Moriond";
const std::string DATE=TAG+"_21fb"+ SUFF;
const float pLUMI = 20.2769; //I->J
const unsigned int   MINRUN = 200804;
const unsigned int   MAXRUN = 215643;
const bool BLIND_DATA=false;
#endif

const bool USE_NOJVF_bSF=true;


//PICK VALUE FROM QFLIP PACKAGE - TEMPORARY
//const float QFLIP_RESCLALE=0.755;// trial 7 fake
//const float QFLIP_RESCLALE=0.6176;// trial 8 fake

const float QFLIP_RESCLALE=1;// trila 8 to get overlap


const float MW = 80.385;

#endif


