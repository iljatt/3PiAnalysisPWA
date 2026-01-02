#include "RooStats/SPlot.h"
#include "TSPlot.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TChain.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooPolynomial.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "TH1F.h"
#include <TCanvas.h>
#include <map>
#include <string>

using namespace std;
using namespace RooFit;







void setstyle(TH1D *dhisto, TH1D *dhisto2)
{
    
    
    dhisto->SetMarkerSize(0.4);
    dhisto->SetMarkerStyle(20);
    dhisto->SetMarkerColor(kBlack);
    dhisto->SetTitleOffset(1.0,"X");
    dhisto->SetTitleSize(0.05,"X");
    dhisto->SetTitleOffset(1.2,"Y");
    dhisto->SetTitleSize(0.04,"Y"); 
    dhisto->SetLineColor(kBlack);
    dhisto->SetLineWidth(2);    
    //    dhisto->SetFillColor(kBlue-7);
    //    dhisto->SetFillStyle(3016);


    dhisto2->SetMarkerSize(0.4);
    dhisto2->SetMarkerStyle(20);
    dhisto2->SetMarkerColor(kRed+2);
    dhisto2->SetLineColor(kRed+2);
    dhisto2->SetLineWidth(2);    


}





void draw_bin(TH1D *h_ppip_1, TH1D *h_ppip_2, TH1D *h_3pi_1, TH1D *h_3pi_2, TH1D *h_2pi_1, TH1D *h_2pi_2, char const *title, int i)
{

  std::string bin_num = Form("%d",i);
  std::string pdfname = "FitDeltaMass/bin"+bin_num+"_compare.pdf";

  setstyle(h_ppip_1,h_ppip_2);
  setstyle(h_3pi_1,h_3pi_2);
  setstyle(h_2pi_1,h_2pi_2);


  h_ppip_1->SetTitle(Form("%s",title));
  h_ppip_1->GetXaxis()->SetTitle("m(p#pi^{+}(lv))  [GeV]");
  h_ppip_1->GetXaxis()->SetRangeUser(0.8,3.);


  
  h_2pi_1->GetXaxis()->SetTitle("m(#pi^{+}(uv)#pi^{-})  [GeV]");
  h_3pi_1->GetXaxis()->SetTitle("m(#pi^{+}(uv)#pi^{-}#pi^{-})  [GeV]");



  
    TPaveText *ptw = new TPaveText(0.55,0.68,0.7,0.78,"NDC");
    ptw->AddText("#bf{#pi}_{2}(1670), #bf{#pi}_{1}(1600) ?\n");
    ptw->SetTextSize(0.04);
    ptw->SetBorderSize(0);
    ptw->SetFillStyle(0);

    TPaveText *pte = new TPaveText(0.42,0.4,0.52,0.5,"NDC");
    pte->AddText("f_{2}(1270)\n");
    pte->SetTextSize(0.04);
    pte->SetBorderSize(0);
    pte->SetFillStyle(0);

    TPaveText *ptr = new TPaveText(0.45,0.75,0.55,0.85,"NDC");
    ptr->AddText("a_{2}(1320)\n");
    ptr->SetTextSize(0.04);
    ptr->SetBorderSize(0);
    ptr->SetFillStyle(0);

    TPaveText *pts = new TPaveText(0.35,0.75,0.45,0.85,"NDC");
    pts->AddText("#rho(770)\n");
    pts->SetTextSize(0.04);
    pts->SetBorderSize(0);
    pts->SetFillStyle(0);


    
  
  //New canvas to draw everything on // PLOTTING STARTS HERE ////////////////////////////////////////////
  TCanvas *canva = new TCanvas(Form("c_%s",title),Form("c_%s",title),15,10,400,1100);
  canva->Divide(1,3);

  canva->cd(1);
  gPad->SetBottomMargin(0.11);
  h_ppip_1->Draw("*H");
  h_ppip_2->Draw("*H SAME");
  gPad->Update();
  TLine *lrange = new TLine(1.15,0.,1.15,gPad->GetUymax());
  TLine *rrange = new TLine(1.35,0.,1.35,gPad->GetUymax());
  lrange->SetLineColor(kRed); lrange->SetLineWidth(3); lrange->SetLineStyle(kDashed);
  rrange->SetLineColor(kRed); rrange->SetLineWidth(3); rrange->SetLineStyle(kDashed);
  lrange->Draw("SAME");
  rrange->Draw("SAME");
  canva->Update();  



  
  
  canva->cd(2);
  gPad->SetBottomMargin(0.11);
  h_3pi_1->Draw("*H");
  h_3pi_2->Draw("*H SAME");
  if (i==1) ptr->Draw("SAME");
  if (i==2) ptw->Draw("SAME");
  canva->Update();

  canva->cd(3);
  gPad->SetBottomMargin(0.11);
  h_2pi_1->Draw("*H");
  h_2pi_2->Draw("*H SAME");
  if (i==1) pts->Draw("SAME");
  if (i==2) pte->Draw("SAME");
  canva->Update();


  
  canva->SaveAs(pdfname.c_str(),"pdf");
  canva->Close();


}







void superimpose_tbins()
{

  //Import the files with histograms made out of our data
  const char *datafilename01 = "correlations_data_spring2017_precuts_diffpipluscut_fourpionscut_piminuscut.root";
  const char *datafilename02 = "correlations_data_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut.root";

  const char *datafilename1 = "correlations_data_spring2017_chi2_precuts_diffpipluscut_fourpionscut_piminuscut.root";
  const char *datafilename2 = "correlations_data_spring2017_chi2_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut.root";
                              
 TFile *datafile01 = TFile::Open(datafilename01,"READ");
 TFile *datafile02 = TFile::Open(datafilename02,"READ");

  
 TFile *datafile1 = TFile::Open(datafilename1,"READ");
 TFile *datafile2 = TFile::Open(datafilename2,"READ");

   
   if (!datafile01 || !datafile02 || !datafile1 || !datafile2 ) return;



  TH2D *h2d_data_ppip_mass_subtr =  (TH2D*) datafile01->Get("ProtonPiplus2_mass_vs_minust");
  TH2D *h2d_data_2pi_mass_subtr = (TH2D*) datafile02->Get("PiplusUPiminus_mass_vs_minust");
  TH2D *h2d_data_3pi_mass_subtr = (TH2D*) datafile02->Get("PiplusUPiminusPiminus_mass_vs_minust");


  TH2D *h2d_data_ppip_mass_hybrid =  (TH2D*) datafile1->Get("ProtonPiplusL_mass_vs_minust");
  TH2D *h2d_data_2pi_mass_hybrid = (TH2D*) datafile2->Get("PiplusUPiminus_mass_vs_minust");
  TH2D *h2d_data_3pi_mass_hybrid = (TH2D*) datafile2->Get("PiplusUPiminusPiminus_mass_vs_minust");


  

  Double_t left[6] = {0.,0.,0.25,0.5,0.75,1.};
  Double_t right[6] = {2.,0.25,0.5,0.75,1.,2.};
  char const *trange[6] = {"0 #leq -t #leq 2 GeV^{2}","0 #leq -t #leq 0.25 GeV^{2}","0.25 GeV^{2} #leq -t #leq 0.5 GeV^{2}","0.5 GeV^{2} #leq -t #leq 0.75 GeV^{2}","0.75 GeV^{2} #leq -t #leq 1 GeV^{2}","1 GeV^{2} #leq -t #leq 2 GeV^{2}"};

  
  TH1D *hdata_ppip_mass_subtr[6],*hdata_2pi_mass_subtr[6],*hdata_3pi_mass_subtr[6];
  TH1D *hdata_ppip_mass_hybrid[6],*hdata_2pi_mass_hybrid[6],*hdata_3pi_mass_hybrid[6];


  gStyle->SetOptStat(0);
  
     for (int i=0; i<6; i++){

       hdata_ppip_mass_subtr[i]  = h2d_data_ppip_mass_subtr->ProjectionX(Form("data_ppip_subtr_bin%d_px",i),h2d_data_ppip_mass_subtr->GetYaxis()->FindBin(left[i]),h2d_data_ppip_mass_subtr->GetYaxis()->FindBin(right[i]));
	 hdata_2pi_mass_subtr[i]  = h2d_data_2pi_mass_subtr->ProjectionX(Form("data_2pi_subtr_bin%d_px",i),h2d_data_2pi_mass_subtr->GetYaxis()->FindBin(left[i]),h2d_data_2pi_mass_subtr->GetYaxis()->FindBin(right[i]));
       hdata_3pi_mass_subtr[i]  = h2d_data_3pi_mass_subtr->ProjectionX(Form("data_3pi_subtr_bin%d_px",i),h2d_data_3pi_mass_subtr->GetYaxis()->FindBin(left[i]),h2d_data_3pi_mass_subtr->GetYaxis()->FindBin(right[i]));

       hdata_ppip_mass_hybrid[i]  = h2d_data_ppip_mass_hybrid->ProjectionX(Form("data_ppip_hybrid_bin%d_px",i),h2d_data_ppip_mass_hybrid->GetYaxis()->FindBin(left[i]),h2d_data_ppip_mass_hybrid->GetYaxis()->FindBin(right[i]));
       hdata_2pi_mass_hybrid[i]  = h2d_data_2pi_mass_hybrid->ProjectionX(Form("data_2pi_hybrid_bin%d_px",i),h2d_data_2pi_mass_hybrid->GetYaxis()->FindBin(left[i]),h2d_data_2pi_mass_hybrid->GetYaxis()->FindBin(right[i]));
       hdata_3pi_mass_hybrid[i]  = h2d_data_3pi_mass_hybrid->ProjectionX(Form("data_3pi_hybrid_bin%d_px",i),h2d_data_3pi_mass_hybrid->GetYaxis()->FindBin(left[i]),h2d_data_3pi_mass_hybrid->GetYaxis()->FindBin(right[i]));


       draw_bin(hdata_ppip_mass_subtr[i],hdata_ppip_mass_hybrid[i],hdata_3pi_mass_subtr[i],hdata_3pi_mass_hybrid[i],hdata_2pi_mass_subtr[i],hdata_2pi_mass_hybrid[i],trange[i],i);

       
     }
   

     
   

   
   datafile01->Close();
   datafile02->Close();
  
   datafile1->Close();
   datafile2->Close();

}
