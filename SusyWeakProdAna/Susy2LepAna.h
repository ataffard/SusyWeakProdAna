#ifndef SusyWeakProdAna_Susy2LepAna_h
#define SusyWeakProdAna_Susy2LepAna_h

//////////////////////////////////////////////////////////
// General script to implement basic selection with all //
// signal region cut methods.                           //
//////////////////////////////////////////////////////////

#include <fstream>

// Root Packages
#include "TRandom3.h"

// Susy Common
#include "SusyMatrixMethod/DiLeptonMatrixMethod.h"
#include "HistFitterTree/HistFitterTree.h"
#include "SusyXSReader/XSReader.h"

//SusyWeakProdAna 
#include "SusyWeakProdAna/SusyBaseAna.h"


class Susy2LepAna: public SusyBaseAna
{
  public:

    Susy2LepAna(SusyHistos* _histos);
    virtual ~Susy2LepAna(){};

    float getFakeWeight(const LeptonVector* leptons, uint nVtx, bool isMC, 
			int iSR, float metrel, uint iSys=DGSys_NOM);
   
    //Analysis main 
    void doAnalysis(float w, unsigned int isys=DGSys_NOM);
    
    void end();
    
    //Set cuts for a give SR, CR etc...
    void setSelection(std::string s,uint dilType);

    // Full event selection. Specify which leptons to use.
    bool selectEvent(LeptonVector* leptons, 
		     LeptonVector* baseLeptons, 
		     const JetVector* jets,
		     const Met* met,
		     float w);
    
    void fillHistograms(uint iSR,
			uint iSYS,
			const LeptonVector* leptons, 
			const JetVector* jets,
			const Met* met,
			float _ww);

    //HFT
    void initializeHistFitterTree();
    float writeIntoHistFitterTree(uint iSR,
				  LeptonVector* leptons, 
				  const LeptonVector* baseLeptons, 
				  const JetVector* signalJets, 
				  const JetVector* baseJets,
				  const Met* met);
    bool validSystForHFT(uint iSR);
    void moveHFTOutput();

    //Other functions
    void print_SRmT2();
    void print_SRWW();
    void print_SRZjets();
    void print_SRSSjets();
    void print_WWCR();
    void print_TOPCR();
    void print_ZVCR();
    void print_VRSS();
    void print_CRZ();

    ClassDef(Susy2LepAna, 1);

  protected:

    HistFitterTree*  m_histFitterTrees[ DGSys_GEN ];

    //Analysis flags
    bool    m_writeHFT;
    string  HFTName;
   
};

#endif
