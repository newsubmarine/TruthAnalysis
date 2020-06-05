#include <TruthLevelAnalysis/TruthOutputModule.h>
#include <TruthLevelAnalysis/CommonTruthSelectionModule.h>
#include <TruthLevelAnalysis/CombinedSelection.h>

#include <vector>
#include <TLorentzVector.h>

#ifdef __MAKECINT__
#pragma link C++ class vector<TLorentzVector>+;
#endif

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;

#endif

#ifdef __CINT__
#pragma link C++ class CommonTruthSelectionModule+;
#pragma link C++ class TruthOutputModule;
#pragma link C++ class CombinedSelection+;
#endif
