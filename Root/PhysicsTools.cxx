#include "SusyWeakProdAna/PhysicsTools.h"

#include <iostream>
#include <iomanip>
#include <fstream>

#include "TGraph.h"
#include "TMatrixD.h"
#include "TVectorD.h"

//-----------------------------------------------------------------------------
LEP_TYPE getType(int org, int type, 
		 std::string dataset,
		 int DSId,
		 int truthMatchType,
		 bool isEle,
		 bool isChargeFlip)
{
  TString ss = dataset;
  /*
  if(ss.Contains("simplifiedModel")){
    std::cout << " org " << org << " type " <<  type <<std::endl; 
    return PR;
  }
  */
  if(org==22) return PR;

  if(isPT(org, type,DSId,truthMatchType,isEle,dataset)) return PR;
  else if(isHF(org, type,truthMatchType)) return HF;
  else if(isLF(org, type,DSId,truthMatchType, isEle,isChargeFlip)) return LF;
  else if(isConv(org, type,truthMatchType, isEle,isChargeFlip)) return CONV;
  return TYPE_Undef;
}
//-----------------------------------------------------------------------------
bool isPT(int org, int type,
	  int mcId,
	  int truthMatchType,
	  bool isEle,
	  std::string dataset)
{
  TString ss = dataset;
  if(org==22) return true;
  /*
  if(ss.Contains("simplifiedModel")){
   
    return PR;
  }
  */
  // Updated way of handling real and fake leptons using LeptonTruthTools
  return (truthMatchType == PR);
  
  // Code taken from Steve.  There seems to be an issue with Sherpa samples, so 
  // need to handle those separately. Also just for clarification:
  // * mcOrigin = 9 -- Tau Lepton
  // * mcType   = 1 -- Unknown Electron
  // * mcType   = 2 -- Iso Electron
  // * mcType   = 5 -- Unknown Muon
  // * mcType   = 6 -- Iso Muon

  // Cut is sample dependent due to Sherpa classifications broken


//   // All tau leptons are classified as non-iso
//   // I'm not sure why, yet, but for now I will treat them as real leptons.
//   if(org == 9) return true;
  
//   // Sherpa diboson, assume all unknowns are real leptons
//   // This is an approximation, but probably ok.
//   // *** I will prob need to add run numbers here ***
//   if( (mcId>=126892 && mcId<=126895) || (mcId>=147770 && mcId<=147772) ||
//       (mcId>=147774 && mcId<=147776)){
//     if(isEle) return type == 1 || type == 2;
//     else      return type == 5 || type == 6;
//   }
//   else{
//     // 2-lep classifies everything as real if it 
//     // is from W, Z, tau, or top..
//     //uint origin = lep->mcOrigin;
//     //return origin == 9 || origin == 12 || origin == 13 || origin == 10;
    
//     if(isEle) return type == 2;
//     else      return type == 6;
//   }


  /*
  if( org==1 ||
      org==2 ||
      org==9 ||
      org==10 ||
      (org>=12 && org<=22)) return true;  
  if( org==0 && (type==1 || type==5))  return true;  //for Sherpa
  */
  return false;
}
/*--------------------------------------------------------------------------------*/
bool isFake(int org, int type,int mcId,
	    int truthMatchType,
	    bool isEle,std::string dataset)
{
  return !isPT(org,type, mcId,truthMatchType,isEle,dataset);
}
//-----------------------------------------------------------------------------
bool isHF(int org, int type,int truthMatchType)
{
  return (truthMatchType == HF);

//   return org == 25 || org == 26 || org == 27 || org == 28 ||
//     org == 29 || org == 32 || org == 33;

  return false;
}

//-----------------------------------------------------------------------------
bool isLF(int org, int type, 
	  int mcId,
	  int truthMatchType,
	  bool isEle,
	  bool isChargeFlip)
{
  return (truthMatchType == LF);

  // Steve's way:
 //  bool isqFlip = isEle ? isChargeFlip : false;
//   return isFake(org,type,mcId,truthMatchType,isEle) && 
//     !isConv(org,type,mcId,truthMatchType,isEle) &&
//     !isHF(org,type,truthMatchType) && 
//     !isqFlip;


  /*
  if( //org==0 ||
      org==4 ||
      org==8 ||
      org==11 ||
      (org>=23 && org<=24) ||
      (org>=30 && org<=31) ||
      org==34 ||
      org==35 ||
      org==41 || org==42 ) return true;
  if(org==0 && type==17) return true; //for Sherpa
  */
  return false;
}

//-----------------------------------------------------------------------------
bool isConv(int org, int type,
	    int truthMatchType,
	    bool isEle,
	    bool isChargeFlip)
{
  //return lep->mcOrigin == 5;
  //CONV flag not working on SUSY samples !!
  bool isConv       = truthMatchType == CONV;
 
  //  bool isConv       = org == 5;
  bool isqFlip =  isEle ? isChargeFlip : false; 

  /*
  std:: cout << "truth " << truthMatchType 
	     << " isEle " << isEle 
	     << " qFlip " << isChargeFlip << std:: endl;
  */
  return isConv && !isqFlip;

  /*
  if( org==3 ||
      (org>=5 && org<=7) ||
      ( org>=36 && org<=40)) 
    return true;
  */
  return false;
}



//-----------------------------------------------------------------------------
void binomialError(float Num, float Den, float& Eff, float& EffErr){
  //Compute Eff=N1/N2 where N1=pass, N2=total (pass+fail)
  Eff=0; 
  EffErr=0;
  Eff = (Den >0. ? Num/Den : 0.);
  EffErr = (Den >0.  ? sqrt((Eff-pow(Eff,2))/Den) : 0.);
}


//-----------------------------------------------------------------------------
// Transverse mass 
float mT(TLorentzVector _l, TLorentzVector _nu)
{
  float dphi = acos(cos(_l.Phi()-_nu.Phi()));
  return sqrt(2*_l.Pt()*_nu.Pt() * (1- cos(dphi)) ); 
}

//-----------------------------------------------------------------------------
// Transverse mass WW
float mTWW(TLorentzVector _ll, TLorentzVector _nu,bool MvvTrue)
{
  float dphi = acos(cos(_ll.Phi()-_nu.Phi()));
  float mll = _ll.M();
  float mvv=0;
  if(!MvvTrue) mvv=mll;

  float mT=0;
  mT = sqrt( pow(mll,2) + pow(mvv,2) 
	     + 2*( sqrt(pow(_ll.Pt(),2) + pow(mll,2)) * sqrt(pow(_nu.Pt(),2) + pow(mvv,2)) 
		   - _ll.Pt()*_nu.Pt()*cos(dphi) ) );
  
  return mT;

}
//-----------------------------------------------------------------------------
float mCT(TLorentzVector v1, TLorentzVector v2){
  float mct = (v1.Et() + v2.Et())*(v1.Et() + v2.Et()) - (v1-v2).Perp2();
  if(mct<0) return 0;//AT
  return sqrt( fabs(mct) );
};

//-----------------------------------------------------------------------------
float mCTperp(TLorentzVector lep0, TLorentzVector lep1, TLorentzVector met){
  /*
  // Get 3 vectors for objects
  TVector3 U_t  = (-met-lep0-lep1).Vect();
  U_t.SetZ(0);
  U_t = U_t.Unit();
  TVector3 l0_t = lep0.Vect(); l0_t.SetZ(0);
  TVector3 l1_t = lep1.Vect(); l1_t.SetZ(0);
  
  // Calculate
  TVector3 p0_t = U_t.Cross( l0_t.Cross(U_t) );
  TVector3 p1_t = U_t.Cross( l1_t.Cross(U_t) );
  
  float value = 2*(p0_t.Mag()*p1_t.Mag() + p0_t*p1_t);
  if( value < 0 ) return 0;
  return sqrt(value);
  */
 // Access the 3-D vectors
  
  TVector3 v13D = lep0.Vect();
  TVector3 v23D = lep1.Vect();
  TVector3 u3D  = met.Vect();
  u3D.SetZ(0);
  u3D = u3D.Unit();
  
  // Calculate p[1,2]Perp
  TVector3 p1Perp = u3D.Cross(v13D.Cross(u3D));
  TVector3 p2Perp = u3D.Cross(v23D.Cross(u3D));
  
  double ET1Perp  = sqrt(p1Perp.Perp2() + lep0.M2());
  double ET2Perp  = sqrt(p2Perp.Perp2() + lep1.M2());
  
  double mCTSQ = pow(ET1Perp + ET2Perp,2) - (p1Perp-p2Perp).Perp2();
  
  return ((mCTSQ > 0.) ? sqrt(mCTSQ) : 0.);
  

};

//-----------------------------------------------------------------------------
float mCTpara(TLorentzVector lep0, TLorentzVector lep1, TLorentzVector met){
  /*
  TVector3 U_t  = (-met-lep0-lep1).Vect().Unit();
  TVector3 p0_t = (lep0.Vect() * U_t) * U_t;
  TVector3 p1_t = (lep1.Vect() * U_t) * U_t;
  return sqrt( 2*(p0_t.Perp()*p1_t.Perp() + p0_t.X()*p1_t.X() + p0_t.Y()*p1_t.Y()) );
  */
  TVector3 v13D = lep0.Vect();
  TVector3 v23D = lep1.Vect();
  TVector3 u3D  = met.Vect();
  u3D.SetZ(0);
  u3D = u3D.Unit();
  
  // Calculate p[1,2]Para
  TVector3 p1Para = v13D.Dot(u3D)*u3D;
  TVector3 p2Para = v23D.Dot(u3D)*u3D;
  
  double ET1Para  = sqrt(p1Para.Perp2() + lep0.M2());
  double ET2Para  = sqrt(p2Para.Perp2() + lep1.M2());
  
  double mCTSQ = pow(ET1Para + ET2Para,2) - (p1Para-p2Para).Perp2();
  
  return ((mCTSQ > 0.) ? sqrt(mCTSQ) : 0.);
  
};

//-----------------------------------------------------------------------------
// d0 signed wrt to jet direction
float signedD0(float d0, float sigmaD0,
	       TLorentzVector _p, TLorentzVector _j){
  float sD0=-999;

  //  float d0 = _m->getF("D0");
  //  float sigmaD0 = _m->getF("sD0");
  float qd0=d0/fabs(d0);
  float m_sPhi= _p.Phi() + qd0 * TMath::Pi()/2.;
  float dPhi = m_sPhi- _j.Phi();
  float signIP=  fabs(cos(dPhi))/(cos(dPhi)+1e-32) * fabs(d0); 
  sD0=signIP/sigmaD0;
  
  return sD0;

}
/*--------------------------------------------------------------------------------*/
// pT rel to jet axis
float ptRel(TLorentzVector j, TLorentzVector p){
  TVector3 jet(j.Px(), j.Py(), j.Pz());
  TVector3 part(p.Px(), p.Py(), p.Pz());
  return part.Perp(jet);
}

/*--------------------------------------------------------------------------------*/
// mtt collinear approximation
float mZTauTau(TLorentzVector l0, TLorentzVector l1, TLorentzVector met){
  float px0(l0.Px()), py0(l0.Py());
  float px1(l1.Px()), py1(l1.Py());
  float pxm(met.Px()), pym(met.Py());
  float num( px0*py1 - py0*px1 );
  float den1( py1*pxm - px1*pym + px0*py1 - py0*px1 );
  float den2( px0*pym - py0*pxm + px0*py1 - py0*px1 );
  float x1 = ( den1 != 0.0  ? (num/den1) : 0.0);
  float x2 = ( den2 != 0.0  ? (num/den2) : 0.0);
  // not guaranteed that this configuration is kinematically possible
  return (x1*x2 > 0.0 ? (l0+l1).M() / sqrt(x1*x2) : -1.0);
}

/*--------------------------------------------------------------------------------*/
float acoplanarity(TLorentzVector v0, TLorentzVector v1)
{
  return v1.Phi() - v0.Phi() - TMath::Pi();
}


//-----------------------------------------------------------------------------
//Take from https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SUSYD3PDSnippets#Transverse_Sphericity_ST
// Fill the 3 vectors with px,py,pz of the different particles entering sphericity computation.
float Sphericity(std::vector<float> &pxVector,
                 std::vector<float> &pyVector,
                 std::vector<float> &pzVector,
                 bool IsTransverseSphericity)
{
  //!matrix elements are set to 0
  // Defines the Sphericity Matrix
  TMatrixD sphericityMatrix(3, 3);
  for (int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      sphericityMatrix[i][j] = 0.;
  Double_t d_sph_p = 0.;
  
  //////////////////////////
  //! Calculates Sphericity Matrix S_{ij} = \sum_k p_k^j p_k^j  / \sum_k |p_k|^2
  Double_t d_sph_Pxyz[3];
  for(int i = 0; i < 3; i++) d_sph_Pxyz[i] = 0.;
  for(unsigned int indexI = 0;indexI < pxVector.size();indexI++) {
    for(int i = 0; i < 3; i++) d_sph_Pxyz[i] = 0.;
    d_sph_Pxyz[0] = pxVector[indexI];
    d_sph_Pxyz[1] = pyVector[indexI];
    
    //! only if the 3D Sph is calculated, one needs pz != 0
    if (!IsTransverseSphericity) d_sph_Pxyz[2] = pzVector[indexI];
    
    //! The Sphericity Matrix is calculated. If pz = 0 it
    //! is the needed 2x2 Matrix
    for(int iMatrixI = 0; iMatrixI < 3; iMatrixI++)
      for(int iMatrixJ = 0; iMatrixJ < 3; iMatrixJ++)
        sphericityMatrix[iMatrixI][iMatrixJ] += d_sph_Pxyz[iMatrixI]*d_sph_Pxyz[iMatrixJ];
    
    //! calculates \sum_k |p_k|^2
    d_sph_p += d_sph_Pxyz[0]*d_sph_Pxyz[0] +
      d_sph_Pxyz[1]*d_sph_Pxyz[1] +
      d_sph_Pxyz[2]*d_sph_Pxyz[2];
  }
  
  //!  Normalizes S_{ij}
  if(d_sph_p != 0.) {
    for(int iMatrixI = 0; iMatrixI < 3; iMatrixI++) {
      for(int iMatrixJ = 0; iMatrixJ < 3; iMatrixJ++) {
        sphericityMatrix[iMatrixI][iMatrixJ]=
          sphericityMatrix[iMatrixI][iMatrixJ]/d_sph_p;
      }
    }
    //! if there are no values available, it crashes.
  } else {
    // Cleaning
    return -99.;
  }
  
  //! Calculate the EigenValues
  TVectorD eigenValues;
  const TMatrixD eigenVectoren = sphericityMatrix.EigenVectors(eigenValues);
  
  //! The EigenValues have to be sorted: Lambda1 > Lambda2 > Lambda3
  Int_t eigenLambda1 = 0, eigenLambda2 = 1, eigenLambda3 = 2;
  // from the babar sphericity code...
  double Emax = eigenValues[0];
  double Emin = eigenValues[0];
  for ( int i = 0; i <= 2 ; ++i ) {
    if ( eigenValues[i] > Emax ) {
      Emax = eigenValues[i];
      eigenLambda1 = i;
    }
    if ( eigenValues[i] < Emin ) {
      Emin = eigenValues[i];
      eigenLambda3 = i;
    }
  }
  eigenLambda2 = 3 - eigenLambda3 - eigenLambda1;
  
  //! Calculates the Sphericity with
  //! S_T = 2 \lambda_2/(\lambda_1 + \lambda_2)
  //! TDR Vol II, May 1999, Page 820
  double sphericity = -99.;
  if (IsTransverseSphericity)
    sphericity = 2.*eigenValues[eigenLambda2]/
      (eigenValues[eigenLambda1] + eigenValues[eigenLambda2]);
  else
    sphericity = (3./2.)*
      (eigenValues[eigenLambda2] + eigenValues[eigenLambda3]);
  
  return sphericity;
}




//-----------------------------------------------------------------------------
// https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/InDetTrackingPerformanceGuidelines
// MC beam spot size sigma_z = 66mm
// Data beam spot size sigma_z = 47mm
//Retunr the number of matching Npv in data given a MC
float GetNVertexBsCorrected(float nRecoVtx){
   
  TGraph *g_nvtx_nreco_bs66mm = new TGraph(41);
  g_nvtx_nreco_bs66mm->SetName("g_shadowedAverage_bs66mm");
  g_nvtx_nreco_bs66mm->SetPoint(0, 0, 0);
  g_nvtx_nreco_bs66mm->SetPoint(1, 1, 1);
  g_nvtx_nreco_bs66mm->SetPoint(2, 2, 1.97943);
  g_nvtx_nreco_bs66mm->SetPoint(3, 3, 2.93912);
  g_nvtx_nreco_bs66mm->SetPoint(4, 4, 3.87986);
  g_nvtx_nreco_bs66mm->SetPoint(5, 5, 4.8024);
  g_nvtx_nreco_bs66mm->SetPoint(6, 6, 5.70743);
  g_nvtx_nreco_bs66mm->SetPoint(7, 7, 6.59561);
  g_nvtx_nreco_bs66mm->SetPoint(8, 8, 7.46756);
  g_nvtx_nreco_bs66mm->SetPoint(9, 9, 8.32386);
  g_nvtx_nreco_bs66mm->SetPoint(10, 10, 9.16509);
  g_nvtx_nreco_bs66mm->SetPoint(11, 11, 9.99175);
  g_nvtx_nreco_bs66mm->SetPoint(12, 12, 10.8044);
  g_nvtx_nreco_bs66mm->SetPoint(13, 13, 11.6034);
  g_nvtx_nreco_bs66mm->SetPoint(14, 14, 12.3893);
  g_nvtx_nreco_bs66mm->SetPoint(15, 15, 13.1624);
  g_nvtx_nreco_bs66mm->SetPoint(16, 16, 13.9233);
  g_nvtx_nreco_bs66mm->SetPoint(17, 17, 14.6723);
  g_nvtx_nreco_bs66mm->SetPoint(18, 18, 15.4097);
  g_nvtx_nreco_bs66mm->SetPoint(19, 19, 16.136);
  g_nvtx_nreco_bs66mm->SetPoint(20, 20, 16.8514);
  g_nvtx_nreco_bs66mm->SetPoint(21, 21, 17.5562);
  g_nvtx_nreco_bs66mm->SetPoint(22, 22, 18.2509);
  g_nvtx_nreco_bs66mm->SetPoint(23, 23, 18.9356);
  g_nvtx_nreco_bs66mm->SetPoint(24, 24, 19.6106);
  g_nvtx_nreco_bs66mm->SetPoint(25, 25, 20.2763);
  g_nvtx_nreco_bs66mm->SetPoint(26, 26, 20.9329);
  g_nvtx_nreco_bs66mm->SetPoint(27, 27, 21.5806);
  g_nvtx_nreco_bs66mm->SetPoint(28, 28, 22.2197);
  g_nvtx_nreco_bs66mm->SetPoint(29, 29, 22.8504);
  g_nvtx_nreco_bs66mm->SetPoint(30, 30, 23.4729);
  g_nvtx_nreco_bs66mm->SetPoint(31, 31, 24.0874);
  g_nvtx_nreco_bs66mm->SetPoint(32, 32, 24.6941);
  g_nvtx_nreco_bs66mm->SetPoint(33, 33, 25.2933);
  g_nvtx_nreco_bs66mm->SetPoint(34, 34, 25.8851);
  g_nvtx_nreco_bs66mm->SetPoint(35, 35, 26.4697);
  g_nvtx_nreco_bs66mm->SetPoint(36, 36, 27.0473);
  g_nvtx_nreco_bs66mm->SetPoint(37, 37, 27.6179);
  g_nvtx_nreco_bs66mm->SetPoint(38, 38, 28.1819);
  g_nvtx_nreco_bs66mm->SetPoint(39, 39, 28.7394);
  g_nvtx_nreco_bs66mm->SetPoint(40, 40, 29.2904);
  
  TGraph *g_nvtx_nreco_bs47mm = new TGraph(41);
  g_nvtx_nreco_bs47mm->SetName("g_shadowedAverage_bs47mm");
  g_nvtx_nreco_bs47mm->SetPoint(0, 0, 0);
  g_nvtx_nreco_bs47mm->SetPoint(1, 1, 1);
  g_nvtx_nreco_bs47mm->SetPoint(2, 2, 1.97111);
  g_nvtx_nreco_bs47mm->SetPoint(3, 3, 2.91499);
  g_nvtx_nreco_bs47mm->SetPoint(4, 4, 3.83313);
  g_nvtx_nreco_bs47mm->SetPoint(5, 5, 4.72692);
  g_nvtx_nreco_bs47mm->SetPoint(6, 6, 5.59763);
  g_nvtx_nreco_bs47mm->SetPoint(7, 7, 6.44645);
  g_nvtx_nreco_bs47mm->SetPoint(8, 8, 7.27445);
  g_nvtx_nreco_bs47mm->SetPoint(9, 9, 8.08265);
  g_nvtx_nreco_bs47mm->SetPoint(10, 10, 8.87199);
  g_nvtx_nreco_bs47mm->SetPoint(11, 11, 9.64332);
  g_nvtx_nreco_bs47mm->SetPoint(12, 12, 10.3975);
  g_nvtx_nreco_bs47mm->SetPoint(13, 13, 11.1352);
  g_nvtx_nreco_bs47mm->SetPoint(14, 14, 11.8572);
  g_nvtx_nreco_bs47mm->SetPoint(15, 15, 12.5641);
  g_nvtx_nreco_bs47mm->SetPoint(16, 16, 13.2567);
  g_nvtx_nreco_bs47mm->SetPoint(17, 17, 13.9353);
  g_nvtx_nreco_bs47mm->SetPoint(18, 18, 14.6007);
  g_nvtx_nreco_bs47mm->SetPoint(19, 19, 15.2532);
  g_nvtx_nreco_bs47mm->SetPoint(20, 20, 15.8935);
  g_nvtx_nreco_bs47mm->SetPoint(21, 21, 16.5219);
  g_nvtx_nreco_bs47mm->SetPoint(22, 22, 17.1389);
  g_nvtx_nreco_bs47mm->SetPoint(23, 23, 17.7449);
  g_nvtx_nreco_bs47mm->SetPoint(24, 24, 18.3404);
  g_nvtx_nreco_bs47mm->SetPoint(25, 25, 18.9255);
  g_nvtx_nreco_bs47mm->SetPoint(26, 26, 19.5008);
  g_nvtx_nreco_bs47mm->SetPoint(27, 27, 20.0665);
  g_nvtx_nreco_bs47mm->SetPoint(28, 28, 20.623);
  g_nvtx_nreco_bs47mm->SetPoint(29, 29, 21.1705);
  g_nvtx_nreco_bs47mm->SetPoint(30, 30, 21.7094);
  g_nvtx_nreco_bs47mm->SetPoint(31, 31, 22.2399);
  g_nvtx_nreco_bs47mm->SetPoint(32, 32, 22.7622);
  g_nvtx_nreco_bs47mm->SetPoint(33, 33, 23.2767);
  g_nvtx_nreco_bs47mm->SetPoint(34, 34, 23.7835);
  g_nvtx_nreco_bs47mm->SetPoint(35, 35, 24.2829);
  g_nvtx_nreco_bs47mm->SetPoint(36, 36, 24.7751);
  g_nvtx_nreco_bs47mm->SetPoint(37, 37, 25.2604);
  g_nvtx_nreco_bs47mm->SetPoint(38, 38, 25.7388);
  g_nvtx_nreco_bs47mm->SetPoint(39, 39, 26.2106);
  g_nvtx_nreco_bs47mm->SetPoint(40, 40, 26.6759);
  
  
  //get corresponding NReconstructible (points are already sorted in X, monotonic in Y)
  float nRecon=-1;
  if (nRecoVtx < g_nvtx_nreco_bs66mm->GetY()[0]) {
    //std::cout << "NVtx_bs_correction.C: WARNING: Requested nVertex outside the expected range: " << nRecoVtx << std::endl;
    return nRecoVtx; //do not correct
  }
  if (nRecoVtx > g_nvtx_nreco_bs66mm->GetY()[g_nvtx_nreco_bs66mm->GetN()-1]) {
    //std::cout << "NVtx_bs_correction.C: WARNING: Requested nVertex outside the expected range: " << nRecoVtx << std::endl;
    return nRecoVtx; //do not correct 
  }
  for (int i=1; i < g_nvtx_nreco_bs66mm->GetN(); i++) {
    if (nRecoVtx < g_nvtx_nreco_bs66mm->GetY()[i]) {
      //linear interpolation
      nRecon = g_nvtx_nreco_bs66mm->GetX()[i-1]+(nRecoVtx - (g_nvtx_nreco_bs66mm->GetY()[i-1])) *
	(g_nvtx_nreco_bs66mm->GetX()[i] - g_nvtx_nreco_bs66mm->GetX()[i-1]) / 
	(g_nvtx_nreco_bs66mm->GetY()[i] - g_nvtx_nreco_bs66mm->GetY()[i-1]);
       
      break;
    }
  }
  
  //now return corresponding reconstructed vertices for bs=47mm

  float val = g_nvtx_nreco_bs47mm->Eval(nRecon);

  delete g_nvtx_nreco_bs47mm;
  delete g_nvtx_nreco_bs66mm;

  return val;
  
 }


//-----------------------------------------------------------------------------
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
    //std::cout << "getCrossSection::WARNING No match is found for MCID :: " << _dsId << std::endl;
    //std::cout << "                         Returning input value..."     << std::endl;
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
