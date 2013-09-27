#include <iomanip>
#include "SusyWeakProdAna/Susy3LepAna.h"
#include "SusyWeakProdAna/PhysicsTools.h"

#include "SusyWeakProdAna/SusyAnaCommon.h"

using namespace std;
using namespace Susy;



/*--------------------------------------------------------------------------------*/
// Susy3LepAna Constructor
/*--------------------------------------------------------------------------------*/
Susy3LepAna::Susy3LepAna(SusyHistos* _histos) :
  SusyBaseAna(_histos)
{
  cout << "-------------------------------" << endl;
  cout << "    Running 3L/4L Analysis        " << endl;
  cout << "-------------------------------" << endl;

}

/*--------------------------------------------------------------------------------*/
// Main process loop function 
/*--------------------------------------------------------------------------------*/
void Susy3LepAna::doAnalysis(unsigned int isys)
{
  reset();
  SYST = isys;
  
  //Call this here, since have multiple instances of SusyNtTools
  //setAnaType(Ana_2Lep); //Use 2L iso
  //setAnaType(Ana_3Lep); //Use 3L iso
  
  // Check Event
  if(m_useLooseLep){  //use baseline leptons - for fake MM estimate
    if(!CUTFLOW && v_baseLep->size()<3) return;
    if(!selectEvent(v_baseLep, v_sigTau, v_sigJet, m_met)) return;
  }
  else{
    if(!CUTFLOW && v_baseLep->size()<3) return;
    if(!selectEvent(v_sigLep, v_sigTau, v_sigJet, m_met)) return;
  }

  return;
}

/*--------------------------------------------------------------------------------*/
// The Terminate() function is the last function to be called
/*--------------------------------------------------------------------------------*/
void Susy3LepAna::end()
{
  _hh->Sum3LHistograms();
  sumArray();

  cout << endl;
  cout << endl;
  cout << "**********************************************" << endl;
  cout << "Susy3LepAna event counters"    << endl;
  cout << std::setprecision(0) << std::fixed;
  cout << "read in:            " << n_readin        << endl;
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

  cout << "pass Tau Veto       " << n_pass_tauVeto[ET_lll]  << endl;
  cout << endl;
  cout << "pass 3L             " << n_pass_nBase3Lep   << endl;
  cout << "pass 4L             " << n_pass_nBase4Lep   << endl;


  //3 Leptons
  cout << "=============================="<<endl;
  cout << "         3 LEPTONS            "<<endl;
  cout << "=============================="<<endl;
  cout << "\t\t\t";
  for(int i=ET_eee; i<=ET_lll; i++)   cout << ML_FLAV[i] << "\t";
  cout << endl;
      
  print_CF3L();
  print_VRWZ();

  //4 Leptons
  cout << "=============================="<<endl;
  cout << "         4 LEPTONS            "<<endl;
  cout << "=============================="<<endl;
  cout << "\t\t\t";
  for(int i=ET_eeee; i<=ET_llll; i++)  cout << ML_FLAV[i] << "\t";
  cout << endl;

  print_VRZZ();


  finish();
}

/*--------------------------------------------------------------------------------*/
// Set Selection 
/*--------------------------------------------------------------------------------*/
void Susy3LepAna::setSelection(std::string s)
{
  std::string m_sel = s;
  resetCuts();

  //----------------------------//
  // Validation for 2L diboson WZ/ZZ
  //----------------------------//

  if(m_sel=="CF3L"){
    m_cutNBaseLep = true;
    m_nLep3Min=3;
    m_nLep3Max=3;
    m_selSFOS=true;
    m_metMin=75;
    m_vetoZ=true;
    //m_vetoExtZ=true;??
    m_vetoB=true;
  }

  if(m_sel=="VRWZ"){
    m_cutNBaseLep = true;
    m_nLep3Min=3;
    m_nLep3Max=3;
    m_selSFOS=true;
    m_selZ=true;
    m_metMin=30;
    m_vetoB=true;
    m_mtMin=40;
  }
  if(m_sel=="VRZZ"){
    m_cutNBaseLep = true;
    m_nLep4Min=4;
    m_nLep4Max=4;
    m_metMax=50;
    m_selZ=true;
    m_vetoB=true;
    m_selZllZll=true;
  }
  if(m_sel=="VRemulWW"){ //Emulate WW from WZ events
    m_cutNBaseLep = true;
    m_nLep3Min=3;
    m_nLep3Max=3;
    m_selSFOS=true;
    m_selZ=true;
    //m_metMin=30;
    m_vetoB=true;
    //m_mtMin=40;
  }

 
}
/*--------------------------------------------------------------------------------*/
// Full event selection
/*--------------------------------------------------------------------------------*/
bool Susy3LepAna::selectEvent(LeptonVector* leptons, 
			      const TauVector* taus,
			      const JetVector* jets, const Met* met)
{
  //Set increment to mc weight (otherwise confusing w/ sample w/ -1 weight)
  if(nt->evt()->isMC) _inc = nt->evt()->w; 
  else _inc =1;
  if(nt->evt()->isMC && !WEIGHT_COUNT && WEIGHT_ONE) _inc=1;
  
  if(SYST==DGSys_NOM) n_readin+=_inc;


  if(!passEventCleaning() ) return false;
  if( !passTauVeto(v_sigTau) ){
    if(dbg()>10) cout << "Fail Tau veto " << endl;
    return false;
  }
  
  //Backup met/lepton
  saveOriginal(); //Backup Met & leptons  --> for WW emulation

  int nTightLep=0;
  for(uint iSR=0; iSR<ML_NSR; iSR++){
    string sSR=ML_SRNAME[iSR];
    setSelection(sSR);
    SR=iSR;

    //Restore leptons/met
    restoreOriginal(*leptons,met);

    float _ww = eventWeight(LUMIMODE); //set _ww to the appropriate weighting
    if(!WEIGHT_COUNT) _ww=1;
    float _lepSFW  = getLepSFWeight(leptons);
    if(WEIGHT_COUNT)  _ww *= _lepSFW;
    float _wwBck= _ww;
    
    if(WEIGHT_COUNT) _inc = _ww;
    else{
      if(!WEIGHT_ONE) _inc = nt->evt()->w;
    }

    if(iSR==ML_CF3L){
      if(!passNBase3LepCut(leptons)) continue;
      nTightLep=3;
    }
    else if(iSR==ML_VRZZ){
      if(!passNBase4LepCut(leptons)) continue;
      nTightLep=4;
    }

   

    //*********************//
    //  >=4 Leptons
    //*********************//
    if(iSR>=ML_VRZZ && nTightLep==4){ 
       int icut=0;
       m_ET =  get4LType(*leptons);
       if(dbg()>5) cout << "** SR" << ML_SRNAME[iSR] << "\t4L " << ML_FLAV[m_ET] << endl;

       if(!passNLep4Cut(leptons)) continue;

       if(!pass3LTrigger(leptons,taus,m_trig3LObj))  continue;
  
       _hh->H1FILL(_hh->ML_cutflow[SR][m_ET][SYST],icut++,_ww);

       if(!passZCut(leptons))     continue;
       _hh->H1FILL(_hh->ML_cutflow[SR][m_ET][SYST],icut++,_ww); 


       if( !passMET (&new_met))     continue;
       _hh->H1FILL(_hh->ML_cutflow[SR][m_ET][SYST],icut++,_ww); 
              
       float bWeight = getBTagSF(nt->evt(),v_sigJet,SYST);
       if(USE_BWEIGHT && nt->evt()->isMC) {
	 _ww *= bWeight;
	 if(WEIGHT_COUNT) _inc = _ww;
       }
       
       if( !passBJet(jets))        continue;
       _hh->H1FILL(_hh->ML_cutflow[SR][m_ET][SYST],icut++,_ww);

       if( !passZllZll(leptons))        continue;
       _hh->H1FILL(_hh->ML_cutflow[SR][m_ET][SYST],icut++,_ww);

    }
    //*********************//
    //  3 Leptons
    //*********************//
    else{ 
      if(nTightLep!=3) continue;
      int icut=0;
      
      m_ET =  get3LType(*leptons);
      if(dbg()>5) cout << "** SR: " << ML_SRNAME[iSR] << "\t3L " << ML_FLAV[m_ET] << endl;

      if(!passNLep3Cut(leptons)) continue;

      if(!pass3LTrigger(leptons,taus,m_trig3LObj))  continue;
      
      _hh->H1FILL(_hh->ML_cutflow[SR][m_ET][SYST],icut++,_ww);

      if( !passSFOSCut(leptons)) continue;
      _hh->H1FILL(_hh->ML_cutflow[SR][m_ET][SYST],icut++,_ww); 
      
      if(!passZCut(leptons))     continue;
      _hh->H1FILL(_hh->ML_cutflow[SR][m_ET][SYST],icut++,_ww); 

      if(iSR==ML_VRemulWW){
	bool isOK = emulateWW(leptons,&new_met);
	if(!isOK){
	  cout << "WW emulation issue run " << nt->evt()->run 
	       << " event " << nt->evt()->event  << endl;
	  abort();
	}
      }
      
      if( !passMET (&new_met))     continue;
      _hh->H1FILL(_hh->ML_cutflow[SR][m_ET][SYST],icut++,_ww); 
      
      float bWeight = getBTagSF(nt->evt(),v_sigJet,SYST);
      if(USE_BWEIGHT && nt->evt()->isMC) {
	_ww *= bWeight;
	if(WEIGHT_COUNT) _inc = _ww;
      }
     
      if( !passBJet(jets))        continue;
      _hh->H1FILL(_hh->ML_cutflow[SR][m_ET][SYST],icut++,_ww);

      if( !passMtCut(leptons,&new_met))        continue;
      _hh->H1FILL(_hh->ML_cutflow[SR][m_ET][SYST],icut++,_ww);


      /*
      if(dbg()==-99 && hasSFOS(*v_sigLep)){
	uint zl1, zl2;
	float mT=0;
	findBestZ(zl1, zl2, *v_sigLep);
	for(uint iL=0; iL<v_sigLep->size(); iL++)	{
	  if(iL!=zl1 && iL!=zl2)  mT= Mt(v_sigLep->at(iL),m_met);
	}
	cout << "PASS " << ML_SRNAME[SR][m_ET][SYST] 
	     << " " << nt->evt()->run << " " << nt->evt()->event 
	     << " " << v_sigJet->size() << " " << mT
	     <<endl;
      }
      */
      
 
    }
    
    if(dbg()==-99) cout << "PASS " << ML_SRNAME[SR] 
			<< " " << nt->evt()->run 
			<< " " << nt->evt()->event <<endl;
    fillHistograms(iSR, SYST, leptons, jets, &new_met,_ww);
  }


  restoreOriginal(*leptons,met);

  return true;
}


/*--------------------------------------------------------------------------------*/
void Susy3LepAna::print_CF3L(){

  int j= ML_CF3L;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << ML_SRNAME[j] <<endl;

  print_line("pass Trig        ", ET_eee, ET_lll, j, n_pass_3Ltrig);
  print_line("pass SFOS        ", ET_eee, ET_lll, j, n_pass_sfos);
  print_line("pass Zveto       ", ET_eee, ET_lll, j, n_pass_Z);
  print_line("pass MET         ", ET_eee, ET_lll, j, n_pass_met);
  print_line("pass Bveto       ", ET_eee, ET_lll, j, n_pass_BJet);
  
}

/*--------------------------------------------------------------------------------*/
void Susy3LepAna::print_VRWZ(){

  int j= ML_VRWZ;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << ML_SRNAME[j] <<endl;

  print_line("pass Trig        ", ET_eee, ET_lll, j, n_pass_3Ltrig);
  print_line("pass SFOS        ", ET_eee, ET_lll, j, n_pass_sfos);
  print_line("pass selZ        ", ET_eee, ET_lll, j, n_pass_Z);
  print_line("pass MET         ", ET_eee, ET_lll, j, n_pass_met);
  print_line("pass Bveto       ", ET_eee, ET_lll, j, n_pass_BJet);
  print_line("pass mT          ", ET_eee, ET_lll, j, n_pass_mt3L);
  

}

/*--------------------------------------------------------------------------------*/
void Susy3LepAna::print_VRZZ()
{
  int j= ML_VRZZ;
  cout << "---------------------------------"    << endl;
  cout << ">>> SR " << ML_SRNAME[j] <<endl;

  print_line("pass Trig        ", ET_eeee, ET_llll, j, n_pass_3Ltrig);
  print_line("pass selZ        ", ET_eeee, ET_llll, j, n_pass_Z);
  print_line("pass MET         ", ET_eeee, ET_llll, j, n_pass_met);
  print_line("pass Bveto       ", ET_eeee, ET_llll, j, n_pass_BJet);
  print_line("pass ZZ->llll    ", ET_eeee, ET_llll, j, n_pass_ZllZll);

}

/*--------------------------------------------------------------------------------*/
void Susy3LepAna::fillHistograms(uint iSR, uint iSYS,
				 const LeptonVector* leptons, 
				 const JetVector* jets,
				 const Susy::Met* met,
				 float _ww)
{

 if(dbg()>5) cout << "Fill histo " << ML_FLAV[m_ET] << " " << ML_SRNAME[iSR] << endl;

  _hh->H1FILL(_hh->ML_pred[iSR][m_ET][iSYS],0,_ww); 

  _hh->H1FILL(_hh->ML_nLep[iSR][m_ET][iSYS], leptons->size(), _ww); 
  _hh->H1FILL(_hh->ML_evtCatgUnOrdered[iSR][m_ET][iSYS], evtCatgUnOrd(leptons), _ww);
  _hh->H1FILL(_hh->ML_evtCatgOSpair[iSR][m_ET][iSYS], evtCatgOrd(leptons,true), _ww);
  _hh->H1FILL(_hh->ML_evtCatgSSpair[iSR][m_ET][iSYS], evtCatgOrd(leptons,false), _ww);
  
  bool sfos = hasSFOS(*leptons);
  TLorentzVector _2l;
  TLorentzVector _3l;
  TLorentzVector _4l;
  TLorentzVector _ZZ4l;
  uint idx_l[4]={-999};
  uint idx=0;
  float dRll=999;

  //cout << "PASS " << ML_SRNAME[iSR] << endl;
  for(uint ilep=0; ilep<leptons->size(); ilep++){
    const Susy::Lepton* _l = leptons->at(ilep);
    if(ilep>=4) continue;
    if(iSR==ML_VRemulWW){
      if(_l->Pt()<=0.) continue;
      idx_l[idx++]=ilep;
    }
    //_l->print();

    if(ilep<2){
      //cout <<" 2l ilep " << ilep << "\t"; _l->print();
      _2l += (*_l);
      if(ilep==1) dRll  = leptons->at(0)->DeltaR(*leptons->at(1));
    }
    if(ilep<3) _3l += (*_l);
    if(ilep<4) _4l += (*_l);
    bool isChargeFlip =  _l->isEle() ? ((Electron*) _l)->isChargeFlip : false; 
    LEP_TYPE lType = getType(_l->mcOrigin,
			     _l->mcType,
			     _hh->sampleName(),
			     nt->evt()->mcChannel,
			     _l->truthType,
			     _l->isEle(),
			     isChargeFlip);
    if(ilep==0){
      _hh->H1FILL(_hh->ML_ptl1[iSR][m_ET][iSYS],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etal1[iSR][m_ET][iSYS],_l->Eta(),_ww); 
      _hh->H1FILL(_hh->ML_d0Sl1[iSR][m_ET][iSYS],_l->d0/_l->errD0,_ww); 
      _hh->H1FILL(_hh->ML_z0sinthetal1[iSR][m_ET][iSYS],_l->z0SinTheta(),_ww); 
      _hh->H1FILL(_hh->ML_orgl1[iSR][m_ET][iSYS],lType,_ww); 
    }
    else if(ilep==1){
      _hh->H1FILL(_hh->ML_ptl2[iSR][m_ET][iSYS],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etal2[iSR][m_ET][iSYS],_l->Eta(),_ww); 
      _hh->H1FILL(_hh->ML_d0Sl2[iSR][m_ET][iSYS],_l->d0/_l->errD0,_ww); 
      _hh->H1FILL(_hh->ML_z0sinthetal2[iSR][m_ET][iSYS],_l->z0SinTheta(),_ww); 
      _hh->H1FILL(_hh->ML_orgl2[iSR][m_ET][iSYS],lType,_ww); 
    }
    else if(ilep==2){
      _hh->H1FILL(_hh->ML_ptl3[iSR][m_ET][iSYS],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etal3[iSR][m_ET][iSYS],_l->Eta(),_ww); 
      _hh->H1FILL(_hh->ML_d0Sl3[iSR][m_ET][iSYS],_l->d0/_l->errD0,_ww); 
      _hh->H1FILL(_hh->ML_z0sinthetal3[iSR][m_ET][iSYS],_l->z0SinTheta(),_ww); 
      _hh->H1FILL(_hh->ML_orgl3[iSR][m_ET][iSYS],lType,_ww); 
    }
    else if(ilep==3){
      _hh->H1FILL(_hh->ML_ptl4[iSR][m_ET][iSYS],_l->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etal4[iSR][m_ET][iSYS],_l->Eta(),_ww); 
      _hh->H1FILL(_hh->ML_d0Sl4[iSR][m_ET][iSYS],_l->d0/_l->errD0,_ww); 
      _hh->H1FILL(_hh->ML_z0sinthetal4[iSR][m_ET][iSYS],_l->z0SinTheta(),_ww); 
      _hh->H1FILL(_hh->ML_orgl4[iSR][m_ET][iSYS],lType,_ww); 
    }
  }

  _hh->H1FILL(_hh->ML_dRll[iSR][m_ET][iSYS],dRll,_ww); 
  _hh->H1FILL(_hh->ML_pTll[iSR][m_ET][iSYS],_2l.Pt(),_ww); 
  

  _hh->H1FILL(_hh->ML_AllMlll[iSR][m_ET][iSYS],_3l.M(),_ww); 
  if(sfos){
    _hh->H1FILL(_hh->ML_SFOSMlll[iSR][m_ET][iSYS],_3l.M(),_ww); 
    
    //Compute MT
    //Find the 2 SFOS leptons - closest to Z peak
    //Use the 3rd one to reco MT
    uint zl1, zl2;
    float mT=0;
    findBestZ(zl1, zl2, *leptons);
    for(uint iL=0; iL<leptons->size(); iL++)	{
      if(iL!=zl1 && iL!=zl2)  mT= Mt(leptons->at(iL),met);
    }
    _hh->H1FILL(_hh->ML_SFOSMT[iSR][m_ET][iSYS],mT,_ww); 
  }
  
  uint idx_Z1l1, idx_Z1l2, idx_Z2l1, idx_Z2l2;
  if(leptons->size()>3 && 
     hasZllZll(*leptons, &idx_Z1l1, &idx_Z1l2, &idx_Z2l1, &idx_Z2l2)){
    //cout << "hasZZ " << idx_Z1l1 << " " << idx_Z1l2 
    //	 << " " << idx_Z2l1 << " " << idx_Z2l2 << endl;
    
    TLorentzVector Z1 = *(leptons->at(idx_Z1l1)) + *(leptons->at(idx_Z1l2));
    TLorentzVector Z2 = *(leptons->at(idx_Z2l1)) + *(leptons->at(idx_Z2l2));
    TLorentzVector ZZ = Z1+Z2;
    _hh->H1FILL(_hh->ML_SFOSMZZ[iSR][m_ET][iSYS],ZZ.M(),_ww);
   
  }
  
  //mCT for the emulate WW 
  if(iSR==ML_VRemulWW){
    float mWmZ    = MW/MZ;
    float mct     = mCT(*leptons->at(idx_l[0]),*leptons->at(idx_l[1])) * mWmZ;
    TLorentzVector recoil = -met->lv() - (*leptons->at(idx_l[0])) - (*leptons->at(idx_l[1]));
    float mctPerp = mCTperp(*leptons->at(idx_l[0]),*leptons->at(idx_l[1]),recoil) * mWmZ;
    LeptonVector _myLeptons;
    _myLeptons.push_back(leptons->at(idx_l[0]));
    _myLeptons.push_back(leptons->at(idx_l[1]));
    float mT2 = getMT2(_myLeptons, met);
    
    _hh->H1FILL(_hh->ML_mct[iSR][m_ET][iSYS],mct,_ww); 
    _hh->H1FILL(_hh->ML_mctPerp[iSR][m_ET][iSYS],mctPerp,_ww); 
    _hh->H1FILL(_hh->ML_mt2[iSR][m_ET][iSYS],mT2,_ww); 
    _hh->H1FILL(_hh->ML_mt2b[iSR][m_ET][iSYS],mT2,_ww); 

    if(jets->size()==0){
      _hh->H1FILL(_hh->ML_mct_0J[iSR][m_ET][iSYS],mct,_ww); 
      _hh->H1FILL(_hh->ML_mctPerp_0J[iSR][m_ET][iSYS],mctPerp,_ww); 
      _hh->H1FILL(_hh->ML_mt2_0J[iSR][m_ET][iSYS],mT2,_ww); 
      _hh->H1FILL(_hh->ML_mt2b_0J[iSR][m_ET][iSYS],mT2,_ww); 
    }
  }

  //Etmiss
  _hh->H1FILL(_hh->ML_etmiss[iSR][m_ET][iSYS],met->lv().Pt(),_ww); 
  _hh->H1FILL(_hh->ML_metRefEle[iSR][m_ET][iSYS],met->refEle,_ww); 
  _hh->H1FILL(_hh->ML_metRefGam[iSR][m_ET][iSYS],met->refGamma,_ww); 
  _hh->H1FILL(_hh->ML_metRefMuo[iSR][m_ET][iSYS],met->refMuo,_ww); 
  _hh->H1FILL(_hh->ML_metRefJet[iSR][m_ET][iSYS],met->refJet,_ww); 
  _hh->H1FILL(_hh->ML_metRefSJet[iSR][m_ET][iSYS],met->softJet,_ww); 
  _hh->H1FILL(_hh->ML_metCellout[iSR][m_ET][iSYS],met->refCell,_ww); 

  float metRel = getMetRel(met,*leptons,*jets);
  float mEff   = Meff(*leptons,*jets,met,JET_PT_CUT);
  float metSig = mEff/met->lv().Pt();

  _hh->H1FILL(_hh->ML_metrel[iSR][m_ET][iSYS],metRel,_ww); 
  _hh->H1FILL(_hh->ML_meff[iSR][m_ET][iSYS],mEff,_ww); 
  _hh->H1FILL(_hh->ML_metSig[iSR][m_ET][iSYS],metSig,_ww); 

  //Dilepton mass
  for(uint iL1=0; iL1<leptons->size(); iL1++){
    if(leptons->at(iL1)->Pt()<=0.) continue;
    for(uint iL2=iL1+1; iL2<leptons->size(); iL2++){
      if(leptons->at(iL2)->Pt()<=0.) continue;
      float mll = Mll(leptons->at(iL1),leptons->at(iL2));
      _hh->H1FILL(_hh->ML_AllMll[iSR][m_ET][iSYS],mll,_ww); 
      if(isSFOS(leptons->at(iL1),leptons->at(iL2)))
	_hh->H1FILL(_hh->ML_SFOSMll[iSR][m_ET][iSYS],mll,_ww); 
    }
  }

  int nBJets=0;
  _hh->H1FILL(_hh->ML_nJets[iSR][m_ET][iSYS],jets->size(),_ww); 
  for(uint ijet=0; ijet<jets->size(); ijet++){
    const Susy::Jet* _j = jets->at(ijet);
    
    if(isCentralBJet(_j)){
      nBJets++;
      _hh->H1FILL(_hh->ML_ptbj[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etabj[iSR][m_ET][iSYS],_j->Eta(),_ww); 
    }
    if(ijet==0){
      _hh->H1FILL(_hh->ML_ptj1[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etaj1[iSR][m_ET][iSYS],_j->Eta(),_ww); 
    }
    if(ijet==1){
      _hh->H1FILL(_hh->ML_ptj2[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etaj2[iSR][m_ET][iSYS],_j->Eta(),_ww); 
    }
    if(ijet==2){
      _hh->H1FILL(_hh->ML_ptj3[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etaj3[iSR][m_ET][iSYS],_j->Eta(),_ww); 
    }
    if(ijet==3){
      _hh->H1FILL(_hh->ML_ptj4[iSR][m_ET][iSYS],_j->Pt(),_ww); 
      _hh->H1FILL(_hh->ML_etaj4[iSR][m_ET][iSYS],_j->Eta(),_ww); 
    }
  }
  _hh->H1FILL(_hh->ML_nBJets[iSR][m_ET][iSYS],nBJets,_ww); 

  if(jets->size()>1){
    _hh->H1FILL(_hh->ML_predGe2J[iSR][m_ET][iSYS],0,_ww); 
    TLorentzVector _jj = (*jets->at(0)) + (*jets->at(1));
    _hh->H1FILL(_hh->ML_mjj[iSR][m_ET][iSYS],_jj.M(),_ww); 
  }
  
}
/*--------------------------------------------------------------------------------*/
int Susy3LepAna::evtCatgUnOrd(const LeptonVector* leptons){
  int evtCatgUnOrd=99;
  int nE=0;
  int nM=0;
  if(leptons->size()<3) return evtCatgUnOrd;
  for(uint iL=0; iL<leptons->size(); iL++){
    if( leptons->at(iL)->isEle() ) nE++;
    if( leptons->at(iL)->isMu()  ) nM++;
  }
  
  if(nE==3)      evtCatgUnOrd=0;
  else if(nE==2) evtCatgUnOrd=1;
  else if(nM==3) evtCatgUnOrd=2;
  else if(nM==2) evtCatgUnOrd=3;
  
  return evtCatgUnOrd;
}

/*--------------------------------------------------------------------------------*/
int Susy3LepAna::evtCatgOrd(const LeptonVector* leptons, bool useOS){
  int evtCatgOrd=99;
  if(leptons->size()<3) return evtCatgOrd;
  if(leptons->size()>3) return evtCatgOrd;
  
  int lType[3];
  int qProdLeadPair=1;
  for(uint iL=0; iL<leptons->size(); iL++){
    lType[iL] = (leptons->at(iL)->isEle()) ? 0 : 1;
    if(iL<2) qProdLeadPair *= leptons->at(iL)->q;
  }

  if(useOS && qProdLeadPair>0) return evtCatgOrd=99; //SS
  else if(!useOS && qProdLeadPair<0) return evtCatgOrd=99; //OS

  if(lType[0]==0 && lType[1]==0){ //eeX
    if(lType[2]==0) evtCatgOrd=0;
    if(lType[2]==1) evtCatgOrd=1;
  }
  else if(lType[0]==1 && lType[1]==1){//mmX
    if(lType[2]==0) evtCatgOrd=2;
    if(lType[2]==1) evtCatgOrd=3;
  }
  else if((lType[0]==0 && lType[1]==1) || 
	  (lType[0]==1 && lType[1]==0)){
    if(lType[2]==0) evtCatgOrd=4;
    if(lType[2]==1) evtCatgOrd=5;
  }
  return evtCatgOrd=99;
}
