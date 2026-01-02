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


  Double_t Norm = data_hist->Integral(lfit,rfit);
  Int_t NData = data_hist->GetEntries();
  Double_t binwidth = data_hist->GetBinWidth(1);
  Double_t norm[num],weights[num],errweights[num],totweight=0.;


  

  //Save copies of the template histograms as local histograms (needed only for drawing separate contributions) 
  //Rescale local histograms and save them in a new array of histograms
  TObjArray *mc_hists = new TObjArray(num);
  TH1D *htemp[num];  
  for (int i=0; i<num; i++){
    htemp[i] = (TH1D*) template_hists->At(i);
    norm[i] = htemp[i]->Integral(lfit,rfit);
    htemp[i]->Scale(Norm/norm[i]);
    mc_hists->Add(htemp[i]);
  }



  //Configure the fit 
  TFractionFitter *combined_fit = new TFractionFitter(data_hist,mc_hists);
  for (int i=0; i<num; i++)   combined_fit->Constrain(i,0.,1.); 
  combined_fit->SetRangeX(lfit,rfit);

  
  gErrorIgnoreLevel = kError;

  
  Int_t status = combined_fit->Fit();
  std::cout << "Status: " << status << std::endl;
  //    std::cout << "Binwidth = " << binwidth << ";\t" << "NData = " << NData << ";\t" << "DataIntegral = " << Norm << endl;

  

  if (status >= 0) {                    
    
    TH1D *check_hist = new TH1D("check","A staightforward sum with the weights",100,data_hist->GetXaxis()->GetXmin(),data_hist->GetXaxis()->GetXmax());
    
    for (int i=0; i<num; i++){
      combined_fit->GetResult(i,weights[i],errweights[i]);
      
      //Multiply each contribution by a fraction
      htemp[i]->Scale(weights[i]);
      check_hist->Add(htemp[i]);
      totweight += weights[i];
    }
    

    
    printf("Fit result:\n");
    for (int i=0; i<num; i++)  printf("\t\t+%.2f*exp(-%.1ft)\t|\t%.3f\n",weights[i],slope[i],weights[i]*Norm/norm[i]);
    printf("Coeffs Sum:\t%.3f\n",totweight);
      
    

    

    //DRAWING STARTS HERE
    gStyle->SetOptStat(0);
    TCanvas *canva = new TCanvas(var,var,15,10,400,300);
    gPad->SetLogy(0);  

    
     TH1D* fitres = (TH1D*) combined_fit->GetPlot();

     //     if (status==0) data_hist->SetTitle("Binned ML fit converged");
     //     else data_hist->SetTitle("Binned ML fit has not converged");
     data_hist->SetTitle(title);
     
     
     defstyle(data_hist); data_hist->GetXaxis()->SetTitle("-t  [GeV^{2}]");
     defstyle(fitres); fitres->SetLineColor(kRed);
     defstyle(check_hist); check_hist->SetLineColor(kGray+2); check_hist->SetFillStyle(0); check_hist->SetFillColor(kGray);

     data_hist->Draw("EP");
     fitres->Draw("HIST SAME");
     check_hist->Draw("HIST SAME");
     canva->Update();

   
     //     TLegend* leg = new TLegend(0.1, 0.1, 0.3, 0.35);
        TLegend* leg = new TLegend(0.7, 0.5, 0.9, 0.9);
     leg->AddEntry(data_hist, "Data","p");
     leg->AddEntry(fitres, "Full Fit","l");
     leg->AddEntry(check_hist,  "Weighted sum","l");   


     //Draw all the contributions separately     
      for (int i=0; i<num; i++){
	if (weights[i]>=0.01){
	  defstyle(htemp[i]); htemp[i]->SetLineColor(11-i); htemp[i]->SetFillStyle(3002+i); htemp[i]->SetFillColor(11-i);  

	  if (i==1){ htemp[1]->SetLineColor(kAzure); htemp[1]->SetFillColor(kAzure);}

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
  Int_t lfit, rfit;
  
  
  //Import the files with histograms made out of our data, MC signal and four MC backgrounds
  TFile *data_histfile = TFile::Open("../HistosDataMC/histos_data_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut.root","READ");
  TFile *mc_histfile = TFile::Open("../MCphspSpring2017/Different_tslopes/tslope_histos.root","READ");

    
  TH1D *var_data =        (TH1D*) data_histfile->Get("minustvar");  
  TH1D *var_mc_slope14 =  (TH1D*) mc_histfile->Get("minustvar_tslope14");
  TH1D *var_mc_slope12 =  (TH1D*) mc_histfile->Get("minustvar_tslope12");
  TH1D *var_mc_slope10 =  (TH1D*) mc_histfile->Get("minustvar_tslope10");
  TH1D *var_mc_slope8 =   (TH1D*) mc_histfile->Get("minustvar_tslope8");
  TH1D *var_mc_slope6 =   (TH1D*) mc_histfile->Get("minustvar_tslope6");
  TH1D *var_mc_slope4 =   (TH1D*) mc_histfile->Get("minustvar_tslope4");
  TH1D *var_mc_slope3 =   (TH1D*) mc_histfile->Get("minustvar_tslope3");
  TH1D *var_mc_slope2 =   (TH1D*) mc_histfile->Get("minustvar_tslope2");
  TH1D *var_mc_slope1p5 = (TH1D*) mc_histfile->Get("minustvar_tslope1p5");
  TH1D *var_mc_slope1 =   (TH1D*) mc_histfile->Get("minustvar_tslope1");
   

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



    //Define the fit range and normalize templates by an integal value
    lfit = var_data->GetXaxis()->FindBin(0.);
    rfit = var_data->GetXaxis()->FindBin(2.0);
    const char *title = "";//ML binned fit in the [0,2] range";

     
    fit_templates(NUM_TEMPLATES,lfit,rfit,title,var_data,mc_array);

  
   data_histfile->Close();
   mc_histfile->Close();   

}
