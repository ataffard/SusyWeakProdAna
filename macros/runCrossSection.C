#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <iomanip>

// Forward Declare for ROOT
double getCrossSection(std::ifstream &infile, int mcID, double crossSection, int syst = 0);

// App
void runCrossSection()
{
  ///////////////////////////////////////////////////////////////////
  ///// This part is only going to be called in the initialization
  ///////////////////////////////////////////////////////////////////
  std::ifstream infile;                        // Do this only once in the
  infile.open("Simplified_CrossSections.txt"); // not to slow down the code

  ///////////////////////////////////////////////////////////////////
  ///// This part is the only thing to be called in the event loop
  ///////////////////////////////////////////////////////////////////
  std::cout << "===============================================================================" << std::endl;
  std::cout << "Cross section for 144871 - UP  = " << getCrossSection(infile,144871,6.52258, 1)  << std::endl;
  std::cout << "Cross section for 144871 - NOM = " << getCrossSection(infile,144871,6.52258, 0)  << std::endl;
  std::cout << "Cross section for 144871 - DN  = " << getCrossSection(infile,144871,6.52258,-1)  << std::endl;
  std::cout << "===============================================================================" << std::endl;
  std::cout << "Cross section for 164391 - UP  = " << getCrossSection(infile,164391,0.002066, 1) << std::endl;
  std::cout << "Cross section for 164391 - NOM = " << getCrossSection(infile,164391,0.002066, 0) << std::endl;
  std::cout << "Cross section for 164391 - DN  = " << getCrossSection(infile,164391,0.002066,-1) << std::endl;
  std::cout << "===============================================================================" << std::endl;

  ///////////////////////////////////////////////////////////////////
  ///// This part is only going to be called in the end
  ///////////////////////////////////////////////////////////////////
  infile.close();                              // Close the file when done
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Function to return the cross section w/ the desired systematic variation
// Inputs: infile      :: Input file w/ cross sections 
//
//         mcID        :: MCID (nt->evt()->mcChannel) 
//
//         crossSection:: Cross section from SusyNtuple [for consistency check and read fail]
//                        (nt->evt()->xsec)
//
//         systematic  :: 0 is nom, 1 is up 1sigma and -1 is down 1sigma
///////////////////////////////////////////////////////////////////////////////////////////////
double getCrossSection(std::ifstream &infile, int mcID, double crossSection, int syst)
{
  int _dbg = 0;

  // Clear the seekg so that we can read the file multiple times
  infile.clear();
  infile.seekg(0,std::ios::beg); 

  // Check if can read the file
  if(infile.fail())
  {
    std::cout << "getCrossSection::WARNING Cannot read cross section file, return input value..." << std::endl;
    return crossSection;
  }

  // Read the text file information
  bool        matched     = false;
  int         _dsId       = 0;
  std::string currentLine = "", 
              _suffix     = "";  
  float       _mC1        = 0., 
              _mN1        = 0., 
              _mSlep      = 0., 
              _xsec       = 0., 
              _relError   = 0.; 

  // Read through file
  while(!infile.eof())
  {
    // Get the line
    std::getline(infile,currentLine);

    if(_dbg > 0)
    {
      std::cout << " MCID : "        << std::setw(6) << _dsId 
                << " !! mC1/mN2 : "  << std::setw(5) << _mC1
                << " !! mSlep : "    << std::setw(5) << _mSlep
                << " !! mN1 : "      << std::setw(5) << _mN1
                << " !! xsec : "     << std::setw(6) << _xsec
                << " !! relError : " << std::setw(6) << _relError << std::endl;
    }

    // Scan the line
    sscanf(currentLine.c_str(),"%i\t%f\t%f\t%f\t%f\t%f",&_dsId,&_mC1,&_mSlep,&_mN1,&_xsec,&_relError);

    // Look for match
    if(_dsId == mcID) { 
      matched = true;
      break;
    }
  }  

  // Check if match is found
  if(!matched) {
    std::cout << "getCrossSection::WARNING No match is found for MCID :: " << _dsId << std::endl;
    std::cout << "                         Returning input value..."     << std::endl;
    return crossSection;
  }

  // Consistency check between SusyNt and text file cross sections
  if (fabs(crossSection - _xsec) > 1.e-3) 
  {
    std::cout << "getCrossSection::WARNING Possible inconsistency in cross sections for MCID :: " << _dsId << std::endl;
  }

  // Return the result
  if ( syst == 0 ){
    return crossSection;
  } else if (syst ==  1){
    return crossSection*(1 + 0.01*_relError);
  } else if (syst == -1){
    return crossSection*(1 - 0.01*_relError);
  } else {
    std::cout << "getCrossSection::WARNING Unkown Systematic variation, returning nominal..." << std::endl;
    return crossSection;
  }
}
