#ifndef SusyCommon_Histos_Fake_h
#define SusyCommon_Histos_Fake_h

//////////////////////////////////////////////////////////
// Lepton histograms - Fake studies
//////////////////////////////////////////////////////////

#include "SusyNtuple/TGuiUtils.h"

#include "SusyWeakProdAna/Histos_Common.h"


//  typedef TH1F     *H1s[nHM][nHMC];
//  typedef TH1F     *H1sCR[nHM][nHMC][nCR];
//  typedef TH3F     *H3s[nHM][nHMC]; //iso -vs pt -vs Npv
//  typedef TH2F     *H2sTrig[nHM][nHMC][nTRIG];
//  typedef TH3F     *HDG2L_fk[nHSR_DG2L][nLEPTON][nHMC]; //3D: pt, |eta|, metrel. [2]: E,M
//  typedef TH3F     *HML_fk[nHSR_ML][nLEPTON][nHMC]; //4: eee,eem, mmm, mme.  [2]: E,M
//  typedef TProfile *PH1s[nHM][nHMC];


class Histos_Fake
{
 public:
  
  Histos_Fake(){
    _utils = new TGuiUtils();
    gROOT->SetStyle("ATLAS");
    _utils->atlasStyle->SetOptStat("emr");
    _utils->atlasStyle->SetOptStat("");
  };
  virtual ~Histos_Fake(){};

  void BookFakeHistograms(TDirectory* _hDir);

  TGuiUtils* _utils;

#define DEF_HPROBE( name )       name[nHM][nHMC];                //TH1,TH2F, TH3, TProfile
#define DEF_HPROBE_CR( name )    name[nHM][nHMC][nCR];           //TH1
#define DEF_HPROBE_TRIG( name )  name[nHM][nHMC][nTRIG];         //TH2  
#define DEF_DG2L_FK( name )      name[nHSR_DG2L][nLEPTON][nHMC]; //TH3
#define DEF_ML_FK( name )        name[nHSR_ML][nLEPTON][nHMC];   //TH3
  

  TH1F* Mee_os;
  TH1F* Mmm_os;
  TH1F* W_mu;
  
  //Muon
  TH1F* DEF_HPROBE( m_org );
  TH1F* DEF_HPROBE( m_type );
  TH1F* DEF_HPROBE( m_pt );
  TH1F* DEF_HPROBE( m_eta );
  TH1F* DEF_HPROBE( m_d0 );
  TH1F* DEF_HPROBE( m_d0S );
  TH1F* DEF_HPROBE( m_z0sintheta );
  TH1F* DEF_HPROBE( m_dR_jet );
  TH1F* DEF_HPROBE( m_ptrel );
  TH1F* DEF_HPROBE( m_signd0 );
  TH1F* DEF_HPROBE( m_cjet_pt );
  TH1F* DEF_HPROBE( m_cjet_nTrk );
  TH1F* DEF_HPROBE( m_cjet_isBTag );
  TH1F* DEF_HPROBE( m_cjet_idB );
  TH1F* DEF_HPROBE( m_cjet_bj_pt );
  TH1F* DEF_HPROBE( m_cjet_b_pt );
  TH1F* DEF_HPROBE( m_ptCone20 );
  TH1F* DEF_HPROBE( m_ptCone30 );
  TH1F* DEF_HPROBE( m_ptCone40 );
  TH1F* DEF_HPROBE( m_etCone20 );
  TH1F* DEF_HPROBE( m_etCone30 );
  TH1F* DEF_HPROBE( m_etCone40 );
  TH1F* DEF_HPROBE( m_ptCone20Rel );
  TH1F* DEF_HPROBE( m_ptCone30Rel );  
  TH1F* DEF_HPROBE( m_ptCone40Rel );
  TH1F* DEF_HPROBE( m_etCone20Rel );
  TH1F* DEF_HPROBE( m_etCone30Rel );  
  TH1F* DEF_HPROBE( m_etCone40Rel );

  TH2F* DEF_HPROBE( m_ptCone30Pt );
  TH2F* DEF_HPROBE( m_ptCone30RelPt );
  TH2F* DEF_HPROBE( m_etCone30Pt );
  TH2F* DEF_HPROBE( m_etCone30RelPt );
  
  TH3F* DEF_HPROBE( m_ptCone20_npv );
  TH3F* DEF_HPROBE( m_ptCone30_npv );
  TH3F* DEF_HPROBE( m_ptCone40_npv );
  TH3F* DEF_HPROBE( m_etCone20_npv );
  TH3F* DEF_HPROBE( m_etCone30_npv );
  TH3F* DEF_HPROBE( m_etCone40_npv );
  TH3F* DEF_HPROBE( m_ptCone20Rel_npv );
  TH3F* DEF_HPROBE( m_ptCone30Rel_npv );  
  TH3F* DEF_HPROBE( m_ptCone40Rel_npv );
  TH3F* DEF_HPROBE( m_etCone20Rel_npv );
  TH3F* DEF_HPROBE( m_etCone30Rel_npv );  
  TH3F* DEF_HPROBE( m_etCone40Rel_npv );
  
  TH3F* DEF_HPROBE( m_ptCone20_mu );
  TH3F* DEF_HPROBE( m_ptCone30_mu );
  TH3F* DEF_HPROBE( m_ptCone40_mu );
  TH3F* DEF_HPROBE( m_etCone20_mu );
  TH3F* DEF_HPROBE( m_etCone30_mu );
  TH3F* DEF_HPROBE( m_etCone40_mu );
  TH3F* DEF_HPROBE( m_ptCone20Rel_mu );
  TH3F* DEF_HPROBE( m_ptCone30Rel_mu );  
  TH3F* DEF_HPROBE( m_ptCone40Rel_mu );
  TH3F* DEF_HPROBE( m_etCone20Rel_mu );
  TH3F* DEF_HPROBE( m_etCone30Rel_mu );  
  TH3F* DEF_HPROBE( m_etCone40Rel_mu );
  
  TH1F* DEF_HPROBE( m_ptCone20_Eff );
  TH1F* DEF_HPROBE( m_ptCone30_Eff );
  TH1F* DEF_HPROBE( m_ptCone40_Eff );
  TH1F* DEF_HPROBE( m_etCone20_Eff );
  TH1F* DEF_HPROBE( m_etCone30_Eff );
  TH1F* DEF_HPROBE( m_etCone40_Eff );
  TH1F* DEF_HPROBE( m_ptCone20Rel_Eff );
  TH1F* DEF_HPROBE( m_ptCone30Rel_Eff );
  TH1F* DEF_HPROBE( m_ptCone40Rel_Eff );
  TH1F* DEF_HPROBE( m_etCone20Rel_Eff );
  TH1F* DEF_HPROBE( m_etCone30Rel_Eff );
  TH1F* DEF_HPROBE( m_etCone40Rel_Eff );
  
  
  //To compute FR & Eff
  TH1F*  DEF_HPROBE_CR( m_loose_pt);
  TH1F*  DEF_HPROBE_CR( m_loose_eta );
  TH1F*  DEF_HPROBE_CR( m_loose_d0S );
  TH1F*  DEF_HPROBE_CR( m_loose_z0sintheta );
  TH1F*  DEF_HPROBE_CR( m_loose_met );
  TH1F*  DEF_HPROBE_CR( m_loose_metrel );
  TH1F*  DEF_HPROBE_CR( m_loose_dPhilmet );
  TH1F*  DEF_HPROBE_CR( m_loose_dPhijmet );
  TH1F*  DEF_HPROBE_CR( m_loose_nJets );
  TH1F*  DEF_HPROBE_CR( m_tight_pt );
  TH1F*  DEF_HPROBE_CR( m_tight_eta );
  TH1F*  DEF_HPROBE_CR( m_tight_d0S );
  TH1F*  DEF_HPROBE_CR( m_tight_z0sintheta );
  TH1F*  DEF_HPROBE_CR( m_tight_met );
  TH1F*  DEF_HPROBE_CR( m_tight_metrel );
  TH1F*  DEF_HPROBE_CR( m_tight_dPhilmet );
  TH1F*  DEF_HPROBE_CR( m_tight_dPhijmet );
  TH1F*  DEF_HPROBE_CR( m_tight_nJets );
  TH1F*  DEF_HPROBE_CR( m_tightNI_pt );
  TH1F*  DEF_HPROBE_CR( m_tightNI_eta );
  TH1F*  DEF_HPROBE_CR( m_tightNI_d0S );
  TH1F*  DEF_HPROBE_CR( m_tightNI_z0sintheta );
  TH1F*  DEF_HPROBE_CR( m_tightNI_met );
  TH1F*  DEF_HPROBE_CR( m_tightNI2_pt );
  TH1F*  DEF_HPROBE_CR( m_tightNI2_eta );
  TH1F*  DEF_HPROBE_CR( m_tightNI2_d0S );
  TH1F*  DEF_HPROBE_CR( m_tightNI2_z0sintheta );
  TH1F*  DEF_HPROBE_CR( m_tightNI2_met );
  TH1F*  DEF_HPROBE_CR( m_tightNIIP_pt );
  TH1F*  DEF_HPROBE_CR( m_tightNIIP_eta );
  TH1F*  DEF_HPROBE_CR( m_tightNIIP_d0S );
  TH1F*  DEF_HPROBE_CR( m_tightNIIP_z0sintheta );
  TH1F*  DEF_HPROBE_CR( m_tightNIIP_met );
  TH1F*  DEF_HPROBE_CR( m_reff_pt );
  TH1F*  DEF_HPROBE_CR( m_reff_eta );
  TH1F*  DEF_HPROBE_CR( m_reff_met );
  TH1F*  DEF_HPROBE_CR( m_reff_metrel );
  TH1F*  DEF_HPROBE_CR( m_reff_dPhilmet );
  TH1F*  DEF_HPROBE_CR( m_reff_dPhijmet );
  TH1F*  DEF_HPROBE_CR( m_reff_nJets );
  TH1F*  DEF_HPROBE_CR( m_fr_pt );
  TH1F*  DEF_HPROBE_CR( m_fr_eta );
  TH1F*  DEF_HPROBE_CR( m_fr_met );
  TH1F*  DEF_HPROBE_CR( m_fr_metrel );
  TH1F*  DEF_HPROBE_CR( m_fr_dPhilmet );
  TH1F*  DEF_HPROBE_CR( m_fr_dPhijmet );
  TH1F*  DEF_HPROBE_CR( m_fr_nJets );
  
  //TODO: Add plots for all Iso var 
  //For electron use TightPP
  TProfile* DEF_HPROBE( m_iso_mu ); //selected isolation
  TProfile* DEF_HPROBE( m_iso_npv );
  
  //Trigger studies - data only
  TH2F* DEF_HPROBE_TRIG( m_pt_Npv );
  TH2F* DEF_HPROBE_TRIG( m_pt_Npv_pTrig );
  TH2F* DEF_HPROBE_TRIG( m_ptCone20Rel_Npv );
  TH2F* DEF_HPROBE_TRIG( m_ptCone20Rel_Npv_pTrig );
  TH2F* DEF_HPROBE_TRIG( m_pt_wiso_Npv );
  TH2F* DEF_HPROBE_TRIG( m_pt_wiso_Npv_pTrig );
  
  TH2F* DEF_HPROBE_TRIG( m_pt_Mu );
  TH2F* DEF_HPROBE_TRIG( m_pt_Mu_pTrig );
  TH2F* DEF_HPROBE_TRIG( m_ptCone20Rel_Mu );
  TH2F* DEF_HPROBE_TRIG( m_ptCone20Rel_Mu_pTrig );
  TH2F* DEF_HPROBE_TRIG( m_pt_wiso_Mu );
  TH2F* DEF_HPROBE_TRIG( m_pt_wiso_Mu_pTrig );
  
  //Electron
  TH1F* DEF_HPROBE( e_org );
  TH1F* DEF_HPROBE( e_type );
  TH1F* DEF_HPROBE( e_pt );
  TH1F* DEF_HPROBE( e_eta );
  TH1F* DEF_HPROBE( e_d0 );
  TH1F* DEF_HPROBE( e_d0S );
  TH1F* DEF_HPROBE( e_z0sintheta );
  TH1F* DEF_HPROBE( e_dR_jet );
  TH1F* DEF_HPROBE( e_signd0 );
  TH1F* DEF_HPROBE( e_ptrel );
  TH1F* DEF_HPROBE( e_cjet_pt );
  TH1F* DEF_HPROBE( e_cjet_nTrk );
  TH1F* DEF_HPROBE( e_cjet_isBTag );
  TH1F* DEF_HPROBE( e_cjet_idB );
  TH1F* DEF_HPROBE( e_cjet_bj_pt );
  TH1F* DEF_HPROBE( e_cjet_b_pt );
  TH1F* DEF_HPROBE( e_ptCone20 );
  TH1F* DEF_HPROBE( e_ptCone30 );
  TH1F* DEF_HPROBE( e_ptCone40 );
  TH1F* DEF_HPROBE( e_etCone20 );
  TH1F* DEF_HPROBE( e_etCone30 );
  TH1F* DEF_HPROBE( e_etCone40 );
  TH1F* DEF_HPROBE( e_etConeCorr20 );
  TH1F* DEF_HPROBE( e_etConeCorr30 );
  TH1F* DEF_HPROBE( e_etConeCorr40 );
  TH1F* DEF_HPROBE( e_etConeTopoCorr20 );
  TH1F* DEF_HPROBE( e_etConeTopoCorr30 );
  TH1F* DEF_HPROBE( e_etConeTopoCorr40 );
  TH1F* DEF_HPROBE( e_ptCone20Rel );
  TH1F* DEF_HPROBE( e_ptCone30Rel );  
  TH1F* DEF_HPROBE( e_ptCone40Rel );
  TH1F* DEF_HPROBE( e_etCone20Rel );
  TH1F* DEF_HPROBE( e_etCone30Rel );  
  TH1F* DEF_HPROBE( e_etCone40Rel );
  TH1F* DEF_HPROBE( e_etConeCorr20Rel );
  TH1F* DEF_HPROBE( e_etConeCorr30Rel );
  TH1F* DEF_HPROBE( e_etConeCorr40Rel );
  TH1F* DEF_HPROBE( e_etConeTopoCorr20Rel );
  TH1F* DEF_HPROBE( e_etConeTopoCorr30Rel );
  TH1F* DEF_HPROBE( e_etConeTopoCorr40Rel );
  
  TH2F* DEF_HPROBE( e_ptCone30Pt );
  TH2F* DEF_HPROBE( e_ptCone30RelPt );
  TH2F* DEF_HPROBE( e_etCone30Pt );
  TH2F* DEF_HPROBE( e_etCone30RelPt );

  TH3F* DEF_HPROBE( e_ptCone20_npv );
  TH3F* DEF_HPROBE( e_ptCone30_npv );
  TH3F* DEF_HPROBE( e_ptCone40_npv );
  TH3F* DEF_HPROBE( e_etCone20_npv );
  TH3F* DEF_HPROBE( e_etCone30_npv );
  TH3F* DEF_HPROBE( e_etCone40_npv );
  TH3F* DEF_HPROBE( e_etConeCorr20_npv );
  TH3F* DEF_HPROBE( e_etConeCorr30_npv );
  TH3F* DEF_HPROBE( e_etConeCorr40_npv );
  TH3F* DEF_HPROBE( e_etConeTopoCorr20_npv );
  TH3F* DEF_HPROBE( e_etConeTopoCorr30_npv );
  TH3F* DEF_HPROBE( e_etConeTopoCorr40_npv );
  TH3F* DEF_HPROBE( e_ptCone20Rel_npv );
  TH3F* DEF_HPROBE( e_ptCone30Rel_npv );  
  TH3F* DEF_HPROBE( e_ptCone40Rel_npv );
  TH3F* DEF_HPROBE( e_etCone20Rel_npv );
  TH3F* DEF_HPROBE( e_etCone30Rel_npv );  
  TH3F* DEF_HPROBE( e_etCone40Rel_npv );
  TH3F* DEF_HPROBE( e_etConeCorr20Rel_npv );
  TH3F* DEF_HPROBE( e_etConeCorr30Rel_npv );
  TH3F* DEF_HPROBE( e_etConeCorr40Rel_npv );
  TH3F* DEF_HPROBE( e_etConeTopoCorr20Rel_npv );
  TH3F* DEF_HPROBE( e_etConeTopoCorr30Rel_npv );
  TH3F* DEF_HPROBE( e_etConeTopoCorr40Rel_npv );
  
  TH3F* DEF_HPROBE( e_ptCone20_mu );
  TH3F* DEF_HPROBE( e_ptCone30_mu );
  TH3F* DEF_HPROBE( e_ptCone40_mu );
  TH3F* DEF_HPROBE( e_etCone20_mu );
  TH3F* DEF_HPROBE( e_etCone30_mu );
  TH3F* DEF_HPROBE( e_etCone40_mu );
  TH3F* DEF_HPROBE( e_etConeCorr20_mu );
  TH3F* DEF_HPROBE( e_etConeCorr30_mu );
  TH3F* DEF_HPROBE( e_etConeCorr40_mu );
  TH3F* DEF_HPROBE( e_etConeTopoCorr20_mu );
  TH3F* DEF_HPROBE( e_etConeTopoCorr30_mu );
  TH3F* DEF_HPROBE( e_etConeTopoCorr40_mu );
  TH3F* DEF_HPROBE( e_ptCone20Rel_mu );
  TH3F* DEF_HPROBE( e_ptCone30Rel_mu );  
  TH3F* DEF_HPROBE( e_ptCone40Rel_mu );
  TH3F* DEF_HPROBE( e_etCone20Rel_mu );
  TH3F* DEF_HPROBE( e_etCone30Rel_mu );  
  TH3F* DEF_HPROBE( e_etCone40Rel_mu );
  TH3F* DEF_HPROBE( e_etConeCorr20Rel_mu );
  TH3F* DEF_HPROBE( e_etConeCorr30Rel_mu );
  TH3F* DEF_HPROBE( e_etConeCorr40Rel_mu );
  TH3F* DEF_HPROBE( e_etConeTopoCorr20Rel_mu );
  TH3F* DEF_HPROBE( e_etConeTopoCorr30Rel_mu );
  TH3F* DEF_HPROBE( e_etConeTopoCorr40Rel_mu );
  
  TH1F* DEF_HPROBE( e_ptCone20_Eff );
  TH1F* DEF_HPROBE( e_ptCone30_Eff );
  TH1F* DEF_HPROBE( e_ptCone40_Eff );
  TH1F* DEF_HPROBE( e_etCone20_Eff );
  TH1F* DEF_HPROBE( e_etCone30_Eff );
  TH1F* DEF_HPROBE( e_etCone40_Eff );
  TH1F* DEF_HPROBE( e_etConeCorr20_Eff );
  TH1F* DEF_HPROBE( e_etConeCorr30_Eff );
  TH1F* DEF_HPROBE( e_etConeCorr40_Eff );
  TH1F* DEF_HPROBE( e_etConeTopoCorr20_Eff );
  TH1F* DEF_HPROBE( e_etConeTopoCorr30_Eff );
  TH1F* DEF_HPROBE( e_etConeTopoCorr40_Eff );
  TH1F* DEF_HPROBE( e_ptCone20Rel_Eff );
  TH1F* DEF_HPROBE( e_ptCone30Rel_Eff );
  TH1F* DEF_HPROBE( e_ptCone40Rel_Eff );
  TH1F* DEF_HPROBE( e_etCone20Rel_Eff );
  TH1F* DEF_HPROBE( e_etCone30Rel_Eff );
  TH1F* DEF_HPROBE( e_etCone40Rel_Eff );
  TH1F* DEF_HPROBE( e_etConeCorr20Rel_Eff );
  TH1F* DEF_HPROBE( e_etConeCorr30Rel_Eff );
  TH1F* DEF_HPROBE( e_etConeCorr40Rel_Eff );
  TH1F* DEF_HPROBE( e_etConeTopoCorr20Rel_Eff );
  TH1F* DEF_HPROBE( e_etConeTopoCorr30Rel_Eff );
  TH1F* DEF_HPROBE( e_etConeTopoCorr40Rel_Eff );
  
  
  TH1F*  DEF_HPROBE_CR( e_loose_pt );
  TH1F*  DEF_HPROBE_CR( e_loose_eta );
  TH1F*  DEF_HPROBE_CR( e_loose_d0S );
  TH1F*  DEF_HPROBE_CR( e_loose_z0sintheta );
  TH1F*  DEF_HPROBE_CR( e_loose_met );
  TH1F*  DEF_HPROBE_CR( e_loose_metrel );
  TH1F*  DEF_HPROBE_CR( e_loose_dPhilmet );
  TH1F*  DEF_HPROBE_CR( e_loose_dPhijmet );
  TH1F*  DEF_HPROBE_CR( e_loose_nJets );
  TH1F*  DEF_HPROBE_CR( e_tight_pt );
  TH1F*  DEF_HPROBE_CR( e_tight_eta );
  TH1F*  DEF_HPROBE_CR( e_tight_d0S );
  TH1F*  DEF_HPROBE_CR( e_tight_z0sintheta );
  TH1F*  DEF_HPROBE_CR( e_tight_met );
  TH1F*  DEF_HPROBE_CR( e_tight_metrel );
  TH1F*  DEF_HPROBE_CR( e_tight_dPhilmet );
  TH1F*  DEF_HPROBE_CR( e_tight_dPhijmet );
  TH1F*  DEF_HPROBE_CR( e_tight_nJets );
  TH1F*  DEF_HPROBE_CR( e_tightPP_pt );
  TH1F*  DEF_HPROBE_CR( e_tightPP_eta );
  TH1F*  DEF_HPROBE_CR( e_tightPP_d0S );
  TH1F*  DEF_HPROBE_CR( e_tightPP_z0sintheta );
  TH1F*  DEF_HPROBE_CR( e_tightPP_met );
  TH1F*  DEF_HPROBE_CR( e_tightNI_pt );
  TH1F*  DEF_HPROBE_CR( e_tightNI_eta );
  TH1F*  DEF_HPROBE_CR( e_tightNI_d0S );
  TH1F*  DEF_HPROBE_CR( e_tightNI_z0sintheta );
  TH1F*  DEF_HPROBE_CR( e_tightNI_met );
  TH1F*  DEF_HPROBE_CR( e_tightNI2_pt );
  TH1F*  DEF_HPROBE_CR( e_tightNI2_eta );
  TH1F*  DEF_HPROBE_CR( e_tightNI2_d0S );
  TH1F*  DEF_HPROBE_CR( e_tightNI2_z0sintheta );
  TH1F*  DEF_HPROBE_CR( e_tightNI2_met );
  TH1F*  DEF_HPROBE_CR( e_tightNIIP_pt );
  TH1F*  DEF_HPROBE_CR( e_tightNIIP_eta );
  TH1F*  DEF_HPROBE_CR( e_tightNIIP_d0S );
  TH1F*  DEF_HPROBE_CR( e_tightNIIP_z0sintheta );
  TH1F*  DEF_HPROBE_CR( e_tightNIIP_met );
  TH1F*  DEF_HPROBE_CR( e_reff_pt );
  TH1F*  DEF_HPROBE_CR( e_reff_eta );
  TH1F*  DEF_HPROBE_CR( e_reff_met );
  TH1F*  DEF_HPROBE_CR( e_reff_metrel );
  TH1F*  DEF_HPROBE_CR( e_reff_dPhilmet );
  TH1F*  DEF_HPROBE_CR( e_reff_dPhijmet );
  TH1F*  DEF_HPROBE_CR( e_reff_nJets );
  TH1F*  DEF_HPROBE_CR( e_fr_pt );
  TH1F*  DEF_HPROBE_CR( e_fr_eta );
  TH1F*  DEF_HPROBE_CR( e_fr_met );
  TH1F*  DEF_HPROBE_CR( e_fr_metrel );
  TH1F*  DEF_HPROBE_CR( e_fr_dPhilmet );
  TH1F*  DEF_HPROBE_CR( e_fr_dPhijmet );
  TH1F*  DEF_HPROBE_CR( e_fr_nJets );
  
  
  TProfile* DEF_HPROBE( e_iso_mu ); //selected isolation
  TProfile* DEF_HPROBE( e_iso_npv );
  
  
  //Trigger studies - data only
  TH2F* DEF_HPROBE_TRIG( e_pt_Npv );
  TH2F* DEF_HPROBE_TRIG( e_pt_Npv_pTrig );
  TH2F* DEF_HPROBE_TRIG( e_ptCone20Rel_Npv );
  TH2F* DEF_HPROBE_TRIG( e_ptCone20Rel_Npv_pTrig );
  TH2F* DEF_HPROBE_TRIG( e_pt_wiso_Npv );
  TH2F* DEF_HPROBE_TRIG( e_pt_wiso_Npv_pTrig );
  
  TH2F* DEF_HPROBE_TRIG( e_pt_Mu );
  TH2F* DEF_HPROBE_TRIG( e_pt_Mu_pTrig );
  TH2F* DEF_HPROBE_TRIG( e_ptCone20Rel_Mu );
  TH2F* DEF_HPROBE_TRIG( e_ptCone20Rel_Mu_pTrig );
  TH2F* DEF_HPROBE_TRIG( e_pt_wiso_Mu );
  TH2F* DEF_HPROBE_TRIG( e_pt_wiso_Mu_pTrig );
  
  TH3F* DEF_DG2L_FK( DG2L_fakeLep );//to compute fake bkg fraction in each SR's
  TH3F* DEF_ML_FK( ML_fakeLep ); //to compute fake bkg fraction in each SR's
  
  
  /** Fill histo given type and  method*/
  //void H1s_Fill(H1s &h, int idx1, int idx2, float x, float w){
  //    h[idx1][idx2]->Fill(x,w);
  //  };
  
  //  void H1sCR_Fill(TH1F*  DEF_H1sCR(h), int idx1, int idx2, int idx3, float x, float w){
  //      h[idx1][idx2][idx3]->Fill(x,w);
  //  };
  
  /*
  void PH1s_Fill(PH1s &h, int idx1, int idx2, float x, float y, float w){
    h[idx1][idx2]->Fill(x,y,w);
  };
  
  void H3s_Fill(H3s &h, int idx1, int idx2, float x, float y, float z, float w){
    h[idx1][idx2]->Fill(x,y,z,w);
  };
  
  void H2sTrig_Fill(H2sTrig &h, int idx1, int idx2, int idx3, float x, float y, float w){
    h[idx1][idx2][idx3]->Fill(x,y,w);
  };
  
  void H1sDGfk_Fill(HDG2L_fk &h, int idx1, int idx2,  int idx3, float x, float y, float z, float w){
    h[idx1][idx2][idx3]->Fill(x,y,z,w);
  };
  
  void H1sMLfk_Fill(HML_fk &h, int idx1, int idx2,  int idx3, float x, float y, float z, float w){
    h[idx1][idx2][idx3]->Fill(x,y,z,w);
  };
  
  */
  //TO DO EFF histos !!

  ClassDef(Histos_Fake,1);
  
};

#endif
