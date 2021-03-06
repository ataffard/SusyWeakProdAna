#define ToyNt_cxx

#include "SusyWeakProdAna/ToyNt.h"
#include "TParameter.h"

#include "SusyWeakProdAna/PhysicsTools.h"


using namespace std;


//-----------------------------------------------------------------------------------------------------------
ToyNt::ToyNt(TString MCID, TString suffix) :
  SusyNtTools()
{
  filename = MCID + "_" + suffix + ".root";
  
  file= TFile::Open(filename,"RECREATE");
  tree = new TTree("ToyNt","ToyNt");
  (tree)->SetAutoSave(10000000);
  TTree::SetBranchStyle(1);
  (tree)->SetDirectory(file);

  _b_vBeta_z      = new TVector3();
  _b_pTCM         = new TVector3();
  _b_vBetaT_CMtoR = new TVector3();
  _b_vBetaR       = new TVector3();

}
  
//-----------------------------------------------------------------------------------------------------------
void ToyNt::setBlocks(bool metD, bool dijetB, 
		      bool OS2LB, bool SS2LB,
		      bool ZBalB, bool diverVarsB,
		      bool LFVB, bool razorB,
		      bool fakeB)
{
  metDetails      = metD;
  dijetBlock      = dijetB;
  OS2LBlock       = OS2LB;
  SS2LBlock       = SS2LB;
  ZBalanceBlock   = ZBalB;
  diversVarsBlock = diverVarsB;
  fakeBlock       = fakeB;
  LFVBlock        = LFVB;
  razorBlock      = razorB;

}

//-----------------------------------------------------------------------------------------------------------
void ToyNt::BookTree()
{

  //Event Block
  tree->Branch("run",&_b_run,"run/I");
  tree->Branch("event",&_b_event,"event/I");
  tree->Branch("mcId",&_b_mcId,"mcId/I");
  tree->Branch("npv",&_b_npv,"npv/F");
  tree->Branch("npvCorr",&_b_npvCorr,"npvCorr/F");
  tree->Branch("w",&_b_w,"w/D");
  tree->Branch("wbtag",&_b_wbtag,"w/D");
  tree->Branch("wtrig",&_b_wtrig,"w/D");
  tree->Branch("wqflip",&_b_wqflip,"w/D");
  tree->Branch("passDilTrig",&_b_passDilTrig,"w/O");
  tree->Branch("isEg",&_b_isEg,"w/O");
  tree->Branch("isMc",&_b_isMc,"w/O");

  //Lepton Block
  tree->Branch("nlep",&_b_nlep,"nlep/I");
  tree->Branch("l_pt",_b_l_pt,"l_pt[nlep]/F");
  tree->Branch("l_pz",_b_l_pz,"l_pz[nlep]/F");
  tree->Branch("l_eta",_b_l_eta,"l_eta[nlep]/F");
  tree->Branch("l_phi",_b_l_phi,"l_phi[nlep]/F");
  tree->Branch("l_e",_b_l_e,"l_e[nlep]/F");
  tree->Branch("l_Y",_b_l_Y,"l_Y[nlep]/F");
  tree->Branch("l_q",_b_l_q,"l_q[nlep]/I");
  tree->Branch("l_ptcone30",_b_l_ptcone30,"l_ptcone30[nlep]/F");
  tree->Branch("l_etcone30",_b_l_etcone30,"l_etcone30[nlep]/F");
  tree->Branch("l_etconetopo30",_b_l_etconetopo30,"l_etconetopo30[nlep]/F");
  tree->Branch("l_d0",_b_l_d0,"l_d0[nlep]/F");
  tree->Branch("l_d0Err",_b_l_d0Err,"l_d0Err[nlep]/F");
  tree->Branch("l_z0",_b_l_z0,"l_z0[nlep]/F");
  tree->Branch("l_isEle",_b_l_isEle,"l_isEle[nlep]/O");
  tree->Branch("l_isT",_b_l_isT,"l_isT[nlep]/O");
  tree->Branch("l_org",_b_l_org,"l_org[nlep]/I");
  tree->Branch("l_isQFlip",_b_l_isQFlip,"l_isQFlip[nlep]/O");


  //Dilepton Block
  tree->Branch("isOS",&_b_isOS,"isOS/O");
  tree->Branch("isGenuineSS",&_b_isGenuineSS,"isGenuineSS/O");
  tree->Branch("llType",&_b_llType,"llType/I");
  tree->Branch("dphi_ll",&_b_dphi_ll,"dphi_ll/F");
  tree->Branch("deta_ll",&_b_deta_ll,"deta_ll/F");
  tree->Branch("pTll",&_b_pTll,"pTll/F");
  tree->Branch("phill",&_b_phill,"phill/F");
  tree->Branch("dR_ll",&_b_dR_ll,"dR_ll/F");
  tree->Branch("mll",&_b_mll,"mll/F");
  tree->Branch("mll_collApprox",&_b_mll_collApprox,"mll_collApprox/F");
  tree->Branch("dphi_ll_j1",&_b_dphi_ll_j1,"dphi_ll_j1/F");


  //Met Block
  tree->Branch("met",&_b_met,"met/F");
  tree->Branch("met_phi",&_b_met_phi,"met_phi/F");
  tree->Branch("metrel",&_b_metrel,"metrel/F");
  if(metDetails){
    tree->Branch("met_refEle",&_b_met_refEle,"met_refEle/F");
    tree->Branch("met_refMuo",&_b_met_refMuo,"met_refMuo/F");
    tree->Branch("met_refJet",&_b_met_refJet,"met_refJet/F");
    tree->Branch("met_cellout",&_b_met_cellout,"met_cellout/F");
  }
  
  //Jet Block
  tree->Branch("nJets",&_b_nJets,"nJets/I");
  tree->Branch("nSJets",&_b_nSJets,"nSJets/I");
  tree->Branch("nCJets",&_b_nCJets,"nCJets/I");
  tree->Branch("nBJets",&_b_nBJets,"nBJets/I");
  tree->Branch("nFJets",&_b_nFJets,"nFJets/I");
  tree->Branch("j_isC20",_b_j_isC20,"j_isC20[nJets]/O");
  tree->Branch("j_isB20",_b_j_isB20,"j_isB20[nJets]/O");
  tree->Branch("j_isF30",_b_j_isF30,"j_isF30[nJets]/O");
  tree->Branch("j_pt",_b_j_pt,"j_pt[nJets]/F");
  tree->Branch("j_eta",_b_j_eta,"j_eta[nJets]/F");
  tree->Branch("j_phi",_b_j_phi,"j_phi[nJets]/F");
  tree->Branch("j_e",_b_j_e,"j_e[nJets]/F");
  tree->Branch("j_Y",_b_j_Y,"j_Y[nJets]/F");
  tree->Branch("j_jvf",_b_j_jvf,"j_jvf[nJets]/F");
  tree->Branch("j_mv1",_b_j_mv1,"j_mv1[nJets]/F");
  tree->Branch("j_nEle",_b_j_nEle,"j_nEle[nJets]/I");
  tree->Branch("j_nMu",_b_j_nMu,"j_nMu[nJets]/I");
  tree->Branch("j_isTruth",_b_j_isTruth,"j_isTruth[nJets]/O");
  tree->Branch("j_label",_b_j_label,"j_label[nJets]/I");

  // General event kinematics
  tree->Branch("mEff",&_b_mEff,"mEff/F");
  tree->Branch("ST",&_b_ST,"ST/F");
  tree->Branch("topTag",&_b_topTag,"topTag/O");

  //MET & lepton/jet kinematics  
  tree->Branch("dphi_metl",_b_dphi_metl,"dphi_metl[nlep]/F");
  tree->Branch("mTl",_b_mTl,"mTl[nlep]/F");
  tree->Branch("dphi_metcl",&_b_dphi_metcl,"dphi_metcl/F");
  tree->Branch("dphi_metcj",&_b_dphi_metcj,"dphi_metcj/F");


  //Dijet system 2 leading  ON/OFF, OFF by default
  if(dijetBlock){
    tree->Branch("mjj",&_b_mjj,"mjj/F");
    tree->Branch("dRjj",&_b_dRjj,"dRjj/F");
    tree->Branch("dEtajj",&_b_dEtajj,"dEtajj/F");
    tree->Branch("pTjj",&_b_pTjj,"pTjj/F");
  }

  //2L-OS mT2 mcT  ON/OFF, OFF by default
  if(OS2LBlock){
    tree->Branch("mT2",&_b_mT2,"mT2/F");
    tree->Branch("mT2jj",&_b_mT2jj,"mT2jj/F");
    tree->Branch("mT2J",&_b_mT2J,"mT2J/F");
    tree->Branch("mct",&_b_mct,"mct/F");
    tree->Branch("mctPerp",&_b_mctPerp,"mctPerp/F");
    tree->Branch("mctPara",&_b_mctPara,"mctPara/F");
  }

  //2L-SS kinematic vars ON/OFF, OFF by default
  if(SS2LBlock){
    tree->Branch("mWWT",&_b_mWWT,"mWWT/F");
    tree->Branch("mlj",&_b_mlj,"mlj/F");
    tree->Branch("mljj",&_b_mljj,"mljj/F");
  }

  
  //Z balance studies kine  - ON/OFF: OFF by default
  if(ZBalanceBlock){
    tree->Branch("nOJets",&_b_nOJets,"nOJets/I");
    tree->Branch("j_isOJ", _b_j_isOJ, "j_isOJ[nJets]/O");
    tree->Branch("j_isRecoil",_b_j_isRecoil,"j_isRecoil[nJets]/O");
    tree->Branch("j_isSublead",_b_j_isSublead,"j_isSublead[nJets]/O");
    tree->Branch("dphi_ll_oj1",&_b_dphi_ll_oj1,"dphi_ll_oj1/F");
    tree->Branch("dphi_Zj",&_b_dphi_Zj,"dphi_Zj/F");
    tree->Branch("dphi_metcoj",&_b_dphi_metcoj,"dphi_metcoj/F");
  }

  
  if(diversVarsBlock){
    tree->Branch("sphericity",&_b_sphericity,"sphericity/F");
    tree->Branch("sphericityTrans",&_b_sphericityTrans,"sphericityTrans/F");
    tree->Branch("llAcoplanarity",&_b_llAcoplanarity,"llAcoplanarity/F");
    tree->Branch("jjAcoplanarity",&_b_jjAcoplanarity,"jjAcoplanarity/F");
    tree->Branch("JZBjets",&_b_JZBjets,"JZBjets/F");
    tree->Branch("JZBmet",&_b_JZBmet,"JZBmet/F");   
    tree->Branch("pTll_Tb",&_b_pTll_Tb,"pTll_Tb/F");
    tree->Branch("dPhib",&_b_dPhib,"dPhib/F");
  }

  //LFV block
  if(LFVBlock){
    tree->Branch("mcoll",&_b_mcoll,"mcoll/F");
    tree->Branch("metCorr",&_b_metCorr,"metCorr/F");
    tree->Branch("metCorrPhi",&_b_metCorrPhi,"metCorrPhi/F");
    tree->Branch("metrelCorr",&_b_metrelCorr,"metrelCorr/F");
    tree->Branch("mcollCorr",&_b_mcollCorr,"mcollCorr/F");
    tree->Branch("mTl0Corr",&_b_mTl0Corr,"mTl0Corr/F");
    tree->Branch("mTl1Corr",&_b_mTl1Corr,"mTl1Corr/F");
  }

  if(razorBlock){
    tree->Branch("shatr",&_b_shatr,"shatr/D");
    tree->Branch("dphi_ll_vBetaT",&_b_dphi_ll_vBetaT,"dphi_ll_vBetaT/D");
    tree->Branch("dphi_l1l2",&_b_dphi_l1l2,"dphi_l1l2/D");
    tree->Branch("gammaR",&_b_gammaR,"gammaR/D");
    tree->Branch("dphi_vBetaR_vBetaT",&_b_dphi_vBetaR_vBetaT,"dphi_vBetaR_vBetaT/D");
    tree->Branch("mDeltaR",&_b_mDeltaR,"mDeltaR/D");
    tree->Branch("cosThetaRp1",&_b_cosThetaRp1,"cosThetaRp1/D");
    tree->Branch("cosTheta_b",&_b_cosTheta_b,"cosTheta_b/D");
    tree->Branch("vBeta_z","TVector3",&_b_vBeta_z);
    tree->Branch("pTCM","TVector3",&_b_pTCM);
    tree->Branch("vBetaT_CMtoR","TVector3",&_b_vBetaT_CMtoR);
    tree->Branch("vBetaR","TVector3",&_b_vBetaR);
  }

  //trigger block
  tree->Branch("trig_e12_mu8",&_b_trig_e12_mu8,"trig_e12_mu8/O");
  tree->Branch("trig_mu18_e7",&_b_trig_mu18_e7,"trig_mu18_e7/O");
  tree->Branch("trig_e60",&_b_trig_e60,"trig_e60/O");
  tree->Branch("trig_mu36",&_b_trig_mu36,"trig_mu36/O");
  tree->Branch("trig_e24i",&_b_trig_e24i,"trig_e24i/O");
  tree->Branch("trig_mu24i",&_b_trig_mu24i,"trig_mu24i/O");
 
  tree->Branch("l_e7",_b_l_e7,"l_e7[nlep]/O");
  tree->Branch("l_e12",_b_l_e12,"l_e12[nlep]/O");
  tree->Branch("l_e60",_b_l_e60,"l_e60[nlep]/O");
  tree->Branch("l_e24i",_b_l_e24i,"l_e24i[nlep]/O");
  tree->Branch("l_mu8",_b_l_mu8,"l_mu8[nlep]/O");
  tree->Branch("l_mu18",_b_l_mu18,"l_mu18[nlep]/O");
  tree->Branch("l_mu36",_b_l_mu36,"l_mu36[nlep]/O");
  tree->Branch("l_mu24i",_b_l_mu24i,"l_mu24i[nlep]/O");


  //Fake lepton Block
  if(fakeBlock){
    tree->Branch("l_ll_FType",&_b_ll_FType,"ll_FType/I");

    tree->Branch("pass_SS1j",&_b_pass_SS1j,"pass_SS1j/O");
    tree->Branch("pass_SSEM",&_b_pass_SSEM,"pass_SSEM/O");
    tree->Branch("pass_HFTP",&_b_pass_HFTP,"pass_HFTP/O");
    tree->Branch("pass_MCEff",&_b_pass_MCEff,"pass_MCEff/O");
    tree->Branch("pass_ZTP",&_b_pass_ZTP,"pass_ZTP/O");
    tree->Branch("pass_ZConv",&_b_pass_ZConv,"pass_Zconv/O");
    tree->Branch("pass_ZHFLF",&_b_pass_ZHFLF,"pass_ZHFLF/O");

    tree->Branch("SSEM_tagIdx",&_b_SSEM_tagIdx,"SSEM_tagIdx/I");
    tree->Branch("SSEM_probeIdx",&_b_SSEM_probeIdx,"SSEM_probeIdx/I");
    tree->Branch("HFTP_tagIdx",&_b_HFTP_tagIdx,"HFTP_tagIdx/I");
    tree->Branch("HFTP_probeIdx",&_b_HFTP_probeIdx,"HFTP_probeIdx/I");
    tree->Branch("ZTP_tagIdx1",&_b_ZTP_tagIdx1,"ZTP_tagIdx1/I");
    tree->Branch("ZTP_probeIdx1",&_b_ZTP_probeIdx1,"ZTP_probeIdx1/I");
    tree->Branch("ZTP_tagIdx2",&_b_ZTP_tagIdx2,"ZTP_tagIdx2/I");
    tree->Branch("ZTP_probeIdx2",&_b_ZTP_probeIdx2,"ZTP_probeIdx2/I");
    tree->Branch("ZConv_tagIdx1",&_b_ZConv_tagIdx1,"ZConv_tagIdx1/I");
    tree->Branch("ZConv_tagIdx2",&_b_ZConv_tagIdx2,"ZConv_tagIdx2/I");
    tree->Branch("ZConv_probeIdx",&_b_ZConv_probeIdx,"ZConv_probeIdx/I");
    tree->Branch("ZHFLF_tagIdx1",&_b_ZHFLF_tagIdx1,"ZHFLF_tagIdx1/I");
    tree->Branch("ZHFLF_tagIdx2",&_b_ZHFLF_tagIdx2,"ZHFLF_tagIdx2/I");
    tree->Branch("ZHFLF_probeIdx",&_b_ZHFLF_probeIdx,"ZHFLF_probeIdx/I");
    tree->Branch("ZHFLF_mlll",&_b_ZHFLF_mlll,"ZHFLF_mlll/F");
  }


  clearOutputBranches();
  return;

}



//-----------------------------------------------------------------------------------------------------------
void ToyNt::clearOutputBranches(void) {

  _b_run=-1;
  _b_event=-1;
  _b_mcId=-1;
  _b_npv=-1;
  _b_npvCorr=-1;
  _b_w=0;
  _b_wbtag=1;
  _b_wtrig=1;
  _b_wqflip=1;
  _b_passDilTrig=false;
  _b_isEg=false;
  _b_isMc=false;


  _b_nlep=0;
  for(uint i=0; i<nLepMax; i++){
    _b_l_pt[i]=-999;
    _b_l_pz[i]=-999;
    _b_l_eta[i]=-999;
    _b_l_phi[i]=-999;
    _b_l_e[i]=-999;
    _b_l_Y[i]=-999;
    _b_l_q[i]=0;
    _b_l_ptcone30[i]=-999;
    _b_l_etcone30[i]=-999;
    _b_l_etconetopo30[i]=-999;
    _b_l_d0[i]=-999;
    _b_l_d0Err[i]=-999;
    _b_l_z0[i]=-999;
    _b_l_isEle[i]=false;
    _b_l_isT[i]=false;
    _b_l_org[i]=4;
    _b_l_isQFlip[i]=false;
  }
  
  _b_isOS=false;
  _b_isGenuineSS=false;
  _b_llType = -1;
  _b_dphi_ll=-999;
  _b_deta_ll=-999;
  _b_pTll=-999;
  _b_phill=-999;
  _b_dR_ll=-999;
  _b_mll=-999;
  _b_mll_collApprox=-999;
  _b_dphi_ll_j1=-999;

  _b_met=-999;
  _b_met_phi=-999;
  _b_metrel=-999;
  _b_met_refEle=-999;
  _b_met_refMuo=-999;
  _b_met_refJet=-999;
  _b_met_cellout=-999;
  
  
  _b_nJets=0; 
  _b_nSJets=0; 
  _b_nCJets=0; 
  _b_nBJets=0; 
  _b_nFJets=0;
  for(uint i=0; i<25; i++){
    _b_j_isC20[i]=false;
    _b_j_isB20[i]=false;
    _b_j_isF30[i]=false;
    
    _b_j_pt[i]=-999;
    _b_j_eta[i]=-999;
    _b_j_phi[i]=-999;
    _b_j_e[i]=-999;
    _b_j_Y[i]=-999;
    _b_j_jvf[i]=-999;
    _b_j_mv1[i]=-999;
    _b_j_nEle[i]=0;
    _b_j_nMu[i]=0;
    _b_j_isTruth[i]=false;
    _b_j_label[i]=-999;
  }

  _b_mEff=-999;
  _b_ST=-999;
  _b_topTag=false;

  for(uint i=0; i<nLepMax; i++){
    _b_dphi_metl[i]=-999; 
    _b_mTl[i]=-999;  
  }
  _b_dphi_metcl=-999;
  _b_dphi_metcj=999;
  
  _b_mjj=-999;
  _b_dRjj=-999;
  _b_dEtajj=-999;
  _b_pTjj=-999;


  _b_mT2=-999;
  _b_mT2jj=-999;
  _b_mT2J=-999;
  _b_mct=-999;
  _b_mctPerp=-999;
  _b_mctPara=-999;
  
  _b_mWWT=-999;
  _b_mlj=-999;
  _b_mljj=-999;

  _b_nOJets=0;
  for(uint i=0; i<25; i++){
    _b_j_isOJ[i]=false;
    _b_j_isRecoil[i]=false;
    _b_j_isSublead[i]=false;
  }
  _b_dphi_Zj=-999;
  _b_dphi_ll_oj1=-999;
  _b_dphi_metcoj=999;

 
  _b_sphericity=-999;
  _b_sphericityTrans=-999;
  _b_llAcoplanarity=-999;
  _b_jjAcoplanarity=-999;
  _b_JZBjets=-999;
  _b_JZBmet=-999;
  _b_pTll_Tb=-999;
  _b_dPhib=-999;

  _b_mcoll=-999;
  _b_metCorr=-999;
  _b_metCorrPhi=-999;
  _b_metrelCorr=-999;
  _b_mcollCorr=-999;
  _b_mTl0Corr=-999;
  _b_mTl1Corr=-999;

  _b_shatr=-999;
  _b_dphi_ll_vBetaT=-999;
  _b_dphi_l1l2=-999;
  _b_gammaR=-999;
  _b_dphi_vBetaR_vBetaT=-999;
  _b_mDeltaR=-999;
  _b_cosThetaRp1=-999;
  _b_cosTheta_b=-999;
  _b_vBeta_z->Clear();
  _b_pTCM->Clear();
  _b_vBetaT_CMtoR->Clear();
  _b_vBetaR->Clear();
  
  
  _b_trig_e12_mu8 = false;
  _b_trig_mu18_e7 = false;
  _b_trig_e60     = false;
  _b_trig_mu36    = false;
  _b_trig_e24i    = false;
  _b_trig_mu24i   = false;

  for(uint i=0; i<nLepMax; i++){
    _b_l_e7[i] = false;
    _b_l_e12[i] = false;
    _b_l_e60[i] = false;
    _b_l_e24i[i] = false;
    _b_l_mu8[i] = false;
    _b_l_mu18[i] = false;
    _b_l_mu36[i] = false;
    _b_l_mu24i[i] = false;
  }
  
  _b_ll_FType=-1;
  _b_pass_SS1j=false;
  _b_pass_SSEM=false;
  _b_pass_HFTP=false;
  _b_pass_MCEff=false;
  _b_pass_ZTP=false;
  _b_pass_ZConv=false;
  _b_pass_ZHFLF=false;

  _b_SSEM_tagIdx=-1;
  _b_SSEM_probeIdx=-1;
  _b_HFTP_tagIdx=-1;
  _b_HFTP_probeIdx=-1;
  _b_ZTP_tagIdx1=-1;
  _b_ZTP_probeIdx1=-1;
  _b_ZTP_tagIdx2=-1;
  _b_ZTP_probeIdx2=-1;
  _b_ZConv_tagIdx1=-1;
  _b_ZConv_tagIdx2=-1;
  _b_ZConv_probeIdx=-1;
  _b_ZHFLF_tagIdx1=-1;
  _b_ZHFLF_tagIdx2=-1;
  _b_ZHFLF_probeIdx=-1;
  _b_ZHFLF_mlll=-999;

  return;
}

//-----------------------------------------------------------------------------------------------------------
void ToyNt::SaveTree(){
  
  // Write out the output tree and close the output file
  file->cd();
  tree->SetDirectory(file);
  tree->Write();
  tree->SetDirectory(0);
  delete tree;
  file->Close();
  delete file;
}

//-----------------------------------------------------------------------------------------------------------
void ToyNt::WriteTree(){
  tree->Fill();
  clearOutputBranches();
} 

//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTreeEvent(const Susy::Event* evt,
			  float npvCorr,
			  double w, double wbtag, double wqflip, double wtrig, 
			  bool passDilTrig)
{
  _b_run = evt->run;
  _b_event = evt->event;
  _b_mcId = evt->mcChannel;
  _b_npv = evt->nVtx;
  _b_npvCorr = npvCorr;
  _b_w       = w;
  _b_wbtag   = wbtag;
  _b_wtrig   = wtrig;
  _b_wqflip  = wqflip;
  _b_passDilTrig   = passDilTrig;
  _b_isEg    = evt->stream == Stream_Egamma;
  _b_isMc    = evt->stream == Stream_MC;
}

//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTreeLeptons(const LeptonVector* leptons,
			    ElectronVector& baseElectrons, MuonVector& baseMuons, 
			    const Met* met, int nVtx, bool isMc, int llType)
{
  if(leptons->size()<nLepMax) _b_nlep=leptons->size();
  else{ 
    cerr << "FillTreeLeptons - too many " << leptons->size() << endl;
    abort();
  }
  

  _b_dphi_metcl=999;  
  _b_dphi_ll=999; 
  _b_deta_ll=999; 
  int qqType=1; 
  TLorentzVector _ll; 

  for(uint ilep=0; ilep<leptons->size(); ilep++){
     const Susy::Lepton* _l = leptons->at(ilep);
     if(ilep<2) {
       _ll += (*_l);
       qqType *= _l->q;
     }

    _b_l_pt[ilep] = _l->Pt();
    _b_l_pz[ilep] = _l->Pz();
    _b_l_eta[ilep] = _l->Eta();
    _b_l_phi[ilep] = _l->Phi();
    _b_l_e[ilep] = _l->E();
    _b_l_Y[ilep] = _l->Rapidity();
    _b_l_q[ilep] = _l->q;
    _b_l_d0[ilep] = _l->d0Unbiased;
    _b_l_d0Err[ilep] = _l->errD0Unbiased;
    _b_l_z0[ilep] = _l->z0Unbiased;
    _b_l_isEle[ilep] = _l->isEle();

    if(_l->isEle()){
      Electron* _e = (Electron*) _l;
      float etconetopo =  elEtTopoConeCorr(_e, baseElectrons, baseMuons,nVtx, isMc);
      _b_l_etcone30[ilep] = _e->etcone30Corr;
      _b_l_etconetopo30[ilep] = etconetopo;
      _b_l_ptcone30[ilep] = _l->ptcone30;
    }
    else{
      Muon* _m = (Muon*) _l;
      float ptcone = _m->ptcone30ElStyle; 
      _b_l_etcone30[ilep] = muEtConeCorr(_m, baseElectrons, baseMuons, nVtx,isMc); 
      _b_l_ptcone30[ilep] = ptcone;
    }

    //Dphi w/ MET and mT
    _b_dphi_metl[ilep]=fabs(met->lv().DeltaPhi(*_l)); 
    _b_mTl[ilep] = mT(*_l, met->lv());  

    //Dphi met closest lepton
    float _dPhi=fabs(met->lv().DeltaPhi(*_l));
    if(_dPhi<_b_dphi_metcl) _b_dphi_metcl=_dPhi;  

  }

  //Dilepton kinematics
  if(leptons->size()>1){
    _b_isOS = (qqType<0) ? true : false;
    _b_llType = llType;
    _b_dphi_ll = fabs(leptons->at(0)->DeltaPhi(*leptons->at(1)));
    _b_deta_ll = leptons->at(0)->Eta()-leptons->at(1)->Eta();
    _b_dR_ll = leptons->at(0)->DeltaR(*leptons->at(1));
    _b_pTll = _ll.Pt();
    _b_phill = _ll.Phi();
    _b_mll = _ll.M();
        
    //2LSS kinematics
    _b_mWWT = mT(_ll, met->lv());
        
    //Divers - kine vars
    TLorentzVector _pTll_b = _ll + met->lv();  //???
    _b_pTll_Tb = _pTll_b.Pt();  //???
    _b_dPhib = _pTll_b.DeltaPhi(met->lv()); //???
    
  }

}
//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTreeSignalJets(const JetVector* jets, const LeptonVector* leptons, const Met* met,
			       const ElectronVector* preElectrons, 
			       const MuonVector* preMuons)
{
  _b_mEff =  Meff(*leptons,*jets,met,JET_PT_CUT);

  _b_nSJets = jets->size();
  if(_b_nSJets==0){
    // cout << "no signal jet " << endl;
    return;
  }

  TLorentzVector _ll;
  for(uint ilep=0; ilep<leptons->size(); ilep++){
     const Susy::Lepton* _l = leptons->at(ilep);
     if(ilep>1) continue;
    _ll = _ll + (*_l);
  }
 
  TLorentzVector _jj; //central dijet
  int nCjj=0;
  _b_ST=0;

  for(uint ijet=0; ijet<jets->size(); ijet++){
    if(ijet>25) continue;
    const Susy::Jet* _j = jets->at(ijet); 
    _b_nJets++;
    //cout << "  dEta " a<< _j->detEta << " " ;
    //_j->print();
    
    if(isCentralBJet(_j)){
      _b_nBJets++;
      _b_j_isB20[ijet] = true;
      //cout << "Got BJet" << endl;
    }    
    else if(isCentralLightJet(_j,m_jvfTool, (SusyNtSys) DGSys_NOM, m_anaType)){
      _b_nCJets++;
      _b_j_isC20[ijet] = true;
      // cout << "Got CJet" << endl;
    }
    else if(isForwardJet(_j)){
      _b_nFJets++;
      _b_j_isF30[ijet] = true;
      //cout << "Got FJet" << endl;
    }
    //else cout << "Unidenfify jet type" << endl;

    _b_j_pt[ijet]  = _j->Pt();
    _b_j_eta[ijet] = _j->Eta();
    _b_j_phi[ijet] = _j->Phi();
    _b_j_e[ijet]   = _j->E();
    _b_j_Y[ijet]   = _j->Rapidity();
    _b_j_jvf[ijet] = _j->jvf;
    _b_j_mv1[ijet] = _j->mv1;
    _b_j_isTruth[ijet] = _j->matchTruth;
    _b_j_label[ijet] = _j->truthLabel;
    
    //dphi ll & leading jet
    if(ijet==0) _b_dphi_ll_j1 = fabs(_ll.DeltaPhi(*jets->at(ijet)));

    //dPhi met and closest jet
    float _dPhi=fabs(met->lv().DeltaPhi(*_j));
    if(_dPhi<_b_dphi_metcj) _b_dphi_metcj=_dPhi;

    _b_ST += _j->Pt();
    if(isCentralLightJet(_j,m_jvfTool, (SusyNtSys) DGSys_NOM, m_anaType) && nCjj<2){
      _jj += (*jets->at(ijet));
      nCjj++;
    }
    //cout << "SJ " << _j->Pt() << " " << ijet << " " << _b_j_pt[ijet] << endl;

    //Count preLep (pT>6 inside jet)
    for(uint iele=0; iele<preElectrons->size(); iele++){
      const Susy::Electron* _e = preElectrons->at(iele);
      float dR = _e->DeltaR(*_j);
      if(dR<E_J_DR) _b_j_nEle[ijet]++;
    }
    for(uint imuo=0; imuo<preMuons->size(); imuo++){
      const Susy::Muon* _m = preMuons->at(imuo);
      float dR = _m->DeltaR(*_j);
      if(dR<M_J_DR) _b_j_nMu[ijet]++;
    }

  }
  
  if(jets->size()>1){
    _b_dRjj = jets->at(0)->DeltaR(*jets->at(1));
    _b_dEtajj = jets->at(0)->Eta() - jets->at(1)->Eta();
  }

  if(nCjj>=2){
    _b_mjj = _jj.M();
    _b_pTjj = _jj.Pt();
  }


  //cout << "nSJets " << _b_nSJets << " nCJets " << _b_nCJets 
  //<< " nBJets " << _b_nBJets << " nFJets " << _b_nFJets << endl;
  //getAnaType();

}

//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTreeMetVar(const Met* met,float metrel)
{
  _b_met= met->lv().Pt();
  _b_met_phi = met->lv().Phi();
  _b_metrel=metrel;
  _b_met_refEle=met->refEle;
  _b_met_refMuo=met->refMuo;
  _b_met_refJet=met->refJet;
  _b_met_cellout=met->refCell;
}

//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTreeEventVar(float mlj, float mljj,
			     float sphericity, float sphericityTrans, 
			     float llAcoplanarity, float jjAcoplanarity,
			     bool topTag, float mllCollApprox)
{
  _b_mlj=mlj;
  _b_mljj=mljj;

  _b_sphericity=sphericity;  
  _b_sphericityTrans=sphericityTrans;  
  _b_llAcoplanarity=llAcoplanarity;
  _b_jjAcoplanarity=jjAcoplanarity;

  _b_topTag = topTag;

  _b_mll_collApprox = mllCollApprox;

}

//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTrigger(const Susy::Event* evt,const LeptonVector* leptons)
{
  long long evtflag =evt->trigFlags;

  _b_trig_e12_mu8 = evtflag & TRIG_e12Tvh_medium1_mu8;
  _b_trig_mu18_e7 = evtflag & TRIG_mu18_tight_e7_medium1;
  _b_trig_e60     = evtflag & TRIG_e60_medium1;
  _b_trig_mu36    = evtflag & TRIG_mu36_tight;
  _b_trig_e24i    = evtflag & TRIG_e24vhi_medium1;
  _b_trig_mu24i   = evtflag & TRIG_mu24i_tight;


  for(uint ilep=0; ilep<leptons->size(); ilep++){
    const Susy::Lepton* _l = leptons->at(ilep);
    long long trigflag =_l->trigFlags;
    _b_l_e7[ilep]    = trigflag & TRIG_e7_medium1;
    _b_l_e12[ilep]   = trigflag & TRIG_e12Tvh_medium1;
    _b_l_e60[ilep]   = trigflag & TRIG_e60_medium1;
    _b_l_e24i[ilep]  = trigflag & TRIG_e24vhi_medium1;
    _b_l_mu8[ilep]   = trigflag & TRIG_mu8;
    _b_l_mu18[ilep]  = trigflag & TRIG_mu18_tight;
    _b_l_mu36[ilep]  = trigflag & TRIG_mu36_tight ;
    _b_l_mu24i[ilep] = trigflag & TRIG_mu24i_tight;
  }

}

//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillMCT(float mct, float mctPerp, float mctPara)
{
  _b_mct = mct;
  _b_mctPerp = mctPerp;
  _b_mctPara = mctPara;
}

//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillMT2(float mT2, float mT2jj, float mT2J)
{
  _b_mT2=mT2;
  _b_mT2jj=mT2jj;
  _b_mT2J=mT2J;
}

//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillJZB(float JZBjets, float JZBmet)
{
  _b_JZBjets = JZBjets;
  _b_JZBmet = JZBmet;
}

//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillLFV(const LeptonVector* leptons, const Met* met, const JetVector* jets)
{
  _b_mcoll   = mColl(*leptons->at(0),*leptons->at(1),met->lv());
  
  //Substract the soft Term from met
  float met_x = met->lv().Px()-met->softTerm_etx;
  float met_y = met->lv().Py()-met->softTerm_ety;
  TVector2 metCorr(met_x,met_y);
  _b_metCorr = metCorr.Mod();// sqrt(met_x*met_x + met_y*met_y);
  _b_metCorrPhi = metCorr.Phi();

  TLorentzVector metLV(metCorr.Mod(),0,metCorr.Phi(),metCorr.Mod());
  Susy::Met corrMet = *met;
  corrMet.Et = metCorr.Mod();
  corrMet.phi = metCorr.Phi();
  
  _b_metrelCorr   = getMetRel(&corrMet, *leptons, *jets);
  _b_mcollCorr    = mColl(*leptons->at(0),*leptons->at(1),metLV);
  _b_mTl0Corr     =  mT(*leptons->at(0), corrMet.lv());  
  _b_mTl1Corr     =  mT(*leptons->at(1), corrMet.lv());  
  


}
//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillRazor(const LeptonVector* leptons, const Met* met)
{
  superRazor( *leptons, met, 
	      *_b_vBeta_z, *_b_pTCM, *_b_vBetaT_CMtoR, *_b_vBetaR,
	      _b_shatr, _b_dphi_ll_vBetaT, _b_dphi_l1l2,
	      _b_gammaR, _b_dphi_vBetaR_vBetaT, _b_mDeltaR, _b_cosThetaRp1,
	      _b_cosTheta_b);
}

//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTreeOtherJets(JetVector* jets, const LeptonVector* leptons, const Met* met)
{

  int idxj0=-999;
  int iijet = _b_nJets;
  for(uint ijet=0; ijet<jets->size(); ijet++){
    if(iijet>25) continue;
    const Susy::Jet* _j = jets->at(ijet); 
    if(isSignalJet2Lep(_j)) continue;
    if(idxj0<0) idxj0=ijet;
    _b_nOJets++;
    _b_nJets++;
    _b_j_pt[iijet]  = _j->Pt();
    _b_j_eta[iijet] = _j->Eta();
    _b_j_phi[iijet] = _j->Phi();
    _b_j_e[iijet]   = _j->E();
    _b_j_jvf[iijet] = _j->jvf;
    _b_j_mv1[iijet] = _j->mv1;
    _b_j_isTruth[iijet] = _j->matchTruth;
    _b_j_label[iijet] = _j->truthLabel;
    _b_j_isOJ[iijet] = true;

    float _dPhi=fabs(met->lv().DeltaPhi(*_j));
    if(_dPhi<_b_dphi_metcoj) _b_dphi_metcoj=_dPhi;
    iijet++;

  }

  TLorentzVector _ll;
  for(uint ilep=0; ilep<leptons->size(); ilep++){
     const Susy::Lepton* _l = leptons->at(ilep);
     if(ilep>1) continue;
    _ll = _ll + (*_l);
  }
  
  if(idxj0>-1) _b_dphi_ll_oj1 = fabs(_ll.DeltaPhi(*jets->at(idxj0)));

  findRecoilJet();
}


//-----------------------------------------------------------------------------------------------------------
// Quick and dirty ZPt balance - use recoil jet to measure JVF
void ToyNt::findRecoilJet()
{
  //See 
  //https://cdsweb.cern.ch/record/1452641/files/ATLAS-CONF-2012-053.pdf 
  //

  //
  // Selection
  //  jet |eta|<1.2
  //  dR(j,l)>0.35  //implied by our selection already
  //  subleading jet pT < 20% of ZpT
  //

  //
  //Find the 2 leading jets within |eta|<1.2
  //
  static const float maxEta = 2.5;
  int idxj0  = -999;
  int idxj1  = -999;
  float j0Pt = -999;
  float j1Pt = -999;
  for(int ijet=0; ijet<_b_nJets; ijet++){
    if(fabs(_b_j_eta[ijet])>maxEta) continue;
    if(_b_j_pt[ijet]>j0Pt){
      idxj0=ijet;
      j0Pt = _b_j_pt[ijet];
      //cout << "found lead " << idxj0 << " " <<_b_j_pt[ijet] << endl;
    }
  }
  for(int ijet=0; ijet<_b_nJets; ijet++){
    if(fabs(_b_j_eta[ijet])>maxEta) continue;
    if(ijet == idxj0) continue;
    if(_b_j_pt[ijet]>j1Pt){
      idxj1=ijet;
      j1Pt = _b_j_pt[ijet];
      //cout << "found sublead " << idxj1 << " " <<_b_j_pt[ijet] << endl;
    }
  }

  //cout << "Z " << _b_pTll << " phi " << _b_phill << endl;

  if( (idxj1>-999 &&_b_j_pt[idxj1]/_b_pTll < 0.2) || idxj1<0){
    if(idxj1>-1) _b_j_isSublead[idxj1] = true;
    if(idxj0>-1) _b_j_isRecoil[idxj0] = true;

    if(idxj0>-1){
      //cout << "\tRecoil " << idxj0 << " " << _b_j_pt[idxj0] 
      //<< " " << _b_j_phi[idxj0] << endl;
      _b_dphi_Zj = acos(cos(_b_phill-_b_j_phi[idxj0]));
    }
    //if(idxj1>-1)
      //cout << "\tSublead " << idxj1 << " " << _b_j_pt[idxj1] 
      //<< " " << _b_j_phi[idxj1] << endl;
    
    /*
    if(idxj0>-1)
      cout << "\tDphi(jr,ll) " << _b_dphi_Zj
	   << " pTbal " << _b_j_pt[idxj0]/_b_pTll
	   << "\n" << endl ;
    */
  }
  

}


//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTreeFakeLeptons(const LeptonVector* leptons,
				ElectronVector& baseElectrons, MuonVector& baseMuons,
				const Met* met, int nVtx, bool isMc)
{
 
  bool T1  = _b_l_isT[0];
  bool T2  = _b_l_isT[1];
  int catg = -1;
  if     ( T1  && T2  ) catg = 0; //TT=0
  else if( T1  && !T2 ) catg = 1; //TL=1
  else if( !T1 && T2  ) catg = 2; //LT=2
  else if( !T1 && !T2 ) catg = 3; //LL=3
  _b_ll_FType = catg;

  
}



//-----------------------------------------------------------------------------------------------------------
void ToyNt::setSumOfMcWeights(double sumOfMcWeights) {
  // Define histogram
  TH1D *sumwhist = new TH1D("sumOfMcWeights_"+_mcid,"sumOfMcWeights_"+_mcid,1,0.,1.);
  
  // Fill histogram
  sumwhist -> Fill( 0. , sumOfMcWeights ) ;
  
  // Write intLumi to file
  file->cd();
  sumwhist->SetDirectory(file);
  sumwhist->Write();
  sumwhist->SetDirectory(0);
  
  delete sumwhist;
  
  return;
}

