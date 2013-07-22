#ifndef SusyWeakProdAna_SusyWHAna_h
#define SusyWeakProdAna_SusyWHAna_h

//////////////////////////////////////////////////////////
// General script to implement basic selection with all //
// signal region cut methods.                           //
//////////////////////////////////////////////////////////

#include <fstream>

// Root Packages
#include "TRandom3.h"

// Susy Common
#include "SusyNtuple/SleptonXsecReader.h"
#include "SusyMatrixMethod/DiLeptonMatrixMethod.h"
#include "HistFitterTree/HistFitterTree.h"
#include "SusyXSReader/XSReader.h"

//SusyWeakProdAna 
#include "SusyWeakProdAna/SusyBaseAna.h"
#include "SusyWeakProdAna/SUSYCrossSection.h"
#include "SusyWeakProdAna/SusyHistos.h"

#include "SusyWeakProdAna/ToyNt.h"


class SusyWHAna: public SusyBaseAna
{
  public:

    SusyWHAna(SusyHistos* _histos);
    virtual ~SusyWHAna(){};

    float getFakeWeight(const LeptonVector* leptons, uint nVtx, bool isMC, 
			int iSR, float metrel, uint iSys=DGSys_NOM);
   
    //Analysis main 
    void doAnalysis(unsigned int isys=DGSys_NOM);
    
    void end();
    
    //Set cuts for a give SR, CR etc...
    void setSelection(std::string s,DiLepEvtType dilType);

    // Full event selection. Specify which leptons to use.
    bool selectEvent(LeptonVector* leptons, 
		     LeptonVector* baseLeptons, 
		     const JetVector* jets,
		     const Met* met);
    
    void fillHistograms(uint iSR,
			uint iSYS,
			const LeptonVector* leptons, 
			const JetVector* jets,
			const Met* met,
			float _ww);

    //Other functions
    void print_SRSS();
    void print_SROSOF2jets();
   
    ClassDef(SusyWHAna, 1);

  protected:

   
};

#endif
