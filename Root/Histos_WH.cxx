#include "SusyWeakProdAna/Histos_WH.h"
#include "TSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
/*--------------------------------------------------------------------------------*/
// SusyHistos BookWHHistograms
/*--------------------------------------------------------------------------------*/
void Histos_WH::BookWHHistograms(TDirectory* _hDir, bool useSys)
{
  std::cout << "Booking WH histos " << std::endl;
  std::cout << "Number of Signal regions " << WH_NSR << endl;

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

#define BOOK_SRDGWH(hN,xT,u,yT, ...)					\
  for(int i=0; i<3; i++){						\
    string sFlav;							\
    if(i==0) sFlav="EE_";						\
    if(i==1) sFlav="MM_";						\
    if(i==2) sFlav="EM_";						\
    for(int j=0; j<WH_NSR; j++){					\
      for(int isys=0; isys<maxSys; isys++){				\
	if(string(u).length()>0) sx = string(xT+string(" [")+u+string("]")); \
	else sx = 	string(xT);					\
	string stype = "DGWH_" + WH_SRNAME[j] + "_" + sFlav + #hN + "_" + DGSystNames[isys];	\
	hN[j][i][isys] = (TH1F*) _utils->myTH1F(stype.c_str(),		\
						stype.c_str(),		\
						__VA_ARGS__,sx.c_str() ,yT); \
	_utils->yAxis(hN[j][i][isys],u);				\
      }									\
    }									\
  }
  
  BOOK_SRDGWH(DGWH_pred,"","",syaxis,1,-0.5,0.5);
  BOOK_SRDGWH(DGWH_cutflow,"","",syaxis,30,-0.5,29.5);
  BOOK_SRDGWH(DGWH_nJets,"NJets","",syaxis,10,-0.5,9.5);
  BOOK_SRDGWH(DGWH_nCJets,"NJets - central","",syaxis,10,-0.5,9.5);
  BOOK_SRDGWH(DGWH_nFJets,"NJets - forward ","",syaxis,10,-0.5,9.5);
  BOOK_SRDGWH(DGWH_nBJets,"NBJets","",syaxis,5,-0.5,4.5);
  BOOK_SRDGWH(DGWH_nSoftJets,"NSoftJets","",syaxis,10,-0.5,9.5);
  BOOK_SRDGWH(DGWH_qq,"","",syaxis,10,-0.5,1.5);
  BOOK_SRDGWH(DGWH_mll,"m_{ll}","GeV",syaxis,130,20,280); 
  BOOK_SRDGWH(DGWH_mllShift,"m_{ll}","GeV",syaxis,130,20,280); 
  BOOK_SRDGWH(DGWH_mllcoarse,"m_{ll}","GeV",syaxis,52,20,280); 
  BOOK_SRDGWH(DGWH_mllcoarser,"m_{ll}","GeV",syaxis,26,20,280); 
  BOOK_SRDGWH(DGWH_mjj,"m_{jj}","GeV",syaxis,50,0,500);
  BOOK_SRDGWH(DGWH_pTll,"p_{T}^{ll}","GeV",syaxis,25,0,250);
  BOOK_SRDGWH(DGWH_mWWT,"m_{T}^{WW}","GeV",syaxis,30,0,300);
  BOOK_SRDGWH(DGWH_dPhill,"dPhi(l,l)","degree",syaxis,9,0,180);
  BOOK_SRDGWH(DGWH_dRll,"dR(l,l)","degree",syaxis,40,0,4);
  BOOK_SRDGWH(DGWH_dEtall,"d#eta(l,l)","degree",syaxis,30,0,3);
  BOOK_SRDGWH(DGWH_dPhilMet,"dPhi(l,Met)","degree",syaxis,9,0,180);
  BOOK_SRDGWH(DGWH_dPhiJetMet,"dPhi(j,Met)","degree",syaxis,9,0,180);
  BOOK_SRDGWH(DGWH_JZBJet,"JZB","GeV",syaxis,120,-400,800);
  BOOK_SRDGWH(DGWH_JZBEtmiss,"JZB","GeV",syaxis,120,-400,800);
  BOOK_SRDGWH(DGWH_etmiss,"#slash{E}_{T}","GeV",syaxis,40,0,200);
  BOOK_SRDGWH(DGWH_etmissPhi,"Phi_{#slash{E}_{T}}","degree",syaxis,18,-180,180);
  BOOK_SRDGWH(DGWH_metrel,"E_{T}^{miss,rel}","GeV",syaxis,20,0,200);
  BOOK_SRDGWH(DGWH_metrel1,"E_{T}^{miss,rel}","GeV",syaxis,13,40,300);
  BOOK_SRDGWH(DGWH_metrel2,"E_{T}^{miss,rel}","GeV",syaxis,26,40,300);
  BOOK_SRDGWH(DGWH_metrel3,"E_{T}^{miss,rel}","GeV",syaxis,18,40,400);
  BOOK_SRDGWH(DGWH_metRefEle,"#slash{E}_{T}^{RefEle}","GeV",syaxis,20,0,200);
  BOOK_SRDGWH(DGWH_metRefGam,"#slash{E}_{T}^{RefGamma}","GeV",syaxis,20,0,200);
  BOOK_SRDGWH(DGWH_metRefMuo,"#slash{E}_{T}^{RefMuo}","GeV",syaxis,20,0,200);
  BOOK_SRDGWH(DGWH_metRefJet,"#slash{E}_{T}^{RefJet}","GeV",syaxis,20,0,200);
  BOOK_SRDGWH(DGWH_metRefSJet,"#slash{E}_{T}^{RefSoftJet}","GeV",syaxis,20,0,200);
  BOOK_SRDGWH(DGWH_metCellout,"#slash{E}_{T}^{Cellout}","GeV",syaxis,20,0,200);
  BOOK_SRDGWH(DGWH_mt2,"m_{T2}","GeV",syaxis,20,0,200);
  BOOK_SRDGWH(DGWH_mt2b,"m_{T2}","GeV",syaxis,40,0,400);
  BOOK_SRDGWH(DGWH_mt2j,"m_{T2}^{J}","GeV",syaxis,40,0,400);
  BOOK_SRDGWH(DGWH_mlj,"m_{lj}","GeV",syaxis,40,0,400);
  BOOK_SRDGWH(DGWH_mljj,"m_{ljj}","GeV",syaxis,40,0,400);
  BOOK_SRDGWH(DGWH_mEff,"m_{eff}","GeV",syaxis,50,0,1000);
  BOOK_SRDGWH(DGWH_ST,"S_{T}","GeV",syaxis,50,0,1000);
  BOOK_SRDGWH(DGWH_MetSig,"E_{T}^{miss,signif}","",syaxis,40,0,20);
  BOOK_SRDGWH(DGWH_mTl1,"m_{T}^{l1}","GeV",syaxis,50,0,500);
  BOOK_SRDGWH(DGWH_mTl2,"m_{T}^{l2}","GeV",syaxis,50,0,500);
  BOOK_SRDGWH(DGWH_max_mT,"m_{T}^{max}","GeV",syaxis,50,0,500);
  BOOK_SRDGWH(DGWH_npv,"N_{pv}","",syaxis,30,-0.5,29.5);
  BOOK_SRDGWH(DGWH_mu,"<#mu>","",syaxis,40,-0.5,39.5);
  BOOK_SRDGWH(DGWH_ptl1,"p_{T}^{l1}","GeV",syaxis,40,0,200);
  BOOK_SRDGWH(DGWH_ptl2,"p_{T}^{l2}","GeV",syaxis,40,0,200);
  BOOK_SRDGWH(DGWH_etal1,"#eta^{l1}","",syaxis,25,-2.5,2.5);
  BOOK_SRDGWH(DGWH_etal2,"#eta^{l2}","",syaxis,25,-2.5,2.5);
  BOOK_SRDGWH(DGWH_ePt,"p_{T}^{e}","GeV",syaxis,40,0,200);
  BOOK_SRDGWH(DGWH_mPt,"p_{T}^{#mu}","GeV",syaxis,40,0,200);
  BOOK_SRDGWH(DGWH_eEta,"#eta^{e}","",syaxis,25,-2.5,2.5);
  BOOK_SRDGWH(DGWH_mEta,"#eta^{#mu}","",syaxis,25,-2.5,2.5);
  BOOK_SRDGWH(DGWH_d0Sl1,"d0/#sigma_{d0}^{l1}","",syaxis,100,-10,10);
  BOOK_SRDGWH(DGWH_d0Sl2,"d0/#sigma_{d0}^{l2}","",syaxis,100,-10,10);
  BOOK_SRDGWH(DGWH_z0sinthetal1,"z0 sin(#theta)^{l1}","cm",syaxis,200,-1,1);
  BOOK_SRDGWH(DGWH_z0sinthetal2,"z0 sin(#theta)^{l2}","cm",syaxis,200,-1,1);
  BOOK_SRDGWH(DGWH_orgl1,"MC Origin","",syaxis,5,-0.5,4.5);
  BOOK_SRDGWH(DGWH_orgl2,"MC Origin","",syaxis,5,-0.5,4.5);
  BOOK_SRDGWH(DGWH_ptj1,"p_{T}^{j1}","GeV",syaxis,40,0,200);
  BOOK_SRDGWH(DGWH_ptj2,"p_{T}^{j2}","GeV",syaxis,40,0,200);
  BOOK_SRDGWH(DGWH_ptj3,"p_{T}^{j3}","GeV",syaxis,40,0,200);
  BOOK_SRDGWH(DGWH_ptj4,"p_{T}^{j4}","GeV",syaxis,40,0,200);
  BOOK_SRDGWH(DGWH_etaj1,"#eta^{j1}","",syaxis,25,-2.5,2.5);
  BOOK_SRDGWH(DGWH_etaj2,"#eta^{j2}","",syaxis,25,-2.5,2.5);
  BOOK_SRDGWH(DGWH_etaj3,"#eta^{j3}","",syaxis,25,-2.5,2.5);
  BOOK_SRDGWH(DGWH_etaj4,"#eta^{j4}","",syaxis,25,-2.5,2.5);
  BOOK_SRDGWH(DGWH_jvfj1,"JVF^{j1}","",syaxis,10,0,1);
  BOOK_SRDGWH(DGWH_jvfj2,"JVF^{j2}","",syaxis,10,0,1);
  BOOK_SRDGWH(DGWH_ptbj,"p_{T}^{bjet}","GeV",syaxis,40,0,200);
  BOOK_SRDGWH(DGWH_etabj,"#eta^{bjet}","",syaxis,25,-2.5,2.5);
  BOOK_SRDGWH(DGWH_jvfbj,"JVF^{j1}","",syaxis,10,0,1);
  BOOK_SRDGWH(DGWH_ptSj1,"p_{T}^{j1-soft}","GeV",syaxis,40,0,200);
  BOOK_SRDGWH(DGWH_ptSj2,"p_{T}^{j2-soft}","GeV",syaxis,40,0,200);
  BOOK_SRDGWH(DGWH_etaSj1,"#eta^{j1-soft}","",syaxis,25,-2.5,2.5);
  BOOK_SRDGWH(DGWH_etaSj2,"#eta^{j2-soft}","",syaxis,25,-2.5,2.5);
  BOOK_SRDGWH(DGWH_jvfSj1,"JVF^{j1-soft}","",syaxis,10,0,1);
  BOOK_SRDGWH(DGWH_jvfSj2,"JVF^{j2-soft}","",syaxis,10,0,1);


#undef BOOK_SRDGWH
  
  std::vector<TString> LepType;
  LepType.clear();
  LepType.push_back("PR");
  LepType.push_back("CONV");
  LepType.push_back("HF");
  LepType.push_back("LF");
  LepType.push_back("Unknown");
 
  for(uint i=0; i<WH_NSR; i++){
    for(int ilep=0; ilep<3; ilep++){			
      for(uint j=0; j<LepType.size(); j++){
	for(int isys=0; isys<maxSys; isys++){				
	  DGWH_orgl1[i][ilep][isys]->GetXaxis()->SetBinLabel(j+1,LepType.at(j).Data());
	  DGWH_orgl2[i][ilep][isys]->GetXaxis()->SetBinLabel(j+1,LepType.at(j).Data());
	}
      }
    }
  }
  
  



}

/*--------------------------------------------------------------------------------*/
// SusyHistos SaveHistograms
// Split ouput file as 
/*--------------------------------------------------------------------------------*/
void Histos_WH::SaveSplitWHHistograms(TDirectory* _hDir, string sample,
				       int method,
				       bool mcMll, bool isZAlpgenSherpa, 
				       string sys1, string sys2)
{
  _hDir->cd(); //running dir where all the histos are cached

  string baseDir =  string(getenv("HISTOANA")) + "/SusyAna/histOutputs";
  gSystem->mkdir(baseDir.c_str(),kTRUE);

  TFile *histoFile[WH_NSR][3];
  std::map<string, std::pair< int,int> > _label;  //eg: CR2LepOS_EE

  //Put  each SR/Lep combo in a dir otherwise too many files to ls
  for(int ilep=0; ilep<3; ilep++){	
    for(int isr=0; isr<WH_NSR; isr++){ //SR/CR's loop
      string chanSR =  string(WH_SRNAME[isr]) + "_" + string(WH_FLAV[ilep]);
      //cout << "Label " << chanSR << endl;
      _label.insert(_label.end(),std::make_pair(string(chanSR), std::make_pair(ilep,isr)));
      string dir =  baseDir + "/" + chanSR; 
      gSystem->mkdir(dir.c_str(),kTRUE);

      string fileName  = dir + "/" + "histo_" + sample;

      if(isZAlpgenSherpa && mcMll){
	fileName = fileName + "_mll60";
      }

      chanSR =  "_" + chanSR;
      fileName = fileName + chanSR;

      fileName = fileName + "_" + SMETHOD[method];
      if(strlen(sys1.c_str())>0) fileName = fileName + "_" + sys1;
      if(strlen(sys2.c_str())>0) fileName = fileName + "_" + sys2;
      
      fileName = fileName +".root";
      //cout << "File name " << fileName << endl;
      string title     = sample + "_" + chanSR + " histo file";      
      histoFile[isr][ilep] = new TFile(fileName.c_str(),"RECREATE",title.c_str());
    }
  }

  //Looping over the histo and saving them in there appropriate files

  TIter next(_hDir->GetList()); //Use GetListOfKeys when reading back file
  TObject* obj=0; 
  while((obj=next())) { 
    if (!obj->InheritsFrom("TH1")) continue; //skip anything that is not TH1

    string _name =  string(obj->GetName());
    string _name2 = _name.substr(5); // DGWH_
    uint idx1 = _name2.find("_"); //idx of 1st _
    uint idx2 = _name2.find("_",idx1+1); //idx of 2nd _
    uint idx3 = _name2.find("_",idx2+1); //idx of 2nd _
    string _subName = _name2.substr(0,idx3);
    //cout << "histName " << _name <<  "\t histo name extracted " << _subName << endl;
      
    std::map<string, std::pair< int,int> >::const_iterator it = _label.find(_subName);
    if( it != _label.end()){
      int ilep = it->second.first;
      int isr = it->second.second;
      histoFile[isr][ilep]->cd();
      obj->Write();
    }
    else cout << "Error cannot find the file to associate " << _subName << endl;

  } 

  //Closing all the files
  for(int ilep=0; ilep<3; ilep++){	
    int count = 0;
    for(int isr=0; isr<WH_NSR; isr++){ //SR/CR's loop
      count++;
      histoFile[isr][ilep]->Close();
      delete histoFile[isr][ilep];
    }
    cout << "Histograms for " << WH_FLAV[ilep] << " "  << count << " files saved " << endl;
  }
  
}
