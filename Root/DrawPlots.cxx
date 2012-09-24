#include "SusyWeakProdAna/DrawPlots.h"
#include "SusyWeakProdAna/SusyAnaCommon.h"


#include "TMath.h"

typedef unsigned uint;
//-----------------------------------------------------------------------------
DrawPlots::DrawPlots(){

  _pathHisto  =  string(getenv("WORKAREA")) + "/histoAna" + "/SusyAna/histos_" + DATE;
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

  _mcStack = new THStack("tmpStack","tmpStack");

}

//-------------------------------------------//
// Open histo files
//-------------------------------------------//
void DrawPlots::openHistoFiles(string method, 
			       string Top, string WW, string ZX,
			       string ZJets, string Fake)
{
  std::cout << "loading histo from method " << method << endl;

  _dataFileName = _pathHisto + "/histo_data12_" + method + ".root";
  
  std::cout << "Loading data " << std::endl;
  _dataFile = new TFile(_dataFileName.c_str(),"READ",SFILE[5]);

  _mcFileName.clear();
  _mcFile.clear();
  _mcFileName.push_back(string(_pathHisto +"/" + Top + "_" + method + ".root").c_str());
  _mcFileName.push_back(string(_pathHisto +"/" + WW + "_" + method + ".root").c_str());
  _mcFileName.push_back(string(_pathHisto +"/" + ZX + "_" + method + ".root").c_str());
  _mcFileName.push_back(string(_pathHisto +"/" + ZJets + "_" + method + ".root").c_str());
  _mcFileName.push_back(string(_pathHisto +"/" + Fake + "_" + method + ".root").c_str());

  for(uint i=0; i<_mcFileName.size(); i++){
    std::cout << "Loading " << SFILE[i] << " " << _mcFileName[i].c_str() << std::endl;
    TFile* _f = new TFile(_mcFileName[i].c_str(),"READ",SFILE[i]);
    _mcFile.push_back(_f);
  }

  _sigFileName.clear();
  _sigFile.clear();
  _sigFileName.push_back(string(_pathHisto +"/histo_Herwigpp_simplifiedModel_wA_slep_noWcascade_10.144876.root").c_str());
  _sigFileName.push_back(string(_pathHisto +"/histo_Herwigpp_simplifiedModel_wA_slep_noWcascade_18.144884.root").c_str());
  
  //  _sigFileName.push_back(string(_pathHisto +"/histo_SimplifiedModel_wA_slep.root").c_str());
  //  _sigFileName.push_back(string(_pathHisto +"/histo_SimplifiedModel_wC_slep.root").c_str());
  for(uint i=0; i<_sigFileName.size(); i++){
    std::cout << "Loading " << SIGFILE[i] << std::endl;
    TFile* _f = new TFile(_sigFileName[i].c_str(),"READ",SIGFILE[i]);
    _sigFile.push_back(_f);
  }


}
//-------------------------------------------//
// Grab TH1 histo from files
//-------------------------------------------//
void DrawPlots::grabHisto(string name, bool quiet)
{
  _mcH1.clear();
  _mcName.clear();
  _mcColor.clear();
  _mcMarker.clear();
  string title;
  TH1F* _h;
  TH1F* _tmp;

  _dataFile->cd();
  title = "DATA_"+name;
  _h = (TH1F*) _dataFile->Get(name.c_str());
  if(_h==NULL){
    cerr <<" Could not find histo " << name << " in data file " << _dataFile->GetName() << endl;
    abort();
  }
  if(_moveUO) _utils->moveUnderOverFlow(_h);
  _dataH1 = (TH1F*) _h->Clone(title.c_str());
  
  _tmp = (TH1F*) _dataH1->Clone();
  _tmp->Reset();


  int nLoad=0;
  for(uint i=0; i<_mcFile.size(); i++){
    TFile* _f = _mcFile[i];
    _f->cd();
    _h = (TH1F*) _f->Get(name.c_str());
    if(_h==NULL){ //put an empty histo
      //cerr <<" Could not find histo " << name << " in MC file " << _f->GetName() << " using empty histo" << endl;
      _h = (TH1F*) _tmp->Clone();
    }
    if(_moveUO) _utils->moveUnderOverFlow(_h);
    switch (i){
    case TOP:
      _mcColor.push_back(C_TOP);
      break;
    case WW:
      _mcColor.push_back(C_WW);
      break;
    case ZX:
      _mcColor.push_back(C_ZX);
      break;
    case ZJETS:
      _mcColor.push_back(C_ZJETS);
      break;
    case FAKE:
      _mcColor.push_back(C_FAKE);
      break;
    }
    
    _mcName.push_back(SFILE[i]);
    _mcMarker.push_back(iMarker[i+1]);
    title = string(SFILE[i]) + "_" + name;
    
    _h->SetTitle(title.c_str());
    _h->SetName(title.c_str());
    //std::cout << "MC " << _h->GetName() << " entries " << _h->Integral(0,-1) <<std::endl;
    _mcH1.push_back(_h);
    nLoad++;
  }
   

  for(uint i=0; i<_sigFile.size(); i++){
    TFile* _f = _sigFile[i];
    _f->cd();
    _h = (TH1F*) _f->Get(name.c_str());
    if(_h==NULL){ //put an empty histo
      //cerr <<" Could not find histo " << name << " in MC file " << _f->GetName() << " using empty histo" << endl;
      _h = (TH1F*) _tmp->Clone();
    }
    if(_moveUO) _utils->moveUnderOverFlow(_h);
    switch (i){
    case modeANoSlep_15:
      _sigColor.push_back(C_SIG1);
      _sigName.push_back(SIGFILE[i]);
      title = "SIG_"+name;
      break;
    case modeANoSlep_18:
      _sigColor.push_back(C_SIG2);
      _sigName.push_back(SIGFILE[i]);
      title = "SIG_"+name;
      break;
    }
    _h->SetTitle(title.c_str());
    _h->SetName(title.c_str());
    _h->SetFillStyle(0);
    _h->SetLineStyle(5);
    if(!quiet)
      std::cout << "SIG " << _h->GetName() 
		<< " " << _h->Integral(0,-1) <<std::endl;
    _sigH1.push_back(_h);
    nLoad++;
  }

}

//-------------------------------------------//
// Open histo files
//-------------------------------------------//
void DrawPlots::buildStack(string name)
{
  _mcStackH = (TH1F*) _mcH1[0]->Clone();
  _mcStackH->Reset();
  _mcStackH->SetTitle(string("Stack_"+name).c_str());
  _mcStackH->SetName(string("Stack_"+name).c_str());

  //  _mcStack = new THStack(name.c_str(),name.c_str());
  _mcStack->Clear();
  _mcStack->SetName(name.c_str());
  _mcStack->SetTitle(name.c_str());

  for(uint i=0; i<_mcH1.size(); i++){
    std::cout << _mcH1[i]->GetName() << " \t Int " 
	      << _mcH1[i]->Integral(0,-1) << std::endl;
    
    _utils->addToTHStack(_mcStack,_mcH1[i],_mcColor[i], 
			 "HIST", _leg, _mcName[i].c_str());
    _mcStackH->Add(_mcH1[i],1);
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

  _leg = new TLegend(0.6,0.5,0.75,0.85);
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
    TH1F* _tmpH1 = (TH1F*) _mcH1[i]->Clone();
    if(_tmpH1==NULL) {
      cerr << "Cannot find MC histo " << i << endl;
      abort();
    }
    if(norm) _utils->normalize(_tmpH1,1);
    _tmp_mcH1.push_back(_tmpH1);
    _leg->AddEntry(_tmpH1,_mcName[i].c_str(),"p");

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

  _leg = new TLegend(0.55,0.45,0.85,0.93);
  _utils->legendSetting(_leg); 

  //Grabs all histos: data, MC, signal points
  grabHisto(name,false);

  //Build the mc stack and retrieve histo of the total. Add entry to legend
  buildStack(name);

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
    _leg->AddEntry(_sigH1[i],SIGFILE[i],"l");
    _c0->Update();
    _pTop->Update();
  }

  float avgRatio = 0;
  if(_stackH->Integral(0,-1)>0) avgRatio =_dataH1->Integral(0,-1) / _stackH->Integral(0,-1);
  std::cout << "Average ratio data/MC " << avgRatio << std::endl;
  
  _c0->cd(1);
  _pTop->cd();
  drawChannelText(name,0.75,0.85);
  drawLumi();
  drawATLAS();
  _c0->Update();

  string fName= _pathPlots + "/" + "pred_" + name + _sLogy;
  //  _c0->SaveAs((fName+".pdf").c_str());
  _c0->SaveAs((fName+".png").c_str());




}
//-------------------------------------------//
// Draw channel labelx
//-------------------------------------------//
void DrawPlots::drawChannelText(string name, float x, float y)
{
  string _text;
 
  TString hName(name);
  if(hName.Contains("DG2L_")){

    if(hName.Contains("SRjveto"))        _text = "SRjveto ";
    else if(hName.Contains("SRSSjveto")) _text = "SRSSjveto ";
    else if(hName.Contains("SR2jets"))   _text = "SR2jets ";
    else if(hName.Contains("SRmT2b"))    _text = "SRmT2b ";
    else if(hName.Contains("SRmT2"))     _text = "SRmT2 ";
    else if(hName.Contains("CRZ"))       _text = "CRZ ";
    else if(hName.Contains("NTOP"))      _text = "NTOP ";
    else if(hName.Contains("NWW1"))      _text = "NWW1 ";
    else if(hName.Contains("NWW2"))      _text = "NWW2 ";
    else if(hName.Contains("NWW3"))      _text = "NWW3 ";
    else if(hName.Contains("ZXCR1"))     _text = "ZXCR1 ";
    else if(hName.Contains("ZXCR3"))     _text = "ZXCR3 ";
    else if(hName.Contains("ZXCR4"))     _text = "ZXCR4 ";
    else if(hName.Contains("CR2LepOS"))  _text = "CR2LepOS ";
    else if(hName.Contains("CR2LepSS"))  _text = "CR2LepSS ";
    
    if(hName.Contains("EE")) _text += "ee";
    else if(hName.Contains("MM")) _text += "#mu#mu";
    else if(hName.Contains("EM")) _text += "e#mu";
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
  _utils->myText(x,y,kBlack,_text.c_str(),0.05);
}
//-------------------------------------------//
// Draw Lumi
//-------------------------------------------//
void DrawPlots::drawLumi(float x, float y)
{
  char s[20] ="";
  sprintf(s,"%4.2f",pLUMI);
  string _text = "#int L dt=" + string(s) + "fb^{-1}  #sqrt{s}=8 TeV";
  _utils->myText(x,y,kBlack,_text.c_str(),0.05);
}
//-------------------------------------------//
// Draw ATLAS label
//-------------------------------------------//
void DrawPlots::drawATLAS(float x, float y)
{
  string _text = "ATLAS Work In Progress";
  _utils->myText(x,y,kBlack,_text.c_str(),0.05);
  
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
TH1F* DrawPlots::calcRatio(TH1F* hnum, TH1F* hden, string name)
{
  //Compute FR
  TH1F* _h_R = (TH1F*) hnum->Clone();
  _h_R->Reset();
  _h_R->SetName(name.c_str()); 
  _h_R->SetTitle(name.c_str());
  _h_R->GetYaxis()->SetTitle("Efficiency");
  _h_R->Divide(hnum,hden,1,1,"B");

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
	string ss = string(SFILE[imc]) + "_fkFrac";
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
  /*
  TCanvas* _c1  = _utils->myCanvas("dummy",800,500,3,2);
  for(uint imc=0; imc<_hFrac.size(); imc++){
    _c1->cd(imc+1);
    _hFrac[imc]->Draw("e");
  }
  string fName1= _pathPlots + "/" + "tmp_fakeFrac";
  _c1->SaveAs((fName1+".png").c_str());  
  */

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
  if(SEL.Contains("trm") || (SEL.Contains("sel") && SAMPLE.Contains("BB"))){
    string _name = "/histo_" + sample + ".root";
    string _fileName = _pathHisto + _name;
    std::cout << "Loading " << _fileName.c_str() << std::endl;
    TFile* _f = new TFile(_fileName.c_str(),"READ",sample.c_str());
    _h_loose = (TH1F*) _f->Get(loose.c_str());
    _h_tight = (TH1F*) _f->Get(tight.c_str());
  }
  else if(SAMPLE.Contains("DATA")){
    //If sel & data - DO EWK bkg substraction Top, Wjets, Zjets
    TFile* _f_top   = new TFile((s=_pathHisto+string("/histo_top.root")).c_str(),"READ","top");
    TFile* _f_wjets = new TFile((s=_pathHisto+string("/histo_Wjets.root")).c_str(),"READ","Wjets");
    TFile* _f_zjets = new TFile((s=_pathHisto+string("/histo_Zjets.root")).c_str(),"READ","Zjets");
    TFile* _f_diB   = new TFile((s=_pathHisto+string("/histo_diBoson.root")).c_str(),"READ","diBoson");
    
    TFile* _f_data = NULL;
    if(LEP.Contains("e")){
      _f_data = new TFile((s=_pathHisto+string("/histo_data12.root")).c_str(),"READ","Egamma");
      std::cout << "Retreiving Egamma file" <<std::endl;
    }
    if(LEP.Contains("m")){
      _f_data = new TFile((s=_pathHisto+string("/histo_data12.root")).c_str(),"READ","Muons");
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
  TH1F* _h_FR = (TH1F*) _h_loose->Clone();
  _h_FR->Reset();
  _h_FR->GetYaxis()->SetTitle("Efficiency");
  _h_FR->Divide(_h_tight,_h_loose,1,1,"B");

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
  sBKGCOL.push_back("Top");
  sBKGCOL.push_back("WW");
  sBKGCOL.push_back("ZX");
  sBKGCOL.push_back("Z+jets");
  sBKGCOL.push_back("Fake");
  sBKGCOL.push_back("Total");

  std::vector<string> sBKG;
  sBKG.clear();
  sBKG.push_back("TOP");
  sBKG.push_back("WW");
  sBKG.push_back("ZX");
  sBKG.push_back("ZJETS");
  sBKG.push_back("FAKE");
  sBKG.push_back("TOTAL");

  std::vector<string> sDATA;
  sDATA.clear();
  sDATA.push_back("Data");

  //SRs CRs NRs
  std::vector<string> sSRCR;
  sSRCR.clear();
  sSRCR.push_back("CR2LepOS");
  sSRCR.push_back("CR2LepSS");
  sSRCR.push_back("CRZ");
  sSRCR.push_back("NTOP");
  sSRCR.push_back("NWW1");
  sSRCR.push_back("NWW2");
  sSRCR.push_back("NWW3");
  sSRCR.push_back("SRjveto");
  sSRCR.push_back("SR2jets");
  sSRCR.push_back("SRSSjveto");
  sSRCR.push_back("SRmT2");
  sSRCR.push_back("SRmT2b");
  sSRCR.push_back("ZXCR1");
  sSRCR.push_back("ZXCR3");
  sSRCR.push_back("ZXCR4");


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
    const int NBKG=7; //MUST mathc sBKG size
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
	_hBkgLep[ib][ilep]->Add(_mcH1[ib]);
	_hBkgLep[sBKG.size()-1][ilep]->Add(_mcH1[ib]);       //Sum over lep type
	_hBkgLep[ib][sLEP.size()-1]->Add(_mcH1[ib]);         //Sum over bkg type
	_hBkgLep[sBKG.size()-1][sLEP.size()-1]->Add(_mcH1[ib]);//Sum over all
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
    


  }//SR's 

  tex.close();
}
//-------------------------------------------//
// ML Bkg estimate
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
      _hBkgLep[ib]->Add(_mcH1[ib]);
      _hBkgLep[sBKG.size()-1]->Add(_mcH1[ib]);       //Sum over lep type
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
