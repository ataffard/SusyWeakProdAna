#ifndef SusyWeakProdAna_SusyAnaCommon_h
#define SusyWeakProdAna_SusyAnaCommon_h

#include <string>

enum LUMISET {NOLUMI=0, LUMI1FB, LUMI5FB, LUMI13FB};

//#define LUMI_RAW  //Raw count
#define LUMI_1FB  //A->B3
//#define LUMI_5FB  //A->B14
//#define LUMI_13FB  //A->E5

const std::string TAG = "100212";
const std::string SUFF = "_n0102_std";

const bool WEIGHT_COUNT=true; //false //Apply weight to dump count table - NLO weight always applied

const bool SKIP_METCUT_SR = false;//true;
const bool DO_FILL_HISTO =false;

//Normal mode - all these are true
const bool USE_DGWEIGHT=true;
const bool USE_BWEIGHT=true;//true; 
const bool USE_LEPSF=true; //true;

const bool USE_QFLIP=false; //Toggled charge flip prob

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
const string LUMW  = "A-X";
const std::string DATE=TAG+"_13fb" + SUFF;
const float pLUMI = 13; //A->E5
const unsigned int   MAXRUN = 210308;
const bool BLIND_DATA=true;
#endif


const bool USE_NOJVF_bSF=true;




#endif


