#include "SusyWeakProdAna/Histos_3L.h"

/*--------------------------------------------------------------------------------*/
// SusyHistos Book3LHistograms
/*--------------------------------------------------------------------------------*/
void Histos_3L::Book3LHistograms(TDirectory* _hDir)
{
  std::cout << "Booking 3L histos " << std::endl;

  _hDir->cd();

  string    book_s1, book_s2,sx, sy, sz;
  char      syaxis[200];
  sprintf(syaxis,"Entries");


#define BOOK_SRML(hN,xT,u,yT, ...)					\
  for(int j=0; j<nHSR_ML; j++){						\
    if(string(u).length()>0) sx = string(xT+string(" [")+u+string("]")); \
    else sx = 	string(xT);						\
    string stype;							\
    if(j==0) stype="ML_SR3Lep_";					\
    if(j==1) stype="ML_SRB_";						\
    if(j==2) stype="ML_SR1a_";						\
    if(j==3) stype="ML_SR1b_";						\
    if(j==4) stype="ML_SR2_";						\
    if(j==5) stype="ML_VR0_";						\
    if(j==6) stype="ML_VR1_";						\
    if(j==7) stype="ML_VR2_";						\
    if(j==8) stype="ML_VR3_";						\
    if(j==9) stype="ML_VRWZ_";						\
    if(j==10) stype="ML_NRWZ_";						\
    if(j==11) stype="ML_SR4lep_";					\
    if(j==12) stype="ML_SR4lepNoZ_";					\
    hN[j] = _utils->myTH1F((book_s1=stype + #hN).c_str(),		\
			   (book_s2=stype + #hN).c_str(),		\
			   __VA_ARGS__,sx.c_str() ,yT);			\
    _utils->yAxis(hN[j],u);						\
    									\
  }
  
  BOOK_SRML(ML_pred,"","",syaxis,1,-0.5,0.5);
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
  BOOK_SRML(ML_SFOSMT,"M_{T}^{SFOS}","",syaxis,100,20,220);
  BOOK_SRML(ML_etmiss,"#slash{E}_{T}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_metRefEle,"#slash{E}_{T}^{RefEle}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_metRefMuo,"#slash{E}_{T}^{RefMuo}","GeV",syaxis,40,0,200);
  BOOK_SRML(ML_metRefJet,"#slash{E}_{T}^{RefJet}","GeV",syaxis,40,0,200);
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

  
#undef BOOK_SRML


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
  LepType.push_back("HF");
  LepType.push_back("LF");
  LepType.push_back("CONV");
  LepType.push_back("Unknown");


  for(uint i=0; i<nHSR_ML; i++){
    for(uint j=0; j<v3Lep.size(); j++)
      ML_evtCatgUnOrdered[i]->GetXaxis()->SetBinLabel(j+1,v3Lep.at(j).Data());
    for(uint j=0; j<v3LepOS.size(); j++)
      ML_evtCatgOSpair[i]->GetXaxis()->SetBinLabel(j+1,v3LepOS.at(j).Data());

    for(uint j=0; j<LepType.size(); j++){
	ML_orgl1[i]->GetXaxis()->SetBinLabel(j+1,LepType.at(j).Data());
	ML_orgl2[i]->GetXaxis()->SetBinLabel(j+1,LepType.at(j).Data());
	ML_orgl3[i]->GetXaxis()->SetBinLabel(j+1,LepType.at(j).Data());
	ML_orgl4[i]->GetXaxis()->SetBinLabel(j+1,LepType.at(j).Data());
    }
    
  }




}
