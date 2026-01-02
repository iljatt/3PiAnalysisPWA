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



//Maybe add a function just for drawing




void fit_single_roofit(const char *var, TFile *data_histfile, TFile *mc_sig_histfile, TFile *mc_bg2_histfile, TFile *mc_bg3_histfile, TFile *mc_bg4_histfile, TFile *mc_bg5_histfile){


  std::string varname = var;
  std::string pdfname = "Comp_DataMC_fitted_separately_Roofit/"+varname+"_fitted.pdf";
  Double_t posl,posr;

  if (strncmp(var,"ProtonPiplusUPiminus1Piminus2_M",31)==0 || strncmp(var,"ProtonPiplusLPiminus1Piminus2_M",31)==0 ||
      strncmp(var,"ProtonPiplusUPiminus1_M",23)==0 || strncmp(var,"ProtonPiplusLPiplusUPiminus1_M",30)==0){
       posl = 0.1;
       posr = 0.3;
     }
     else {
       posl = 0.7;
       posr = 0.9;
  }	 

  
    
  TH1D *var_data =  (TH1D*)   data_histfile->Get(var);
  TH1D *var_mcsig = (TH1D*) mc_sig_histfile->Get(var);
  TH1D *var_mcbg2 = (TH1D*) mc_bg2_histfile->Get(var);
  TH1D *var_mcbg3 = (TH1D*) mc_bg3_histfile->Get(var);
  TH1D *var_mcbg4 = (TH1D*) mc_bg4_histfile->Get(var);
  TH1D *var_mcbg5 = (TH1D*) mc_bg5_histfile->Get(var);

  
  RooRealVar x("x", var,0.,10.);

  
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
    RooRealVar nsig("Sig", "sig", 1.e+4, 1., 1.e+6);

    RooRealVar rbg2("bgratio2","BG2SIG ratio",0., 0., 1.0);
    RooRealVar rbg3("bgratio3","BG2SIG ratio",0., 0., 1.0);
    RooRealVar rbg4("bgratio4","BG2SIG ratio",0., 0., 1.0);
    RooRealVar rbg5("bgratio5","BG2SIG ratio",0., 0., 1.0);

    RooProduct nbg2("bg2", "bg2", RooArgList(nsig,rbg2));
    RooProduct nbg3("bg3", "bg3", RooArgList(nsig,rbg3));
    RooProduct nbg4("bg4", "bg4", RooArgList(nsig,rbg4));
    RooProduct nbg5("bg5", "bg5", RooArgList(nsig,rbg5));


    
    //    Double_t lval = x.getMin();
    //    Double_t rval = x.getMax();    
    //    if (strncmp(var,"FourPions_M",11)==0)  lval = 1.8;


  //Adding the functions together into one PDF which describes the entire shape and fitting
  RooAddPdf total("total", "Sig+bg2+bg3+bg4+bg5", RooArgList(pdfSig,pdfBg2,pdfBg3,pdfBg4,pdfBg5), RooArgList(nsig,nbg2,nbg3,nbg4,nbg5));
  RooFitResult *fitres = total.fitTo(dhData, Extended(true), Save(kTRUE)); //Range(lval,rval)
  Int_t status = fitres->status();

  

  //Define RooFit objects for drawing
  RooPlot *dataframe = x.frame(Title(""));
     if (status==0) dataframe->SetTitle("An extended binned ML fit converged");
     else dataframe->SetTitle("An extended binned ML fit has not converged");


  dhData.plotOn(dataframe, MarkerSize(0.7), RooFit::Name("Data"));

  
  //total.paramOn(dataframe, Parameters(RooArgList(sigmean, sigwidth)), Format("NEU", AutoPrecision(2)), Layout(0.9,0.6,0.9));
  total.plotOn(dataframe, RooFit::Components(pdfSig), LineStyle(kSolid),  LineColor(kCyan), FillColor(kCyan), FillStyle(3004), RooFit::DrawOption("LF"), RooFit::Name("Signal"));
  total.plotOn(dataframe, RooFit::Components(pdfBg2), LineStyle(kDashed), LineColor(kBlue+2), FillColor(kBlue+2), FillStyle(3005), RooFit::DrawOption("LF"), RooFit::Name("BG2"));
  total.plotOn(dataframe, RooFit::Components(pdfBg3), LineStyle(kDashed), LineColor(kGreen+2), FillColor(kGreen+2), FillStyle(3004), RooFit::DrawOption("LF"), RooFit::Name("BG3"));
  total.plotOn(dataframe, RooFit::Components(pdfBg4), LineStyle(kDashed), LineColor(kYellow-4), FillColor(kYellow-4), FillStyle(3001), RooFit::DrawOption("LF"), RooFit::Name("BG4"));
  total.plotOn(dataframe, RooFit::Components(pdfBg5), LineStyle(kDashed), LineColor(kMagenta+2), FillColor(kMagenta+2), FillStyle(3003), RooFit::DrawOption("LF"), RooFit::Name("BG5"));
  total.plotOn(dataframe, RooFit::LineColor(kRed), RooFit::Name("Fullfit"));

  
    
  //New canvas to draw everything on // PLOTTING STARTS HERE ////////////////////////////////////////////
  TCanvas *canva = new TCanvas(var,var,15,10,400,300);

  dataframe->SetXTitle(var);
  dataframe->SetYTitle("");
  dataframe->GetYaxis()->SetTitleOffset(1.1);
  dataframe->GetXaxis()->SetTitleOffset(1.2);

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




void fit_eachMCcomb_roofit()
{

  //Import the files with histograms made out of our data, MC signal and four MC backgrounds
  TFile *data_histfile = TFile::Open("histos_data_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");

  

  /*
  TFile *data_file = TFile::Open("flattree_2pip2pim_sum_30274_31057_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TTree *tree = (TTree*)data_file->TFile::Get("kin"); 

  // Add our data tree into a roodataset and convert it into RooDataHist
  RooDataSet fulldata("data", "Full dataset", observables, Import(*tree));
  RooDataHist data_binned("data_binned","Binned dataset",observables,fulldata);

  // Call fitting    
  RooFitResult *fitres = total.fitTo(data_binned, Extended(true), Save(kTRUE));
  */

  
  
  TFile *mc_sig_histfile = TFile::Open("histos_mc_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_bg2_histfile = TFile::Open("histos_mc_bgcomb2_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_bg3_histfile = TFile::Open("histos_mc_bgcomb3_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_bg4_histfile = TFile::Open("histos_mc_bgcomb4_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_bg5_histfile = TFile::Open("histos_mc_bgcomb5_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");

  



  
  
  std::string variables[12] = {"Piminus1Proton_M","Piminus2Proton_M","ProtonPiplusUPiminus1_M","ProtonPiplusUPiminus2_M","ProtonPiplusLPiminus1_M","ProtonPiplusLPiminus2_M",
    "ProtonPiplusLPiminus1Piminus2_M","ProtonPiplusLPiplusUPiminus1_M","PiplusLPiplusUPiminus1_M","PiplusLPiplusUPiminus2_M","ProtonPiplusUPiminus1Piminus2_M","FourPions_M"};


   for (int i=0; i<12; i++){
     const char *var = variables[i].c_str();
     fit_single_roofit(var,data_histfile,mc_sig_histfile,mc_bg2_histfile,mc_bg3_histfile,mc_bg4_histfile,mc_bg5_histfile);
   }



   
  
   data_histfile->Close();
   mc_sig_histfile->Close();
   mc_bg2_histfile->Close();
   mc_bg3_histfile->Close();
   mc_bg4_histfile->Close();
   mc_bg5_histfile->Close();  
  

}
