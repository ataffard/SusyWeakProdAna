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

//SusyNtuple
#include "SusyNtuple/SusyNtAna.h"
#include "SusyNtuple/MCWeighter.h"

//SusyWeakProdAna 
#include "SusyWeakProdAna/SusyHistos.h"
#include "SusyWeakProdAna/SusyFakeAna.h"
#include "SusyWeakProdAna/Susy2LepAna.h"
#include "SusyWeakProdAna/SusyWHAna.h"
#include "SusyWeakProdAna/Susy3LepAna.h"
#include "SusyWeakProdAna/HiggsLFVAna.h"
#include "SusyWeakProdAna/SusyAnaCommon.h"

class SusyAnaLooper : public SusyNtAna
{
  public:

  SusyAnaLooper(bool do2L, bool do3L, bool doWH, bool doFake, bool doLFV);
    virtual ~SusyAnaLooper(){};

    ofstream out;

    // Init is called when TTree (or TChain) is attached
    virtual void    Init(TTree* tree);
    // Begin is called before looping on entries
    virtual void    Begin(TTree *tree);
    // Terminate is called after looping is finished
    virtual void    Terminate();
    // Main event loop function
    virtual Bool_t  Process(Long64_t entry);

    void setMCWeighter(MCWeighter* mcw) { m_mcWeighter = mcw; }
    void do2L(bool b){_do2LAna=b;}
    void doWH(bool b){_doWHAna=b;}
    void doMll(bool b){_doMll=b;}
    void do3L(bool b){_do3LAna=b;}
    void doLFV(bool b){_doLFVAna=b;}
    void setMethod(int m) {_method=m;}
    void setSystematic(string sys) {
      if(!DO_SYS){
	cerr<< "ERROR DO_SYS False and requested sys " << endl;
	abort();
      }
      _systematic1=sys; 
      _runOneSys=true;
      cout << "Processing one systematic " << _systematic1 << endl;
    }
    void doSysRange(string sys1, string sys2) {
      if(!DO_SYS){
	cerr<< "ERROR DO_SYS False and requested sys " << endl;
	abort();
      }
      _systematic1=sys1; 
      _systematic2=sys2; 
      _runSysRange=true;
      cout << "Processing systematic range: " 
	   << _systematic1 << " --> " << _systematic2 << endl;
      cout << "Sys 1 " << _systematic1.length() << endl;
    }
    int getSysIndex(string sys){
      for(uint iiSyst=DGSys_NOM; iiSyst<DGSys_N; iiSyst++){
	if(DGSystNames[iiSyst]==sys) return iiSyst;
      }
      cout << "ERROR Sys requested not found " << sys << endl; 
      abort();
    }

    void useLooseLep(bool b){_useLooseLep=b;}

    void printSettings();

    //Dump event info for debugging
    void dumpEvent();
   
    ClassDef(SusyAnaLooper, 1);

  protected:
    TDirectory* _histoDir;

    SusyHistos*  _susyHistos;
    Susy2LepAna* _susy2LAna;
    SusyWHAna*   _susyWHAna;
    Susy3LepAna* _susy3LAna;
    SusyFakeAna* _susyFakeAna;
    HiggsLFVAna* _higgsLFVAna;

    MCWeighter*         m_mcWeighter;   // My MC weight class
   
    bool _do2LAna;
    bool _doWHAna;
    bool _doMll;
    bool _do3LAna;
    bool _doFakeAna;
    bool _doLFVAna;
    bool _useLooseLep;    

    int _method;
    string _systematic1;
    string _systematic2;
    bool   _runOneSys;
    bool   _runSysRange;

    bool _isZAlpgenSherpa;
    int nHFOR;
    int nMllCut;

    Long64_t nCount; //number of event processed
    Long64_t nDuplicate; //number of duplicat events in data
    Long64_t nLepFilter;

};

#endif
