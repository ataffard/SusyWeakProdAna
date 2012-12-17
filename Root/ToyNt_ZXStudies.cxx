#include <iomanip>
#include <iostream>
#include <math.h>
#include "TCanvas.h"
#include "SusyWeakProdAna/ToyNt_ZXStudies.h"

using namespace std;
typedef unsigned uint;

/*--------------------------------------------------------------------------------*/
// ToyNt_ZXStudies Constructor
/*--------------------------------------------------------------------------------*/
ToyNt_ZXStudies::ToyNt_ZXStudies()
{

  _utils = new TGuiUtils();
}

/*--------------------------------------------------------------------------------*/
// The Begin() function is called at the start of the query.
/*--------------------------------------------------------------------------------*/
void ToyNt_ZXStudies::Begin(TTree* /*tree*/)
{
  ToyNtAna::Begin(0);
  bookHistograms(_histoDir);
}

/*--------------------------------------------------------------------------------*/
// Main process loop function 
/*--------------------------------------------------------------------------------*/
Bool_t ToyNt_ZXStudies::Process(Long64_t entry)
{
  GetEntry(entry);

  // Chain entry not the same as tree entry
  //static Long64_t chainEntry = -1;
  m_chainEntry++;

  Analyze();
  
  return kTRUE;
}

/*--------------------------------------------------------------------------------*/
// The Terminate() function is the last function to be called
/*--------------------------------------------------------------------------------*/
void ToyNt_ZXStudies::Terminate()
{
  saveHistograms(_histoDir);
  ToyNtAna::Terminate();
  
}
/*--------------------------------------------------------------------------------*/
// Book histos
/*--------------------------------------------------------------------------------*/
void ToyNt_ZXStudies::saveHistograms(TDirectory* hDir)
{
  string dir =  string(getenv("WORKAREA")) + "/histoAna" + "/ToyAna";
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
// Book histos
/*--------------------------------------------------------------------------------*/
void ToyNt_ZXStudies::bookHistograms(TDirectory* hDir)
{
  hDir->cd();
  vector<string> LEP;
  LEP.push_back("EE");
  LEP.push_back("MM");
  LEP.push_back("EM");

  vector<string> CUTS;
  CUTS.push_back("Inc");  //[0]
  
  CUTS.push_back("jVeto");        //DG2L jet veto [1]
  CUTS.push_back("jVetoPTll40");  //DG2L jet veto [2]
  CUTS.push_back("jVetoPTll80");  //DG2L jet veto [3]

  CUTS.push_back("j30NoJvf");              //jet veto pT>30 - no JVF [4]
  CUTS.push_back("j30NoJvfPTll40");        //jet veto pT>30 - no JVF [5]
  CUTS.push_back("j30NoJvfPTll80");        //jet veto pT>30 - no JVF [6]

  CUTS.push_back("j25Jvf05");       //HWW jet veto [7]
  CUTS.push_back("j25Jvf05PTll40"); //HWW jet veto [8]
  CUTS.push_back("j25Jvf05PTll80"); //HWW jet veto [9]

  CUTS.push_back("BjVeto");        //b-jet veto [10]
  CUTS.push_back("BjVetoPTll40");  //b-jet veto [11]
  CUTS.push_back("BjVetoPTll80");  //b-jet veto [12]


  for(uint ilep=0; ilep<LEP.size(); ilep++){
    for(uint i=0; i<CUTS.size(); i++){
      
      h_pTll[ilep][i] = _utils->myTH1F(string("pTll_"+LEP[ilep]+"_"+CUTS[i]).c_str(),
				 string("pTll_"+LEP[ilep]+"_"+CUTS[i]).c_str(),
				 50,0,200,
				 "p_{T}^{ll} [GeV]","Entries/bin");

      h_met[ilep][i] = _utils->myTH1F(string("met_"+LEP[ilep]+"_"+CUTS[i]).c_str(),
				 string("met_"+LEP[ilep]+"_"+CUTS[i]).c_str(),
				 50,0,200,
				 "#slash{E}_{T} [GeV]","Entries/bin");

      h_metrel[ilep][i] = _utils->myTH1F(string("metrel_"+LEP[ilep]+"_"+CUTS[i]).c_str(),
				 string("metrel_"+LEP[ilep]+"_"+CUTS[i]).c_str(),
				 50,0,200,
				 "#slash{E}_{T}^{rel} [GeV]","Entries/bin");


    }
    
  }

}


/*--------------------------------------------------------------------------------*/
// Analysis
/*--------------------------------------------------------------------------------*/
void ToyNt_ZXStudies::Analyze()
{
  int icut=0;
  
  //Inclusive
  h_pTll[llType][icut]->Fill(pTll,w);
  h_met[llType][icut]->Fill(met,w);
  h_metrel[llType][icut]->Fill(metrel,w);

  //DG2L std jet selection
  int nCJ=nCentralJ();
  int nBJ=nBJet();
  int nFJ=nFwdJ();
  if(nCJ-nCJets !=0) cout << "nC mismatch " << run << " " << event << endl;
  if(nBJ-nBJets !=0) cout << "nB mismatch " << run << " " << event << endl;
  if(nFJ-nFJets !=0) cout << "nF mismatch " << run << " " << event << endl;
    
  icut++;

  if((nCJ+nBJ+nFJ)==0){
    h_pTll[llType][icut]->Fill(pTll,w);
    h_met[llType][icut]->Fill(met,w);
    h_metrel[llType][icut]->Fill(metrel,w);
    if(pTll>40){
      h_pTll[llType][icut+1]->Fill(pTll,w);
      h_met[llType][icut+1]->Fill(met,w);
      h_metrel[llType][icut+1]->Fill(metrel,w);
    }
    if(pTll>80){
      h_pTll[llType][icut+2]->Fill(pTll,w);
      h_met[llType][icut+2]->Fill(met,w);
      h_metrel[llType][icut+2]->Fill(metrel,w);
    }
  }

  //J30 no JVF
  icut=4;
  nCJ=nCentralJ(30, -99, false, false);
  if(nCJ==0){
    h_pTll[llType][icut]->Fill(pTll,w);
    h_met[llType][icut]->Fill(met,w);
    h_metrel[llType][icut]->Fill(metrel,w);
    if(pTll>40){
      h_pTll[llType][icut+1]->Fill(pTll,w);
      h_met[llType][icut+1]->Fill(met,w);
      h_metrel[llType][icut+1]->Fill(metrel,w);
    }
    if(pTll>80){
      h_pTll[llType][icut+2]->Fill(pTll,w);
      h_met[llType][icut+2]->Fill(met,w);
      h_metrel[llType][icut+2]->Fill(metrel,w);
    }
  }

  //J25 JVF 50
  icut=7;
  nCJ=nCentralJ(25, 0.5, false, true);
  if(nCJ==0){
    h_pTll[llType][icut]->Fill(pTll,w);
    h_met[llType][icut]->Fill(met,w);
    h_metrel[llType][icut]->Fill(metrel,w);
    if(pTll>40){
      h_pTll[llType][icut+1]->Fill(pTll,w);
      h_met[llType][icut+1]->Fill(met,w);
      h_metrel[llType][icut+1]->Fill(metrel,w);
    }
    if(pTll>80){
      h_pTll[llType][icut+2]->Fill(pTll,w);
      h_met[llType][icut+2]->Fill(met,w);
      h_metrel[llType][icut+2]->Fill(metrel,w);
    }
  }
  

  //B-jet veto 
  icut=10;
  if(nBJ==0){
    h_pTll[llType][icut]->Fill(pTll,w);
    h_met[llType][icut]->Fill(met,w);
    h_metrel[llType][icut]->Fill(metrel,w);
    if(pTll>40){
      h_pTll[llType][icut+1]->Fill(pTll,w);
      h_met[llType][icut+1]->Fill(met,w);
      h_metrel[llType][icut+1]->Fill(metrel,w);
    }
    if(pTll>80){
      h_pTll[llType][icut+2]->Fill(pTll,w);
      h_met[llType][icut+2]->Fill(met,w);
      h_metrel[llType][icut+2]->Fill(metrel,w);
    }
  }



}

/*--------------------------------------------------------------------------------*/
// Jet couting
/*--------------------------------------------------------------------------------*/
int ToyNt_ZXStudies::nCentralJ(float pTmin, float jvf, 
			       bool notBTag, bool useAbs)
{
  const float MV1_85 = 0.122;

  int nCJ=0;
  for(int j=0; j<nJets; j++){
    if(j_pt[j]<pTmin) continue;
    if(fabs(j_eta[j])>2.5) continue;
    if(useAbs)
      if(fabs(j_jvf[j])<jvf) continue;
    else
      if(j_jvf[j]<jvf) continue;
    if(notBTag && j_mv1[j]>MV1_85) continue;

    nCJ++;
  }
  
  /*
 if(jet->Pt() < JET_PT_L25_CUT     )  return false;
  if(fabs(jet->Eta()) > JET_ETA_CUT )  return false;
  if(jet->jvf < JET_JVF_CUT_2L      )  return false;
  if(jet->mv1 > MV1_85              )  return false;
  */

  return nCJ;
}

int ToyNt_ZXStudies::nBJet(float pTmin)
{
  const float MV1_85 = 0.122;

  int nBJ=0;
  for(int j=0; j<nJets; j++){
    if(fabs(j_eta[j])>2.5) continue;
    if(j_pt[j]<pTmin) continue;
    if(j_mv1[j]<MV1_85) continue;
    nBJ++;
  }
  return nBJ;
}

int ToyNt_ZXStudies::nFwdJ(float pTmin)
{
  int nFJ=0;
  for(int j=0; j<nJets; j++){
    if(fabs(j_eta[j])<2.5) continue;
    if(fabs(j_eta[j])>4.5) continue;
    if(j_pt[j]<pTmin) continue;
    nFJ++;
  }
  return nFJ;
}
