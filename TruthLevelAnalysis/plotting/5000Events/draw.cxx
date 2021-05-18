void Compare_Distribution(TString var, int nBins, double xMin, double xMax, TString name,TString xTitle, const std::vector<TTree*> trees, TCut cut)
{
  TCanvas* c1 = new TCanvas(name+"_canvas", "", 800, 600);
   
  std::vector<TH1D*> hists;
  std::vector<unsigned int> colors = {kBlack, kBlue, kRed};
  std::vector<int> styles = {0, 2};

  double maximum = 0.0;

  TString option = cut.GetTitle();
  if (option.Length() == 0) {
    option = "weight";
  }
  else {
    option = TString::Format("(%s)*weight", cut.GetTitle());
  }
  
  if (name == "weight") {
    option = "";
  }

  std::cout << option << std::endl;
  
  // Plotting
  for (size_t index = 0; index < trees.size(); ++index) {
    TTree* tree = trees.at(index);
    TH1D* hist = new TH1D(name + "_" + tree->GetTitle(), tree->GetTitle(), nBins, xMin, xMax);
    tree->Project(hist->GetName(), var, option);
    
    double weight = hist->GetSumOfWeights();
    hist->Scale(1./weight);
    
    hist->GetXaxis()->SetTitle(xTitle);
    hist->GetYaxis()->SetTitle("A.U.");
    hist->GetXaxis()->CenterTitle();
    hist->SetLineWidth(2);
    hist->SetLineColor(colors.at(index/styles.size()));
    hist->SetLineStyle(styles.at(index%styles.size()));
    hists.push_back(hist);

    if (index == 0) {
      hist->Draw("hist");
    }
    else {
      hist->Draw("samehist");
    }

    if (hist->GetMaximum() > maximum) maximum = hist->GetMaximum();
  }
  hists.at(0)->GetYaxis()->SetRangeUser(0, 1.15*maximum);
  
  // Legend
  TLegend* lg = new TLegend(0.85, 0.5, 0.95, 0.9);

  for (const TH1D* hist : hists) {
    lg->AddEntry(hist, hist->GetTitle(), "l");
  }
  
  lg -> SetFillStyle(0);
  lg -> SetBorderSize(0);
  lg -> Draw("same");

  c1->Update();
  c1->SaveAs(name+".png");
}




void draw_plots() {
  
  // Set the ATLAS plotting style
  SetAtlasStyle();
  gStyle->SetOptStat(0);

  // Samples to compare
  std::vector<TString> samples = {"2l0tau", "2l1tau", "3l0tau", "3l1tau", "4l0tau", "4l1tau"};
  //std::vector<TString> samples = {"yy0l", "yy1l", "yy2l"};
  TString version = "cHHHp01d0";
  
  std::vector<TTree*> trees;
  for (const TString& sample : samples) {
    TString fileName = sample + "_" + version;
    TFile* file = new TFile(fileName + ".root", "open");
    TTree* tree = static_cast<TTree*>(file->Get("Nominal/PreSelection_all_higgs"));
    tree->SetTitle(sample);
    trees.push_back(tree);
  }
  
  int mode[6] = {30,38,39,46,47,48};
  TString title[6] = {"#tau#tau#tau#tau","#tau#tauZZ","#tau#tauWW","ZZZZ","WWZZ","WWWW"}; 
  TCut cut = "";
  for (int i=0; i<6; ++i) {
    //TString cut_mode = "higgsDecayMode[0]+higgsDecayMode[1]=="+convert<TString>(mode[i]);
    TString cut_mode = "";
    cut = cut && cut_mode;
    title[0] = "";

    Compare_Distribution("weight", 100, -0.05, 0.05, "weight", "weight", trees, cut);
#if 1
    Compare_Distribution("higgsDecayMode[0]+higgsDecayMode[1]",35,25,60, "higgsDecayMode", "di-higgs decay mode", trees, cut);
    Compare_Distribution("dihiggsDeltaR", 100, 0, 7, "dihiggsDeltaR", "#DeltaR", trees, cut);
    Compare_Distribution("dihiggsDeltaEta", 100, -10, 10, "dihiggsDeltaEta", "#Delta#eta", trees, cut);
    Compare_Distribution("dihiggsDeltaPhi", 100, -5, 5, "dihiggsDeltaPhi", "#Delta#Phi", trees, cut);
    
    Compare_Distribution("dihiggsMass/1000", 100, 250, 1000,  "dihiggsMass", "HH mass (GeV)", trees, cut);
    Compare_Distribution("dihiggsPt/1000", 100, 0, 2500,     "dihiggsPt"  , "HH p_{T} (GeV)", trees, cut);
    Compare_Distribution("dihiggsEta", 100, -10, 10,         "dihiggsEta" , "HH #eta", trees, cut);
    Compare_Distribution("dihiggsPhi", 100, -5, 5,           "dihiggsPhi" , "HH #Phi", trees, cut);
    
    Compare_Distribution("higgsMass[0]/1000", 100, 100, 150,  "higgs1Mass", "H1 mass (GeV)", trees, cut);
    Compare_Distribution("higgsPt[0]/1000", 100, 0, 2500,     "higgs1Pt"  , "H1 p_{T} (GeV)", trees, cut);
    Compare_Distribution("higgsEta[0]", 100, -10, 10,         "higgs1Eta" , "H1 #eta", trees, cut);
    Compare_Distribution("higgsPhi[0]", 100, -5, 5,           "higgs1Phi" , "H1 #Phi", trees, cut);
    
    Compare_Distribution("higgsMass[1]/1000", 100, 100, 150,  "higgs2Mass", "H2 mass (GeV)", trees, cut);
    Compare_Distribution("higgsPt[1]/1000", 100, 0, 2500,     "higgs2Pt"  , "H2 p_{T} (GeV)", trees, cut);
    Compare_Distribution("higgsEta[1]", 100, -10, 10,         "higgs2Eta" , "H2 #eta", trees, cut);
    Compare_Distribution("higgsPhi[1]", 100, -5, 5,           "higgs2Phi" , "H2 #Phi", trees, cut);
#endif

#if 1
    cut = "";
    Compare_Distribution("nW", 10, 0, 10,                   "nW", "N_{W}", trees, cut);

    cut = "nW>=2";
    Compare_Distribution("wMass[0]/1000", 100, 0, 100,  "w1Mass", "W_{1} mass (GeV)", trees, cut);
    Compare_Distribution("wPt[0]/1000", 100, 0, 2500,     "w1Pt"  , "W_{1} p_{T} (GeV)", trees, cut);
    Compare_Distribution("wEta[0]", 100, -10, 10,         "w1Eta" , "W_{1} #eta", trees, cut);
    Compare_Distribution("wPhi[0]", 100, -5, 5,           "w1Phi" , "W_{1} #Phi", trees, cut);
    
    Compare_Distribution("wMass[1]/1000", 100, 0, 100,  "w2Mass", "W_{2} mass (GeV)", trees, cut);
    Compare_Distribution("wPt[1]/1000", 100, 0, 2500,     "w2Pt"  , "W_{2} p_{T} (GeV)", trees, cut);
    Compare_Distribution("wEta[1]", 100, -10, 10,         "w2Eta" , "W_{2} #eta", trees, cut);
    Compare_Distribution("wPhi[1]", 100, -5, 5,           "w2Phi" , "W_{2} #Phi", trees, cut);
    
    cut = "nW>=4";
    Compare_Distribution("wMass[2]/1000", 100, 0, 100,  "w3Mass", "W_{3} mass (GeV)", trees, cut);
    Compare_Distribution("wPt[2]/1000", 100, 0, 2500,     "w3Pt"  , "W_{3} p_{T} (GeV)", trees, cut);
    Compare_Distribution("wEta[2]", 100, -10, 10,         "w3Eta" , "W_{3} #eta", trees, cut);
    Compare_Distribution("wPhi[2]", 100, -5, 5,           "w3Phi" , "W_{3} #Phi", trees, cut);
    
    Compare_Distribution("wMass[3]/1000", 100, 0, 100,  "w4Mass", "W_{4} mass (GeV)", trees, cut);
    Compare_Distribution("wPt[3]/1000", 100, 0, 2500,     "w4Pt"  , "W_{4} p_{T} (GeV)", trees, cut);
    Compare_Distribution("wEta[3]", 100, -10, 10,         "w4Eta" , "W_{4} #eta", trees, cut);
    Compare_Distribution("wPhi[3]", 100, -5, 5,           "w4Phi" , "W_{4} #Phi", trees, cut);

    cut = "";
    Compare_Distribution("nZ", 10, 0, 10,                   "nZ", "N_{Z}", trees, cut);

    cut = "nZ>=2";
    Compare_Distribution("zMass[0]/1000", 100, 0, 100,  "z1Mass", "Z_{1} mass (GeV)", trees, cut);
    Compare_Distribution("zPt[0]/1000", 100, 0, 2500,     "z1Pt"  , "Z_{1} p_{T} (GeV)", trees, cut);
    Compare_Distribution("zEta[0]", 100, -10, 10,         "z1Eta" , "Z_{1} #eta", trees, cut);
    Compare_Distribution("zPhi[0]", 100, -5, 5,           "z1Phi" , "Z_{1} #Phi", trees, cut);
    
    Compare_Distribution("zMass[1]/1000", 100, 0, 100,  "z2Mass", "Z_{2} mass (GeV)", trees, cut);
    Compare_Distribution("zPt[1]/1000", 100, 0, 2500,     "z2Pt"  , "Z_{2} p_{T} (GeV)", trees, cut);
    Compare_Distribution("zEta[1]", 100, -10, 10,         "z2Eta" , "Z_{2} #eta", trees, cut);
    Compare_Distribution("zPhi[1]", 100, -5, 5,           "z2Phi" , "Z_{2} #Phi", trees, cut);
    
    cut = "nZ>=4";
    Compare_Distribution("zMass[2]/1000", 100, 0, 100,  "z3Mass", "Z_{3} mass (GeV)", trees, cut);
    Compare_Distribution("zPt[2]/1000", 100, 0, 2500,     "z3Pt"  , "Z_{3} p_{T} (GeV)", trees, cut);
    Compare_Distribution("zEta[2]", 100, -10, 10,         "z3Eta" , "Z_{3} #eta", trees, cut);
    Compare_Distribution("zPhi[2]", 100, -5, 5,           "z3Phi" , "Z_{3} #Phi", trees, cut);
    
    Compare_Distribution("zMass[3]/1000", 100, 0, 100,  "z4Mass", "Z_{4} mass (GeV)", trees, cut);
    Compare_Distribution("zPt[3]/1000", 100, 0, 2500,     "z4Pt"  , "Z_{4} p_{T} (GeV)", trees, cut);
    Compare_Distribution("zEta[3]", 100, -10, 10,         "z4Eta" , "Z_{4} #eta", trees, cut);
    Compare_Distribution("zPhi[3]", 100, -5, 5,           "z4Phi" , "Z_{4} #Phi", trees, cut);

#endif

#if 1
    cut = "";
    Compare_Distribution("nPhotons", 10, 0, 10,                   "nPhotons", "N_{lep} (e, #mu, #tauh)", trees, cut);
    cut = "nPhotons>=2";
    Compare_Distribution("diphotonMass/1000", 100, 100, 150,  "diphotonMass", "diphoton mass (GeV)", trees, cut);
    Compare_Distribution("diphotonPt/1000", 100, 0, 2500,     "diphotonPt"  , "diphoton p_{T} (GeV)", trees, cut);
    Compare_Distribution("diphotonEta", 100, -10, 10,         "diphotonEta" , "diphoton #eta", trees, cut);
    Compare_Distribution("diphotonPhi", 100, -5, 5,           "diphotonPhi" , "diphoton #Phi", trees, cut);
    Compare_Distribution("diphotonDeltaR", 100, 0, 10,    "diphotonDeltaR", "#DeltaR", trees, cut);
    Compare_Distribution("diphotonDeltaEta", 100, -10, 10,"diphotonDeltaEta", "#Delta#eta", trees, cut);
    Compare_Distribution("diphotonDeltaPhi", 100, -5, 5,  "diphotonDeltaPhi", "#Delta#Phi", trees, cut);
    
    cut = "nPhotons>=1";
    Compare_Distribution("photonPt[0]/1000", 100, 0, 800, "photon1Pt", "leading photon p_{T} (GeV)", trees, cut);
    Compare_Distribution("photonEta[0]", 100, -10, 10, "photon1Eta", "leading photon #eta", trees, cut);
    Compare_Distribution("photonPhi[0]", 100, -5, 5, "photon1Phi", "leading photon #Phi", trees, cut);
    
    cut = "nPhotons>=2"; 
    Compare_Distribution("photonPt[1]/1000", 100, 0, 800, "photon2Pt", "sub-leading photon p_{T} (GeV)", trees, cut);
    Compare_Distribution("photonEta[1]", 100, -10, 10, "photon2Eta", "sub-leading photon #eta", trees, cut);
    Compare_Distribution("photonPhi[1]", 100, -5, 5, "photon2Phi", "sub-leading photon #Phi", trees, cut);
#endif

#if 1
    cut = "";
    Compare_Distribution("nLep", 10, 0, 10,                   "nLep", "N_{lep} (e, #mu, #tauh)", trees, cut);
    cut = "nLep>=2";
    Compare_Distribution("multileptonsDeltaR", 100, 0, 10,    "multileptonsDeltaR", "#DeltaR", trees, cut);
    Compare_Distribution("multileptonsDeltaEta", 100, -10, 10,"multileptonsDeltaEta", "#Delta#eta", trees, cut);
    Compare_Distribution("multileptonsDeltaPhi", 100, -5, 5,  "multileptonsDeltaPhi", "#Delta#Phi", trees, cut);
    
    cut = "nLep>=1";
    Compare_Distribution("leptonPt[0]/1000", 100, 0, 800, "lepton1Pt", "leading lepton p_{T(vis)} (GeV)", trees, cut);
    Compare_Distribution("leptonEta[0]", 100, -10, 10, "lepton1Eta", "leading lepton #eta(vis)", trees, cut);
    Compare_Distribution("leptonPhi[0]", 100, -5, 5, "lepton1Phi", "leading lepton #Phi(vis)", trees, cut);
    
    cut = "nLep>=2"; 
    Compare_Distribution("leptonPt[1]/1000", 100, 0, 800, "lepton2Pt", "sub-leading lepton p_{T(vis)} (GeV)", trees, cut);
    Compare_Distribution("leptonEta[1]", 100, -10, 10, "lepton2Eta", "sub-leading lepton #eta(vis)", trees, cut);
    Compare_Distribution("leptonPhi[1]", 100, -5, 5, "lepton2Phi", "sub-leading lepton #Phi(vis)", trees, cut);

    cut = "nLep>=3"; 
    Compare_Distribution("leptonPt[2]/1000", 100, 0, 200, "lepton3Pt", "3rd lepton p_{T(vis)} (GeV)", trees, cut);
    Compare_Distribution("leptonEta[2]", 100, -10, 10, "lepton3Eta", "3rd lepton #eta(vis)", trees, cut);
    Compare_Distribution("leptonPhi[2]", 100, -5, 5, "lepton3Phi", "3rd lepton #Phi(vis)", trees, cut);

    cut = "nLep>=4"; 
    Compare_Distribution("leptonPt[3]/1000", 100, 0, 200, "lepton4Pt", "4th lepton p_{T(vis)} (GeV)", trees, cut);
    Compare_Distribution("leptonEta[3]", 100, -10, 10, "lepton4Eta", "4th lepton #eta(vis)", trees, cut);
    Compare_Distribution("leptonPhi[3]", 100, -5, 5, "lepton4Phi", "4th lepton #Phi(vis)", trees, cut);

    cut = "nLep==2";
    Compare_Distribution("nEles*1+nMus*3+nTausH*10",27,0,27,"leptonFlavor","lepton flavor", trees, cut);
#endif

#if 1
    cut = "";
    Compare_Distribution("nEles", 10, 0, 10,                   "nEles", "N_{e}", trees, cut);
    
    cut = "nEles>=1";
    Compare_Distribution("electronPt[0]/1000", 100, 0, 800, "electron1Pt", "leading e p_{T} (GeV)", trees, cut);
    Compare_Distribution("electronEta[0]", 100, -10, 10, "electron1Eta", "leading e #eta", trees, cut);
    Compare_Distribution("electronPhi[0]", 100, -5, 5, "electron1Phi", "leading e #Phi", trees, cut);
    
    cut = "nEles>=2";
    Compare_Distribution("electronPt[1]/1000", 100, 0, 800, "electron2Pt", "sub-leading e p_{T} (GeV)", trees, cut);
    Compare_Distribution("electronEta[1]", 100, -10, 10, "electron2Eta", "sub-leading e #eta", trees, cut);
    Compare_Distribution("electronPhi[1]", 100, -5, 5, "electron2Phi", "sub-leading e #Phi", trees, cut);
    
    cut = "";
    Compare_Distribution("nMus", 10, 0, 10,                   "nMus", "N_{#mu}", trees, cut);
    
    cut = "nMus>=1";
    Compare_Distribution("muonPt[0]/1000", 100, 0, 800, "muon1Pt", "leading #mu p_{T} (GeV)", trees, cut);
    Compare_Distribution("muonEta[0]", 100, -10, 10, "muon1Eta", "leading #mu #eta", trees, cut);
    Compare_Distribution("muonPhi[0]", 100, -5, 5, "muon1Phi", "leading #mu #Phi", trees, cut);
    
    cut = "nMus>=2";
    Compare_Distribution("muonPt[1]/1000", 100, 0, 800, "muon2Pt", "sub-leading #mu p_{T} (GeV)", trees, cut);
    Compare_Distribution("muonEta[1]", 100, -10, 10, "muon2Eta", "sub-leading #mu #eta", trees, cut);
    Compare_Distribution("muonPhi[1]", 100, -5, 5, "muon2Phi", "sub-leading #mu #Phi", trees, cut);
    
    cut = "";
    Compare_Distribution("nTausH", 10, 0, 10,                   "nTausH", "N_{#tau_{h}}", trees, cut);
    
    cut = "nTausH>=1";
    Compare_Distribution("tauPt[0]/1000", 100, 0, 800, "tau1Pt", "leading #tau p_{Tvis} (GeV)", trees, cut);
    Compare_Distribution("tauEta[0]", 100, -10, 10, "tau1Eta", "leading #tau #etavis", trees, cut);
    Compare_Distribution("tauPhi[0]", 100, -5, 5, "tau1Phi", "leading #tau #Phivis", trees, cut);
    
    cut = "nTausH>=2"; 
    Compare_Distribution("tauPt[1]/1000", 100, 0, 800, "tau2Pt", "sub-leading #tau p_{Tvis} (GeV)", trees, cut);
    Compare_Distribution("tauEta[1]", 100, -10, 10, "tau2Eta", "sub-leading #tau #etavis", trees, cut);
    Compare_Distribution("tauPhi[1]", 100, -5, 5, "tau2Phi", "sub-leading #tau #Phivis", trees, cut);
#endif

#if 1
    cut = "";
    Compare_Distribution("nJets", 10, 0 ,10, "nJets", "number of jets", trees, cut);
    
    cut = "nVBFJets>=2";   
    Compare_Distribution("vbfJetPt[0]/1000", 100, 0, 800, "vbfJet0Pt", "leading VBF quark p_{T} (GeV)", trees, cut);
    Compare_Distribution("vbfJetEta[0]", 100, -10, 10, "vbfJet0Eta", "leading VBF quark #eta", trees, cut);
    Compare_Distribution("vbfJetPhi[0]", 100, -5, 5, "vbfJet0Phi", "leading VBF quark #Phi", trees, cut);
    
    Compare_Distribution("vbfJetPt[1]/1000", 100, 0, 800, "vbfJet1Pt", "sub-leading VBF quark p_{T} (GeV)", trees, cut);
    Compare_Distribution("vbfJetEta[1]", 100, -10, 10, "vbfJet1Eta", "sub-leading VBF quark #eta", trees, cut);
    Compare_Distribution("vbfJetPhi[1]", 100, -5, 5, "vbfJet1Phi", "sub-leading VBF quark #Phi", trees, cut);

    Compare_Distribution("vbfJetsDeltaR", 100, 0, 12,    "vbfJetsDeltaR", "#DeltaR", trees, cut);
    Compare_Distribution("vbfJetsDeltaEta", 100, -12, 12,"vbfJetsDeltaEta", "#Delta#eta", trees, cut);
    Compare_Distribution("vbfJetsDeltaPhi", 100, -5, 5,  "vbfJetsDeltaPhi", "#Delta#Phi", trees, cut);
    Compare_Distribution("vbfJetsMass/1000", 100, 0, 6000,  "vbfJetsMass", "M_{qq} (GeV)", trees, cut);
#endif 

    break;
  }
}
