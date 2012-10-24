#ifndef SusyWeakProdAna_SusyAnaCommon_h
#define SusyWeakProdAna_SusyAnaCommon_h

#include <string>

enum LUMISET {NOLUMI=0, LUMI1FB, LUMI5FB, LUMI13FB};

//#define LUMI_RAW  //Raw count
//#define LUMI_1FB  //A->B3
//#define LUMI_5FB  //A->B14
#define LUMI_13FB  //A->E5

const std::string TAG  = "102312";
const std::string SUFF = "_n0107_DD_predSR"; //METRELCUT_SR & METREL40 - sys & HFT on this
//const std::string SUFF = "_n0107_DD_noCut";  //both false 
//const std::string SUFF = "_n0107_STD_predSR";
//const std::string SUFF = "_n0107_STD_noCut";

//
//Normal mode - all these are true
//
//Pred Mode
const bool METRELCUT_SR   = true;           //Apply Metrel/Mt2 cut in SR - SR prediction
const bool METREL40       = true;           //Apply min MetRel40 to CR2LepOS and CR2LepSS
//noCut mode
//const bool METRELCUT_SR   = false;          //Apply Metrel/Mt2 cut in SR - SR prediction
//const bool METREL40       = false;          //Apply min MetRel40 to CR2LepOS and CR2LepSS


const bool BLIND_MC       = false;

const bool WEIGHT_COUNT   = true;                      //Apply weight to dump count table - NLO weight always applied
const bool DO_FILL_HISTO  = true;

const bool USE_DGWEIGHT   = true;
const bool USE_BWEIGHT    = true;
const bool USE_LEPSF      = true; 
const bool USE_QFLIP      = true;     //Toggled charge flip prob

const bool DUMP_RUNEVT    = false;    //Dump run:event passing CR2LepOS/CR2LepSS

const bool DO_SYS         = true; /*false;*/        //Toggles systematics looper
const bool FILL_HFT       = true; /* false;*/        //Write the HistFitterTree

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
//const float QFLIP_RESCLALE= 0.5537; //+/- 0.0081  //Alpgen Zee
//const float QFLIP_RESCLALE= 0.5820;// +/- 0.0084 (stat.) //Sherpa Zee
const float QFLIP_RESCLALE=0.5966;// +/- 0.0011 (stat.) //Sherpa Zee - fix mcTruthMatch




#endif


