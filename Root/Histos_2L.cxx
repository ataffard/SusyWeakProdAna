#include "SusyWeakProdAna/Histos_2L.h"
#include "TSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
/*--------------------------------------------------------------------------------*/
// SusyHistos Book2LHistograms
/*--------------------------------------------------------------------------------*/
void Histos_2L::Book2LHistograms(TDirectory* _hDir, bool useSys)
{
  std::cout << "Booking 2L histos " << std::endl;
  std::cout << "Number of Signal regions " << DIL_NSR << endl;

  _hDir->cd();

  string    book_s1, book_s2,sx, sy, sz;
  char      syaxis[200];
  sprintf(syaxis,"Entries");

  int maxSys = DGSys_N;
  if(useSys==false){
    maxSys=1;
    cout << "\t No systematics histos booked " << endl;
  }
  //
  //SR histogram
  //

#define BOOK_SRDG2L(hN,xT,u,yT, ...)					\
  for(int i=0; i<3; i++){						\
    string sFlav;							\
    if(i==0) sFlav="EE_";						\
    if(i==1) sFlav="MM_";						\
    if(i==2) sFlav="EM_";						\
    for(int j=0; j<DIL_NSR; j++){					\
      for(int isys=0; isys<maxSys; isys++){				\
	if(string(u).length()>0) sx = string(xT+string(" [")+u+string("]")); \
	else sx = 	string(xT);					\
	string stype = "DG2L_" + DIL_SRNAME[j] + "_" + sFlav + #hN + "_" + DG2LSystNames[isys];	\
	hN[j][i][isys] = (TH1F*) _utils->myTH1F(stype.c_str(),		\
						stype.c_str(),		\
						__VA_ARGS__,sx.c_str() ,yT); \
	_utils->yAxis(hN[j][i][isys],u);				\
      }									\
    }									\
  }
  
  BOOK_SRDG2L(DG2L_pred,"","",syaxis,1,-0.5,0.5);
  BOOK_SRDG2L(DG2L_Zcount,"","",syaxis,520,1,520);
  BOOK_SRDG2L(DG2L_cutflow,"","",syaxis,20,-0.5,19.5);
  BOOK_SRDG2L(DG2L_nJets,"NJets","",syaxis,10,-0.5,9.5);
  BOOK_SRDG2L(DG2L_nCJets,"NJets - central","",syaxis,10,-0.5,9.5);
  BOOK_SRDG2L(DG2L_nFJets,"NJets - forward ","",syaxis,10,-0.5,9.5);
  BOOK_SRDG2L(DG2L_nBJets,"NBJets","",syaxis,5,-0.5,4.5);
  BOOK_SRDG2L(DG2L_nSoftJets,"NSoftJets","",syaxis,10,-0.5,9.5);
  BOOK_SRDG2L(DG2L_qq,"","",syaxis,10,-0.5,1.5);
  BOOK_SRDG2L(DG2L_mll,"m_{ll}","GeV",syaxis,130,20,280); 
  BOOK_SRDG2L(DG2L_mllcoarse,"m_{ll}","GeV",syaxis,52,20,280); 
  BOOK_SRDG2L(DG2L_mllcoarser,"m_{ll}","GeV",syaxis,26,20,280); 
  BOOK_SRDG2L(DG2L_mjj,"m_{jj}","GeV",syaxis,50,0,500);
  BOOK_SRDG2L(DG2L_pTll,"p_{T}^{ll}","GeV",syaxis,42,10,220);
  BOOK_SRDG2L(DG2L_mWWT,"m_{T}^{WW}","GeV",syaxis,50,0,500);
  BOOK_SRDG2L(DG2L_dPhill,"dPhi(l,l)","degree",syaxis,9,0,180);
  BOOK_SRDG2L(DG2L_dPhilMet,"dPhi(l,Met)","degree",syaxis,9,0,180);
  BOOK_SRDG2L(DG2L_dPhiJetMet,"dPhi(j,Met)","degree",syaxis,9,0,180);
  BOOK_SRDG2L(DG2L_JZBJet,"JZB","GeV",syaxis,120,-400,800);
  BOOK_SRDG2L(DG2L_JZBEtmiss,"JZB","GeV",syaxis,120,-400,800);
  BOOK_SRDG2L(DG2L_etmiss,"#slash{E}_{T}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_etmissPhi,"Phi_{#slash{E}_{T}}","degree",syaxis,18,-180,180);
  BOOK_SRDG2L(DG2L_metrel,"E_{T}^{miss,rel}","GeV",syaxis,20,0,200);
  BOOK_SRDG2L(DG2L_metrel1,"E_{T}^{miss,rel}","GeV",syaxis,13,40,300);
  BOOK_SRDG2L(DG2L_metrel2,"E_{T}^{miss,rel}","GeV",syaxis,26,40,300);
  BOOK_SRDG2L(DG2L_metrel3,"E_{T}^{miss,rel}","GeV",syaxis,18,40,400);
  BOOK_SRDG2L(DG2L_metRefEle,"#slash{E}_{T}^{RefEle}","GeV",syaxis,20,0,200);
  BOOK_SRDG2L(DG2L_metRefGam,"#slash{E}_{T}^{RefGamma}","GeV",syaxis,20,0,200);
  BOOK_SRDG2L(DG2L_metRefMuo,"#slash{E}_{T}^{RefMuo}","GeV",syaxis,20,0,200);
  BOOK_SRDG2L(DG2L_metRefJet,"#slash{E}_{T}^{RefJet}","GeV",syaxis,20,0,200);
  BOOK_SRDG2L(DG2L_metRefSJet,"#slash{E}_{T}^{RefSoftJet}","GeV",syaxis,20,0,200);
  BOOK_SRDG2L(DG2L_metCellout,"#slash{E}_{T}^{Cellout}","GeV",syaxis,20,0,200);
  BOOK_SRDG2L(DG2L_mt2,"m_{T2}","GeV",syaxis,20,0,200);
  BOOK_SRDG2L(DG2L_mt2b,"m_{T2}","GeV",syaxis,40,0,400);
  BOOK_SRDG2L(DG2L_mEff,"m_{eff}","GeV",syaxis,100,0,1000);
  BOOK_SRDG2L(DG2L_ST,"S_{T}","GeV",syaxis,100,0,1000);
  BOOK_SRDG2L(DG2L_mTl1,"m_{T}^{l1}","GeV",syaxis,100,0,500);
  BOOK_SRDG2L(DG2L_mTl2,"m_{T}^{l2}","GeV",syaxis,100,0,500);
  BOOK_SRDG2L(DG2L_npv,"N_{pv}","",syaxis,30,-0.5,29.5);
  BOOK_SRDG2L(DG2L_mu,"<#mu>","",syaxis,40,-0.5,39.5);
  BOOK_SRDG2L(DG2L_ptl1,"p_{T}^{l1}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_ptl2,"p_{T}^{l2}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_etal1,"#eta^{l1}","",syaxis,25,-2.5,2.5);
  BOOK_SRDG2L(DG2L_etal2,"#eta^{l2}","",syaxis,25,-2.5,2.5);
  BOOK_SRDG2L(DG2L_ePt,"p_{T}^{e}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_mPt,"p_{T}^{#mu}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_eEta,"#eta^{e}","",syaxis,25,-2.5,2.5);
  BOOK_SRDG2L(DG2L_mEta,"#eta^{#mu}","",syaxis,25,-2.5,2.5);
  BOOK_SRDG2L(DG2L_d0Sl1,"d0/#sigma_{d0}^{l1}","",syaxis,100,-10,10);
  BOOK_SRDG2L(DG2L_d0Sl2,"d0/#sigma_{d0}^{l2}","",syaxis,100,-10,10);
  BOOK_SRDG2L(DG2L_z0sinthetal1,"z0 sin(#theta)^{l1}","cm",syaxis,200,-1,1);
  BOOK_SRDG2L(DG2L_z0sinthetal2,"z0 sin(#theta)^{l2}","cm",syaxis,200,-1,1);
  BOOK_SRDG2L(DG2L_orgl1,"MC Origin","",syaxis,5,-0.5,4.5);
  BOOK_SRDG2L(DG2L_orgl2,"MC Origin","",syaxis,5,-0.5,4.5);
  BOOK_SRDG2L(DG2L_ptj1,"p_{T}^{j1}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_ptj2,"p_{T}^{j2}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_ptj3,"p_{T}^{j3}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_ptj4,"p_{T}^{j4}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_etaj1,"#eta^{j1}","",syaxis,25,-2.5,2.5);
  BOOK_SRDG2L(DG2L_etaj2,"#eta^{j2}","",syaxis,25,-2.5,2.5);
  BOOK_SRDG2L(DG2L_etaj3,"#eta^{j3}","",syaxis,25,-2.5,2.5);
  BOOK_SRDG2L(DG2L_etaj4,"#eta^{j4}","",syaxis,25,-2.5,2.5);
  BOOK_SRDG2L(DG2L_jvfj1,"JVF^{j1}","",syaxis,10,0,1);
  BOOK_SRDG2L(DG2L_jvfj2,"JVF^{j2}","",syaxis,10,0,1);
  BOOK_SRDG2L(DG2L_ptbj,"p_{T}^{bjet}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_etabj,"#eta^{bjet}","",syaxis,25,-2.5,2.5);
  BOOK_SRDG2L(DG2L_jvfbj,"JVF^{j1}","",syaxis,10,0,1);
  BOOK_SRDG2L(DG2L_ptSj1,"p_{T}^{j1-soft}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_ptSj2,"p_{T}^{j2-soft}","GeV",syaxis,40,0,200);
  BOOK_SRDG2L(DG2L_etaSj1,"#eta^{j1-soft}","",syaxis,25,-2.5,2.5);
  BOOK_SRDG2L(DG2L_etaSj2,"#eta^{j2-soft}","",syaxis,25,-2.5,2.5);
  BOOK_SRDG2L(DG2L_jvfSj1,"JVF^{j1-soft}","",syaxis,10,0,1);
  BOOK_SRDG2L(DG2L_jvfSj2,"JVF^{j2-soft}","",syaxis,10,0,1);


#undef BOOK_SRDG2L
  
  std::vector<TString> LepType;
  LepType.clear();
  LepType.push_back("PR");
  LepType.push_back("CONV");
  LepType.push_back("HF");
  LepType.push_back("LF");
  LepType.push_back("Unknown");
 
  for(uint i=0; i<DIL_NSR; i++){
    for(int ilep=0; ilep<3; ilep++){			
      for(uint j=0; j<LepType.size(); j++){
	for(int isys=0; isys<maxSys; isys++){				
	  DG2L_orgl1[i][ilep][isys]->GetXaxis()->SetBinLabel(j+1,LepType.at(j).Data());
	  DG2L_orgl2[i][ilep][isys]->GetXaxis()->SetBinLabel(j+1,LepType.at(j).Data());
	}
      }
    }
  }
  
  string dir =  string(getenv("WORKAREA")) + "/SusyWeakProdAna/data";
  string fileName  = dir + "/" + "Moriond_lumi_GRL_v58.txt";

  ifstream _file;
  _file.open(fileName.c_str());
  if (!_file.is_open()){
    cerr << "Failed opening " << fileName << endl;
    exit(1);
  }
  
  int run;
  float lumi;

  int ibin=1;
  runBins.clear();
  while( _file >> run >> lumi){
    string sRun;
    ostringstream convert;
    convert << run;  
    sRun = convert.str();
    runBins.insert(runBins.end(),make_pair(run,ibin));
    for(uint i=0; i<DIL_NSR; i++){
      for(int ilep=0; ilep<3; ilep++){	
	for(int isys=0; isys<maxSys; isys++){
	  DG2L_Zcount[i][ilep][isys]->GetXaxis()->SetBinLabel(ibin,sRun.c_str());
	}
      }
    }
    ibin++;
  }

  _file.close();
  



}
