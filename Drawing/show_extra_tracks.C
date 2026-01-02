#include <cstdlib>
#include <cstring>


void set_captions(TH2D *histo, const char *title)
{
    histo->SetTitleOffset(0.9,"X");
    histo->SetTitleSize(0.05,"X");
    histo->SetTitleOffset(0.9,"Y");
    histo->SetTitleSize(0.05,"Y");     

    histo->GetXaxis()->SetTitle("#theta [deg]");
    histo->GetYaxis()->SetTitle("#phi [deg]");
    histo->SetTitle(Form("%s",title));
 

}






void show_extra_tracks()
{

  const Int_t NN = 2;
    
   gROOT->SetBatch(kTRUE);  
   gStyle->SetOptStat(0);
   
   TFile *file = TFile::Open("correlations_data_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut.root","READ");
   if (!file) return;


   const char *range[NN] = {"0 unused tracks","1 unused track"};

  
   TH2D *histo_proton[NN], *histo_piplusU[NN], *histo_piplusL[NN], *histo_piminus1[NN], *histo_piminus2[NN];
   TCanvas *canva[NN][5];


  TH3D *h3d_proton =  (TH3D*) file->Get("AnglesProton_vs_NumUnusedTracks");
  TH3D *h3d_piplusU =  (TH3D*) file->Get("AnglesPiplusU_vs_NumUnusedTracks");
  TH3D *h3d_piplusL =  (TH3D*) file->Get("AnglesPiplusL_vs_NumUnusedTracks");
  TH3D *h3d_piminus1 =  (TH3D*) file->Get("AnglesPiminus1_vs_NumUnusedTracks");
  TH3D *h3d_piminus2 =  (TH3D*) file->Get("AnglesPiminus2_vs_NumUnusedTracks");


   
  
  
   for (int jj=0; jj<NN; jj++){

     h3d_proton->GetZaxis()->SetRange(jj+1,jj+1);
     h3d_piplusU->GetZaxis()->SetRange(jj+1,jj+1);
     h3d_piplusL->GetZaxis()->SetRange(jj+1,jj+1);
     h3d_piminus1->GetZaxis()->SetRange(jj+1,jj+1);
     h3d_piminus2->GetZaxis()->SetRange(jj+1,jj+1);

     histo_proton[jj] = (TH2D*) h3d_proton->Project3D("yx");
     histo_piplusU[jj] = (TH2D*) h3d_piplusU->Project3D("yx");
     histo_piplusL[jj] = (TH2D*) h3d_piplusL->Project3D("yx");
     histo_piminus1[jj] = (TH2D*) h3d_piminus1->Project3D("yx");
     histo_piminus2[jj] = (TH2D*) h3d_piminus2->Project3D("yx");

     
     cout << "Z-axis: bin center value = \t" << h3d_proton->GetZaxis()->GetBinCenter(jj+1) << endl;


     
     
     set_captions(histo_proton[jj],range[jj]);
     set_captions(histo_piplusU[jj],range[jj]);
     set_captions(histo_piplusL[jj],range[jj]);
     set_captions(histo_piminus1[jj],range[jj]);
     set_captions(histo_piminus2[jj],range[jj]);

          
    
    canva[jj][0] = new TCanvas(Form("c0_bin%d",jj),Form("c0_bin%d",jj),15,10,480,300);
    //    gPad->SetLogx();
    histo_proton[jj]->Draw("COLZ");
    canva[jj][0]->Print(Form("UnusedTracks/angles_proton_%d_extra_tracks.pdf",jj), "pdf");
    canva[jj][0]->Close();

    canva[jj][1] = new TCanvas(Form("c1_bin%d",jj),Form("c1_bin%d",jj),15,10,480,300);
    //    gPad->SetLogx();
    histo_piplusU[jj]->Draw("COLZ");
    canva[jj][1]->Print(Form("UnusedTracks/angles_piplusU_%d_extra_tracks.pdf",jj), "pdf");
    canva[jj][1]->Close();

    canva[jj][2] = new TCanvas(Form("c2_bin%d",jj),Form("c2_bin%d",jj),15,10,480,300);
    //    gPad->SetLogx();
    histo_piplusL[jj]->Draw("COLZ");
    canva[jj][2]->Print(Form("UnusedTracks/angles_piplusL_%d_extra_tracks.pdf",jj), "pdf");
    canva[jj][2]->Close();

    canva[jj][3] = new TCanvas(Form("c3_bin%d",jj),Form("c3_bin%d",jj),15,10,480,300);
    //    gPad->SetLogx();
    histo_piminus1[jj]->Draw("COLZ");
    canva[jj][3]->Print(Form("UnusedTracks/angles_piminus1_%d_extra_tracks.pdf",jj), "pdf");
    canva[jj][3]->Close();

    canva[jj][4] = new TCanvas(Form("c4_bin%d",jj),Form("c4_bin%d",jj),15,10,480,300);
    //   gPad->SetLogx();
    histo_piminus2[jj]->Draw("COLZ");
    canva[jj][4]->Print(Form("UnusedTracks/angles_piminus2_%d_extra_tracks.pdf",jj), "pdf");
    canva[jj][4]->Close();

    
  
  }

      

   file->Close();
    
}
