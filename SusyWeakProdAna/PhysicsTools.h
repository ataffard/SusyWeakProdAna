#ifndef SusyWeakProdAna_PhysicsTools_h
#define SusyWeakProdAna_PhysicsTools_h

#include <math.h>

// Root Packages
#include "TLorentzVector.h"

// Classify using MCTruthClassifier
enum LEP_TYPE{PR=0, HF, LF, CONV, TYPE_Undef};

bool isHF(int org, int type);
bool isPT(int org, int type);
bool isConv(int org, int type);
bool isLF(int org, int type);
LEP_TYPE  getType(int org, int type, std::string dataset="");

//Efficiency using binomial error
void binomialError(float Num, float Den, float& Eff, float& EffErr);


//NVtx correction for data/MC beam size
float GetNVertexBsCorrected(float nRecoVtx);

//
//Physics quantities
//

//Transverse mass
float mT(TLorentzVector _l, TLorentzVector _nu);

// Signed d0 wrt to jet axis 
float signedD0(float d0, float sigmaD0, TLorentzVector _p, TLorentzVector _j);

// pT relative to jet axis 
float ptRel(TLorentzVector j, TLorentzVector p);

#endif
