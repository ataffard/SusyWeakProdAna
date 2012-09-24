#ifndef SusyWeakProdAna_SusyHistos_h
#define SusyWeakProdAna_SusyHistos_h

//////////////////////////////////////////////////////////
// General script to implement basic selection with all //
// signal region cut methods.                           //
//////////////////////////////////////////////////////////


// SusyWeakProdAna Packages
#include "SusyWeakProdAna/Histos_Fake.h"
#include "SusyWeakProdAna/Histos_2L.h"
#include "SusyWeakProdAna/Histos_3L.h"

//Select running mode
//std: All lepton
//rlep: Prompt lep only - for DD method
//flep: Fake lep only - for MC fake estimate only

const char* const SMETHOD[] = {"std","rlep","flep"};
enum METHOD {STD=0, RLEP=1, FLEP=2};


class SusyHistos: public Histos_Fake, 
                  public Histos_2L, 
                  public Histos_3L
{
 public:
  
  SusyHistos();
  virtual ~SusyHistos(){};

  void setSample(std::string s){_sample = s;}
  std::string sampleName() const {return _sample;}

  void SaveHistograms(TDirectory* _hDir,
		      int method=STD, 
		      bool mcMll=false, bool isAlpgen=false);
  
  void H1FILL(TH1F* h, float x, float w) { h->Fill(x,w); }
  void H2FILL(TH2F* h, float x, float y, float w) {h->Fill(x,y, w);}
  void H3FILL(TH3F* h, float x, float y, float z, float w) {h->Fill(x,y,z,w);}
  void PFILL(TProfile* h, float x, float y, float w) {h->Fill(x,y,w);}

  ClassDef(SusyHistos, 1);
  
 protected:

  std::string _sample;

};

#endif
