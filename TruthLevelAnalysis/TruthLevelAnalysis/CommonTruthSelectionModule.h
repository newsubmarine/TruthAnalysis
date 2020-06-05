#pragma once

#include <ELCore/Module.h>
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODJet/JetContainer.h"
class LPXKfactorTool;

class CommonTruthSelectionModule : public Module
{
 public:
 CommonTruthSelectionModule(): Module(){}
  virtual ~CommonTruthSelectionModule(){}

  virtual void initialize();
  static bool compareByPtAndPtvis(xAOD::TruthParticle* a, xAOD::TruthParticle* b);

 private:

  void fetchTruthParticles(std::string truth_key, std::string store_key);
  void fetchJets(std::string key, std::string store_key);
  bool vbfJetSel(xAOD::Jet* jet, std::string store_key);
  void fetchMET(std::string key, std::string store_key, std::string term_key);
  void fetchEventInfo(const std::string& eventinfo_key);
  void fetchTruthEvent(std::string key);
  void print(const std::string& key);
  void printMother(const std::string& key);
  bool performOLR(xAOD::IParticle* particle, const std::string& other_key, double dR);
  double nEqual(const std::string& container_key, int quantity);
  double nKeyEqualKey(const std::string& container_key1, const std::string& container_key2);
  double nLepEqual(const std::string& container_key1, const std::string& container_key2, int quantity);
  double nLepWithin(const std::string& container_key1,const std::string& container_key2, int low, int high);
  double nMin(const std::string& key, int quantity);
  double higgsDecayModeCut(const std::string& key);
  double decayModeCut(const std::string& store_key, int n_TauH, int n_TauL);
  bool pidSel(xAOD::TruthParticle* higgs, int absPdgId);
  bool isoLepSel(xAOD::TruthParticle* par);
  bool tauSel(xAOD::TruthParticle* tau, double pTcut, double etacut);
  bool eleSel(xAOD::TruthParticle* electron, double pTcut, double etacut);
  bool muonSel(xAOD::TruthParticle* muon, double pTcut, double etacut);
  bool simEff(xAOD::TruthParticle* par, double prob);
  double minPt(std::string container, int position, double minPt);
  double minTauPt(std::string container, int position, double minPt);
  double applyMcWeight(std::string truth_key);
  double doNothing();
  bool jetSel(xAOD::Jet* jet, double minPt, double maxAbsEta);
  bool tranRegionCut(xAOD::TruthParticle* par);
  void calcBweight(std::string jet_key, std::string tau_key);
  void mergeContainers(std::string truth_keys, std::string store_key);
  const xAOD::TruthParticle* getMother(const xAOD::TruthParticle* ) const;

  std::shared_ptr<LPXKfactorTool> m_LPXKfactorTool; //!
  double ApplyLPXKfactor();

  ClassDef(CommonTruthSelectionModule, 1);
};
