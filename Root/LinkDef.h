#ifndef SUSYWEAKPRODANA_LINKDEF_H
#define SUSYWEAKPRODANA_LINKDEF_H

#include <vector>
#include <map>
#include <set>
#include <string>

#include "SusyWeakProdAna/Histos_Fake.h"
#include "SusyWeakProdAna/Histos_2L.h"
#include "SusyWeakProdAna/Histos_WH.h"
#include "SusyWeakProdAna/Histos_3L.h"
#include "SusyWeakProdAna/SusyHistos.h"
#include "SusyWeakProdAna/DrawPlots.h"

#include "SusyWeakProdAna/PhysicsTools.h"

#include "SusyWeakProdAna/ToyNt.h"

#include "SusyWeakProdAna/SusyAnaCommon.h"
#include "SusyWeakProdAna/SusySelection.h"
#include "SusyWeakProdAna/SusyBaseAna.h"
#include "SusyWeakProdAna/SusyFakeAna.h"
#include "SusyWeakProdAna/Susy2LepAna.h"
#include "SusyWeakProdAna/SusyWHAna.h"
#include "SusyWeakProdAna/Susy3LepAna.h"
#include "SusyWeakProdAna/SusyAnaLooper.h"
#include "SusyWeakProdAna/ToyNtAna.h"
#include "SusyWeakProdAna/ToyNt_ZXStudies.h"
#include "SusyWeakProdAna/ToyNt_SROptimization.h"
//#include "SusyWeakProdAna/SROptimization.h"


#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;

#pragma link C++ class Histos_Fake+;
#pragma link C++ class Histos_2L+;
#pragma link C++ class Histos_WH+;
#pragma link C++ class Histos_3L+;
#pragma link C++ class SusyHistos+;
#pragma link C++ class DrawPlots+;

//#pragma link C++ class PhysicsTools+;

#pragma link C++ class ToyNt+;

#pragma link C++ class SusySelection+;
#pragma link C++ class SusyBaseAna+;
#pragma link C++ class SusyFakeAna+;
#pragma link C++ class Susy2LepAna+;
#pragma link C++ class SusyWHAna+;
#pragma link C++ class Susy3LepAna+;
#pragma link C++ class SusyAnaLooper+;
#pragma link C++ class ToyNtAna+;
#pragma link C++ class ToyNt_SROptimization+;
#pragma link C++ class ToyNt_ZXStudies+;
//#pragma link C++ class SROptimization+;

#pragma link C++ class std::vector< TH1F* >+;
#pragma link C++ class std::vector< std::vector< TH1F* > >+;

#endif

#endif

