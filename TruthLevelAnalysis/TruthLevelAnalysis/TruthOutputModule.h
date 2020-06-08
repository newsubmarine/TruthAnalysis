#pragma once

#include <ELCore/Module.h>
#include "xAODTruth/TruthParticleContainer.h"

namespace LHAPDF{ class GridPDF; }

class TruthOutputModule : public Module
{
 public:
 TruthOutputModule(): Module(){}
  virtual ~TruthOutputModule(){}

  virtual void initialize();

 private:

  std::vector<TLorentzVector> getP4(std::string keys);
  std::vector<TLorentzVector> getP4(std::vector<const xAOD::TruthParticle*> pars);

  int getParNum(const std::string& key);
  int getJetsNum(const std::string& key);
  int getTotalCharge(const std::string& key1, const std::string& key2);
  std::vector<int> getHiggsMode(const std::string& key);

  float getMass(std::string key);
  float getPt(std::string key);
  float getEta(std::string key);
  float getPhi(std::string key);
  float getDeltaR(std::string key);
  float getDeltaPhi(std::string key);
  float getDeltaEta(std::string key);
  float getMassBQuarks(std::string key, std::string target);
  float getPtBQuarks(std::string key, std::string target);
  float getEtaBQuarks(std::string key, std::string target);
  float getPhiBQuarks(std::string key, std::string target);
  float getDeltaRBQuarks(std::string key, std::string target);
  float getDeltaPhiBQuarks(std::string key, std::string target);
  float getDeltaEtaBQuarks(std::string key, std::string target);
  
  std::vector<float> getPtVector(std::string key);
  std::vector<float> getEtaVector(std::string key);
  std::vector<float> getPhiVector(std::string key);
  std::vector<float> getPtVectorBQuarks(std::string key, std::string target);
  std::vector<float> getEtaVectorBQuarks(std::string key, std::string target);
  std::vector<float> getPhiVectorBQuarks(std::string key, std::string target);

  std::vector<const xAOD::TruthParticle*> getBQuarks(const std::string & key, const std::string & target);

  const xAOD::TruthParticle* loopSelf(const xAOD::TruthParticle* par);

  ClassDef(TruthOutputModule, 1);
};
