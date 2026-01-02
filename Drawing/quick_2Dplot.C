#include <cstdlib>
#include <cstring>


void defstyle(TH2D *histo)
{

    histo->SetMarkerSize(0.5);
    histo->SetMarkerStyle(20);
    histo->SetTitleOffset(1.0,"X");
    histo->SetTitleSize(0.06,"X");
    histo->SetTitleOffset(0.9,"Y");
    histo->SetTitleSize(0.05,"Y"); 
    histo->SetLineColor(kBlack);
    histo->SetLineWidth(2);    

    
 }





void quick_2Dplot(const char *filename, const char *var)
{
  std::string pdfname = var;
  pdfname +=".pdf";
  
  //vanHoveXY_for_ppipLX"
   gROOT->SetBatch(kTRUE);  
   
   TFile *file = TFile::Open(filename,"READ");
   if (!file) return;
 
   TH2D *histo = (TH2D*)file->TFile::Get(var);

   histo->SetTitle("");
   histo->GetXaxis()->SetTitle("");
   histo->GetYaxis()->SetTitle("N(unique particles) per event");

   
   histo->GetXaxis()->SetBinLabel(1,"#gamma_{beam}");
   histo->GetXaxis()->SetBinLabel(2,"p");
   histo->GetXaxis()->SetBinLabel(3,"#pi^{+}_{fast}");
   histo->GetXaxis()->SetBinLabel(4,"#pi^{+}_{slow}");
   histo->GetXaxis()->SetBinLabel(5,"#pi^{-}_{fast}");
   histo->GetXaxis()->SetBinLabel(6,"#pi^{-}_{slow}");

   defstyle(histo);
   
    
    /*    
    defstyle(piplusL_theta); piplusL_theta->SetLineColor(kMagenta); piplusL_theta->SetFillStyle(3005); piplusL_theta->SetFillColor(kMagenta);
    piplus1_theta->GetXaxis()->SetRangeUser(0.,150.);

    TLegend *leg1 = new TLegend(0.7,0.6,0.9,0.9); //0.74, 0.75
    leg1->AddEntry(piplusU_mag,"  #pi^{+}_{U},    MC True ","f");    
    leg1->AddEntry(piplus2_mag,"  #pi^{+}_{slow}, MC Reco ","f");
    */

   /*    
      double x = 5/2./TMath::Sqrt(3.);
      TLine *line0deg = new TLine(-2.5,0.,2.5,0.);
      TLine *line120deg = new TLine(-x,2.5,x,-2.5);
      TLine *line240deg = new TLine(-x,-2.5,x,2.5);

      TText *sectorI = new TText(1.8,1.5,"B");
      sectorI->SetTextAlign(21); sectorI->SetTextSize(0.1);
      TText *sectorII = new TText(0,1.5,"M");
      sectorII->SetTextAlign(21); sectorII->SetTextSize(0.1);
      TText *sectorIV = new TText(-1.8,-1.5,"B");
      sectorIV->SetTextAlign(21); sectorIV->SetTextSize(0.1);
      TText *sectorV = new TText(0,-1.5,"M");
      sectorV->SetTextAlign(21); sectorV->SetTextSize(0.1);
   */   

      
      gStyle->SetOptStat(0);
      gStyle->SetTextSize(0.05);

      
      TCanvas* canva = new TCanvas("c1","c1",15,10,400,300);
      //      gPad->SetBottomMargin(0.26);   
      //      gPad->SetLeftMargin(0.08);
      gPad->SetRightMargin(0.12);


      histo->Draw("COLZ");
      //      leg1->Draw("SAME");

      /*      
      line0deg->Draw("SAME");
      line120deg->Draw("SAME");
      line240deg->Draw("SAME");

      sectorI->Draw("SAME");
      sectorII->Draw("SAME");
      sectorIV->Draw("SAME");
      sectorV->Draw("SAME");
      */

      
      canva->Print(pdfname.c_str(), "pdf");
      canva->Close();


      file->Close();
    
}
