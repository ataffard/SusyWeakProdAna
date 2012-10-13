#include "SusyWeakProdAna/Histos_2L.h"

/*--------------------------------------------------------------------------------*/
// SusyHistos Book2LHistograms
/*--------------------------------------------------------------------------------*/
void Histos_2L::Book2LHistograms(TDirectory* _hDir)
{
  std::cout << "Booking 2L histos " << std::endl;

  _hDir->cd();

  string    book_s1, book_s2,sx, sy, sz;
  char      syaxis[200];
  sprintf(syaxis,"Entries");


  //
  //SR histogram
  //

#define BOOK_SRDG2L(hN,xT,u,yT, ...)					\
  for(int i=0; i<3; i++){						\
    string sFlav;							\
    if(i==0) sFlav="EE_";						\
    if(i==1) sFlav="MM_";						\
    if(i==2) sFlav="EM_";						\
    for(int j=0; j<nHSR_DG2L; j++){					\
      if(string(u).length()>0) sx = string(xT+string(" [")+u+string("]")); \
      else sx = 	string(xT);					\
      string stype;							\
      if(j==0) stype="DG2L_SRjveto_";					\
      if(j==1) stype="DG2L_SRSSjveto_";					\
      if(j==2) stype="DG2L_SR2jets_";					\
      if(j==3) stype="DG2L_SRmT2_";					\
      if(j==4) stype="DG2L_SRmT2b_";					\
      if(j==5) stype="DG2L_CRZ_";					\
      if(j==6) stype="DG2L_NTOP_";					\
      if(j==7) stype="DG2L_NWW1_";					\
      if(j==8) stype="DG2L_NWW2_";					\
      if(j==9) stype="DG2L_NWW3_";					\
      if(j==10) stype="DG2L_ZXCR1_";					\
      if(j==11) stype="DG2L_ZXCR3_";					\
      if(j==12) stype="DG2L_ZXCR4_";					\
      if(j==13) stype="DG2L_CR2LepOS_";					\
      if(j==14) stype="DG2L_CR2LepSS_";					\
      hN[j][i] = (TH1F*) _utils->myTH1F((book_s1=stype + sFlav + #hN).c_str(), \
				(book_s2=stype + sFlav + #hN).c_str(),	\
				__VA_ARGS__,sx.c_str() ,yT);		\
      _utils->yAxis(hN[j][i],u);					\
    }									\
  }
  
  BOOK_SRDG2L(DG2L_pred,"","",syaxis,1,-0.5,0.5);
  BOOK_SRDG2L(DG2L_cutflow,"","",syaxis,20,-0.5,19.5);
  BOOK_SRDG2L(DG2L_nJets,"NJets","",syaxis,10,-0.5,9.5);
  BOOK_SRDG2L(DG2L_nCJets,"NJets - central","",syaxis,10,-0.5,9.5);
  BOOK_SRDG2L(DG2L_nFJets,"NJets - forward ","",syaxis,10,-0.5,9.5);
  BOOK_SRDG2L(DG2L_nBJets,"NBJets","",syaxis,5,-0.5,4.5);
  BOOK_SRDG2L(DG2L_qq,"","",syaxis,10,-0.5,1.5);
  BOOK_SRDG2L(DG2L_mll,"M_{ll}","GeV",syaxis,110,0,220); 
  BOOK_SRDG2L(DG2L_mjj,"M_{jj}","GeV",syaxis,50,0,500);
  BOOK_SRDG2L(DG2L_pTll,"p_{T}^{ll}","GeV",syaxis,105,10,220);
  BOOK_SRDG2L(DG2L_mWWT,"m_{T}^{WW}","GeV",syaxis,100,0,500);
  BOOK_SRDG2L(DG2L_dPhill,"dPhi(l,l)","degree",syaxis,9,0,180);
  BOOK_SRDG2L(DG2L_JZBJet,"JZB","GeV",syaxis,120,-400,800);
  BOOK_SRDG2L(DG2L_JZBEtmiss,"JZB","GeV",syaxis,120,-400,800);
  BOOK_SRDG2L(DG2L_etmiss,"#slash{E}_{T}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_metrel,"#slash{E}_{T}^{Rel}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_metRefEle,"#slash{E}_{T}^{RefEle}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_metRefGam,"#slash{E}_{T}^{RefGamma}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_metRefMuo,"#slash{E}_{T}^{RefMuo}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_metRefJet,"#slash{E}_{T}^{RefJet}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_metRefSJet,"#slash{E}_{T}^{RefSoftJet}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_metCellout,"#slash{E}_{T}^{Cellout}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_mt2,"M_{T2}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_npv,"N_{pv}","",syaxis,30,-0.5,29.5);
  BOOK_SRDG2L(DG2L_mu,"<#mu>","",syaxis,40,-0.5,39.5);
  BOOK_SRDG2L(DG2L_ptl1,"P_{T}^{l1}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_ptl2,"P_{T}^{l2}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_etal1,"#eta^{l1}","",syaxis,25,-2.5,2.5);
  BOOK_SRDG2L(DG2L_etal2,"#eta^{l2}","",syaxis,25,-2.5,2.5);
  BOOK_SRDG2L(DG2L_d0Sl1,"d0/#sigma_{d0}^{l1}","",syaxis,100,-10,10);
  BOOK_SRDG2L(DG2L_d0Sl2,"d0/#sigma_{d0}^{l2}","",syaxis,100,-10,10);
  BOOK_SRDG2L(DG2L_z0sinthetal1,"z0 sin(#theta)^{l1}","cm",syaxis,200,-1,1);
  BOOK_SRDG2L(DG2L_z0sinthetal2,"z0 sin(#theta)^{l2}","cm",syaxis,200,-1,1);
  BOOK_SRDG2L(DG2L_orgl1,"MC Origin","",syaxis,5,-0.5,4.5);
  BOOK_SRDG2L(DG2L_orgl2,"MC Origin","",syaxis,5,-0.5,4.5);
  BOOK_SRDG2L(DG2L_ptj1,"P_{T}^{j1}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_ptj2,"P_{T}^{j2}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_ptj3,"P_{T}^{j3}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_ptj4,"P_{T}^{j4}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_etaj1,"#eta^{j1}","",syaxis,25,-2.5,2.5);
  BOOK_SRDG2L(DG2L_etaj2,"#eta^{j2}","",syaxis,25,-2.5,2.5);
  BOOK_SRDG2L(DG2L_etaj3,"#eta^{j3}","",syaxis,25,-2.5,2.5);
  BOOK_SRDG2L(DG2L_etaj4,"#eta^{j4}","",syaxis,25,-2.5,2.5);
  BOOK_SRDG2L(DG2L_ptbj,"P_{T}^{bjet}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_etabj,"#eta^{bjet}","",syaxis,25,-2.5,2.5);

#undef BOOK_SRDG2L
  
  std::vector<TString> LepType;
  LepType.clear();
  LepType.push_back("PR");
  LepType.push_back("CONV");
  LepType.push_back("HF");
  LepType.push_back("LF");
  LepType.push_back("Unknown");
 
  for(uint i=0; i<nHSR_DG2L; i++){
    for(int ilep=0; ilep<3; ilep++){			
      for(uint j=0; j<LepType.size(); j++){
	DG2L_orgl1[i][ilep]->GetXaxis()->SetBinLabel(j+1,LepType.at(j).Data());
	DG2L_orgl2[i][ilep]->GetXaxis()->SetBinLabel(j+1,LepType.at(j).Data());
      }
    }
  }
  
}
