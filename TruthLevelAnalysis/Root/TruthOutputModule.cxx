#include <TruthLevelAnalysis/TruthOutputModule.h>
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODTruth/TruthEventContainer.h"
#include "LHAPDF/PDFSet.h"
#include "LHAPDF/PDF.h"
#include "LHAPDF/GridPDF.h"
#include "LHAPDF/Reweighting.h"
#include "TMath.h"
#include <random>

//_____________________________________________________________________________

void TruthOutputModule::initialize (){

  // Basic event infomation
  std::function<int()> f_nTausH = std::bind(&TruthOutputModule::getParNum, this, "kTausH");
  core()->AddTreeBranch("nTausH", f_nTausH, "all");
  std::function<int()> f_nEles = std::bind(&TruthOutputModule::getParNum, this, "kElectrons");
  core()->AddTreeBranch("nEles", f_nEles, "all");
  std::function<int()> f_nMus = std::bind(&TruthOutputModule::getParNum, this, "kMuons");
  core()->AddTreeBranch("nMus", f_nMus, "all");
  std::function<int()> f_nPhotons = std::bind(&TruthOutputModule::getParNum, this, "kPhotons");
  core()->AddTreeBranch("nPhotons", f_nPhotons, "all");
  
  std::function<std::vector<int>()> f_higgsMode = std::bind(&TruthOutputModule::getHiggsMode, this, "kHiggs");
  core()->AddTreeBranch("higgsDecayMode", f_higgsMode, "all");
  std::function<int()> f_totalCharge = std::bind(&TruthOutputModule::getTotalCharge, this, "kElectrons","kMuons");
  core()->AddTreeBranch("emuCharge", f_totalCharge, "all");

  // HH Kinematics
  // mass
  std::function<float()> f_dihiggsMass = std::bind(&TruthOutputModule::getMass, this, "kHiggs");
  core()->AddTreeBranch("dihiggsMass", f_dihiggsMass, "all");
  // pt
  std::function<float()> f_dihiggsPt = std::bind(&TruthOutputModule::getPt, this, "kHiggs");
  core()->AddTreeBranch("dihiggsPt", f_dihiggsPt, "all");
  // eta
  std::function<float()> f_dihiggsEta = std::bind(&TruthOutputModule::getEta, this, "kHiggs");
  core()->AddTreeBranch("dihiggsEta", f_dihiggsEta, "all");
  // phi
  std::function<float()> f_dihiggsPhi = std::bind(&TruthOutputModule::getPhi, this, "kHiggs");
  core()->AddTreeBranch("dihiggsPhi", f_dihiggsPhi, "all");
  // deltaR
  std::function<float()> f_dihiggsDeltaR = std::bind(&TruthOutputModule::getDeltaR, this, "kHiggs");
  core()->AddTreeBranch("dihiggsDeltaR", f_dihiggsDeltaR, "all");
  // deltaEta
  std::function<float()> f_dihiggsDeltaEta = std::bind(&TruthOutputModule::getDeltaEta, this, "kHiggs");
  core()->AddTreeBranch("dihiggsDeltaEta", f_dihiggsDeltaEta, "all");
  // deltaPhi
  std::function<float()> f_dihiggsDeltaPhi = std::bind(&TruthOutputModule::getDeltaPhi, this, "kHiggs");
  core()->AddTreeBranch("dihiggsDeltaPhi", f_dihiggsDeltaPhi, "all");

  // H Kinematics
  // mass
  auto f_higgsMass = core()->ApplyToElements(&xAOD::TruthParticle::m, &CycleBase::getContainer<xAOD::TruthParticle>, "kHiggs");
  core()->AddTreeBranch("higgsMass", f_higgsMass, "all");
  // pt
  auto f_higgsPt = core()->ApplyToElements(&xAOD::TruthParticle::pt, &CycleBase::getContainer<xAOD::TruthParticle>, "kHiggs");
  core()->AddTreeBranch("higgsPt", f_higgsPt, "all");
  // eta
  auto f_higgsEta = core()->ApplyToElements(&xAOD::TruthParticle::eta, &CycleBase::getContainer<xAOD::TruthParticle>, "kHiggs");
  core()->AddTreeBranch("higgsEta", f_higgsEta, "all");
  // phi
  auto f_higgsPhi = core()->ApplyToElements(&xAOD::TruthParticle::phi, &CycleBase::getContainer<xAOD::TruthParticle>, "kHiggs");
  core()->AddTreeBranch("higgsPhi", f_higgsPhi, "all");
  
  // Z Kinematics
  // count
  std::function<int()> f_nZ = std::bind(&TruthOutputModule::getParNum, this, "kZ");
  core()->AddTreeBranch("nZ", f_nZ, "all");
  // mass
  auto f_zMass = core()->ApplyToElements(&xAOD::TruthParticle::m, &CycleBase::getContainer<xAOD::TruthParticle>, "kZ");
  core()->AddTreeBranch("zMass", f_zMass, "all");
  // pt
  auto f_zPt = core()->ApplyToElements(&xAOD::TruthParticle::pt, &CycleBase::getContainer<xAOD::TruthParticle>, "kZ");
  core()->AddTreeBranch("zPt", f_zPt, "all");
  // eta
  auto f_zEta = core()->ApplyToElements(&xAOD::TruthParticle::eta, &CycleBase::getContainer<xAOD::TruthParticle>, "kZ");
  core()->AddTreeBranch("zEta", f_zEta, "all");
  // phi
  auto f_zPhi = core()->ApplyToElements(&xAOD::TruthParticle::phi, &CycleBase::getContainer<xAOD::TruthParticle>, "kZ");
  core()->AddTreeBranch("zPhi", f_zPhi, "all");

  // W kinematics
  // count
  std::function<int()> f_nW = std::bind(&TruthOutputModule::getParNum, this, "kW");
  core()->AddTreeBranch("nW", f_nW, "all");
  auto f_wMass = core()->ApplyToElements(&xAOD::TruthParticle::m, &CycleBase::getContainer<xAOD::TruthParticle>, "kW");
  core()->AddTreeBranch("wMass", f_wMass, "all");
  // pt
  auto f_wPt = core()->ApplyToElements(&xAOD::TruthParticle::pt, &CycleBase::getContainer<xAOD::TruthParticle>, "kW");
  core()->AddTreeBranch("wPt", f_wPt, "all");
  // eta
  auto f_wEta = core()->ApplyToElements(&xAOD::TruthParticle::eta, &CycleBase::getContainer<xAOD::TruthParticle>, "kW");
  core()->AddTreeBranch("wEta", f_wEta, "all");
  // phi
  auto f_wPhi = core()->ApplyToElements(&xAOD::TruthParticle::phi, &CycleBase::getContainer<xAOD::TruthParticle>, "kW");
  core()->AddTreeBranch("wPhi", f_wPhi, "all");
  
  // diphoton kinematics (leading and subleading)
  // mass
  std::function<float()> f_diphotonMass = std::bind(&TruthOutputModule::getMass, this, "kPhotons");
  core()->AddTreeBranch("diphotonMass", f_diphotonMass, "all");
  // pt
  std::function<float()> f_diphotonPt = std::bind(&TruthOutputModule::getPt, this, "kPhotons");
  core()->AddTreeBranch("diphotonPt", f_diphotonPt, "all");
  // eta
  std::function<float()> f_diphotonEta = std::bind(&TruthOutputModule::getEta, this, "kPhotons");
  core()->AddTreeBranch("diphotonEta", f_diphotonEta, "all");
  // phi
  std::function<float()> f_diphotonPhi = std::bind(&TruthOutputModule::getPhi, this, "kPhotons");
  core()->AddTreeBranch("diphotonPhi", f_diphotonPhi, "all");
  // deltaR
  std::function<float()> f_diphotonDeltaR = std::bind(&TruthOutputModule::getDeltaR, this, "kPhotons");
  core()->AddTreeBranch("diphotonDeltaR", f_diphotonDeltaR, "all");
  // deltaEta
  std::function<float()> f_diphotonDeltaEta = std::bind(&TruthOutputModule::getDeltaEta, this, "kPhotons");
  core()->AddTreeBranch("diphotonDeltaEta", f_diphotonDeltaEta, "all");
  // deltaPhi
  std::function<float()> f_diphotonDeltaPhi = std::bind(&TruthOutputModule::getDeltaPhi, this, "kPhotons");
  core()->AddTreeBranch("diphotonDeltaPhi", f_diphotonDeltaPhi, "all");
  
  // photon kinematics
  // pt
  auto f_photonPt = core()->ApplyToElements(&xAOD::TruthParticle::pt, &CycleBase::getContainer<xAOD::TruthParticle>, "kPhotons");
  core()->AddTreeBranch("photonPt", f_photonPt, "all");
  // eta
  auto f_photonEta = core()->ApplyToElements(&xAOD::TruthParticle::eta, &CycleBase::getContainer<xAOD::TruthParticle>, "kPhotons");
  core()->AddTreeBranch("photonEta", f_photonEta, "all");
  // phi
  auto f_photonPhi = core()->ApplyToElements(&xAOD::TruthParticle::phi, &CycleBase::getContainer<xAOD::TruthParticle>, "kPhotons");
  core()->AddTreeBranch("photonPhi", f_photonPhi, "all");
  
  // multi-lepton kinematics (leading and subleading)
  // mass
  std::function<float()> f_multileptonsMass = std::bind(&TruthOutputModule::getMass, this, "kLeptons");
  core()->AddTreeBranch("multileptonsMass", f_multileptonsMass, "all");
  // pt
  std::function<float()> f_multileptonsPt = std::bind(&TruthOutputModule::getPt, this, "kLeptons");
  core()->AddTreeBranch("multileptonsPt", f_multileptonsPt, "all");
  // eta
  std::function<float()> f_multileptonsEta = std::bind(&TruthOutputModule::getEta, this, "kLeptons");
  core()->AddTreeBranch("multileptonsEta", f_multileptonsEta, "all");
  // phi
  std::function<float()> f_multileptonsPhi = std::bind(&TruthOutputModule::getPhi, this, "kLeptons");
  core()->AddTreeBranch("multileptonsPhi", f_multileptonsPhi, "all");
  // deltaR
  std::function<float()> f_multileptonsDeltaR = std::bind(&TruthOutputModule::getDeltaR, this, "kLeptons");
  core()->AddTreeBranch("multileptonsDeltaR", f_multileptonsDeltaR, "all");
  // deltaEta
  std::function<float()> f_multileptonsDeltaEta = std::bind(&TruthOutputModule::getDeltaEta, this, "kLeptons");
  core()->AddTreeBranch("multileptonsDeltaEta", f_multileptonsDeltaEta, "all");
  // deltaPhi
  std::function<float()> f_multileptonsDeltaPhi = std::bind(&TruthOutputModule::getDeltaPhi, this, "kLeptons");
  core()->AddTreeBranch("multileptonsDeltaPhi", f_multileptonsDeltaPhi, "all");
 
  // lepton kinematics
  // num
  std::function<int()> f_nLep = std::bind(&TruthOutputModule::getParNum, this, "kLeptons");
  core()->AddTreeBranch("nLep", f_nLep, "all");
  // pt
  std::function<std::vector<float>()> f_leptonPt = std::bind(&TruthOutputModule::getPtVector, this, "kLeptons");
  core()->AddTreeBranch("leptonPt", f_leptonPt, "all");
  // eta
  std::function<std::vector<float>()> f_leptonEta = std::bind(&TruthOutputModule::getEtaVector, this, "kLeptons");
  core()->AddTreeBranch("leptonEta", f_leptonEta, "all");
  // phi
  std::function<std::vector<float>()> f_leptonPhi = std::bind(&TruthOutputModule::getPhiVector, this, "kLeptons");
  core()->AddTreeBranch("leptonPhi", f_leptonPhi, "all");
  
  //electron kinematics
  // pt
  auto f_electronPt = core()->ApplyToElements(&xAOD::TruthParticle::pt, &CycleBase::getContainer<xAOD::TruthParticle>, "kElectrons");
  core()->AddTreeBranch("electronPt", f_electronPt, "all");
  // eta
  auto f_electronEta = core()->ApplyToElements(&xAOD::TruthParticle::eta, &CycleBase::getContainer<xAOD::TruthParticle>, "kElectrons");
  core()->AddTreeBranch("electronEta", f_electronEta, "all");
  // phi
  auto f_electronPhi = core()->ApplyToElements(&xAOD::TruthParticle::phi, &CycleBase::getContainer<xAOD::TruthParticle>, "kElectrons");
  core()->AddTreeBranch("electronPhi", f_electronPhi, "all");

  //muon kinematics
  // pt
  auto f_muonPt = core()->ApplyToElements(&xAOD::TruthParticle::pt, &CycleBase::getContainer<xAOD::TruthParticle>, "kMuons");
  core()->AddTreeBranch("muonPt", f_muonPt, "all");
  // eta
  auto f_muonEta = core()->ApplyToElements(&xAOD::TruthParticle::eta, &CycleBase::getContainer<xAOD::TruthParticle>, "kMuons");
  core()->AddTreeBranch("muonEta", f_muonEta, "all");
  // phi
  auto f_muonPhi = core()->ApplyToElements(&xAOD::TruthParticle::phi, &CycleBase::getContainer<xAOD::TruthParticle>, "kMuons");
  core()->AddTreeBranch("muonPhi", f_muonPhi, "all");

  // tau kinematics
  // pt
  std::function<std::vector<float>()> f_tauPt = std::bind(&TruthOutputModule::getPtVector, this, "kTausH");
  core()->AddTreeBranch("tauPt", f_tauPt, "all");
  // eta
  std::function<std::vector<float>()> f_tauEta = std::bind(&TruthOutputModule::getEtaVector, this, "kTausH");
  core()->AddTreeBranch("tauEta", f_tauEta, "all");
  // phi
  std::function<std::vector<float>()> f_tauPhi = std::bind(&TruthOutputModule::getPhiVector, this, "kTausH");
  core()->AddTreeBranch("tauPhi", f_tauPhi, "all");

#if 1
  std::function<int()> f_nVBFJets = std::bind(&TruthOutputModule::getParNum, this, "kVBFJets");
  core()->AddTreeBranch("nVBFJets", f_nVBFJets, "all");
  // jet kinematics
  // mass
  std::function<float()> f_vbfJetsMass = std::bind(&TruthOutputModule::getMass, this, "kVBFJets");
  core()->AddTreeBranch("vbfJetsMass", f_vbfJetsMass, "all");
  // pt
  std::function<float()> f_vbfJetsPt = std::bind(&TruthOutputModule::getPt, this, "kVBFJets");
  core()->AddTreeBranch("vbfJetsPt", f_vbfJetsPt, "all");
  // eta
  std::function<float()> f_vbfJetsEta = std::bind(&TruthOutputModule::getEta, this, "kVBFJets");
  core()->AddTreeBranch("vbfJetsEta", f_vbfJetsEta, "all");
  // phi
  std::function<float()> f_vbfJetsPhi = std::bind(&TruthOutputModule::getPhi, this, "kVBFJets");
  core()->AddTreeBranch("vbfJetsPhi", f_vbfJetsPhi, "all");
  // deltaR
  std::function<float()> f_vbfJetsDeltaR = std::bind(&TruthOutputModule::getDeltaR, this, "kVBFJets");
  core()->AddTreeBranch("vbfJetsDeltaR", f_vbfJetsDeltaR, "all");
  // deltaEta
  std::function<float()> f_vbfJetsDeltaEta = std::bind(&TruthOutputModule::getDeltaEta, this, "kVBFJets");
  core()->AddTreeBranch("vbfJetsDeltaEta", f_vbfJetsDeltaEta, "all");
  // deltaPhi
  std::function<float()> f_vbfJetsDeltaPhi = std::bind(&TruthOutputModule::getDeltaPhi, this, "kVBFJets");
  core()->AddTreeBranch("vbfJetsDeltaPhi", f_vbfJetsDeltaPhi, "all");

  // pt
  auto f_vbfJetPt = core()->ApplyToElements(&xAOD::TruthParticle::pt, &CycleBase::getContainer<xAOD::TruthParticle>, "kVBFJets");
  core()->AddTreeBranch("vbfJetPt", f_vbfJetPt, "all");
  // eta
  auto f_vbfJetEta = core()->ApplyToElements(&xAOD::TruthParticle::eta, &CycleBase::getContainer<xAOD::TruthParticle>, "kVBFJets");
  core()->AddTreeBranch("vbfJetEta", f_vbfJetEta, "all");
  // phi
  auto f_vbfJetPhi = core()->ApplyToElements(&xAOD::TruthParticle::phi, &CycleBase::getContainer<xAOD::TruthParticle>, "kVBFJets");
  core()->AddTreeBranch("vbfJetPhi", f_vbfJetPhi, "all");
#endif
}


std::vector<TLorentzVector> TruthOutputModule::getP4(std::string key)
{
  std::vector<TLorentzVector> result;
  result.clear();

  auto particles = core()->getContainer<xAOD::TruthParticle>(key);
  for (auto par : *particles) {
    if (par->isAvailable<double>("pt_vis")) {
      TLorentzVector tauP4Vis;
      tauP4Vis.SetPtEtaPhiM(par->auxdata<double>("pt_vis"), 
                            par->auxdata<double>("eta_vis"),
                            par->auxdata<double>("phi_vis"),
                            par->auxdata<double>("m_vis"));
      result.push_back(tauP4Vis);
    }
    else {
      result.push_back(par->p4());
    }
  }
  
  return result;
}


float TruthOutputModule::getMass(std::string key)
{
  std::vector<TLorentzVector> parP4 = this->getP4(key);
  if (parP4.size() < 2)  return -999;
  TLorentzVector totalP4 = parP4[0]+parP4[1];

  return totalP4.M();
}

float TruthOutputModule::getPt(std::string key)
{
  std::vector<TLorentzVector> parP4 = this->getP4(key);
  if (parP4.size() < 2)  return -999;
  TLorentzVector totalP4 = parP4[0]+parP4[1];
  
  return totalP4.Pt();
}

float TruthOutputModule::getEta(std::string key)
{
  std::vector<TLorentzVector> parP4 = this->getP4(key);
  if (parP4.size() < 2)  return -999;
  TLorentzVector totalP4 = parP4[0]+parP4[1];
  
  return totalP4.Eta();
}

float TruthOutputModule::getPhi(std::string key)
{
  std::vector<TLorentzVector> parP4 = this->getP4(key);
  if (parP4.size() < 2)  return -999;
  TLorentzVector totalP4 = parP4[0]+parP4[1];
  
  return totalP4.Phi();
}

float TruthOutputModule::getDeltaR(std::string key)
{
  std::vector<TLorentzVector> parP4 = this->getP4(key);
  if (parP4.size() < 2)  return -999;

  return parP4[0].DeltaR(parP4[1]);
}

float TruthOutputModule::getDeltaPhi(std::string key)
{
  std::vector<TLorentzVector> parP4 = this->getP4(key);
  if (parP4.size() < 2)  return -999;

  return parP4[0].DeltaPhi(parP4[1]);
}

float TruthOutputModule::getDeltaEta(std::string key)
{
  std::vector<TLorentzVector> parP4 = this->getP4(key);
  if (parP4.size() < 2)  return -999;

  return (parP4[0].Eta() - parP4[1].Eta());
}

std::vector<float> TruthOutputModule::getPtVector(std::string key)
{
  std::vector<TLorentzVector> parP4s = this->getP4(key);
  std::vector<float> result;
  result.clear();

  for (auto parP4 : parP4s) {
    result.push_back(parP4.Pt());
  }
  return result;
}

std::vector<float> TruthOutputModule::getEtaVector(std::string key)
{
  std::vector<TLorentzVector> parP4s = this->getP4(key);
  std::vector<float> result;
  result.clear();

  for (auto parP4 : parP4s) {
    result.push_back(parP4.Eta());
  }
  return result;
}

std::vector<float> TruthOutputModule::getPhiVector(std::string key)
{
  std::vector<TLorentzVector> parP4s = this->getP4(key);
  std::vector<float> result;
  result.clear();

  for (auto parP4 : parP4s) {
    result.push_back(parP4.Phi());
  }
  return result;
}

/// Sum of Charge
int TruthOutputModule::getTotalCharge(const std::string & key1, const std::string & key2){
  auto pars1 = core()->getContainer<xAOD::TruthParticle>(key1);
  auto pars2 = core()->getContainer<xAOD::TruthParticle>(key2);
  int charge = 0;
  for (auto par: *pars1)
    charge += par->charge();
  for (auto par: *pars2)
    charge += par->charge();
  return charge;
}

/// Num Of Particles
int TruthOutputModule::getParNum(const std::string & key){
  auto pars = core()->getContainer<xAOD::TruthParticle>(key);
  return pars->size();
}

/// Num Of Particles
int TruthOutputModule::getJetsNum(const std::string & key){
  auto pars = core()->getContainer<xAOD::Jet>(key);
  return pars->size();
}

/// Higgs Decay Mode
/// modes such as gamma Z0 are not included
std::vector<int> TruthOutputModule::getHiggsMode(const std::string & key){
  std::vector<int> decaymode;
  decaymode.clear();
  auto pars = core()->getContainer<xAOD::TruthParticle>(key);
  for (auto par : *pars) 
  {
    if (25!=par->pdgId()) continue;
    if (0==par->nChildren()) continue;
    if (25==par->child(0)->pdgId())  continue;
    
    int child0_pdg = par->child(0)->absPdgId();
    int child1_pdg = par->child(1)->absPdgId();
	
    if (child0_pdg!=child1_pdg) 
	child0_pdg = 0;
    decaymode.push_back(child0_pdg);
  }
  return decaymode;  
}
