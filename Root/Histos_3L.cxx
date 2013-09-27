#include "SusyWeakProdAna/Histos_3L.h"

/*--------------------------------------------------------------------------------*/
// SusyHistos Book3LHistograms
/*--------------------------------------------------------------------------------*/
void Histos_3L::Book3LHistograms(TDirectory* _hDir, bool useSys)
{
  std::cout << "Booking 3L histos " << std::endl;
  std::cout << "Number of Signal regions " << ML_NSR << endl;

  _hDir->cd();

  string    book_s1, book_s2,sx, sy, sz;
  char      syaxis[200];
  sprintf(syaxis,"Entries");

  int maxSys = DGSys_N;
  if(useSys==false){
    maxSys=1;
    cout << "\t No systematics histos booked " << endl;
  }
  
  
#define BOOK_SRML(hN,xT,u,yT, ...)					\
  for(int i=0; i<ML_N; i++){					 	\
    string sFlav=string(ML_FLAV[i]) + "_";				\
    for(int j=0; j<ML_NSR; j++){					\
      for(int isys=0; isys<maxSys; isys++){				\
      if(string(u).length()>0) sx = string(xT+string(" [")+u+string("]")); \
      else sx = 	string(xT);					\
      string stype = "ML_" + ML_SRNAME[j] + "_" + sFlav + #hN + "_" + DGSystNames[isys]; \
      hN[j][i][isys] = (TH1F*) _utils->myTH1F(stype.c_str(),		\
					      stype.c_str(),		\
					      __VA_ARGS__,sx.c_str() ,yT); \
      _utils->yAxis(hN[j][i][isys],u);					\
      }									\
    }									\
  }
  
  BOOK_SRML(ML_pred,"","",syaxis,1,-0.5,0.5);
  BOOK_SRML(ML_predGe2J,"","",syaxis,1,-0.5,0.5);
  BOOK_SRML(ML_cutflow,"","",syaxis,10,-0.5,9.5);
  BOOK_SRML(ML_evtCatgUnOrdered,"","",syaxis,4,-0.5,3.5); //eee, mmm, eem, mme
  BOOK_SRML(ML_evtCatgOSpair,"","",syaxis,6,-0.5,5.5); //(ee)e, (ee)m, (mm)e, (mm)m, (em/me)e, (em/me)m,  
  BOOK_SRML(ML_evtCatgSSpair,"","",syaxis,6,-0.5,5.5); //(ee)e, (ee)m, (mm)e, (mm)m, (em/me)e, (em/me)m,  
  BOOK_SRML(ML_nLep,"NLeptons","",syaxis,10,-0.5,9.5);
  BOOK_SRML(ML_nJets,"NJets","",syaxis,10,-0.5,9.5);
  BOOK_SRML(ML_nBJets,"NBJets","",syaxis,5,-0.5,4.5);
  BOOK_SRML(ML_SFOSMll,"M_{ll}^{SFOS}","",syaxis,105,10,220);
  BOOK_SRML(ML_SFOSMlll,"M_{lll}^{SFOS}","",syaxis,105,10,220);
  BOOK_SRML(ML_AllMll,"M_{ll}^{all}","",syaxis,105,10,220);
  BOOK_SRML(ML_AllMlll,"M_{lll}^{all}","",syaxis,105,10,220);
  BOOK_SRML(ML_SFOSMZZ,"M_{llll}^{SFOS}","",syaxis,50,100,600);
  BOOK_SRML(ML_SFOSMT,"M_{T}^{SFOS}","",syaxis,50,20,220);
  BOOK_SRML(ML_etmiss,"#slash{E}_{T}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_metrel,"E_{T}^{miss,rel}","GeV",syaxis,15,0,300);
  BOOK_SRML(ML_meff,"m_{eff}","GeV",syaxis,100,0,1000);
  BOOK_SRML(ML_metSig,"MET significance","",syaxis,20,0,10);
  BOOK_SRML(ML_mt2,"m_{T2}","GeV",syaxis,20,0,200);
  BOOK_SRML(ML_mt2b,"m_{T2}","GeV",syaxis,40,0,400);
  BOOK_SRML(ML_mct,"m_{CT}","GeV",syaxis,29,10,300);
  BOOK_SRML(ML_mctPerp,"m_{CT#perp}","GeV",syaxis,29,10,300);
  BOOK_SRML(ML_mt2_0J,"m_{T2}","GeV",syaxis,20,0,200);
  BOOK_SRML(ML_mt2b_0J,"m_{T2}","GeV",syaxis,40,0,400);
  BOOK_SRML(ML_mct_0J,"m_{CT}","GeV",syaxis,29,10,300);
  BOOK_SRML(ML_mctPerp_0J,"m_{CT#perp}","GeV",syaxis,29,10,300);
  BOOK_SRML(ML_metRefEle,"#slash{E}_{T}^{RefEle}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_metRefGam,"#slash{E}_{T}^{RefGam}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_metRefMuo,"#slash{E}_{T}^{RefMuo}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_metRefJet,"#slash{E}_{T}^{RefJet}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_metRefSJet,"#slash{E}_{T}^{RefSoftJet}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_metCellout,"#slash{E}_{T}^{Cellout}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_ptl1,"P_{T}^{l1}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_ptl2,"P_{T}^{l2}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_ptl3,"P_{T}^{l3}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_ptl4,"P_{T}^{l4}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_etal1,"#eta^{l1}","",syaxis,25,-2.5,2.5);
  BOOK_SRML(ML_etal2,"#eta^{l2}","",syaxis,25,-2.5,2.5);
  BOOK_SRML(ML_etal3,"#eta^{l3}","",syaxis,25,-2.5,2.5);
  BOOK_SRML(ML_etal4,"#eta^{l4}","",syaxis,25,-2.5,2.5);
  BOOK_SRML(ML_d0Sl1,"d0/#sigma_{d0}^{l1}","",syaxis,100,-10,10);
  BOOK_SRML(ML_d0Sl2,"d0/#sigma_{d0}^{l2}","",syaxis,100,-10,10);
  BOOK_SRML(ML_d0Sl3,"d0/#sigma_{d0}^{l3}","",syaxis,100,-10,10);
  BOOK_SRML(ML_d0Sl4,"d0/#sigma_{d0}^{l4}","",syaxis,100,-10,10);
  BOOK_SRML(ML_z0sinthetal1,"z0 sin(#theta)^{l1}","cm",syaxis,200,-1,1);
  BOOK_SRML(ML_z0sinthetal2,"z0 sin(#theta)^{l2}","cm",syaxis,200,-1,1);
  BOOK_SRML(ML_z0sinthetal3,"z0 sin(#theta)^{l3}","cm",syaxis,200,-1,1);
  BOOK_SRML(ML_z0sinthetal4,"z0 sin(#theta)^{l4}","cm",syaxis,200,-1,1);
  BOOK_SRML(ML_orgl1,"MC Origin","",syaxis,5,-0.5,4.5);
  BOOK_SRML(ML_orgl2,"MC Origin","",syaxis,5,-0.5,4.5);
  BOOK_SRML(ML_orgl3,"MC Origin","",syaxis,5,-0.5,4.5);
  BOOK_SRML(ML_orgl4,"MC Origin","",syaxis,5,-0.5,4.5);
  BOOK_SRML(ML_pTll,"p_{T}^{ll}","GeV",syaxis,42,10,220);
  BOOK_SRML(ML_dRll,"dR_{ll}","",syaxis,40,0,4);
  BOOK_SRML(ML_ptj1,"P_{T}^{j1}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_ptj2,"P_{T}^{j2}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_ptj3,"P_{T}^{j3}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_ptj4,"P_{T}^{j4}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_etaj1,"#eta^{j1}","",syaxis,25,-2.5,2.5);
  BOOK_SRML(ML_etaj2,"#eta^{j2}","",syaxis,25,-2.5,2.5);
  BOOK_SRML(ML_etaj3,"#eta^{j3}","",syaxis,25,-2.5,2.5);
  BOOK_SRML(ML_etaj4,"#eta^{j4}","",syaxis,25,-2.5,2.5);
  BOOK_SRML(ML_ptbj,"P_{T}^{bjet}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_etabj,"#eta^{bjet}","",syaxis,25,-2.5,2.5);
  BOOK_SRML(ML_mjj,"m_{jj}","GeV",syaxis,50,0,500);

  
#undef BOOK_SRML

  /*
  std::vector<TString> v3Lep;
  v3Lep.clear();
  v3Lep.push_back("eee");   
  v3Lep.push_back("#mu#mu#mu");   
  v3Lep.push_back("ee#mu");   
  v3Lep.push_back("#mu#mue"); 

  std::vector<TString> v3LepOS;
  v3LepOS.clear();
  v3LepOS.push_back("(ee)e");   
  v3LepOS.push_back("(ee)#mu");   
  v3LepOS.push_back("(#mu#mu)e");   
  v3LepOS.push_back("(#mu#mu)m");   
  v3LepOS.push_back("(e#mu/#mu e)e");   
  v3LepOS.push_back("(e#mu/#mu e)#mu");   

  std::vector<TString> LepType;
  LepType.clear();
  LepType.push_back("PR");
  LepType.push_back("CONV");
  LepType.push_back("HF");
  LepType.push_back("LF");
  LepType.push_back("Unknown");

  for(int i=0; i<ML_N; i++){
    for(uint j=0; j<ML_NSR; j++){
      for(int isys=0; isys<maxSys; isys++){
	for(uint b=0; b<v3Lep.size(); b++)
	  ML_evtCatgUnOrdered[j][i][isys]->GetXaxis()->SetBinLabel(b+1,v3Lep.at(b).Data());
	for(uint b=0; b<v3LepOS.size(); b++)
	  ML_evtCatgOSpair[j][i][isys]->GetXaxis()->SetBinLabel(b+1,v3LepOS.at(b).Data());
	
	for(uint j=0; j<LepType.size(); j++){
	  ML_orgl1[j][i][isys]->GetXaxis()->SetBinLabel(j+1,LepType.at(j).Data());
	  ML_orgl2[j][i][isys]->GetXaxis()->SetBinLabel(j+1,LepType.at(j).Data());
	  ML_orgl3[j][i][isys]->GetXaxis()->SetBinLabel(j+1,LepType.at(j).Data());
	  ML_orgl4[j][i][isys]->GetXaxis()->SetBinLabel(j+1,LepType.at(j).Data());
	}
      }
    }
  }

  */


}


/*--------------------------------------------------------------------------------*/
// SusyHistos Book3LHistograms
/*--------------------------------------------------------------------------------*/
void Histos_3L::Sum3LHistograms()
{
  std::cout << "Summing 3L histos " << std::endl;

  for(int j=0; j<ML_NSR; j++){
    for(int isys=0; isys<DGSys_N; isys++){
      
      //Sum 3L      
      for(int i=ET_eee; i<ET_lll; i++){
	ML_pred[j][ET_lll][isys]->Add(ML_pred[j][i][isys]);
	ML_predGe2J[j][ET_lll][isys]->Add(ML_predGe2J[j][i][isys]);
	ML_cutflow[j][ET_lll][isys]->Add(ML_cutflow[j][i][isys]);
	ML_evtCatgUnOrdered[j][ET_lll][isys]->Add(ML_evtCatgUnOrdered[j][i][isys]);
	ML_evtCatgOSpair[j][ET_lll][isys]->Add(ML_evtCatgOSpair[j][i][isys]);
	ML_evtCatgSSpair[j][ET_lll][isys]->Add(ML_evtCatgSSpair[j][i][isys]);
	ML_nLep[j][ET_lll][isys]->Add(ML_nLep[j][i][isys]);
	ML_nJets[j][ET_lll][isys]->Add(ML_nJets[j][i][isys]);
	ML_nBJets[j][ET_lll][isys]->Add(ML_nBJets[j][i][isys]);
	ML_SFOSMll[j][ET_lll][isys]->Add(ML_SFOSMll[j][i][isys]);
	ML_SFOSMlll[j][ET_lll][isys]->Add(ML_SFOSMlll[j][i][isys]);
	ML_AllMll[j][ET_lll][isys]->Add(ML_AllMll[j][i][isys]);
	ML_AllMlll[j][ET_lll][isys]->Add(ML_AllMlll[j][i][isys]);
	ML_SFOSMZZ[j][ET_lll][isys]->Add(ML_SFOSMZZ[j][i][isys]);
	ML_SFOSMT[j][ET_lll][isys]->Add(ML_SFOSMT[j][i][isys]);
	ML_etmiss[j][ET_lll][isys]->Add(ML_etmiss[j][i][isys]);
	ML_metrel[j][ET_lll][isys]->Add(ML_metrel[j][i][isys]);
	ML_meff[j][ET_lll][isys]->Add(ML_meff[j][i][isys]);
	ML_metSig[j][ET_lll][isys]->Add(ML_metSig[j][i][isys]);
	ML_mct[j][ET_lll][isys]->Add(ML_mct[j][i][isys]);
	ML_mctPerp[j][ET_lll][isys]->Add(ML_mctPerp[j][i][isys]);
	ML_mt2[j][ET_lll][isys]->Add(ML_mt2[j][i][isys]);
	ML_mt2b[j][ET_lll][isys]->Add(ML_mt2b[j][i][isys]);
	ML_mct_0J[j][ET_lll][isys]->Add(ML_mct_0J[j][i][isys]);
	ML_mctPerp_0J[j][ET_lll][isys]->Add(ML_mctPerp_0J[j][i][isys]);
	ML_mt2_0J[j][ET_lll][isys]->Add(ML_mt2_0J[j][i][isys]);
	ML_mt2b_0J[j][ET_lll][isys]->Add(ML_mt2b_0J[j][i][isys]);
	ML_metRefEle[j][ET_lll][isys]->Add(ML_metRefEle[j][i][isys]);
	ML_metRefGam[j][ET_lll][isys]->Add(ML_metRefGam[j][i][isys]);
	ML_metRefMuo[j][ET_lll][isys]->Add(ML_metRefMuo[j][i][isys]);
	ML_metRefJet[j][ET_lll][isys]->Add(ML_metRefJet[j][i][isys]);
	ML_metRefSJet[j][ET_lll][isys]->Add(ML_metRefSJet[j][i][isys]);
	ML_metCellout[j][ET_lll][isys]->Add(ML_metCellout[j][i][isys]);
	ML_ptl1[j][ET_lll][isys]->Add(ML_ptl1[j][i][isys]);
	ML_ptl2[j][ET_lll][isys]->Add(ML_ptl2[j][i][isys]);
	ML_ptl3[j][ET_lll][isys]->Add(ML_ptl3[j][i][isys]);
	ML_ptl4[j][ET_lll][isys]->Add(ML_ptl4[j][i][isys]);
	ML_etal1[j][ET_lll][isys]->Add(ML_etal1[j][i][isys]);
	ML_etal2[j][ET_lll][isys]->Add(ML_etal2[j][i][isys]);
	ML_etal3[j][ET_lll][isys]->Add(ML_etal3[j][i][isys]);
	ML_etal4[j][ET_lll][isys]->Add(ML_etal4[j][i][isys]);
	ML_d0Sl1[j][ET_lll][isys]->Add(ML_d0Sl1[j][i][isys]);
	ML_d0Sl2[j][ET_lll][isys]->Add(ML_d0Sl2[j][i][isys]);
	ML_d0Sl3[j][ET_lll][isys]->Add(ML_d0Sl3[j][i][isys]);
	ML_d0Sl4[j][ET_lll][isys]->Add(ML_d0Sl4[j][i][isys]);
	ML_z0sinthetal1[j][ET_lll][isys]->Add(ML_z0sinthetal1[j][i][isys]);
	ML_z0sinthetal2[j][ET_lll][isys]->Add(ML_z0sinthetal2[j][i][isys]);
	ML_z0sinthetal3[j][ET_lll][isys]->Add(ML_z0sinthetal3[j][i][isys]);
	ML_z0sinthetal4[j][ET_lll][isys]->Add(ML_z0sinthetal4[j][i][isys]);
	ML_orgl1[j][ET_lll][isys]->Add(ML_orgl1[j][i][isys]);
	ML_orgl2[j][ET_lll][isys]->Add(ML_orgl2[j][i][isys]);
	ML_orgl3[j][ET_lll][isys]->Add(ML_orgl3[j][i][isys]);
	ML_orgl4[j][ET_lll][isys]->Add(ML_orgl4[j][i][isys]);
	ML_pTll[j][ET_lll][isys]->Add(ML_pTll[j][i][isys]);
	ML_dRll[j][ET_lll][isys]->Add(ML_dRll[j][i][isys]);
	ML_ptj1[j][ET_lll][isys]->Add(ML_ptj1[j][i][isys]);
	ML_ptj2[j][ET_lll][isys]->Add(ML_ptj2[j][i][isys]);
	ML_ptj3[j][ET_lll][isys]->Add(ML_ptj3[j][i][isys]);
	ML_ptj4[j][ET_lll][isys]->Add(ML_ptj4[j][i][isys]);
	ML_etaj1[j][ET_lll][isys]->Add(ML_etaj1[j][i][isys]);
	ML_etaj2[j][ET_lll][isys]->Add(ML_etaj2[j][i][isys]);
	ML_etaj3[j][ET_lll][isys]->Add(ML_etaj3[j][i][isys]);
	ML_etaj4[j][ET_lll][isys]->Add(ML_etaj4[j][i][isys]);
	ML_ptbj[j][ET_lll][isys]->Add(ML_ptbj[j][i][isys]);
	ML_etabj[j][ET_lll][isys]->Add(ML_etabj[j][i][isys]);
	ML_mjj[j][ET_lll][isys]->Add(ML_mjj[j][i][isys]);
      }


      //Sum 4L      
      for(int i=ET_eeee; i<ET_llll; i++){
	ML_pred[j][ET_llll][isys]->Add(ML_pred[j][i][isys]);
	ML_predGe2J[j][ET_llll][isys]->Add(ML_predGe2J[j][i][isys]);
	ML_cutflow[j][ET_llll][isys]->Add(ML_cutflow[j][i][isys]);
	ML_evtCatgUnOrdered[j][ET_llll][isys]->Add(ML_evtCatgUnOrdered[j][i][isys]);
	ML_evtCatgOSpair[j][ET_llll][isys]->Add(ML_evtCatgOSpair[j][i][isys]);
	ML_evtCatgSSpair[j][ET_llll][isys]->Add(ML_evtCatgSSpair[j][i][isys]);
	ML_nLep[j][ET_llll][isys]->Add(ML_nLep[j][i][isys]);
	ML_nJets[j][ET_llll][isys]->Add(ML_nJets[j][i][isys]);
	ML_nBJets[j][ET_llll][isys]->Add(ML_nBJets[j][i][isys]);
	ML_SFOSMll[j][ET_llll][isys]->Add(ML_SFOSMll[j][i][isys]);
	ML_SFOSMlll[j][ET_llll][isys]->Add(ML_SFOSMlll[j][i][isys]);
	ML_AllMll[j][ET_llll][isys]->Add(ML_AllMll[j][i][isys]);
	ML_AllMlll[j][ET_llll][isys]->Add(ML_AllMlll[j][i][isys]);
	ML_SFOSMZZ[j][ET_llll][isys]->Add(ML_SFOSMZZ[j][i][isys]);
	ML_SFOSMT[j][ET_llll][isys]->Add(ML_SFOSMT[j][i][isys]);
	ML_etmiss[j][ET_llll][isys]->Add(ML_etmiss[j][i][isys]);
	ML_metrel[j][ET_llll][isys]->Add(ML_metrel[j][i][isys]);
	ML_meff[j][ET_llll][isys]->Add(ML_meff[j][i][isys]);
	ML_metSig[j][ET_llll][isys]->Add(ML_metSig[j][i][isys]);
	ML_mct[j][ET_llll][isys]->Add(ML_mct[j][i][isys]);
	ML_mctPerp[j][ET_llll][isys]->Add(ML_mctPerp[j][i][isys]);
	ML_mt2[j][ET_llll][isys]->Add(ML_mt2[j][i][isys]);
	ML_mt2b[j][ET_llll][isys]->Add(ML_mt2b[j][i][isys]);
	ML_mct_0J[j][ET_llll][isys]->Add(ML_mct_0J[j][i][isys]);
	ML_mctPerp_0J[j][ET_llll][isys]->Add(ML_mctPerp_0J[j][i][isys]);
	ML_mt2_0J[j][ET_llll][isys]->Add(ML_mt2_0J[j][i][isys]);
	ML_mt2b_0J[j][ET_llll][isys]->Add(ML_mt2b_0J[j][i][isys]);
	ML_metRefEle[j][ET_llll][isys]->Add(ML_metRefEle[j][i][isys]);
	ML_metRefGam[j][ET_llll][isys]->Add(ML_metRefGam[j][i][isys]);
	ML_metRefMuo[j][ET_llll][isys]->Add(ML_metRefMuo[j][i][isys]);
	ML_metRefJet[j][ET_llll][isys]->Add(ML_metRefJet[j][i][isys]);
	ML_metRefSJet[j][ET_llll][isys]->Add(ML_metRefSJet[j][i][isys]);
	ML_metCellout[j][ET_llll][isys]->Add(ML_metCellout[j][i][isys]);
	ML_ptl1[j][ET_llll][isys]->Add(ML_ptl1[j][i][isys]);
	ML_ptl2[j][ET_llll][isys]->Add(ML_ptl2[j][i][isys]);
	ML_ptl3[j][ET_llll][isys]->Add(ML_ptl3[j][i][isys]);
	ML_ptl4[j][ET_llll][isys]->Add(ML_ptl4[j][i][isys]);
	ML_etal1[j][ET_llll][isys]->Add(ML_etal1[j][i][isys]);
	ML_etal2[j][ET_llll][isys]->Add(ML_etal2[j][i][isys]);
	ML_etal3[j][ET_llll][isys]->Add(ML_etal3[j][i][isys]);
	ML_etal4[j][ET_llll][isys]->Add(ML_etal4[j][i][isys]);
	ML_d0Sl1[j][ET_llll][isys]->Add(ML_d0Sl1[j][i][isys]);
	ML_d0Sl2[j][ET_llll][isys]->Add(ML_d0Sl2[j][i][isys]);
	ML_d0Sl3[j][ET_llll][isys]->Add(ML_d0Sl3[j][i][isys]);
	ML_d0Sl4[j][ET_llll][isys]->Add(ML_d0Sl4[j][i][isys]);
	ML_z0sinthetal1[j][ET_llll][isys]->Add(ML_z0sinthetal1[j][i][isys]);
	ML_z0sinthetal2[j][ET_llll][isys]->Add(ML_z0sinthetal2[j][i][isys]);
	ML_z0sinthetal3[j][ET_llll][isys]->Add(ML_z0sinthetal3[j][i][isys]);
	ML_z0sinthetal4[j][ET_llll][isys]->Add(ML_z0sinthetal4[j][i][isys]);
	ML_orgl1[j][ET_llll][isys]->Add(ML_orgl1[j][i][isys]);
	ML_orgl2[j][ET_llll][isys]->Add(ML_orgl2[j][i][isys]);
	ML_orgl3[j][ET_llll][isys]->Add(ML_orgl3[j][i][isys]);
	ML_orgl4[j][ET_llll][isys]->Add(ML_orgl4[j][i][isys]);
	ML_pTll[j][ET_llll][isys]->Add(ML_pTll[j][i][isys]);
	ML_dRll[j][ET_llll][isys]->Add(ML_dRll[j][i][isys]);
	ML_ptj1[j][ET_llll][isys]->Add(ML_ptj1[j][i][isys]);
	ML_ptj2[j][ET_llll][isys]->Add(ML_ptj2[j][i][isys]);
	ML_ptj3[j][ET_llll][isys]->Add(ML_ptj3[j][i][isys]);
	ML_ptj4[j][ET_llll][isys]->Add(ML_ptj4[j][i][isys]);
	ML_etaj1[j][ET_llll][isys]->Add(ML_etaj1[j][i][isys]);
	ML_etaj2[j][ET_llll][isys]->Add(ML_etaj2[j][i][isys]);
	ML_etaj3[j][ET_llll][isys]->Add(ML_etaj3[j][i][isys]);
	ML_etaj4[j][ET_llll][isys]->Add(ML_etaj4[j][i][isys]);
	ML_ptbj[j][ET_llll][isys]->Add(ML_ptbj[j][i][isys]);
	ML_etabj[j][ET_llll][isys]->Add(ML_etabj[j][i][isys]);
	ML_mjj[j][ET_llll][isys]->Add(ML_mjj[j][i][isys]);
      }
         

      
    }
  }


}
  
