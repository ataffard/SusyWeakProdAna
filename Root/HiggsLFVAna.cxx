#include <iomanip>
#include <iostream>
#include "TCanvas.h"
#include "SusyWeakProdAna/HiggsLFVAna.h"

#include "SusyWeakProdAna/SusyAnaCommon.h"

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// HiggsLFVAna Constructor
/*--------------------------------------------------------------------------------*/
HiggsLFVAna::HiggsLFVAna(SusyHistos* _histos):
  SusyBaseAna(_histos,false,true,true)
{
  cout << "----------------------------------" << endl;
  cout << "    Running Higgs-LFV Analysis    " << endl;
  cout << "----------------------------------" << endl;
}

/*--------------------------------------------------------------------------------*/
// Main process loop function 
/*--------------------------------------------------------------------------------*/
void HiggsLFVAna::doAnalysis(float w, unsigned int isys)
{
  reset();
  SYST = isys;


}

/*--------------------------------------------------------------------------------*/
// End of job
/*--------------------------------------------------------------------------------*/
void HiggsLFVAna::end()
{
  cout << endl;
  cout << endl;
  cout << "**********************************************" << endl;
  cout << "SusyWHAna::event counters" <<endl;
  cout << std::setprecision(0) << std::fixed;
  cout << "read in:            " << n_readin        << endl;
  cout << "pass SUSY grid:     " << n_pass_SUSYGrid << endl;
  cout << "pass GRL:           " << n_pass_GRL      << endl;
  cout << "pass TileTrip:      " << n_pass_TileTrip << endl;
  cout << "pass LarErr:        " << n_pass_LarErr   << endl;
  cout << "pass BadJet:        " << n_pass_BadJet  << endl;
  cout << "pass DeadRegion:    " << n_pass_DeadRegion << endl;
  cout << "pass BadMu:         " << n_pass_BadMuon << endl;
  cout << "pass Cosmic:        " << n_pass_Cosmic  << endl;
  cout << "pass HotSpot:       " << n_pass_HotSpot << endl;
  cout << "pass TTCVeto:       " << n_pass_TTCVeto  << endl;
  cout << "pass Good Vtx       " << n_pass_GoodVtx  << endl;

  cout << "pass atleast 2 base " << n_pass_atleast2BaseLep << endl;
  cout << "pass exactly 2 base " << n_pass_exactly2BaseLep << endl;
  cout << "pass mll20          " << n_pass_mll20   << endl;
  
  cout << std::setprecision(1) << std::fixed;

  cout << "Channels        " << WH_FLAV[0] << "\t " << WH_FLAV[1] << "\t " << WH_FLAV[2] <<endl;
  print_line("pass category ",n_pass_dil[0], n_pass_dil[1], n_pass_dil[2]);
  print_line("pass nLep     ",n_pass_signalLep[0], n_pass_signalLep[1], n_pass_signalLep[2]);
  print_line("pass tau veto ",n_pass_tauVeto[0], n_pass_tauVeto[1], n_pass_tauVeto[2]);
  print_line("pass trig     ",n_pass_trig[0],n_pass_trig[1],n_pass_trig[2]);
  print_line("pass truth    ",n_pass_truth[0],n_pass_truth[1],n_pass_truth[2]);

  print_preOS();

  
  cout << std::endl << std::endl; 

 
  finish();

}


/*--------------------------------------------------------------------------------*/
// Set cut selection
/*--------------------------------------------------------------------------------*/
void HiggsLFVAna::setSelection(std::string s, uint dilType)
{
  TString m_sel(s);
  resetCuts();
 
  m_nLepMin=2;
  m_nLepMax=2;


}

/*--------------------------------------------------------------------------------*/
// Full event selection
/*--------------------------------------------------------------------------------*/
bool HiggsLFVAna::selectEvent(LeptonVector* leptons, 
			    LeptonVector* baseLeps, 
			    const JetVector* signalJets,
			    const Met* met,
			    float w)
{



  return true;
}

/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
void HiggsLFVAna::print_preOS()
{


}

/*--------------------------------------------------------------------------------*/
// Fill histograms 
/*--------------------------------------------------------------------------------*/
void HiggsLFVAna::fillHistograms(uint iSR,uint iSYS,
				 const LeptonVector* leptons, 
				 const JetVector* jets,
				 const Met* met,
				 float _ww)
{

}
