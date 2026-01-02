#include "RooStats/SPlot.h"
#include "TSPlot.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TChain.h"
#include "TH1F.h"
#include "TError.h"
#include <TCanvas.h>
#include <map>
#include <string>

using namespace std;




void defstyle(TH1D *histo)
{
  //    histo->GetYaxis()->SetTitle("Entries in 100 bins");
  
    histo->SetMarkerSize(0.5);
    histo->SetMarkerStyle(20);
    histo->SetTitleOffset(0.9,"X");
    histo->SetTitleSize(0.05,"X");
    histo->SetTitleOffset(1.2,"Y");
    histo->SetTitleSize(0.04,"Y"); 
    histo->SetLineColor(kBlack);
    histo->SetLineWidth(2);
    //    histo->GetXaxis()->SetTitleFont(132);
    //    histo->GetYaxis()->SetTitleFont(132);
    //    histo->GetXaxis()->SetLabelFont(132);
    //    histo->GetYaxis()->SetLabelFont(132);
    
 }





void fit_single(const char *var, TFile *data_histfile, TFile *mc_sig_histfile, TFile *mc_bg2_histfile, TFile *mc_bg3_histfile, TFile *mc_bg4_histfile, TFile *mc_bg5_histfile){

  
  std::string varname = var;
  std::string pdfname = "Comp_DataMC_fitted/"+varname+"_fitted.pdf";
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


  Int_t lval = 1;
  Int_t rval = var_data->GetNbinsX();


  if (strncmp(var,"FourPions_M",11)==0) lval = var_data->GetXaxis()->FindBin(1.8);
  


  

  // Normalize by an Integral value
  var_mcsig->Scale(var_data->Integral(lval,rval)/var_mcsig->Integral(lval,rval));
  var_mcbg2->Scale(var_data->Integral(lval,rval)/var_mcbg2->Integral(lval,rval));
  var_mcbg3->Scale(var_data->Integral(lval,rval)/var_mcbg3->Integral(lval,rval));
  var_mcbg4->Scale(var_data->Integral(lval,rval)/var_mcbg4->Integral(lval,rval));
  var_mcbg5->Scale(var_data->Integral(lval,rval)/var_mcbg5->Integral(lval,rval));



  Int_t num = 5;
  Int_t NData = var_data->GetEntries();
  Double_t DataIntegral = var_data->Integral(lval,rval);
  Double_t binwidth = var_data->GetBinWidth(1);
  Double_t wt_mcsig,wt_mcbg2,wt_mcbg3,wt_mcbg4,wt_mcbg5;
  Double_t errwt_mcsig,errwt_mcbg2,errwt_mcbg3,errwt_mcbg4,errwt_mcbg5;
  Double_t totweight=0.;
  

  TObjArray *mc_array = new TObjArray(num);
  mc_array->Add(var_mcsig);
  mc_array->Add(var_mcbg2);
  mc_array->Add(var_mcbg3);
  mc_array->Add(var_mcbg4);
  mc_array->Add(var_mcbg5);
  

  
  TFractionFitter *combined_fit = new TFractionFitter(var_data,mc_array);
  combined_fit->SetRangeX(lval,rval);

  combined_fit->Constrain(0,0.7,1.);
  for (int i=1; i<num; i++)   combined_fit->Constrain(i,0.,0.3);


  
  gErrorIgnoreLevel = kError;
  
  Int_t status = combined_fit->Fit();
  std::cout << "Status " << var << ": " << status << std::endl;


  
    combined_fit->GetResult(0,wt_mcsig,errwt_mcsig);
    combined_fit->GetResult(1,wt_mcbg2,errwt_mcbg2);
    combined_fit->GetResult(2,wt_mcbg3,errwt_mcbg3);
    combined_fit->GetResult(3,wt_mcbg4,errwt_mcbg4);
    combined_fit->GetResult(4,wt_mcbg5,errwt_mcbg5);
    totweight = wt_mcsig+wt_mcbg2+wt_mcbg3+wt_mcbg4+wt_mcbg5;

    /*    
    wt_mcsig /= totweight;
    wt_mcbg2 /= totweight;
    wt_mcbg3 /= totweight;
    wt_mcbg4 /= totweight;
    wt_mcbg5 /= totweight;
    */

  

    //    if (std::abs(totweight-1.) > 0.1) {
    if (status >= 0) {                    


     printf("Fit result:\t+%.2f*[Sig]\n\t\t+%.2f*[LV(p)]\n\t\t+%.2f*[LV(ppi+pi-)]\n\t\t+%.2f*[LV(ppi+pi-pi-)]\n\t\t+%.2f*[LV(ppi+pi+pi-)]\nSum:\t\t%.3f\n",wt_mcsig,wt_mcbg2,wt_mcbg3,wt_mcbg4,wt_mcbg5,totweight);


    //    std::cout << "The fractions are " << wt_mcsig << ":" << wt_mcbg2 << ":" << wt_mcbg3 << ":" << wt_mcbg4 << ":" << wt_mcbg5 << endl;
    //    std::cout << "Binwidth = " << binwidth << ";\t" << "NData = " << NData << ";\t" << "DataIntegral = " << DataIntegral << endl;
    
    

    var_mcsig->Scale(wt_mcsig);
    var_mcbg2->Scale(wt_mcbg2);
    var_mcbg3->Scale(wt_mcbg3);
    var_mcbg4->Scale(wt_mcbg4);
    var_mcbg5->Scale(wt_mcbg5);


    
    TH1D *check = new TH1D("check","A staightforward sum with the weights",100,var_mcsig->GetXaxis()->GetXmin(),var_mcsig->GetXaxis()->GetXmax());
    check->Add(var_mcsig);
    check->Add(var_mcbg2);
    check->Add(var_mcbg3);
    check->Add(var_mcbg4);
    check->Add(var_mcbg5);
    
 
    TCanvas *canva = new TCanvas(var,var,15,10,400,300);
    gStyle->SetOptStat(0);
  

     TH1D* fitres = (TH1D*) combined_fit->GetPlot();

     if (status==0) var_data->SetTitle("Converged");
     else var_data->SetTitle("Has not converged");
     
     defstyle(var_data); var_data->GetXaxis()->SetTitle(var);
     defstyle(fitres); fitres->SetLineColor(kRed);
     defstyle(var_mcsig); var_mcsig->SetLineColor(kCyan); var_mcsig->SetFillStyle(3004); var_mcsig->SetFillColor(kCyan);
     defstyle(var_mcbg2); var_mcbg2->SetLineColor(kBlue+2); var_mcbg2->SetFillStyle(3005); var_mcbg2->SetFillColor(kBlue+2);
     defstyle(var_mcbg3); var_mcbg3->SetLineColor(kGreen+2); var_mcbg3->SetFillStyle(3004); var_mcbg3->SetFillColor(kGreen+2);
     defstyle(var_mcbg4); var_mcbg4->SetLineColor(kYellow-4); var_mcbg4->SetFillStyle(3001); var_mcbg4->SetFillColor(kYellow-4);
     defstyle(var_mcbg5); var_mcbg5->SetLineColor(kMagenta); var_mcbg5->SetFillStyle(3003); var_mcbg5->SetFillColor(kMagenta);
     defstyle(check); check->SetLineColor(kGray+2); check->SetFillStyle(0); check->SetFillColor(kGray);


     var_data->Draw("EP");
     var_mcsig->Draw("HIST SAME");
     var_mcbg2->Draw("HIST SAME");
     var_mcbg3->Draw("HIST SAME");
     var_mcbg4->Draw("HIST SAME");
     var_mcbg5->Draw("HIST SAME");
     fitres->Draw("HIST SAME");
     check->Draw("HIST SAME");
     
 
     TLegend* leg = new TLegend(posl, 0.55, posr, 0.9);
     leg->AddEntry(var_data, "Data","p");
     leg->AddEntry(fitres, "Full Fit","l");
     leg->AddEntry(check,  "Weighted sum","l");   
     leg->AddEntry(var_mcsig, "MC signal ph.sp.","l");
     leg->AddEntry(var_mcbg2, "LV(p) ph.sp.","l");
     leg->AddEntry(var_mcbg3, "LV(p#pi^{+}#pi^{-}) ph.sp.","l");
     leg->AddEntry(var_mcbg4, "LV(p#pi^{+}#pi^{-}#pi^{-}) ph.sp.","l");
     leg->AddEntry(var_mcbg5, "LV(p#pi^{+}#pi^{+}#pi^{-}) ph.sp.","l");
     leg->Draw("SAME");
     canva->Update();

     
     canva->SaveAs(pdfname.c_str(),"pdf");
     canva->Close();

  }

  
}



void fit_eachMCcomb()
{

  //Import the files with histograms made out of our data, MC signal and four MC backgrounds
  TFile *data_histfile = TFile::Open("histos_data_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_sig_histfile = TFile::Open("histos_mc_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_bg2_histfile = TFile::Open("histos_mc_bgcomb2_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_bg3_histfile = TFile::Open("histos_mc_bgcomb3_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_bg4_histfile = TFile::Open("histos_mc_bgcomb4_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_bg5_histfile = TFile::Open("histos_mc_bgcomb5_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");

  



  
  
  std::string variables[12] = {"Piminus1Proton_M","Piminus2Proton_M","ProtonPiplusUPiminus1_M","ProtonPiplusUPiminus2_M","ProtonPiplusLPiminus1_M","ProtonPiplusLPiminus2_M",
    "ProtonPiplusUPiminus1Piminus2_M","ProtonPiplusLPiminus1Piminus2_M","ProtonPiplusLPiplusUPiminus1_M","PiplusLPiplusUPiminus1_M","PiplusLPiplusUPiminus2_M","FourPions_M"};


   for (int i=11; i>-1; i--){
     const char *var = variables[i].c_str();
     fit_single(var,data_histfile,mc_sig_histfile,mc_bg2_histfile,mc_bg3_histfile,mc_bg4_histfile,mc_bg5_histfile);
   }


  
   data_histfile->Close();
   mc_sig_histfile->Close();
   mc_bg2_histfile->Close();
   mc_bg3_histfile->Close();
   mc_bg4_histfile->Close();
   mc_bg5_histfile->Close();  
  

}
