#ifndef SusyWeakProdAna_HiggsLFVAna_h
#define SusyWeakProdAna_HiggsLFVAna_h

//////////////////////////////////////////////////////////
// General script to implement basic selection with all //
// signal region cut methods.                           //
//////////////////////////////////////////////////////////

#include <fstream>


//SusyWeakProdAna 
#include "SusyWeakProdAna/SusyBaseAna.h"
#include "SusyWeakProdAna/SusyHistos.h"

class HiggsLFVAna: public SusyBaseAna
{
  public:

    HiggsLFVAna(SusyHistos* _histos);
    virtual ~HiggsLFVAna(){};


   //Analysis main 
    void doAnalysis(float w, unsigned int isys=DGSys_NOM);
    
    void end();
    
    //Set cuts for a give SR, CR etc...
    void setSelection(std::string s, uint dilType);

    // Full event selection. Specify which leptons to use.
    bool selectEvent(LeptonVector* leptons, 
		     LeptonVector* baseLeptons, 
		     const JetVector* jets,
		     const Met* met,
		     float _ww);
    
    void fillHistograms(uint iSR,
			uint iSYS,
			const LeptonVector* leptons, 
			const JetVector* jets,
			const Met* met,
			float _ww);


    //Other functions
    void print_base();
    void print_SR();


    ClassDef(HiggsLFVAna, 1);

  protected:

   
};

#endif
