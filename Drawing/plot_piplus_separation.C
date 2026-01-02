

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





void plot_piplus_separation()
{

  gROOT->SetBatch(kTRUE);


   const char *filename = "histos_thrown_reco_spring2017_precuts_deltacut__pipluscut.root";
  
   
   TFile *file = TFile::Open(filename,"READ");
   if (!file) return;
 
    TH1D *piplus1_mag = (TH1D*)file->TFile::Get("Mag_piplus1");
    TH1D *piplusU_mag = (TH1D*)file->TFile::Get("Mag_piplusU");
    TH1D *piplus2_mag = (TH1D*)file->TFile::Get("Mag_piplus2");
    TH1D *piplusL_mag = (TH1D*)file->TFile::Get("Mag_piplusL");

    TH1D *piplus1_theta = (TH1D*)file->TFile::Get("Theta_piplus1");
    TH1D *piplusU_theta = (TH1D*)file->TFile::Get("Theta_piplusU");
    TH1D *piplus2_theta = (TH1D*)file->TFile::Get("Theta_piplus2");
    TH1D *piplusL_theta = (TH1D*)file->TFile::Get("Theta_piplusL");
    
    
    gStyle->SetOptStat(0);

    defstyle(piplus1_mag); piplus1_mag->SetLineColor(kRed);  piplus1_mag->SetFillStyle(3004); piplus1_mag->SetFillColor(kRed); 
    defstyle(piplusU_mag); piplusU_mag->SetLineColor(kCyan); piplusU_mag->SetFillStyle(3005); piplusU_mag->SetFillColor(kCyan);
    defstyle(piplus2_mag); piplus2_mag->SetLineColor(kBlue); piplus2_mag->SetFillStyle(3004); piplus2_mag->SetFillColor(kBlue); 
    defstyle(piplusL_mag); piplusL_mag->SetLineColor(kMagenta); piplusL_mag->SetFillStyle(3005); piplusL_mag->SetFillColor(kMagenta);

    defstyle(piplus1_theta); piplus1_theta->SetLineColor(kRed);  piplus1_theta->SetFillStyle(3004); piplus1_theta->SetFillColor(kRed); 
    defstyle(piplusU_theta); piplusU_theta->SetLineColor(kCyan); piplusU_theta->SetFillStyle(3005); piplusU_theta->SetFillColor(kCyan);
    defstyle(piplus2_theta); piplus2_theta->SetLineColor(kBlue); piplus2_theta->SetFillStyle(3004); piplus2_theta->SetFillColor(kBlue); 
    defstyle(piplusL_theta); piplusL_theta->SetLineColor(kMagenta); piplusL_theta->SetFillStyle(3005); piplusL_theta->SetFillColor(kMagenta);

      

    piplus2_mag->GetXaxis()->SetTitle("|#bar{p}|  [GeV]");
    piplus1_theta->GetXaxis()->SetTitle("#theta  [deg]");
    piplus1_theta->GetXaxis()->SetRangeUser(0.,150.);

    
    TLegend *leg1 = new TLegend(0.7,0.6,0.9,0.9); //0.74, 0.75
    TLegend *leg2 = new TLegend(0.7,0.6,0.9,0.9); //0.74, 0.75

    
    leg1->AddEntry(piplusU_mag,"  #pi^{+}_{U},    MC True ","f");    
    leg1->AddEntry(piplus1_mag,"  #pi^{+}_{fast}, MC Reco ","f");
    leg1->AddEntry(piplusL_mag,"  #pi^{+}_{L},    MC True ","f");
    leg1->AddEntry(piplus2_mag,"  #pi^{+}_{slow}, MC Reco ","f");

    leg2->AddEntry(piplusU_theta,"  #pi^{+}_{U},    MC True ","f");    
    leg2->AddEntry(piplus1_theta,"  #pi^{+}_{fast}, MC Reco ","f");
    leg2->AddEntry(piplusL_theta,"  #pi^{+}_{L},    MC True ","f");
    leg2->AddEntry(piplus2_theta,"  #pi^{+}_{slow}, MC Reco ","f");


    
      
      TCanvas* canva1 = new TCanvas("c1","c1",15,10,400,300);
      piplus2_mag->Draw("HIST");
      piplusL_mag->Draw("HIST SAME");
      piplus1_mag->Draw("HIST SAME");
      piplusU_mag->Draw("HIST SAME");
      leg1->Draw("SAME");
      canva1->Print("piplus_sep_mag.pdf", "pdf");
      canva1->Close();

      
      TCanvas* canva2 = new TCanvas("c2","c2",15,10,400,300);      
      piplus1_theta->Draw("HIST");
      piplusU_theta->Draw("HIST SAME");
      piplus2_theta->Draw("HIST SAME");
      piplusL_theta->Draw("HIST SAME");
      leg2->Draw("SAME");
      canva2->Print("piplus_sep_theta.pdf", "pdf");
      canva2->Close();

      file->Close();
    
}
