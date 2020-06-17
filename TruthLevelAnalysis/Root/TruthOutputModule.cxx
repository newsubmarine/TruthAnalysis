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
  std::function<int()> f_nJets = std::bind(&TruthOutputModule::getJetsNum, this, "kJets");
  core()->AddTreeBranch("nJets", f_nJets, "all");
  std::function<int()> f_nVBFJets = std::bind(&TruthOutputModule::getJetsNum, this, "kVBFJets");
  core()->AddTreeBranch("nVBFJets", f_nVBFJets, "all");
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
  auto f_vbfJetPt = core()->ApplyToElements(&xAOD::Jet::pt, &CycleBase::getContainer<xAOD::Jet>, "kVBFJets");
  core()->AddTreeBranch("vbfJetPt", f_vbfJetPt, "all");
  // eta
  auto f_vbfJetEta = core()->ApplyToElements(&xAOD::Jet::eta, &CycleBase::getContainer<xAOD::Jet>, "kVBFJets");
  core()->AddTreeBranch("vbfJetEta", f_vbfJetEta, "all");
  // phi
  auto f_vbfJetPhi = core()->ApplyToElements(&xAOD::Jet::phi, &CycleBase::getContainer<xAOD::Jet>, "kVBFJets");
  core()->AddTreeBranch("vbfJetPhi", f_vbfJetPhi, "all");


  // b quark kinematics
  // mass
  std::function<float()> f_bQuarksMass = std::bind(&TruthOutputModule::getMassBQuarks, this, "kHiggs", "bquarks");
  core()->AddTreeBranch("bQuarksMass", f_bQuarksMass, "all");
  // pt
  std::function<float()> f_bQuarksPt = std::bind(&TruthOutputModule::getPtBQuarks, this, "kHiggs", "bquarks");
  core()->AddTreeBranch("bQuarksPt", f_bQuarksPt, "all");
  // eta
  std::function<float()> f_bQuarksEta = std::bind(&TruthOutputModule::getEtaBQuarks, this, "kHiggs", "bquarks");
  core()->AddTreeBranch("bQuarksEta", f_bQuarksEta, "all");
  // phi
  std::function<float()> f_bQuarksPhi = std::bind(&TruthOutputModule::getPhiBQuarks, this, "kHiggs", "bquarks");
  core()->AddTreeBranch("bQuarksPhi", f_bQuarksPhi, "all");
  // deltaR
  std::function<float()> f_bQuarksDeltaR = std::bind(&TruthOutputModule::getDeltaRBQuarks, this, "kHiggs", "bquarks");
  core()->AddTreeBranch("bQuarksDeltaR", f_bQuarksDeltaR, "all");
  // deltaEta
  std::function<float()> f_bQuarksDeltaEta = std::bind(&TruthOutputModule::getDeltaEtaBQuarks, this, "kHiggs", "bquarks");
  core()->AddTreeBranch("bQuarksDeltaEta", f_bQuarksDeltaEta, "all");
  // deltaPhi
  std::function<float()> f_bQuarksDeltaPhi = std::bind(&TruthOutputModule::getDeltaPhiBQuarks, this, "kHiggs", "bquarks");
  core()->AddTreeBranch("bQuarksDeltaPhi", f_bQuarksDeltaPhi, "all");

  // pt
  std::function<std::vector<float>()> f_bQuarkPt = std::bind(&TruthOutputModule::getPtVectorBQuarks, this, "kHiggs", "bquarks");
  core()->AddTreeBranch("bQuarkPt", f_bQuarkPt, "all");
  // eta
  std::function<std::vector<float>()> f_bQuarkEta = std::bind(&TruthOutputModule::getEtaVectorBQuarks, this, "kHiggs", "bquarks");
  core()->AddTreeBranch("bQuarkEta", f_bQuarkEta, "all");
  // phi
  std::function<std::vector<float>()> f_bQuarkPhi = std::bind(&TruthOutputModule::getPhiVectorBQuarks, this, "kHiggs", "bquarks");
  core()->AddTreeBranch("bQuarkPhi", f_bQuarkPhi, "all");

  // Z leptons kinematics
  // mass
  std::function<float()> f_ZLeptonsMass = std::bind(&TruthOutputModule::getMassBQuarks, this, "kZBosons", "leptons_Z");
  core()->AddTreeBranch("ZLeptonsMass", f_ZLeptonsMass, "all");
  // pt
  std::function<float()> f_ZLeptonsPt = std::bind(&TruthOutputModule::getPtBQuarks, this, "kZBosons", "leptons_Z");
  core()->AddTreeBranch("ZLeptonsPt", f_ZLeptonsPt, "all");
  // eta
  std::function<float()> f_ZLeptonsEta = std::bind(&TruthOutputModule::getEtaBQuarks, this, "kZBosons", "leptons_Z");
  core()->AddTreeBranch("ZLeptonsEta", f_ZLeptonsEta, "all");
  // phi
  std::function<float()> f_ZLeptonsPhi = std::bind(&TruthOutputModule::getPhiBQuarks, this, "kZBosons", "leptons_Z");
  core()->AddTreeBranch("ZLeptonsPhi", f_ZLeptonsPhi, "all");
  // deltaR
  std::function<float()> f_ZLeptonsDeltaR = std::bind(&TruthOutputModule::getDeltaRBQuarks, this, "kZBosons", "leptons_Z");
  core()->AddTreeBranch("ZLeptonsDeltaR", f_ZLeptonsDeltaR, "all");
  // deltaEta
  std::function<float()> f_ZLeptonsDeltaEta = std::bind(&TruthOutputModule::getDeltaEtaBQuarks, this, "kZBosons", "leptons_Z");
  core()->AddTreeBranch("ZLeptonsDeltaEta", f_ZLeptonsDeltaEta, "all");
  // deltaPhi
  std::function<float()> f_ZLeptonsDeltaPhi = std::bind(&TruthOutputModule::getDeltaPhiBQuarks, this, "kZBosons", "leptons_Z");
  core()->AddTreeBranch("ZLeptonsDeltaPhi", f_ZLeptonsDeltaPhi, "all");

  // Z quarks kinematics
  // mass
  std::function<float()> f_ZQuarksMass = std::bind(&TruthOutputModule::getMassBQuarks, this, "kZBosons", "quarks_Z");
  core()->AddTreeBranch("ZQuarksMass", f_ZQuarksMass, "all");
  // pt
  std::function<float()> f_ZQuarksPt = std::bind(&TruthOutputModule::getPtBQuarks, this, "kZBosons", "quarks_Z");
  core()->AddTreeBranch("ZQuarksPt", f_ZQuarksPt, "all");
  // eta
  std::function<float()> f_ZQuarksEta = std::bind(&TruthOutputModule::getEtaBQuarks, this, "kZBosons", "quarks_Z");
  core()->AddTreeBranch("ZQuarksEta", f_ZQuarksEta, "all");
  // phi
  std::function<float()> f_ZQuarksPhi = std::bind(&TruthOutputModule::getPhiBQuarks, this, "kZBosons", "quarks_Z");
  core()->AddTreeBranch("ZQuarksPhi", f_ZQuarksPhi, "all");
  // deltaR
  std::function<float()> f_ZQuarksDeltaR = std::bind(&TruthOutputModule::getDeltaRBQuarks, this, "kZBosons", "quarks_Z");
  core()->AddTreeBranch("ZQuarksDeltaR", f_ZQuarksDeltaR, "all");
  // deltaEta
  std::function<float()> f_ZQuarksDeltaEta = std::bind(&TruthOutputModule::getDeltaEtaBQuarks, this, "kZBosons", "quarks_Z");
  core()->AddTreeBranch("ZQuarksDeltaEta", f_ZQuarksDeltaEta, "all");
  // deltaPhi
  std::function<float()> f_ZQuarksDeltaPhi = std::bind(&TruthOutputModule::getDeltaPhiBQuarks, this, "kZBosons", "quarks_Z");
  core()->AddTreeBranch("ZQuarksDeltaPhi", f_ZQuarksDeltaPhi, "all");

  // pt
  std::function<std::vector<float>()> f_ZQuarkPt = std::bind(&TruthOutputModule::getPtVectorBQuarks, this, "kZBosons", "quarks_Z");
  core()->AddTreeBranch("ZQuarkPt", f_ZQuarkPt, "all");
  // eta
  std::function<std::vector<float>()> f_ZQuarkEta = std::bind(&TruthOutputModule::getEtaVectorBQuarks, this, "kZBosons", "quarks_Z");
  core()->AddTreeBranch("ZQuarkEta", f_ZQuarkEta, "all");
  // phi
  std::function<std::vector<float>()> f_ZQuarkPhi = std::bind(&TruthOutputModule::getPhiVectorBQuarks, this, "kZBosons", "quarks_Z");
  core()->AddTreeBranch("ZQuarkPhi", f_ZQuarkPhi, "all");
}


std::vector<TLorentzVector> TruthOutputModule::getP4(std::string key)
{
  std::vector<TLorentzVector> result;
  result.clear();

  if (key.find("Jet") != std::string::npos) {
    auto particles = core()->getContainer<xAOD::Jet>(key);
    for (auto par : *particles) {
      result.push_back(par->p4());
    }
  }
  else {
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
  }
  return result;
}

std::vector<TLorentzVector> TruthOutputModule::getP4(std::vector<const xAOD::TruthParticle*> pars)
{
  std::vector<TLorentzVector> result;
  result.clear();

  for(auto par : pars){
    result.push_back(par->p4());
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

float TruthOutputModule::getMassBQuarks(std::string key, std::string target)
{
  auto pars = this->getBQuarks(key, target);
  std::vector<TLorentzVector> parP4 = this->getP4(pars);
  if (parP4.size() < 2)  return -999;
  TLorentzVector totalP4 = parP4[0]+parP4[1];

  return totalP4.M();
}

float TruthOutputModule::getPtBQuarks(std::string key, std::string target)
{
  auto pars = this->getBQuarks(key, target);
  std::vector<TLorentzVector> parP4 = this->getP4(pars);
  if (parP4.size() < 2)  return -999;
  TLorentzVector totalP4 = parP4[0]+parP4[1];
  
  return totalP4.Pt();
}

float TruthOutputModule::getEtaBQuarks(std::string key, std::string target)
{
  auto pars = this->getBQuarks(key, target);
  std::vector<TLorentzVector> parP4 = this->getP4(pars);
  if (parP4.size() < 2)  return -999;
  TLorentzVector totalP4 = parP4[0]+parP4[1];
  
  return totalP4.Eta();
}

float TruthOutputModule::getPhiBQuarks(std::string key, std::string target)
{
  auto pars = this->getBQuarks(key, target);
  std::vector<TLorentzVector> parP4 = this->getP4(pars);
  if (parP4.size() < 2)  return -999;
  TLorentzVector totalP4 = parP4[0]+parP4[1];
  
  return totalP4.Phi();
}

float TruthOutputModule::getDeltaRBQuarks(std::string key, std::string target)
{
  auto pars = this->getBQuarks(key, target);
  std::vector<TLorentzVector> parP4 = this->getP4(pars);
  if (parP4.size() < 2)  return -999;

  return parP4[0].DeltaR(parP4[1]);
}

float TruthOutputModule::getDeltaPhiBQuarks(std::string key, std::string target)
{
  auto pars = this->getBQuarks(key, target);
  std::vector<TLorentzVector> parP4 = this->getP4(pars);
  if (parP4.size() < 2)  return -999;

  return parP4[0].DeltaPhi(parP4[1]);
}

float TruthOutputModule::getDeltaEtaBQuarks(std::string key, std::string target)
{
  auto pars = this->getBQuarks(key, target);
  std::vector<TLorentzVector> parP4 = this->getP4(pars);
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

std::vector<float> TruthOutputModule::getPtVectorBQuarks(std::string key, std::string target)
{
  auto pars = this->getBQuarks(key, target);
  std::vector<TLorentzVector> parP4s = this->getP4(pars);
  std::vector<float> result;
  result.clear();

  for (auto parP4 : parP4s) {
    result.push_back(parP4.Pt());
  }
  return result;
}

std::vector<float> TruthOutputModule::getEtaVectorBQuarks(std::string key, std::string target)
{
  auto pars = this->getBQuarks(key, target);
  std::vector<TLorentzVector> parP4s = this->getP4(pars);
  std::vector<float> result;
  result.clear();

  for (auto parP4 : parP4s) {
    result.push_back(parP4.Eta());
  }
  return result;
}

std::vector<float> TruthOutputModule::getPhiVectorBQuarks(std::string key, std::string target)
{
  auto pars = this->getBQuarks(key, target);
  std::vector<TLorentzVector> parP4s = this->getP4(pars);
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

std::vector<const xAOD::TruthParticle*> TruthOutputModule::getBQuarks(const std::string & key, const std::string & target){
  std::vector<const xAOD::TruthParticle*> bottoms;
  bottoms.clear();
  auto pars = core()->getContainer<xAOD::TruthParticle>(key);

  if(target == "bquarks"){
    for(auto par : *pars){
      if(par->pdgId() != 25) continue;
      for(int i = 0; i < par->nChildren(); i++){
        int id = par->child(i)->absPdgId();
        if(id == 5){
          bottoms.push_back(this->loopSelf(par->child(i)));
        }
      }
    }
  }

  if(target == "leptons_Z"){
    std::cout<<__LINE__<<std::endl;
    for(auto par : *pars){
      if(par->pdgId() != 23) continue;
      for(int i = 0; i < par->nChildren(); i++){
        int id = par->child(i)->absPdgId();
      std::cout<<"Z i th, child: "<<i<<", "<<id<<std::endl;
        if(id == 11 || id == 13 || id == 15){
          bottoms.push_back(this->loopSelf(par->child(i)));
        }
      }
    }
  }

  if(target == "quarks_Z"){
    for(auto par : *pars){
      if(par->pdgId() != 23) continue;
      for(int i = 0; i < par->nChildren(); i++){
        int id = par->child(i)->absPdgId();
        if(0 < id && id < 6){
          bottoms.push_back(this->loopSelf(par->child(i)));
        }
      }
    }
  }

  return bottoms;
}

const xAOD::TruthParticle* TruthOutputModule::loopSelf(const xAOD::TruthParticle* par){
  while(par->child(0) && par->pdgId() == par->child(0)->pdgId()) { par = par->child(0); }
  return par;
}
