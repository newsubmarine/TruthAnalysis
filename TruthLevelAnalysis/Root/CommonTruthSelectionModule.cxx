#include <TruthLevelAnalysis/CommonTruthSelectionModule.h>
#include "xAODEventInfo/EventInfo.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODTruth/TruthEventContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODJet/JetContainer.h"
#ifdef ROOTCORE_PACKAGE_LPXKfactorTool  
#include "LPXKfactorTool/LPXKfactorTool.h"
#endif
#include "TRandom.h"
//_____________________________________________________________________________

void CommonTruthSelectionModule::initialize (){

  //cut templates
  core()->addTemplate("FETCH_EVENTINFO", &CommonTruthSelectionModule::fetchEventInfo, this);
  core()->addTemplate("FETCH_TRUTHEVENT", &CommonTruthSelectionModule::fetchTruthEvent, this);
  core()->addTemplate("FETCH_TRUTH_PARTICLES", &CommonTruthSelectionModule::fetchTruthParticles, this);
  core()->addTemplate("FETCH_JETS", &CommonTruthSelectionModule::fetchJets, this);
  core()->addTemplate("FETCH_MET", &CommonTruthSelectionModule::fetchMET, this);
  core()->addTemplate("MERGE_CONTAINERS", &CommonTruthSelectionModule::mergeContainers, this);
  core()->addTemplate("PRINT", &CommonTruthSelectionModule::print, this);
  core()->addTemplate("PRINT_MOTHER", &CommonTruthSelectionModule::printMother, this);
  core()->addTemplate("OLR", &CommonTruthSelectionModule::performOLR, this);
  core()->addTemplate("N_EQUAL", &CommonTruthSelectionModule::nEqual, this);
  core()->addTemplate("N_KEYEQUALKEY", &CommonTruthSelectionModule::nKeyEqualKey, this);
  core()->addTemplate("N_MIN", &CommonTruthSelectionModule::nMin, this);
  core()->addTemplate("N_LEP_EQUAL", &CommonTruthSelectionModule::nLepEqual, this);
  core()->addTemplate("N_LEP_WITHIN", &CommonTruthSelectionModule::nLepWithin, this);
  core()->addTemplate("DECAY_MODE_CUT", &CommonTruthSelectionModule::decayModeCut, this);
  core()->addTemplate("HIGGS_DECAY_MODE_CUT", &CommonTruthSelectionModule::higgsDecayModeCut, this);
  core()->addTemplate("LEAD_LEP_CUT", &CommonTruthSelectionModule::leadLepCut, this);
  core()->addTemplate("PID_SEL", &CommonTruthSelectionModule::pidSel, this);
  core()->addTemplate("SIM_EFF", &CommonTruthSelectionModule::simEff, this);
  core()->addTemplate("ISO_SEL", &CommonTruthSelectionModule::isoSel, this);
  core()->addTemplate("TAU_SEL", &CommonTruthSelectionModule::tauSel, this);
  core()->addTemplate("ELE_SEL", &CommonTruthSelectionModule::eleSel, this);
  core()->addTemplate("MUON_SEL", &CommonTruthSelectionModule::muonSel, this);
  core()->addTemplate("MINPT", &CommonTruthSelectionModule::minPt, this);
  core()->addTemplate("MINTAUPT", &CommonTruthSelectionModule::minTauPt, this);
  core()->addTemplate("APPLYMCWEIGHT", &CommonTruthSelectionModule::applyMcWeight, this);
  core()->addTemplate("NOTHING", &CommonTruthSelectionModule::doNothing, this);
  core()->addTemplate("JET_SEL", &CommonTruthSelectionModule::jetSel, this);
  core()->addTemplate("VBF_JET_SEL", &CommonTruthSelectionModule::vbfJetSel, this);
  core()->addTemplate("CALC_BWEIGHT", &CommonTruthSelectionModule::calcBweight, this);
  core()->addTemplate("TRANREGION_CUT", &CommonTruthSelectionModule::tranRegionCut, this);

#ifdef ROOTCORE_PACKAGE_LPXKfactorTool  
  std::string sampleName = core()->wk()->metaData()->castString("sample_name");
  if(sampleName.find("DY") != std::string::npos || sampleName.find("36110") != std::string::npos)
  {
    m_LPXKfactorTool = std::make_shared<LPXKfactorTool>("LPXKfactorTool");
    if(sampleName.find("Powheg") == std::string::npos)
      CHK(m_LPXKfactorTool->setProperty("applyEWCorr",false));
    CHK(m_LPXKfactorTool->initialize());
  }
  core()->addTemplate("APPLYLPXKFACTOR", &CommonTruthSelectionModule::ApplyLPXKfactor, this);
#endif
}

//_____________________________________________________________________________

double CommonTruthSelectionModule::ApplyLPXKfactor()
{
  #ifdef ROOTCORE_PACKAGE_LPXKfactorTool  
  if(m_LPXKfactorTool){
    m_LPXKfactorTool->execute();
  }
  #endif
  return 1.;
}

//_____________________________________________________________________________

void CommonTruthSelectionModule :: fetchTruthParticles(std::string truth_key, std::string store_key){
  core()->fetchContainer<xAOD::TruthParticleContainer>(truth_key, store_key);
  auto particles = core()->getContainer<xAOD::TruthParticle>(store_key);
  
  if (truth_key == "TruthTaus") {
    particles->sort([](xAOD::TruthParticle* a, xAOD::TruthParticle* b) -> bool {return (a->auxdata<double>("pt_vis") > b->auxdata<double>("pt_vis"));});
  }
  else if (store_key == "kW" || store_key == "kZ") {
    particles->sort([](xAOD::TruthParticle* a, xAOD::TruthParticle* b) -> bool {return (a->m() > b->m());});
  }
  else {
    particles->sort([](xAOD::TruthParticle* a, xAOD::TruthParticle* b) -> bool {return (a->pt() > b->pt());});
  }

  if (store_key == "kVBFJets") {
    m_vbfQuarks.clear();
    for (auto particle : *particles) {
      if (particle->hasProdVtx()==1 && 
          particle->prodVtx()->barcode()==-3 && 
          particle->status()==23 && 
          abs(particle->pdgId())<6) {
        m_vbfQuarks.push_back(correctedParticle(particle));
      }
    }
  } 
}


//_____________________________________________________________________________
void CommonTruthSelectionModule :: mergeContainers(std::string truth_keys, std::string store_key){
  std::istringstream stream(truth_keys);
  std::string key1;
  std::string key2;
  stream >> key1;
  stream >> key2;
  core()->mergeParticleContainer(key1, key2, store_key); 
  while (stream >> key1) {
      core()->mergeParticleContainer(key1, store_key, store_key);
  }
  
  auto combinedContainer = core()->getContainer<xAOD::TruthParticle>(store_key);
  combinedContainer->sort(compareByPtAndPtvis); 
}

bool CommonTruthSelectionModule::compareByPtAndPtvis(xAOD::TruthParticle* a, xAOD::TruthParticle* b)
{
  float pt_a;
  if (a->isAvailable<double>("pt_vis")) 
    pt_a = a->auxdata<double>("pt_vis");
  else
    pt_a = a->pt();

  float pt_b;
  if (b->isAvailable<double>("pt_vis")) 
    pt_b = b->auxdata<double>("pt_vis");
  else
    pt_b = b->pt();

  return pt_a > pt_b;
}
//_____________________________________________________________________________

void CommonTruthSelectionModule :: fetchJets(std::string key, std::string store_key){
  core()->fetchContainer<xAOD::JetContainer>(key, store_key);
}



const xAOD::TruthParticle* CommonTruthSelectionModule :: correctedParticle( const xAOD::TruthParticle * part ){
  const xAOD::TruthParticle* corrpart = part;
  for( unsigned ikid=0; ikid<part->nChildren(); ++ikid ){
    const xAOD::TruthParticle* kid = part->child( ikid );
    if( kid->pdgId() == part->pdgId() ){
      corrpart = correctedParticle( kid );
      break;
    }
  }
  return corrpart;
}



bool CommonTruthSelectionModule :: vbfJetSel(xAOD::TruthParticle* particle) {
  for( unsigned ikid=0; ikid<particle->nChildren(); ++ikid ){
    const xAOD::TruthParticle* kid = particle->child( ikid );
    if( kid->pdgId() == particle->pdgId() ){
      return false;
    }
  }

  for (auto vbfQuark : m_vbfQuarks) { 
    if (particle->pdgId() != vbfQuark->pdgId()) continue;
    if ((particle->p4()-vbfQuark->p4()).P() < 1e-6)  return true;
  }

  return false;
}

//_____________________________________________________________________________

void CommonTruthSelectionModule :: fetchMET(std::string key, std::string store_key, std::string term_key){
  core()->fetchContainer<xAOD::MissingETContainer>(key, store_key);
  auto metc = core()->getContainer<xAOD::MissingET>(store_key);
  for(const auto& met : *metc){
    if(met->name() == "NonInt"){
      core()->putObject(term_key, met);
      return;
    }
  }
}

//_____________________________________________________________________________

void CommonTruthSelectionModule::fetchEventInfo(const std::string& eventinfo_key){
  const xAOD::EventInfo* eventInfo = nullptr;

  CHK( core()->m_event->retrieve( eventInfo, "EventInfo") );

  core()->putObject(eventinfo_key, eventInfo);
}

//_____________________________________________________________________________

void CommonTruthSelectionModule::fetchTruthEvent(std::string key){
  const xAOD::TruthEventContainer* truthevents = nullptr;
  CHK( core()->m_event->retrieve(truthevents, "TruthEvents") );
  core()->putObject(key, truthevents->at(0));
}

//_____________________________________________________________________________

bool CommonTruthSelectionModule::performOLR(xAOD::IParticle* particle, const std::string& other_key, double dR){
  auto others = core()->getContainer<xAOD::IParticle>(other_key);
  TLorentzVector p4 = particle->p4();
  if(particle->isAvailable<double>("pt_vis")){
    p4.SetPtEtaPhiM(particle->auxdata<double>("pt_vis"), particle->auxdata<double>("eta_vis"), particle->auxdata<double>("phi_vis"), particle->auxdata<double>("m_vis"));
  }
  for(const auto& other: *others){
    TLorentzVector op4 = other->p4();
  if(other->isAvailable<double>("pt_vis")){
    op4.SetPtEtaPhiM(other->auxdata<double>("pt_vis"), other->auxdata<double>("eta_vis"), other->auxdata<double>("phi_vis"), other->auxdata<double>("m_vis"));
  }
    if(op4.DeltaR(p4) < dR) return false;
  }
  return true;
}

//_____________________________________________________________________________

double CommonTruthSelectionModule::leadLepCut(const std::string& key, double minPt) {
  auto container = core()->getContainer<xAOD::TruthParticle>(key);
 
  double leadLepPt = 0.0;
  for (const xAOD::TruthParticle* particle : *container)  {
    if (particle->isAvailable<double>("pt_vis")) {
      return 1.0;
    }

    leadLepPt = particle->pt();
    break;
  }

  if (leadLepPt <= minPt) {
    std::cout << "WARNING: leading lepton pT is less than 13 GeV" << std::endl;   
    return 0.0;
  }

  return 1.0;
}


double CommonTruthSelectionModule::nEqual(const std::string& container_key, int quantity){
  auto container = core()->getContainer(container_key);
  if(int(container->size()) != quantity) return 0.;
  else return 1.;
}

//_____________________________________________________________________________

double CommonTruthSelectionModule::nKeyEqualKey(const std::string& container_key1, const std::string& container_key2){
  auto container1 = core()->getContainer(container_key1);
  auto container2 = core()->getContainer(container_key2);

  if (container1->size()!=container2->size())
  {
    std::cout << container_key1 << "\t" << container1->size() << std::endl;
    std::cout << container_key2 << "\t" << container2->size() << std::endl;
    return 0.0;
  }
  return 1.0;
}

//_____________________________________________________________________________

double CommonTruthSelectionModule::nMin(const std::string& key, int quantity){
  auto container = core()->getContainer(key);
  if(int(container->size()) >= quantity) return 1.;
  else return 0.;
}

//_____________________________________________________________________________

double CommonTruthSelectionModule::nLepEqual(const std::string& key1, const std::string& key2, int quantity){
  auto container1 = core()->getContainer(key1);
  auto container2 = core()->getContainer(key2);
  int sum = container1->size()+container2->size();
  if(sum == quantity) return 1.;
  else return 0.;
}
//_____________________________________________________________________________

double CommonTruthSelectionModule::nLepWithin(const std::string& key1, const std::string& key2,  int low, int high){
  auto container1 = core()->getContainer(key1);
  auto container2 = core()->getContainer(key2);
  int sum = container1->size()+container2->size();
  if(low <= sum && high >= sum) return 1.;
  else return 0.;
}
//_____________________________________________________________________________

double CommonTruthSelectionModule::decayModeCut(const std::string& store_key, int n_TauH, int n_TauL){
  //std::cout << ">>> Informations <<<" << std::endl;
  auto taus = core()->getContainer<xAOD::TruthParticle>(store_key);
  int counterH = 0;
  int counterL = 0;
  for(auto tau : *taus){
    const xAOD::TruthParticle_v1* tau_tmp = tau;
    const xAOD::TruthParticle_v1* tau_tmp2 = tau;
    while(tau_tmp2->parent(0)->absPdgId() == 15) {
        tau_tmp2=tau_tmp2->parent(0);
    }

    // ........
 //   std::cout << "\t"<< tau_tmp->pdgId() << "\t" << tau_tmp->barcode() << "\t" << tau_tmp->parent(0)->barcode()<<","<<tau_tmp->parent(0)->pdgId() << "\t" << tau_tmp->child(0)->pdgId()<<","<< tau_tmp->child(1)->pdgId()<< std::endl; 
  if(tau_tmp2->parent(0)->pdgId() != 25 && tau_tmp2->parent(0)->pdgId() != 23) continue;
    //if(tau_tmp->parent(0)->pdgId() != 25 && tau_tmp->parent(0)->pdgId() != 23 && tau_tmp->parent(0)->absPdgId() != 15) continue;
    while(tau_tmp->child(0)->absPdgId() == 15){
      tau_tmp = tau_tmp->child(0);
    }
    if(tau_tmp->child(0)->isHadron() || tau_tmp->child(1)->isHadron() || (tau_tmp->nChildren()>2 && tau_tmp->child(2)->isHadron()) || (tau_tmp->nChildren()>3 && tau_tmp->child(3)->isHadron())) counterH++;
    if(tau_tmp->child(0)->absPdgId() == 11 || tau_tmp->child(0)->absPdgId() == 13 || tau_tmp->child(1)->absPdgId() == 11 || tau_tmp->child(1)->absPdgId() == 13) counterL++;
  }
  if((counterH + counterL) != 4) {
      std::cout<<"CommonTruthSelectionModule::decayModeCut():\n";
      std::cout << ">>> Informations <<<" << std::endl;
      std::cout<<"n_TauH, n_TauL = "<<counterH<<", "<<counterL<<std::endl;
      std::cout << "\t" <<"pdgID" << "\t" << "Barcode" << "\t" << "parent" << "\t" << "child" << std::endl;
      for(auto tau : *taus){
      const xAOD::TruthParticle_v1* tau_tmp = tau;
      while(tau_tmp->child(0)->absPdgId() == 15){
        tau_tmp = tau_tmp->child(0);
      }
//      std::cout << "\t"<< tau_tmp->pdgId() << "\t" << tau_tmp->barcode() << "\t" << tau_tmp->parent(0)->barcode()<<","<<tau_tmp->parent(0)->pdgId() << "\t" << tau_tmp->child(0)->pdgId()<<","<< tau_tmp->child(1)->pdgId()<< std::endl;
      }
  }
      
  if(counterH == n_TauH && counterL == n_TauL) return 1.;
  else return 0.;
}

/// Print mother and daughter
void CommonTruthSelectionModule::printMother(const std::string & key)
{
  auto particles = core()->getContainer<xAOD::TruthParticle>(key);
  for (auto par: *particles)
  {
    const xAOD::TruthParticle_v1* par_mo = par;
    while (0!=par_mo->nParents())
    {
      par_mo = par_mo->parent(0);
      if (par_mo->pdgId()!=par->pdgId()) break;
    }
    
    const xAOD::TruthParticle_v1* par_gm = par_mo;
    while (0!=par_gm->nParents())
    {
      par_gm = par_gm->parent(0);
      if (par_gm->pdgId()!=par_mo->pdgId()) break;
    }

    std::cout << "mother:" << par_mo->pdgId() << "\tgrand mother:" << par_gm->pdgId() << std::endl;
  }
}  

//_____________________________________________________________________________
/// Print mother and daughter
void CommonTruthSelectionModule::print(const std::string & key)
{
  auto particles = core()->getContainer<xAOD::TruthParticle>(key);
  std::cout << "*******************************" << std::endl;
  int pdgs[6] = {11, 13, 15, 23, 24, 25};
  for (auto par : *particles) 
  {
    bool flag = false;
    for (int i=0; i<6; ++i)
	if (pdgs[i]==par->absPdgId()) flag = true;   

    if (!flag) continue;
    const xAOD::TruthParticle_v1* par_tmp = par;
    while (0!=par_tmp->nParents())
    {
      par_tmp = par_tmp->parent(0);
      if (par_tmp->pdgId()!=par->pdgId()) break;
    }
    std::cout << key << "\t" << par->pdgId() << std::endl;
    if (par_tmp->pdgId()!=par->pdgId())
      std::cout << "mother" << "\t" << par_tmp->pdgId() << std::endl;
    if (1<=par->nChildren())
    {
      std::cout << "child" << "\t";
      for (size_t i=0; i<par->nChildren(); ++i)	
        std::cout << par->child(i)->pdgId() << "\t";
      std::cout << std::endl;
    }
  }
}


//_____________________________________________________________________________
/// Select by abspdgid
bool CommonTruthSelectionModule::pidSel(xAOD::TruthParticle* par, int absPdgId)
{
  if (absPdgId!=par->absPdgId()) return false; 
  //std::cout << "pdg:" << par->absPdgId() << "\t" << "status:" << par->status() << "\t" 
  //          << "pt:" << par->pt() << "\t" 
  //          << "ndau:" << par->nChildren()  << std::endl;
  if (0==par->nChildren()) return true;
  for (unsigned int i=0; i<par->nChildren(); ++i) {
    int childPdgId = par->child(i)->absPdgId();
  //  std::cout << "dau" << i << ": " << childPdgId << std::endl;
    if (absPdgId==childPdgId) return false;
    if (15==absPdgId && (11==childPdgId||13==childPdgId)) return false;
  }
  return true;
}


double CommonTruthSelectionModule::higgsDecayModeCut(const std::string& key)
{
  auto pars = core()->getContainer<xAOD::TruthParticle>(key);
  for (auto par : *pars) 
  {
    if (25!=par->pdgId()) continue;
    if (0==par->nChildren()) continue;
    if (25==par->child(0)->pdgId())  continue;
    
    int child0_pdg = par->child(0)->absPdgId();
    int child1_pdg = par->child(1)->absPdgId();
	
    if (child0_pdg!=child1_pdg||(15!=child0_pdg&&23!=child0_pdg&&24!=child0_pdg&&22!=child0_pdg)) 
    {
	//std::cout << "Higgs not wanted..." << std::endl;
	//print(key);	
	return 0.0;
    }
  }
  return 1.0;
}

bool CommonTruthSelectionModule::simEff(xAOD::TruthParticle* par, double prob){
  if (15==par->absPdgId()) { 
    if(!par->auxdata<char>("IsHadronicTau")) {
      std::cout << "should not occur!" << std::endl;
      return false;
    }
  }

  static TRandom* radm = new TRandom();
  double v =radm->Uniform(1);

  if (v>prob) return false;
  return true;
}


//_____________________________________________________________________________
bool CommonTruthSelectionModule::isoSel(xAOD::TruthParticle* par){
  if (par->isAvailable< unsigned int >("classifierParticleType")) {
    unsigned int type= par->auxdata< unsigned int >("classifierParticleType");
    if ((2==type||6==type||10==type)) {
      return true;
    }
    else if (14==type) {
      unsigned int origin = par->auxdata< unsigned int >("classifierParticleOrigin");
      if (14==origin) {
        return true;
      }
      else {
        return false;
      }
    }
    else {
      return false;
    }
  }
  else {
    const xAOD::TruthParticle* mother = getMother(par);
    
    // mother
    if (25==mother->absPdgId()) return true;
    if (par->pdgId() == 22) return false;

    // grandmother
    mother = getMother(mother);
    if (25==mother->absPdgId()) return true; // grand mother
    
    // great grand mother
    mother = getMother(mother);
    if (25==mother->absPdgId()) return true; // great grand mother
    return false;
  }
}

const xAOD::TruthParticle* CommonTruthSelectionModule::getMother(const xAOD::TruthParticle* par) const
{
  const xAOD::TruthParticle* mother = par->parent(0); 
  while (mother->pdgId() == par->pdgId())
    mother = mother->parent(0);
  return mother;
}
//_____________________________________________________________________________

bool CommonTruthSelectionModule::tauSel(xAOD::TruthParticle* tau, double pTcut, double etacut){
  if(! tau->isAvailable<char>("IsHadronicTau")) throw Exception("tau not decorated"); 
  if(! tau->auxdata<char>("IsHadronicTau")) return false;
  if(! tau->isAvailable<double>("pt_vis")) throw Exception("tau not decorated"); 
  if(tau->auxdata<double>("pt_vis") < pTcut * 1000. && pTcut>0) return false;
  double abseta = std::abs(tau->auxdata<double>("eta_vis"));
  if(abseta > etacut && etacut>0) return false;
  return true;
}

//_____________________________________________________________________________

bool CommonTruthSelectionModule::eleSel(xAOD::TruthParticle* electron, double pTcut, double etacut){
  double pt = electron->pt();
  double absEta = std::abs(electron->eta());
  if(pt < pTcut * 1000. && pTcut >0) return false;
  if(absEta > etacut && etacut >0) return false;
  return true;
}

bool CommonTruthSelectionModule::tranRegionCut(xAOD::TruthParticle* par)
{
  double abseta = .0;
  if (par->isAvailable<double>("eta_vis")) { //tau
    abseta = std::abs(par->auxdata<double>("eta_vis"));
  } 
  else {
    abseta = std::abs(par->eta());
  }
  if (abseta>1.37&&abseta<1.52) return false;
  return true;
}
//_____________________________________________________________________________

bool CommonTruthSelectionModule::muonSel(xAOD::TruthParticle* muon, double pTcut, double etacut){
  double pt = muon->pt();
  double absEta = std::abs(muon->eta());
  if(pt < pTcut * 1000. && pTcut >0) return false;
  if(absEta > etacut && etacut >0) return false;
  return true;
}

//_____________________________________________________________________________

double CommonTruthSelectionModule::minPt(std::string container, int position, double minPt){
  auto cont = core()->getContainer<xAOD::IParticle>(container);
  if(int(cont->size()) <= position){
    log->warn() << "Tried to access nonexisting object in PT cut. Discarded event.";
    return 0.;
  }
  if(cont->at(position)->pt() >= minPt * 1000) return 1.;
  else return 0.;
}

//_____________________________________________________________________________

double CommonTruthSelectionModule::minTauPt(std::string container, int position, double minPt){
  auto cont = core()->getContainer<xAOD::IParticle>(container);
  if(int(cont->size()) <= position){
    log->warn() << "Tried to access nonexisting object in PT cut. Discarded event.";
    return 0.;
  }
  if(cont->at(position)->auxdata<double>("pt_vis") >= minPt * 1000) return 1.;
  else return 0.;
}

//_____________________________________________________________________________

double CommonTruthSelectionModule::applyMcWeight(std::string truth_key){
  return core()->getObject<const xAOD::TruthEvent>(truth_key)->weights().at(0);
}

//_____________________________________________________________________________

double CommonTruthSelectionModule::doNothing(){
  return 1.;
}

//_____________________________________________________________________________

bool CommonTruthSelectionModule::jetSel(xAOD::Jet* jet, double minPt, double maxAbsEta){
  return jet->pt() > minPt * 1000. && std::abs(jet->eta()) < maxAbsEta;
}


//_____________________________________________________________________________
//https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingBenchmarks#b_tagging_Benchmarks_for_tagger
//the probability for at least one jet being tagged is (1 - no_jet_tagged)
//decorate prob. to leading tau
void CommonTruthSelectionModule::calcBweight(std::string jet_key, std::string tau_key){
  auto jets = core()->getContainer<xAOD::Jet>(jet_key);
  double prob_no_tag = 1.;
  for(auto jet: *jets){
    auto truthid = jet->auxdata<int>("HadronConeExclTruthLabelID");
    if(truthid == 5){
      //true b
      prob_no_tag *= 0.3003;
    }else if(truthid == 4){
      //true c
      prob_no_tag *= 0.9178;
    }else if(truthid == 15){
      //true tau
      prob_no_tag *= 0.9817;
    }else{
      //true light
      prob_no_tag *= 0.9974;
    }
  }
  core()->getContainer<xAOD::TruthParticle>(tau_key)->at(0)->auxdata<double>("prob_tag") = 1 - prob_no_tag;
}
