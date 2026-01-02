#include <cstdlib>
#include <cstring>


void setstyle_captions(TH2D *histo)
{

    histo->SetTitleOffset(0.9,"X");
    histo->SetTitleSize(0.05,"X");
    histo->SetTitleOffset(0.9,"Y");
    histo->SetTitleSize(0.05,"Y"); 

    
 }





void acceptance_plot()
{

  const Int_t NN = 6;
    
   gROOT->SetBatch(kTRUE);  
   gStyle->SetOptStat(0);
   
   TFile *file_thrown = TFile::Open("correlations_thrown_spring2017.root","READ");
   TFile *file_reco = TFile::Open("correlations_mcw_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_comcut0p1.root","READ");
   if (!file_thrown || !file_reco) return;


  char const *trange[NN] = {"0 #leq -t #leq 2 GeV^{2}","0 #leq -t #leq 0.25 GeV^{2}","0.25 GeV^{2} #leq -t #leq 0.5 GeV^{2}","0.5 GeV^{2} #leq -t #leq 0.75 GeV^{2}","0.75 GeV^{2} #leq -t #leq 1 GeV^{2}","1 GeV^{2} #leq -t #leq 2 GeV^{2}"};

  
  TH2D *histo_thrown_GJ[NN], *histo_thrown_HF[NN], *histo_reco_GJ[NN], *histo_reco_HF[NN];
  TCanvas *canva1[NN], *canva2[NN];

  
  
   for (int jj=0; jj<NN; jj++){
     histo_thrown_GJ[jj] = (TH2D*)file_thrown->TFile::Get(Form("AnglesThrownGJ_bin%d",jj));
     histo_thrown_HF[jj] = (TH2D*)file_thrown->TFile::Get(Form("AnglesThrownHF_bin%d",jj));
     histo_reco_GJ[jj] = (TH2D*)file_reco->TFile::Get(Form("AnglesRecoGJ_bin%d",jj));
     histo_reco_HF[jj] = (TH2D*)file_reco->TFile::Get(Form("AnglesRecoHF_bin%d",jj));

     setstyle_captions(histo_reco_GJ[jj]);
     setstyle_captions(histo_reco_HF[jj]);


     histo_reco_GJ[jj]->GetXaxis()->SetTitle("cos #theta (GJ)");
     histo_reco_GJ[jj]->GetYaxis()->SetTitle("#phi (GJ)  [deg]");
     histo_reco_HF[jj]->GetXaxis()->SetTitle("cos #theta (HF)");
     histo_reco_HF[jj]->GetYaxis()->SetTitle("#phi (HF)  [deg]");
     histo_reco_GJ[jj]->SetTitle(Form("%s",trange[jj]));
     histo_reco_HF[jj]->SetTitle(Form("%s",trange[jj]));

     
     histo_reco_GJ[jj]->Divide(histo_thrown_GJ[jj]);
     histo_reco_HF[jj]->Divide(histo_thrown_HF[jj]);
 
     
   }
   

 
    
  for (int jj=0; jj<NN; jj++){
 
      
    canva1[jj] = new TCanvas(Form("c1_bin%d",jj),Form("c1_bin%d",jj),15,10,480,300);
    histo_reco_GJ[jj]->Draw("COLZ");
    canva1[jj]->Print(Form("Acceptances/acceptance_GJ_bin%d.pdf",jj), "pdf");
    canva1[jj]->Close();

      
    canva2[jj] = new TCanvas(Form("c2_bin%d",jj),Form("c2_bin%d",jj),15,10,480,300);
    histo_reco_HF[jj]->Draw("COLZ");
    canva2[jj]->Print(Form("Acceptances/acceptance_HF_bin%d.pdf",jj), "pdf");
    canva2[jj]->Close();

  }

      


      file_thrown->Close();
      file_reco->Close();
    
}
