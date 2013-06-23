#include "SusyWeakProdAna/SROptimization.h"

#include "SusyWeakProdAna/SusyAnaCommon.h"


//--------------------------------------------------------//
// Constructor
//--------------------------------------------------------//
SROptimization::SROptimization(RegionOption option, SusyProcess sp, 
			       string skim, string dilType):
  m_dbg(0),
  m_bkgFile(NULL),
  m_var(""),
  m_selection(""),
  m_process(""),
  m_xtitle(""),
  m_ytitle("")
{
  _utils = new TGuiUtils();
  gROOT->SetStyle("ATLAS");
  gStyle->SetPaintTextFormat(".0f");
  _utils->atlasStyle->SetOptStat("emr");
  _utils->atlasStyle->SetOptStat("");
  
  m_sigCalc= new SignificanceCalculator();

  m_skim = skim;
  m_lep = dilType;

  cout << "------------------------" << endl;
  cout << " Skim : " << RegionNames[option] << endl;
  cout << " DilType: " << dilType << endl;
  cout << " Susy Grid: " << SPNames[sp] << endl;
  cout << "------------------------" << endl;


  m_dirInput =  string(getenv("WORKAREA")) 
    + "/SusyWeakProdAna/scripts/inputHistoList/";
  m_dirHist =  string(getenv("HISTOANA")) + "/ToyAna/"+ m_skim +"/";

  m_process = SPNames[sp];
  m_procId  = sp;
  if( sp == SP_slepton ){
    cout << "Loading Slepton grid" << endl;
    loadSleptonFiles();
    m_xtitle = "m(#tilde{l}) [GeV]";
    m_ytitle = "m(#chi^{0}_{1}) [GeV]";
  }
  else if( sp == SP_modeC ){
    cout << "Loading mode C grid" << endl;
    loadModeCFiles();   
    m_xtitle = "m(#chi^{#pm}_{1}) [GeV]";
    m_ytitle = "m(#chi^{0}_{1}) [GeV]";
  }
  else if( sp == SP_modeA_slep ){
    cout << "Loading mode A w/ slepton grid" << endl;
    loadModeAFiles(true);   
    m_xtitle = "m(#chi^{0}_{2}) [GeV]";
    m_ytitle = "m(#chi^{0}_{1}) [GeV]";
  }
  else if( sp == SP_modeA_noslep_hadW ){
    cout << "Loading mode A no slepton W hadronic grid" << endl;
    loadModeAFiles();   
    m_xtitle = "m(#chi^{0}_{2}) [GeV]";
    m_ytitle = "m(#chi^{0}_{1}) [GeV]";
  }
 
  _mcHistFileName.push_back("histo_higgs.root");
  _mcHistFileName.push_back("histo_fake.root");
  _mcHistFileName.push_back("histo_ZV.root");
  _mcHistFileName.push_back("histo_Zjets.root");
  _mcHistFileName.push_back("histo_top.root");
  _mcHistFileName.push_back("histo_WW.root");
  _mcHistFileName.push_back("histo_allBkg.root");

  // Specify the run option
  m_var = "yield";
  m_selection = RegionNames[option];
  cout << "Using selection " << m_selection << endl;
    

}

//--------------------------------------------------------//
// Destructor
//--------------------------------------------------------//
SROptimization::~SROptimization()
{

}
//--------------------------------------------------------//
// Initialize some things
//--------------------------------------------------------//
void SROptimization::init()
{
  m_histName = m_var + "_" + m_lep + "_" + m_selection;
 
}
/*--------------------------------------------------------------------------------*/
// Convert int to string
/*--------------------------------------------------------------------------------*/
string SROptimization::itos(int i){
  string s;
  std::stringstream out;
  out << i;
  s = out.str();
  return s;
}
//--------------------------------------------------------//
// Load Files
//--------------------------------------------------------//
void SROptimization::loadSleptonFiles()
{

  // This relies on having the input file in 
  // the inputHistoList directory
  string file = "slepton.txt";
  string path =  m_dirInput + file;
  ifstream input(path.c_str(), ifstream::in);

  string line = "";
  string previous = "";
  while(input.good()){
    
    input >> line;
    if( previous == line ) break;

    // Save filename
    string fname = line;

    // Get dsid
    int loc_run = line.rfind(".");
    int run     = atoi(line.substr(loc_run+1, line.size()).c_str());
    line        = line.substr(0,loc_run);

    // Get Neutralino Mass
    int loc_neut = line.rfind("_");
    float neut   = atof(line.substr(loc_neut+1, line.size()).c_str());
    line         = line.substr(0,loc_neut);
    line         = line.substr(0, line.rfind("_"));
    previous = line;

    // Chargino Mass
    int loc_slep = line.rfind("_");
    float slep   = atof(line.substr(loc_slep+1, line.size()).c_str());
    
    m_susyFiles.push_back( sl_File(run, slep, neut, fname) );
    
    if(m_dbg>5) cout << "DSID " << run << "slepton mass " << slep 
		     << " LSP mass " << neut << endl;
    
  }

}
//--------------------------------------------------------//
void SROptimization::loadModeCFiles(bool wSlepton)
{
  // This relies on having the input file in 
  // the inputHistoList directory
  string file;
  if(wSlepton) file = "wC_slep.txt";
  else         file = "wC_noslep.txt";

  string path =  m_dirInput + file;
  
  ifstream input(path.c_str(), ifstream::in);

  // Will need to use the susyXS object to get the
  // necessary masses
  XSReader* susyXS = new XSReader();
  if(m_dbg>5) susyXS->setDebug(m_dbg);
  susyXS->LoadXSInfo();

  string line = "";
  string previous = "";
  while(input.good()){
    
    input >> line;

    if( previous == line ) break;

    // Save filename
    string fname = line;
    
    // Get dataest Id
    int loc_run = line.rfind(".");
    int run     = atoi(line.substr(loc_run+1, line.size()).c_str());
    line        = line.substr(0,loc_run);
    previous = line;
	
    // Now use the tool to get the 
    // neutralino and chargino mass
    float neut  = susyXS->GetMN1(run);
    float charg = susyXS->GetMC1MN2(run);
    float xs    = susyXS->GetXS(run);
    float xsSys = susyXS->GetXSSys(run);

    if(m_dbg>5) cout << "DSID " << run << " chargino mass " << charg 
		     << " LSP mass " << neut << endl;
    
    m_susyFiles.push_back( sl_File(run, charg, neut, fname, xs, xsSys) );
  }

  delete susyXS;


}
//--------------------------------------------------------//
void SROptimization::loadModeAFiles(bool wSlepton)
{
  // This relies on having the input file in 
  // the inputHistoList directory
  string file;
  if(wSlepton) file = "wA_slep.txt";
  else         file = "wA_noslep_hadW.txt";

  string path =  m_dirInput + file;
  ifstream input(path.c_str(), ifstream::in);

  // Will need to use the susyXS object to get the
  // necessary masses
  XSReader* susyXS = new XSReader();
  if(m_dbg>5) susyXS->setDebug(true);
  susyXS->LoadXSInfo();

  string line = "";
  string previous = "";
  while(input.good()){
    
    input >> line;
    
    if( previous == line ) break;

    // Save filename
    string fname = line;
    
    // Get dataest Id
    int loc_run = line.rfind(".");
    int run     = atoi(line.substr(loc_run+1, line.size()).c_str());
    line        = line.substr(0,loc_run);
    previous = line;
	
    // Now use the tool to get the 
    // neutralino and chargino mass
    float neut  = susyXS->GetMN1(run);
    float charg = susyXS->GetMC1MN2(run);
    float xs    = susyXS->GetXS(run);
    float xsSys = susyXS->GetXSSys(run);

    if(m_dbg >5) cout << "DSID " << run << " chargino mass " << charg 
		      << " LSP mass " << neut << endl;
    
    m_susyFiles.push_back( sl_File(run, charg, neut, fname, xs, xsSys) );
 
  }

  delete susyXS;

}

//--------------------------------------------------------//
// Load Bkg histo
//--------------------------------------------------------//
void SROptimization::loadBkgHistos()
{
  cout << "Grabbing Bkg histograms for var " << m_var 
       << " selection " << m_selection  << endl; 
  
  TH1F* _hTemp = NULL;
  TH1F* _h = NULL;
  for(uint i=0; i<N_MC; i++){
    cout << "\tLoading " <<_mcHistFileName[i] << endl;
    TFile* _f = new TFile( (m_dirHist + _mcHistFileName[i]).c_str() );
    if(_f->IsOpen()==kFALSE){
      //Assume  we have the first file & histo!
      _h = (TH1F*) _hTemp->Clone();
      for(int ilep=0; ilep<N_LEP; ilep++){
	string hName = m_var + "_" + LepN[ilep] + "_" + m_selection;
	_mcH1[ilep].push_back((TH1F*) _h->Clone());
      }
    }
    else{
      for(int ilep=0; ilep<N_LEP; ilep++){
	string hName = m_var + "_" + LepN[ilep] + "_" + m_selection;
	_h = (TH1F*) _f->Get(hName.c_str());
	if(i==0 && ilep==0){
	  _hTemp = (TH1F*) _h->Clone();
	  _hTemp->Reset();
	}
	_mcH1[ilep].push_back((TH1F*) _h->Clone());
      }
    }
  }
}

//--------------------------------------------------------//
// Get Contour Graph
//--------------------------------------------------------//
TGraph* SROptimization::getContourGraph(TGraph2D* signif)
{
  TGraph* gr0    = new TGraph();
  TH2D* internal = (TH2D*) (signif->GetHistogram()->Clone("hist_sig"));
  TGraph* gr1    = (TGraph*) gr0->Clone("temp");
  
  internal->SetContour(1);
  double pval = 0.05;
  double significance = TMath::NormQuantile(1-pval);
  internal->SetContourLevel(0, significance);
  internal->Draw("CONT LIST");
  internal->SetDirectory(0);
  gPad->Update();

  TObjArray *contours = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  TList* list = (TList*) contours->At(0);
  if( list->GetSize() == 0 ) return gr1;

  gr1 = (TGraph*) list->First();
  gr1->SetTitle("ContourExcl");
  gr1->SetName("ContourExcl");
  
  return gr1;
}
//--------------------------------------------------------//
// Plot Significance
//--------------------------------------------------------//
TGraph2D* SROptimization::buildSigCut(vector<sl_File> files, 
				      float err,
				      TH1F* bkg, 
				      string name,
				      float cut, float upper)
{
  TGraph2D* signif = NULL;

  if(bkg==NULL) {
    cout << "No Bkg histo " << endl;
    exit(0);
  }

  // Get Lower Bound and the signal value
  int lowerBound = bkg->FindBin(cut);
  int upperBound = upper < 0 ? upper : bkg->FindBin(upper);
  Double_t bkg_statErr =0;
  float n_bkg    = bkg->IntegralAndError(lowerBound, upperBound,bkg_statErr);
  if(m_dbg> 2) cout << "Bin low: " << lowerBound << " " << bkg->GetBinLowEdge(lowerBound)
		    << " - high " << upperBound << " " <<  bkg->GetBinLowEdge(upperBound) << endl;

  double bkg_fracErr =  getStatError(bkg, lowerBound, upperBound)/n_bkg;
  double error   = sqrt(pow(bkg_fracErr,2)+pow(SYS_ERR,2)); 
  if(m_dbg>0) cout << "Bkg: " << n_bkg
		   <<" Sys:  "  << SYS_ERR 
		   <<" bkg: "   << bkg_fracErr 
		   <<" Total: " << error << endl << endl;
  
  // Counters for signal points
  const int NSignalPts=500;
  double MN1_pts[NSignalPts];
  double MSP_pts[NSignalPts];
  double Sig_pts[NSignalPts];

  
  //Fudge
  MN1_pts[0] = 0;
  MSP_pts[0] = 0;
  Sig_pts[0] = 2;

  int npts = 1;
  // Loop over the files:
  for(uint f=0; f<files.size(); ++f){
    sl_File* file = &files.at(f);
    double mN     = file->getNeutralinoMass();
    double mSpart = file->getSusyMass();

    // Load signal file, get n_sig
    string s_DSID = itos(file->getId());
    string hFName = "histo_" + m_process + "_" + s_DSID + ".root";

    double zn = 0.;
    double n_sig =0.;
    TFile*    f_sig  = new TFile((m_dirHist + hFName).c_str());
    if(f_sig->IsOpen()){
      TH1F* signal = (TH1F*) f_sig->Get(m_histName.c_str());
      n_sig   = signal->Integral(lowerBound,upperBound);
      // Get Weight
      zn = m_sigCalc->Zn(n_sig,n_bkg,error);
      if( zn < 0 ) zn = 0.001;
      signal->Delete();  

      f_sig->Close();
      f_sig->Delete();
    }
    if(m_dbg>0) cout << "Signal point " << s_DSID 
		     << " [" << mSpart << "," << mN <<"]"
		     << " Sig " << n_sig << " ZN " << zn << endl;
    
    // Save:
    MN1_pts[f+1] = mN;
    MSP_pts[f+1] = mSpart;
    Sig_pts[f+1] = zn;
    npts++;
  }

  
  //  signif = new TGraph2D(npts-1, MSP_pts, MN1_pts, Sig_pts);
  signif = new TGraph2D(npts, MSP_pts, MN1_pts, Sig_pts);
  signif->SetTitle("Zn");
  signif->SetName("Zn");
  

  return signif;

}
//--------------------------------------------------------//
// Get Numbers per grid points
//--------------------------------------------------------//

TH2D* SROptimization::buildNumbers(vector<sl_File> files,
				   string name,
				   int nbinsx, float minx, float maxx,
				   int nbinsy, float miny, float maxy,
				   float cut, float upper)
{
  TFile* temp = NULL;
  TH1F*  hist = NULL;

  TH2D* hist2 = new TH2D((name+"_SignalYield").c_str(),
			 "",nbinsx,minx,maxx,
			 nbinsy,miny,maxy);
  
  for(uint i=0; i<files.size(); ++i){
    sl_File* file = &files.at(i);
    double mN     = file->getNeutralinoMass();
    double mSpart = file->getSusyMass();

    // Load signal file, get n_sig
    string s_DSID = itos(file->getId());
    string hFName = "histo_" + m_process + "_" + s_DSID + ".root";
    temp  = new TFile((m_dirHist + hFName).c_str());
    if(temp->IsOpen()==kFALSE) continue;

    hist = (TH1F*) temp->Get(name.c_str());
    int lowerBound = hist->FindBin(cut);
    int upperBound = upper < 0 ? upper : hist->FindBin(upper);
    double n_sig   = hist->Integral(lowerBound, upperBound);
    
    hist2->Fill(mSpart, mN, n_sig);

    hist->Delete();
    hist = NULL;
    temp->Close();
  }
  
  return hist2;
}

//--------------------------------------------------------//
// Get the stat error
//--------------------------------------------------------//
double SROptimization::getStatError(TH1* hist, int lowBin, int highBin)
{
  double stat = 0;
  int max = highBin < 0 ? hist->GetNbinsX() : highBin;
  
  for(int bin=lowBin; bin<=max; ++bin){
    double be = hist->GetBinError(bin);
    //cout<<"\tbin: "<<hist->GetBinLowEdge(bin)<<" err: "<<be<<" "<<be/hist->GetBinContent(bin)<<endl;
    stat += be*be;
  }
  return sqrt(stat);
};

//--------------------------------------------------------//
// Plot Significance
//--------------------------------------------------------//
void SROptimization::PlotSig()
{

  gStyle->SetPaintTextFormat(".0f");

  loadBkgHistos();
  TH1F* _histoSM = NULL; 
  if(m_lep=="ALL") _histoSM = _mcH1[ALL][ALLBKG];
  if(m_lep=="EE")  _histoSM = _mcH1[EE][ALLBKG];
  if(m_lep=="MM")  _histoSM = _mcH1[MM][ALLBKG];
  if(m_lep=="EM")  _histoSM = _mcH1[EM][ALLBKG];

  cout << "Computing significance with " << _histoSM->GetTitle() 
       << " total: " << _histoSM->Integral(0,-1) << endl;

  
  TCanvas* _c0  = _utils->myCanvas("Significance");
  
  // Get Significance
  TGraph2D* signif = buildSigCut(m_susyFiles, 
				 SYS_ERR, _histoSM, m_histName);
  TGraph* contour = getContourGraph(signif);
  TH2D* numbers = buildNumbers(m_susyFiles, 
			       m_histName,
			       1200, 0, 400, 600, 0, 200);

  // Set attributes
  setAttributes(signif, m_xtitle, m_ytitle, "Z_{n}", _c0);
  signif->SetMaximum(1.6);
  
  contour->SetLineColor(kBlack);
  contour->SetLineWidth(3);
  
  // Draw objects
  signif->Draw("colz");
  contour->Draw("same");
  numbers->Draw("text45 same");
  addDecorations(_c0);

  char sSys[200];
  sprintf(sSys,"_%2.2f",SYS_ERR);
  string fName = "Zn_" + m_selection + "_" + m_lep 
    + "_" + m_process + sSys + ".png";
  _c0->SaveAs(fName.c_str());

  fName = "Zn_" + m_selection + "_" + m_lep
    + "_" + m_process + sSys + ".root";
  TFile* _f = new TFile(fName.c_str(), "RECREATE");
  _c0->Write();
  signif->Write();
  contour->Write();
  _histoSM->Write();
  numbers->Write();
  _f->Close();

}


//--------------------------------------------------------//
// Set attributes for 2D graph
//--------------------------------------------------------//
void SROptimization::setAttributes(TGraph2D* graph, 
				   string xtitle, 
				   string ytitle, 
				   string ztitle,
				   TCanvas* &canvas)
{
  cout<<"Setting attributes: "<<endl;
  cout<<xtitle<<endl;
  cout<<ytitle<<endl;
  canvas->cd();
  graph->GetXaxis()->SetTitle(xtitle.c_str());
  graph->GetYaxis()->SetTitle(ytitle.c_str());
  graph->GetZaxis()->SetTitle(ztitle.c_str());
  graph->GetXaxis()->SetTitleSize(0.045);
  graph->GetYaxis()->SetTitleSize(0.045);
  graph->GetZaxis()->SetTitleSize(0.05);
  graph->GetXaxis()->SetTitleOffset(1.1);
  graph->GetYaxis()->SetTitleOffset(1.25);
  graph->GetZaxis()->SetTitleOffset(0.75);
}

//--------------------------------------------------------//
// Add decorations
//--------------------------------------------------------//
void SROptimization::addDecorations(TCanvas* &can, int metRel)
{
  can->cd();
  drawLumi();

}
//-------------------------------------------//
// Draw Lumi
//-------------------------------------------//
void SROptimization::drawLumi(float x, float y)
{
  char s[20] ="";
  sprintf(s,"%3.1f",pLUMI);
  string _text = "#int L dt=" + string(s) + " fb^{-1}  #sqrt{s}=8 TeV";
  _utils->myText(x,y,kBlack,_text.c_str(),0.03);

  char sSys[20] = "";
  sprintf(sSys, "Sys %2.2f %%", SYS_ERR);
  _utils->myText(x,y-0.05,kBlack,sSys,0.03);
}
//--------------------------------------------------------//
// Plot Maximium Significance
//--------------------------------------------------------//
void SROptimization::PlotMaxSig()
{

}


//--------------------------------------------------------//
// Dump Table
//--------------------------------------------------------//
void SROptimization::dumpBkgTable()
{
  static const int nPrec = 2;
  
  string fileName= "BkgYield_" + m_selection + ".tex";
  std::ofstream outTEX;
  outTEX.open(fileName.c_str());
  if (!outTEX.is_open()){
    printf("Problem opening output file .... bailing out \n %s \n",fileName.c_str());
    abort();
  }
  
  //Header Tex table
  outTEX << "\\begin{table}[htbp] "<< std::endl;
  outTEX << "\\begin{center} "<< std::endl;
  outTEX << "\\begin{tabular}{l|" ;
  for(uint ilep=0; ilep<N_LEP-1; ilep++) outTEX << "r|";
  outTEX << "r}"<< std::endl;
  outTEX << "\\hline \\hline"<< std::endl;
  //header
  outTEX << std::setw(17) << " & ";
  for(uint ilep=0; ilep<N_LEP; ilep++){
    outTEX << LepNames[ilep];
    if(ilep<N_LEP-1) outTEX << " & ";
  }
  outTEX << "\\\\ \\hline" << std::endl;

  for(int ibkg=0; ibkg<N_MC; ibkg++){
    outTEX << MCNames[ibkg] << " & ";
    for(int ilep=0; ilep<N_LEP; ilep++){
      TH1F* _h = _mcH1[ilep][ibkg];
      Double_t statErr=0;
      float nBkg = _h->IntegralAndError(0,-1, statErr);
      outTEX << std::setprecision(nPrec) << std::fixed 
	     << nBkg << " $\\pm$ " << statErr;
      if(ilep<N_LEP-1) outTEX << " & ";
    }
    if(ibkg==WW)         outTEX << "\\\\ \\hline" <<std::endl;
    else if(ibkg<ALLBKG) outTEX << "\\\\" <<std::endl;
  }
  
  //Footer
  outTEX << "\\\\ \\hline \\hline" <<std::endl;
  outTEX << "\\end{tabular} " << std::endl;
  
  string caption = "\\caption{Background estimate for selection " + m_selection + " }";
  outTEX << caption << std::endl;
  
  outTEX << "\\end{center}" << std::endl;
  outTEX << "\\end{table}" << std::endl;
  outTEX << std::endl;
  outTEX << std::endl;
  
  outTEX << "\\FloatBarrier" << std::endl;

  outTEX.close();
}

//--------------------------------------------------------//
// Dump Table
//--------------------------------------------------------//
void SROptimization::dumpSignalTable()
{
  static const int nPrec = 2;
  
  string fileName= "SignalYield_" + m_selection + "_" + m_process + ".tex";
  std::ofstream outTEX;
  outTEX.open(fileName.c_str());
  if (!outTEX.is_open()){
    printf("Problem opening output file .... bailing out \n %s \n",fileName.c_str());
    abort();
  }
  
  //Header Tex table
  outTEX << "\\begin{table}[htbp] "<< std::endl;
  outTEX << "\\begin{center} "<< std::endl;
  outTEX << "\\tiny " << std::endl;
  outTEX << "\\begin{tabular}{l|" ;
  for(uint ilep=0; ilep<N_LEP-1; ilep++) outTEX << "r|";
  outTEX << "r}"<< std::endl;
  outTEX << "\\hline \\hline"<< std::endl;
  //header
  outTEX << std::setw(17) << " & ";
  for(uint ilep=0; ilep<N_LEP; ilep++){
    outTEX << LepNames[ilep];
    if(ilep<N_LEP-1) outTEX << " & ";
  }
  outTEX << "\\\\ \\hline" << std::endl;

  
  // Loop over the signal files:
  for(uint f=0; f<m_susyFiles.size(); ++f){
    sl_File* file = &m_susyFiles.at(f);
    double mN     = file->getNeutralinoMass();
    double mSpart = file->getSusyMass();
    string s_DSID = itos(file->getId());
    outTEX << std::setprecision(0) << std::fixed  
	   << s_DSID << " [" <<mSpart << "," << mN << "]" << " & ";

    float nSig    = 0.;
    Double_t statErr=0;
    string hFName = "histo_" + m_process + "_" + s_DSID + ".root";
    TFile*    f_sig  = new TFile((m_dirHist + hFName).c_str());
    if(f_sig->IsOpen()){
      for(int ilep=0; ilep<N_LEP; ilep++){
	string hName = m_var + "_" + LepN[ilep] + "_" + m_selection;
	TH1F* signal = (TH1F*) f_sig->Get(hName.c_str());
	nSig = signal->IntegralAndError(0,-1, statErr);
	outTEX << std::setprecision(nPrec) << std::fixed 
	       << nSig << " $\\pm$ " << statErr;
	if(ilep<N_LEP-1) outTEX << " & ";
      }
    }
    else{
      for(int ilep=0; ilep<N_LEP; ilep++){
	outTEX << std::setprecision(nPrec) << std::fixed 
	       << nSig << " $\\pm$ " << statErr;
	if(ilep<N_LEP-1) outTEX << " & ";
      }
    }
    if(f<m_susyFiles.size()-1) outTEX << "\\\\" <<std::endl;
  }
  
  //Footer
  outTEX << "\\\\ \\hline \\hline" <<std::endl;
  outTEX << "\\end{tabular} " << std::endl;
  
  string caption = "\\caption{Signal estimate for " 
    + m_process + " selection " + m_selection + " }";
  outTEX << caption << std::endl;
  
  outTEX << "\\end{center}" << std::endl;
  outTEX << "\\end{table}" << std::endl;
  outTEX << std::endl;
  outTEX << std::endl;
  
  outTEX << "\\FloatBarrier" << std::endl;

  outTEX.close();

}

//--------------------------------------------------------//
// Combine list of Zn plots
//--------------------------------------------------------//
void SROptimization::combineZn()
{
  string _dir = string(getenv("WORKAREA")) + "/SusyWeakProdAna/run/";
  string _ZnFileList = "ZnList.txt";
  string fileName = _dir + _ZnFileList;
  
  FILE* fInput;
  if ((fInput = fopen(fileName.c_str(),"r")) == NULL) {
    cout << "File " << fileName << " could not be opened. Exit" << endl;;
    abort();
  }

  //Grab all the Zn plots
  vector<TGraph2D*> _ZnPlots;
  
  char _name[200];
  int iFile=0;
  TGraph2D* _Zn = NULL;
  while (!feof( fInput )) {
    if (fscanf(fInput, "%s\n",&_name[0])){
      string ZnFile = _dir + string(_name);
      std::cout << "Reading Zn from " << ZnFile << std::endl;
      TFile* _f = new TFile( ZnFile.c_str() );
      if(_f->IsOpen()==kFALSE){
	cout << "Cannot open " << ZnFile << endl;
	abort();
      }

      _Zn = (TGraph2D*) _f->Get("Zn");
      char sName[200];
      sprintf(sName,"Zn_file%02d",iFile);
      _Zn->SetTitle(sName);
      _Zn->SetName(sName);
      _ZnPlots.push_back(_Zn);
      iFile++;
      //_f->Close();
    }
  }

    
  //Make the combine Zn TGraph2D
  TGraph2D* ZnCombined = ZnCombo(_ZnPlots);
  TGraph* contour = getContourGraph(ZnCombined);

  //Plot & Save

  // Set attributes
  TCanvas* _c0  = _utils->myCanvas("Significance Combined");
  setAttributes(ZnCombined, m_xtitle, m_ytitle, "Z_{n}", _c0);
  ZnCombined->SetMaximum(1.6);
  
  contour->SetLineColor(kBlack);
  contour->SetLineWidth(3);
  
  // Draw objects
  ZnCombined->Draw("colz");
  contour->Draw("same");
  addDecorations(_c0);
  
  char sSys[200];
  sprintf(sSys,"_%2.2f",SYS_ERR);
  string fName = "ZnCombined_" + m_selection + "_" + m_process + sSys + ".png";
  _c0->SaveAs(fName.c_str());



}
//--------------------------------------------------------//
// Combine Zn plots in quad
//--------------------------------------------------------//
TGraph2D* SROptimization::ZnCombo(vector<TGraph2D*> _ZnGraph)
{
  if(_ZnGraph.size()==0){
    cout << "No Zn plot to combine. Aborting. "  << endl;
    abort();
  }
  
  int ipt = _ZnGraph[0]->GetN();

  if(m_dbg>0) cout << "Combining Zn with " << ipt << " points " << endl;

  double* _xx = new double[ipt+1]; 
  double* _yy = new double[ipt+1];
  double* _zz = new double[ipt+1];

  memset(_xx,0.0,sizeof(_xx[0])*ipt+1);
  memset(_yy,0.0,sizeof(_yy[0])*ipt+1);
  memset(_zz,0.0,sizeof(_zz[0])*ipt+1);

  

  for(uint iZn=0; iZn<_ZnGraph.size(); iZn++){
    if(m_dbg>0) cout << "Grabbing Zn from " <<  _ZnGraph[iZn]->GetTitle() << endl;
  
    double* _xVal = _ZnGraph[iZn]->GetX();
    double* _yVal = _ZnGraph[iZn]->GetY();
    double* _zVal = _ZnGraph[iZn]->GetZ();

    //fudge
    _xx[0]=0;
    _yy[0]=0;
    _zz[0]=4;
    for(int i=1; i<ipt+1; i++){
      _xx[i] = _xVal[i];
      _yy[i] = _yVal[i];
      _zz[i] += (_zVal[i]>0.1) ? _zVal[i] *_zVal[i] : 0.001;
    }
  }

  for(int i=0; i<ipt; i++){
    _zz[i] = sqrt(_zz[i]);
    if(m_dbg>5) cout << "Comb Zn [" << _xx[i] << "," << _yy[i] << "]" 
		     << " Zn " << _zz[i] << endl;
  }
  
  TGraph2D* _comb = new TGraph2D(ipt/*ipt-1*/, _xx, _yy, _zz);
  _comb->SetTitle("ZnCombine");
  _comb->SetName("ZnCombine");

  delete _xx;
  delete _yy;
  delete _zz;
  
  return _comb;
}
