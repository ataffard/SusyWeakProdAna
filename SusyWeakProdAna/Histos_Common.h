#ifndef SusyCommon_Histos_Common_h
#define SusyCommon_Histos_Common_h

#include <string>
#include <fstream>
#include <iostream>

// Root Packages
#include "TROOT.h"
#include "TDirectory.h"
#include "TSystem.h"
#include "TFile.h"
#include "TObject.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TProfile.h"

// SusyNtuple Packages
#include "SusyNtuple/MCTruthClassifierDefs.h"

using namespace std;

#define nHM  2  //0: selected probe, 1: truth matching [method]
#define nHMC 4  //prompt, hf, LF, conv [type]
#define nCR  11 //Evt selection CR fro DG2L & ML FR measurements. CR make sense only from trm_fr histo
                //0: all, 
                //DG2L 0: =2lep
                //DG2L 1: =2lep Z veto <2 ana jet (veto jet remove too many events -try again)
                //DG2L 2: =2lep Zveto, >=2 ana jet 
                //DG2L 3: =2lep Zveto, >=2 ana jet, bjet veto
                //ML   4: =3lep 
                //ML   5: =3lep SFOS in Z peak
                //ML   6: =3lep SFOS pair, Zveto
                //ML   7: =3lep SFOS pair, Zveto, bjet veto
                //ML   8: =3lep >=2 anaJet pt>50
                //ML   9: =3lep >=2 anaJet pt>30 SFOS mll<50

#define nTRIG 5
#define nLEPTON 2

#define nHSR_DG2L 23     // Regions for DIL ana = DIL_NSR in Susy2LepAna
#define nHSR_ML   13     // Regions for ML ana = ML_NSR in Susy3LepAna

//
//https://svnweb.cern.ch/trac/atlasphys/browser/Physics/SUSY/Analyses/WeakProduction/HistFitterTree/trunk/Systematics_naming.txt
//
enum DG2L_SYS 
{
  DGSys_NOM = 0,
  DGSys_EES_Z_UP,            // Electron Scale Zsys + sigma
  DGSys_EES_Z_DN,            // Electron Scale Zsys - sigma
  DGSys_EES_MAT_UP,          // Electron Scale Material + sigma
  DGSys_EES_MAT_DN,          // Electron Scale Material - sigma
  DGSys_EES_PS_UP,           // Electron Scale Presampler + sigma
  DGSys_EES_PS_DN,           // Electron Scale Presampler - sigma
  DGSys_EES_LOW_UP,          // Electron Scale Low Pt + sigma
  DGSys_EES_LOW_DN,          // Electron Scale Low Pt - sigma
  DGSys_EER_UP,              // Electron Resolution + sigma
  DGSys_EER_DN,              // Electron Resolution - sigma
  DGSys_MS_UP,               // Muon MS track + sigma
  DGSys_MS_DN,               // Muon MS track - sigma
  DGSys_ID_UP,               // Muon ID track + sigma
  DGSys_ID_DN,               // Muon ID track - sigma
  DGSys_JES_UP,              // Jet Energy Scale + sigma
  DGSys_JES_DN,              // Jet Energy Scale - sigma
  DGSys_JER,                 // Jet Energy Resolution (gaussian)
  DGSys_SCALEST_UP,          // Met scale soft term + sigma
  DGSys_SCALEST_DN,          // Met scale soft term - sigma
  DGSys_RESOST,              // Met resolution soft term + sigma
  DGSys_TRIGSF_EL_UP,        // Trigger Scale factor + error
  DGSys_TRIGSF_EL_DN,        // Trigger Scale factor - error
  DGSys_TRIGSF_MU_UP,        // Trigger Scale factor + error
  DGSys_TRIGSF_MU_DN,        // Trigger Scale factor - error
  DGSys_BJet_DN,             // BJet sys - sigma
  DGSys_CJet_DN,             // CJet sys - sigma
  DGSys_LJet_DN,             // LJet sys - sigma
  DGSys_BJet_UP,             // BJet sys + sigma
  DGSys_CJet_UP,             // CJet sys + sigma
  DGSys_LJet_UP,             // LJet sys + sigma
  DGSys_BKGMETHOD_UP,        // charge flip, jet veto...
  DGSys_BKGMETHOD_DN,        // charge flip, jet veto...
  DGSys_FAKE_EL_RE_UP,       // Fake Sys 
  DGSys_FAKE_EL_RE_DN,
  DGSys_FAKE_EL_FR_UP,
  DGSys_FAKE_EL_FR_DN,
  DGSys_FAKE_MU_RE_UP,
  DGSys_FAKE_MU_RE_DN,
  DGSys_FAKE_MU_FR_UP,
  DGSys_FAKE_MU_FR_DN,
  DGSys_GEN,                 // Generator uncertainties, parton/hadronization
  DGSys_GEN_UP,              // Generator param Alpgen, ISR/FSR
  DGSys_GEN_DN,              // Generator param Alpgen, ISR/FSR
  DGSys_PDF_UP,              // PDF
  DGSys_PDF_DN,              // PDF
  DGSys_XS_UP,               // Cross section
  DGSys_XS_DN,               // Cross section
  DGSys_N
};


// Human-readable names for enum SusyNtSys 
const std::string DG2LSystNames[DGSys_N] = {
  "NOM",
  "EES_Z_UP",    
  "EES_Z_DN",    
  "EES_MAT_UP",  
  "EES_MAT_DN",  
  "EES_PS_UP",   
  "EES_PS_DN",   
  "EES_LOW_UP",  
  "EES_LOW_DN",  
  "EER_UP",      
  "EER_DN",      
  "MS_UP",       
  "MS_DN",       
  "ID_UP",       
  "ID_DN",       
  "JES_UP",      
  "JES_DN",      
  "JER",         
  "SCALEST_UP",  
  "SCALEST_DN",  
  "RESOST",
  "TRIGSF_EL_UP",
  "TRIGSF_EL_DN",
  "TRIGSF_MU_UP",
  "TRIGSF_MU_DN",
  "BTag_BJet_DN",
  "BTag_CJet_DN",
  "BTag_LJet_DN",
  "BTag_BJet_UP",
  "BTag_CJet_UP",
  "BTag_LJet_UP",
  "BKGMETHOD_UP",
  "BKGMETHOD_DN",
  "FAKE_EL_RE_UP",  
  "FAKE_EL_RE_DN",
  "FAKE_EL_FR_UP",
  "FAKE_EL_FR_DN",
  "FAKE_MU_RE_UP",
  "FAKE_MU_RE_DN",
  "FAKE_MU_FR_UP",
  "FAKE_MU_FR_DN",
  "GEN",         
  "GEN_UP",
  "GEN_DN",
  "PDF_UP",
  "PDF_DN",
  "XS_UP",
  "XS_DN"
};




#endif
