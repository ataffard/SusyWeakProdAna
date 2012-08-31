#ifndef SusyWeakProdAna_SusyAnaCommon_h
#define SusyWeakProdAna_SusyAnaCommon_h

#include <string>

//#define LUMI_RAW  //Raw count
#define LUMI_1FB  //A->B3
//#define LUMI_5FB  //A->B14

const std::string TAG = "083112";
const std::string SUFF = "_SRnoMet_n0044_mll20";//"_IP2";

//const std::string TAG = "081312";
//const std::string SUFF = "_SRnoMet_unbiasIP";//"_IP2";

enum LUMISET {NOLUMI=0, LUMI1FB, LUMI5FB};


#ifdef LUMI_RAW
const int LUMIMODE =NOLUMI;
const std::string DATE=TAG+"_1fb"+ SUFF;
const float pLUMI = 1.04;
#endif

#ifdef LUMI_1FB
const int LUMIMODE =LUMI1FB;
const std::string DATE=TAG+"_1fb"+ SUFF;
const float pLUMI = 1.04;
#endif

#ifdef LUMI_5FB
const int LUMIMODE =LUMI5FB;
const std::string DATE=TAG+"_5fb" + SUFF;
const float pLUMI = 5.83;
#endif





#endif


