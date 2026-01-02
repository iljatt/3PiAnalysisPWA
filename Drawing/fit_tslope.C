void setstyle(TH1D *histo, TF1 *fithisto, TPaveText *ptw)
{

    histo->SetMarkerSize(0.5);
    histo->SetMarkerStyle(20);
    histo->SetTitleOffset(0.9,"X");
    histo->SetTitleSize(0.05,"X");
    histo->SetTitleOffset(1.2,"Y");
    histo->SetTitleSize(0.04,"Y"); 
    histo->SetLineColor(kBlue);
    histo->SetMarkerColor(kBlue);
    histo->SetLineWidth(1);    

    histo->GetYaxis()->SetTitle("");//Events/ 0.02 [GeV^{2}]");
    histo->GetXaxis()->SetTitle("-Q^{2}  [GeV^{2}]");

    fithisto->SetLineColor(kRed);
    fithisto->SetLineWidth(3);

    ptw->SetTextSize(0.04);
    ptw->SetBorderSize(0);
    ptw->SetBorderSize(2);
    ptw->SetFillColor(kGray);
      
 }




void fit_tslope(double lval, double rval)
{

  const char *filename = "histos_data_spring2017.root";
  char *pavetext = new char[20];

  
   TFile *file = TFile::Open(filename,"READ");
   if (!file) return;


    TH1D *tslope = (TH1D*)file->TFile::Get("minustvar2");

    Double_t maxpoint=tslope->GetXaxis()->GetBinCenter(tslope->GetMaximumBin());



    TF1 *fit_tslope = new TF1("h02","[0]*exp(-[1]*x)",lval,rval);
    tslope->Fit(fit_tslope,"R");

    

    sprintf(pavetext,"~ exp(-%.2f*x)",fit_tslope->GetParameter(1));


    
    // Drawing in the rest of this macro 

    TPaveText *ptw = new TPaveText(0.7,0.65,0.89,0.75,"NDC");
    ptw->AddText("Bin width = 0.02\n");
    ptw->AddText(pavetext);
    setstyle(tslope,fit_tslope,ptw);
    gStyle->SetOptStat(0);


    
    TCanvas* canva = new TCanvas("c2","c2",15,10,400,300);
    tslope->Draw("HIST");
    fit_tslope->Draw("SAME");  
    ptw->Draw("SAME");


    
    canva->Print("tslope_fit.pdf", "pdf");
    canva->Close();
    
      

    file->Close();

}
