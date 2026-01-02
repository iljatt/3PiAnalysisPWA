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







void setstyle(TH1D *dhisto)
{
    
    
    dhisto->SetMarkerSize(0.4);
    dhisto->SetMarkerStyle(20);
    dhisto->SetMarkerColor(kBlack);
    dhisto->SetTitleOffset(1.0,"X");
    dhisto->SetTitleSize(0.07,"X");
    dhisto->SetTitleOffset(1.2,"Y");
    dhisto->SetTitleSize(0.04,"Y"); 
    dhisto->SetLineColor(kBlack);
    dhisto->SetLineWidth(2);    
    dhisto->SetFillColor(kBlue-7);
    dhisto->SetFillStyle(3016);
}





void draw_bin(TH1D *h_ppip, TH1D *h_3pi, TH1D *h_2pi, char const *title, int i)
{

  std::string bin_num = Form("%d",i);
  std::string pdfname = "FitDeltaMass/bin"+bin_num+".pdf";

  setstyle(h_ppip);
  setstyle(h_3pi);
  setstyle(h_2pi);


  h_ppip->SetTitle(Form("%s",title));
  h_ppip->GetXaxis()->SetTitle("m(p#pi^{+}(lv))  [GeV]");
  h_ppip->GetXaxis()->SetRangeUser(0.8,3.);

  h_2pi->GetXaxis()->SetTitle("m(#pi^{+}(uv)#pi^{-})  [GeV]");
  h_3pi->GetXaxis()->SetTitle("m(#pi^{+}(uv)#pi^{-}#pi^{-})  [GeV]");



    TPaveText *ptw = new TPaveText(0.59,0.68,0.74,0.78,"NDC");
    ptw->AddText("#bf{#pi}_{2}(1670), #bf{#pi}_{1}(1600) ?\n");
    ptw->SetTextSize(0.06);
    ptw->SetBorderSize(0);
    ptw->SetFillStyle(0);

    TPaveText *pte = new TPaveText(0.45,0.4,0.55,0.5,"NDC");
    pte->AddText("f_{2}(1270)\n");
    pte->SetTextSize(0.08);
    pte->SetBorderSize(0);
    pte->SetFillStyle(0);

    TPaveText *ptr = new TPaveText(0.5,0.75,0.6,0.85,"NDC");
    ptr->AddText("a_{2}(1320)\n");
    ptr->SetTextSize(0.08);
    ptr->SetBorderSize(0);
    ptr->SetFillStyle(0);

    TPaveText *pts = new TPaveText(0.38,0.75,0.48,0.85,"NDC");
    pts->AddText("#rho(770)\n");
    pts->SetTextSize(0.08);
    pts->SetBorderSize(0);
    pts->SetFillStyle(0);


    
  
  //New canvas to draw everything on // PLOTTING STARTS HERE ////////////////////////////////////////////
  TCanvas *canva = new TCanvas(Form("c_%s",title),Form("c_%s",title),15,10,400,1100);
  canva->Divide(1,3);

  canva->cd(1);
  gPad->SetBottomMargin(0.15);
  h_ppip->Draw("*H");
  gPad->Update();
  TLine *lrange = new TLine(1.15,0.,1.15,gPad->GetUymax());
  TLine *rrange = new TLine(1.35,0.,1.35,gPad->GetUymax());
  lrange->SetLineColor(kRed); lrange->SetLineWidth(3); lrange->SetLineStyle(kDashed);
  rrange->SetLineColor(kRed); rrange->SetLineWidth(3); rrange->SetLineStyle(kDashed);
  lrange->Draw("SAME");
  rrange->Draw("SAME");
  canva->Update();  



  
  
  canva->cd(2);
  gPad->SetBottomMargin(0.15);
  h_3pi->Draw("*H");
  if (i==1) ptr->Draw("SAME");
  if (i==2) ptw->Draw("SAME");
  canva->Update();

  canva->cd(3);
  gPad->SetBottomMargin(0.15);
  h_2pi->Draw("*H");
  if (i==1) pts->Draw("SAME");
  if (i==2) pte->Draw("SAME");
  canva->Update();


  
  canva->SaveAs(pdfname.c_str(),"pdf");
  canva->Close();


}







void draw_tbins()
{

  //Import the files with histograms made out of our data
  const char *datafilename1 = "correlations_data_spring2017_chi2_precuts_diffpipluscut_fourpionscut_piminuscut.root";
  const char *datafilename2 = "correlations_data_spring2017_chi2_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut.root";
                              
  
 TFile *datafile1 = TFile::Open(datafilename1,"READ");
 TFile *datafile2 = TFile::Open(datafilename2,"READ");

   
   if (!datafile1 || !datafile2 ) return;



  TH2D *h2d_data_ppip_mass =  (TH2D*) datafile1->Get("ProtonPiplusL_mass_vs_minust");
  TH2D *h2d_data_2pi_mass = (TH2D*) datafile2->Get("PiplusUPiminus_mass_vs_minust");
  TH2D *h2d_data_3pi_mass = (TH2D*) datafile2->Get("PiplusUPiminusPiminus_mass_vs_minust");


  

  Double_t left[6] = {0.,0.,0.25,0.5,0.75,1.};
  Double_t right[6] = {2.,0.25,0.5,0.75,1.,2.};
  char const *trange[6] = {"0 #leq -t #leq 2 GeV^{2}","0 #leq -t #leq 0.25 GeV^{2}","0.25 GeV^{2} #leq -t #leq 0.5 GeV^{2}","0.5 GeV^{2} #leq -t #leq 0.75 GeV^{2}","0.75 GeV^{2} #leq -t #leq 1 GeV^{2}","1 GeV^{2} #leq -t #leq 2 GeV^{2}"};

  
  TH1D *hdata_ppip_mass[6],*hdata_2pi_mass[6],*hdata_3pi_mass[6];


  gStyle->SetOptStat(0);
  gStyle->SetTitleFontSize(0.08);
  
     for (int i=0; i<6; i++){

       hdata_ppip_mass[i]  = h2d_data_ppip_mass->ProjectionX(Form("data_ppip_bin%d_px",i),h2d_data_ppip_mass->GetYaxis()->FindBin(left[i]),h2d_data_ppip_mass->GetYaxis()->FindBin(right[i]));
       hdata_2pi_mass[i]  = h2d_data_2pi_mass->ProjectionX(Form("data_2pi_bin%d_px",i),h2d_data_2pi_mass->GetYaxis()->FindBin(left[i]),h2d_data_2pi_mass->GetYaxis()->FindBin(right[i]));
       hdata_3pi_mass[i]  = h2d_data_3pi_mass->ProjectionX(Form("data_3pi_bin%d_px",i),h2d_data_3pi_mass->GetYaxis()->FindBin(left[i]),h2d_data_3pi_mass->GetYaxis()->FindBin(right[i]));

       draw_bin(hdata_ppip_mass[i],hdata_3pi_mass[i],hdata_2pi_mass[i],trange[i],i);

       
     }
   

     
   

   
  
   datafile1->Close();
   datafile2->Close();

}
