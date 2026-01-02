
void plot_all_masses_with_fsroot()
{
    TString FN("/w/halld-scshelf2101/home/ilbelov/DataSpring2017/flattree_2pip2pim_sum_30274_31057.root");
    TString TN(FSTree::getTreeNameFromFile(FN));

    FSHistogram::readHistogramCache();
    
    // Define histograms coming from generated file
    FSHistogram::enableRDataFrame(false);
    /*
    TH1F* tslope = FSHistogram::getTH1F(FN,TN,"-1.*MASS2(GLUEXTARGET,-1)","(100,-0.01,4)","");
    TH1F* uvMass = FSHistogram::getTH1F(FN,TN,"MASS(2,3)","(100,0.8,4)","");
    TH1F* lvMass = FSHistogram::getTH1F(FN,TN,"MASS(1)","(100,0.7,1.1)","");
    TH1F* pBeam  = FSHistogram::getTH1F(FN,TN,"EnPB","(100,6,11)","");
    TH2F* aVSm   = FSHistogram::getTH2F(FN,TN,"GJCOSTHETA(3;2;B):MASS(2,3)","(75,0.8,4,75,-1.1,1.1)","");
    */

    /*
    TH1F *piplus1piminus1_mass = FSHistogram::getTH1F(FN,TN,"Piplus1Piminus1_M","(100,0.,3.)","");
    TH1F *piplus1piminus2_mass = FSHistogram::getTH1F(FN,TN,"Piplus1Piminus2_M","(100,0.,3.)","");
    TH1F *piplus2piminus1_mass = FSHistogram::getTH1F(FN,TN,"Piplus2Piminus1_M","(100,0.,3.)","");
    TH1F *piplus2piminus2_mass = FSHistogram::getTH1F(FN,TN,"Piplus2Piminus2_M","(100,0.,3.)","");

    TH1F *piplus1piplus2piminus1_mass = FSHistogram::getTH1F(FN,TN,"Piplus1Piplus2Piminus1_M","(100,0.2,3.2)","");
    TH1F *piplus1piplus2piminus2_mass = FSHistogram::getTH1F(FN,TN,"Piplus1Piplus2Piminus2_M","(100,0.2,3.2)","");
    TH1F *piplus1piminus1piminus2_mass = FSHistogram::getTH1F(FN,TN,"Piplus1Piminus1Piminus2_M","(100,0.2,3.2)","");
    TH1F *piplus2piminus1piminus2_mass = FSHistogram::getTH1F(FN,TN,"Piplus2Piminus1Piminus2_M","(100,0.2,3.2)","");
    */
    
    TH1F *piplus1proton_mass = FSHistogram::getTH1F(FN,TN,"Piplus1Proton_M","(100,0.8,3.8)","");
    TH1F *piplus2proton_mass = FSHistogram::getTH1F(FN,TN,"Piplus2Proton_M","(100,0.8,3.8)","");
    TH1F *piminus1proton_mass = FSHistogram::getTH1F(FN,TN,"Piminus1Proton_M","(100,0.8,3.8)","");
    TH1F *piminus2proton_mass = FSHistogram::getTH1F(FN,TN,"Piminus2Proton_M","(100,0.8,3.8)","");

    
    
    
    FSHistogram::executeRDataFrame();

    // Place a title for each histogram

    /*    
    piplus1piminus1_mass->SetTitle("; m(#pi^{+}#pi^{-})_{11}  [GeV]; Events/ 0.03 [GeV]");
    piplus1piminus2_mass->SetTitle("; m(#pi^{+}#pi^{-})_{12}  [GeV]; Events/ 0.03 [GeV]");
    piplus2piminus1_mass->SetTitle("; m(#pi^{+}#pi^{-})_{21}  [GeV]; Events/ 0.03 [GeV]");
    piplus2piminus2_mass->SetTitle("; m(#pi^{+}#pi^{-})_{22}  [GeV]; Events/ 0.03 [GeV]");

    piplus1piplus2piminus1_mass->SetTitle("; m(#pi^{+}#pi^{+}#pi^{-})_{121}  [GeV]; Events/ 0.03 [GeV]");
    piplus1piplus2piminus2_mass->SetTitle("; m(#pi^{+}#pi^{+}#pi^{-})_{122}  [GeV]; Events/ 0.03 [GeV]");
    piplus1piminus1piminus2_mass->SetTitle("; m(#pi^{+}#pi^{-}#pi^{-})_{112}  [GeV]; Events/ 0.03 [GeV]");
    piplus2piminus1piminus2_mass->SetTitle("; m(#pi^{+}#pi^{-}#pi^{-})_{212}  [GeV]; Events/ 0.03 [GeV]");
    */
    
    piplus1proton_mass->SetTitle("; m(#pi^{+} p)_{1}  [GeV]; Events/ 0.03 [GeV]");
    piplus2proton_mass->SetTitle("; m(#pi^{+} p)_{2}  [GeV]; Events/ 0.03 [GeV]");
    piminus1proton_mass->SetTitle("; m(#pi^{-} p)_{1}  [GeV]; Events/ 0.03 [GeV]");
    piminus2proton_mass->SetTitle("; m(#pi^{-} p)_{2}  [GeV]; Events/ 0.03 [GeV]");


    bool toggleCanvas = true;    // Keep toggleCanvas = true, if you want all plots in one canvas

    gROOT->SetBatch(kTRUE);
        
    if(toggleCanvas){

      
      /*
      TCanvas* c1 = new TCanvas("c1","c1",15,10,1200,300);
      c1->Divide(4);
      
      
      c1->cd(1); piplus1piminus1_mass->Draw();
      c1->cd(2); piplus1piminus2_mass->Draw();
      c1->cd(3); piplus2piminus1_mass->Draw();
      c1->cd(4); piplus2piminus2_mass->Draw();

      c1->Print("all_2pi_masses.pdf", "pdf");
      c1->Close();

      
      TCanvas* c2 = new TCanvas("c2","c2",15,10,1200,300);
      c2->Divide(4);
            
      c2->cd(1); piplus1piplus2piminus1_mass->Draw();
      c2->cd(2); piplus1piplus2piminus2_mass->Draw();
      c2->cd(3); piplus1piminus1piminus2_mass->Draw(); 
      c2->cd(4); piplus2piminus1piminus2_mass->Draw();

      c2->Print("all_3pi_masses.pdf", "pdf");
      c2->Close();
      */

      TCanvas* c3 = new TCanvas("c2","c2",15,10,700,500);
      c3->Divide(2,2);
            
      c3->cd(1); piplus1proton_mass->Draw();
      c3->cd(2); piplus2proton_mass->Draw();
      c3->cd(3); piminus1proton_mass->Draw(); 
      c3->cd(4); piminus2proton_mass->Draw();

      c3->Print("all_piproton_masses.pdf", "pdf");
      c3->Close();
    
      
    }
    
    // Place toggleCanvas = false, if you want all plot on a seperate canvas
    if(!toggleCanvas){
      TCanvas* c1 = new TCanvas("c1","c1");
      TCanvas* c2 = new TCanvas("c2","c2");
      TCanvas* c3 = new TCanvas("c3","c3");
      TCanvas* c4 = new TCanvas("c4","c4");
      TCanvas* c5 = new TCanvas("c5","c5");
        
  
      c1->cd(); 
      c2->cd(); 
      c3->cd(); 
      c4->cd(); 
 
    }

    



}
