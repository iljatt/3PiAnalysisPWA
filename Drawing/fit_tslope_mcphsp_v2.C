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





void fit_templates(const Int_t num, Int_t lfit, Int_t rfit, const char *title, TH1D *data_hist, TObjArray *template_hists)
{
 
  const char *var = "minustvar";
  std::string pdfname = "minustvar_fitted.pdf";
  
  Double_t slope[10]= {14.,12.,10.,8.,6.,4.,3.,2.,1.5,1.};
    
  
  Double_t DataIntegral = data_hist->Integral();
  Int_t NData = data_hist->GetEntries();
  Double_t binwidth = data_hist->GetBinWidth(1);
  Double_t weights[num],errweights[num],totweight=0.;


  //Save copies of the template histograms as local histograms (needed only for drawing separate contributions)
  TH1D *htemp[num];  
  for (int i=0; i<num; i++) htemp[i] = (TH1D*) template_hists->At(i);


  
  TFractionFitter *combined_fit = new TFractionFitter(data_hist,template_hists);
  for (int i=0; i<num; i++)
    combined_fit->Constrain(i,0.,1.e+5); 
   combined_fit->SetRangeX(lfit,rfit);

  
  gErrorIgnoreLevel = kError;

  
  Int_t status = combined_fit->Fit();
  std::cout << "Status: " << status << std::endl;
  //    std::cout << "Binwidth = " << binwidth << ";\t" << "NData = " << NData << ";\t" << "DataIntegral = " << DataIntegral << endl;

  

  if (status >= 0) {                    
    
    TH1D *check_hist = new TH1D("check","A staightforward sum with the weights",100,data_hist->GetXaxis()->GetXmin(),data_hist->GetXaxis()->GetXmax());
    
    for (int i=0; i<num; i++){
      combined_fit->GetResult(i,weights[i],errweights[i]);
      
      //Multiply each contribution by a fraction
      htemp[i]->Scale(weights[i]);
      check_hist->Add(htemp[i]);
      totweight += weights[i];
    }
    //    check_hist->Scale(data_hist->GetMaximum()/check_hist->GetMaximum());

    
    printf("Fit result:\t+%.2f*exp(-14t)\n\t\t+%.2f*exp(-12t)\n\t\t+%.2f*exp(-10t)\n\t\t+%.2f*exp(-8t)\n\t\t+%.2f*exp(-6t)\n\t\t+%.2f*exp(-4t)\n\t\t+%.2f*exp(-3t)\n\t\t+%.2f*exp(-2t)\n\t\t+%.2f*exp(-1.5t)\n\t\t+%.2f*exp(-1t)\nCoeffs Sum:\t%.3f\n",weights[0],weights[1],weights[2],weights[3],weights[4],weights[5],weights[6],weights[7],weights[8],weights[9],totweight);

    
    

    //DRAWING STARTS HERE
    gStyle->SetOptStat(0);
    TCanvas *canva = new TCanvas(var,var,15,10,400,300);
    gPad->SetLogy(0);  

    
     TH1D* fitres = (TH1D*) combined_fit->GetPlot();

     //     if (status==0) data_hist->SetTitle("Binned ML fit converged");
     //     else data_hist->SetTitle("Binned ML fit has not converged");
     data_hist->SetTitle(title);
     
     
     defstyle(data_hist); data_hist->GetXaxis()->SetTitle(var);
     defstyle(fitres); fitres->SetLineColor(kRed);
     defstyle(check_hist); check_hist->SetLineColor(kGray+2); check_hist->SetFillStyle(0); check_hist->SetFillColor(kGray);

     data_hist->Draw("EP");
     fitres->Draw("HIST SAME");
     check_hist->Draw("HIST SAME");
     canva->Update();

   
     //     TLegend* leg = new TLegend(0.1, 0.1, 0.3, 0.35);
        TLegend* leg = new TLegend(0.7, 0.6, 0.9, 0.9);
     leg->AddEntry(data_hist, "Data","p");
     leg->AddEntry(fitres, "Full Fit","l");
     leg->AddEntry(check_hist,  "Weighted sum","l");   


     //Draw all the contributions separately     
      for (int i=0; i<num; i++){
	if (weights[i]>=0.01){
	  defstyle(htemp[i]); htemp[i]->SetLineColor(11-i); htemp[i]->SetFillStyle(3002+i); htemp[i]->SetFillColor(11-i);  
	  htemp[i]->Draw("HIST SAME");
	  leg->AddEntry(htemp[i], Form("MC %.2f*Exp(-%.1f t)",weights[i],slope[i]),"l");
	}
	}
 
     leg->Draw("SAME");
     canva->Update();

     
     canva->SaveAs(pdfname.c_str(),"pdf");
     canva->Close();

  }

  
}



void fit_tslope_mcphsp()
{

  const Int_t NUM_TEMPLATES = 10;
  const char *var = "minustvar";
  Int_t lfit, rfit;
  
  
  //Import the files with histograms made out of our data, MC signal and four MC backgrounds
  TFile *data_histfile = TFile::Open("histos_data_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope14_histfile = TFile::Open("Different_tslopes/histos_mc_tslope14_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope12_histfile = TFile::Open("Different_tslopes/histos_mc_tslope12_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope10_histfile = TFile::Open("Different_tslopes/histos_mc_tslope10_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope8_histfile = TFile::Open("Different_tslopes/histos_mc_tslope8_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope6_histfile = TFile::Open("Different_tslopes/histos_mc_tslope6_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope4_histfile = TFile::Open("Different_tslopes/histos_mc_tslope4_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope3_histfile = TFile::Open("Different_tslopes/histos_mc_tslope3_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope2_histfile = TFile::Open("Different_tslopes/histos_mc_tslope2_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope1p5_histfile = TFile::Open("Different_tslopes/histos_mc_tslope1p5_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");
  TFile *mc_slope1_histfile = TFile::Open("Different_tslopes/histos_mc_tslope1_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root","READ");




    
  TH1D *var_data = (TH1D*) data_histfile->Get(var);  
  TH1D *var_mc_slope10 = (TH1D*) mc_slope10_histfile->Get(var);
  TH1D *var_mc_slope6 = (TH1D*) mc_slope6_histfile->Get(var);
  TH1D *var_mc_slope4 = (TH1D*) mc_slope4_histfile->Get(var);
  TH1D *var_mc_slope1 = (TH1D*) mc_slope1_histfile->Get(var);
  TH1D *var_mc_slope14 = (TH1D*) mc_slope14_histfile->Get(var);
  TH1D *var_mc_slope12 = (TH1D*) mc_slope12_histfile->Get(var);
  TH1D *var_mc_slope8 = (TH1D*) mc_slope8_histfile->Get(var);
  TH1D *var_mc_slope3 = (TH1D*) mc_slope3_histfile->Get(var);
  TH1D *var_mc_slope2 = (TH1D*) mc_slope2_histfile->Get(var);
  TH1D *var_mc_slope1p5 = (TH1D*) mc_slope1p5_histfile->Get(var);



  //Define the fit range and normalize templates by an integal value
  lfit = var_data->GetXaxis()->FindBin(0.1);
  rfit = var_data->GetXaxis()->FindBin(2.);
  const char *title = "ML bin fit in the [0.1,2] range";



  var_data->Scale(1./var_data->Integral(lfit,rfit));
  var_mc_slope14->Scale(1./var_mc_slope14->Integral(lfit,rfit));
  var_mc_slope12->Scale(1./var_mc_slope12->Integral(lfit,rfit));
  var_mc_slope10->Scale(1./var_mc_slope10->Integral(lfit,rfit));
  var_mc_slope8->Scale(1./var_mc_slope8->Integral(lfit,rfit));
  var_mc_slope6->Scale(1./var_mc_slope6->Integral(lfit,rfit));
  var_mc_slope4->Scale(1./var_mc_slope4->Integral(lfit,rfit));
  var_mc_slope3->Scale(1./var_mc_slope3->Integral(lfit,rfit));
  var_mc_slope2->Scale(1./var_mc_slope2->Integral(lfit,rfit));
  var_mc_slope1p5->Scale(1./var_mc_slope1p5->Integral(lfit,rfit));
  var_mc_slope1->Scale(1./var_mc_slope1->Integral(lfit,rfit));

     
   

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




    fit_templates(NUM_TEMPLATES,lfit,rfit,title,var_data,mc_array);



  
   data_histfile->Close();
   mc_slope10_histfile->Close();
   mc_slope6_histfile->Close();  
   mc_slope4_histfile->Close();
   mc_slope1_histfile->Close();  
   mc_slope14_histfile->Close();
   mc_slope12_histfile->Close();
   mc_slope8_histfile->Close();
   mc_slope3_histfile->Close();  
   mc_slope2_histfile->Close();  
   mc_slope1p5_histfile->Close();
 

   

}
