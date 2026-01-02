

void defstyle(TH1D *histo)
{

    histo->SetMarkerSize(0.5);
    histo->SetMarkerStyle(20);
    histo->SetTitleOffset(0.9,"X");
    histo->SetTitleSize(0.05,"X");
    histo->SetTitleOffset(1.2,"Y");
    histo->SetTitleSize(0.04,"Y"); 
    histo->SetLineColor(kBlack);
    histo->SetLineWidth(2);    

 }





void plot_masses(const char *mode)
{

  gROOT->SetBatch(kTRUE);


   const char *filename0 = "histos_data_spring2017.root";
   const char *filename1 = "histos_data_spring2017_deltacut_1st.root";
   const char *filename2 = "histos_data_spring2017_deltacut_2nd.root";
   const char *filename3 = "histos_data_spring2017_deltacut_both.root";
  
   
   TFile *file = TFile::Open(filename3,"READ");
   if (!file) return;
 

    TH1D *piplus1piminus1_mass = (TH1D*)file->TFile::Get("Piplus1Piminus1_M");
    TH1D *piplus1piminus2_mass = (TH1D*)file->TFile::Get("Piplus1Piminus2_M");
    TH1D *piplus2piminus1_mass = (TH1D*)file->TFile::Get("Piplus2Piminus1_M");
    TH1D *piplus2piminus2_mass = (TH1D*)file->TFile::Get("Piplus2Piminus2_M");

    TH1D *piplus1piplus2piminus1_mass = (TH1D*)file->TFile::Get("Piplus1Piplus2Piminus1_M"); 
    TH1D *piplus1piplus2piminus2_mass = (TH1D*)file->TFile::Get("Piplus1Piplus2Piminus2_M");

    TH1D *piplus1piminus1piminus2_mass = (TH1D*)file->TFile::Get("Piplus1Piminus1Piminus2_M");
    TH1D *piplus2piminus1piminus2_mass = (TH1D*)file->TFile::Get("Piplus2Piminus1Piminus2_M");    
    
    TH1D *piplus1proton_mass = (TH1D*)file->TFile::Get("Piplus1Proton_M");
    TH1D *piplus2proton_mass = (TH1D*)file->TFile::Get("Piplus2Proton_M");


    TH1D *piminus1proton_mass = (TH1D*)file->TFile::Get("Piminus1Proton_M");
    TH1D *piminus2proton_mass = (TH1D*)file->TFile::Get("Piminus2Proton_M");

        
    TH1D *minust1 = (TH1D*)file->TFile::Get("minustvar1");
    TH1D *minust2 = (TH1D*)file->TFile::Get("minustvar2");
    
    
    TH1D *pipluspiminus_mass = (TH1D*)piplus1piminus1_mass->Clone();    
    pipluspiminus_mass->Add(piplus1piminus2_mass);
    pipluspiminus_mass->Add(piplus2piminus1_mass);
    pipluspiminus_mass->Add(piplus2piminus2_mass);
    
    
    TH1D *pipluspipluspiminus_mass = (TH1D*)piplus1piplus2piminus1_mass->Clone();
    pipluspipluspiminus_mass->Add(piplus1piplus2piminus2_mass);

    TH1D *pipluspiminuspiminus_mass = (TH1D*)piplus1piminus1piminus2_mass->Clone();
    pipluspiminuspiminus_mass->Add(piplus2piminus1piminus2_mass);

    
    TH1D *piplusproton_mass = (TH1D*)piplus1proton_mass->Clone(); 
    piplusproton_mass->Add(piplus2proton_mass);

    
    TH1D *piminusproton_mass = (TH1D*)piminus1proton_mass->Clone(); 
    piminusproton_mass->Add(piminus2proton_mass);

    
    TH1D *minust = (TH1D*)minust1->Clone();
    minust->Add(minust2);

    
    
    if (strncmp(mode,"merged",6)==0){

      gStyle->SetOptStat(0);

      defstyle(pipluspiminus_mass);
      defstyle(pipluspipluspiminus_mass);
      defstyle(pipluspiminuspiminus_mass);
      defstyle(piplusproton_mass);
      defstyle(piminusproton_mass);
      defstyle(minust);
      
      pipluspiminus_mass->GetYaxis()->SetTitle("Events/ 0.03 [GeV]");
      pipluspiminus_mass->GetXaxis()->SetTitle("m(#pi^{+}#pi^{-})  [GeV]");
      pipluspipluspiminus_mass->GetYaxis()->SetTitle("Events/ 0.03 [GeV]");
      pipluspipluspiminus_mass->GetXaxis()->SetTitle("m(#pi^{+}#pi^{+}#pi^{-})  [GeV]");
      pipluspiminuspiminus_mass->GetYaxis()->SetTitle("Events/ 0.03 [GeV]");
      pipluspiminuspiminus_mass->GetXaxis()->SetTitle("m(#pi^{+}#pi^{-}#pi^{-})  [GeV]");
      piplusproton_mass->GetYaxis()->SetTitle("Events/ 0.03 [GeV]");
      piplusproton_mass->GetXaxis()->SetTitle("m(#pi^{+} p)  [GeV]");
      piminusproton_mass->GetYaxis()->SetTitle("Events/ 0.002 [GeV]");
      piminusproton_mass->GetXaxis()->SetTitle("m(#pi^{-} p)  [GeV]");
      minust->GetYaxis()->SetTitle("Events/ 0.02 [GeV^{2}]");
      minust->GetXaxis()->SetTitle("-Q^{2}  [GeV^{2}]");


      
      TCanvas* canva = new TCanvas("c2","c2",15,10,1000,500);
      canva->Divide(3,2);
      
      canva->cd(1); pipluspiminus_mass->Draw("P");
      canva->cd(2); pipluspiminuspiminus_mass->Draw("P");
      canva->cd(3); pipluspipluspiminus_mass->Draw("P");
      canva->cd(4); minust->Draw("P");
      canva->cd(5); piplusproton_mass->Draw("P"); 
      canva->cd(6); piminusproton_mass->Draw("P");

      canva->Print("masses_summed.pdf", "pdf");
      canva->Close();

            
    }
    
 
    else if (strncmp(mode,"superimposed",12)==0){

      gStyle->SetOptStat(0);
      
      defstyle(piplus1piminus1_mass); piplus1piminus1_mass->SetMarkerColor(kBlack);
      defstyle(piplus1piminus2_mass); piplus1piminus2_mass->SetMarkerColor(kRed);
      defstyle(piplus2piminus1_mass); piplus2piminus1_mass->SetMarkerColor(kGreen);
      defstyle(piplus2piminus2_mass); piplus2piminus2_mass->SetMarkerColor(kBlue);      

      defstyle(piplus1piplus2piminus1_mass); piplus1piplus2piminus1_mass->SetMarkerColor(kBlue);
      defstyle(piplus1piplus2piminus2_mass); piplus1piplus2piminus2_mass->SetMarkerColor(kRed);
      defstyle(piplus1piminus1piminus2_mass); piplus1piminus1piminus2_mass->SetMarkerColor(kBlack);
      defstyle(piplus2piminus1piminus2_mass); piplus2piminus1piminus2_mass->SetMarkerColor(kGreen);

      defstyle(piplus1proton_mass); piplus1proton_mass->SetMarkerColor(kBlack);
      defstyle(piplus2proton_mass); piplus2proton_mass->SetMarkerColor(kGreen);
      defstyle(piminus1proton_mass); piminus1proton_mass->SetMarkerColor(kBlue);
      defstyle(piminus2proton_mass); piminus2proton_mass->SetMarkerColor(kRed);

      defstyle(minust1); minust1->SetMarkerColor(kBlue);
      defstyle(minust2); minust2->SetMarkerColor(kRed);
      
      
      piplus1piminus1_mass->GetYaxis()->SetTitle("Events/ 0.03 [GeV]");
      piplus1piminus1_mass->GetXaxis()->SetTitle("m(#pi^{+}#pi^{-})  [GeV]");
      piplus1piplus2piminus1_mass->GetYaxis()->SetTitle("Events/ 0.03 [GeV]");
      piplus1piplus2piminus1_mass->GetXaxis()->SetTitle("m(#pi^{+}#pi^{+}#pi^{-})  [GeV]");
      piplus1piminus1piminus2_mass->GetYaxis()->SetTitle("Events/ 0.03 [GeV]");
      piplus1piminus1piminus2_mass->GetXaxis()->SetTitle("m(#pi^{+}#pi^{-}#pi^{-})  [GeV]");      
      piplus1proton_mass->GetYaxis()->SetTitle("Events/ 0.03 [GeV]");
      piplus1proton_mass->GetXaxis()->SetTitle("m(#pi^{+} p)  [GeV]");
      piplus1proton_mass->SetMaximum(250000.);
      piminus1proton_mass->GetYaxis()->SetTitle("Events/ 0.002 [GeV]");
      piminus1proton_mass->GetXaxis()->SetTitle("m(#pi^{-} p)  [GeV]");
      minust1->GetYaxis()->SetTitle("Events/ 0.02 [GeV^{2}]");
      minust1->GetXaxis()->SetTitle("-Q^{2}  [GeV^{2}]");
      minust1->SetMaximum(28000.);
      
      
      TCanvas* canva = new TCanvas("c2","c2",15,10,1000,500);
      canva->Divide(3,2);

      canva->cd(1); 
        piplus1piminus1_mass->Draw("P");
        piplus1piminus2_mass->Draw("P SAME");
        piplus2piminus1_mass->Draw("P SAME");
        piplus2piminus2_mass->Draw("P SAME");
	canva->cd(2);
        piplus1piminus1piminus2_mass->Draw("P");
        piplus2piminus1piminus2_mass->Draw("P SAME");
	canva->cd(3); 
        piplus1piplus2piminus1_mass->Draw("P");
        piplus1piplus2piminus2_mass->Draw("P SAME");
	canva->cd(4);
	minust1->Draw("P");
	minust2->Draw("P SAME");
	canva->cd(5); 
        piplus1proton_mass->Draw("P");
        piplus2proton_mass->Draw("P SAME"); 
	canva->cd(6); 
        piminus1proton_mass->Draw("P");
	piminus2proton_mass->Draw("P SAME");


      canva->Print("masses_superimposed.pdf", "pdf");
      canva->Close();

      
    }


    file->Close();
    
}
