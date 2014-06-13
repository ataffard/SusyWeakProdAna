#ifndef SusyWeakProdAna_Susy3LepAna_h
#define SusyWeakProdAna_Susy3LepAna_h

//////////////////////////////////////////////////////////
// General script to implement basic selection with all //
// signal region cut methods.                           //
//////////////////////////////////////////////////////////

#include <fstream>

//SusyWeakProdAna 
#include "SusyWeakProdAna/SusyBaseAna.h"

class Susy3LepAna: public SusyBaseAna
{
  public:

    Susy3LepAna(SusyHistos* _histos);
    virtual ~Susy3LepAna(){};

    //Analysis main 
    void doAnalysis(float w, unsigned int isys);

    // Full event selection. Specify which leptons to use.
    bool selectEvent(LeptonVector* leptons, 
		     const TauVector* taus,
		     const JetVector* jets, 
		     const Susy::Met* met,
		     float w);

    // Selection region 
    void setSelection(std::string s);

    void end();
   
    void fillHistograms(uint iSR, uint iSYS,
			const LeptonVector* leptons, 
			const JetVector* jets, 
			const Susy::Met* met,
			float _ww);
 		  
    //categorize event as eee=0, eem=1, mmm=2,  mme=3;
    int evtCatgUnOrd(const LeptonVector* leptons);
    //categorize event using the ordered highest pt OS pair
    //eee/eem=0,  mmm/mme=1, eme/mee=2, emm/mem=3
    int evtCatgOrd(const LeptonVector* leptons, bool useOS);
 
      
    //Other functions
    void print_CF3L();
    void print_VRWZ();
    void print_VRZZ();



    ClassDef(Susy3LepAna, 1);

  protected:
    
   

 

};

#endif
