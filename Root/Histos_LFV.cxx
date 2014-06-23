#include "SusyWeakProdAna/Histos_LFV.h"
#include "TSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
/*--------------------------------------------------------------------------------*/
// SusyHistos Book2LHistograms
/*--------------------------------------------------------------------------------*/
void Histos_LFV::BookLFVHistograms(TDirectory* _hDir, bool useSys)
{
  std::cout << "Booking LFV histos " << std::endl;
  std::cout << "Number of Signal regions " << LFV_NSR << endl;

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

#define BOOK_SRLFV(hN,xT,u,yT, ...)					\
  for(int i=0; i<4; i++){						\
    string sFlav;							\
    if(i==0) sFlav="EE_";						\
    if(i==1) sFlav="MM_";						\
    if(i==2) sFlav="EM_";						\
    if(i==3) sFlav="ME_";						\
    for(int j=0; j<LFV_NSR; j++){					\
      for(int isys=0; isys<maxSys; isys++){				\
	if(string(u).length()>0) sx = string(xT+string(" [")+u+string("]")); \
	else sx = 	string(xT);					\
	string stype = "LFV_" + LFV_SRNAME[j] + "_" + sFlav + #hN + "_" + DGSystNames[isys];	\
	hN[j][i][isys] = (TH1F*) _utils->myTH1F(stype.c_str(),		\
						stype.c_str(),		\
						__VA_ARGS__,sx.c_str() ,yT); \
	_utils->yAxis(hN[j][i][isys],u);				\
      }									\
    }									\
  }

  BOOK_SRLFV(LFV_pred,"","",syaxis,1,-0.5,0.5);
  BOOK_SRLFV(LFV_cutflow,"","",syaxis,30,-0.5,29.5);


#undef BOOK_SRDG2L



}
