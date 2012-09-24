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

#define nHSR_DG2L 15     // Regions for DIL ana = DIL_NSR in Susy2LepAna
#define nHSR_ML   13     // Regions for ML ana = ML_NSR in Susy3LepAna


#endif
