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
    dhisto->SetTitleOffset(1.1,"X");
    dhisto->SetTitleSize(0.04,"X");
    dhisto->SetTitleOffset(1.2,"Y");
    dhisto->SetTitleSize(0.04,"Y"); 
    dhisto->SetLineColor(kBlack);
    dhisto->SetLineWidth(2);    
    dhisto->SetFillColor(kBlue-7);
    dhisto->SetFillStyle(3016);
}





void draw_single(TH1D *var_data, char const *title, int i)
{

  std::string bin_num = Form("%d",i);
  std::string pdfname = "FitDeltaMass/bin"+bin_num+"_mppip.pdf";

  setstyle(var_data);
  var_data->SetTitle(Form("%s",title));
  var_data->GetXaxis()->SetTitle("m(p#pi^{+}_{slow})  [GeV]");
  var_data->GetXaxis()->SetRangeUser(0.8,3.);


  
  //New canvas to draw everything on // PLOTTING STARTS HERE ////////////////////////////////////////////
  TCanvas *canva = new TCanvas(Form("c_%s",title),Form("c_%s",title),15,10,400,300);
  TLegend* leg = new TLegend(0.7, 0.6, 0.7, 0.9);

  
  var_data->Draw("*H");
  canva->Update();

  
  TLine *lrange = new TLine(1.15,0.,1.15,canva->GetUymax());
  TLine *rrange = new TLine(1.35,0.,1.35,canva->GetUymax());
  lrange->SetLineColor(kRed); lrange->SetLineWidth(3); lrange->SetLineStyle(kDashed);
  rrange->SetLineColor(kRed); rrange->SetLineWidth(3); rrange->SetLineStyle(kDashed);
  lrange->Draw("SAME");
  rrange->Draw("SAME");
 

  
  canva->SaveAs(pdfname.c_str(),"pdf");
  canva->Close();


}






void fit_single_roofit(TH1D *var_data, TH1D *var_mcsig, TH1D *var_mcbg2, TH1D *var_mcbg3, TH1D *var_mcbg4, TH1D *var_mcbg5, char const *title){


  std::string varname = title;
  std::string pdfname = "FitDeltaMass/"+varname+"_fitted.pdf";
  Double_t posl=0.7,posr=0.9;
  
      
  RooRealVar x("x", "ProtonPiplusSlow_M",0.8,3.8);

  
  
  //Convert all TH1 histograms into RooDataHists
  RooDataHist dhData("dhData", "Measured data", x, var_data);
  RooDataHist dhMCsig("dhMCsig", "MC signal", x, var_mcsig);
  RooDataHist dhMCbg2("dhMCbg2", "MC background #2", x, var_mcbg2);
  RooDataHist dhMCbg3("dhMCbg3", "MC background #3", x, var_mcbg3);
  RooDataHist dhMCbg4("dhMCbg4", "MC background #4", x, var_mcbg4);
  RooDataHist dhMCbg5("dhMCbg5", "MC background #5", x, var_mcbg5);
  

  
  // Build the total PDF
  RooHistPdf pdfSig("pdfSig", "Signal PDF", x, dhMCsig,2);
  RooHistPdf pdfBg2("pdfBg2", "Background PDF #2", x, dhMCbg2,2);
  RooHistPdf pdfBg3("pdfBg3", "Background PDF #3", x, dhMCbg3,2);
  RooHistPdf pdfBg4("pdfBg4", "Background PDF #4", x, dhMCbg4,2);
  RooHistPdf pdfBg5("pdfBg5", "Background PDF #5", x, dhMCbg5,2);


  
  //Variables for counting the number of signal and background events 
    RooRealVar nsig("Sig", "sig", 1.e+4, 1.e+3, 1.e+7);

    RooRealVar nbg2("bg2", "bg2", 1.e+4, 1.e+3, 1.e+7);
    RooRealVar nbg3("bg3", "bg3", 1.e+4, 1.e+3, 1.e+7);
    RooRealVar nbg4("bg4", "bg4", 1.e+4, 1.e+3, 1.e+7);
    RooRealVar nbg5("bg5", "bg5", 1.e+4, 1.e+3, 1.e+7);
    
    /*
    RooRealVar rbg2("bgratio2","BG2SIG ratio",0.1, 0., 1.0);
    RooRealVar rbg3("bgratio3","BG2SIG ratio",0.1, 0., 1.0);
    RooRealVar rbg4("bgratio4","BG2SIG ratio",0.1, 0., 1.0);
    RooRealVar rbg5("bgratio5","BG2SIG ratio",0.1, 0., 1.0);

    RooProduct nbg2("bg2", "bg2", RooArgList(nsig,rbg2));
    RooProduct nbg3("bg3", "bg3", RooArgList(nsig,rbg3));
    RooProduct nbg4("bg4", "bg4", RooArgList(nsig,rbg4));
    RooProduct nbg5("bg5", "bg5", RooArgList(nsig,rbg5));
    */

    
    //    Double_t lval = x.getMin();
    //    Double_t rval = x.getMax();    
    //    if (strncmp(var,"FourPions_M",11)==0)  lval = 1.8;


  //Adding the functions together into one PDF which describes the entire shape and fitting
  RooAddPdf total("total", "Sig+bg2+bg3+bg4+bg5", RooArgList(pdfSig,pdfBg2,pdfBg3,pdfBg4,pdfBg5), RooArgList(nsig,nbg2,nbg3,nbg4,nbg5));
  RooFitResult *fitres = total.fitTo(dhData, Extended(true), Save(kTRUE)); //Range(lval,rval)
  Int_t status = fitres->status();

  
  

  //Define RooFit objects for drawing
  RooPlot *dataframe = x.frame(Range(0.8,2.8));
  dataframe->SetTitle(title);
  //if (status==0) dataframe->SetTitle("An extended binned ML fit converged");
  //     else dataframe->SetTitle("An extended binned ML fit has not converged");


  dhData.plotOn(dataframe, MarkerSize(0.5), RooFit::Name("Data"));

  
  //total.paramOn(dataframe, Parameters(RooArgList(sigmean, sigwidth)), Format("NEU", AutoPrecision(2)), Layout(0.9,0.6,0.9));
  total.plotOn(dataframe, RooFit::Components(pdfSig), LineStyle(kSolid),  LineColor(kCyan), FillColor(kCyan), FillStyle(3004), RooFit::DrawOption("LF"), RooFit::Name("Signal"));
  total.plotOn(dataframe, RooFit::Components(pdfBg2), LineStyle(kDashed), LineColor(kBlue+2), FillColor(kBlue+2), FillStyle(3005), RooFit::DrawOption("LF"), RooFit::Name("BG2"));
  total.plotOn(dataframe, RooFit::Components(pdfBg3), LineStyle(kDashed), LineColor(kGreen+2), FillColor(kGreen+2), FillStyle(3004), RooFit::DrawOption("LF"), RooFit::Name("BG3"));
  total.plotOn(dataframe, RooFit::Components(pdfBg4), LineStyle(kDashed), LineColor(kYellow-4), FillColor(kYellow-4), FillStyle(3001), RooFit::DrawOption("LF"), RooFit::Name("BG4"));
  total.plotOn(dataframe, RooFit::Components(pdfBg5), LineStyle(kDashed), LineColor(kMagenta+2), FillColor(kMagenta+2), FillStyle(3003), RooFit::DrawOption("LF"), RooFit::Name("BG5"));
  total.plotOn(dataframe, RooFit::LineColor(kRed), RooFit::Name("Fullfit"));

  
    
  //New canvas to draw everything on // PLOTTING STARTS HERE ////////////////////////////////////////////
  TCanvas *canva = new TCanvas(Form("c_%s",title),Form("c_%s",title),15,10,400,300);

  dataframe->GetYaxis()->SetTitleOffset(1.1);
  dataframe->GetXaxis()->SetTitleOffset(1.1);
  dataframe->GetXaxis()->SetTitleSize(0.04);
  dataframe->SetXTitle("m(p#pi^{+}_{slow})  [GeV]");
  dataframe->SetYTitle("");


  
  TAxis* xachse = dataframe->GetXaxis(); TAxis* yachse = dataframe->GetYaxis();
  xachse->SetTitleFont(132);
  yachse->SetTitleFont(132);
  xachse->SetLabelFont(132);
  yachse->SetLabelFont(132);
  yachse->SetTitleOffset(1.5);
  
  dataframe->Draw();
  canva->Update();
  

  
  TLegend* leg = new TLegend(posl, 0.6, posr, 0.9);
  leg->AddEntry(dataframe->findObject("Data"), "Data");
  leg->AddEntry(dataframe->findObject("Fullfit"), "Full Fit");
  leg->AddEntry(dataframe->findObject("Signal"), "MC signal ph.sp.");
  leg->AddEntry(dataframe->findObject("BG2"), "LV(p) ph.sp.","lf");
  leg->AddEntry(dataframe->findObject("BG3"), "LV(p#pi^{+}#pi^{-}) ph.sp.","lf");
  leg->AddEntry(dataframe->findObject("BG4"), "LV(p#pi^{+}#pi^{-}#pi^{-}) ph.sp.","lf");
  leg->AddEntry(dataframe->findObject("BG5"), "LV(p#pi^{+}#pi^{+}#pi^{-}) ph.sp.","lf");
  leg->Draw("same");
  
  canva->SaveAs(pdfname.c_str(),"pdf");
  canva->Close();
  
}




void fitDeltaMass()
{

  //Import the files with histograms made out of our data, MC signal and four MC backgrounds  
  const char *datafilename = "correlations_data_spring2017_precuts_diffpipluscut_fourpionscut_piminuscut.root";
  const char *mcfilename = "correlations_mcw_spring2017_precuts_diffpipluscut_fourpionscut_piminuscut.root";
  const char *mcbg1filename = "CombBGs/correlations_mc_bgcomb1_spring2017_precuts_diffpipluscut_fourpionscut_piminuscut.root";
  const char *mcbg2filename = "CombBGs/correlations_mc_bgcomb2_spring2017_precuts_diffpipluscut_fourpionscut_piminuscut.root";
  const char *mcbg3filename = "CombBGs/correlations_mc_bgcomb3_spring2017_precuts_diffpipluscut_fourpionscut_piminuscut.root";
  const char *mcbg4filename = "CombBGs/correlations_mc_bgcomb4_spring2017_precuts_diffpipluscut_fourpionscut_piminuscut.root";
  const char *mcbg5filename = "CombBGs/correlations_mc_bgcomb5_spring2017_precuts_diffpipluscut_fourpionscut_piminuscut.root";



   TFile *datafile = TFile::Open(datafilename,"READ");
   TFile *mcfile = TFile::Open(mcfilename,"READ");
   TFile *mcbg1file = TFile::Open(mcbg1filename,"READ");
   TFile *mcbg2file = TFile::Open(mcbg2filename,"READ");
   TFile *mcbg3file = TFile::Open(mcbg3filename,"READ");
   TFile *mcbg4file = TFile::Open(mcbg4filename,"READ");
   TFile *mcbg5file = TFile::Open(mcbg5filename,"READ");
   
   if (!datafile || !mcfile || !mcbg1filename || !mcbg2filename || !mcbg3filename || !mcbg4filename || !mcbg5filename) return;

   const char *var = "ProtonPiplus2_mass_vs_minust";


  TH2D *var_data =  (TH2D*) datafile->Get(var);
  TH2D *var_mcsig = (TH2D*) mcfile->Get(var);
  TH2D *var_mcbg2 = (TH2D*) mcbg2file->Get(var);
  TH2D *var_mcbg3 = (TH2D*) mcbg3file->Get(var);
  TH2D *var_mcbg4 = (TH2D*) mcbg4file->Get(var);
  TH2D *var_mcbg5 = (TH2D*) mcbg5file->Get(var);

   
   /*
   std::string variables[2] = {"Piplus1Proton_M","Piplus2Proton_M"};

   for (int i=1; i<2; i++){
     const char *var = variables[i].c_str();
     fit_single_roofit(var,datafile,mcfile,mcbg2file,mcbg3file,mcbg4file,mcbg5file,"0 #leq -t #leq 2 GeV^{2}");
   }
   */


  Double_t left[6] = {0.,0.,0.25,0.5,0.75,1.};
  Double_t right[6] = {2.,0.25,0.5,0.75,1.,2.};
  char const *trange[6] = {"0 #leq -t #leq 2 GeV^{2}","0 #leq -t #leq 0.25 GeV^{2}","0.25 GeV^{2} #leq -t #leq 0.5 GeV^{2}","0.5 GeV^{2} #leq -t #leq 0.75 GeV^{2}","0.75 GeV^{2} #leq -t #leq 1 GeV^{2}","1 GeV^{2} #leq -t #leq 2 GeV^{2}"};

  
  TH1D *hdata[6],*hmcsig[6],*hmcbg2[6],*hmcbg3[6],*hmcbg4[6],*hmcbg5[6];


  gStyle->SetOptStat(0);
  
     for (int i=0; i<6; i++){

       hdata[i]  = var_data->ProjectionX(Form("data_bin%d_px",i),var_data->GetYaxis()->FindBin(left[i]),var_data->GetYaxis()->FindBin(right[i]));
       hmcsig[i] = var_mcsig->ProjectionX(Form("mcsig_bin%d_px",i),var_mcsig->GetYaxis()->FindBin(left[i]),var_mcsig->GetYaxis()->FindBin(right[i]));
       hmcbg2[i] = var_mcbg2->ProjectionX(Form("mcbg2_bin%d_px",i),var_mcbg2->GetYaxis()->FindBin(left[i]),var_mcbg2->GetYaxis()->FindBin(right[i]));
       hmcbg3[i] = var_mcbg3->ProjectionX(Form("mcbg3_bin%d_px",i),var_mcbg3->GetYaxis()->FindBin(left[i]),var_mcbg3->GetYaxis()->FindBin(right[i]));
       hmcbg4[i] = var_mcbg4->ProjectionX(Form("mcbg4_bin%d_px",i),var_mcbg4->GetYaxis()->FindBin(left[i]),var_mcbg4->GetYaxis()->FindBin(right[i]));
       hmcbg5[i] = var_mcbg5->ProjectionX(Form("mcbg5_bin%d_px",i),var_mcbg5->GetYaxis()->FindBin(left[i]),var_mcbg5->GetYaxis()->FindBin(right[i]));

       /*       
       hdata[i]  = var_data->ProjectionX(Form("data_bin%d_px",i),0,-1);
       hmcsig[i] = var_mcsig->ProjectionX(Form("mcsig_bin%d_px",i),0,-1);
       hmcbg2[i] = var_mcbg2->ProjectionX(Form("mcbg2_bin%d_px",i),0,-1);
       hmcbg3[i] = var_mcbg3->ProjectionX(Form("mcbg3_bin%d_px",i),0,-1);
       hmcbg4[i] = var_mcbg4->ProjectionX(Form("mcbg4_bin%d_px",i),0,-1);
       hmcbg5[i] = var_mcbg5->ProjectionX(Form("mcbg5_bin%d_px",i),0,-1);
       */
       
       //       fit_single_roofit(hdata[i],hmcsig[i],hmcbg2[i],hmcbg3[i],hmcbg4[i],hmcbg5[i],trange[i]);

       draw_single(hdata[i],trange[i],i);
       
     }
   

     
   

   
  
   datafile->Close();
   mcfile->Close();
   mcbg2file->Close();
   mcbg3file->Close();
   mcbg4file->Close();
   mcbg5file->Close();  
  

}
