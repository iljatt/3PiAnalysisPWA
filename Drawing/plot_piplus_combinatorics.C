

void defstyle(TH1D *histo)
{

  //    histo->GetYaxis()->SetTitle("Entries in 100 bins");
  
    histo->SetMarkerSize(0.5);
    histo->SetMarkerStyle(20);
    histo->SetTitleOffset(0.9,"X");
    histo->SetTitleSize(0.06,"X");
    histo->SetTitleOffset(1.2,"Y");
    histo->SetTitleSize(0.04,"Y"); 
    histo->SetLineColor(kBlack);
    histo->SetLineWidth(2);     

    
 }




     
void plot_piplus_combinatorics()
{

  gROOT->SetBatch(kTRUE);


  //  const char *filename = "histos_data_spring2017_precuts_diffpipluscut_deltacut.root";
  const char *filename = "histos_data_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut1p0.root";

   
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
 
    TH1D *fourpion_mass = (TH1D*)file->TFile::Get("FourPions_M");
    
        
    TH1D *minust = (TH1D*)file->TFile::Get("minustvar");
    
    TH1D *piplusL_p = (TH1D*)file->TFile::Get("PiplusL_P");
    TH1D *piplusU_p = (TH1D*)file->TFile::Get("PiplusU_P");
    TH1D *piplusL_pz = (TH1D*)file->TFile::Get("PiplusL_Pz");
    TH1D *piplusU_pz = (TH1D*)file->TFile::Get("PiplusU_Pz");

    TH1D *deltaphi = (TH1D*)file->TFile::Get("DeltaPhi_LV_UV");
    TH1D *deltaphi_wrong = (TH1D*)file->TFile::Get("DeltaPhi_LV_UV_wrong");
    TH1D *deltatheta = (TH1D*)file->TFile::Get("DeltaTheta_LV_UV");
    TH1D *deltatheta_wrong = (TH1D*)file->TFile::Get("DeltaTheta_LV_UV_wrong");
    

      //SUMMING OVER THE TWO PI'S SINCE (THEY ARE EITHER NDISTINGUISHABLE OR SORTED BY MOMENTUM NAGNITUDE)
      piplusLpiminus1_mass->Add(piplusLpiminus2_mass);
      piplusUpiminus1_mass->Add(piplusUpiminus2_mass);
      protonpiplusLpiminus1_mass->Add(protonpiplusLpiminus2_mass);
      protonpiplusUpiminus1_mass->Add(protonpiplusUpiminus2_mass);
      piplusLpiplusUpiminus1_mass->Add(piplusLpiplusUpiminus2_mass);
      protonpiplusLpiplusUpiminus1_mass->Add(protonpiplusLpiplusUpiminus2_mass);
      piminus1proton_mass->Add(piminus2proton_mass);      

      
    
      //DRAWING
      gStyle->SetOptStat(0);
      
      defstyle(piplusLpiminus1_mass); piplusLpiminus1_mass->SetMarkerColor(kGray); 
      defstyle(piplusUpiminus1_mass); piplusUpiminus1_mass->SetMarkerColor(kBlack);  

      defstyle(piplusLpiplusUpiminus1_mass); piplusLpiplusUpiminus1_mass->SetMarkerColor(kBlack);


      defstyle(piplusLpiminus1piminus2_mass); piplusLpiminus1piminus2_mass->SetMarkerColor(kGray); 
      defstyle(piplusUpiminus1piminus2_mass); piplusUpiminus1piminus2_mass->SetMarkerColor(kBlack);

      
      defstyle(piplusLproton_mass); piplusLproton_mass->SetMarkerColor(kBlack);
      defstyle(piplusUproton_mass); piplusUproton_mass->SetMarkerColor(kGray);

      defstyle(piminus1proton_mass); piminus1proton_mass->SetMarkerColor(kBlack);


      defstyle(protonpiplusLpiminus1_mass); protonpiplusLpiminus1_mass->SetMarkerColor(kBlack);
      defstyle(protonpiplusUpiminus1_mass); protonpiplusUpiminus1_mass->SetMarkerColor(kGray);

      defstyle(protonpiplusLpiplusUpiminus1_mass); protonpiplusLpiplusUpiminus1_mass->SetMarkerColor(kBlack);

      defstyle(protonpiplusLpiminus1piminus2_mass); protonpiplusLpiminus1piminus2_mass->SetMarkerColor(kBlack);
      defstyle(protonpiplusUpiminus1piminus2_mass); protonpiplusUpiminus1piminus2_mass->SetMarkerColor(kGray);


      defstyle(fourpion_mass); fourpion_mass->SetMarkerColor(kBlack); 
      defstyle(minust); minust->SetMarkerColor(kBlack);

      
      defstyle(piplusL_p); piplusL_p->SetMarkerColor(kBlack);
      defstyle(piplusU_p); piplusU_p->SetMarkerColor(kGray);
      defstyle(piplusL_pz); piplusL_pz->SetMarkerColor(kBlack);
      defstyle(piplusU_pz); piplusU_pz->SetMarkerColor(kGray);

      defstyle(deltaphi); deltaphi->SetMarkerColor(kBlack);
      defstyle(deltaphi_wrong); deltaphi_wrong->SetMarkerColor(kGray);
      defstyle(deltatheta); deltatheta->SetMarkerColor(kBlack);
      defstyle(deltatheta_wrong); deltatheta_wrong->SetMarkerColor(kGray);

      

      
      piplusUpiminus1_mass->GetXaxis()->SetTitle("m(#pi^{+}(uv) #pi^{-})  [GeV]");
      piplusLpiplusUpiminus1_mass->GetXaxis()->SetTitle("m(#pi^{+}#pi^{+}#pi^{-})  [GeV]");
      piplusUpiminus1piminus2_mass->GetXaxis()->SetTitle("m(#pi^{+}(uv) #pi^{-}#pi^{-})  [GeV]"); 
      piplusLproton_mass->GetXaxis()->SetTitle("m(p#pi^{+}(lv))  [GeV]");
      piminus1proton_mass->GetXaxis()->SetTitle("m(p#pi^{-})  [GeV]");
      protonpiplusUpiminus1_mass->GetXaxis()->SetTitle("m(p#pi^{+}#pi^{-})  [GeV]");
      protonpiplusLpiplusUpiminus1_mass->GetXaxis()->SetTitle("m(p#pi^{+}#pi^{+}#pi^{-})  [GeV]");
      protonpiplusUpiminus1piminus2_mass->GetXaxis()->SetTitle("m(p#pi^{+}#pi^{-}#pi^{-})  [GeV]");

      
      fourpion_mass->GetXaxis()->SetTitle("m(#pi^{+}#pi^{+}#pi^{-}#pi^{-})  [GeV]");
      minust->GetXaxis()->SetTitle("-Q^{2}  [GeV^{2}]");
      piplusL_p->GetXaxis()->SetTitle("|P(#pi^{+})| [GeV]");
      piplusL_pz->GetXaxis()->SetTitle("P_{z}(#pi^{+}) [GeV]");
      deltaphi->GetXaxis()->SetTitle("#Delta#phi(uv-lv) [deg]");
      deltatheta->GetXaxis()->SetTitle("#Delta#theta(uv-lv) [deg]");


      
         TLegend* leg5 = new TLegend(0.35,0.75,0.54,0.85);
         leg5->AddEntry(protonpiplusLpiminus1_mass, "#pi^{+}(lv)  ", "p");
         leg5->AddEntry(protonpiplusUpiminus1_mass, "#pi^{+}(uv)  ", "p");

         TLegend* leg8 = new TLegend(0.2,0.65,0.39,0.75);
         leg8->AddEntry(protonpiplusLpiminus1piminus2_mass, "#pi^{+}(lv)  ", "p");
         leg8->AddEntry(protonpiplusUpiminus1piminus2_mass, "#pi^{+}(uv)  ", "p");

	 TLegend* leg12 = new TLegend(0.65,0.65,0.84,0.75); 
         leg12->AddEntry(piplusL_p, "#pi^{+}(lv)  ", "p");
         leg12->AddEntry(piplusU_p, "#pi^{+}(uv)  ", "p");

      

      
      TCanvas* canva = new TCanvas("c2","c2",15,10,1200,750);
      canva->Divide(4,3);

        canva->cd(1); 
	gPad->SetBottomMargin(0.12);
        piplusUpiminus1_mass->Draw("P");
	//        piplusLpiminus1_mass->Draw("P SAME");	
	canva->cd(2);
	gPad->SetBottomMargin(0.12);
        piplusUpiminus1piminus2_mass->Draw("P");
	//        piplusLpiminus1piminus2_mass->Draw("P SAME");
	canva->cd(3); 
	gPad->SetBottomMargin(0.12);
        piplusLpiplusUpiminus1_mass->Draw("P");
	canva->cd(4);
	gPad->SetBottomMargin(0.12);
        protonpiplusLpiplusUpiminus1_mass->Draw("P");
	canva->cd(5);
	gPad->SetBottomMargin(0.12);
        protonpiplusUpiminus1_mass->Draw("P");
	protonpiplusLpiminus1_mass->Draw("P SAME");
	leg5->Draw("SAME");
	canva->cd(6); 
	gPad->SetBottomMargin(0.12);
        piplusLproton_mass->Draw("P");
	//        piplusUproton_mass->Draw("P SAME");
	canva->cd(7); 
	gPad->SetBottomMargin(0.12);
        piminus1proton_mass->Draw("P");
	canva->cd(8);
	gPad->SetBottomMargin(0.12);
        protonpiplusUpiminus1piminus2_mass->Draw("P");
        protonpiplusLpiminus1piminus2_mass->Draw("P SAME");
	leg8->Draw("SAME");
	canva->cd(9);
	gPad->SetBottomMargin(0.12);
	fourpion_mass->Draw("P");
	canva->cd(10);
	gPad->SetBottomMargin(0.12);
	minust->Draw("P");
	canva->cd(11);
	gPad->SetBottomMargin(0.12);
	deltatheta->Draw("P");
	canva->cd(12);
	gPad->SetBottomMargin(0.12);
	piplusL_p->Draw("P");
	piplusU_p->Draw("P SAME");
	leg12->Draw("SAME");


	
      canva->Print("piplus_combinatorics.pdf", "pdf");
      canva->Close();
      


    file->Close();
    
}
