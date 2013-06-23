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
  

  tree->Branch("run",&_b_run,"run/I");
  tree->Branch("event",&_b_event,"event/I");
  tree->Branch("npv",&_b_npv,"npv/F");
  tree->Branch("npvCorr",&_b_npvCorr,"npvCorr/F");
  tree->Branch("iSR",&_b_iSR,"iSR/I");
  tree->Branch("llType",&_b_llType,"llType/I");
  tree->Branch("w",&_b_w,"w/D");

  tree->Branch("nlep",&_b_nlep,"nlep/I");
  tree->Branch("l_pt",_b_l_pt,"l_pt[nlep]/F");
  tree->Branch("l_eta",_b_l_eta,"l_eta[nlep]/F");
  tree->Branch("l_phi",_b_l_phi,"l_phi[nlep]/F");
  tree->Branch("l_e",_b_l_e,"l_e[nlep]/F");
  tree->Branch("l_q",_b_l_q,"l_q[nlep]/I");
  tree->Branch("l_ptcone30",_b_l_ptcone30,"l_ptcone30[nlep]/F");
  tree->Branch("l_etcone30",_b_l_etcone30,"l_etcone30[nlep]/F");
  tree->Branch("l_etconetopo30",_b_l_etconetopo30,"l_etconetopo30[nlep]/F");
  tree->Branch("l_d0",_b_l_d0,"l_d0[nlep]/F");
  tree->Branch("l_z0",_b_l_z0,"l_z0[nlep]/F");
  tree->Branch("l_isEle",_b_l_isEle,"l_isEle[nlep]/O");
  tree->Branch("dphi_metl",_b_dphi_metl,"dphi_metl[nlep]/F");
  tree->Branch("mTl",_b_mTl,"mTl[nlep]/F");

  tree->Branch("pTll",&_b_pTll,"pTll/F");
  tree->Branch("phill",&_b_phill,"phill/F");
  tree->Branch("mll",&_b_mll,"mll/F");
  tree->Branch("mll_collApprox",&_b_mll_collApprox,"mll_collApprox/F");
  tree->Branch("dR_ll",&_b_dR_ll,"dR_ll/F");
  tree->Branch("dphi_ll",&_b_dphi_ll,"dphi_ll/F");
  tree->Branch("isOS",&_b_isOS,"isOS/O");

  tree->Branch("nJets",&_b_nJets,"nJets/I");
  tree->Branch("nSJets",&_b_nSJets,"nSJets/I");
  tree->Branch("nCJets",&_b_nCJets,"nCJets/I");
  tree->Branch("nBJets",&_b_nBJets,"nBJets/I");
  tree->Branch("nFJets",&_b_nFJets,"nFJets/I");
  tree->Branch("nOJets",&_b_nOJets,"nOJets/I");

  tree->Branch("j_isC20",_b_j_isC20,"j_isC20[nJets]/O");
  tree->Branch("j_isB20",_b_j_isB20,"j_isB20[nJets]/O");
  tree->Branch("j_isF30",_b_j_isF30,"j_isF30[nJets]/O");
  tree->Branch("j_isOJ", _b_j_isOJ, "j_isOJ[nJets]/O");

  tree->Branch("j_pt",_b_j_pt,"j_pt[nJets]/F");
  tree->Branch("j_eta",_b_j_eta,"j_eta[nJets]/F");
  tree->Branch("j_phi",_b_j_phi,"j_phi[nJets]/F");
  tree->Branch("j_e",_b_j_e,"j_e[nJets]/F");
  tree->Branch("j_jvf",_b_j_jvf,"j_jvf[nJets]/F");
  tree->Branch("j_mv1",_b_j_mv1,"j_mv1[nJets]/F");
  tree->Branch("j_isTruth",_b_j_isTruth,"j_isTruth[nJets]/O");
  tree->Branch("j_label",_b_j_label,"j_label[nJets]/I");
  tree->Branch("j_isRecoil",_b_j_isRecoil,"j_isRecoil[nJets]/O");
  tree->Branch("j_isSublead",_b_j_isSublead,"j_isSublead[nJets]/O");

  tree->Branch("met",&_b_met,"met/F");
  tree->Branch("met_phi",&_b_met_phi,"met_phi/F");
  tree->Branch("metrel",&_b_metrel,"metrel/F");
  tree->Branch("met_refEle",&_b_met_refEle,"met_refEle/F");
  tree->Branch("met_refMuo",&_b_met_refMuo,"met_refMuo/F");
  tree->Branch("met_refJet",&_b_met_refJet,"met_refJet/F");
  tree->Branch("met_cellout",&_b_met_cellout,"met_cellout/F");

  tree->Branch("mWWT",&_b_mWWT,"mWWT/F");

  tree->Branch("dphi_metcl",&_b_dphi_metcl,"dphi_metcl/F");

  tree->Branch("dphi_metcj",&_b_dphi_metcj,"dphi_metcj/F");
  tree->Branch("dphi_metcoj",&_b_dphi_metcoj,"dphi_metcoj/F");

  tree->Branch("dphi_ll_j1",&_b_dphi_ll_j1,"dphi_ll_j1/F");
  tree->Branch("dphi_ll_oj1",&_b_dphi_ll_oj1,"dphi_ll_oj1/F");
  tree->Branch("dphi_Zj",&_b_dphi_Zj,"dphi_Zj/F");

  tree->Branch("mT2",&_b_mT2,"mT2/F");
  tree->Branch("mT2jj",&_b_mT2jj,"mT2jj/F");
  tree->Branch("sphericity",&_b_sphericity,"sphericity/F");
  tree->Branch("sphericityTrans",&_b_sphericityTrans,"sphericityTrans/F");
  tree->Branch("llAcoplanarity",&_b_llAcoplanarity,"llAcoplanarity/F");
  tree->Branch("jjAcoplanarity",&_b_jjAcoplanarity,"jjAcoplanarity/F");
  tree->Branch("topTag",&_b_topTag,"topTag/O");
  tree->Branch("mEff",&_b_mEff,"mEff/F");
  tree->Branch("ST",&_b_ST,"ST/F");
  tree->Branch("mjj",&_b_mjj,"mjj/F");

  tree->Branch("mct",&_b_mct,"mct/F");
  tree->Branch("mctPerp",&_b_mctPerp,"mctPerp/F");
  tree->Branch("mctPara",&_b_mctPara,"mctPara/F");

  tree->Branch("JZBjets",&_b_JZBjets,"JZBjets/F");
  tree->Branch("JZBmet",&_b_JZBmet,"JZBmet/F");

  clearOutputBranches();
  return;

}
//-----------------------------------------------------------------------------------------------------------
void ToyNt::clearOutputBranches(void) {
  _b_run=-1;
  _b_event=-1;
  _b_npv=-1;
  _b_npvCorr=-1;
  _b_iSR=-1; 
  _b_llType = -1;
  _b_w=0;

  _b_nlep=0;
  for(uint i=0; i<2; i++){
    _b_l_pt[i]=-999;
    _b_l_eta[i]=-999;
    _b_l_phi[i]=-999;
    _b_l_e[i]=-999;
    _b_l_q[i]=0;
    _b_l_ptcone30[i]=-999;
    _b_l_etcone30[i]=-999;
    _b_l_etconetopo30[i]=-999;
    _b_l_d0[i]=-999;
    _b_l_z0[i]=-999;
    _b_l_isEle[i]=false;
    _b_dphi_metl[i]=-999; 
    _b_mTl[i]=-999;

  }
  
  _b_pTll=-999;
  _b_phill=-999;
  _b_mll=-999;
  _b_mll_collApprox=-999;
  _b_dR_ll=-999;
  _b_dphi_ll=-999;
  _b_isOS=false;

  _b_nJets=0; 
  _b_nSJets=0; 
  _b_nCJets=0; 
  _b_nBJets=0; 
  _b_nFJets=0;
  _b_nOJets=0;

  for(uint i=0; i<25; i++){
    _b_j_isC20[i]=false;
    _b_j_isB20[i]=false;
    _b_j_isF30[i]=false;
    _b_j_isOJ[i]=false;
    
    _b_j_pt[i]=-999;
    _b_j_eta[i]=-999;
    _b_j_phi[i]=-999;
    _b_j_e[i]=-999;
    _b_j_jvf[i]=-999;
    _b_j_mv1[i]=-999;
    _b_j_isTruth[i]=false;
    _b_j_label[i]=-999;

    _b_j_isRecoil[i]=false;
    _b_j_isSublead[i]=false;

    /*
    _b_oj_pt[i]=-999;
    _b_oj_eta[i]=-999;
    _b_oj_phi[i]=-999;
    _b_oj_e[i]=-999;
    _b_oj_jvf[i]=-999;
    */
  }

  _b_met=-999;
  _b_met_phi=-999;
  _b_metrel=-999;
  _b_met_refEle=-999;
  _b_met_refMuo=-999;
  _b_met_refJet=-999;
  _b_met_cellout=-999;
  
  _b_mWWT=-999;
  
  _b_dphi_metcl=-999;

  _b_dphi_metcj=999;
  _b_dphi_metcoj=999;

  _b_dphi_ll_j1=-999;
  _b_dphi_ll_oj1=-999;

  _b_dphi_Zj=-999;

  _b_mT2=-999;
  _b_mT2jj=-999;
  _b_sphericity=-999;
  _b_sphericityTrans=-999;
  _b_llAcoplanarity=-999;
  _b_jjAcoplanarity=-999;
  _b_topTag=false;
  _b_mEff=-999;
  _b_ST=-999;
  _b_mjj=-999;

  _b_mct=-999;
  _b_mctPerp=-999;
  _b_mctPara=-999;
  _b_JZBjets=-999;
  _b_JZBmet=-999;

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
void ToyNt::FillTreeEvent(int run, int event, float  npv, float npvCorr, int iSR, int llType, double w)
{
  _b_run = run;
  _b_event = event;
  _b_npv = npv;
  _b_npvCorr = npvCorr;
  _b_iSR = iSR;
  _b_llType = llType;
  _b_w   = w;
  
}
//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTreeLeptons(const LeptonVector* leptons,
			    ElectronVector& baseElectrons, MuonVector& baseMuons, 
			    const Met* met, int nVtx, bool isMc)
{
  if(leptons->size()==2) _b_nlep=2;
  else{ 
    cerr << "FillTreeLeptons - too many " << leptons->size() << endl;
    abort();
  }
  
  //  SusyNtTools* _ntTools = new SusyNtTools();

  _b_dphi_metcl=999; 
  _b_dphi_ll=999; 
  int qqType=1;
  TLorentzVector _ll;
  for(uint ilep=0; ilep<leptons->size(); ilep++){
     const Susy::Lepton* _l = leptons->at(ilep);
    _ll = _ll + (*_l);

    _b_l_pt[ilep] = _l->Pt();
    _b_l_eta[ilep] = _l->Eta();
    _b_l_phi[ilep] = _l->Phi();
    _b_l_e[ilep] = _l->E();
    _b_l_q[ilep] = _l->q;
    _b_l_d0[ilep] = _l->d0Unbiased;
    _b_l_z0[ilep] = _l->z0Unbiased;
    if(_l->isEle()){
      Electron* _e = (Electron*) _l;
      float etconetopo =  elEtTopoConeCorr(_e, baseElectrons, baseMuons,nVtx, isMc);
      _b_l_etcone30[ilep] = _e->etcone30Corr;
      _b_l_etconetopo30[ilep] = etconetopo;
      _b_l_ptcone30[ilep] = _l->ptcone30;
    }
    else{
      Muon* _m = (Muon*) _l;
      float ptcone = muPtConeCorr(_m, baseElectrons, baseMuons, nVtx,isMc);
      _b_l_etcone30[ilep] = _m->etcone30;
      _b_l_ptcone30[ilep] = ptcone;
    }

    _b_l_isEle[ilep] = _l->isEle();
    _b_dphi_metl[ilep]=fabs(met->lv().DeltaPhi(*_l));
    _b_mTl[ilep] = mT(*_l, met->lv());

    float _dPhi=fabs(met->lv().DeltaPhi(*_l));
    if(_dPhi<_b_dphi_metcl) _b_dphi_metcl=_dPhi;
    qqType *= _l->q;
  }

  _b_pTll = _ll.Pt();
  _b_phill = _ll.Phi();
  _b_mll = _ll.M();
  _b_mWWT = mT(_ll, met->lv());
  _b_dphi_ll = fabs(leptons->at(0)->DeltaPhi(*leptons->at(1)));
  _b_dR_ll = leptons->at(0)->DeltaR(*leptons->at(1));
  _b_isOS = (qqType<0) ? true : false;

  //  delete _ntTools;
}
//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTreeEventVar(const Met* met,float metrel, float mT2, 
			     float mT2jj, float sphericity, float sphericityTrans, 
			     float llAcoplanarity, float jjAcoplanarity,
			     bool topTag, float mllCollApprox)
{
  _b_met= met->lv().Pt();
  _b_met_phi = met->lv().Phi();
  _b_metrel=metrel;
  _b_met_refEle=met->refEle;
  _b_met_refMuo=met->refMuo;
  _b_met_refJet=met->refJet;
  _b_met_cellout=met->refCell;
  _b_mT2=mT2;
  _b_mT2jj=mT2jj;
  _b_sphericity=sphericity;  
  _b_sphericityTrans=sphericityTrans;  
  _b_llAcoplanarity=llAcoplanarity;
  _b_jjAcoplanarity=jjAcoplanarity;
  _b_mll_collApprox = mllCollApprox;
  _b_topTag = topTag;

}
//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTreeSignalJets(const JetVector* jets, const LeptonVector* leptons, const Met* met)
{
  _b_nSJets = jets->size();
  if(_b_nSJets==0) return;

  //  SusyNtTools* _ntTools = new SusyNtTools(); 

  TLorentzVector _ll;
  for(uint ilep=0; ilep<leptons->size(); ilep++){
     const Susy::Lepton* _l = leptons->at(ilep);
     if(ilep>1) continue;
    _ll = _ll + (*_l);
  }
 
  TLorentzVector _jj;
  _b_ST=0;
  int njj=0;
  for(uint ijet=0; ijet<jets->size(); ijet++){
    if(ijet>25) continue;
    const Susy::Jet* _j = jets->at(ijet); 
    _b_nJets++;

    if(ijet==0) _b_dphi_ll_j1 = fabs(_ll.DeltaPhi(*jets->at(ijet)));

    if(isCentralLightJet(_j)){
      _b_nCJets++;
      _b_j_isC20[ijet] = true;
    }
    else if(isForwardJet(_j)){
      _b_nFJets++;
      _b_j_isF30[ijet] = true;
    }
    else if(isCentralBJet(_j)){
      _b_nBJets++;
      _b_j_isB20[ijet] = true;
    }

    _b_j_pt[ijet]  = _j->Pt();
    _b_j_eta[ijet] = _j->Eta();
    _b_j_phi[ijet] = _j->Phi();
    _b_j_e[ijet]   = _j->E();
    _b_j_jvf[ijet] = _j->jvf;
    _b_j_mv1[ijet] = _j->mv1;
    _b_j_isTruth[ijet] = _j->matchTruth;
    _b_j_label[ijet] = _j->truthLabel;
    
    float _dPhi=fabs(met->lv().DeltaPhi(*_j));
    if(_dPhi<_b_dphi_metcj) _b_dphi_metcj=_dPhi;
    _b_ST += _j->Pt();
    if(isCentralLightJet(_j) && njj<2){
      _jj += (*jets->at(ijet));
      njj++;
    }
    
    //cout << "SJ " << _j->Pt() << " " << ijet << " " << _b_j_pt[ijet] << endl;

  }

  _b_mjj = _jj.M();
  _b_mEff = _b_ST + met->lv().Pt();

  
  //  delete _ntTools;

}
//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillTreeOtherJets(JetVector* jets, const LeptonVector* leptons, const Met* met)
{

  //  SusyNtTools* _ntTools = new SusyNtTools();

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

    //cout << "OJ " << _j->Pt() << " " << iijet << " " << _b_j_pt[iijet] << endl;

    float _dPhi=fabs(met->lv().DeltaPhi(*_j));
    if(_dPhi<_b_dphi_metcoj) _b_dphi_metcoj=_dPhi;
    iijet++;

  }
  //cout << endl;

  TLorentzVector _ll;
  for(uint ilep=0; ilep<leptons->size(); ilep++){
     const Susy::Lepton* _l = leptons->at(ilep);
     if(ilep>1) continue;
    _ll = _ll + (*_l);
  }
  
  if(idxj0>-1) _b_dphi_ll_oj1 = fabs(_ll.DeltaPhi(*jets->at(idxj0)));

  findRecoilJet();
  //  delete _ntTools;
}
//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillMCT(float mct, float mctPerp, float mctPara)
{
  _b_mct = mct;
  _b_mctPerp = mctPerp;
  _b_mctPara = mctPara;
}

//-----------------------------------------------------------------------------------------------------------
void ToyNt::FillJZB(float JZBjets, float JZBmet)
{
  _b_JZBjets = JZBjets;
  _b_JZBmet = JZBmet;
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
