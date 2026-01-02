

void defstyle(TH1D *histo)
{

  //    histo->GetYaxis()->SetTitle("Entries in 100 bins");
  
    histo->SetMarkerSize(0.5);
    histo->SetMarkerStyle(20);
    histo->SetTitleOffset(0.9,"X");
    histo->SetTitleSize(0.05,"X");
    histo->SetTitleOffset(1.2,"Y");
    histo->SetTitleSize(0.04,"Y"); 
    histo->SetLineColor(kBlack);
    histo->SetLineWidth(2);     

    
 }




     
void plot_piminus_combinatorics_a()
{

  gROOT->SetBatch(kTRUE);



   const char *filename = "histos_data_spring2017_deltacut_ab_piminuscut_a.root";

   
   TFile *file = TFile::Open(filename,"READ");

   if (!file) return;
 

    TH1D *piplusLpiminus1_mass = (TH1D*)file->TFile::Get("PiplusLPiminus1_M");
    TH1D *piplusLpiminus2_mass = (TH1D*)file->TFile::Get("PiplusLPiminus2_M");
    TH1D *piplusUpiminus1_mass = (TH1D*)file->TFile::Get("PiplusUPiminus1_M");
    TH1D *piplusUpiminus2_mass = (TH1D*)file->TFile::Get("PiplusUPiminus2_M");

    TH1D *piplusLpiplusUpiminus1_mass = (TH1D*)file->TFile::Get("PiplusLPiplusUPiminus1_M"); 
    TH1D *piplusLpiplusUpiminus2_mass = (TH1D*)file->TFile::Get("PiplusLPiplusUPiminus2_M");
    TH1D *piplusLpiminus1piminus2_mass = (TH1D*)file->TFile::Get("PiplusLPiminus1Piminus2_M");
    TH1D *piplusUpiminus1piminus2_mass = (TH1D*)file->TFile::Get("PiplusUPiminus1Piminus2_M");    
    
    TH1D *piplusLproton_mass = (TH1D*)file->TFile::Get("PiplusLProton_M");
    TH1D *piplusUproton_mass = (TH1D*)file->TFile::Get("PiplusUProton_M");
    TH1D *piminus1proton_mass = (TH1D*)file->TFile::Get("Piminus1Proton_M");
    TH1D *piminus2proton_mass = (TH1D*)file->TFile::Get("Piminus2Proton_M");

    TH1D *protonpiplusLpiminus1_mass = (TH1D*)file->TFile::Get("ProtonPiplusLPiminus1_M");
    TH1D *protonpiplusLpiminus2_mass = (TH1D*)file->TFile::Get("ProtonPiplusLPiminus2_M");
    TH1D *protonpiplusUpiminus1_mass = (TH1D*)file->TFile::Get("ProtonPiplusUPiminus1_M");
    TH1D *protonpiplusUpiminus2_mass = (TH1D*)file->TFile::Get("ProtonPiplusUPiminus2_M");

    
    TH1D *protonpiplusLpiplusUpiminus1_mass = (TH1D*)file->TFile::Get("ProtonPiplusLPiplusUPiminus1_M"); 
    TH1D *protonpiplusLpiplusUpiminus2_mass = (TH1D*)file->TFile::Get("ProtonPiplusLPiplusUPiminus2_M");
    TH1D *protonpiplusLpiminus1piminus2_mass = (TH1D*)file->TFile::Get("ProtonPiplusLPiminus1Piminus2_M");
    TH1D *protonpiplusUpiminus1piminus2_mass = (TH1D*)file->TFile::Get("ProtonPiplusUPiminus1Piminus2_M");    
 
    
        
    TH1D *minust = (TH1D*)file->TFile::Get("minustvar");
    
    TH1D *piplusL_p = (TH1D*)file->TFile::Get("PiplusL_P");
    TH1D *piplusU_p = (TH1D*)file->TFile::Get("PiplusU_P");
    TH1D *piplusL_pz = (TH1D*)file->TFile::Get("PiplusL_Pz");
    TH1D *piplusU_pz = (TH1D*)file->TFile::Get("PiplusU_Pz");
    TH1D *piminus1_p = (TH1D*)file->TFile::Get("Piminus1_P");
    TH1D *piminus2_p = (TH1D*)file->TFile::Get("Piminus2_P");
    TH1D *piminus1_pz = (TH1D*)file->TFile::Get("Piminus1_Pz");
    TH1D *piminus2_pz = (TH1D*)file->TFile::Get("Piminus2_Pz");

    
    TH1D *deltaphi = (TH1D*)file->TFile::Get("DeltaPhi_LV_UV");
    TH1D *deltaphi_wrong = (TH1D*)file->TFile::Get("DeltaPhi_LV_UV_wrong");
    TH1D *deltatheta = (TH1D*)file->TFile::Get("DeltaTheta_LV_UV");
    TH1D *deltatheta_wrong = (TH1D*)file->TFile::Get("DeltaTheta_LV_UV_wrong");
    
    
    

      gStyle->SetOptStat(0);
      
      defstyle(piplusUpiminus1_mass); piplusUpiminus1_mass->SetMarkerColor(kRed);
      defstyle(piplusUpiminus2_mass); piplusUpiminus2_mass->SetMarkerColor(kBlue);      

      defstyle(piplusLpiplusUpiminus1_mass); piplusLpiplusUpiminus1_mass->SetMarkerColor(kRed);
      defstyle(piplusLpiplusUpiminus2_mass); piplusLpiplusUpiminus2_mass->SetMarkerColor(kBlue);
      defstyle(piplusLpiminus1piminus2_mass); piplusLpiminus1piminus2_mass->SetMarkerColor(kGray);
      defstyle(piplusUpiminus1piminus2_mass); piplusUpiminus1piminus2_mass->SetMarkerColor(kBlack);

      defstyle(piplusLproton_mass); piplusLproton_mass->SetMarkerColor(kBlack);
      defstyle(piplusUproton_mass); piplusUproton_mass->SetMarkerColor(kGray);
      
      defstyle(piminus1proton_mass); piminus1proton_mass->SetMarkerColor(kRed);
      defstyle(piminus2proton_mass); piminus2proton_mass->SetMarkerColor(kBlue);


      defstyle(protonpiplusLpiminus1_mass); protonpiplusLpiminus1_mass->SetMarkerColor(kRed);
      defstyle(protonpiplusLpiminus2_mass); protonpiplusLpiminus2_mass->SetMarkerColor(kBlue);
      defstyle(protonpiplusUpiminus1_mass); protonpiplusUpiminus1_mass->SetMarkerColor(kRed+2);
      defstyle(protonpiplusUpiminus2_mass); protonpiplusUpiminus2_mass->SetMarkerColor(kBlue+2);      

      defstyle(protonpiplusLpiplusUpiminus1_mass); protonpiplusLpiplusUpiminus1_mass->SetMarkerColor(kRed);
      defstyle(protonpiplusLpiplusUpiminus2_mass); protonpiplusLpiplusUpiminus2_mass->SetMarkerColor(kBlue);

      defstyle(protonpiplusLpiminus1piminus2_mass); protonpiplusLpiminus1piminus2_mass->SetMarkerColor(kBlack);
      defstyle(protonpiplusUpiminus1piminus2_mass); protonpiplusUpiminus1piminus2_mass->SetMarkerColor(kGray);


      
      defstyle(minust); minust->SetMarkerColor(kBlack);

      
      defstyle(piplusL_p); piplusL_p->SetMarkerColor(kBlack);
      defstyle(piplusU_p); piplusU_p->SetMarkerColor(kGreen);
      defstyle(piplusL_pz); piplusL_pz->SetMarkerColor(kBlack);
      defstyle(piplusU_pz); piplusU_pz->SetMarkerColor(kGreen);

      defstyle(piminus1_p); piminus1_p->SetMarkerColor(kRed);
      defstyle(piminus2_p); piminus2_p->SetMarkerColor(kBlue);
      defstyle(piminus1_pz); piminus1_pz->SetMarkerColor(kRed);
      defstyle(piminus2_pz); piminus2_pz->SetMarkerColor(kBlue);
 
      
      defstyle(deltaphi); deltaphi->SetMarkerColor(kBlack);
      defstyle(deltaphi_wrong); deltaphi_wrong->SetMarkerColor(kGreen);
      defstyle(deltatheta); deltatheta->SetMarkerColor(kBlack);
      defstyle(deltatheta_wrong); deltatheta_wrong->SetMarkerColor(kGreen);

      
      
      piplusUpiminus2_mass->GetXaxis()->SetTitle("m(#pi^{+}(uv) #pi^{-}(b))  [GeV]");
      piplusLpiplusUpiminus2_mass->GetXaxis()->SetTitle("m(#pi^{+}#pi^{+}#pi^{-})  [GeV]");
      piplusUpiminus1piminus2_mass->GetXaxis()->SetTitle("m(#pi^{+}(uv) #pi^{-}#pi^{-})  [GeV]");
      piplusLproton_mass->GetXaxis()->SetTitle("m(p#pi^{+}(lv))  [GeV]");
      piminus2proton_mass->GetXaxis()->SetTitle("m(p#pi^{-})  [GeV]");
      protonpiplusUpiminus1_mass->GetXaxis()->SetTitle("m(p#pi^{+}#pi^{-})  [GeV]");
      protonpiplusLpiplusUpiminus1_mass->GetXaxis()->SetTitle("m(p#pi^{+}#pi^{+}#pi^{-})  [GeV]");
      protonpiplusUpiminus1piminus2_mass->GetXaxis()->SetTitle("m(p#pi^{+}#pi^{-}#pi^{-})  [GeV]");

  

      minust->GetXaxis()->SetTitle("-Q^{2}  [GeV^{2}]");
      piplusL_p->GetXaxis()->SetTitle("|P(#pi^{+})| [GeV]");
      piplusL_pz->GetXaxis()->SetTitle("P_{z}(#pi^{+}) [GeV]");
      piminus2_p->GetXaxis()->SetTitle("|P(#pi^{-})| [GeV]");
      piminus2_pz->GetXaxis()->SetTitle("P_{z}(#pi^{-}) [GeV]");


      protonpiplusUpiminus1_mass->SetMaximum(4000.); //60000.
      piminus2_p->SetMaximum(4000.);//50000.
      

      TLegend *leg = new TLegend(0.1,0.3,0.8,0.7);
      leg->AddEntry(piminus1_p, "Combinations with #pi^{-}_{a}  ", "p");
      leg->AddEntry(piminus2_p, "Combinations with #pi^{-}_{b}  ", "p");

      
      TLegend* leg5 = new TLegend(0.35,0.68,0.55,0.89);
      leg5->AddEntry(protonpiplusLpiminus1_mass, "#pi^{+}(lv) #pi^{-}(a)  ", "p");
      leg5->AddEntry(protonpiplusUpiminus1_mass, "#pi^{+}(uv) #pi^{-}(a)  ", "p");
      leg5->AddEntry(protonpiplusLpiminus2_mass, "#pi^{+}(lv) #pi^{-}(b)  ", "p");
      leg5->AddEntry(protonpiplusUpiminus2_mass, "#pi^{+}(uv) #pi^{-}(b)  ", "p");

      
      TLegend *leg8 = new TLegend(0.2,0.65,0.39,0.75);
      leg8->AddEntry(protonpiplusLpiminus1piminus2_mass, "#pi^{+}(lv)  ", "p");
      leg8->AddEntry(protonpiplusUpiminus1piminus2_mass, "#pi^{+}(uv)  ", "p");


      
      TCanvas* canva = new TCanvas("c2","c2",15,10,1200,750);
      canva->Divide(4,3);
      canva->cd(1); 
        piplusUpiminus2_mass->Draw("P");
	//	piplusUpiminus1_mass->Draw("P SAME");
	canva->cd(2);
        piplusUpiminus1piminus2_mass->Draw("P");
	canva->cd(3); 
        piplusLpiplusUpiminus2_mass->Draw("P");
        piplusLpiplusUpiminus1_mass->Draw("P SAME");
	canva->cd(4);
	piminus1proton_mass->Draw("P");
        piminus2proton_mass->Draw("P SAME");
	canva->cd(5);
        protonpiplusUpiminus1_mass->Draw("P");
	protonpiplusLpiminus1_mass->Draw("P SAME");
	protonpiplusLpiminus2_mass->Draw("P SAME");
	protonpiplusUpiminus2_mass->Draw("P SAME");
	leg5->Draw("SAME");
	canva->cd(6);
	piplusLproton_mass->Draw("P");
	canva->cd(7);
	protonpiplusLpiplusUpiminus1_mass->Draw("P");
        protonpiplusLpiplusUpiminus2_mass->Draw("P SAME");	
	canva->cd(8);
	protonpiplusUpiminus1piminus2_mass->Draw("P");
	protonpiplusLpiminus1piminus2_mass->Draw("P SAME");
	leg8->Draw("SAME");
	canva->cd(9);
	piminus2_p->Draw("P");
	piminus1_p->Draw("P SAME");
	canva->cd(10);
	leg->Draw("");

	
      canva->Print("piminus_combinatorics.pdf", "pdf");
      canva->Close();
      


    file->Close();
    
}
