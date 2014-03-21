#ifndef SusyWeakProdAna_PhysicsTools_h
#define SusyWeakProdAna_PhysicsTools_h

#include <math.h>

// Root Packages
#include "TLorentzVector.h"


//Efficiency using binomial error
void binomialError(float Num, float Den, float& Eff, float& EffErr);


//NVtx correction for data/MC beam size
float GetNVertexBsCorrected(float nRecoVtx);

//
//Physics quantities
//

//Transverse mass
float mT(TLorentzVector _l, TLorentzVector _nu);

//Transverse mass WW
float mTWW(TLorentzVector _ll, TLorentzVector _nu, bool MvvTrue=true);

// Signed d0 wrt to jet axis 
float signedD0(float d0, float sigmaD0, TLorentzVector _p, TLorentzVector _j);

// pT relative to jet axis 
float ptRel(TLorentzVector j, TLorentzVector p);


/*
  References: mCT.0802.2879v3 & mCT.0910.1584v2
*/
//mCT
float mCT(TLorentzVector v1, TLorentzVector v2);

//mCTperp
float mCTperp(TLorentzVector lep0, TLorentzVector lep1, TLorentzVector met);

//mCTpara
float mCTpara(TLorentzVector lep0, TLorentzVector lep1, TLorentzVector met);

//mll collinear approx
float mZTauTau(TLorentzVector l0, TLorentzVector l1, TLorentzVector met);

//Acoplananrity
float acoplanarity(TLorentzVector v0, TLorentzVector v1);

//Sphericity
float Sphericity(std::vector<float> &pxVector,
                 std::vector<float> &pyVector,
                 std::vector<float> &pzVector,
                 bool IsTransverseSphericity);



#endif
