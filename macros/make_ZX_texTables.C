/*
.L ../macros/make_ZX_texTables.C++   
 make_ZX_texTables("TF");
 make_ZX_texTables("SF");
 make_ZX_texTables("EST");
 make_ZX_texTables("MT2aEff");
 make_ZX_texTables("MT2bEff");

*/

#include <iostream>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <TString.h>

using namespace std;
typedef unsigned uint;
bool verbose = false;

std::ofstream outFile;

int numPrecision=3;

vector<string> sysList;
vector<string> LEP;
vector<string> SREG;

void getRegion(string var, string sReg);
void readFile(string var, string sReg, int ilep,  std::map<string,float>* sysVal);
void getSymSysVal(std::map<string,float>* sysVal, std::map<string,float>* _sysValSymm);
void dump(std::map<string,float>* sysVal);
void addLine(string sReg,std::map<string,float>* sysVal_ee,std::map<string,float>* sysVal_mm);

//_______________________________________________________
// 
// Make latex table of ZX estimate for note
//_______________________________________________________
void make_ZX_texTables(string var)
{
  SREG.clear();
  SREG.push_back("SRjveto");
  SREG.push_back("SR2jets");
  SREG.push_back("SRmT2");
  SREG.push_back("SRmT2b");
  SREG.push_back("preSRmT2a"); 
  SREG.push_back("preSRmT2b"); 
  SREG.push_back("NWW1");
  SREG.push_back("NWW2");

  string fileName= "ZX_" + var + ".tex";
  outFile.open(fileName.c_str());
  if (!outFile.is_open()){
    printf("Problem opening output file .... bailing out \n %s \n",fileName.c_str());
    abort();
  }
  
  string sVar="";
  string caption;
  if(var == "TF"){
    sVar = "$\\mathcal{T}_f$";
    caption = "\\caption{$Z+X$ $\\mathcal{T}_f$ }";
  }
  else if(var == "SF"){
    sVar = "$\\mathcal{S}$";
    caption = "\\caption{$Z+X$ $\\mathcal{S}$ }";
  }
  else if(var == "EST"){
    sVar = "Estimated $Z+X$ ";
    caption = "\\caption{$Z+X$ estimate }";
    numPrecision=2;
  }
  else if(var == "MT2aEff" || var == "MT2bEff"){
    if(var == "MT2aEff") sVar = "\\mtta\\ $\\mathcal{E}$ ";
    if(var == "MT2bEff") sVar = "\\mttb\\ $\\mathcal{E}$ ";
    caption = "\\caption{\\mtt\\ efficiency }";
  }

  outFile << "\\begin{table}[htbp] "<< std::endl;
  outFile << "\\begin{center} "<< std::endl;
  outFile << "\\begin{tabular}{l >{\\small}c >{\\small}c }\\hline\\hline" << std::endl;
  outFile << sVar;
  outFile << "& $e^{+}e^{-}$ & $\\mu^{+}\\mu^{-}$ \\\\ \\hline" << std::endl;

  for(uint ireg=0; ireg<SREG.size();ireg++){
    if(var=="MT2aEff" && ireg != 4) continue;
    if(var=="MT2bEff" && ireg != 5) continue;
    if(var=="TF" && ireg==5) continue;
    getRegion(var,SREG[ireg]);
  }

  outFile << "\\hline \\hline" <<std::endl;
  outFile << "\\end{tabular} " << std::endl;
  outFile << caption << std::endl;
  outFile << "\\end{center}" << std::endl;
  outFile << "\\end{table}" << std::endl;
  outFile << std::endl;
  outFile << std::endl;
  
  outFile.close();

}

//_______________________________________________________
//
// Get Val + sys for given CR's
//_______________________________________________________
void getRegion(string var, string sReg){

  map<string,float>* _sysVal_ee = new map<string,float>();
  map<string,float>* _sysVal_mm = new map<string,float>();

  readFile(var,sReg,0,_sysVal_ee);
  readFile(var,sReg,1,_sysVal_mm);

  //Symmetrize sys
  std::map<string,float>* _sysValSymm_ee = new map<string,float>();
  std::map<string,float>* _sysValSymm_mm = new map<string,float>();
  //cout << "EE " << endl;
  getSymSysVal(_sysVal_ee,_sysValSymm_ee);
  //cout << "MM " << endl;
  getSymSysVal(_sysVal_mm,_sysValSymm_mm);

  if(verbose) {
    cout << "******** " << LEP[0]  << "******** " << endl;
    dump(_sysValSymm_ee);
    cout << "******** " << LEP[1]  << "******** " << endl;
    dump(_sysValSymm_mm);
  }

  addLine(sReg,_sysValSymm_ee,_sysValSymm_mm);

  delete _sysVal_ee;
  delete _sysVal_mm;
  delete _sysValSymm_ee;
  delete _sysValSymm_mm;

}

//_______________________________________________________
//
// Read input file 
//_______________________________________________________
void readFile(string var, string sReg, int ilep, std::map<string,float>* sysVal){

  string dir = "histos_110812_13fb_n0111_DD_MMtrial9_SYS_HFT/ZX_est_v2/";
  string _pathFile = string(getenv("WORKAREA")) + "/histoAna/SusyAna/"+ dir;
  //string _pathFile = string(getenv("WORKAREA")) + "/SusyWeakProdAna/run/";

  if(verbose) cout << "Dir location " << _pathFile << endl;

  LEP.clear();
  LEP.push_back("EE");
  LEP.push_back("MM");


  string fileName   = "ZX_" + var + "_" + LEP[ilep] + "_" + sReg + ".txt";
  string fileNameZX =  _pathFile + fileName;

  std::ifstream infile;
  infile.open(fileNameZX.c_str());
  if(infile.fail())    {
    std::cout << "Fail opening file " << fileNameZX << std::endl;
    abort();
  }

  //
  //Read input file
  //
  string sSys="";
  float  fVal=0;
  std::string currentLine = "";

  sysVal->clear();
  while(!infile.eof()){
    // Get the line
    std::getline(infile,currentLine);   
    char _name[200];
    sscanf(currentLine.c_str(),"%s\t%f",&_name[0],&fVal);
    sSys = string(_name);
    if(verbose) cout << "Read " << sSys << "\t" << fVal << endl;

    sysVal->insert(sysVal->end(),make_pair(sSys,fVal));
  }
  if(verbose) cout << endl;

}
//_______________________________________________________
//
// Symmetrize/group systematics
// Compute each indidual shift by sys categories 
//_______________________________________________________
void getSymSysVal(std::map<string,float>* sysVal, std::map<string,float>* sysValSymm){

  sysList.clear();
  sysList.push_back("NOM");
  sysList.push_back("CRSTAT");
  sysList.push_back("EES_Z");
  sysList.push_back("EES_MAT");
  sysList.push_back("EES_PS");
  sysList.push_back("EES_LOW");
  sysList.push_back("EER");
  sysList.push_back("MS");
  sysList.push_back("ID");
  sysList.push_back("JES");
  sysList.push_back("JER");        
  sysList.push_back("SCALEST");
  sysList.push_back("RESOST");     
  sysList.push_back("TRIGSF_EL");
  sysList.push_back("TRIGSF_MU");
  sysList.push_back("BTag_BJet");
  sysList.push_back("BTag_CJet");
  sysList.push_back("BTag_LJet");
  sysList.push_back("BKGMETHOD");

  //Get nominal 
  float _nom = 0;
  float _totSys=0; //no stat
  float _bTagSys=0;
  sysValSymm->clear();

  std::map<string,float>::iterator _iter =  sysVal->find(sysList[0]);
  if(_iter != sysVal->end()){
    _nom = (*_iter).second;
    sysValSymm->insert(sysValSymm->end(),make_pair(sysList[0],_nom));
  }
  
  //Get all the sys
  //All but NOM, JER and  RESOST and UP & DN
  for(uint isys=1; isys<sysList.size(); isys++){
    if(sysList[isys] == "JER" || sysList[isys] == "RESOST"){
      _iter = sysVal->find(sysList[isys]);
      float _sys=0;
      if(_iter != sysVal->end()) _sys = (*_iter).second - _nom;
      sysValSymm->insert(sysValSymm->end(),make_pair(sysList[isys],_sys));
      _totSys += pow(_sys,2);
      //cout << "add to tot " << sysList[isys] << " " << _sys << endl;
    }
    else{
      string sSysUp = sysList[isys] +"_UP";
      string sSysDn = sysList[isys] +"_DN";
      float _sysUp = 0; 
      float _sysDn = 0;
      _iter =  sysVal->find(sSysUp);
      if(_iter != sysVal->end()) _sysUp = (*_iter).second - _nom;
      _iter =  sysVal->find(sSysDn);
      if(_iter != sysVal->end()) _sysDn = (*_iter).second - _nom;
      float _symSys=0;
      if(fabs(_sysUp)>fabs(_sysDn)) _symSys = fabs(_sysUp);
      else                          _symSys = fabs(_sysDn); 
      TString _thisSys(sysList[isys]);
      if(_thisSys.Contains("BTag")) _bTagSys += pow(_symSys,2); 
      else  sysValSymm->insert(sysValSymm->end(),make_pair(sysList[isys],_symSys));
      if(sysList[isys] != "CRSTAT"){
	//cout << "add to tot " << sysList[isys] << " " << _symSys << endl;
	_totSys += pow(_symSys,2);
      }
    }
  }
  sysValSymm->insert(sysValSymm->end(),make_pair("BTag",sqrt(_bTagSys)));
  sysValSymm->insert(sysValSymm->end(),make_pair("TOTAL",sqrt(_totSys)));
  //cout << endl;

}
//_______________________________________________________
//
// Debug dump
//_______________________________________________________
void dump(std::map<string,float>* sysVal){

  std::map<string,float>::iterator _iter;
  for(_iter = sysVal->begin(); _iter != sysVal->end(); ++_iter )
    cout << _iter->first << "\t \t" << _iter->second << "\n";

}
//_______________________________________________________
//
// Add line to latex Table
//_______________________________________________________
void addLine(string sReg,std::map<string,float>* sysVal_ee,std::map<string,float>* sysVal_mm)
{
  float val_ee=0;
  float val_ee_stat=0;
  float val_ee_sys=0;

  float val_mm=0;
  float val_mm_stat=0;
  float val_mm_sys=0;

  std::map<string,float>::iterator _iter;
  _iter = sysVal_ee->find("NOM");
  if(_iter != sysVal_ee->end()) val_ee = (*_iter).second;

  _iter = sysVal_ee->find("CRSTAT");
  if(_iter != sysVal_ee->end()) val_ee_stat = (*_iter).second;

  _iter = sysVal_ee->find("TOTAL");
  if(_iter != sysVal_ee->end()) val_ee_sys = (*_iter).second;


  _iter = sysVal_mm->find("NOM");
  if(_iter != sysVal_mm->end()) val_mm = (*_iter).second;

  _iter = sysVal_mm->find("CRSTAT");
  if(_iter != sysVal_mm->end()) val_mm_stat = (*_iter).second;

  _iter = sysVal_mm->find("TOTAL");
  if(_iter != sysVal_mm->end()) val_mm_sys = (*_iter).second;



  outFile << std::setprecision(numPrecision) << std::fixed 
	  << sReg << " & " 
	  << val_ee << " $\\pm$ " << val_ee_stat << " $\\pm$ " << val_ee_sys
	  << " & "
	  << val_mm << " $\\pm$ " << val_mm_stat << " $\\pm$ " << val_mm_sys
	  << "\\\\" << std::endl;
  

}
