
TString num_lep = "2l";

void Compare_Distribution(TString var, int nBins, int xMin, int xMax, TString name,TString xTitle, TTree* tree1, TTree* tree2, TCut cut)
{
  TCanvas* c1 = new TCanvas(name+"_c1", "", 800, 600);

  TH1D* hist1;
  if (tree1!=nullptr) { 
  hist1 = new TH1D(name, "", nBins, xMin, xMax);
  long sum1 = tree1->Project(hist1->GetName(), var, cut);
  hist1->GetXaxis()->SetTitle(xTitle);
  hist1->GetYaxis()->SetTitle("A.U.");
  hist1->GetXaxis()->CenterTitle();
  hist1->SetLineColor(kBlack);
  hist1->SetLineWidth(2);
  hist1->Scale(1./sum1);
  hist1->Draw("hist");
  }

  TH1D* hist2;
  if (tree2!=nullptr) {
  hist2 = new TH1D(name+"_hist2", "", nBins, xMin, xMax);
  long sum2 = tree2->Project(hist2->GetName(), var, cut);
  hist2->GetXaxis()->SetTitle(xTitle);
  hist2->GetXaxis()->CenterTitle();
  hist2->SetLineColor(kRed);
  hist2->SetLineWidth(2);
  hist2->Scale(1./sum2);
  if (tree1==nullptr) hist2->Draw();
  else hist2->Draw("samehist");
  }
  
  TLegend* lg = new TLegend(0.82, 0.6, 0.95, 0.9);
  if (tree1!=nullptr)  lg -> AddEntry(hist1, num_lep+", #Kappa_{#lambda}=1", "l");
  if (tree2!=nullptr)  lg -> AddEntry(hist2, num_lep+", #Kappa_{#lambda}=10", "l");
  lg -> SetFillStyle(0);
  lg -> SetBorderSize(0);
  lg -> Draw("same");
 
  double y1 = 0;
  if (tree1!=nullptr) y1= hist1->GetMaximum();
  double y2 = 0;
  if (tree2!=nullptr) y2= hist2->GetMaximum();
  double max = y1;
  if (y2>max) max = y2;

  if (tree1!=nullptr) hist1->GetYaxis()->SetRangeUser(0, 1.15*max);
  else if (tree2!=nullptr) hist2->GetYaxis()->SetRangeUser(0, 1.15*max);
  c1->Update();
  c1->SaveAs(name+".png");
}




void compare_KLs()
{
  //SetAtlasStyle();
  //gStyle->SetOptStat(0);

  TString file_name1 = num_lep+"_01.root";
  TFile* f1 = new TFile(file_name1, "open");
  TTree* tree1 = (TTree*)f1->Get("Nominal/PreSelection_all_higgs");

  TString file_name2 = num_lep+"_10.root";
  TFile* f2 = new TFile(file_name2, "open");
  TTree* tree2 = (TTree*)f2->Get("Nominal/PreSelection_all_higgs");
  
  //Compare_Distribution("higgsPt[0]/1000", 100, 0, 1000,     "higgs1Pt"  , "H1 p_{T} (GeV)", tree1, tree2, "");

  int mode[6] = {30,38,39,46,47,48};
  TString title[6] = {"#tau#tau#tau#tau","#tau#tauZZ","#tau#tauWW","ZZZZ","WWZZ","WWWW"}; 
  TCut cut = "";
  for (int i=0; i<6; ++i) {
    //TString cut_mode = "higgsDecayMode[0]+higgsDecayMode[1]=="+convert<TString>(mode[i]);
    TString cut_mode = "";
    cut = cut && cut_mode;
    title[0] = "";
#if 1
    Compare_Distribution("higgsDecayMode[0]+higgsDecayMode[1]",35,25,60, "higgsDecayMode", "di-higgs decay mode", tree1, tree2, cut);
    Compare_Distribution("dihiggsDeltaR", 100, 0, 7, "dihiggsDeltaR", "#DeltaR", tree1, tree2,cut);
    Compare_Distribution("dihiggsDeltaEta", 100, -10, 10, "dihiggsDeltaEta", "#Delta#eta", tree1, tree2,cut);
    Compare_Distribution("dihiggsDeltaPhi", 100, -5, 5, "dihiggsDeltaPhi", "#Delta#Phi", tree1, tree2,cut);
    
    Compare_Distribution("higgsMass[0]/1000", 100, 100, 150,  "higgs1Mass", "H1 mass (GeV)", tree1, tree2, cut);
    Compare_Distribution("higgsPt[0]/1000", 100, 0, 1000,     "higgs1Pt"  , "H1 p_{T} (GeV)", tree1, tree2, cut);
    Compare_Distribution("higgsEta[0]", 100, -10, 10,         "higgs1Eta" , "H1 #eta", tree1, tree2, cut);
    Compare_Distribution("higgsPhi[0]", 100, -5, 5,           "higgs1Phi" , "H1 #Phi", tree1, tree2, cut);
    
    Compare_Distribution("higgsMass[1]/1000", 100, 100, 150,  "higgs2Mass", "H2 mass (GeV)", tree1, tree2, cut);
    Compare_Distribution("higgsPt[1]/1000", 100, 0, 1000,     "higgs2Pt"  , "H2 p_{T} (GeV)", tree1, tree2, cut);
    Compare_Distribution("higgsEta[1]", 100, -10, 10,         "higgs2Eta" , "H2 #eta", tree1, tree2, cut);
    Compare_Distribution("higgsPhi[1]", 100, -5, 5,           "higgs2Phi" , "H2 #Phi", tree1, tree2, cut);
#endif

#if 1
    cut = "";
    Compare_Distribution("nLep", 10, 0, 10,                   "nLep", "N_{lep} (e, #mu, #tauh)", tree1, tree2, cut);
    cut = "nLep>=2";
    Compare_Distribution("multileptonsDeltaR", 100, 0, 6,    "multileptonsDeltaR", "#DeltaR", tree1, tree2, cut);
    Compare_Distribution("multileptonsDeltaEta", 100, -6, 6,"multileptonsDeltaEta", "#Delta#eta", tree1, tree2, cut);
    Compare_Distribution("multileptonsDeltaPhi", 100, -5, 5,  "multileptonsDeltaPhi", "#Delta#Phi", tree1, tree2, cut);
    
    cut = "nLep>=1";
    Compare_Distribution("leptonPt[0]/1000", 100, 0, 300, "lepton1Pt", "leading lepton p_{T(vis)} (GeV)", tree1, tree2, cut);
    Compare_Distribution("leptonEta[0]", 100, -6, 6, "lepton1Eta", "leading lepton #eta(vis)", tree1, tree2, cut);
    Compare_Distribution("leptonPhi[0]", 100, -5, 5, "lepton1Phi", "leading lepton #Phi(vis)", tree1, tree2, cut);
    
    cut = "nLep>=2"; 
    Compare_Distribution("leptonPt[1]/1000", 100, 0, 300, "lepton2Pt", "sub-leading lepton p_{T(vis)} (GeV)", tree1, tree2, cut);
    Compare_Distribution("leptonEta[1]", 100, -6, 6, "lepton2Eta", "sub-leading lepton #eta(vis)", tree1, tree2, cut);
    Compare_Distribution("leptonPhi[1]", 100, -5, 5, "lepton2Phi", "sub-leading lepton #Phi(vis)", tree1, tree2, cut);

    cut = "nLep>=3"; 
    Compare_Distribution("leptonPt[2]/1000", 100, 0, 300, "lepton3Pt", "3rd lepton p_{T(vis)} (GeV)", tree1, tree2, cut);
    Compare_Distribution("leptonEta[2]", 100, -6, 6, "lepton3Eta", "3rd lepton #eta(vis)", tree1, tree2, cut);
    Compare_Distribution("leptonPhi[2]", 100, -5, 5, "lepton3Phi", "3rd lepton #Phi(vis)", tree1, tree2, cut);

    cut = "nLep>=4"; 
    Compare_Distribution("leptonPt[3]/1000", 100, 0, 300, "lepton4Pt", "4th lepton p_{T(vis)} (GeV)", tree1, tree2, cut);
    Compare_Distribution("leptonEta[3]", 100, -6, 6, "lepton4Eta", "4th lepton #eta(vis)", tree1, tree2, cut);
    Compare_Distribution("leptonPhi[3]", 100, -5, 5, "lepton4Phi", "4th lepton #Phi(vis)", tree1, tree2, cut);

    cut = "nLep==2";
    Compare_Distribution("nEles*1+nMus*3+nTausH*10",27,0,27,"leptonFlavor","lepton flavor", tree1, tree2, cut);
#endif

#if 1
    cut = "nEles>=1";
    Compare_Distribution("electronPt[0]/1000", 100, 0, 300, "electron1Pt", "leading e p_{T} (GeV)", tree1, tree2, cut);
    Compare_Distribution("electronEta[0]", 100, -6, 6, "electron1Eta", "leading e #eta", tree1, tree2, cut);
    Compare_Distribution("electronPhi[0]", 100, -5, 5, "electron1Phi", "leading e #Phi", tree1, tree2, cut);
    
    cut = "nEles>=2";
    Compare_Distribution("electronPt[1]/1000", 100, 0, 300, "electron2Pt", "sub-leading e p_{T} (GeV)", tree1, tree2, cut);
    Compare_Distribution("electronEta[1]", 100, -6, 6, "electron2Eta", "sub-leading e #eta", tree1, tree2, cut);
    Compare_Distribution("electronPhi[1]", 100, -5, 5, "electron2Phi", "sub-leading e #Phi", tree1, tree2, cut);
    
    cut = "nMus>=1";
    Compare_Distribution("muonPt[0]/1000", 100, 0, 300, "muon1Pt", "leading #mu p_{T} (GeV)", tree1, tree2, cut);
    Compare_Distribution("muonEta[0]", 100, -6, 6, "muon1Eta", "leading #mu #eta", tree1, tree2, cut);
    Compare_Distribution("muonPhi[0]", 100, -5, 5, "muon1Phi", "leading #mu #Phi", tree1, tree2, cut);
    
    cut = "nMus>=2";
    Compare_Distribution("muonPt[1]/1000", 100, 0, 300, "muon2Pt", "sub-leading #mu p_{T} (GeV)", tree1, tree2, cut);
    Compare_Distribution("muonEta[1]", 100, -6, 6, "muon2Eta", "sub-leading #mu #eta", tree1, tree2, cut);
    Compare_Distribution("muonPhi[1]", 100, -5, 5, "muon2Phi", "sub-leading #mu #Phi", tree1, tree2, cut);
    
    cut = "nTausH>=1";
    Compare_Distribution("tauPt[0]/1000", 100, 0, 300, "tau1Pt", "leading #tau p_{Tvis} (GeV)", tree1, tree2, cut);
    Compare_Distribution("tauEta[0]", 100, -6, 6, "tau1Eta", "leading #tau #etavis", tree1, tree2, cut);
    Compare_Distribution("tauPhi[0]", 100, -5, 5, "tau1Phi", "leading #tau #Phivis", tree1, tree2, cut);
    
    cut = "nTausH>=2"; 
    Compare_Distribution("tauPt[1]/1000", 100, 0, 300, "tau2Pt", "sub-leading #tau p_{Tvis} (GeV)", tree1, tree2, cut);
    Compare_Distribution("tauEta[1]", 100, -6, 6, "tau2Eta", "sub-leading #tau #etavis", tree1, tree2, cut);
    Compare_Distribution("tauPhi[1]", 100, -5, 5, "tau2Phi", "sub-leading #tau #Phivis", tree1, tree2, cut);
#endif

#if 1
    cut = "";
    Compare_Distribution("nJets", 10, 0 ,10, "nJets", "number of jets", tree1, tree2, cut);
    
    cut = "nVBFJets>=2";   
    Compare_Distribution("vbfJetPt[0]/1000", 100, 0, 800, "vbfJet0Pt", "leading VBF jet p_{T} (GeV)", tree1, tree2, cut);
    Compare_Distribution("vbfJetEta[0]", 100, -10, 10, "vbfJet0Eta", "leading VBF Jet #eta", tree1, tree2, cut);
    Compare_Distribution("vbfJetPhi[0]", 100, -5, 5, "vbfJet0Phi", "leading VBF Jet #Phi", tree1, tree2, cut);
    
    Compare_Distribution("vbfJetPt[1]/1000", 100, 0, 800, "vbfJet1Pt", "sub-leading VBF jet p_{T} (GeV)", tree1, tree2, cut);
    Compare_Distribution("vbfJetEta[1]", 100, -10, 10, "vbfJet1Eta", "sub-leading VBF Jet #eta", tree1, tree2, cut);
    Compare_Distribution("vbfJetPhi[1]", 100, -5, 5, "vbfJet1Phi", "sub-leading VBF Jet #Phi", tree1, tree2, cut);

    Compare_Distribution("vbfJetsDeltaR", 100, 0, 12,    "vbfJetsDeltaR", "#DeltaR", tree1, tree2, cut);
    Compare_Distribution("vbfJetsDeltaEta", 100, -12, 12,"vbfJetsDeltaEta", "#Delta#eta", tree1, tree2, cut);
    Compare_Distribution("vbfJetsDeltaPhi", 100, -5, 5,  "vbfJetsDeltaPhi", "#Delta#Phi", tree1, tree2, cut);
#endif 

    break;
  }
}
