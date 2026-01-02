
void plot_masses_with_fsroot()
{
    TString FN("/w/halld-scshelf2101/home/ilbelov/DataSpring2017/flattree_2pip2pim_sum_AMO_30274_31057.root");
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

 
    TH1F *pipluspiminus_mass = FSHistogram::getTH1F(FN,TN,"Piplus1Piminus1_M+Piplus1Piminus2_M+Piplus2Piminus1_M+Piplus2Piminus2_M","(100,0.,3.)","");
    TH1F *pipipi_mass = FSHistogram::getTH1F(FN,TN,"Piplus1Piplus2Piminus1_M+Piplus1Piplus2Piminus2_M+Piplus1Piminus1Piminus2_M+Piplus2Piminus1Piminus2_M","(100,0.2,3.2)","");
    TH1F *piplusproton_mass = FSHistogram::getTH1F(FN,TN,"Piplus1Proton_M+Piplus2Proton_M","(100,0.8,3.8)","");    
    TH1F *piminusproton_mass = FSHistogram::getTH1F(FN,TN,"Piminus1Proton_M+Piminus2Proton_M","(100,0.8,3.8)","");

    
    
    FSHistogram::executeRDataFrame();

    // Place a title for each histogram


    pipluspiminus_mass->SetTitle("; m(#pi^{+}#pi^{-})_{tot}  [GeV]; Events/ 0.03 [GeV]");
    pipipi_mass->SetTitle("; m(#pi#pi#pi)^{#pm}_{tot}  [GeV]; Events/ 0.03 [GeV]");
    piplusproton_mass->SetTitle("; m(#pi^{+} p)_{tot}  [GeV]; Events/ 0.03 [GeV]");
    piminusproton_mass->SetTitle("; m(#pi^{-} p)_{tot}  [GeV]; Events/ 0.03 [GeV]");


    
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
            
      c3->cd(1); piplusproton_mass->Draw();
      c3->cd(2); piminusproton_mass->Draw();
      c3->cd(3); pipipi_mass->Draw(); 
      c3->cd(4); pipluspiminus_mass->Draw();

      c3->Print("masses_summed.pdf", "pdf");
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
