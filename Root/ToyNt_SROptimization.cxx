#include "assert.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "SusyWeakProdAna/ToyNt_SROptimization.h"
#include "SusyNtuple/SusyDefs.h"

using namespace std;
typedef unsigned uint;

/*--------------------------------------------------------------------------------*/
// ToyNt_SROptimization Constructor
/*--------------------------------------------------------------------------------*/
ToyNt_SROptimization::ToyNt_SROptimization():
  nEvtProcess(0)
{
  _utils = new TGuiUtils();

  LEP.push_back("EE");
  LEP.push_back("MM");
  LEP.push_back("EM");
  LEP.push_back("ALL");

  memset(nEvtPass,0,sizeof(nEvtPass[0][0])*4*nCUT);
}

/*--------------------------------------------------------------------------------*/
// The Begin() function is called at the start of the query.
/*--------------------------------------------------------------------------------*/
void ToyNt_SROptimization::Begin(TTree* /*tree*/)
{
  ToyNtAna::Begin(0);
  bookHistograms(_histoDir);
}

/*--------------------------------------------------------------------------------*/
// Main process loop function 
/*--------------------------------------------------------------------------------*/
Bool_t ToyNt_SROptimization::Process(Long64_t entry)
{
  GetEntry(entry);

  // Chain entry not the same as tree entry
  //static Long64_t chainEntry = -1;
  m_chainEntry++;
  
  if(m_dbgEvt && !processThisEvent(run, event)) return kFALSE;


  Analyze();
  
  
  return kTRUE;
}

/*--------------------------------------------------------------------------------*/
// The Terminate() function is the last function to be called
/*--------------------------------------------------------------------------------*/
void ToyNt_SROptimization::Terminate()
{
  saveHistograms(_histoDir);
  PrintInfo();
  ToyNtAna::Terminate();
  
}
/*--------------------------------------------------------------------------------*/
// Book histos
/*--------------------------------------------------------------------------------*/
void ToyNt_SROptimization::saveHistograms(TDirectory* hDir)
{
  addHistograms();

  string dir =  string(getenv("HISTOANA")) + "/ToyAna/" + string(getenv("TOYNTSR"));
  gSystem->mkdir(dir.c_str(),kTRUE);
  string fileName  = dir + "/" + "histo_" + m_sample + ".root";
  
  string title     = m_sample + " histo file";
  TFile *histoFile = new TFile(fileName.c_str(),"RECREATE",title.c_str());

  TIter next(hDir->GetList()); //Use GetListOfKeys when reading back file
  TObject* obj=0; 
  while((obj=next())) { 
    //std::cout << obj->GetName() << std::endl; 
    if (!obj->InheritsFrom("TH1")) continue; //skip anything that is not TH1
    obj->Write(); 
  } 
  
  histoFile->Close();
  cout << "Histograms saved " << histoFile->GetName() << endl;
  delete histoFile;

}
/*--------------------------------------------------------------------------------*/
// Print info on event processed, selected
/*--------------------------------------------------------------------------------*/
void ToyNt_SROptimization::PrintInfo()
{

  cout << "--------------------------------------------" << endl;
  cout << " Number of event processed " << nEvtProcess << endl << endl;
  cout << " Cut # \t EE \t MM \t EM \t ALL " << endl;
  for(uint iCut=0; iCut<nCUT; iCut++)
    cout <<  std::setprecision(2) << std::fixed 
	 << iCut << "\t" << nEvtPass[0][iCut] << "\t" << nEvtPass[1][iCut] 
	 << "\t" << nEvtPass[2][iCut] << "\t" << nEvtPass[3][iCut] << endl;
  cout << "--------------------------------------------" << endl;

}
/*--------------------------------------------------------------------------------*/
// Book histos
/*--------------------------------------------------------------------------------*/
void ToyNt_SROptimization::bookHistograms(TDirectory* hDir)
{
  hDir->cd();

  CUTS.push_back("Base");     //OS -SF, MZ-10, veto F30, B20, nC20>=2
  CUTS.push_back("Sarah");    //OS -SF, MZ-10, veto F30, B20, nC20>=2,50<mjj<100, pTJ>50, topTag, metrel>50, mT2>70
  CUTS.push_back("Andreas");  //OS -SF, MZ-10, veto F30, B20, nC20>=2
  CUTS.push_back("AnyesSRZjetsOpt1");
  CUTS.push_back("AnyesSRZjetsOpt2");
  CUTS.push_back("AnyesSRZjetsOpt3");
  CUTS.push_back("AnyesSRZjetsOpt4");
  CUTS.push_back("AnyesSRSS_Opt1");
  CUTS.push_back("AnyesSRSS_WH_base");
  CUTS.push_back("AnyesSRSS_WH_MM_a");
  CUTS.push_back("AnyesSRSS_WH_MM_b");
  CUTS.push_back("AnyesSRSS_WH_MM_c");
  CUTS.push_back("AnyesSRSS_WH_MM_d");
  CUTS.push_back("AnyesSRSS_WH_EM_a");
  CUTS.push_back("AnyesSRSS_WH_EM_b");
  CUTS.push_back("AnyesSRSS_WH_EE_a");
  CUTS.push_back("AnyesSRSS_WH_EE_b");
    
  cout << "nCUTS " << CUTS.size() << endl;
  if(CUTS.size() != nCUT){
    cout << "Error booking histo nCUT differ !" << endl;
    assert(CUTS.size() != nCUT);
  }

  for(uint ilep=0; ilep<LEP.size(); ilep++){
    for(uint i=0; i<nCUT; i++){
      
      h_yield[ilep][i] = _utils->myTH1F(string("yield_"+LEP[ilep]+"_"+CUTS[i]).c_str(),
				 string("yield_"+LEP[ilep]+"_"+CUTS[i]).c_str(),
				 1,-0.5,0.5,
				 "NEvents","Entries/bin");

      h_metrel[ilep][i] = _utils->myTH1F(string("metrel_"+LEP[ilep]+"_"+CUTS[i]).c_str(),
				 string("metrel_"+LEP[ilep]+"_"+CUTS[i]).c_str(),
				 15,0,300,
				 "E_{T}^{miss,rel} [GeV]","Entries/bin");
    }
  }

  cout << "Done booking " << endl;

}

/*--------------------------------------------------------------------------------*/
// Book histos
/*--------------------------------------------------------------------------------*/
void ToyNt_SROptimization::addHistograms()
{
  uint NALL=LEP.size()-1;
  for(uint i=0; i<CUTS.size(); i++){
    for(uint ilep=0; ilep<NALL; ilep++){
      h_yield[NALL][i]->Add(h_yield[ilep][i]);
      h_metrel[NALL][i]->Add(h_metrel[ilep][i]);
    }
  }
}

/*--------------------------------------------------------------------------------*/
// Analysis
/*--------------------------------------------------------------------------------*/
void ToyNt_SROptimization::Analyze()
{
  for(uint i=0; i<nCUT; i++){
    if(passCut(i)) fillHistograms(i);
  }
  
  nEvtProcess++;
}

/*--------------------------------------------------------------------------------*/
// FillHistograms
/*--------------------------------------------------------------------------------*/
void ToyNt_SROptimization::fillHistograms(int icut)
{
  h_yield[llType][icut]->Fill(0.,w);
  h_metrel[llType][icut]->Fill(metrel,w);


  nEvtPass[llType][icut]+=w;
  nEvtPass[3][icut]+=w;
}

/*--------------------------------------------------------------------------------*/
// Cut selection
/*--------------------------------------------------------------------------------*/
bool ToyNt_SROptimization::passCut(int icut)
{
  if(icut==0) return true;
  
  if(icut==1){ //Sarah SRZjets
    if (j_pt[0]>50 && j_isC20[0] && j_pt[1]>50 && j_isC20[1] &&
	mjj>50 && mjj<100 &&
	topTag &&
	metrel>50 &&
	mT2>70){
      return true;
    }
    else return false;
  }   
  
  if(icut==2){ //Andreas SRZJets
    if( (llType==0 || llType==1) &&  
	fabs(mll-MZ)<10. &&
	nCJets>=2 && 
	(j_pt[0]>45 && j_isC20[0]) &&
        (j_pt[1]>45 && j_isC20[1]) &&
	metrel>80 && 
        mjj>50 && mjj<100 &&
	dR_ll>0.4 && dR_ll<1.5 &&
	pTll>80 
	){
      return true;
    }
    else return false;
  }

  if(icut==3){ //Anyes SRZjets Opt1
    if( (llType==0 || llType==1) &&  
	fabs(mll-MZ)<10. &&
	nCJets>=2 && 
	(j_pt[0]>30 && j_isC20[0]) &&
	(j_pt[1]>30 && j_isC20[1]) &&
	mWWT>200 && 
	mjj<100 &&
	mEff>300 &&
	dR_ll>0.4 && dR_ll<2.0 &&
	mctPerp>50 &&
	metrel>70
	//mT2>100 
	){
      return true;
    }
    else return false;
  }
  
  if(icut==4){ //Anyes SRZjets Opt2
    if( (llType==0 || llType==1) &&  
	fabs(mll-MZ)<10. &&
	nCJets>=2 && 
	(j_pt[0]>50 && j_isC20[0]) &&
	(j_pt[1]>50 && j_isC20[1]) &&
	mWWT>130 && 
	mjj>50 && mjj<100 &&
	mEff>250 &&
	dR_ll<1.5 &&
	mctPerp>50 &&
	metrel>70
	){
      return true;
    }
    else return false;
  }

  if(icut==5){ //Anyes SRZjets Opt3 (higher mass)
    if( (llType==0 || llType==1) &&  
	fabs(mll-MZ)<10. &&
	nCJets>=2 && 
	/*
	  (j_pt[0]>20 && j_isC20[0]) &&
	  (j_pt[1]>20 && j_isC20[1]) &&
	  mEff>300 &&
	  met/mEff>0.3 &&
	  mjj>50 && mjj<100 &&
	  metrel>80 &&
	  mWWT>130 && 
	  dR_ll<2.0 &&
	  mT2>100 //70
	*/
	(j_pt[0]>40 && j_isC20[0]) &&
	(j_pt[1]>25 && j_isC20[1]) &&
	mjj>50 && mjj<100 &&
	//dR_ll<2.0 &&
	mT2>100 &&
	//(met/mEff)>0.3 &&
	metrel>50 &&
	mEff>250
	){
      return true;
    }
    else return false;
  }
  
  if(icut==6){ //Anyes SRZjets Opt4 (low mass)
    if( (llType==0 || llType==1) &&  
	fabs(mll-MZ)<10. &&
	nCJets>=2 && 
	(j_pt[0]>40 && j_isC20[0]) &&
	(j_pt[1]>25 && j_isC20[1]) &&
	mjj>50 && mjj<=100. &&
	mT2<100. && mT2jj>100. && 
	(met/mEff)>0.2 &&
	metrel>20 && 
	ST>160        
	){
      return true;
    }
    else return false;
    /*
      (j_pt[0]>50 && j_isC20[0]) &&
      (j_pt[1]>25 && j_isC20[1]) &&
      met/mEff>0.2 &&
      mjj>50 && mjj<110 && 
      metrel>80 &&
      mctPerp>10 && 
      l_pt[0]>50 &&
      dR_ll<2.0 &&
      mT2<100
    */
  }
  
  if(icut==7){ //SS modeA_slep 
    if(llType==0 && 
       fabs(mll-MZ)>10. &&
       nCJets>=1  && nBJets==0 && nFJets==0 &&
       l_pt[0]>20 && l_pt[1]>20 &&
       metrel>40 &&
       pTll<40 &&
       mWWT>40 &&
       dphi_ll<2.5
       ){
      return true;
    }
    else if(llType==1 &&
	    nCJets>=0 && nBJets==0 && nFJets==0 &&
	    l_etcone30[1]/l_pt[1]<0.1 && 
	    l_etcone30[0]/l_pt[0]<0.1 &&
	    l_pt[0]>20 && l_pt[1]>15 &&
	    metrel>20 &&
	    mct<40 &&
	    mll<100 &&
	    pTll<20 //&&
	    //	    mWWT <40
	    //	    mTl[0]>30 && mTl[1]>30 &&
	    ){
      return true;
    }
    else if(llType==2 &&
	    nCJets>=1  && nBJets==0 && nFJets==0 &&
	    l_pt[0]>20 && l_pt[1]>20 &&
	    metrel>40 &&
	    pTll<40 &&
	    mll_collApprox>100
	    ){
      return true;
    }
    else return false;
  }
  
  if(icut==8){ //modeA WH -SS baseline
    if(llType==0 && 
       fabs(mll-MZ)>10. &&
       nCJets>=1  && nBJets==0 && nFJets==0 &&
       l_pt[0]>30 && l_pt[1]>20)
      return true;
    else if(llType==1 &&
	    l_etcone30[1]/l_pt[1]<0.1 && 
	    l_etcone30[0]/l_pt[0]<0.1 &&
	    l_pt[0]>30 && 
	    nCJets>=1 && nBJets==0 && nFJets==0)
      return true;
    else if (llType==2 &&
	     ( (l_etcone30[1]/l_pt[1]<0.1 && !l_isEle[1]) || 
	       (l_etcone30[0]/l_pt[0]<0.1 && !l_isEle[0]) ) &&
	     l_pt[0]>30 && l_pt[1]>20 &&
	     nCJets>=1  && nBJets==0 && nFJets==0) 
      return true;
    else return false;
  }
  

  if(icut>=9 && icut <=16){
    if(llType==0 && 
       fabs(mll-MZ)>10. &&
       nCJets>=1  && nBJets==0 && nFJets==0 &&
       l_pt[0]>30 && l_pt[1]>20)
      {
	if( mWWT>150 && metrel>50){
	  if(icut==15) return true;
	  else if(icut==16 && mT2>90) return true;
	}
      }
    
    else if(llType==1 &&
       l_etcone30[1]/l_pt[1]<0.1 && 
       l_etcone30[0]/l_pt[0]<0.1 &&
       l_pt[0]>30 && 
       nCJets>=1 && nBJets==0 && nFJets==0)
      {
	if( (mEff+l_pt[0]+l_pt[1])>200 && mWWT>100){
	  if(icut==9) return true;
	  else{
	    if(icut==10 && mWWT>150) return true;
	    else if(icut==11 && mWWT>200) return true;
	    else if(icut==12 && mWWT>200 && metrel>50) return true;
	    else return false;
	  }
	}
	else return false;
      }
    else if (llType==2 &&
	     ( (l_etcone30[1]/l_pt[1]<0.1 && !l_isEle[1]) || 
	       (l_etcone30[0]/l_pt[0]<0.1 && !l_isEle[0]) ) &&
	     l_pt[0]>30 && l_pt[1]>20 &&
	     nCJets>=1  && nBJets==0 && nFJets==0){
      if((mEff+l_pt[0]+l_pt[1])>200 && mWWT>140){
	if(icut==13) return true;
	else if(icut==14 && metrel>50) return true;
	else return false;
      }
      else return false;
    }
    else return false;
  }

  
  return false;

}

