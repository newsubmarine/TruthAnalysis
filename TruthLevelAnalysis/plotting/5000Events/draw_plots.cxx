#include <iomanip>
#include <fstream>
#include <THStack.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1F.h>
#include <TMath.h>
#include <TTree.h>
#include <iostream>
#include <map>
#include <vector>
#include <TStyle.h>
#include <TCut.h>
#include <TFile.h>
#include <TLegend.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TPolyMarker.h>
#include <TPaveStats.h>
#include "/afs/ihep.ac.cn/users/x/xinst/myROOTUtils/AtlasStyle.C"
#include "/afs/ihep.ac.cn/users/x/xinst/myROOTUtils/AtlasUtils.C"

using namespace std;
using namespace ROOT;

//#include "system"
void Compare_Distribution(TString var, int nBins, int xMin, int xMax, TString name,TString xTitle, const std::vector<TTree*> trees, TCut cut)
{
    TCanvas* c1 = new TCanvas(name+"_c1", "", 800, 800);
    //ratio plot
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.35, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    //pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();
    std::vector<TH1D*> hists;
    std::vector<unsigned int> colors = {kBlue, kRed};
    std::vector<int> styles = {0, 2};

    double maximum = 0.0;

    TString option = cut.GetTitle();
    if (option.Length() == 0) {
        option = "weight";
    }
    else {
        option = TString::Format("(%s)*weight", cut.GetTitle());
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
        hist->SetLineColor(colors.at(index%styles.size()));
        //hist->SetLineStyle(styles.at(index%styles.size()));
        hists.push_back(hist);
        ATLAS_LABEL(0.20, 0.87 );
        myText( 0.353, 0.87 , 1, "Internal");
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
    TLegend* lg = new TLegend(0.80, 0.7, 0.95, 0.9);

    for (const TH1D* hist : hists) {
        lg->AddEntry(hist, hist->GetTitle(), "l");
    }

    lg -> SetFillStyle(0);
    lg -> SetBorderSize(0);
    lg -> Draw("same");


    c1->cd();
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.35);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.3);
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad
    TH1F *ratio = (TH1F*)hists.at(0)->Clone("pythia8");
    ratio->SetLineColor(kRed);
    ratio->SetMinimum(0);  // Define Y ..
    ratio->SetMaximum(2); // .. range
    ratio->Sumw2();
    //ratio->SetStats(0);
    ratio->Divide(hists.at(1));
    ratio->SetMarkerStyle(20);
    //ratio->SetGridlines(1);
    ratio->Draw("ep");       // Draw the ratio plot
    ratio->SetTitle(""); // Remove the ratio title

    ratio->GetYaxis()->SetTitle("pythia8/herwig7");
    ratio->GetYaxis()->SetNdivisions(505);
    ratio->GetXaxis()->SetTitle(xTitle);
    
    ratio->GetYaxis()->SetTitleSize(0.12);
    ratio->GetYaxis()->SetTitleFont(42);
    ratio->GetYaxis()->SetTitleOffset(0.5);
    ratio->GetYaxis()->SetLabelFont(42); // Absolute font size in pixel (precision 3)
    ratio->GetYaxis()->SetLabelSize(0.12);
    
    ratio->GetXaxis()->SetTitleSize(0.12);
    ratio->GetXaxis()->SetTitleFont(42);
    ratio->GetXaxis()->SetTitleOffset(1.);
    ratio->GetXaxis()->SetLabelFont(42); // Absolute font size in pixel (precision 3)
    ratio->GetXaxis()->SetLabelSize(0.12);
     
    c1->Update();
    TLine *l=new TLine(ratio->GetXaxis()->GetXmin(),1.0,ratio->GetXaxis()->GetXmax(),1.0);
    //cout <<" left " << c1->GetUxmin() << " right " << c1->GetUxmax() << endl;
    l->SetLineColor(kRed);
    l->SetLineStyle(2);
    l->Draw();
    c1->SaveAs(name+".pdf");
}




void draw_plots() {

    // Set the ATLAS plotting style
    //gROOT->SetStyle("ATLAS");
    SetAtlasStyle();
    gStyle->SetOptStat(0);
    // Generators to compare
    std::vector<TString> generators = {"pythia8","herwig7"};
    // Samples to compare
    //std::vector<TString> samples = {"2l0tau", "2l1tau", "3l0tau", "3l1tau", "4l0tau", "4l1tau","yy0l","yy1l","yy2l"};
    std::vector<TString> samples = {"bbZZ_4l","bbZZ_2l2v"};
    // SM or BSM 
    std::vector<TString> versions = {"cHHHp01d0","cHHHp10d0"};

    std::vector<TTree*> trees;
    for (const TString& sample : samples){
        for (const TString&version : versions){
            for (const TString& generator : generators) {
                TString fileName = generator + "/" + sample + "/" + sample + "_" + version;
                TFile* file = new TFile(fileName + ".root", "open");
                TTree* tree = static_cast<TTree*>(file->Get("Nominal/PreSelection_all_higgs"));
                tree->SetTitle(generator);
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
    Compare_Distribution("dihiggsPt/1000", 100, 0, 1000,     "dihiggsPt"  , "HH p_{T} (GeV)", trees, cut);
    Compare_Distribution("dihiggsEta", 100, -10, 10,         "dihiggsEta" , "HH #eta", trees, cut);
    Compare_Distribution("dihiggsPhi", 100, -5, 5,           "dihiggsPhi" , "HH #Phi", trees, cut);
    
    Compare_Distribution("higgsMass[0]/1000", 100, 100, 150,  "higgs1Mass", "H1 mass (GeV)", trees, cut);
    Compare_Distribution("higgsPt[0]/1000", 100, 0, 1000,     "higgs1Pt"  , "H1 p_{T} (GeV)", trees, cut);
    Compare_Distribution("higgsEta[0]", 100, -10, 10,         "higgs1Eta" , "H1 #eta", trees, cut);
    Compare_Distribution("higgsPhi[0]", 100, -5, 5,           "higgs1Phi" , "H1 #Phi", trees, cut);
    
    Compare_Distribution("higgsMass[1]/1000", 100, 100, 150,  "higgs2Mass", "H2 mass (GeV)", trees, cut);
    Compare_Distribution("higgsPt[1]/1000", 100, 0, 1000,     "higgs2Pt"  , "H2 p_{T} (GeV)", trees, cut);
    Compare_Distribution("higgsEta[1]", 100, -10, 10,         "higgs2Eta" , "H2 #eta", trees, cut);
    Compare_Distribution("higgsPhi[1]", 100, -5, 5,           "higgs2Phi" , "H2 #Phi", trees, cut);
#endif

#if 1
    cut = "";
    Compare_Distribution("nW", 10, 0, 10,                   "nW", "N_{W}", trees, cut);

    cut = "nW>=2";
    Compare_Distribution("wMass[0]/1000", 100, 0, 100,  "w1Mass", "W_{1} mass (GeV)", trees, cut);
    Compare_Distribution("wPt[0]/1000", 100, 0, 1000,     "w1Pt"  , "W_{1} p_{T} (GeV)", trees, cut);
    Compare_Distribution("wEta[0]", 100, -10, 10,         "w1Eta" , "W_{1} #eta", trees, cut);
    Compare_Distribution("wPhi[0]", 100, -5, 5,           "w1Phi" , "W_{1} #Phi", trees, cut);
    
    Compare_Distribution("wMass[1]/1000", 100, 0, 100,  "w2Mass", "W_{2} mass (GeV)", trees, cut);
    Compare_Distribution("wPt[1]/1000", 100, 0, 1000,     "w2Pt"  , "W_{2} p_{T} (GeV)", trees, cut);
    Compare_Distribution("wEta[1]", 100, -10, 10,         "w2Eta" , "W_{2} #eta", trees, cut);
    Compare_Distribution("wPhi[1]", 100, -5, 5,           "w2Phi" , "W_{2} #Phi", trees, cut);
    
    cut = "nW>=4";
    Compare_Distribution("wMass[2]/1000", 100, 0, 100,  "w3Mass", "W_{3} mass (GeV)", trees, cut);
    Compare_Distribution("wPt[2]/1000", 100, 0, 1000,     "w3Pt"  , "W_{3} p_{T} (GeV)", trees, cut);
    Compare_Distribution("wEta[2]", 100, -10, 10,         "w3Eta" , "W_{3} #eta", trees, cut);
    Compare_Distribution("wPhi[2]", 100, -5, 5,           "w3Phi" , "W_{3} #Phi", trees, cut);
    
    Compare_Distribution("wMass[3]/1000", 100, 0, 100,  "w4Mass", "W_{4} mass (GeV)", trees, cut);
    Compare_Distribution("wPt[3]/1000", 100, 0, 1000,     "w4Pt"  , "W_{4} p_{T} (GeV)", trees, cut);
    Compare_Distribution("wEta[3]", 100, -10, 10,         "w4Eta" , "W_{4} #eta", trees, cut);
    Compare_Distribution("wPhi[3]", 100, -5, 5,           "w4Phi" , "W_{4} #Phi", trees, cut);

    cut = "";
    Compare_Distribution("nZ", 10, 0, 10,                   "nZ", "N_{Z}", trees, cut);

    cut = "nZ>=2";
    Compare_Distribution("zMass[0]/1000", 100, 0, 100,  "z1Mass", "Z_{1} mass (GeV)", trees, cut);
    Compare_Distribution("zPt[0]/1000", 100, 0, 1000,     "z1Pt"  , "Z_{1} p_{T} (GeV)", trees, cut);
    Compare_Distribution("zEta[0]", 100, -10, 10,         "z1Eta" , "Z_{1} #eta", trees, cut);
    Compare_Distribution("zPhi[0]", 100, -5, 5,           "z1Phi" , "Z_{1} #Phi", trees, cut);
    
    Compare_Distribution("zMass[1]/1000", 100, 0, 100,  "z2Mass", "Z_{2} mass (GeV)", trees, cut);
    Compare_Distribution("zPt[1]/1000", 100, 0, 1000,     "z2Pt"  , "Z_{2} p_{T} (GeV)", trees, cut);
    Compare_Distribution("zEta[1]", 100, -10, 10,         "z2Eta" , "Z_{2} #eta", trees, cut);
    Compare_Distribution("zPhi[1]", 100, -5, 5,           "z2Phi" , "Z_{2} #Phi", trees, cut);
    
    cut = "nZ>=4";
    Compare_Distribution("zMass[2]/1000", 100, 0, 100,  "z3Mass", "Z_{3} mass (GeV)", trees, cut);
    Compare_Distribution("zPt[2]/1000", 100, 0, 1000,     "z3Pt"  , "Z_{3} p_{T} (GeV)", trees, cut);
    Compare_Distribution("zEta[2]", 100, -10, 10,         "z3Eta" , "Z_{3} #eta", trees, cut);
    Compare_Distribution("zPhi[2]", 100, -5, 5,           "z3Phi" , "Z_{3} #Phi", trees, cut);
    
    Compare_Distribution("zMass[3]/1000", 100, 0, 100,  "z4Mass", "Z_{4} mass (GeV)", trees, cut);
    Compare_Distribution("zPt[3]/1000", 100, 0, 1000,     "z4Pt"  , "Z_{4} p_{T} (GeV)", trees, cut);
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
    Compare_Distribution("photonPt[1]/1000", 100, 0, 500, "photon2Pt", "sub-leading photon p_{T} (GeV)", trees, cut);
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
    Compare_Distribution("leptonPt[0]/1000", 100, 0, 500, "lepton1Pt", "leading lepton p_{T(vis)} (GeV)", trees, cut);
    Compare_Distribution("leptonEta[0]", 100, -10, 10, "lepton1Eta", "leading lepton #eta(vis)", trees, cut);
    Compare_Distribution("leptonPhi[0]", 100, -5, 5, "lepton1Phi", "leading lepton #Phi(vis)", trees, cut);
    
    cut = "nLep>=2"; 
    Compare_Distribution("leptonPt[1]/1000", 100, 0, 300, "lepton2Pt", "sub-leading lepton p_{T(vis)} (GeV)", trees, cut);
    Compare_Distribution("leptonEta[1]", 100, -10, 10, "lepton2Eta", "sub-leading lepton #eta(vis)", trees, cut);
    Compare_Distribution("leptonPhi[1]", 100, -5, 5, "lepton2Phi", "sub-leading lepton #Phi(vis)", trees, cut);

    cut = "nLep>=3"; 
    Compare_Distribution("leptonPt[2]/1000", 100, 0, 160, "lepton3Pt", "3rd lepton p_{T(vis)} (GeV)", trees, cut);
    Compare_Distribution("leptonEta[2]", 100, -10, 10, "lepton3Eta", "3rd lepton #eta(vis)", trees, cut);
    Compare_Distribution("leptonPhi[2]", 100, -5, 5, "lepton3Phi", "3rd lepton #Phi(vis)", trees, cut);

    cut = "nLep>=4"; 
    Compare_Distribution("leptonPt[3]/1000", 100, 0, 160, "lepton4Pt", "4th lepton p_{T(vis)} (GeV)", trees, cut);
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
    Compare_Distribution("muonPt[0]/1000", 100, 0, 500, "muon1Pt", "leading #mu p_{T} (GeV)", trees, cut);
    Compare_Distribution("muonEta[0]", 100, -10, 10, "muon1Eta", "leading #mu #eta", trees, cut);
    Compare_Distribution("muonPhi[0]", 100, -5, 5, "muon1Phi", "leading #mu #Phi", trees, cut);
    
    cut = "nMus>=2";
    Compare_Distribution("muonPt[1]/1000", 100, 0, 400, "muon2Pt", "sub-leading #mu p_{T} (GeV)", trees, cut);
    Compare_Distribution("muonEta[1]", 100, -10, 10, "muon2Eta", "sub-leading #mu #eta", trees, cut);
    Compare_Distribution("muonPhi[1]", 100, -5, 5, "muon2Phi", "sub-leading #mu #Phi", trees, cut);
    
    cut = "";
    Compare_Distribution("nTausH", 10, 0, 10,                   "nTausH", "N_{#tau_{h}}", trees, cut);
    
    cut = "nTausH>=1";
    Compare_Distribution("tauPt[0]/1000", 100, 0, 400, "tau1Pt", "leading #tau p_{Tvis} (GeV)", trees, cut);
    Compare_Distribution("tauEta[0]", 100, -10, 10, "tau1Eta", "leading #tau #etavis", trees, cut);
    Compare_Distribution("tauPhi[0]", 100, -5, 5, "tau1Phi", "leading #tau #Phivis", trees, cut);
    
    cut = "nTausH>=2"; 
    Compare_Distribution("tauPt[1]/1000", 100, 0, 300, "tau2Pt", "sub-leading #tau p_{Tvis} (GeV)", trees, cut);
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

            TString command;
            command = "mkdir -p "+ sample + "/" + version;
            system(command.Data());
            TString mv_command = "mv *pdf ./" + sample + "/" + version;
            system(mv_command.Data());

            trees.clear();
            
        }
    }
}

int main(){
    draw_plots();
    return 0;
}
