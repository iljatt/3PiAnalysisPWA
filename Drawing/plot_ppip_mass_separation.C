

void defstyle(TH1D *histo)
{

    histo->SetMarkerSize(0.5);
    histo->SetMarkerStyle(20);
    histo->SetTitleOffset(0.9,"X");
    histo->SetTitleSize(0.05,"X");
    histo->SetTitleOffset(1.2,"Y");
    histo->SetTitleSize(0.04,"Y"); 
    histo->SetLineColor(kBlack);
    histo->SetLineWidth(2);    
    
 }





void plot_ppip_mass_separation()
{

  gROOT->SetBatch(kTRUE);


  const char *filename = "histos_data_spring2017_precuts_diffpipluscut_fourpionscut_piminuscut.root";
  
   
   TFile *file = TFile::Open(filename,"READ");
   if (!file) return;
 
    TH1D *ppip1_m = (TH1D*)file->TFile::Get("Piplus1Proton_M");
    TH1D *ppip2_m = (TH1D*)file->TFile::Get("Piplus2Proton_M");
    TH1D *sum = new TH1D("sum","",100,0.8,3.8);
    
    sum->Add(ppip1_m);
    sum->Add(ppip2_m);

    
    gStyle->SetOptStat(0);

    defstyle(ppip1_m); ppip1_m->SetLineColor(kCyan);  ppip1_m->SetFillStyle(3004); ppip1_m->SetFillColor(kCyan); 
    defstyle(ppip2_m); ppip2_m->SetLineColor(kMagenta); ppip2_m->SetFillStyle(3005); ppip2_m->SetFillColor(kMagenta);

    defstyle(sum);
    sum->GetXaxis()->SetTitle("m(p#pi^{+})  [GeV]");
    sum->GetXaxis()->SetRangeUser(0.8,3.5);
    
    
    TLegend *leg = new TLegend(0.7,0.6,0.9,0.9); //0.74, 0.75
    leg->AddEntry(ppip1_m,"  m(p#pi^{+}_{fast}) ","lf");    
    leg->AddEntry(ppip2_m,"  m(p#pi^{+}_{slow}) ","lf");
    leg->AddEntry(sum,"  Sum ","lf");



    
    
    TCanvas* canva = new TCanvas("c1","c1",15,10,400,300);
    sum->Draw("HIST");
    ppip1_m->Draw("HIST SAME");
    ppip2_m->Draw("HIST SAME");
    leg->Draw("SAME");
    canva->Update();

    TLine *lrange = new TLine(1.15,0.,1.15,canva->GetUymax());
    TLine *rrange = new TLine(1.35,0.,1.35,canva->GetUymax());
    lrange->SetLineColor(kRed); lrange->SetLineWidth(3); lrange->SetLineStyle(kDashed);
    rrange->SetLineColor(kRed); rrange->SetLineWidth(3); rrange->SetLineStyle(kDashed);
    lrange->Draw("SAME");
    rrange->Draw("SAME");


    
    
    canva->Print("ppip_mass_sep.pdf", "pdf");
    canva->Close();


    file->Close();
    
}
