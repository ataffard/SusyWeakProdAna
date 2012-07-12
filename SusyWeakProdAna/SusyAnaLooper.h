#ifndef SusyWeakProdAna_SusyAnaLooper_h
#define SusyAna_SusyAnaLooper_h

//////////////////////////////////////////////////////////
// General script to loop over SusyNt and run various   //
// analyses.                                            //
//////////////////////////////////////////////////////////

//Standard includes
#include <string>
#include <sstream>
#include <fstream>
#include <utility>
#include <iostream>
#include <math.h>
#include <vector>

// Root Packages
#include "TTree.h"
#include "TStopwatch.h"

// Susy Common
#include "SusyNtuple/SusyNtAna.h"

//SusyWeakProdAna 
#include "SusyWeakProdAna/SusyHistos.h"
#include "SusyWeakProdAna/SusyFakeAna.h"
#include "SusyWeakProdAna/Susy2LepAna.h"
#include "SusyWeakProdAna/Susy3LepAna.h"

class SusyAnaLooper : public SusyNtAna
{
  public:

    SusyAnaLooper();
    virtual ~SusyAnaLooper(){};

    ofstream out;

    // Begin is called before looping on entries
    virtual void    Begin(TTree *tree);
    // Terminate is called after looping is finished
    virtual void    Terminate();

    // Main event loop function
    virtual Bool_t  Process(Long64_t entry);

    void do2L(bool b){_do2LAna=b;}
    void do3L(bool b){_do3LAna=b;}
    void doFake(bool b){_doFakeAna=b;}


    ClassDef(SusyAnaLooper, 1);

  protected:

    TStopwatch* _timer;

    TDirectory* _histoDir;

    SusyHistos*  _susyHistos;
    Susy2LepAna* _susy2LAna;
    Susy3LepAna* _susy3LAna;
    SusyFakeAna* _susyFakeAna;
    
    bool _do2LAna;
    bool _do3LAna;
    bool _doFakeAna;


};

#endif
