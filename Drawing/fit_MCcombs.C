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






void draw_projection(const char *var, RooPlot *dataframe)
{

  std::string varname = var;
  std::string pdfname = "Comp_DataMC_fitted_Roofit/"+varname+"_fitted.pdf";
  Double_t posl,posr;

  if (strncmp(var,"ProtonPiplusUPiminus1Piminus2_M",31)==0 || strncmp(var,"ProtonPiplusLPiminus1Piminus2_M",31)==0 ||
      strncmp(var,"ProtonPiplusUPiminus1_M",23)==0 || strncmp(var,"ProtonPiplusLPiplusUPiminus1_M",30)==0){
       posl = 0.1;
       posr = 0.25;
     }
     else {
       posl = 0.75;
       posr = 0.9;
  }	 



  TCanvas *canva = new TCanvas(var,var,15,10,400,300);

  dataframe->SetXTitle(var);
  dataframe->SetYTitle("");
  dataframe->GetYaxis()->SetTitleOffset(1.1);
  dataframe->GetXaxis()->SetTitleOffset(1.2);

  TAxis* xachse = dataframe->GetXaxis();
  TAxis* yachse = dataframe->GetYaxis();
  xachse->SetTitleFont (132);
  yachse->SetTitleFont (132);
  xachse->SetLabelFont (132);
  yachse->SetLabelFont (132);
  yachse->SetTitleOffset(1.5);
  
  dataframe->Draw();
  canva->Update();
  

  
  TLegend* leg = new TLegend(posl, 0.6, posr, 0.9);
  leg->AddEntry(dataframe->findObject("Data"), "Data");
  leg->AddEntry(dataframe->findObject("Fullfit"), "Full Fit","l");
  leg->AddEntry(dataframe->findObject("Sig"), "MC sig ","lf");
  leg->AddEntry(dataframe->findObject("BG2"), "LV(p)","lf");
  leg->AddEntry(dataframe->findObject("BG3"), "LV(p#pi^{+}#pi^{-})","lf");
  leg->AddEntry(dataframe->findObject("BG4"), "LV(p#pi^{+}#pi^{-}#pi^{-})","lf");
  leg->AddEntry(dataframe->findObject("BG5"), "LV(p#pi^{+}#pi^{+}#pi^{-})","lf");
  leg->Draw("same");
  
  canva->SaveAs(pdfname.c_str(),"pdf");
  canva->Close();
 
}
  



    

  
    
  




void fit_MCcombs(int NUM = 3)
{
                                  
  TFile *data_file = TFile::Open("flattree_2pip2pim_sum_30274_31057_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TTree *tree = (TTree*)data_file->TFile::Get("kin"); 
  
  //Open the files with histograms made out of our data, MC signal and four MC backgrounds
  TFile *data_histfile = TFile::Open("histos_data_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_sig_histfile = TFile::Open("histos_mc_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_bg2_histfile = TFile::Open("histos_mc_bgcomb2_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_bg3_histfile = TFile::Open("histos_mc_bgcomb3_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_bg4_histfile = TFile::Open("histos_mc_bgcomb4_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_bg5_histfile = TFile::Open("histos_mc_bgcomb5_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");


  //Variables to fit
  const char *var[12] = {"ProtonPiplusUPiminus1Piminus2_M","FourPions_M","PiplusLPiplusUPiminus1_M","PiplusLPiplusUPiminus2_M","Piminus1Proton_M","Piminus2Proton_M","ProtonPiplusUPiminus1_M","ProtonPiplusUPiminus2_M","ProtonPiplusLPiminus1_M","ProtonPiplusLPiminus2_M","ProtonPiplusLPiminus1Piminus2_M","ProtonPiplusLPiplusUPiminus1_M"};
  
    
  std::vector<RooRealVar*> x;
  std::vector<TH1D*> var_data;
  std::vector<TH1D*> var_mcsig;
  std::vector<TH1D*> var_mcbg2;
  std::vector<TH1D*> var_mcbg3;
  std::vector<TH1D*> var_mcbg4;
  std::vector<TH1D*> var_mcbg5;
  std::vector<RooDataHist*> dhData;
  std::vector<RooDataHist*> dhMCsig;
  std::vector<RooDataHist*> dhMCbg2;
  std::vector<RooDataHist*> dhMCbg3;
  std::vector<RooDataHist*> dhMCbg4;
  std::vector<RooDataHist*> dhMCbg5;
  std::vector<RooHistPdf*> pdfSig;
  std::vector<RooHistPdf*> pdfBg2;
  std::vector<RooHistPdf*> pdfBg3;
  std::vector<RooHistPdf*> pdfBg4;
  std::vector<RooHistPdf*> pdfBg5;
  std::vector<RooPlot*> rooframe;
  
    
  RooArgList sigList,bg2List,bg3List,bg4List,bg5List;
  RooArgSet observables;  

  
  for (int i=0; i<NUM; i++){ 
	x.push_back(new RooRealVar(Form("%s",var[i]), Form("%s",var[i]), 0., 5.));
	observables.add(*x[i]);
  }


  // Add our data tree into a roodataset and convert it into RooDataHist
  RooDataSet fulldata("data", "Full dataset", observables, Import(*tree));
  RooDataHist data_binned("data_binned","Binned dataset",observables,fulldata);
  
    
  //Read all 1-dimensional histograms and convert them into RooHistPdfs
  for (int i=0; i<NUM; i++){ 

	var_data.push_back((TH1D*)   data_histfile->TFile::Get(var[i]));
	var_mcsig.push_back((TH1D*) mc_sig_histfile->TFile::Get(var[i]));
	var_mcbg2.push_back((TH1D*) mc_bg2_histfile->TFile::Get(var[i]));
	var_mcbg3.push_back((TH1D*) mc_bg3_histfile->TFile::Get(var[i]));
	var_mcbg4.push_back((TH1D*) mc_bg4_histfile->TFile::Get(var[i]));
	var_mcbg5.push_back((TH1D*) mc_bg5_histfile->TFile::Get(var[i]));

	dhData.push_back(new RooDataHist(Form("dhData_%s",var[i]), Form("Measured data (%s projection)",var[i]), *x[i], var_data[i]));
	dhMCsig.push_back(new RooDataHist(Form("dhMCsig_%s",var[i]), Form("MC signal (%s projection)",var[i]), *x[i], var_mcsig[i]));
	dhMCbg2.push_back(new RooDataHist(Form("dhMCbg2_%s",var[i]), Form("MC background #2 (%s projection)",var[i]), *x[i], var_mcbg2[i]));
	dhMCbg3.push_back(new RooDataHist(Form("dhMCbg3_%s",var[i]), Form("MC background #3 (%s projection)",var[i]), *x[i], var_mcbg3[i]));
	dhMCbg4.push_back(new RooDataHist(Form("dhMCbg4_%s",var[i]), Form("MC background #4 (%s projection)",var[i]), *x[i], var_mcbg4[i]));
	dhMCbg5.push_back(new RooDataHist(Form("dhMCbg5_%s",var[i]), Form("MC background #5 (%s projection)",var[i]), *x[i], var_mcbg5[i]));


	pdfSig.push_back(new RooHistPdf(Form("pdfSig_%s",var[i]), Form("Signal PDF (%s projection)",var[i]), *x[i], *dhMCsig[i],2));
	pdfBg2.push_back(new RooHistPdf(Form("pdfBg2_%s",var[i]), Form("Background PDF #2 (%s projection)",var[i]), *x[i], *dhMCbg2[i],2));
	pdfBg3.push_back(new RooHistPdf(Form("pdfBg3_%s",var[i]), Form("Background PDF #3 (%s projection)",var[i]), *x[i], *dhMCbg3[i],2));
	pdfBg4.push_back(new RooHistPdf(Form("pdfBg4_%s",var[i]), Form("Background PDF #4 (%s projection)",var[i]), *x[i], *dhMCbg4[i],2));
	pdfBg5.push_back(new RooHistPdf(Form("pdfBg5_%s",var[i]), Form("Background PDF #5 (%s projection)",var[i]), *x[i], *dhMCbg5[i],2));

	
        sigList.add(*pdfSig[i]);
        bg2List.add(*pdfBg2[i]);
        bg3List.add(*pdfBg3[i]);
        bg4List.add(*pdfBg4[i]);
        bg5List.add(*pdfBg5[i]);

  }  


  for (int i=0; i<NUM; i++) printf("%-35s\t%s\n",pdfSig[i]->GetName(),pdfSig[i]->GetTitle());


  
  
    //Variables for counting the number of signal and background events 
    RooRealVar rbg("bgratio","BG2SIG ratio",0.0, 0.0, 1.0); 
    RooRealVar nsig("sig", "sig", 1.e+4, 0., 1.e+6);

    RooProduct nbg2("bg2", "bg2", RooArgList(nsig,rbg));
    RooProduct nbg3("bg3", "bg3", RooArgList(nsig,rbg));
    RooProduct nbg4("bg4", "bg4", RooArgList(nsig,rbg));
    RooProduct nbg5("bg5", "bg5", RooArgList(nsig,rbg));

  
    //Build a multidimensional fit without corellations: multiply pdfs for all variables in a single 12Dim PDF
    //Add a few 12Dim PDFs together into a total PDF, which describes an entire shape
    RooProdPdf PDFsigProdNonresMasses("prod_masses_signal","Signal PDF for product of all masses with a ph. sp. lineshape.",sigList);
    RooProdPdf PDFbg2ProdNonresMasses("prod_masses_bg2","Bg#2 PDF for product of all masses with a ph. sp. lineshape.",bg2List);
    RooProdPdf PDFbg3ProdNonresMasses("prod_masses_bg3","Bg#3 PDF for product of all masses with a ph. sp. lineshape.",bg3List);
    RooProdPdf PDFbg4ProdNonresMasses("prod_masses_bg4","Bg#4 PDF for product of all masses with a ph. sp. lineshape.",bg4List);
    RooProdPdf PDFbg5ProdNonresMasses("prod_masses_bg5","Bg#5 PDF for product of all masses with a ph. sp. lineshape.",bg5List);
    RooAddPdf total("total","Sig+Bg#2+Bg#3+Bg#4+Bg#5",RooArgList(PDFsigProdNonresMasses,PDFbg2ProdNonresMasses,PDFbg3ProdNonresMasses,PDFbg4ProdNonresMasses,PDFbg5ProdNonresMasses),
    RooArgList(nsig,nbg2,nbg3,nbg4,nbg5));

    //RooAddPdf total("total","Sig+Bg#2+Bg#3",RooArgList(PDFsigProdNonresMasses,PDFbg2ProdNonresMasses,PDFbg3ProdNonresMasses),RooArgList(nsig,nbg2,nbg3));

    
    // Call fitting    
     RooFitResult *fitres = total.fitTo(data_binned, Extended(true), Save(kTRUE));
    


        
   //Define RooFit frames for drawing
    for (int i=0; i<NUM; i++){
      rooframe.push_back(x[i]->frame());
      rooframe[i]->SetTitle(Form("Result of an extended ML %d-dim fit",NUM));      

      data_binned.plotOn(rooframe[i], MarkerSize(0.7), RooFit::Name("Data")); 
      total.plotOn(rooframe[i], RooFit::Components(PDFsigProdNonresMasses), LineStyle(kSolid),LineColor(kCyan),FillColor(kCyan),FillStyle(3004), RooFit::DrawOption("LF"), RooFit::Name("Sig"));
      total.plotOn(rooframe[i], RooFit::Components(PDFbg2ProdNonresMasses), LineStyle(kDashed),LineColor(kBlue+2),FillColor(kBlue+2),FillStyle(3005), RooFit::DrawOption("LF"), RooFit::Name("BG2"));
      total.plotOn(rooframe[i], RooFit::Components(PDFbg3ProdNonresMasses), LineStyle(kDashed),LineColor(kGreen+2),FillColor(kGreen+2),FillStyle(3004), RooFit::DrawOption("LF"), RooFit::Name("BG3"));
      total.plotOn(rooframe[i], RooFit::Components(PDFbg4ProdNonresMasses), LineStyle(kDashed),LineColor(kYellow-4),FillColor(kYellow-4),FillStyle(3001), RooFit::DrawOption("LF"), RooFit::Name("BG4"));
      total.plotOn(rooframe[i],  Components(PDFbg5ProdNonresMasses), LineStyle(kDashed),LineColor(kMagenta+2),FillColor(kMagenta+2),FillStyle(3003), RooFit::DrawOption("LF"), RooFit::Name("BG5"));
      total.plotOn(rooframe[i], RooFit::LineColor(kRed), RooFit::Name("Fullfit"));

    }


//    total.paramOn(dataframe, Parameters(RooArgList(sigmean, sigwidth)), Format("NEU", AutoPrecision(2)), Layout(0.9,0.6,0.9));      
//    dhData[i]->plotOn(rooframe[i], MarkerSize(0.7), RooFit::Name("Data")); // Plot a 1-dimensional histogram

    

    //Drawing  
    for (int i=0; i<NUM; i++)
      draw_projection(var[i],rooframe[i]);



  




   data_file->Close(); 
   data_histfile->Close();
   mc_sig_histfile->Close();
   mc_bg2_histfile->Close();
   mc_bg3_histfile->Close();
   mc_bg4_histfile->Close();
   mc_bg5_histfile->Close();  
  

}
