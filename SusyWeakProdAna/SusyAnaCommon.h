#ifndef SusyWeakProdAna_SusyAnaCommon_h
#define SusyWeakProdAna_SusyAnaCommon_h

#include <string>

enum LUMISET {NOLUMI=0, LUMI1FB, LUMI5FB, LUMI10FB};

//#define LUMI_RAW  //Raw count
#define LUMI_1FB  //A->B3
//#define LUMI_5FB  //A->B14
//#define LUMI_10FB  //A->D7

const std::string TAG = "092312";
const std::string SUFF = "_n0101";
//const std::string SUFF = "_n0101_noDGTrig";

const bool WEIGHT_COUNT=false; //false //Apply weight to dump count table 

const bool USE_DGWEIGHT=false; //true
const bool USE_BWEIGHT=true; //true
const bool USE_LEPSF=true; //true

//
// Should not need to change anything below
//

#ifdef LUMI_RAW
const int LUMIMODE =NOLUMI;
const string LUMW  = "A-B3"; //!!!!! A->B3 DEFAULT
const std::string DATE=TAG+"_raw"+ SUFF;
const float pLUMI = 999.0;
const unsigned int   MAXRUN = 9999999;
#endif

#ifdef LUMI_1FB
const int LUMIMODE =LUMI1FB;
const string LUMW  = "A-B3";
const std::string DATE=TAG+"_1fb"+ SUFF;
const float pLUMI = 1.04; //A->B3
const unsigned int   MAXRUN = 203195;
#endif

#ifdef LUMI_5FB
const int LUMIMODE =LUMI5FB;
const string LUMW  = "A-B";
const std::string DATE=TAG+"_5fb" + SUFF;
const float pLUMI = 5.83; //A+B
const unsigned int   MAXRUN = 205113;
#endif

#ifdef LUMI_10FB
const int LUMIMODE =LUMI10FB;
const string LUMW  = "A-X";
const std::string DATE=TAG+"_10fb" + SUFF;
const float pLUMI = 10.454; //A->D
const unsigned int   MAXRUN = 209025;
#endif



const bool USE_NOJVF_bSF=true;

#endif


