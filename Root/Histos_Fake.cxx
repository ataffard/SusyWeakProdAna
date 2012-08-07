#include "SusyWeakProdAna/Histos_Fake.h"

/*--------------------------------------------------------------------------------*/
// SusyHistos BookFakeHistograms
/*--------------------------------------------------------------------------------*/
void Histos_Fake::BookFakeHistograms(TDirectory* _hDir)
{
  std::cout << "Booking Fake histos " << std::endl;

  _hDir->cd();

  const int nMuPtBins=6;
  const double pt_m_bins[]   = {0,10,15,25,40,100,200};

  const int nElePtBins=6;
  const double pt_e_bins[]   = {0,10,15,25,40,100,200};

  string    book_s1, book_s2,sx, sy, sz;
  char      syaxis[200];
  sprintf(syaxis,"Entries");

  std::string MCTYPE[] = {"PR","HF","LF","CONV"};
 
#define BOOK1(hN,xT,u,yT, ...)						\
  for(int j=0; j<nHMC; j++){						\
    if(string(u).length()>0) sx = string(xT+string(" [")+u+string("]")); \
    else sx = 	string(xT);						\
    string stype;							\
    if(j==0) stype="pr_";						\
    if(j==1) stype="hf_";						\
    if(j==2) stype="lf_";						\
    if(j==3) stype="cv_";						\
    hN[0][j] = _utils->myTH1F((book_s1=string("sel_")+stype+ #hN).c_str(), \
			      (book_s2=string("sel_")+stype+ #hN).c_str(), \
			      __VA_ARGS__,sx.c_str() ,yT);		\
    _utils->yAxis(hN[0][j],u);						\
    									\
    hN[1][j] = _utils->myTH1F((book_s1=string("trm_")+stype+ #hN).c_str(), \
			      (book_s2=string("trm_")+stype+ #hN).c_str(), \
			      __VA_ARGS__,sx.c_str() ,yT);		\
    _utils->yAxis(hN[1][j],u);						\
  }

  //Muons  
  BOOK1(m_org,"","",syaxis,43,-0.5,42.5);
  BOOK1(m_type,"","",syaxis,4,-0.5,3.5);
  BOOK1(m_pt,"P_{T}","GeV",syaxis,50,0,100);
  BOOK1(m_eta,"#eta","",syaxis,10,-2.5,2.5);
  BOOK1(m_d0,"d0","mm",syaxis,100,-1,1);
  BOOK1(m_d0S,"d0 significance","",syaxis,200,-10,10);
  BOOK1(m_z0sintheta,"z0 sin(#theta)","mm",syaxis,100,-2,2);
  BOOK1(m_dR_jet,"dR","",syaxis,40,0,4);
  BOOK1(m_signd0,"signed d0","",syaxis,200,-10,10);
  BOOK1(m_ptrel,"ptRel","GeV",syaxis,100,0,20);
  BOOK1(m_cjet_pt,"P_{T}","GeV",syaxis,50,0,100);
  BOOK1(m_cjet_nTrk,"nTrk","",syaxis,20,-0.5,19.5);
  BOOK1(m_cjet_isBTag,"BTag","",syaxis,2,-0.5,1.5);
  BOOK1(m_cjet_idB,"true b","",syaxis,2,-0.5,1.5);
  BOOK1(m_cjet_bj_pt,"P_{T}","GeV",syaxis,40,0,200);
  BOOK1(m_cjet_b_pt,"P_{T}","GeV",syaxis,40,0,200);
  BOOK1(m_ptCone20,"PtCone20","GeV",syaxis,24,-1,5);
  BOOK1(m_ptCone30,"PtCone30","GeV",syaxis,24,-1,5);
  BOOK1(m_ptCone40,"PtCone40","GeV",syaxis,24,-1,5);
  BOOK1(m_etCone20,"EtCone20","GeV",syaxis,150,-5,25);
  BOOK1(m_etCone30,"EtCone30","GeV",syaxis,150,-5,25);
  BOOK1(m_etCone40,"EtCone40","GeV",syaxis,150,-5,25);
  BOOK1(m_ptCone20Rel,"PtCone20/Pt","",syaxis,150,-0.5,2.5);
  BOOK1(m_ptCone30Rel,"PtCone30/Pt","",syaxis,150,-0.5,2.5);
  BOOK1(m_ptCone40Rel,"PtCone40/Pt","",syaxis,150,-0.5,2.5);
  BOOK1(m_etCone20Rel,"EtCone20/Pt","",syaxis,150,-0.5,2.5);
  BOOK1(m_etCone30Rel,"EtCone30/Pt","",syaxis,150,-0.5,2.5);
  BOOK1(m_etCone40Rel,"EtCone40/Pt","",syaxis,150,-0.5,2.5);

  BOOK1(m_ptCone20_Eff,"PtCone20","GeV","Eff [%]",24,-1,5);
  BOOK1(m_ptCone30_Eff,"PtCone30","GeV","Eff [%]",24,-1,5);
  BOOK1(m_ptCone40_Eff,"PtCone40","GeV","Eff [%]",24,-1,5);
  BOOK1(m_etCone20_Eff,"EtCone20","GeV","Eff [%]",150,-5,25);
  BOOK1(m_etCone30_Eff,"EtCone30","GeV","Eff [%]",150,-5,25);
  BOOK1(m_etCone40_Eff,"EtCone40","GeV","Eff [%]",150,-5,25);
  BOOK1(m_ptCone20Rel_Eff,"PtCone20/Pt","","Eff [%]",150,-0.5,2.5);
  BOOK1(m_ptCone30Rel_Eff,"PtCone30/Pt","","Eff [%]",150,-0.5,2.5);
  BOOK1(m_ptCone40Rel_Eff,"PtCone40/Pt","","Eff [%]",150,-0.5,2.5);
  BOOK1(m_etCone20Rel_Eff,"EtCone20/Pt","","Eff [%]",150,-0.5,2.5);
  BOOK1(m_etCone30Rel_Eff,"EtCone30/Pt","","Eff [%]",150,-0.5,2.5);
  BOOK1(m_etCone40Rel_Eff,"EtCone40/Pt","","Eff [%]",150,-0.5,2.5);



  //Electrons
  BOOK1(e_org,"","",syaxis,43,-0.5,42.5);
  BOOK1(e_type,"","",syaxis,4,-0.5,3.5);
  BOOK1(e_pt,"P_{T}","GeV",syaxis,50,0,100);
  BOOK1(e_eta,"#eta","",syaxis,10,-2.5,2.5);
  BOOK1(e_d0,"d0","mm",syaxis,100,-1,1);
  BOOK1(e_d0S,"d0/#sigma_{d0}","",syaxis,200,-10,10);
  BOOK1(e_signd0,"signed d0","",syaxis,200,-10,10);
  BOOK1(e_z0sintheta,"z0 sin(#theta)","mm",syaxis,100,-2,2);
  BOOK1(e_ptrel,"ptRel","GeV",syaxis,100,0,20);
  BOOK1(e_dR_jet,"dR","",syaxis,40,0,4);
  BOOK1(e_cjet_pt,"P_{T}","GeV",syaxis,50,0,100);
  BOOK1(e_cjet_nTrk,"nTrk","",syaxis,20,-0.5,19.5);
  BOOK1(e_cjet_isBTag,"BTag","",syaxis,2,-0.5,1.5);
  BOOK1(e_cjet_idB,"true b","",syaxis,2,-0.5,1.5);
  BOOK1(e_cjet_bj_pt,"P_{T}","GeV",syaxis,40,0,200);
  BOOK1(e_cjet_b_pt,"P_{T}","GeV",syaxis,40,0,200);
  BOOK1(e_ptCone20,"PtCone20","GeV",syaxis,24,-1,5);
  BOOK1(e_ptCone30,"PtCone30","GeV",syaxis,24,-1,5);
  BOOK1(e_ptCone40,"PtCone40","GeV",syaxis,24,-1,5);
  BOOK1(e_etCone20,"EtCone20","GeV",syaxis,150,-5,25);
  BOOK1(e_etCone30,"EtCone30","GeV",syaxis,150,-5,25);
  BOOK1(e_etCone40,"EtCone40","GeV",syaxis,150,-5,25);
  BOOK1(e_etConeCorr20,"EtConeCorr20","GeV",syaxis,150,-5,25);
  BOOK1(e_etConeCorr30,"EtConeCorr30","GeV",syaxis,150,-5,25);
  BOOK1(e_etConeCorr40,"EtConeCorr40","GeV",syaxis,150,-5,25);
  BOOK1(e_etConeTopoCorr20,"EtConeTopoCorr20","GeV",syaxis,150,-5,25);
  BOOK1(e_etConeTopoCorr30,"EtConeTopoCorr30","GeV",syaxis,150,-5,25);
  BOOK1(e_etConeTopoCorr40,"EtConeTopoCorr40","GeV",syaxis,150,-5,25);
  BOOK1(e_ptCone20Rel,"PtCone20/Pt","",syaxis,150,-0.5,2.5);
  BOOK1(e_ptCone30Rel,"PtCone30/Pt","",syaxis,150,-0.5,2.5);
  BOOK1(e_ptCone40Rel,"PtCone40/Pt","",syaxis,150,-0.5,2.5);
  BOOK1(e_etCone20Rel,"EtCone20/Pt","",syaxis,150,-0.5,2.5);
  BOOK1(e_etCone30Rel,"EtCone30/Pt","",syaxis,150,-0.5,2.5);
  BOOK1(e_etCone40Rel,"EtCone40/Pt","",syaxis,150,-0.5,2.5);
  BOOK1(e_etConeCorr20Rel,"EtConeCorr20/Pt","",syaxis,150,-0.5,2.5);
  BOOK1(e_etConeCorr30Rel,"EtConeCorr30/Pt","",syaxis,150,-0.5,2.5);
  BOOK1(e_etConeCorr40Rel,"EtConeCorr40/Pt","",syaxis,150,-0.5,2.5);
  BOOK1(e_etConeTopoCorr20Rel,"EtConeTopoCorr20/Pt","",syaxis,150,-0.5,2.5);
  BOOK1(e_etConeTopoCorr30Rel,"EtConeTopoCorr30/Pt","",syaxis,150,-0.5,2.5);
  BOOK1(e_etConeTopoCorr40Rel,"EtConeTopoCorr40/Pt","",syaxis,150,-0.5,2.5);
  
  BOOK1(e_ptCone20_Eff,"PtCone20","GeV","Eff [%]",20,0,2);
  BOOK1(e_ptCone30_Eff,"PtCone30","GeV","Eff [%]",20,0,2);
  BOOK1(e_ptCone40_Eff,"PtCone40","GeV","Eff [%]",20,0,2);
  BOOK1(e_etCone20_Eff,"EtCone20","GeV","Eff [%]",150,-5,25);
  BOOK1(e_etCone30_Eff,"EtCone30","GeV","Eff [%]",150,-5,25);
  BOOK1(e_etCone40_Eff,"EtCone40","GeV","Eff [%]",150,-5,25);
  BOOK1(e_etConeCorr20_Eff,"EtConeCorr20","GeV","Eff [%]",150,-5,25);
  BOOK1(e_etConeCorr30_Eff,"EtConeCorr30","GeV","Eff [%]",150,-5,25);
  BOOK1(e_etConeCorr40_Eff,"EtConeCorr40","GeV","Eff [%]",150,-5,25);
  BOOK1(e_etConeTopoCorr20_Eff,"EtConeTopoCorr20","GeV","Eff [%]",150,-5,25);
  BOOK1(e_etConeTopoCorr30_Eff,"EtConeTopoCorr30","GeV","Eff [%]",150,-5,25);
  BOOK1(e_etConeTopoCorr40_Eff,"EtConeTopoCorr40","GeV","Eff [%]",150,-5,25);
  BOOK1(e_ptCone20Rel_Eff,"PtCone20/Pt","","Eff [%]",150,-0.5,2.5);
  BOOK1(e_ptCone30Rel_Eff,"PtCone30/Pt","","Eff [%]",150,-0.5,2.5);
  BOOK1(e_ptCone40Rel_Eff,"PtCone40/Pt","","Eff [%]",150,-0.5,2.5);
  BOOK1(e_etCone20Rel_Eff,"EtCone20/Pt","","Eff [%]",150,-0.5,2.5);
  BOOK1(e_etCone30Rel_Eff,"EtCone30/Pt","","Eff [%]",150,-0.5,2.5);
  BOOK1(e_etCone40Rel_Eff,"EtCone40/Pt","","Eff [%]",150,-0.5,2.5);
  BOOK1(e_etConeCorr20Rel_Eff,"EtConeCorr20/Pt","","Eff [%]",150,-0.5,2.5);
  BOOK1(e_etConeCorr30Rel_Eff,"EtConeCorr30/Pt","","Eff [%]",150,-0.5,2.5);
  BOOK1(e_etConeCorr40Rel_Eff,"EtConeCorr40/Pt","","Eff [%]",150,-0.5,2.5);
  BOOK1(e_etConeTopoCorr20Rel_Eff,"EtConeTopoCorr20/Pt","","Eff [%]",150,-0.5,2.5);
  BOOK1(e_etConeTopoCorr30Rel_Eff,"EtConeTopoCorr30/Pt","","Eff [%]",150,-0.5,2.5);
  BOOK1(e_etConeTopoCorr40Rel_Eff,"EtConeTopoCorr40/Pt","","Eff [%]",150,-0.5,2.5);


#undef BOOK1

  MCTruthPartClassifier::ParticleDef partDef;
  for(uint i=0; i<nHM; i++){
    for(uint j=0; j<nHMC; j++){
      for(uint p=0; p< partDef.sParticleOrigin.size(); p++){
	m_org[i][j]->GetXaxis()->SetBinLabel(p+1,partDef.sParticleOrigin.at(p).c_str());
	e_org[i][j]->GetXaxis()->SetBinLabel(p+1,partDef.sParticleOrigin.at(p).c_str());
      }
      m_org[i][j]->GetXaxis()->LabelsOption("v");
      e_org[i][j]->GetXaxis()->LabelsOption("v");

      for(uint p=0; p<4; p++){
	m_type[i][j]->GetXaxis()->SetBinLabel(p+1,MCTYPE[p].c_str());
	e_type[i][j]->GetXaxis()->SetBinLabel(p+1,MCTYPE[p].c_str());
      }
      m_type[i][j]->GetXaxis()->LabelsOption("v");
      e_type[i][j]->GetXaxis()->LabelsOption("v");

    }
  }


#define BOOK2(hN,xT,xU,yT,yU, ...)					\
  for(int j=0; j<nHMC; j++){						\
    if(string(xU).length()>0) sx = string(xT+string(" [")+xU+string("]")); \
    else sx = 	string(xT);						\
    if(string(yU).length()>0) sx = string(xT+string(" [")+yU+string("]")); \
    else sy = 	string(yT);						\
    string stype;							\
    if(j==0) stype="pr_";						\
    if(j==1) stype="hf_";						\
    if(j==2) stype="lf_";						\
    if(j==3) stype="cv_";						\
    hN[0][j] = _utils->myTH2F((book_s1=string("sel_")+stype+ #hN).c_str(), \
			      (book_s2=string("sel_")+stype+ #hN).c_str(), \
			      __VA_ARGS__,sx.c_str() ,sy.c_str());	\
    hN[0][j]->SetMarkerSize(0.01);					\
									\
    hN[1][j] = _utils->myTH2F((book_s1=string("trm_")+stype+ #hN).c_str(), \
			      (book_s2=string("trm_")+stype+ #hN).c_str(), \
			      __VA_ARGS__,sx.c_str() ,sy.c_str());	\
    hN[1][j]->SetMarkerSize(0.01);					\
  }


  BOOK2(e_ptCone30Pt,"p_{T}","GeV","PtCone30","GeV",25,10,110,50,-1,49);
  BOOK2(e_ptCone30RelPt,"p_{T}","GeV","PtCone30Rel","GeV",25,10,110,60,-0.5,5.5);
  BOOK2(e_etCone30Pt,"p_{T}","GeV","EtCone30","GeV",25,10,60,250,-1,24);
  BOOK2(e_etCone30RelPt,"p_{T}","GeV","EtCone30Rel","GeV",25,10,110,60,-0.5,5.5);

  BOOK2(m_ptCone30Pt,"p_{T}","GeV","PtCone30","GeV",25,10,110,50,-1,49);
  BOOK2(m_ptCone30RelPt,"GeV","p_{T}","PtCone30Rel","GeV",25,10,110,60,-0.5,5.5);
  BOOK2(m_etCone30Pt,"p_{T}","GeV","EtCone30","GeV",25,10,110,250,-1,24);
  BOOK2(m_etCone30RelPt,"p_{T}","GeV","EtCone30Rel","GeV",25,10,110,60,-0.5,5.5);


#undef BOOK2


#define BOOK3(hN,xT,xU,yT,yU,zT,zU, ...)				\
  for(int j=0; j<nHMC; j++){						\
    if(string(xU).length()>0) sx = string(xT+string(" [")+xU+string("]")); \
    else sx = 	string(xT);						\
    if(string(yU).length()>0) sy = string(yT+string(" [")+yU+string("]")); \
    else sy = 	string(yT);						\
    if(string(zU).length()>0) sz = string(zT+string(" [")+zU+string("]")); \
    else sz = 	string(zT);						\
    string stype;							\
    if(j==0) stype="pr_";						\
    if(j==1) stype="hf_";						\
    if(j==2) stype="lf_";						\
    if(j==3) stype="cv_";						\
    hN[0][j] = _utils->myTH3F((book_s1=string("sel_")+stype+ #hN).c_str(), \
			      (book_s2=string("sel_")+stype+ #hN).c_str(), \
			      __VA_ARGS__,				\
			      sx.c_str(),sy.c_str(),sz.c_str());	\
    									\
    hN[1][j] = _utils->myTH3F((book_s1=string("trm_")+stype+ #hN).c_str(), \
			      (book_s2=string("trm_")+stype+ #hN).c_str(), \
			      __VA_ARGS__,				\
			      sx.c_str(),sy.c_str(),sz.c_str());	\
  }
  
  //Muons  
  BOOK3(m_ptCone20_npv,"PtCone20","GeV","p_{T}","GeV","N_{pv}","",24,-1,5,6,10,80,30,-0.5,29.5);
  BOOK3(m_ptCone30_npv,"PtCone30","GeV","p_{T}","GeV","N_{pv}","",24,-1,5,6,10,80,30,-0.5,29.5);
  BOOK3(m_ptCone40_npv,"PtCone40","GeV","p_{T}","GeV","N_{pv}","",24,-1,5,6,10,80,30,-0.5,29.5);
  BOOK3(m_etCone20_npv,"EtCone20","GeV","p_{T}","GeV","N_{pv}","",150,-5,25,6,10,80,30,-0.5,29.5);
  BOOK3(m_etCone30_npv,"EtCone30","GeV","p_{T}","GeV","N_{pv}","",150,-5,25,6,10,80,30,-0.5,29.5);
  BOOK3(m_etCone40_npv,"EtCone40","GeV","p_{T}","GeV","N_{pv}","",150,-5,25,6,10,80,30,-0.5,29.5);
  BOOK3(m_ptCone20Rel_npv,"PtCone20Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);
  BOOK3(m_ptCone30Rel_npv,"PtCone30Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);
  BOOK3(m_ptCone40Rel_npv,"PtCone40Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);
  BOOK3(m_etCone20Rel_npv,"EtCone20Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);
  BOOK3(m_etCone30Rel_npv,"EtCone30Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);
  BOOK3(m_etCone40Rel_npv,"EtCone40Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);


  //Electrons
  BOOK3(e_ptCone20_npv,"PtCone20","GeV","p_{T}","GeV","N_{pv}","",24,-1,5,6,10,80,30,-0.5,29.5);
  BOOK3(e_ptCone30_npv,"PtCone30","GeV","p_{T}","GeV","N_{pv}","",24,-1,5,6,10,80,30,-0.5,29.5);
  BOOK3(e_ptCone40_npv,"PtCone40","GeV","p_{T}","GeV","N_{pv}","",24,-1,5,6,10,80,30,-0.5,29.5);
  BOOK3(e_etCone20_npv,"EtCone20","GeV","p_{T}","GeV","N_{pv}","",150,-5,25,6,10,80,30,-0.5,29.5);
  BOOK3(e_etCone30_npv,"EtCone30","GeV","p_{T}","GeV","N_{pv}","",150,-5,25,6,10,80,30,-0.5,29.5);
  BOOK3(e_etCone40_npv,"EtCone40","GeV","p_{T}","GeV","N_{pv}","",150,-5,25,6,10,80,30,-0.5,29.5);
  BOOK3(e_etConeCorr20_npv,"EtConeCorr20","GeV","p_{T}","GeV","N_{pv}","",150,-5,25,6,10,80,30,-0.5,29.5);
  BOOK3(e_etConeCorr30_npv,"EtConeCorr30","GeV","p_{T}","GeV","N_{pv}","",150,-5,25,6,10,80,30,-0.5,29.5);
  BOOK3(e_etConeCorr40_npv,"EtConeCorr40","GeV","p_{T}","GeV","N_{pv}","",150,-5,25,6,10,80,30,-0.5,29.5);
  BOOK3(e_etConeTopoCorr20_npv,"EtConeTopoCorr20","GeV","p_{T}","GeV","N_{pv}","",150,-5,25,6,10,80,30,-0.5,29.5);
  BOOK3(e_etConeTopoCorr30_npv,"EtConeTopoCorr30","GeV","p_{T}","GeV","N_{pv}","",150,-5,25,6,10,80,30,-0.5,29.5);
  BOOK3(e_etConeTopoCorr40_npv,"EtConeTopoCorr40","GeV","p_{T}","GeV","N_{pv}","",150,-5,25,6,10,80,30,-0.5,29.5);
  BOOK3(e_ptCone20Rel_npv,"PtCone20Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);
  BOOK3(e_ptCone30Rel_npv,"PtCone30Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);
  BOOK3(e_ptCone40Rel_npv,"PtCone40Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);
  BOOK3(e_etCone20Rel_npv,"EtCone20Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);
  BOOK3(e_etCone30Rel_npv,"EtCone30Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);
  BOOK3(e_etCone40Rel_npv,"EtCone40Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);
  BOOK3(e_etConeCorr20Rel_npv,"EtConeCorr20Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);
  BOOK3(e_etConeCorr30Rel_npv,"EtConeCorr30Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);
  BOOK3(e_etConeCorr40Rel_npv,"EtConeCorr40Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);
  BOOK3(e_etConeTopoCorr20Rel_npv,"EtConeTopoCorr20Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);
  BOOK3(e_etConeTopoCorr30Rel_npv,"EtConeTopoCorr30Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);
  BOOK3(e_etConeTopoCorr40Rel_npv,"EtConeTopoCorr40Rel","","p_{T}","GeV","N_{pv}","",150,-0.5,2.5,6,10,80,30,-0.5,29.5);

  //Muons  
  BOOK3(m_ptCone20_mu,"PtCone20","GeV","p_{T}","GeV","<#mu>","",24,-1,5,6,10,80,40,-0.5,39.5);
  BOOK3(m_ptCone30_mu,"PtCone30","GeV","p_{T}","GeV","<#mu>","",24,-1,5,6,10,80,40,-0.5,39.5);
  BOOK3(m_ptCone40_mu,"PtCone40","GeV","p_{T}","GeV","<#mu>","",24,-1,5,6,10,80,40,-0.5,39.5);
  BOOK3(m_etCone20_mu,"EtCone20","GeV","p_{T}","GeV","<#mu>","",150,-5,25,6,10,80,40,-0.5,39.5);
  BOOK3(m_etCone30_mu,"EtCone30","GeV","p_{T}","GeV","<#mu>","",150,-5,25,6,10,80,40,-0.5,39.5);
  BOOK3(m_etCone40_mu,"EtCone40","GeV","p_{T}","GeV","<#mu>","",150,-5,25,6,10,80,40,-0.5,39.5);
  BOOK3(m_ptCone20Rel_mu,"PtCone20Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);
  BOOK3(m_ptCone30Rel_mu,"PtCone30Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);
  BOOK3(m_ptCone40Rel_mu,"PtCone40Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);
  BOOK3(m_etCone20Rel_mu,"EtCone20Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);
  BOOK3(m_etCone30Rel_mu,"EtCone30Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);
  BOOK3(m_etCone40Rel_mu,"EtCone40Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);


  //Electrons
  BOOK3(e_ptCone20_mu,"PtCone20","GeV","p_{T}","GeV","<#mu>","",24,-1,5,6,10,80,40,-0.5,39.5);
  BOOK3(e_ptCone30_mu,"PtCone30","GeV","p_{T}","GeV","<#mu>","",24,-1,5,6,10,80,40,-0.5,39.5);
  BOOK3(e_ptCone40_mu,"PtCone40","GeV","p_{T}","GeV","<#mu>","",24,-1,5,6,10,80,40,-0.5,39.5);
  BOOK3(e_etCone20_mu,"EtCone20","GeV","p_{T}","GeV","<#mu>","",150,-5,25,6,10,80,40,-0.5,39.5);
  BOOK3(e_etCone30_mu,"EtCone30","GeV","p_{T}","GeV","<#mu>","",150,-5,25,6,10,80,40,-0.5,39.5);
  BOOK3(e_etCone40_mu,"EtCone40","GeV","p_{T}","GeV","<#mu>","",150,-5,25,6,10,80,40,-0.5,39.5);
  BOOK3(e_etConeCorr20_mu,"EtConeCorr20","GeV","p_{T}","GeV","<#mu>","",150,-5,25,6,10,80,40,-0.5,39.5);
  BOOK3(e_etConeCorr30_mu,"EtConeCorr30","GeV","p_{T}","GeV","<#mu>","",150,-5,25,6,10,80,40,-0.5,39.5);
  BOOK3(e_etConeCorr40_mu,"EtConeCorr40","GeV","p_{T}","GeV","<#mu>","",150,-5,25,6,10,80,40,-0.5,39.5);
  BOOK3(e_etConeTopoCorr20_mu,"EtConeTopoCorr20","GeV","p_{T}","GeV","<#mu>","",150,-5,25,6,10,80,40,-0.5,39.5);
  BOOK3(e_etConeTopoCorr30_mu,"EtConeTopoCorr30","GeV","p_{T}","GeV","<#mu>","",150,-5,25,6,10,80,40,-0.5,39.5);
  BOOK3(e_etConeTopoCorr40_mu,"EtConeTopoCorr40","GeV","p_{T}","GeV","<#mu>","",150,-5,25,6,10,80,40,-0.5,39.5);
  BOOK3(e_ptCone20Rel_mu,"PtCone20Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);
  BOOK3(e_ptCone30Rel_mu,"PtCone30Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);
  BOOK3(e_ptCone40Rel_mu,"PtCone40Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);
  BOOK3(e_etCone20Rel_mu,"EtCone20Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);
  BOOK3(e_etCone30Rel_mu,"EtCone30Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);
  BOOK3(e_etCone40Rel_mu,"EtCone40Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);
  BOOK3(e_etConeCorr20Rel_mu,"EtConeCorr20Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);
  BOOK3(e_etConeCorr30Rel_mu,"EtConeCorr30Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);
  BOOK3(e_etConeCorr40Rel_mu,"EtConeCorr40Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);
  BOOK3(e_etConeTopoCorr20Rel_mu,"EtConeTopoCorr20Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);
  BOOK3(e_etConeTopoCorr30Rel_mu,"EtConeTopoCorr30Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);
  BOOK3(e_etConeTopoCorr40Rel_mu,"EtConeTopoCorr40Rel","","p_{T}","GeV","<#mu>","",150,-0.5,2.5,6,10,80,40,-0.5,39.5);



#undef BOOK3

#define BOOK2TRIG(hN,xT,xU,yT,yU, ...)				\
  for(int k=0; k<5; k++){						\
    string sTrig;							\
    if(k==0) sTrig = "_TG1";						\
    if(k==1) sTrig = "_TG2";						\
    if(k==2) sTrig = "_TG3";						\
    if(k==3) sTrig = "_TG4";						\
    if(k==4) sTrig = "_TG5";						\
    for(int j=0; j<nHMC; j++){						\
      if(string(xU).length()>0) sx = string(xT+string(" [")+xU+string("]")); \
      else sx = 	string(xT);					\
      if(string(yU).length()>0) sy = string(yT+string(" [")+yU+string("]")); \
      else sy = 	string(yT);					\
      string stype;							\
      if(j==0) stype="pr_";						\
      if(j==1) stype="hf_";						\
      if(j==2) stype="lf_";						\
      if(j==3) stype="cv_";						\
      hN[0][j][k] = _utils->myTH2F((book_s1=string("sel_")+stype+ #hN + sTrig).c_str(), \
				(book_s2=string("sel_")+stype+ #hN + sTrig).c_str(), \
				__VA_ARGS__,				\
				sx.c_str(),sy.c_str());			\
      									\
      hN[1][j][k] = _utils->myTH2F((book_s1=string("trm_")+stype+ #hN + sTrig).c_str(), \
				(book_s2=string("trm_")+stype+ #hN + sTrig).c_str(), \
				__VA_ARGS__,				\
				sx.c_str(),sy.c_str());			\
    }									\
  }
  
  //Muons  
  BOOK2TRIG(m_pt_Npv,"p_{T}","GeV","N_{pv}","",45,10,100,30,-0.5,29.5);
  BOOK2TRIG(m_pt_Npv_pTrig,"p_{T}","GeV","N_{pv}","",45,10,100,30,-0.5,29.5);
  BOOK2TRIG(m_ptCone20Rel_Npv,"PtCone20Rel","","N_{pv}","",100,0,2,30,-0.5,29.5);
  BOOK2TRIG(m_ptCone20Rel_Npv_pTrig,"PtCone20Rel","","N_{pv}","",100,0,2,30,-0.5,29.5);
  BOOK2TRIG(m_pt_wiso_Npv,"p_{T}","GeV","N_{pv}","",45,10,100,30,-0.5,29.5);
  BOOK2TRIG(m_pt_wiso_Npv_pTrig,"p_{T}","GeV","N_{pv}","",45,10,100,30,-0.5,29.5);

  //Muons  
  BOOK2TRIG(e_pt_Npv,"p_{T}","GeV","N_{pv}","",45,10,100,30,-0.5,29.5);
  BOOK2TRIG(e_pt_Npv_pTrig,"p_{T}","GeV","N_{pv}","",45,10,100,30,-0.5,29.5);
  BOOK2TRIG(e_ptCone20Rel_Npv,"PtCone20Rel","","N_{pv}","",100,0,2,30,-0.5,29.5);
  BOOK2TRIG(e_ptCone20Rel_Npv_pTrig,"PtCone20Rel","","N_{pv}","",100,0,2,30,-0.5,29.5);
  BOOK2TRIG(e_pt_wiso_Npv,"p_{T}","GeV","N_{pv}","",45,10,100,30,-0.5,29.5);
  BOOK2TRIG(e_pt_wiso_Npv_pTrig,"p_{T}","GeV","N_{pv}","",45,10,100,30,-0.5,29.5);

  //Muons  
  BOOK2TRIG(m_pt_Mu,"p_{T}","GeV","<#mu>","",45,10,100,40,-0.5,39.5);
  BOOK2TRIG(m_pt_Mu_pTrig,"p_{T}","GeV","<#mu>","",45,10,100,40,-0.5,39.5);
  BOOK2TRIG(m_ptCone20Rel_Mu,"PtCone20Rel","","<#mu>","",100,0,2,40,-0.5,39.5);
  BOOK2TRIG(m_ptCone20Rel_Mu_pTrig,"PtCone20Rel","","<#mu>","",100,0,2,40,-0.5,39.5);
  BOOK2TRIG(m_pt_wiso_Mu,"p_{T}","GeV","<#mu>","",45,10,100,40,-0.5,39.5);
  BOOK2TRIG(m_pt_wiso_Mu_pTrig,"p_{T}","GeV","<#mu>","",45,10,100,40,-0.5,39.5);

  //Muons  
  BOOK2TRIG(e_pt_Mu,"p_{T}","GeV","<#mu>","",45,10,100,40,-0.5,39.5);
  BOOK2TRIG(e_pt_Mu_pTrig,"p_{T}","GeV","<#mu>","",45,10,100,40,-0.5,39.5);
  BOOK2TRIG(e_ptCone20Rel_Mu,"PtCone20Rel","","<#mu>","",100,0,2,40,-0.5,39.5);
  BOOK2TRIG(e_ptCone20Rel_Mu_pTrig,"PtCone20Rel","","<#mu>","",100,0,2,40,-0.5,39.5);
  BOOK2TRIG(e_pt_wiso_Mu,"p_{T}","GeV","<#mu>","",45,10,100,40,-0.5,39.5);
  BOOK2TRIG(e_pt_wiso_Mu_pTrig,"p_{T}","GeV","<#mu>","",45,10,100,40,-0.5,39.5);


#undef BOOK2TRIG

#define BOOK_FRCR(hN,xT,u,yT, ...)					\
  for(int k=0; k<nCR; k++){						\
    string scr;								\
    if(k==0) scr="CR0_";						\
    if(k==1) scr="2LCR0_";						\
    if(k==2) scr="2LCR1_";						\
    if(k==3) scr="2LCR2_";						\
    if(k==4) scr="2LCR3_";						\
    if(k==5) scr="3LCR0_";						\
    if(k==6) scr="3LCR1_";						\
    if(k==7) scr="3LCR2_";						\
    if(k==8) scr="3LCR3_";						\
    if(k==9) scr="3LCR4_";						\
    if(k==10) scr="3LCR5_";						\
for(int j=0; j<nHMC; j++){						\
      if(string(u).length()>0) sx = string(xT+string(" [")+u+string("]")); \
      else sx = 	string(xT);					\
      string stype;							\
      if(j==0) stype="pr_";						\
      if(j==1) stype="hf_";						\
      if(j==2) stype="lf_";						\
      if(j==3) stype="cv_";						\
      hN[0][j][k] = _utils->myTH1F((book_s1=string("sel_")+scr+stype+ #hN).c_str(), \
				   (book_s2=string("sel_")+scr+stype+ #hN).c_str(), \
				   __VA_ARGS__,sx.c_str() ,yT);		\
      _utils->yAxis(hN[0][j][k],u);					\
      									\
      hN[1][j][k] = _utils->myTH1F((book_s1=string("trm_")+scr+stype+ #hN).c_str(), \
				   (book_s2=string("trm_")+scr+stype+ #hN).c_str(), \
				   __VA_ARGS__,sx.c_str() ,yT);		\
      _utils->yAxis(hN[1][j][k],u);					\
    }									\
  }

  BOOK_FRCR(m_loose_pt,"P_{T}","GeV",syaxis,nMuPtBins,pt_m_bins);
  BOOK_FRCR(m_loose_eta,"#eta","",syaxis,10,-2.5,2.5);
  BOOK_FRCR(m_loose_d0S,"d0/#sigma_{d0}^{l1}","",syaxis,100,-10,10);
  BOOK_FRCR(m_loose_z0sintheta,"z0 sin(#theta)^{l1}","mm",syaxis,100,-2,2);
  BOOK_FRCR(m_loose_met,"#slash{E}_{T}","GeV",syaxis,20,0,200);
  BOOK_FRCR(m_loose_metrel,"#slash{E}_{T}^{rel}","GeV",syaxis,20,0,200);
  BOOK_FRCR(m_loose_dPhilmet,"dPhi(#slash{E}_{T},l)","degree",syaxis,9,0,180);
  BOOK_FRCR(m_loose_dPhijmet,"dPhi(#slash{E}_{T},j)","degree",syaxis,9,0,180);
  BOOK_FRCR(m_loose_nJets,"nJets","",syaxis,10,-0.5,9.5);
  BOOK_FRCR(m_tight_pt,"P_{T}","GeV",syaxis,nMuPtBins,pt_m_bins);
  BOOK_FRCR(m_tight_eta,"#eta","",syaxis,10,-2.5,2.5);
  BOOK_FRCR(m_tight_d0S,"d0/#sigma_{d0}^{l1}","",syaxis,100,-10,10);
  BOOK_FRCR(m_tight_z0sintheta,"z0 sin(#theta)^{l1}","mm",syaxis,100,-2,2);
  BOOK_FRCR(m_tight_met,"#slash{E}_{T}","GeV",syaxis,20,0,200);
  BOOK_FRCR(m_tight_metrel,"#slash{E}_{T}^{rel}","GeV",syaxis,20,0,200);
  BOOK_FRCR(m_tight_dPhilmet,"dPhi(#slash{E}_{T},l)","degree",syaxis,9,0,180);
  BOOK_FRCR(m_tight_dPhijmet,"dPhi(#slash{E}_{T},j)","degree",syaxis,9,0,180);
  BOOK_FRCR(m_tight_nJets,"nJets","",syaxis,10,-0.5,9.5);
  BOOK_FRCR(m_tightNI_pt,"P_{T}","GeV",syaxis,nMuPtBins,pt_m_bins);
  BOOK_FRCR(m_tightNI_eta,"#eta","",syaxis,10,-2.5,2.5);
  BOOK_FRCR(m_tightNI_d0S,"d0/#sigma_{d0}^{l1}","",syaxis,100,-10,10);
  BOOK_FRCR(m_tightNI_z0sintheta,"z0 sin(#theta)^{l1}","mm",syaxis,100,-2,2);
  BOOK_FRCR(m_tightNI_met,"#slash{E}_{T}","GeV",syaxis,20,0,200);
  BOOK_FRCR(m_tightNI2_pt,"P_{T}","GeV",syaxis,nMuPtBins,pt_m_bins);
  BOOK_FRCR(m_tightNI2_eta,"#eta","",syaxis,10,-2.5,2.5);
  BOOK_FRCR(m_tightNI2_d0S,"d0/#sigma_{d0}^{l1}","",syaxis,100,-10,10);
  BOOK_FRCR(m_tightNI2_z0sintheta,"z0 sin(#theta)^{l1}","mm",syaxis,100,-2,2);
  BOOK_FRCR(m_tightNI2_met,"#slash{E}_{T}","GeV",syaxis,20,0,200);
  BOOK_FRCR(m_tightNIIP_pt,"P_{T}","GeV",syaxis,nMuPtBins,pt_m_bins);
  BOOK_FRCR(m_tightNIIP_eta,"#eta","",syaxis,10,-2.5,2.5);
  BOOK_FRCR(m_tightNIIP_d0S,"d0/#sigma_{d0}^{l1}","",syaxis,100,-10,10);
  BOOK_FRCR(m_tightNIIP_z0sintheta,"z0 sin(#theta)^{l1}","mm",syaxis,100,-2,2);
  BOOK_FRCR(m_tightNIIP_met,"#slash{E}_{T}","GeV",syaxis,20,0,200);
  BOOK_FRCR(m_reff_pt,"P_{T}","GeV",syaxis,nMuPtBins,pt_m_bins);
  BOOK_FRCR(m_reff_eta,"#eta","",syaxis,10,-2.5,2.5);
  BOOK_FRCR(m_reff_met,"#slash{E}_{T}","GeV",syaxis,20,0,200);
  BOOK_FRCR(m_reff_metrel,"#slash{E}_{T}^{rel}","GeV",syaxis,20,0,200);
  BOOK_FRCR(m_reff_dPhilmet,"dPhi(#slash{E}_{T},l)","degree",syaxis,9,0,180);
  BOOK_FRCR(m_reff_dPhijmet,"dPhi(#slash{E}_{T},j)","degree",syaxis,9,0,180);
  BOOK_FRCR(m_reff_nJets,"nJets","",syaxis,10,-0.5,9.5);
  BOOK_FRCR(m_fr_pt,"P_{T}","GeV",syaxis,nMuPtBins,pt_m_bins);
  BOOK_FRCR(m_fr_eta,"#eta","",syaxis,10,-2.5,2.5);
  BOOK_FRCR(m_fr_met,"#slash{E}_{T}","GeV",syaxis,20,0,200);
  BOOK_FRCR(m_fr_metrel,"#slash{E}_{T}^{rel}","GeV",syaxis,20,0,200);
  BOOK_FRCR(m_fr_dPhilmet,"dPhi(#slash{E}_{T},l)","degree",syaxis,9,0,180);
  BOOK_FRCR(m_fr_dPhijmet,"dPhi(#slash{E}_{T},j)","degree",syaxis,9,0,180);
  BOOK_FRCR(m_fr_nJets,"nJets","",syaxis,10,-0.5,9.5);


  BOOK_FRCR(e_loose_pt,"P_{T}","GeV",syaxis,nElePtBins,pt_e_bins);
  BOOK_FRCR(e_loose_eta,"#eta","",syaxis,10,-2.5,2.5);
  BOOK_FRCR(e_loose_d0S,"d0/#sigma_{d0}^{l1}","",syaxis,100,-10,10);
  BOOK_FRCR(e_loose_z0sintheta,"z0 sin(#theta)^{l1}","mm",syaxis,100,-2,2);
  BOOK_FRCR(e_loose_met,"#slash{E}_{T}","GeV",syaxis,20,0,200);
  BOOK_FRCR(e_loose_metrel,"#slash{E}_{T}^{rel}","GeV",syaxis,20,0,200);
  BOOK_FRCR(e_loose_dPhilmet,"dPhi(#slash{E}_{T},l)","degree",syaxis,9,0,180);
  BOOK_FRCR(e_loose_dPhijmet,"dPhi(#slash{E}_{T},j)","degree",syaxis,9,0,180);
  BOOK_FRCR(e_loose_nJets,"nJets","",syaxis,10,-0.5,9.5);
  BOOK_FRCR(e_tight_pt,"P_{T}","GeV",syaxis,nElePtBins,pt_e_bins);
  BOOK_FRCR(e_tight_eta,"#eta","",syaxis,10,-2.5,2.5);
  BOOK_FRCR(e_tight_d0S,"d0/#sigma_{d0}^{l1}","",syaxis,100,-10,10);
  BOOK_FRCR(e_tight_z0sintheta,"z0 sin(#theta)^{l1}","mm",syaxis,100,-2,2);
  BOOK_FRCR(e_tight_met,"#slash{E}_{T}","GeV",syaxis,20,0,200);
  BOOK_FRCR(e_tight_metrel,"#slash{E}_{T}^{rel}","GeV",syaxis,20,0,200);
  BOOK_FRCR(e_tight_dPhilmet,"dPhi(#slash{E}_{T},l)","degree",syaxis,9,0,180);
  BOOK_FRCR(e_tight_dPhijmet,"dPhi(#slash{E}_{T},j)","degree",syaxis,9,0,180);
  BOOK_FRCR(e_tight_nJets,"nJets","",syaxis,10,-0.5,9.5);
  BOOK_FRCR(e_tightPP_pt,"P_{T}","GeV",syaxis,nElePtBins,pt_e_bins);
  BOOK_FRCR(e_tightPP_eta,"#eta","",syaxis,10,-2.5,2.5);
  BOOK_FRCR(e_tightPP_d0S,"d0/#sigma_{d0}^{l1}","",syaxis,100,-10,10);
  BOOK_FRCR(e_tightPP_z0sintheta,"z0 sin(#theta)^{l1}","mm",syaxis,100,-2,2);
  BOOK_FRCR(e_tightPP_met,"#slash{E}_{T}","GeV",syaxis,20,0,200);
  BOOK_FRCR(e_tightNI_pt,"P_{T}","GeV",syaxis,nElePtBins,pt_e_bins);
  BOOK_FRCR(e_tightNI_eta,"#eta","",syaxis,10,-2.5,2.5);
  BOOK_FRCR(e_tightNI_d0S,"d0/#sigma_{d0}^{l1}","",syaxis,100,-10,10);
  BOOK_FRCR(e_tightNI_z0sintheta,"z0 sin(#theta)^{l1}","mm",syaxis,100,-2,2);
  BOOK_FRCR(e_tightNI_met,"#slash{E}_{T}","GeV",syaxis,20,0,200);
  BOOK_FRCR(e_tightNI2_pt,"P_{T}","GeV",syaxis,nElePtBins,pt_e_bins);
  BOOK_FRCR(e_tightNI2_eta,"#eta","",syaxis,10,-2.5,2.5);
  BOOK_FRCR(e_tightNI2_d0S,"d0/#sigma_{d0}^{l1}","",syaxis,100,-10,10);
  BOOK_FRCR(e_tightNI2_z0sintheta,"z0 sin(#theta)^{l1}","mm",syaxis,100,-2,2);
  BOOK_FRCR(e_tightNI2_met,"#slash{E}_{T}","GeV",syaxis,20,0,200);
  BOOK_FRCR(e_tightNIIP_pt,"P_{T}","GeV",syaxis,nElePtBins,pt_e_bins);
  BOOK_FRCR(e_tightNIIP_eta,"#eta","",syaxis,10,-2.5,2.5);
  BOOK_FRCR(e_tightNIIP_d0S,"d0/#sigma_{d0}^{l1}","",syaxis,100,-10,10);
  BOOK_FRCR(e_tightNIIP_z0sintheta,"z0 sin(#theta)^{l1}","mm",syaxis,100,-2,2);
  BOOK_FRCR(e_tightNIIP_met,"#slash{E}_{T}","GeV",syaxis,20,0,200);
  BOOK_FRCR(e_reff_pt,"P_{T}","GeV",syaxis,nElePtBins,pt_e_bins);
  BOOK_FRCR(e_reff_eta,"#eta","",syaxis,10,-2.5,2.5);
  BOOK_FRCR(e_reff_met,"#slash{E}_{T}","GeV",syaxis,20,0,200);
  BOOK_FRCR(e_reff_metrel,"#slash{E}_{T}^{rel}","GeV",syaxis,20,0,200);
  BOOK_FRCR(e_reff_dPhilmet,"dPhi(#slash{E}_{T},l)","degree",syaxis,9,0,180);
  BOOK_FRCR(e_reff_dPhijmet,"dPhi(#slash{E}_{T},j)","degree",syaxis,9,0,180);
  BOOK_FRCR(e_reff_nJets,"nJets","",syaxis,10,-0.5,9.5);
  BOOK_FRCR(e_fr_pt,"P_{T}","GeV",syaxis,nElePtBins,pt_e_bins);
  BOOK_FRCR(e_fr_eta,"#eta","",syaxis,10,-2.5,2.5);
  BOOK_FRCR(e_fr_met,"#slash{E}_{T}","GeV",syaxis,20,0,200);
  BOOK_FRCR(e_fr_metrel,"#slash{E}_{T}^{rel}","GeV",syaxis,20,0,200);
  BOOK_FRCR(e_fr_dPhilmet,"dPhi(#slash{E}_{T},l)","degree",syaxis,9,0,180);
  BOOK_FRCR(e_fr_dPhijmet,"dPhi(#slash{E}_{T},j)","degree",syaxis,9,0,180);
  BOOK_FRCR(e_fr_nJets,"nJets","",syaxis,10,-0.5,9.5);


#undef BOOK_FRCR



#define PBOOK1(hN,xT,u,yT, ...)						\
  for(int j=0; j<nHMC; j++){						\
    if(string(u).length()>0) sx = string(xT+string(" [")+u+string("]")); \
    else sx = 	string(xT);						\
    string stype;							\
    if(j==0) stype="pr_";						\
    if(j==1) stype="hf_";						\
    if(j==2) stype="lf_";						\
    if(j==3) stype="cv_";						\
    hN[0][j] = _utils->myTProfile((book_s1=string("sel_")+stype+ #hN).c_str(), \
				  (book_s2=string("sel_")+stype+ #hN).c_str(), \
				  __VA_ARGS__,sx.c_str() ,yT);		\
    _utils->yAxis(hN[0][j],u);						\
    									\
    hN[1][j] = _utils->myTProfile((book_s1=string("trm_")+stype+ #hN).c_str(), \
				  (book_s2=string("trm_")+stype+ #hN).c_str(), \
				  __VA_ARGS__,sx.c_str() ,yT);		\
    _utils->yAxis(hN[1][j],u);						\
  }

  PBOOK1(m_iso_mu,"<#mu>","","Iso",40,-0.5,39.5,-1,10);
  PBOOK1(m_iso_npv,"N_{pv}","","Iso",30,-0.5,29.5,-1,10);
  PBOOK1(e_iso_mu,"<#mu>","","Iso",40,-0.5,39.5,-1,10);
  PBOOK1(e_iso_npv,"N_{pv}","","Iso",30,-0.5,29.5,-1,10);

#undef PBOOK1



#define BOOK_DG2Lfk(hN, ...)						\
  for(int i=0; i<nHSR_DG2L; i++){					\
    string sr;								\
    if(i==0) sr="DG2L_SRjveto_";					\
    if(i==1) sr="DG2L_SRSSjveto_";					\
    if(i==2) sr="DG2L_SR2jet_";					\
    if(i==3) sr="DG2L_SRmT2_";					\
    if(i==4) sr="DG2L_SR5_";						\
    if(i==5) sr="DG2L_CRZ_";						\
    if(i==6) sr="DG2L_NTOP_";					\
    if(i==7) sr="DG2L_NWW1_";					\
    if(i==8) sr="DG2L_NWW2_";					\
    if(i==9) sr="DG2L_CR2LepOS_";					\
    if(i==10) sr="DG2L_CR2LepSS_";					\
    for(int j=0; j<2; j++){						\
      string sFlav;							\
      if(j==0) sFlav="E_";						\
      if(j==1) sFlav="M_";						\
      for(int k=0; k<nHMC; k++){					\
	string stype;							\
	if(k==0) stype="pr_";						\
	if(k==1) stype="hf_";						\
	if(k==2) stype="lf_";						\
	if(k==3) stype="cv_";						\
									\
	hN[i][j][k] = new TH3F((book_s1=sr+sFlav+stype+ #hN).c_str(),	\
			       (book_s2=sr+sFlav+stype+ #hN).c_str(),	\
			       __VA_ARGS__);				\
      }									\
    }									\
  }
  
  //Muons  
  BOOK_DG2Lfk(DG2L_fakeLep,5,10,60,4,0,2.5,5,0,100);

#undef BOOK_DG2Lfk

#define BOOK_MLfk(hN, ...)						\
  for(int i=0; i<nHSR_ML; i++){						\
    string sr;								\
    if(i==0) sr="ML_SR3Lep_";						\
    if(i==1) sr="ML_SRB_";						\
    if(i==2) sr="ML_SR1a_";						\
    if(i==3) sr="ML_SR1b_";						\
    if(i==4) sr="ML_SR2_";						\
    if(i==5) sr="ML_VR0_";						\
    if(i==6) sr="ML_VR1_";						\
    if(i==7) sr="ML_VR2_";						\
    if(i==8) sr="ML_VR3_";						\
    if(i==9) sr="ML_VRWZ_";						\
    if(i==10) sr="ML_NRWZ_";						\
    if(i==11) sr="ML_SR4lep_";						\
    if(i==12) sr="ML_SR4lepNoZ_";					\
    for(int j=0; j<2; j++){						\
      string sFlav;							\
      if(j==0) sFlav="E_";						\
      if(j==1) sFlav="M_";						\
      for(int k=0; k<nHMC; k++){					\
	string stype;							\
	if(k==0) stype="pr_";						\
	if(k==1) stype="hf_";						\
	if(k==2) stype="lf_";						\
	if(k==3) stype="cv_";						\
									\
	hN[i][j][k] = new TH3F((book_s1=sr+sFlav+stype+ #hN).c_str(),	\
			       (book_s2=sr+sFlav+stype+ #hN).c_str(),	\
			       __VA_ARGS__);				\
      }									\
    }									\
  }
  
  //Muons  
  BOOK_MLfk(ML_fakeLep,5,10,60,4,0,2.5,5,0,100);

#undef BOOK_MLfk




  Mee_os= _utils->myTH1F("Mee_os", "Mee_os",28,20,160,"M_{ee} [GeV]",syaxis);
  Mmm_os= _utils->myTH1F("Mmm_os", "Mmm_os",28,20,160,"M_{#mu#mu} [GeV]",syaxis);

  W_mu= _utils->myTH1F("w_mu", "w_mu",100,0,3,"Mu_{weight}",syaxis);
  

}
