#ifndef SusyWeakProdAna_SusyAnaCommon_h
#define SusyWeakProdAna_SusyAnaCommon_h

#include <string>

#include "SusyWeakProdAna/Histos_2L.h"

enum LUMISET {NOLUMI=0, LUMI1FB, LUMI5FB, LUMI13FB};

//#define LUMI_RAW  //Raw count
//#define LUMI_1FB  //A->B3
//#define LUMI_5FB  //A->B14
#define LUMI_13FB  //A->E5

//Note V5.0
//const std::string TAG  = "110812";
//const std::string SUFF = "_n0111_DD_MMtrial9_SYS_HFT"; 

//New vars, reorder SR 
const std::string TAG  = "110912";
const std::string SUFF = "_n0114_DD_MMtrial9_SYS_HFT"; 


//
//Normal mode - all these are true
//
//Pred Mode
const unsigned int  NBASELEPMAX      = 3;  //number of base lepton allowed - 2 for std analysis
//const unsigned int  NBASELEPMAX      = 2;  //number of base lepton allowed - 2 for std analysis
const unsigned int  NBASELEPMIN   = 2;  //number of base lepton allowed - 2 for std analysis

const bool   FILL_TOYNT        = false;       //Write ToyNt
const string TOYNT_SR          ="DIL_CRZ";    //SR used to fill ToyNt
const unsigned int TOYNT_iSR   = DIL_CRZ;     //SR used to fill ToyNt

const bool DO_SYS         = true;    //Toggles systematics looper
const bool FILL_HFT       = true;    //Write the HistFitterTree

const bool WEIGHT_COUNT   = true;   //Apply weight to dump count table - NLO weight always applied
const bool DO_FILL_HISTO  = true;

const bool USE_DGWEIGHT   = true;
const bool USE_BWEIGHT    = true;
const bool USE_LEPSF      = true;
const bool USE_QFLIP      = true;    //Toggled charge flip prob

const bool USE_MCWEIGHT   = false;   //Use this to apply only the mcWeight and then one another weight at a time
const bool USE_MCTRIG     = false;   //Use MC trigger. DGWeight = false

const bool BLIND_MC       = false;

const bool DUMP_RUNEVT    = false;   //Dump run:event passing CR2LepOS/CR2LepSS



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

#ifdef LUMI_1FB
const int LUMIMODE =LUMI1FB;
const string LUMW  = "A-B3";
const std::string DATE=TAG+"_1fb"+ SUFF;
const float pLUMI = 1.04; //A->B3
const unsigned int   MAXRUN = 203195;
const bool BLIND_DATA=false;
#endif

#ifdef LUMI_5FB
const int LUMIMODE =LUMI5FB;
const string LUMW  = "A-B";
const std::string DATE=TAG+"_5fb" + SUFF;
const float pLUMI = 5.83; //A+B
const unsigned int   MAXRUN = 205113;
const bool BLIND_DATA=true;
#endif

#ifdef LUMI_13FB  //HCP dataset
const int LUMIMODE =LUMI13FB;
const string LUMW  = "HCP";
const std::string DATE=TAG+"_13fb" + SUFF;
const float pLUMI = 13; //A->E5
const unsigned int   MAXRUN = 210308;
const bool BLIND_DATA=false;//true;
#endif


const bool USE_NOJVF_bSF=true;


//PICK VALUE FROM QFLIP PACKAGE - TEMPORARY
//const float QFLIP_RESCLALE=0.755;// trial 7 fake
//const float QFLIP_RESCLALE=0.6176;// trial 8 fake

const float QFLIP_RESCLALE=1;// trila 8 to get overlap




#endif


