#include <cmath>
#include <cstdlib>
#include <cstring>


void setstyle(TH1D *dhisto, TH1D *mchisto, TLegend *leg)
{

    leg->AddEntry(dhisto,"  Before cut","l");
    leg->AddEntry(mchisto,"  After cut","l");

    Double_t height = 1.1*std::max(dhisto->GetMaximum(),mchisto->GetMaximum());//1.5
   //std:: cout << dhisto->GetMaximum() << "\t" << mchisto->GetMaximum() << "\t" << height << endl;

   dhisto->SetMaximum(height);
   mchisto->SetMaximum(height);


    
    dhisto->SetMarkerSize(0.5);
    dhisto->SetMarkerStyle(20);
    dhisto->SetTitleOffset(0.9,"X");
    dhisto->SetTitleSize(0.05,"X");
    dhisto->SetTitleOffset(1.2,"Y");
    dhisto->SetTitleSize(0.04,"Y"); 
    dhisto->SetLineColor(kBlack);
    dhisto->SetLineWidth(2);    

    mchisto->SetMarkerSize(0.5);
    mchisto->SetMarkerStyle(20);
    mchisto->SetMarkerColor(kCyan);
    mchisto->SetTitleOffset(0.9,"X");
    mchisto->SetTitleSize(0.05,"X");
    mchisto->SetTitleOffset(1.2,"Y");
    mchisto->SetTitleSize(0.04,"Y"); 
    mchisto->SetLineColor(kCyan);
    mchisto->SetLineWidth(2);    


    
}







void compare(const char *variable)
{

  TString output = "pic"+TString(variable)+".pdf";
  
   const char *filename0 = "histos_data_spring2017.root";
   const char *filename1 = "histos_data_spring2017_deltacut_1st.root";
   const char *filename2 = "histos_data_spring2017_deltacut_2nd.root";
   const char *filename3 = "histos_data_spring2017_deltacut_both.root";
  
   
   TFile *datafile = TFile::Open(filename0,"READ");
   TFile *mcfile = TFile::Open(filename1,"READ");
   if (!datafile || !mcfile) return;

   TH1D *var_data = (TH1D*)datafile->TFile::Get(variable);
   TH1D *var_mc = (TH1D*)mcfile->TFile::Get(variable);
   var_data->GetXaxis()->SetTitle(variable);



   

   ///Drawing   
   TLegend *leg = new TLegend(0.7,0.65,0.9,0.9); //0.74, 0.75
   setstyle(var_data,var_mc,leg);   
  
   
    gStyle->SetOptStat(0);
    TCanvas* canva = new TCanvas("c2","c2",15,10,400,300);
    //    gPad->SetLogy();
    //    var_data->SetMinimum(100.);
    var_data->Draw("P"); 
    var_mc->Draw("P SAME");
    leg->Draw("SAME");
    canva->Update();

    
    
    canva->Print(output,"pdf");
    

    //    datafile->Close();
    //    mcfile->Close();
    
}
