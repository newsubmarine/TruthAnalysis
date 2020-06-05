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
  
  std::vector<float> getPtVector(std::string key);
  std::vector<float> getEtaVector(std::string key);
  std::vector<float> getPhiVector(std::string key);

  ClassDef(TruthOutputModule, 1);
};
