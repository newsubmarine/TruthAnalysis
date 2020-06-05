void Compare_Distribution(TString var, int nBins, int xMin, int xMax, TString name,TString xTitle, TTree* tree, TCut cut)
{
gStyle->SetFillStyle(6040);
  TCanvas* c1 = new TCanvas(name+"_c1", "", 800, 600);

  int mode[6] = {30,38,39,46,47,48};
  TString title[6] = {"#tau#tau#tau#tau","#tau#tauZZ","#tau#tauWW","ZZZZ","WWZZ","WWWW"}; 
  TH1D* hist[6];
  int color[6]={1,2,3,4,5,7};
  THStack* stack = new THStack(name+"stack",""); 
  TCut cut1[6];
  long sum = 0;
  for (int i=0; i<6; ++i) {
    cut1[i] = cut && "higgsDecayMode[0]+higgsDecayMode[1]=="+convert<TString>(mode[i]);
    std::cout << cut1[i] << std::endl;
    hist[i] = new TH1D(name+title[i],"",nBins, xMin, xMax);
    sum += tree->Project(hist[i]->GetName(), var, cut1[i]);
    hist[i]->GetXaxis()->SetTitle(xTitle);
    hist[i]->GetYaxis()->SetTitle("A.U.");
    hist[i]->GetXaxis()->CenterTitle();
    hist[i] -> SetFillColor(color[i]);
    hist[i] -> SetLineStyle(0);
  }
  for (int i=0; i<6; ++i) {
      hist [i] -> Scale(1./sum);
      stack->Add(hist[i]);
  }
  stack->Draw("hist");
  TLegend* lg = new TLegend(0.82, 0.6, 0.95, 0.9);
  for (int i=0; i<6; ++i) {
    lg -> AddEntry(hist[i],title[i],"f");
  }
  lg -> SetFillStyle(0);
  lg -> Draw("same");
 
}




void draw_stack()
{
  SetBesStyle();
  gStyle->SetOptStat(0);
  TString file_name1 = "l2.root";
  TFile* f1 = new TFile(file_name1, "open");
  TTree* tree1 = (TTree*)f1->Get("Nominal/PreSelection_all_higgs");

  TCut cut = "";
#if 1
//  Compare_Distribution("dihiggsMass/1000", 100, 0, 3000, "dihiggsMass", "di-higgs mass (GeV)", tree1, tree2, tree3, cut);
//  Compare_Distribution("dihiggsPt/1000", 100, 0, 1500, "dihiggsPt", "di-higgs pt (GeV)", tree1, tree2, tree3,cut);
//  Compare_Distribution("dihiggsEta", 100, -10, 10, "dihiggsEta", "di-higgs eta", tree1, tree2, tree3,cut);
//  Compare_Distribution("dihiggsPhi", 100, -5, 5, "dihiggsPhi", "di-higgs phi", tree1, tree2, tree3,cut);
  Compare_Distribution("higgsDecayMode[0]+higgsDecayMode[1]",35,25,60, "higgsDecayMode", "di-higgs decay mode", tree1, cut);
  Compare_Distribution("dihiggsDeltaR", 100, 0, 7, "dihiggsDeltaR", "#DeltaR", tree1,cut);
  Compare_Distribution("dihiggsDeltaEta", 100, -10, 10, "dihiggsDeltaEta", "#Delta#eta", tree1,cut);
  Compare_Distribution("dihiggsDeltaPhi", 100, -5, 5, "dihiggsDeltaPhi", "#Delta#Phi", tree1,cut);
#endif

 
#if 0
  Compare_Distribution("higgsMass[0]/1000", 100, 100, 150, "higgs1Mass", "H1 mass (GeV)", tree1, tree2, tree3, cut);
  Compare_Distribution("higgsMass[1]/1000", 100, 100, 150, "higgs2Mass", "H2 mass (GeV)", tree1, tree2, tree3, cut);
  Compare_Distribution("higgsPt[0]/1000", 100, 0, 2500, "higgs1Pt", "H1 p_{T} (GeV)", tree1, tree2, tree3, cut);
  Compare_Distribution("higgsPt[1]/1000", 100, 0, 2500, "higgs2Pt", "H2 p_{T} (GeV)", tree1, tree2, tree3, cut);
  Compare_Distribution("higgsEta[0]", 100, -10, 10, "higgs1Eta", "H1 #eta", tree1, tree2, tree3, cut);
  Compare_Distribution("higgsEta[1]", 100, -10, 10, "higgs2Eta", "H2 #eta", tree1, tree2, tree3, cut);
  Compare_Distribution("higgsPhi[0]", 100, -5, 5, "higgs1Phi", "H1 #Phi", tree1, tree2, tree3, cut);
  Compare_Distribution("higgsPhi[1]", 100, -5, 5, "higgs2Phi", "H2 #Phi", tree1, tree2, tree3, cut);
#endif

  cut = "nLep>=2";
#if 0
//  Compare_Distribution("multileptonsPt/1000", 100, 0, 1500, "multileptonsPt", "multi-leptons pt (GeV)", tree1, tree2, tree3);
//  Compare_Distribution("multileptonsEta", 100, -10, 10, "multileptonsEta", "multi-leptons eta", tree1, tree2, tree3);
//  Compare_Distribution("multileptonsPhi", 100, -5, 5, "multileptonsPhi", "multi-leptons phi", tree1, tree2, tree3);
  Compare_Distribution("nLep", 10, 0, 10, "nLep", "N_{lep} (e, #mu, #tauh)", tree1, tree2, tree3, "");
  Compare_Distribution("multileptonsDeltaR", 100, 0, 10, "multileptonsDeltaR", "#DeltaR", tree1, tree2, tree3, cut);
  Compare_Distribution("multileptonsDeltaEta", 100, -10, 10, "multileptonsDeltaEta", "#Delta#eta", tree1, tree2, tree3, cut);
  Compare_Distribution("multileptonsDeltaPhi", 100, -5, 5, "multileptonsDeltaPhi", "#Delta#Phi", tree1, tree2, tree3, cut);
#endif 

#if 0
  Compare_Distribution("leptonPt[0]/1000", 100, 0, 800, "lepton1Pt", "leading lepton p_{T(vis)} (GeV)", tree1, tree2, tree3, "nLep>=1");
  Compare_Distribution("leptonPt[1]/1000", 100, 0, 800, "lepton2Pt", "sub-leading lepton p_{T(vis)} (GeV)", tree1, tree2, tree3, "nLep>=2");
  Compare_Distribution("leptonEta[0]", 100, -10, 10, "lepton1Eta", "leading lepton #eta(vis)", tree1, tree2, tree3, "nLep>=1");
  Compare_Distribution("leptonEta[1]", 100, -10, 10, "lepton2Eta", "sub-leading lepton #eta(vis)", tree1, tree2, tree3, "nLep>=2");
  Compare_Distribution("leptonPhi[0]", 100, -5, 5, "lepton1Phi", "leading lepton #Phi(vis)", tree1, tree2, tree3, "nLep>=1");
  Compare_Distribution("leptonPhi[1]", 100, -5, 5, "lepton2Phi", "sub-leading lepton #Phi(vis)", tree1, tree2, tree3, "nLep>=2");
#endif


#if 0
  Compare_Distribution("leptonPt[2]/1000", 100, 0, 200, "lepton3Pt", "3rd lepton p_{T(vis)} (GeV)", tree1, tree2, tree3, "nLep>=3");
  Compare_Distribution("leptonPt[3]/1000", 100, 0, 200, "lepton4Pt", "4th lepton p_{T(vis)} (GeV)", tree1, tree2, tree3, "nLep>=4");
  Compare_Distribution("leptonEta[2]", 100, -10, 10, "lepton3Eta", "3rd lepton #eta(vis)", tree1, tree2, tree3, "nLep>=3");
  Compare_Distribution("leptonEta[3]", 100, -10, 10, "lepton4Eta", "4th lepton #eta(vis)", tree1, tree2, tree3, "nLep>=4");
  Compare_Distribution("leptonPhi[2]", 100, -5, 5, "lepton3Phi", "3rd lepton #Phi(vis)", tree1, tree2, tree3, "nLep>=3");
  Compare_Distribution("leptonPhi[3]", 100, -5, 5, "lepton4Phi", "4th lepton #Phi(vis)", tree1, tree2, tree3, "nLep>=4");
#endif

#if 0
  Compare_Distribution("electronPt[0]/1000", 100, 0, 800, "electron1Pt", "leading e p_{T} (GeV)", tree1, tree2, tree3, "nEles>=1");
  Compare_Distribution("electronPt[1]/1000", 100, 0, 800, "electron2Pt", "sub-leading e p_{T} (GeV)", tree1, tree2, tree3, "nEles>=2");
  Compare_Distribution("electronEta[0]", 100, -10, 10, "electron1Eta", "leading e #eta", tree1, tree2, tree3, "nEles>=1");
  Compare_Distribution("electronEta[1]", 100, -10, 10, "electron2Eta", "sub-leading e #eta", tree1, tree2, tree3, "nEles>=2");
  Compare_Distribution("electronPhi[0]", 100, -5, 5, "electron1Phi", "leading e #Phi", tree1, tree2, tree3, "nEles>=1");
  Compare_Distribution("electronPhi[1]", 100, -5, 5, "electron2Phi", "sub-leading e #Phi", tree1, tree2, tree3, "nEles>=2");
#endif

#if 0
  Compare_Distribution("muonPt[0]/1000", 100, 0, 800, "muon1Pt", "leading #mu p_{T} (GeV)", tree1, tree2, tree3, "nMus>=1");
  Compare_Distribution("muonPt[1]/1000", 100, 0, 800, "muon2Pt", "sub-leading #mu p_{T} (GeV)", tree1, tree2, tree3, "nMus>=2");
  Compare_Distribution("muonEta[0]", 100, -10, 10, "muon1Eta", "leading #mu #eta", tree1, tree2, tree3, "nMus>=1");
  Compare_Distribution("muonEta[1]", 100, -10, 10, "muon2Eta", "sub-leading #mu #eta", tree1, tree2, tree3, "nMus>=2");
  Compare_Distribution("muonPhi[0]", 100, -5, 5, "muon1Phi", "leading #mu #Phi", tree1, tree2, tree3, "nMus>=1");
  Compare_Distribution("muonPhi[1]", 100, -5, 5, "muon2Phi", "sub-leading #mu #Phi", tree1, tree2, tree3, "nMus>=2");
#endif

#if 0
  Compare_Distribution("tauPt[0]/1000", 100, 0, 800, "tau1Pt", "leading #tau p_{Tvis} (GeV)", tree1, tree2, tree3, "nTausH>=1");
  Compare_Distribution("tauPt[1]/1000", 100, 0, 800, "tau2Pt", "sub-leading #tau p_{Tvis} (GeV)", tree1, tree2, tree3, "nTausH>=2");
  Compare_Distribution("tauEta[0]", 100, -10, 10, "tau1Eta", "leading #tau #etavis", tree1, tree2, tree3, "nTausH>=1");
  Compare_Distribution("tauEta[1]", 100, -10, 10, "tau2Eta", "sub-leading #tau #etavis", tree1, tree2, tree3, "nTausH>=2");
  Compare_Distribution("tauPhi[0]", 100, -5, 5, "tau1Phi", "leading #tau #Phivis", tree1, tree2, tree3, "nTausH>=1");
  Compare_Distribution("tauPhi[1]", 100, -5, 5, "tau2Phi", "sub-leading #tau #Phivis", tree1, tree2, tree3, "nTausH>=2");
#endif

}
