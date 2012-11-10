#include <iostream>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <map>

typedef unsigned uint;

void make_ZX_texTables(string name)
{
  string dir = "histos_110812_13fb_n0111_DD_MMtrial9_SYS_HFT/ZX_est_v2/";
  string _pathFile = string(getenv("WORKAREA")) + "/histoAna/SusyAna/"+ dir;

  cout << "Dir location " << _pathFile << endl;

  std::ifstream infile;
  string fileNameZX =  _pathFile + name;
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
  std::map<string,float> sysVal;

  while(!infile.eof()){
    // Get the line
    std::getline(infile,currentLine);   
    char _name[200];
    sscanf(currentLine.c_str(),"%s\t%f",&_name[0],&fVal);
    sSys = string(_name);
    cout << "Read " << sSys << "\t" << fVal << endl;

    sysVal.insert(sysVal.end(),make_pair(sSys,fVal));
  }

  //
  //list of syst categories
  //All but NOM, JER and  RESOST and UP & DN
  //
  vector<string> sysList;
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
  
  vector<float> _sys;
  std::map<string,float>::iterator _iter;

  //
  // Compute each indidual shift by sys categories
  //
  //Get nominal 
  _iter =  sysVal.find("NOM");
  if(_iter != sysVal.end()){
    float _nom = (*_iter).second;
    cout << "Nom: " << _nom << endl;
    _sys.push_back(_nom);
  }

  for(uint isys=1; isys<sysList.size(); isys++){
    string sSysUp, sSysDn;
    float _sysUp=0;
    float _sysDn=0;
    if(sysList[isys] == "JER" || sysList[isys] == "RESOST"){
      sSysUp  = sysList[isys];
      _iter   = sysVal.find(sSysUp);
      if(_iter != sysVal.end()) _sysUp = (*_iter).second;
      _sysUp -= _sys[0]; 
      cout << sysList[isys] << ": \t" << _sysUp << endl;;
      _sys.push_back(_sysUp);
    }
    else{
      sSysUp = sysList[isys] +"_UP";
      sSysDn = sysList[isys] +"_DN";
      _iter =  sysVal.find(sSysUp);
      if(_iter != sysVal.end()) _sysUp = (*_iter).second;
      _iter =  sysVal.find(sSysDn);
      if(_iter != sysVal.end()) _sysDn = (*_iter).second;
      _sysUp -=  _sys[0]; 
      _sysDn -=  _sys[0];
      float _symSys=0;
      if(fabs(_sysUp)>fabs(_sysDn)) _symSys = fabs(_sysUp);
      else                          _symSys = fabs(_sysDn); 
      cout << sysList[isys] << ": \t" << _symSys << endl;
      _sys.push_back(_symSys);

    }
    

    
  }




  


}
