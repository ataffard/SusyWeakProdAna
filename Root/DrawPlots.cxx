#include "SusyWeakProdAna/DrawPlots.h"
#include "SusyWeakProdAna/SusyAnaCommon.h"


#include "TMath.h"

typedef unsigned uint;
//-----------------------------------------------------------------------------
DrawPlots::DrawPlots(){

  _pathHisto  =  string(getenv("HISTOANA")) + "/SusyAna/histos_" + DATE;
  _pathPlots  =  _pathHisto + "/Plots";
  _pathTables =  _pathHisto + "/Tables";

  gSystem->Setenv("HISTOAREA",_pathHisto.c_str());
  gSystem->mkdir(_pathPlots.c_str(),kTRUE);
  gSystem->mkdir(_pathTables.c_str(),kTRUE);

  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  _utils->atlasStyle->SetOptStat("emr");
  _utils->atlasStyle->SetOptStat("");

  _logy=false;					
  _moveUO=true;
  disableFake = false;

  _mcStack = new THStack("tmpStack","tmpStack");

  SFILE.clear();
  //  SFILE.push_back("Wjets + b#bar{b} + c#bar{c}");
  SFILE.push_back("Higgs");
  SFILE.push_back("Fake leptons MC");
  SFILE.push_back("ZV");
  SFILE.push_back("Z+jets");
  SFILE.push_back("t#bar{t} + Wt");
  SFILE.push_back("WW");
  SFILE.push_back("Data");


  SIGFILE.clear();
  //WH
  SIGFILE.push_back("(m#tilde{#chi}^{#pm}_{1},m#tilde{#chi}^{0}_{2}) = (130,0) GeV");
  //C1C1-slepton
  //  SIGFILE.push_back("(m#tilde{l},m#tilde{#chi}_{1}^{0}) = (251,10) GeV");
  //  SIGFILE.push_back("(m#tilde{#chi}^{#pm}_{1},m#tilde{#chi}^{0}_{1}) = (350,0) GeV");
}

//-------------------------------------------//
// Open histo files
//-------------------------------------------//
void DrawPlots::openHistoFiles(string mode, 
			       string sTop, string sWW, 
			       string sZjets,  string sZV, 
			       string sFake, string sHiggs)
{
  std::cout << "loading histo from method " << mode << endl;
  std::cout << "Using histo path " << _pathHisto <<endl;

  _dataFileName = _pathHisto + "/histo_data12_std.root";
  //_dataFileName = _pathHisto + "/histo_dummy_rlep.root";
  std::cout << "Loading data " << std::endl;
  _dataFile = new TFile(_dataFileName.c_str(),"READ",SFILE[5].c_str());

  _mcFileName.clear();
  _mcFile.clear();
  
  string method;
  if(strcmp(mode.c_str(),"STD")==0){
    cout << "Loading STD MC mode " << endl;
    method="std";
    _mcFileName.push_back(string(sHiggs + "_" + method + ".root").c_str());
    _mcFileName.push_back(string(sFake + "_" + method + ".root").c_str());
    _mcFileName.push_back(string(sZV + "_" + method + ".root").c_str());
    _mcFileName.push_back(string(sZjets + "_" + method + ".root").c_str());
    _mcFileName.push_back(string(sTop + "_" + method + ".root").c_str());
    _mcFileName.push_back(string(sWW + "_" + method + ".root").c_str());

  }
  else if(strcmp(mode.c_str(),"DD")==0){
    cout << "Loading rlep MC & DD fake " << endl;
    //real match on
    method="rlep";
    //Some overlap but - top ->semi lep not included 
    //method="std";
    _mcFileName.push_back(string(sHiggs + "_" + method + ".root").c_str());
    _mcFileName.push_back(string("histo_data12_flep.root").c_str());
    _mcFileName.push_back(string(sZV + "_" + method + ".root").c_str());
    _mcFileName.push_back(string(sZjets + "_" + method + ".root").c_str());
    _mcFileName.push_back(string(sTop + "_" + method + ".root").c_str());
    _mcFileName.push_back(string(sWW + "_" + method + ".root").c_str());

    SFILE[1]="Fake leptons";
  }

  for(uint i=0; i<_mcFileName.size(); i++){
    std::cout << "Loading " << SFILE[i] << " " << _mcFileName[i].c_str() << std::endl;
    string fName = _pathHisto + "/" + _mcFileName[i];
    TFile* _f;
    _f = new TFile(fName.c_str(),"READ",SFILE[i].c_str());
    if(_f->IsOpen()==kFALSE){
      _f->Clear();
      _f->Open(string(_pathHisto+"/"+"histo_dummy_std.root").c_str(), "READ", SFILE[i].c_str());
      cout << "==> opening dummy file instead" << endl;
      _f->ls();
    }
    _mcFile.push_back(_f);
  }

  //Set the bkg color scheme & markers
  _mcColor.clear();
  _mcMarker.clear();
  for(uint i=0; i<_mcFile.size(); i++){
    
    switch (i){
    case HIGGS:
      _mcColor.push_back(C_HIGGS);
      break;
    case FAKE:
      _mcColor.push_back(C_FAKE);
      break;
    case ZV:
      _mcColor.push_back(C_ZV);
      break;
    case WW:
      _mcColor.push_back(C_WW);
      break;
    case TOP:
      _mcColor.push_back(C_TOP);
      break;
    case Zjets:
      _mcColor.push_back(C_Zjets);
      break;
    }
    _mcMarker.push_back(iMarker[i+1]);
  }


  _sigFileName.clear();
  _sigFile.clear();
  //C1C1 - slepton
  //  _sigFileName.push_back("histo_Herwigpp_pMSSM_DLiSlep_MSL_251_M1_010.175510_rlep_NOM_XS_DN.root");
  //  _sigFileName.push_back("histo_Herwigpp_simplifiedModel_wC_slep_noWcascade_20.144917_rlep_NOM_XS_DN.root");
  _sigFileName.push_back("histo_Herwigpp_sM_wA_noslep_notauhad_WH_2Lep_1.177501_rlep.root");
  
  for(uint i=0; i<_sigFileName.size(); i++){
    std::cout << "Loading " << SIGFILE[i].c_str() << " " << _sigFileName[i] << std::endl;
    string fName =  _pathHisto + "/" + _sigFileName[i];
    TFile* _f = new TFile(fName.c_str(),"READ",SIGFILE[i].c_str());
    if(_f->IsOpen()==kFALSE){
      _f->Open(string(_pathHisto+"/"+"histo_dummy_std.root").c_str(), "READ", SFILE[i].c_str());
      cout << "==> opening dummy file instead" << endl;
    }
    _sigFile.push_back(_f);   
  }


}
//-------------------------------------------//
// Grab TH1 histo from files
//-------------------------------------------//
void DrawPlots::grabHisto(string name, bool quiet, bool sysHistos)
{
  TString HNAME(name);

  for(uint i=0; i<_mcH1.size(); i++){
    for(uint j=0; j<_mcH1[i].size(); j++){
      if(_mcH1[i][j]!=NULL) _mcH1[i][j]->Clear();
    }
  }
  _mcH1.clear();
  _mcH1.reserve(OTHER);

  //if(_dataH1!=NULL) _dataH1->Clear();

  string title;
  TH1F* _h;
  TH1F* _tmp;

  _dataFile->cd();
  string dname =  name + "_NOM"; 
  title = "DATA_"+dname;
  _h = (TH1F*) _dataFile->Get(dname.c_str());
  
  if(_h==NULL){
    cerr <<" Could not find histo " << dname << " in data file " << _dataFile->GetName() << endl;
    abort();
  }
  if(HIDEDATA) blindMT2(name,_h);

  if(_moveUO) _utils->moveUnderOverFlow(_h);
  _dataH1 = (TH1F*) _h->Clone(title.c_str());
  if(HNAME.Contains("_metrel")) {
    _dataH1->GetXaxis()->SetTitle("E_{T}^{miss,rel} [GeV]");
  }
  _utils->yAxis(_dataH1,"GeV");		

  if(HIDEDATA) blindDataSR();

  if(!quiet) cout << "Got " << _dataH1->GetName() << endl;

  //Used in case MC file not there
  _tmp = (TH1F*) _dataH1->Clone();
  _tmp->Reset();

  int nLoad=0;
  for(uint i=0; i<_mcFile.size(); i++){
     
    float SF = 1;
    if(USESF) SF = getBkgSF(name,i);

    TFile* _f = _mcFile[i];
    _f->cd();
    
    vector<TH1F*> _hArray;
    TH1F* _hNom=NULL;
    for(int isys=0; isys<DGSys_N; isys++){
      if(!sysHistos && isys>0) continue;
      string hName = name + "_" + DGSystNames[isys];
      _h = (TH1F*) _f->Get(hName.c_str());
      if(HNAME.Contains("_metrel")) {
	_h->GetXaxis()->SetTitle("E_{T}^{miss,rel} [GeV]");
      }
      _utils->yAxis(_h,"GeV");			

      if(USESF)	_h->Scale(SF);
      if(isys==DGSys_NOM) _hNom = (TH1F*) _h->Clone();
      //setGenSys(hName,i, _hNom, _h); //2L hard coded sys !!!!

      _hArray.push_back(_h);
      if(_hArray[isys]==NULL){ //put an empty histo
	cerr <<" Could not find histo " << hName << " in MC file " << _f->GetName() << " using empty histo" << endl;
	_hArray[isys] = (TH1F*) _tmp->Clone();
      }
      if(_moveUO) _utils->moveUnderOverFlow(_hArray[isys]);

      title =MCNames[i] + "_" + hName;
      _hArray[isys]->SetTitle(title.c_str());
      _hArray[isys]->SetName(title.c_str());
      if(!quiet && isys==DGSys_NOM) std::cout << "MC " << _hArray[isys]->GetName() << " entries " << _hArray[isys]->Integral(0,-1) <<std::endl;      
    }

    _mcH1.push_back(_hArray);
    nLoad++;
  }
   

  for(uint i=0; i<_sigFile.size(); i++){
    TFile* _f = _sigFile[i];
    _f->cd();
    string hName = name + "_NOM";
    _h = (TH1F*) _f->Get(hName.c_str());
    //if(HNAME.Contains("_metrel")) _h->Rebin(2);
    if(_h==NULL){ //put an empty histo
      cerr <<" Could not find histo " << name << " in MC file " << _f->GetName() << " using empty histo" << endl;
      _h = (TH1F*) _tmp->Clone();
    }
    if(_moveUO) _utils->moveUnderOverFlow(_h);
    /*
    switch (i){
    case Slepton_255_80:
      _sigColor.push_back(C_SIG1);
      _sigName.push_back(SIGFILE[i]);
      title = "SIG_"+name;
      break;
    case modeCSlep_375_125:
      _sigColor.push_back(C_SIG2);
      _sigName.push_back(SIGFILE[i]);
      title = "SIG_"+name;
      break;
    }
    */
    _sigColor.push_back(C_SIG1);
    _sigName.push_back(SIGFILE[0]);
    title = "SIG_"+name;

    _h->SetTitle(title.c_str());
    _h->SetName(title.c_str());
    _h->SetFillStyle(0);
    _h->SetLineStyle(7);
    _h->SetLineWidth(2);
    if(!quiet)
      std::cout << "SIG " << _h->GetName() 
		<< " " << _h->Integral(0,-1) <<std::endl;
    _sigH1.push_back(_h);
    nLoad++;
  }

  if(!quiet) cout << "Got all histos " << endl<<endl;

}
//-------------------------------------------//
// Open histo files
//-------------------------------------------//
void DrawPlots::blindDataSR(){

  TString hName(_dataH1->GetName());
  
  if((
      hName.Contains("SROSjveto")||
      hName.Contains("SRmT2a") ||
      hName.Contains("SRmT2b") ||
      hName.Contains("SR2jets") ||
      hName.Contains("SRZjets") ||
      hName.Contains("SRSSjets") ||
      hName.Contains("SRWWa") ||
      hName.Contains("SRWWb") ||
      hName.Contains("SRWWc") 
      ) && hName.Contains("preSR")==kFALSE
     ) {
    _dataH1->Reset();
  }
 
}
//-------------------------------------------//
// Bling mT2 >90
//-------------------------------------------//
void DrawPlots::blindMT2(string name, TH1F* h){
  TString hName(name);
  if(hName.Contains("_mt2")){
    int lowMt2 = h->FindBin(90);
    for(int bin=lowMt2; bin<=h->GetNbinsX()+1; ++bin){
      h->SetBinContent(bin,0);
      h->SetBinError(bin,0);
    }
  }
}
//-------------------------------------------//
// Scale Zjets, Top, WW to their values
//-------------------------------------------//
float DrawPlots::getBkgSF(string name, int bkgType){
  TString hName(name);
  
  //SR & preSR
  if( hName.Contains("_SROSjveto_")||
      hName.Contains("_SRmT2a_") ||
      hName.Contains("_SRmT2b_") ||
      hName.Contains("_preSRmT2_") ||
      hName.Contains("_SR2jets_") ||
      hName.Contains("_SRZjets_") ||
      hName.Contains("_SRSSjets_")
      ){

    if(bkgType==TOP){ //updated 040213
      if(hName.Contains("EE"))       return 1;//1.02;//1.037; 
      else if(hName.Contains("MM"))  return 1;//1.02;//1.011; 
      else if(hName.Contains("EM"))  return 1;//1.02;//1.046; 
    }
    else if(bkgType==WW){  //updated 031513
      if(hName.Contains("EE"))       return 1;//1.13;//1.1;
      else if(hName.Contains("MM"))  return 1;//1.13;//1.0;
      else if(hName.Contains("EM"))  return 1;//1.13;//1.0;
    }
    else if(bkgType==Zjets){ //updated 031513     
      if(hName.Contains("EE"))       return 1;// ??
      else if(hName.Contains("MM"))  return 1;// ??
      else if(hName.Contains("EM"))  return 1;//1.14;// Ztt SF was 1.8
    }
    else if(bkgType==ZV){//Updated 040213
      if(hName.Contains("EE")) return 1;//1.062;//1.0756;
      if(hName.Contains("MM")) return 1;//1.062;//1.1594;
    }
  }

  return 1;
}

//-------------------------------------------//
// Set Generator sys
//-------------------------------------------//
void  DrawPlots::setGenSys(string name, int bkgType, TH1F* hNom, TH1F* h){
  TString hName(name);
  if ( !( hName.Contains("GEN_UP") || hName.Contains("GEN_DN") )) return;
  if ( !( hName.Contains("_preSRmT2_") || hName.Contains("_SRmT2a_") || hName.Contains("_SRmT2b_") )){
    h->Add(hNom,1);//Set Gen Sys histo to nominal value.
    return;
  }
  

  if(hNom==NULL ) {
    cout << "GenSys hNom NULL " << endl;
    return;
  }
  h->Add(hNom,1);//Set Gen Sys histo to nominal value.

  float sys=0;
  if ( bkgType == WW){
    //cout << "Bkg " << MCNames[bkgType]  << " " << name << " " <<  h->Integral(0,-1) << endl;
    if (hName.Contains("EE")) sys=0;//0.08;//0.08;
    if (hName.Contains("MM")) sys=0;//0.08;//0.09;
    if (hName.Contains("EM")) sys=0;//0.08;//0.07;
  }
  if ( bkgType == ZV){
    if (hName.Contains("EE")) sys=0;//0.36;//0.66;//0.32;
    if (hName.Contains("MM")) sys=0;//0.36;//0.34;//0.32;
    if (hName.Contains("EM")) sys=0;//0.36;//0.19;//0.18;
  }
  if(bkgType==TOP){ //updated 040713
    if (hName.Contains("EE")) sys=0;//0.25;
    if (hName.Contains("MM")) sys=0;//0.25;
    if (hName.Contains("EM")) sys=0;//0.25;
  }

  if (hName.Contains("UP")) h->Scale(1+sys);
  if (hName.Contains("DN")) h->Scale(1-sys);
  
  //  cout << "\t after scaling " << h->Integral(0,-1) << endl;

}

//-------------------------------------------//
// Open histo files
//-------------------------------------------//
void DrawPlots::buildStack(string name, TLegend* _l)
{
  setStackOrder(name);
  
  _mcStackH = (TH1F*) _mcH1[0][DGSys_NOM]->Clone();
  _mcStackH->Reset();
  _mcStackH->SetTitle(string("Stack_"+name).c_str());
  _mcStackH->SetName(string("Stack_"+name).c_str());

  _mcStack->Clear();
  _mcStack->SetName(name.c_str());
  _mcStack->SetTitle(name.c_str());

  //for(uint i=0; i<_mcH1.size(); i++){
  for(uint i=0; i<_stackOrder.size(); i++){
    int iSample = _stackOrder[i];
    std::cout << _mcH1[iSample][DGSys_NOM]->GetName() << " \t\t Int " 
    	      << _mcH1[iSample][DGSys_NOM]->Integral(0,-1) << std::endl;
    
    _utils->addToTHStack(_mcStack,_mcH1[iSample][DGSys_NOM],_mcColor[iSample], 
			 "HIST", _l, SFILE[iSample].c_str());
    _mcStackH->Add(_mcH1[iSample][DGSys_NOM],1);

    
  }

}

//-------------------------------------------//
// Open histo files
//-------------------------------------------//
void DrawPlots::setStackOrder(string name)
{
  TString hName(name);
  _stackOrder.clear();

  if( hName.Contains("_CRTOP_")){
    _stackOrder.push_back(HIGGS);
    _stackOrder.push_back(ZV);
    _stackOrder.push_back(Zjets);
    _stackOrder.push_back(WW);
    _stackOrder.push_back(FAKE);
    _stackOrder.push_back(TOP);
  }
  else{
    _stackOrder.push_back(HIGGS);
    _stackOrder.push_back(FAKE);
    _stackOrder.push_back(ZV);
    _stackOrder.push_back(Zjets);
    _stackOrder.push_back(TOP);
    _stackOrder.push_back(WW);
  }
  
}

//-------------------------------------------//
// Retrieve TH1 histo from files
//-------------------------------------------//
TObject* DrawPlots::getHisto(string sample, string name, bool moveUnderOver)
{
  string _fileName = _pathHisto + "/histo_" + sample + ".root";
  //std::cout << "Loading " << _fileName.c_str() << std::endl;
  TFile* _f = new TFile(_fileName.c_str(),"READ",sample.c_str());
 
  TObject* h = _f->Get(name.c_str());
  if(h==NULL){
    std::cout << "ERROR cannot find histo " << name.c_str() << " - using empty histo "  << endl;
    //
    //Grad the empty one 
    //
    _fileName = _pathHisto + "/histo_dummy.root";
    _f = new TFile(_fileName.c_str(),"READ",sample.c_str());
    TObject* h = _f->Get(name.c_str())->Clone();
    if(moveUnderOver) _utils->moveUnderOverFlow((TH1F*) h);
    return h;
  }

  if(moveUnderOver) _utils->moveUnderOverFlow((TH1F*) h);
  return h;
} 

//-------------------------------------------//
// Retrieve TH1 histo from files
//-------------------------------------------//
TObject* DrawPlots::retreiveHisto(int iSample, string name, bool moveUnderOver)
{
  _mcFile[iSample]->cd();
  TObject* _h = (TH1F*) _mcFile[iSample]->Get(name.c_str());
  
  if(_h==NULL){
    std::cout << "ERROR cannot find histo " << name.c_str() << " - using empty histo "  << endl;
    string _fileName = _pathHisto + "/histo_dummy.root";
    TFile* _f = new TFile(_fileName.c_str(),"READ",SFILE[iSample].c_str());
    _h = _f->Get(name.c_str())->Clone();
  }
  
  if(moveUnderOver) _utils->moveUnderOverFlow((TH1F*) _h);
  return _h;
}


//-------------------------------------------//
// Compare shape for a given histo
// Histos are normalized locally
//-------------------------------------------//
void DrawPlots::compareShape(string name, bool logy){
  bool norm=true;
  const float maxScaleLin=1.4;
  const float maxScaleLog=11;
  float scale=maxScaleLin;

  scale = maxScaleLin;
  string _sLogy = "";
  setLogy(logy);
  if(logy){
    _sLogy = "_logy";
    scale=maxScaleLog;
  }

  grabHisto(name);

  //Copy & normalize all the histo to 1
  TH1F*           _tmp_dataH1;
  vector<TH1F*>   _tmp_mcH1;
  TString pName(name);
  float yMax=0;

  //We don't what to normalise some histos
  if(pName.Contains("_reff_") || pName.Contains("_fr_")){
    std::cout << "Normalisation turned-off " << std::endl;
    norm=false;
  }

  TLegend* _leg = new TLegend(0.6,0.5,0.75,0.85);
  _utils->legendSetting(_leg); 

  _tmp_dataH1 = (TH1F*) _dataH1->Clone();
  if(_tmp_dataH1==NULL) {
    cerr << "Cannot find data histo " << endl;
    abort();
  }
  if(norm) _utils->normalize(_tmp_dataH1,1);
  _leg->AddEntry(_tmp_dataH1,"Data" ,"p");
  yMax=_tmp_dataH1->GetMaximum();

  //Normalize the bkg
  //Fix this - what to compare the normalized sum of the bkg !
  for(uint i=0; i<_mcH1.size(); i++){
    TH1F* _tmpH1 = (TH1F*) _mcH1[i][DGSys_NOM]->Clone();
    if(_tmpH1==NULL) {
      cerr << "Cannot find MC histo " << i << endl;
      abort();
    }
    if(norm) _utils->normalize(_tmpH1,1);
    _tmp_mcH1.push_back(_tmpH1);
    _leg->AddEntry(_tmpH1,MCNames[i].c_str(),"p");

    if(_tmpH1->GetMaximum()>yMax) yMax=_tmpH1->GetMaximum();
  }
   
  TCanvas* _c0  = _utils->myCanvas(("c_"+name).c_str());
  string mcOpt="same";
  bool noData=true;
  if(_tmp_dataH1->Integral(0,-1)>0){
    _tmp_dataH1->SetMaximum(yMax*scale);
    _utils->myDraw1d(_tmp_dataH1,_c0,1,"e",_logy,kBlack,false,iMarker[0]);
    mcOpt="lsame";
    noData=false;
  }
  for(uint i=0; i<_mcH1.size(); i++){
    _tmp_mcH1[i]->SetMaximum(yMax*scale);
    if(noData && i==0)
      _utils->myDraw1d(_tmp_mcH1[i],_c0,1,"e",_logy,_mcColor[i],false,_mcMarker[i]);
    else
      _utils->myDraw1d(_tmp_mcH1[i],_c0,1,mcOpt.c_str(),_logy,_mcColor[i],false,_mcMarker[i]);

  }
  if(_tmp_dataH1->Integral(0,-1)>0)
    _utils->myDraw1d(_tmp_dataH1,_c0,1,"esame",_logy,kBlack,false,iMarker[0]);
  _leg->Draw();
  
  string fName= _pathPlots + "/" + "comp_" + name + _sLogy;
  //  _c0->SaveAs((fName+".pdf").c_str());
  _c0->SaveAs((fName+".png").c_str());
  _c0->SaveAs((fName+".eps").c_str());
  
}


//-------------------------------------------//
// Compare bkg estimate & data
//-------------------------------------------//
void DrawPlots::drawPlot(string name, bool logy)
{
  const float maxScaleLin=1.4;
  const float maxScaleLog=11;
  float scale=maxScaleLin;

  scale = maxScaleLin;
  string _sLogy = "";
  setLogy(logy);
  if(logy){
    _sLogy = "_logy";
    scale=maxScaleLog;
  }

  TLegend*_leg = new TLegend(0.55,0.45,0.85,0.93);
  _utils->legendSetting(_leg); 

  //Grabs all histos: data, MC, signal points
  grabHisto(name,false);

  //Build the mc stack and retrieve histo of the total. Add entry to legend
  buildStack(name,_leg);

  TH1F* _stackH = (TH1F*) _dataH1->Clone();
  _utils->getStackHisto(_mcStack,_stackH);
 
  char sData[200];
  int nData  = _dataH1->GetEntries();
  sprintf(sData,"Data (%d)",nData);
  std::cout << _dataH1->GetName() << " \t Int " << nData << std::endl;
  _leg->AddEntry(_dataH1,sData ,"p");

  if(HIDEDATA) _dataH1=NULL;
  
  TCanvas* _c0  = _utils->myCanvas(("c_"+name).c_str());
  _c0->Clear();
  TPad* _pTop = NULL;
  TPad* _pBot = NULL;
  if(HIDEDATA){
    _pTop = new TPad("pTop","pTop",0,0.05,1,1);
  }
  else{
    _pTop = new TPad("pTop","pTop",0,0.3,1,1);
    _pBot = new TPad("pBot","pBot",0,0,1,0.3);
  }

  if(_stackH->Integral(0,-1)==0 || _dataH1->Integral(0,-1)==0) logy=false;
  TVirtualPad* _tv = _utils->myDrawRatio(_c0,_pTop, _pBot, 
					_mcStack,_stackH,
					_dataH1,_leg,logy);
  //Add signals template
  for(uint i=0; i<_sigFile.size(); i++){
    _utils->myDraw1d(_sigH1[i],_c0,1,"histsame",logy,_sigColor[i],false,20);
    _leg->AddEntry(_sigH1[i],SIGFILE[i].c_str(),"l");
    _c0->Update();
    _pTop->Update();
  }

  float avgRatio = 0;
  if(_stackH->Integral(0,-1)>0) avgRatio =_dataH1->Integral(0,-1) / _stackH->Integral(0,-1);
  std::cout << "Average ratio data/MC " << avgRatio << std::endl;
  
  _c0->cd(1);
  _pTop->cd();
  drawChannelText(name,0.7,0.40);
  drawLumi();
  drawATLAS();
  _c0->Update();

  string fName= _pathPlots + "/" + "pred_" + name + _sLogy;
  //  _c0->SaveAs((fName+".pdf").c_str());
  _c0->SaveAs((fName+".png").c_str());
}


//-------------------------------------------//
// Compare bkg estimate & data w/ sys band
//-------------------------------------------//
void DrawPlots::drawPlotErrBand(string name, bool logy,bool wSig, bool sysBand)
{
  const float maxScaleLin=1.2;
  const float maxScaleLog=11;
  float scale=maxScaleLin;

  scale = maxScaleLin;
  string _sLogy = "";
 
  if(_mcStack)  _mcStack->Clear();
  if(_mcStackH) _mcStackH->Clear();
  
  TLegend*  _leg = new TLegend(0.58,0.40,0.85,0.9);

  //Grabs all histos: data, MC, signal points including the sys histos
  grabHisto(name,false,sysBand);

  
  //Data
  char sData[200];
  int nData  = _dataH1->Integral(0,-1);
  //sprintf(sData,"Data (%d)",nData);
  sprintf(sData,"Data 2012");



  //Build the mc stack and retrieve histo of the total. 
  buildStack(name,_leg);

  //Build legend
  _leg->Clear();
  _leg->AddEntry(_dataH1,sData ,"p");
  //Reverse legend Order
  for(int i=_stackOrder.size()-1; i>-1; i--){
    _leg->AddEntry(_mcH1[_stackOrder[i]][DGSys_NOM],SFILE[_stackOrder[i]].c_str(),"f");
  }


  float avgRatio = 0;
  if(_mcStackH->Integral(0,-1)>0) avgRatio =_dataH1->Integral(0,-1) / _mcStackH->Integral(0,-1);
  std::cout << "Average ratio data/MC " << avgRatio << std::endl;
  
  //Overwrite log scale if less than 100 entries 
  float maxEntries =500;
  if(_mcStackH->Integral(0,-1)<maxEntries || _dataH1->Integral(0,-1)<maxEntries) logy=false;
  setLogy(logy);
  if(logy){
    _sLogy = "_logy";
    scale=maxScaleLog;
  }
  
  //
  //Error band on prediction
  //
  TGraphAsymmErrors* _nomAsymErrors = getSysErrorBand(_mcStackH,sysBand);
  _leg->AddEntry(_nomAsymErrors,"Bkg. Uncert.","f");

  //
  //Error band on ratio 
  //
  TGraphAsymmErrors* ratioBand   = _utils->myRatioBand(_nomAsymErrors ); 


  //if(HIDEDATA) _dataH1=NULL;
  
  TCanvas* _c0  = _utils->myCanvas(("c_"+name).c_str());
  _c0->Clear();
  TPad* _pTop = NULL;
  TPad* _pBot = NULL;
  _pTop = new TPad("pTop","pTop",0,0.3,1,1);
  _pBot = new TPad("pBot","pBot",0,0,1,0.3);

  if(_mcStackH->Integral(0,-1)==0 || _dataH1->Integral(0,-1)==0) logy=false;
  TVirtualPad* _tv = _utils->myDrawRatio(_c0,_pTop, _pBot, 
					_mcStack,_mcStackH,
					_dataH1,_leg,logy);

  //Add signals template
  wSig=false;
  if(wSig){
    TString hName(name);
    for(uint i=0; i<_sigFile.size(); i++){
      //if(hName.Contains("EM") && i==0) continue; //Skip slepton for EM
      _utils->myDraw1d(_sigH1[i],_c0,1,"histsame",logy,_sigColor[i],false,20);
      _leg->AddEntry(_sigH1[i],SIGFILE[i].c_str(),"l");
      _c0->Update();
      _pTop->Update();
    }
  }
  //Add the error band
  _c0->cd(1);
  _pTop->cd();
  if(_nomAsymErrors) _nomAsymErrors->Draw("SAME && E2");
  _pTop->Modified();
  _pTop->Update();
  
  //Decoration
  drawChannelText(name,0.7,0.40);
  drawLumi();
  drawATLAS();
  _utils->legendSetting(_leg,0.04); 
  _c0->Modified();
  _c0->Update();

  //Bottom ratio band
  _pBot->cd();
  if(ratioBand)  ratioBand->Draw("same && E2");
 
 _pBot->Update();

 string fName= _pathPlots + "/" + "pred_" + name + _sLogy;
 cout << "Save plot in " << fName << endl; 
 if(_mcStackH->Integral(0,-1)!=0){ //Don't bother saving empty plots !
   _c0->SaveAs((fName+".png").c_str());
   _c0->SaveAs((fName+".eps").c_str());
 }


 //dumpBinContent(_dataH1, _mcStackH, _nomAsymErrors);
 

}
//-------------------------------------------//
// Get systematic error band for loaded histo
//-------------------------------------------//
TGraphAsymmErrors* DrawPlots::getSysErrorBand(TH1F* _hist, bool sysBand)
{
  TGraphAsymmErrors* _asymErrors = _utils->TH1TOTGraphAsymErrors(_hist);
  if(!sysBand) return _asymErrors; //Skip the sys, return just the stat error


  TH1F* totalSysHisto = (TH1F*) _hist->Clone();
  totalSysHisto->Reset();
  TGraphAsymmErrors* transient; 

  for(uint isys=DGSys_EES_Z_UP; isys<DGSys_N; isys++){
    //Deal with fake, wz & sz sys separately - uncorrelated 
    if(isys==DGSys_GEN) continue; //N/A
    if(isys>=DGSys_PDF_UP && isys<=DGSys_PDF_DN) continue;//N/A

    if(isys>=DGSys_FAKE_EL_RE_UP && isys <= DGSys_FAKE_MU_FR_DN) continue;
    if(isys>=DGSys_XS_UP && isys<=DGSys_XS_DN) continue;//uncorrelated
    if(isys>=DGSys_GEN_UP && isys<=DGSys_GEN_DN) continue;

    for(uint imc=0; imc<_mcH1.size(); imc++){ //100% correlation between sample - add linear
      TH1F* _hsys;
      if(imc==FAKE && !disableFake)_hsys = _mcH1[FAKE][DGSys_NOM]; //add the nominal values since those sys have no effect
      else  _hsys = _mcH1[imc][isys];
      if(_hsys && _hsys->Integral(0,-1)>0) {
	/*
	if(isys == DGSys_MS_UP || isys==DGSys_GEN_UP ||isys==DGSys_GEN_DN){
	  cout << "\t\t MC " << MCNames[imc]
	       << "\t Sys " << DGSystNames[isys] 
	       << "\t " << _hsys->Integral(0,-1) << endl;
	}
	*/
	totalSysHisto->Add(_hsys);
      }
      //else cout << " Sys " << DGSystNames[isys] << " empty " << endl;
    }   
    if(totalSysHisto->Integral(0,-1)>0){
      /*
      cout << "Sys " << DGSystNames[isys] 
	   << "\t nom " << totalSysHisto->GetBinContent(1) 
	   << "\t err " << totalSysHisto->GetBinError(1) 
	   << "\t diff " << totalSysHisto->Integral(0,-1)-_hist->Integral(0,-1)
	   << endl;
      */
      transient = _utils->TH1TOTGraphAsymErrors(totalSysHisto);   //Mem leak!!!
      _utils->myAddtoBand(transient,_asymErrors); //100 uncorrelated sys - add in quad
    }
    totalSysHisto->Reset();
  }
  //cout << "Sys band mc correlated only " << endl;
  //_asymErrors->Print();

  //XS uncertainty uncorrelated across sample - NOT FINISH
  for(uint imc=0; imc<_mcH1.size(); imc++){
    if(imc==FAKE && !disableFake) continue;
    vector<TH1F*> xsSys;
    TH1F* sys00 = (TH1F*) _mcH1[imc][DGSys_XS_UP]->Clone();
    TH1F* sys11 = (TH1F*) _mcH1[imc][DGSys_XS_DN]->Clone();
    xsSys.push_back(sys00);
    xsSys.push_back(sys11);
    totalSysHisto->Reset();
    for(uint i=0; i<xsSys.size(); i++){//loop xs UP/DN
      for(uint iimc=0; iimc<_mcH1.size(); iimc++){
	if(iimc == imc) continue;
	TH1F* _hsys = _mcH1[iimc][DGSys_NOM];
	totalSysHisto->Add(_hsys);
      }
      totalSysHisto->Add(xsSys[i]); //Add the wz sys
      //cout << "Check Int " << imc << " XS sys i " << xsSys[i]->Integral(0,-1)<<endl;;
 
      transient = _utils->TH1TOTGraphAsymErrors(totalSysHisto);   //Mem leak!!!
      _utils->myAddtoBand(transient,_asymErrors); //100 uncorrelated sys - add in quad to the other MC bkg sys
      totalSysHisto->Reset();
    }
  }
  //cout << "Sys after xs " << endl;
  //_asymErrors->Print();


  //FOR WH analysis 
  //Add the WZ systematic from GEN_UP/DN  (uncorrelated to others)
  vector<TH1F*> wzSys;
  TH1F* sys00 = (TH1F*) _mcH1[ZV][DGSys_GEN_UP]->Clone();
  TH1F* sys11 = (TH1F*) _mcH1[ZV][DGSys_GEN_DN]->Clone();
  //  cout << "WZ NOM " << _mcH1[ZV][DGSys_NOM]->Integral(0,-1) 
  //<< " UP "    << _mcH1[ZV][DGSys_GEN_UP]->Integral(0,-1) 
  //<< " DN "    << _mcH1[ZV][DGSys_GEN_DN]->Integral(0,-1) << endl;

  wzSys.push_back(sys00);
  wzSys.push_back(sys11);
  for(uint i=0; i<wzSys.size(); i++){//loop WZ UP/DN
    totalSysHisto->Reset();
    for(uint imc=0; imc<_mcH1.size(); imc++){
      if(imc==ZV) continue;
      TH1F* _hsys = _mcH1[imc][DGSys_NOM];
      totalSysHisto->Add(_hsys);
    }
    
    //cout << "\t WZ check yield before wz sys " << totalSysHisto->Integral(0,-1) << endl;
    totalSysHisto->Add(wzSys[i]); //Add the wz sys
    //cout << "\t Check Int WZ sys " << wzSys[i]->Integral(0,-1)<<endl;;
    
    transient = _utils->TH1TOTGraphAsymErrors(totalSysHisto);   //Mem leak!!!
    _utils->myAddtoBand(transient,_asymErrors); //100 uncorrelated sys - add in quad to the other MC bkg sys
    totalSysHisto->Reset();
  }
  //cout << "Sys after wz " << endl;
  //_asymErrors->Print();


  //Add the fake systematics
  if(!disableFake){
    vector<TH1F*> fakeSys;
    TH1F* sys0 = (TH1F*) _mcH1[FAKE][DGSys_NOM]->Clone();
    TH1F* sys1 = (TH1F*) _mcH1[FAKE][DGSys_NOM]->Clone();
    sys0->Reset();
    sys1->Reset();
    fakeSys.push_back(sys0);
    fakeSys.push_back(sys1);
    getFakeSys(fakeSys); //Get fake sys UP/DN
    
    totalSysHisto->Reset();
    for(uint i=0; i<fakeSys.size(); i++){//loop fake UP/DN
      // Add to the Fake UP/DN the other backgrounds nominal value
      for(uint imc=0; imc<_mcH1.size(); imc++){
	if(imc==FAKE) continue;
	TH1F* _hsys = _mcH1[imc][DGSys_NOM];
	totalSysHisto->Add(_hsys);
      }

      totalSysHisto->Add(fakeSys[i]); //Add the fake sys
      //cout << "Check Int sys i " << fakeSys[i]->Integral(0,-1)<<endl;;
      
      transient = _utils->TH1TOTGraphAsymErrors(totalSysHisto);   //Mem leak!!!
      _utils->myAddtoBand(transient,_asymErrors); //100 uncorrelated sys - add in quad to the other MC bkg sys
      totalSysHisto->Reset();
    }
    
    
    //clean up 
    for(uint i=0; i<fakeSys.size(); i++) fakeSys[i]->Delete();
    fakeSys.clear();
  }
  
  //cout << "Sys band final " << endl;
  //_asymErrors->Print();



  return _asymErrors;

}
//-------------------------------------------//
// Get Sys UP & Sys DN for fake estimate
// This is a bin by bin computation depending if a given 
// sys give more or less than the nominal
//-------------------------------------------//
void DrawPlots::getFakeSys(vector<TH1F*> &sys)
{
  
  int nbins = _mcH1[FAKE][DGSys_NOM]->GetNbinsX();  
  for(int bin=1; bin<nbins+1; ++bin){
    float stat = pow(_mcH1[FAKE][DGSys_NOM]->GetBinError(bin),2);//stat err
    float err_up = stat; 
    float err_dn = stat;
    float bc = _mcH1[FAKE][DGSys_NOM]->GetBinContent(bin);
    /*
    cout << " Fake " << MCNames[FAKE]
	 << "\t\t Nom " 
	 << "\t " << bc << " " << stat << endl;
    */
    for(uint isys=DGSys_FAKE_EL_RE_UP; isys<=DGSys_FAKE_MU_FR_DN; ++isys){
      float shift = _mcH1[FAKE][isys]->GetBinContent(bin);
      
      /*
      cout << " Fake " << MCNames[FAKE]
	   << "\t\t Sys " << DGSystNames[isys] 
	   << "\t " << shift << endl;
      int ibin1 = _hSys->GetBin(70);
      int ibin2 = _hSys->GetBin(75);
      cout << "\t bin 70 " << _hSys->GetBinContent(ibin1) << " \t" <<  _hSys->GetBinContent(ibin2) <<endl;
      */
      if(shift > bc) err_up += pow(shift-bc,2);
      else           err_dn += pow(shift-bc,2);
    }// end loop over sys
    
    sys[0]->SetBinContent(bin, sqrt(err_up));
    sys[1]->SetBinContent(bin, sqrt(err_dn));
    //cout << "Check Fake sys0 " << sys[0]->GetBinContent(bin) << endl;
    //cout << "Check Fake sys1 " << sys[1]->GetBinContent(bin) << endl;
  }// end loop over bins
  
  sys[0]->Add(_mcH1[FAKE][DGSys_NOM]);
  TH1F* _htmp = (TH1F*) _mcH1[FAKE][DGSys_NOM]->Clone();
  _htmp->Add(sys[1],-1);
  sys[1]->Reset();
  sys[1]->Add(_htmp); 
  _htmp->Delete();
  
  /*
  cout << "Check fake nom " << _mcH1[FAKE][DGSys_NOM]->Integral(0,-1)<<endl;;
  cout << "Check Int sys0 " << sys[0]->Integral(0,-1)<<endl;;
  cout << "Check Int sys1 " << sys[1]->Integral(0,-1)<<endl;;
  */

}
//-------------------------------------------//
// To dump the bin content and errors
//-------------------------------------------//
void DrawPlots::dumpBinContent(TH1* _dataH, TH1* _mcH, TGraphAsymmErrors* _sysBand)
{
  int nbins = _dataH->GetNbinsX();  
  for(int bin=1; bin<nbins+1; ++bin){
    std::cout << "Bin " << bin 
	      << " data " << _dataH->GetBinContent(bin)
	      << " SM " << _mcH->GetBinContent(bin)
	      << " +/- " << _sysBand->GetErrorYlow(bin-1)
	      << " " << _sysBand->GetErrorYhigh(bin-1)
	      << std::endl;
  }

  


}
//-------------------------------------------//
// Draw channel label
//-------------------------------------------//
void DrawPlots::drawChannelText(string name, float x, float y, bool desc)
{
  string _text;
  string _text2;
 
  TString hName(name);
  if(hName.Contains("DG2L_") || hName.Contains("ML_")){

    if(hName.Contains("EE")) _text += "ee";
    else if(hName.Contains("MM")) _text += "#mu#mu";
    else if(hName.Contains("EM")) _text += "e#mu";
    _text2 = _text;


    if(hName.Contains("preSROSjveto"))        _text = "preSROSjveto ";
    else if(hName.Contains("preSRmT2")){
      _text = "preSRmT2 ";
      _text2 = _text2 + " nJets=0, Zveto, E_{T}^{miss,rel}>40 GeV";
    }
    else if(hName.Contains("CRWW5")){
      _text = "WW CR";
      _text2 = _text2 + " nJets=0, Zveto, E_{T}^{miss,rel}>40 GeV, 50<m_{T2}<90 GeV";
    }
    else if(hName.Contains("CRTOP")){
      _text = "TOP CR";
      _text2 = _text2 + " nJets>=1, b-Tag, Zveto, E_{T}^{miss,rel}>40 GeV";
    }
    else if(hName.Contains("SRmT2a")){
      _text = "SRmT2-90 ";
      _text2 = _text2 + " nJets=0, Zveto, m_{T2}>90 GeV";
    }
    else if(hName.Contains("SRmT2b")){
      _text = "SRmT2-120 ";
      _text2 = _text2 + " nJets=0, Zveto, m_{T2}>120 GeV";
    }
    else if(hName.Contains("SRmT2c")){
      _text = "SRmT2-150 ";
      _text2 = _text2 + " nJets=0, Zveto, m_{T2}>150 GeV";
    }
    else if(hName.Contains("ZXCRmT2a")){
      _text = "ZXCRmT2-90 ";
      _text2 = _text2 + " nJets=0, Z-window, E_{T}^{miss,rel}>40 GeV, m_{T2}>90 GeV";
    }
    else if(hName.Contains("ZXCRmT2b")){
      _text = "ZXCRmT2-110 ";
      _text2 = _text2 + " nJets=0, Z-window, E_{T}^{miss,rel}>40 GeV, m_{T2}>110 GeV";
    }
    else if(hName.Contains("VRWZ")){
      _text ="";
      _text2 = "lll, SFOS Z-window, E_{T}^{miss}>30 GeV, m_{T}>40 GeV, NB=0";
    }
    else if(hName.Contains("VRZZ")){
      _text ="";
      _text2 = "llll, 2 SFOS Z-window, E_{T}^{miss}<50 GeV, NB=0";
    }

    else if(hName.Contains("preSR2jets"))   _text = "preSR2jets ";
    else if(hName.Contains("preSRZjets"))   _text = "preSRZjets ";
    else if(hName.Contains("preSRSS")) _text = "preSRSS ";

    else if(hName.Contains("SROSjveto"))        _text = "SROSjveto ";
    else if(hName.Contains("SR2jets"))   _text = "SR2jets ";
    else if(hName.Contains("SRZjets"))   _text = "SRZjets ";
    else if(hName.Contains("SRSSjets")) _text = "SRSSjets ";
    else if(hName.Contains("SRWWa"))    _text = "SRWWa ";
    else if(hName.Contains("SRWWb"))    _text = "SRWWb ";
    else if(hName.Contains("SRWWc"))    _text = "SRWWc ";
    
    else if(hName.Contains("ZXCRjveto"))       _text = "ZXCRjveto ";
    else if(hName.Contains("ZXCR2jets"))       _text = "ZXCR2jets ";
    else if(hName.Contains("ZXCRmT2a"))       _text = "ZXCRmT2a ";
    else if(hName.Contains("ZXCRmT2b"))       _text = "ZXCRmT2b ";
    else if(hName.Contains("CRTOP"))      _text = "CRTOP ";
    else if(hName.Contains("CRTOPWWa"))      _text = "CRTOPWWa ";
    else if(hName.Contains("CRTOPWWb"))      _text = "CRTOPWWb ";
    else if(hName.Contains("CRTOPWWc"))      _text = "CRTOPWWc ";
    else if(hName.Contains("CRWW"))      _text = "CRWW ";
    else if(hName.Contains("CRWWa"))      _text = "CRWWa ";
    else if(hName.Contains("CRWWb"))      _text = "CRWWb ";
    else if(hName.Contains("CRWWc"))      _text = "CRWWc ";

    else if(hName.Contains("VRSS"))     _text = "VRSS ";
    else if(hName.Contains("VRSSbtag"))     _text = "VRSSbtag ";

    else if(hName.Contains("CRZ"))       _text = "CRZ ";
    else if(hName.Contains("CR2LepOS"))  _text = "CR2LepOS ";
    else if(hName.Contains("CR2LepSS40"))  _text = "CR2LepSS40 ";
    else if(hName.Contains("CR2LepSS"))  _text = "CR2LepSS ";


  }

  else if(hName.Contains("ML_")){
     if(hName.Contains("SR3Lep"))    _text = "SR3Lep ";
     if(hName.Contains("SRB"))       _text = "SRB ";
     if(hName.Contains("SR1a"))      _text = "SR1a ";
     if(hName.Contains("SR1b"))      _text = "SR1b ";
     if(hName.Contains("SR2"))       _text = "SR2 ";
     if(hName.Contains("VR0"))       _text = "VR0 ";
     if(hName.Contains("VR1"))       _text = "VR1 ";
     if(hName.Contains("VR2"))       _text = "VR2 ";
     if(hName.Contains("VR3"))       _text = "VR3 ";
     if(hName.Contains("VRWZ"))      _text = "VRWZ ";
     if(hName.Contains("NRWZ"))      _text = "NRWZ ";
     if(hName.Contains("SR4lep"))    _text = "4-leptons "; 
     if(hName.Contains("SR4lepNoZ")) _text = "4-leptons no Z ";
  }



  if(desc){
    _utils->myText(0.15,.95,kBlack,_text2.c_str(),0.04);
  }
  else   _utils->myText(x,y,kBlack,_text.c_str(),0.05);

}
//-------------------------------------------//
// Draw Lumi
//-------------------------------------------//
void DrawPlots::drawLumi(float x, float y)
{
  char s[20] ="";
  sprintf(s,"%3.1f",pLUMI);
  string _text = "#int L dt=" + string(s) + " fb^{-1}  #sqrt{s}=8 TeV";
  //string _text = "#int L dt=" + string(s) + " fb^{-1} ";
  _utils->myText(x,y,kBlack,_text.c_str(),0.05);
}
//-------------------------------------------//
// Draw ATLAS label
//-------------------------------------------//
void DrawPlots::drawATLAS(float x, float y)
{
  //string _text = "ATLAS Work In Progress";
  string _text1 = "#bf{ATLAS} ";
  _utils->myText(x,y,kBlack,_text1.c_str(),0.07);
  string _text2 = "Internal";
  //string _text2 = "Preliminary";
  _utils->myText(x+0.15,y,kBlack,_text2.c_str(),0.05);
  
}
//_____________________________________________________________________________//
std::vector<TH1F*> DrawPlots::loadHisto(TFile* file,string DSId, 
					string name,bool verbose){
  std::vector<TH1F*> _hArray;
  string title;
  TH1F* _h;
  
  file->cd();
  _hArray.reserve(DGSys_N);

  for(int isys=0; isys<DGSys_N; isys++){
    string _hName = name + "_" + DGSystNames[isys];
    _h = (TH1F*) file->Get(_hName.c_str());
    if(_h==NULL)    continue;

    _hArray.push_back(_h);
    _utils->moveUnderOverFlow(_hArray[isys]);
    
    title = DSId + "_" + _hName;
    _hArray[isys]->SetTitle(title.c_str());
    _hArray[isys]->SetName(title.c_str());
    if(verbose && isys==DGSys_NOM) 
      std::cout << "SIG " << _hArray[isys]->GetName() 
		<< " Int: " << _hArray[isys]->Integral(0,-1) <<std::endl;      
  }

  return _hArray;

}
//_____________________________________________________________________________//
std::vector<TH1F*> DrawPlots::sumChannels(std::vector<TH1F*> _histEE,
					  std::vector<TH1F*> _histMM,
					  std::vector<TH1F*> _histEM){
  std::vector<TH1F*> _hArray;
  string title;
  TH1F* _h;

  _hArray.reserve(DGSys_N);
  for(unsigned int isys=0; isys<_histEE.size() /*DGSys_N*/; isys++){
    if(_histEE[isys]==NULL) continue;
    if(_histMM[isys]==NULL) continue;
    if(_histEM[isys]==NULL) continue;
    
    TString _hName = _histEE[isys]->GetName();
    _hName.ReplaceAll("EE","ALL");
    _h = (TH1F*) _histEE[isys]->Clone();
    _h->SetName(_hName.Data());
    _h->SetTitle(_hName.Data());
    _h->Add(_histMM[isys]);
    _h->Add(_histEM[isys]);
    _hArray.push_back(_h);
  }
  return _hArray;
}

//_____________________________________________________________________________//
void DrawPlots::clearHistVect(std::vector<TH1F*> _histVec){

  for(uint i=0; i<_histVec.size(); i++){
    if(_histVec[i]!=NULL) _histVec[i]->Delete();
  }
  _histVec.clear();
  
}
//_____________________________________________________________________________//
void DrawPlots::getYield(std::vector<TH1F*> histV, 
			 Double_t &nom,
			 Double_t &stat_err,
			 Double_t &sysUp, Double_t &sysDn,
			 bool verbose){
  nom      = 0; 
  stat_err = 0;
  sysUp    = 0;
  sysDn    = 0;
 
  if(histV[DGSys_NOM]==NULL){
    cerr << "Empty hist " << endl;
    return;
    //abort();
  }

  nom = histV[DGSys_NOM]->IntegralAndError(0,-1,stat_err);

  if(histV.size()==1) {
    if(verbose)
      cout << "\t Yield " << nom 
	   << " +/- " << stat_err
	   << endl;
    return; //No sys available 
  }

  for(uint isys=DGSys_NOM+1; isys<DGSys_N; isys++){
    //if(isys==DGSys_Pileup_UP || isys==DGSys_Pileup_DN) continue;
    if(histV[isys]==NULL) continue;
    Double_t val = histV[isys]->Integral(0,-1);
    if(val==0) continue; //No Sys
    Double_t shift = val-nom;
    if(shift>0) sysUp += pow(shift,2);
    else        sysDn += pow(shift,2);
    /*
    cout << "\t\t sys " << DGSystNames[isys] 
	 << " " << val
	 << " " << shift 
	 << " + " << sysUp
	 << " - " << sysDn
	 << endl; 
    */
  }
  sysUp = sqrt(sysUp);
  sysDn = sqrt(sysDn);
  
  if(verbose) cout << "\t Yield " << nom 
		   << " +/- " << stat_err
		   << " + " << sysUp << " - " << sysDn 
		   << endl;
  
}


//_____________________________________________________________________________//
void DrawPlots::getYieldBkgAll(std::vector<TH1F*> histFakeV, 
			      std::vector<TH1F*> histZVV, 
			      std::vector<TH1F*> histWWV, 
			      std::vector<TH1F*> histTopV, 
			      std::vector<TH1F*> histZjetsV,
			      std::vector<TH1F*> histHiggs,
			      Double_t &nom,
			      Double_t &stat_err,
			      Double_t &sysUp, Double_t &sysDn,
			      bool verbose){
  
  sysUp=0;
  sysDn=0;

  //Treat MC Bkg sys as correlated - add linearly
  //Fake sys uncorrelated w/ others - add in quad
  TH1F* _hSum = (TH1F*)  histFakeV[DGSys_NOM]->Clone();
  _hSum->SetTitle("totBkg");
  _hSum->SetName("totBkg");
  _hSum->Add(histZVV[DGSys_NOM]);
  _hSum->Add(histWWV[DGSys_NOM]);
  _hSum->Add(histTopV[DGSys_NOM]);
  _hSum->Add(histZjetsV[DGSys_NOM]);
  _hSum->Add(histHiggs[DGSys_NOM]);

  nom = _hSum->IntegralAndError(0,-1,stat_err);
  
  float mcSysUp=0;
  float mcSysDn=0; 
  for(uint isys=DGSys_NOM; isys< DGSys_N; isys++){
    if(isys == DGSys_XS_UP || isys==DGSys_XS_DN ||
       isys == DGSys_GEN_UP || isys==DGSys_GEN_DN) continue;
    //skip fake
    if(isys > DGSys_XS_DN && isys<DGSys_GEN) continue;
    
    Double_t val = 0;
    if(histZVV[isys]->Integral(0,-1)>0) 
      val += histZVV[isys]->Integral(0,-1)-histZVV[DGSys_NOM]->Integral(0,-1);
    if(histWWV[isys]->Integral(0,-1)>0)
      val +=  histWWV[isys]->Integral(0,-1)-histWWV[DGSys_NOM]->Integral(0,-1);
    if(histTopV[isys]->Integral(0,-1)>0)
      val +=  histTopV[isys]->Integral(0,-1)-histTopV[DGSys_NOM]->Integral(0,-1);
    if(histZjetsV[isys]->Integral(0,-1)>0)
      val +=  histZjetsV[isys]->Integral(0,-1)-histZjetsV[DGSys_NOM]->Integral(0,-1);
    if(histHiggs[isys]->Integral(0,-1)>0)
      val +=  histHiggs[isys]->Integral(0,-1)-histHiggs[DGSys_NOM]->Integral(0,-1);
    
    if(val==0) continue; //No Sys
    if(val>0) mcSysUp += pow(val,2);
    else      mcSysDn += pow(val,2);

    if(verbose) 
      cout << "\t SysMC " << DGSystNames[isys] 
	   << " " << val 
	   << " + " << mcSysUp
	   << " - " << mcSysDn
	   << endl; 

  }
  mcSysUp = sqrt(mcSysUp);
  mcSysDn = sqrt(mcSysDn);
  if(verbose) cout << "MC sys " << mcSysUp << " " << mcSysDn << endl;
  
  //Add Xs sys - uncorrelated between grouping - add quad
  float xsSysUp=0;
  float xsSysDn=0;
  for(uint isys=DGSys_XS_UP; isys<DGSys_XS_DN+1; isys++){
    Double_t   val = 0;
    if(histZVV[isys]->Integral(0,-1)>0) 
      val += pow(histZVV[isys]->Integral(0,-1)-histZVV[DGSys_NOM]->Integral(0,-1),2);
    if(histWWV[isys]->Integral(0,-1)>0) 
      val += pow(histWWV[isys]->Integral(0,-1)-histWWV[DGSys_NOM]->Integral(0,-1),2);
    if(histTopV[isys]->Integral(0,-1)>0) 
      val += pow(histTopV[isys]->Integral(0,-1)-histTopV[DGSys_NOM]->Integral(0,-1),2);
    if(histZjetsV[isys]->Integral(0,-1)>0) 
      val += pow(histZjetsV[isys]->Integral(0,-1)-histZjetsV[DGSys_NOM]->Integral(0,-1),2);
    if(histHiggs[isys]->Integral(0,-1)>0) 
      val += pow(histHiggs[isys]->Integral(0,-1)-histHiggs[DGSys_NOM]->Integral(0,-1),2);

    if(isys==DGSys_XS_UP)      xsSysUp = sqrt(val);
    else if(isys==DGSys_XS_DN) xsSysDn = sqrt(val);
  }
   if(verbose) cout << "XS sys " << xsSysUp << " " << xsSysDn << endl;

  //Add WZ sys
  float wzSysUp=0;
  float wzSysDn=0;
  if(histZVV.size()>1){
    for(uint isys=DGSys_GEN_UP; isys<DGSys_GEN_DN+1; isys++){
      if(histZVV[isys]==NULL) continue;
      Double_t val=0;
      if(histZVV[isys]->Integral(0,-1)>0)
	val = histZVV[isys]->Integral(0,-1) - histZVV[DGSys_NOM]->Integral(0,-1);
      if(val>0) wzSysUp += pow(val,2);
      else      wzSysDn += pow(val,2);
    }
  }
  wzSysUp = sqrt(wzSysUp);
  wzSysDn = sqrt(wzSysDn);
   if(verbose) cout << "WZ sys " << wzSysUp << " " << wzSysDn << endl;

  //Add fake sys
  float fakeSysUp=0;
  float fakeSysDn=0;
  if(histFakeV.size()>1){
    for(uint isys=DGSys_FAKE_EL_RE_UP; isys<DGSys_FAKE_MU_FR_DN+1; isys++){
      if(histFakeV[isys]==NULL) continue;
      Double_t val=0;
      if(histFakeV[isys]->Integral(0,-1)>0)
	val = histFakeV[isys]->Integral(0,-1) - histFakeV[DGSys_NOM]->Integral(0,-1);
      if(val>0) fakeSysUp += pow(val,2);
      else      fakeSysDn += pow(val,2);
    }
    fakeSysUp = sqrt(fakeSysUp);
    fakeSysDn = sqrt(fakeSysDn);
    //cout << "Fake sys " << fakeSysUp << " " << fakeSysDn << endl;
  }

  //
  //Add MC & fake sys in quad
  //
  sysUp = sqrt(pow(mcSysUp,2) + pow(xsSysUp,2) + pow(wzSysUp,2) + pow(fakeSysUp,2) );
  sysDn = sqrt(pow(mcSysDn,2) + pow(xsSysDn,2) + pow(wzSysDn,2) + pow(fakeSysDn,2) );
  
  if(verbose)
    cout << "\t Yield " << nom 
	 << " +/- " << stat_err
	 << " + " << sysUp << " - " << sysDn 
	 << endl;
  
}

//_____________________________________________________________________________//
TFile* DrawPlots::openFile(string DSId){
  
  string fileName = getFileName(DSId);

  TFile*_file = new TFile(fileName.c_str(),"READ",DSId.c_str());
  if(_file->IsOpen()==kFALSE){
    std::cout << "Fail opening file " << fileName << std::endl;
    abort();
  }
  else{
    cout << "Opened file " << fileName << endl;
  }

  return _file;
}
//_____________________________________________________________________________//
string  DrawPlots::getFileName(string DSId){

  _pathHisto  =  string(getenv("HISTOANA"))  + "/SusyAna/histos_" + DATE;
  string fileName="";
  
  string tmpFile = "fileName.txt";
  string cmd = "ls -1 " + _pathHisto + "/*" + DSId + "*.root > " +tmpFile;  

  gSystem->Exec(cmd.c_str());

  FILE* fInput;
  if ((fInput = fopen(tmpFile.c_str(),"r")) == NULL) {
    cout << "File " << tmpFile << " could not be opened. Exit" << endl;;
    abort();
  }
  char _name[200];
  while (!feof( fInput )) {
    if (fscanf(fInput, "%s\n",&_name[0])){
      fileName = string(_name);
    }
  }

  cmd = "rm -f " + tmpFile;
  gSystem->Exec(cmd.c_str());

  return fileName;

}




//-----------------------------------------------------------------------------
void DrawPlots::binomialError(float Num, float Den, float& Eff, float& EffErr){
  //Compute Eff=N1/N2 where N1=pass, N2=total (pass+fail)
  Eff=0; 
  EffErr=0;
  Eff = (Den >0. ? Num/Den : 0.);
  EffErr = (Den >0.  ? sqrt((Eff-pow(Eff,2))/Den) : 0.);
}

//-----------------------------------------------------------------------------
TH1F* DrawPlots::calcEff(TH1F* h, int opt)
{
  TH1F* hh = (TH1F*) h->Clone(); hh->Reset();
  float den = h->Integral(0,-1);

  for(int ibin=1; ibin<=h->GetNbinsX(); ibin++){
    float num = h->Integral(0,ibin);
    float eff=0;
    float eff_err=0;
    binomialError(num,den,eff,eff_err);
    if(opt==1) eff = 1-eff;
    hh->SetBinContent(ibin,eff);
    hh->SetBinError(ibin,eff_err);
  }
  return hh;
}

//-----------------------------------------------------------------------------
// Make ratio plot
//-----------------------------------------------------------------------------
TH1F* DrawPlots::calcRatio(TH1F* hnum, TH1F* hden, string name, string opt)
{
  //Compute FR
  TH1F* _h_R = (TH1F*) hnum->Clone();
  _h_R->Reset();
  _h_R->Sumw2();
  _h_R->SetName(name.c_str()); 
  _h_R->SetTitle(name.c_str());
  _h_R->GetYaxis()->SetTitle("Efficiency");
  _h_R->Divide(hnum,hden,1,1,opt.c_str());

  return _h_R;
}
/*--------------------------------------------------------------------------------*/
// Convert int to string
/*--------------------------------------------------------------------------------*/
string DrawPlots::itos(int i){
  string s;
  std::stringstream out;
  out << i;
  s = out.str();
  return s;
}

//-------------------------------------------//
// Make a TGraphErr from 2 histo as xy
//-------------------------------------------//
TGraphErrors* DrawPlots::TGraph2Hist(TH1F* hx, TH1F* hy)
{
  float x[500];
  float x_err[500];
  float y[500];
  float y_err[500];

  int nPoints=hx->GetNbinsX();
  for(int ibin=1; ibin<=hx->GetNbinsX(); ibin++){
    x[ibin-1]     = hx->GetBinContent(ibin);
    x_err[ibin-1] = hx->GetBinError(ibin);
    y[ibin-1]     = hy->GetBinContent(ibin);
    y_err[ibin-1] = hy->GetBinError(ibin);
  }
  TGraphErrors* g0 = new TGraphErrors(nPoints,&x[0],&y[0],&x_err[0],&y_err[0]); 
  return g0;
}

//-------------------------------------------//
// Make a TH1 projection given axis and bining
//-------------------------------------------//
TH1F* DrawPlots::get1DHisto(TH3* _h, string axis, int bin1a, int bin2a, int bin1b, int bin2b)
{
  TString AXIS(axis);
  TH1F* _hh;
  
  if(AXIS.Contains("x")) _hh = (TH1F*) _h->ProjectionX("_px",bin1a, bin2a, bin1b,bin2b);
  if(AXIS.Contains("y")) _hh = (TH1F*) _h->ProjectionY("_py",bin1a, bin2a, bin1b,bin2b);
  if(AXIS.Contains("z")) _hh = (TH1F*) _h->ProjectionZ("_pz",bin1a, bin2a, bin1b,bin2b);
  
  return _hh;
}

//-------------------------------------------//
// Make a profile plot given axis parameters
//-------------------------------------------//
TH1F* DrawPlots::getProfile3D(TH3* _h, string axis, string afix, string aloop)
{
  //std::cout << "here in " << _h->GetName() << std::endl;

  TString AXIS(axis);   //axis to project onto
  TString FIX(afix);    //axis to integrate over
  TString LOOP(aloop);  //axis to loop over to do the projection against
  TH1F* _hp;


  if(AXIS.Contains("x")){
    if(FIX.Contains("y")){ _hp = (TH1F*) _h->ProjectionZ();}
    if(FIX.Contains("z")){ _hp = (TH1F*) _h->ProjectionY();}
  }
  if(AXIS.Contains("y")){
    if(FIX.Contains("x")){ _hp = (TH1F*) _h->ProjectionZ();}
    if(FIX.Contains("z")){ _hp = (TH1F*) _h->ProjectionX();}
  }
  if(AXIS.Contains("z")){
    if(FIX.Contains("x")){ _hp = (TH1F*) _h->ProjectionY();}
    if(FIX.Contains("y")){ _hp = (TH1F*) _h->ProjectionX();}
  }
  _hp->Reset();
  
  for(int i=1; i<_hp->GetXaxis()->GetNbins(); i++){
    TH1F* _h1;
    if(AXIS.Contains("x") && FIX.Contains("y")) _h1 = get1DHisto(_h,axis,0,-1, i,i+1);
    if(AXIS.Contains("x") && FIX.Contains("z")) _h1 = get1DHisto(_h,axis,i,i+1, 0,-1);
    if(AXIS.Contains("y") && FIX.Contains("x")) _h1 = get1DHisto(_h,axis,0,-1, i,i+1);
    if(AXIS.Contains("y") && FIX.Contains("z")) _h1 = get1DHisto(_h,axis,i,i+1, 0,-1);  
    if(AXIS.Contains("z") && FIX.Contains("x")) _h1 = get1DHisto(_h,axis,0,-1, i,i+1);
    if(AXIS.Contains("z") && FIX.Contains("y")) _h1 = get1DHisto(_h,axis,i,i+1, 0,-1);
    
    if(_h1->Integral(0,-1)<20) continue;
    
    float mean = _h1->GetMean();
    float meanErr = _h1->GetMeanError();
    
    _hp->SetBinContent(i,mean);
    _hp->SetBinError(i,meanErr);
    

    //This is not working - tto mnay 0 entries
    //Double_t TMath::Median(Long64_t n, const Double_t *a, const Double_t *w, Long64_t *work)
    //a are the bin centers and array w is the bin contents
    /*
    double *binCenter = new double[_h1->GetXaxis()->GetNbins()];
    double *binContent = new double[_h1->GetXaxis()->GetNbins()];
    for (Int_t  binx = 1; binx <_h1->GetXaxis()->GetNbins(); binx++) {
      binCenter[i]  = _h1->GetBinCenter(binx);
      binContent[i] = _h1->GetBinContent(binx);
      std::cout << "center " <<  binCenter[i] << " val " << binContent[i] <<std::endl;
    }
    double median = TMath::Median(_h1->GetXaxis()->GetNbins(),binCenter,binContent);
    _hp->SetBinContent(i,float(median));

    delete binCenter;
    delete binContent;
    */
  }


  return _hp;
}

//-------------------------------------------//
// Get fake bkg composition
//-------------------------------------------//
void DrawPlots::getFakeComposition(string sAna, string sSR, string lep)
{
  /*
  const char* const FKTYPE[]= {"HF","LF","CONV"}; 
  string sType[3]= {"hf","lf","cv"};
  vector<TH1F*>   _hFake[3]; 
  float _total=0;  //total fake
  vector<TH1F*>   _hFrac; 

  TH1F* _hSum = new TH1F("fkTot","fkTot",3,-0.5,2.5); //Sum of each type
  TH1F* _h1   = _utils->myTH1F("fkFrac","fkFrac",3,-0.5,2.5,"","fake fraction");

  //grab histograms for each fake type & each bkg
  for(uint iFk=0; iFk<3; iFk++){
    string hName = sAna + "_" + sSR + "_" + lep 
      + "_" + sType[iFk] + "_" + sAna + "_fakeLep";
    std::cout << "Grabing histo " << hName << std::endl;
    grabHisto(hName);
    _hFake[iFk].insert(_hFake[iFk].end(),_mcH1.begin(), _mcH1.end());  
    _h1->GetXaxis()->SetBinLabel(iFk+1,FKTYPE[iFk]);
  }
  
  //Dump integral for check
  //Get total fake integral
  for(uint iFk=0; iFk<3; iFk++){
    std::cout << ">>> " << FKTYPE[iFk] << std::endl;
    for(uint imc=0; imc<_hFake[iFk].size(); imc++){
      std::cout << "\t MC " << SFILE[imc] << " I = " 
		<< _hFake[iFk][imc]->Integral(0,-1) << std::endl;
      _total += _hFake[iFk][imc]->Integral(0,-1);
      _hSum->AddBinContent(iFk+1,_hFake[iFk][imc]->Integral(0,-1));

      if(iFk==0){
	string ss = SFILE[imc] + "_fkFrac";
	TH1F* _tmpH1 = (TH1F*) _h1->Clone();
	_tmpH1->SetTitle(ss.c_str());
	_tmpH1->SetName(ss.c_str());
	_hFrac.push_back(_tmpH1);
      }
    } 
  }
  std::cout << "total " << _total << std::endl;

  //Compute the fraction of fake contribution for each bkg
  for(uint iFk=0; iFk<3; iFk++){
    for(uint imc=0; imc<_hFake[iFk].size(); imc++){
      float _frac, _fracErr;
      binomialError(_hFake[iFk][imc]->Integral(0,-1),_total,_frac,_fracErr);
      _hFrac[imc]->SetBinContent(iFk+1,_frac);
      _hFrac[imc]->SetBinError(iFk+1,_fracErr);
    }
  }

  //Build Stack histo, legend
  _mcH1.clear();
  _mcH1.insert(_mcH1.end(),_hFrac.begin(), _hFrac.end()); 
  _leg = new TLegend(0.6,0.7,0.90,0.93);
  _utils->legendSetting(_leg); 
  buildStack("fakeFrac");
  _utils->getStackHisto(_mcStack,_mcStackH);
 
  TCanvas* _c0  = _utils->myCanvas("fakeFrac");
  _c0->cd(1);
  _utils->myDrawTHStack(_c0,_mcStack,_mcStackH,_leg,false);

  string fName= _pathPlots + "/" + "fakeFrac_" +sAna + "_" + sSR + "_" + lep;
  _c0->SaveAs((fName+".png").c_str());

  */
}
//-------------------------------------------//
// Get FR/Eff -vs var for given inputs
// sel: trm,sel
// cr: CR0, 2LCR0->CR3,3LCR0->CR5
// type: hf,lf, cv
// lep: e/m
// var: pt, eta, met, metrel, dPhilmet, dPhijmet, nJets 
//-------------------------------------------// 
TH1F* DrawPlots::getFakeRate(string sample, string sel, string cr, 
			     string type, string lep, string var)
{
  TString SEL(sel);
  TString SAMPLE(sample);
  TString LEP(lep);

  string s;
  
  string loose = sel + "_" + cr + "_" + type + "_" + lep + "_loose_" + var;
  string tight = sel + "_" + cr + "_" + type + "_" + lep + "_tight_" + var;
  
  std::cout << "Using hist " << loose << std::endl;
  std::cout << "Using hist " << tight << std::endl;

  TH1F* _h_loose = NULL;
  TH1F* _h_tight = NULL;
  
  //Truth match FR
  if(SEL.Contains("trm") || (SEL.Contains("sel") && SAMPLE.Contains("bb"))){
    string _name = "/histo_" + sample + "_std.root";
    string _fileName = _pathHisto + _name;
    std::cout << "Loading " << _fileName.c_str() << std::endl;
    TFile* _f = new TFile(_fileName.c_str(),"READ",sample.c_str());
    _h_loose = (TH1F*) _f->Get(loose.c_str());
    _h_tight = (TH1F*) _f->Get(tight.c_str());
  }
  else if(SAMPLE.Contains("data")){
    //If sel & data - DO EWK bkg substraction Top, Wjets, Zjets
    TFile* _f_top   = new TFile((s=_pathHisto+string("/histo_top_PowHeg_rlep.root")).c_str(),"READ","top");
    TFile* _f_wjets = new TFile((s=_pathHisto+string("/histo_Wjets_Alpgen_rlep.root")).c_str(),"READ","Wjets");
    TFile* _f_zjets = new TFile((s=_pathHisto+string("/histo_Zjets_Alpgen_rlep.root")).c_str(),"READ","Zjets");
    TFile* _f_diB   = new TFile((s=_pathHisto+string("/histo_diBoson_Sherpa_rlep.root")).c_str(),"READ","diBoson");
    
    TFile* _f_data = NULL;
    if(LEP.Contains("e")){
      _f_data = new TFile((s=_pathHisto+string("/histo_data12_std.root")).c_str(),"READ","Egamma");
      std::cout << "Retreiving Egamma file" <<std::endl;
    }
    if(LEP.Contains("m")){
      _f_data = new TFile((s=_pathHisto+string("/histo_data12_std.root")).c_str(),"READ","Muons");
      std::cout << "Retreiving Muon file" <<std::endl;
    }
       
    TH1F* _h_bkg_loose;  TH1F* _h_bkg_tight;
    TH1F* _h_data_loose; TH1F* _h_data_tight;
    TH1F* _h_corr_loose; TH1F* _h_corr_tight;

    _f_top->cd();   _h_loose = (TH1F*) _f_top->Get(loose.c_str());    _h_bkg_loose = (TH1F*) _h_loose->Clone();
    _h_tight     = (TH1F*) _f_top->Get(tight.c_str());   _h_bkg_tight = (TH1F*) _h_tight->Clone();
    _f_wjets->cd(); _h_loose = (TH1F*) _f_wjets->Get(loose.c_str());  _h_bkg_loose->Add(_h_loose,1);
    _h_tight     = (TH1F*) _f_wjets->Get(tight.c_str()); _h_bkg_tight->Add(_h_tight,1);
    _f_zjets->cd(); _h_loose = (TH1F*) _f_zjets->Get(loose.c_str());  _h_bkg_loose->Add(_h_loose,1);
    _h_tight     = (TH1F*) _f_zjets->Get(tight.c_str()); _h_bkg_tight->Add(_h_tight,1);
    _f_diB->cd(); _h_loose = (TH1F*) _f_diB->Get(loose.c_str());  _h_bkg_loose->Add(_h_loose,1);
    _h_tight     = (TH1F*) _f_diB->Get(tight.c_str()); _h_bkg_tight->Add(_h_tight,1);

    _f_data->cd();
    _h_data_loose     = (TH1F*) _f_data->Get(loose.c_str());
    _h_data_tight     = (TH1F*) _f_data->Get(tight.c_str());

    _h_corr_loose = (TH1F*) _h_data_loose->Clone();    _h_corr_loose->Add(_h_bkg_loose,-1);
    _h_corr_tight = (TH1F*) _h_data_tight->Clone();    _h_corr_tight->Add(_h_bkg_tight,-1);
    
    TCanvas* _c1  = _utils->myCanvas("Data");
    _utils->myDraw1d(_h_data_loose,_c1,1,"e",false,kRed,false,20);
    _utils->myDraw1d(_h_data_tight,_c1,1,"esame",false,kBlue,false,20);

    TCanvas* _c2  = _utils->myCanvas("Bkg");
    _utils->myDraw1d(_h_bkg_loose,_c2,1,"e",false,kRed,false,20);
    _utils->myDraw1d(_h_bkg_tight,_c2,1,"esame",false,kBlue,false,20);

    TCanvas* _c3  = _utils->myCanvas("Data corr ");
    _utils->myDraw1d(_h_corr_loose,_c3,1,"e",false,kRed,false,20);
    _utils->myDraw1d(_h_corr_tight,_c3,1,"esame",false,kBlue,false,20);

    _h_loose = (TH1F*) _h_corr_loose->Clone();
    _h_tight = (TH1F*) _h_corr_tight->Clone();

  }

  //Compute FR
  TH1F* _h_FR=NULL;
  if(_h_loose && _h_tight){
    _h_FR = (TH1F*) _h_loose->Clone();
    _h_FR->Reset();
    _h_FR->GetYaxis()->SetTitle("Efficiency");
    _h_FR->Divide(_h_tight,_h_loose,1,1,"B");
  }

  return _h_FR;
}


//-------------------------------------------//
// DG2L cutflow Dump
//-------------------------------------------// 
void DrawPlots::dumpCutflow_DG2L(string sample)
{
  std::vector<string> sLEP;
  sLEP.clear();
  sLEP.push_back("EE");   
  sLEP.push_back("MM");   
  sLEP.push_back("EM");   

  std::vector<string> sSR;
  sSR.clear();
  sSR.push_back("DG2L_SR1_");
  sSR.push_back("DG2L_SR2_");
  sSR.push_back("DG2L_SR3_");
  sSR.push_back("DG2L_SR4_");

  std::vector<string> cSR[4];
  cSR[0].push_back("OS/SS Mll20");
  cSR[0].push_back("OS         ");
  cSR[0].push_back("Jet veto   ");
  cSR[0].push_back("Z veto     ");
  cSR[0].push_back("MetRel100  ");

  cSR[1].push_back("OS/SS Mll20");
  cSR[1].push_back("SS         ");
  cSR[1].push_back("Jet veto   ");
  cSR[1].push_back("MetRel100  ");

  cSR[2].push_back("OS/SS Mll20");
  cSR[2].push_back("SFOS       ");
  cSR[2].push_back(">=2 jets   ");
  cSR[2].push_back("Z veto     ");
  cSR[2].push_back("BJet veto  ");
  cSR[2].push_back("! topTagged");
  cSR[2].push_back("MetRel50   ");

  cSR[3].push_back("OS/SS Mll20");
  cSR[3].push_back("OS         ");
  cSR[3].push_back("Jet veto   ");
  cSR[3].push_back("Z veto     ");
  cSR[3].push_back("MetRel40   ");
  cSR[3].push_back("MT2        ");

  string _name = "/histo_" + sample + ".root";
  string _fileName = _pathHisto + _name;
  
  std::cout << "Loading " << _fileName.c_str() << std::endl;
  TFile* _f = new TFile(_fileName.c_str(),"READ",sample.c_str());

  for(uint j=0; j<sLEP.size(); j++){
    std::cout << ">>> Channel " << sLEP[j] << std::endl;
    for(uint i=0; i<sSR.size(); i++){
      string hName = sSR[i] + sLEP[j] + "_DG2L_cutflow";
      TH1F* _h = (TH1F*) _f->Get(hName.c_str());
      std::cout << "  Signal Region " << sSR[i] << endl; 
      for(uint k=0; k<cSR[i].size(); k++){
	float evt = _h->GetBinContent(k+1);
	std::cout << "   Cut: " << cSR[i][k] 
		  << "\t\t\t" << evt << std::endl;
      }
    }
  }

}

//-------------------------------------------//
// ML cutflow Dump
//-------------------------------------------// 
void DrawPlots::dumpCutflow_ML(string sample)
{



}

//-------------------------------------------//
// DG2L Bkg estimate
//-------------------------------------------// 
void DrawPlots:: bkgEstimate_DG2L()
{
  //Column
  std::vector<string> sLEPCOL;
  sLEPCOL.clear();
  sLEPCOL.push_back("$ee$");   
  sLEPCOL.push_back("$\\mu\\mu$");   
  sLEPCOL.push_back("$e\\mu$");   
  sLEPCOL.push_back("All");   

  std::vector<string> sLEP;
  sLEP.clear();
  sLEP.push_back("EE");   
  sLEP.push_back("MM");   
  sLEP.push_back("EM");   
  sLEP.push_back("ALL");   

  //Raws
  std::vector<string> sBKGCOL;
  sBKGCOL.clear();
  sBKGCOL.push_back("Higgs");
  sBKGCOL.push_back("Fake leptons");
  sBKGCOL.push_back("$Z \\to \\tau\\tau$");
  sBKGCOL.push_back("$Z(ee,\\mu\\mu)$+jets + ZV)");
  sBKGCOL.push_back("Top");
  sBKGCOL.push_back("WW");
  sBKGCOL.push_back("Total");

  std::vector<string> sBKG;
  sBKG.clear();
  sBKG.push_back("Higgs");
  sBKG.push_back("FAKE");
  sBKG.push_back("ZV");
  sBKG.push_back("Zjets");
  sBKG.push_back("TOP");
  sBKG.push_back("WW");
  sBKG.push_back("TOTAL");

  std::vector<string> sDATA;
  sDATA.clear();
  sDATA.push_back("Data");

  //SRs CRs NRs
  std::vector<string> sSRCR;
  sSRCR.clear();
  sSRCR.push_back("CR2LepOS");
  sSRCR.push_back("CR2LepOS40");
  sSRCR.push_back("CR2LepSS");
  sSRCR.push_back("CR2LepSS40");
  sSRCR.push_back("VR1SS");
  sSRCR.push_back("CRZ");
  sSRCR.push_back("NTOP");
  sSRCR.push_back("ZXCR1");
  sSRCR.push_back("ZXCR3");
  sSRCR.push_back("ZXCR4");
  sSRCR.push_back("ZXCR5");
  sSRCR.push_back("ZXCR6");
  sSRCR.push_back("ZXCR7");
  sSRCR.push_back("NWW1");
  sSRCR.push_back("NWW2");
  sSRCR.push_back("NWW3");
  sSRCR.push_back("preSRjveto");
  sSRCR.push_back("preSR2jets");
  sSRCR.push_back("preSRSSjveto");
  sSRCR.push_back("preSRmT2");
  sSRCR.push_back("SRjveto");
  sSRCR.push_back("SR2jets");
  sSRCR.push_back("SRSSjveto");
  sSRCR.push_back("SRmT2");
  sSRCR.push_back("SRmT2b");



  string fileName = _pathTables + "/BkgEst_DG2L.tex";
  std::ofstream tex(fileName.c_str());
  std::ostream & out = tex;
  if (!tex.is_open()){
    printf("Problem opening Acceptance table file .... bailing out \n %s \n",fileName.c_str());
    return;
  }
  else printf("Acceptance Table file opened: %s \n",fileName.c_str());


  //Loop over the SR  
  for(uint iSR=0; iSR<sSRCR.size(); iSR++){
    //    printf("DUMPING %s \n ",sSRCR[iSR].c_str());

    out << "\\begin{table}[htbp] "<< std::endl;
    out << "\\begin{center} "<< std::endl;

    string sHist;
    double val=0;
    double err=0;   

    //Matrix hist 
    const int NBKG=7; //MUST match sBKG size
    const int NLEP=4;
    TH1F* _hBkgLep[NBKG][NLEP];
    TH1F* _hDataLep[NLEP];
    for(uint ib=0; ib<sBKG.size(); ib++){
      for(uint ilep=0; ilep<sLEP.size(); ilep++){
	sHist = sBKG[ib] + "_DG2L_" + sSRCR[iSR] + "_" + sLEP[ilep] + "_DG2L_pred";
	//cout << "Bkg Hist name "  << sHist << endl;
	_hBkgLep[ib][ilep] = _utils->myTH1F(sHist.c_str(),sHist.c_str(),1,-0.5,0.5,"","");
	if(ib==0){
	  sHist = "DATA_DG2L_" + sSRCR[iSR] + "_" + sLEP[ilep] + "_DG2L_pred";
	  //cout << "Data Hist name "  << sHist << endl;
	  _hDataLep[ilep] = _utils->myTH1F(sHist.c_str(),sHist.c_str(),1,-0.5,0.5,"","");
	}
      }
    }
    
    //Do the sum over leptons & then bkgs
    for(uint ilep=0; ilep<sLEP.size()-1; ilep++){
      sHist = "DG2L_" + sSRCR[iSR] + "_" + sLEP[ilep] + "_DG2L_pred";
      grabHisto(sHist);//get all the bkg for this lep type
      _hDataLep[ilep]->Add(_dataH1);
      _hDataLep[sLEP.size()-1]->Add(_dataH1);
      for(uint ib=0; ib<sBKG.size()-1; ib++){
	_hBkgLep[ib][ilep]->Add(_mcH1[ib][DGSys_NOM]);
	_hBkgLep[sBKG.size()-1][ilep]->Add(_mcH1[ib][DGSys_NOM]);       //Sum over lep type
	_hBkgLep[ib][sLEP.size()-1]->Add(_mcH1[ib][DGSys_NOM]);         //Sum over bkg type
	_hBkgLep[sBKG.size()-1][sLEP.size()-1]->Add(_mcH1[ib][DGSys_NOM]);//Sum over all
      }
    }
    
    //Do the printout
    out << "\\begin{tabular}{|l|" ;
    for(uint ilep=0; ilep<sLEP.size(); ilep++) out << "r|";
    out << "}"<< std::endl;
    out << "\\hline "<< std::endl;
    //header
    out << std::setw(17) << " & ";
    for(uint ilep=0; ilep<sLEP.size(); ilep++){
      out << sLEPCOL[ilep];
      if(ilep<sLEP.size()-1) out << " & ";
    }
    out << "\\\\ \\hline" << std::endl;
    //bkg
    for(uint ib=0; ib<sBKG.size(); ib++){
      out << sBKGCOL[ib] << " & ";
      for(uint ilep=0; ilep<sLEP.size(); ilep++){
	val = _hBkgLep[ib][ilep]->IntegralAndError(0,-1,err);
	out << std::setprecision(2) << std::fixed << val << " $\\pm$ " << err;
	if(ilep<sLEP.size()-1) out << " & ";
      }
      if(ib<sBKG.size()-2) out << "\\\\" <<std::endl;
      else out << "\\\\ \\hline" <<std::endl;
    }
    
    //Data
    out << "Data " << " & ";
    for(uint ilep=0; ilep<sLEP.size(); ilep++){
      val = _hDataLep[ilep]->Integral(0,-1);
      out << val ;
      if(ilep<sLEP.size()-1) out << " & ";
    }
    out << "\\\\ \\hline" <<std::endl;
    out << "\\end{tabular} " << std::endl;

    string caption = "\\caption{Background estimate for " + sSRCR[iSR] + " }";
    out << caption << std::endl;

    out << "\\end{center}" << std::endl;
    out << "\\end{table}" << std::endl;
    out << std::endl;
    out << std::endl;
    
     out << "\\FloatBarrier" << std::endl;

  }//SR's 

  tex.close();
}
//-------------------------------------------//
// ML Bkg estimate
// TO UPDATE !!!!
//-------------------------------------------// 
void DrawPlots:: bkgEstimate_ML()
{
  //Column
  std::vector<string> sLEPCOL;
  sLEPCOL.clear();

  sLEPCOL.push_back("$eee$");   
  sLEPCOL.push_back("$\\mu\\mu\\mu$");   
  sLEPCOL.push_back("$ee\\mu$");   
  sLEPCOL.push_back("$\\mu\\mu e$");   
  sLEPCOL.push_back("All");   

  /*
  std::vector<string> sLEP;
  sLEP.clear();
  sLEP.push_back("EE");   
  sLEP.push_back("MM");   
  sLEP.push_back("EM");   
  sLEP.push_back("ALL");   
  */

  //Raws
  std::vector<string> sBKGCOL;
  sBKGCOL.clear();
  sBKGCOL.push_back("Top");
  sBKGCOL.push_back("diBoson");
  sBKGCOL.push_back("W+jets");
  sBKGCOL.push_back("Z+jets");
  sBKGCOL.push_back("Total");

  std::vector<string> sBKG;
  sBKG.clear();
  sBKG.push_back("TOP");
  sBKG.push_back("DIB");
  sBKG.push_back("WJETS");
  sBKG.push_back("ZJETS");
  sBKG.push_back("TOTAL");

  std::vector<string> sDATA;
  sDATA.clear();
  sDATA.push_back("Data");

  //SRs CRs NRs
  std::vector<string> sSRCR;
  sSRCR.clear();
  sSRCR.push_back("ML_SR3Lep");
  sSRCR.push_back("ML_SRB");
  sSRCR.push_back("ML_SR1a");
  sSRCR.push_back("ML_SR1b");
  sSRCR.push_back("ML_SR2");
  sSRCR.push_back("ML_VR0");
  sSRCR.push_back("ML_VR1");
  sSRCR.push_back("ML_VR2");
  sSRCR.push_back("ML_VR3");
  sSRCR.push_back("ML_VRWZ");
  sSRCR.push_back("ML_NRWZ");
      
  string fileName = _pathTables + "/BkgEst_ML.tex";
  std::ofstream tex(fileName.c_str());
  std::ostream & out = tex;
  if (!tex.is_open()){
    printf("Problem opening Acceptance table file .... bailing out \n %s \n",fileName.c_str());
    return;
  }
  else printf("Acceptance Table file opened: %s \n",fileName.c_str());


  //Loop over the SR  
  for(uint iSR=0; iSR<sSRCR.size(); iSR++){
    //    printf("DUMPING %s \n ",sSRCR[iSR].c_str());

    out << "\\begin{table}[htbp] "<< std::endl;
    out << "\\begin{center} "<< std::endl;

    string sHist;
    double val=0;
    double err=0;   

    //Martix hist 
    const int NBKG=5;
    TH1F* _hBkgLep[NBKG];
    TH1F* _hDataLep = NULL;
    for(uint ib=0; ib<sBKG.size(); ib++){
      sHist = sBKG[ib] + "_" + sSRCR[iSR] + "_ML_evtCatgUnOrdered";
      _hBkgLep[ib] = _utils->myTH1F(sHist.c_str(),sHist.c_str(),4,-0.5,3.5,"","");
      if(ib==0){
	sHist = "DATA_ML_" + sSRCR[iSR] + "_ML_evtCatgUnOrdered";
	_hDataLep = _utils->myTH1F(sHist.c_str(),sHist.c_str(),4,-0.5,3.5,"","");
      }
    }
    
    //Do the sum over leptons & then bkgs
    sHist = sSRCR[iSR] + "_ML_evtCatgUnOrdered";
    grabHisto(sHist);//get all the bkg for this lep type      
    _hDataLep->Add(_dataH1);
    for(uint ib=0; ib<sBKG.size()-1; ib++){
      _hBkgLep[ib]->Add(_mcH1[ib][DGSys_NOM]);
      _hBkgLep[sBKG.size()-1]->Add(_mcH1[ib][DGSys_NOM]);       //Sum over lep type
    }
    
    //Sum the evt categories into overflow bin
    for(uint ibin=0; ibin<sLEPCOL.size()-1; ibin++){
      _hDataLep->AddBinContent(_hDataLep->GetNbinsX()+1,_hDataLep->GetBinContent(ibin+1));
      for(uint ib=0; ib<sBKG.size(); ib++){
      _hBkgLep[ib]->AddBinContent(_hBkgLep[ib]->GetNbinsX()+1,_hBkgLep[ib]->GetBinContent(ibin+1));
      float err = _hBkgLep[ib]->GetBinError(_hBkgLep[ib]->GetNbinsX()+1) +
	pow(_hBkgLep[ib]->GetBinError(ibin+1),2);
      _hBkgLep[ib]->SetBinError(_hBkgLep[ib]->GetNbinsX()+1,err);
      }
    }
    //sqrt error
    for(uint ib=0; ib<sBKG.size(); ib++){
      float err = _hBkgLep[ib]->GetBinError(_hBkgLep[ib]->GetNbinsX()+1);
      _hBkgLep[ib]->SetBinError(_hBkgLep[ib]->GetNbinsX()+1,sqrt(err));
    }
    
    
    //Do the printout
    out << "\\begin{tabular}{|l|" ;
    for(uint ilep=0; ilep<sLEPCOL.size(); ilep++) out << "r|";
    out << "}"<< std::endl;
    out << "\\hline "<< std::endl;
    //header
    out << std::setw(17) << " & ";
    for(uint ilep=0; ilep<sLEPCOL.size(); ilep++){
      out << sLEPCOL[ilep];
      if(ilep<sLEPCOL.size()-1) out << " & ";
    }
    out << "\\\\ \\hline" << std::endl;
    //bkg
    for(uint ib=0; ib<sBKG.size(); ib++){
      out << sBKGCOL[ib] << " & ";
      for(uint ilep=0; ilep<sLEPCOL.size(); ilep++){
	val = _hBkgLep[ib]->GetBinContent(ilep+1);
	err = _hBkgLep[ib]->GetBinError(ilep+1);
	out << std::setprecision(2) << std::fixed << val << " $\\pm$ " << err;
	if(ilep<sLEPCOL.size()-1) out << " & ";
      }
      if(ib<sBKG.size()-2) out << "\\\\" <<std::endl;
      else out << "\\\\ \\hline" <<std::endl;
    }
    
    //Data
    out << "Data " << " & ";
    for(uint ilep=0; ilep<sLEPCOL.size(); ilep++){
      val = _hDataLep->GetBinContent(ilep+1);
      //err = _hDataLep->GetBinError(ilep+1);
      out << val ;
      if(ilep<sLEPCOL.size()-1) out << " & ";
    }
    out << "\\\\ \\hline" <<std::endl;
    out << "\\end{tabular} " << std::endl;

    string caption = "\\caption{Background estimate for " + sSRCR[iSR] + " }";
    out << caption << std::endl;

    out << "\\end{center}" << std::endl;
    out << "\\end{table}" << std::endl;
    out << std::endl;
    out << std::endl;
    


  }//SR's 

  tex.close();
}
