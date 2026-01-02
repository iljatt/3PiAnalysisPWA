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





void draw_all(const Int_t num, const char *var, TH1D *data_hist, TObjArray *template_hists)
{
 
  std::string varname = var;
  std::string pdfname = "check_"+varname+".pdf";

  Double_t DataIntegral = data_hist->Integral();
  Int_t NData = data_hist->GetEntries();
  Double_t binwidth = data_hist->GetBinWidth(1);
  Double_t weights[num],errweights[num],totweight=0.;


  //Save copies of the template histograms locally
  // Normalize template histograms by an integral value
  TH1D *htemp[num];  
  TH1D *hdata = (TH1D*) data_hist;
  for (int i=0; i<num; i++){
    htemp[i] = (TH1D*) template_hists->At(i);
    htemp[i]->Scale(hdata->GetMaximum()/htemp[i]->GetMaximum());
  }
  //  hdata->Scale(1./);

  
    
    
    

    //DRAWING STARTS HERE
    gStyle->SetOptStat(0);
    TCanvas *canva = new TCanvas(var,var,15,10,400,300);
    gPad->SetLogy(0);  

        
     defstyle(data_hist); data_hist->GetXaxis()->SetTitle(var);   

     data_hist->Draw("EP");
     canva->Update();

     
     //     TLegend* leg = new TLegend(0.1, 0.1, 0.3, 0.6);
     TLegend* leg = new TLegend(0.7, 0.4, 0.9, 0.9);
     leg->AddEntry(data_hist, "Data","p");


     //Draw all the contributions separately     

     for (int i=0; i<3; i++){
       defstyle(htemp[i]); htemp[i]->SetLineColor(2+i); htemp[i]->SetFillStyle(0); htemp[i]->SetFillColor(2+i); //3004
       htemp[i]->Draw("HIST SAME");
      leg->AddEntry(htemp[i], Form("MC slope #%d",i),"l");
     }
     leg->Draw("SAME");
     canva->Update();

     
     canva->SaveAs(pdfname.c_str(),"pdf");
     canva->Close();

  }

  




void check_tslope_mcphsp()
{

  const Int_t NUM_TEMPLATES = 10;
  const char *var = "minustvar";
 

  
  //Import the files with histograms made out of our data, MC signal and four MC backgrounds
  TFile *data_histfile = TFile::Open("../HistosDataMC/histos_data_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope14_histfile = TFile::Open("../HistosDataMC/Different_tslopes/histos_mc_tslope14_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope12_histfile = TFile::Open("../HistosDataMC/Different_tslopes/histos_mc_tslope12_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope10_histfile = TFile::Open("../HistosDataMC/Different_tslopes/histos_mc_tslope10_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope8_histfile = TFile::Open("../HistosDataMC/Different_tslopes/histos_mc_tslope8_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope6_histfile = TFile::Open("../HistosDataMC/Different_tslopes/histos_mc_tslope6_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope4_histfile = TFile::Open("../HistosDataMC/Different_tslopes/histos_mc_tslope4_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope3_histfile = TFile::Open("../HistosDataMC/Different_tslopes/histos_mc_tslope3_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope2_histfile = TFile::Open("../HistosDataMC/Different_tslopes/histos_mc_tslope2_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope1p5_histfile = TFile::Open("../HistosDataMC/Different_tslopes/histos_mc_tslope1p5_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope1_histfile = TFile::Open("../HistosDataMC/Different_tslopes/histos_mc_tslope1_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");


    
  TH1D *var_data = (TH1D*) data_histfile->Get(var);  
  TH1D *var_mc_slope14 = (TH1D*) mc_slope14_histfile->Get(var);
  TH1D *var_mc_slope12 = (TH1D*) mc_slope12_histfile->Get(var);
  TH1D *var_mc_slope10 = (TH1D*) mc_slope10_histfile->Get(var);
  TH1D *var_mc_slope8 = (TH1D*) mc_slope8_histfile->Get(var);
  TH1D *var_mc_slope6 = (TH1D*) mc_slope6_histfile->Get(var);
  TH1D *var_mc_slope4 = (TH1D*) mc_slope4_histfile->Get(var);
  TH1D *var_mc_slope3 = (TH1D*) mc_slope3_histfile->Get(var);
  TH1D *var_mc_slope2 = (TH1D*) mc_slope2_histfile->Get(var);
  TH1D *var_mc_slope1p5 = (TH1D*) mc_slope1p5_histfile->Get(var);
  TH1D *var_mc_slope1 = (TH1D*) mc_slope1_histfile->Get(var);

  

   

  TObjArray *mc_array = new TObjArray(NUM_TEMPLATES);
  mc_array->Add(var_mc_slope14);
  mc_array->Add(var_mc_slope12);
  mc_array->Add(var_mc_slope10);
  mc_array->Add(var_mc_slope8);
  mc_array->Add(var_mc_slope6);
  mc_array->Add(var_mc_slope4);
  mc_array->Add(var_mc_slope3);
  mc_array->Add(var_mc_slope2);
  mc_array->Add(var_mc_slope1p5);
  mc_array->Add(var_mc_slope1);

  

  draw_all(NUM_TEMPLATES,var,var_data,mc_array);



  
   data_histfile->Close();
   mc_slope14_histfile->Close();
   mc_slope12_histfile->Close();
   mc_slope10_histfile->Close();
   mc_slope8_histfile->Close();
   mc_slope6_histfile->Close();  
   mc_slope4_histfile->Close();
   mc_slope3_histfile->Close();  
   mc_slope2_histfile->Close();  
   mc_slope1p5_histfile->Close();
   mc_slope1_histfile->Close();  
   

}
