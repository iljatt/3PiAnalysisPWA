#include <cmath>
#include <cstdlib>
#include <cstring>





void printListOfKeys(TFile *file)
{
 TList:TObjArray *keylist = (TObjArray*) file->TFile::GetListOfKeys();
  keylist->SetOwner(kFALSE);
  keylist->Sort();
   for(int ii = 0; ii < keylist->GetEntries(); ii++)
     cout << "KEY #"<< ii << "\t\t" << keylist->At(ii)->GetTitle() << endl;
}


void loopdir() {
   TFile *f1 = TFile::Open("hsimple.root");
   TIter keyList(f1->GetListOfKeys());
   TKey *key;
   TCanvas c1;
   c1.Print("hsimple.ps[");
   while ((key = (TKey*)keyList())) {
      TClass *cl = gROOT->GetClass(key->GetClassName());
      if (!cl->InheritsFrom("TH1")) continue;
      TH1 *h = (TH1*)key->ReadObj();
      h->Draw();
      c1.Print("hsimple.ps");
   }
   c1.Print("hsimple.ps]");
}


void setstyle(TH1D *dhisto, TH1D *dhistocut, TH1D *dhistocut2nd,TH1D *dhistocut3rd)
{
    
    
    dhisto->SetMarkerSize(0.4);
    dhisto->SetMarkerStyle(20);
    dhisto->SetMarkerColor(kGray+2);
    dhisto->SetTitleOffset(0.9,"X");
    dhisto->SetTitleSize(0.06,"X");
    dhisto->SetTitleOffset(1.2,"Y");
    dhisto->SetTitleSize(0.04,"Y"); 
    dhisto->SetLineColor(kGray);
    dhisto->SetLineWidth(2);    
    dhisto->SetFillColor(kGray);
    dhisto->SetFillStyle(3004);

    
    
    dhistocut->SetMarkerSize(0.4);
    dhistocut->SetMarkerStyle(20);
    dhistocut->SetMarkerColor(kBlue+2);
    dhistocut->SetTitleOffset(1.0,"X");
    dhistocut->SetTitleSize(0.05,"X");
    dhistocut->SetTitleOffset(1.2,"Y");
    dhistocut->SetTitleSize(0.04,"Y"); 
    dhistocut->SetLineColor(kBlue+2);
    dhistocut->SetLineWidth(2);    
    dhistocut->SetFillColor(kBlue-7);
    dhistocut->SetFillStyle(3016);

    
    dhistocut2nd->SetMarkerSize(0.4);
    dhistocut2nd->SetMarkerStyle(20);
    dhistocut2nd->SetMarkerColor(kGray+1);
    dhistocut2nd->SetTitleOffset(1.0,"X");
    dhistocut2nd->SetTitleSize(0.05,"X");
    dhistocut2nd->SetTitleOffset(1.2,"Y");
    dhistocut2nd->SetTitleSize(0.04,"Y"); 
    dhistocut2nd->SetLineColor(kGray);
    dhistocut2nd->SetLineWidth(2);    
    dhistocut2nd->SetFillColor(kGray);
    dhistocut2nd->SetFillStyle(3005);


    dhistocut3rd->SetMarkerSize(0.4);
    dhistocut3rd->SetMarkerStyle(20);
    dhistocut3rd->SetMarkerColor(kRed+2);
    dhistocut3rd->SetTitleOffset(1.0,"X");
    dhistocut3rd->SetTitleSize(0.05,"X");
    dhistocut3rd->SetTitleOffset(1.2,"Y");
    dhistocut3rd->SetTitleSize(0.04,"Y"); 
    dhistocut3rd->SetLineColor(kRed+2);
    dhistocut3rd->SetLineWidth(2);    
    dhistocut3rd->SetFillColor(kRed-7);
    dhistocut3rd->SetFillStyle(3016);
    
    
}







void plotBothPz()
{


  const char *data1filename = "histos_data_spring2017_precuts_diffpipluscut_deltacut_fourpionscut.root";
  const char *data2filename = "histos_data_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut.root";


  
  Double_t posl,posr;

  
   
   TFile *data1file = TFile::Open(data1filename,"READ");
   TFile *data2file = TFile::Open(data2filename,"READ");
   if (!data1file || !data2file) return;

  
   
   TH1D *var_data1 = new TH1D();
   TH1D *var_data1cut = new TH1D();
   TH1D *var_data2 = new TH1D();
   TH1D *var_data2cut = new TH1D();


 
   var_data2 = (TH1D*)data1file->TFile::Get("Piminus2_Pz");
   var_data2cut = (TH1D*)data2file->TFile::Get("Piminus2_Pz");
   var_data1 = (TH1D*)data1file->TFile::Get("Piminus1_Pz");
   var_data1cut = (TH1D*)data2file->TFile::Get("Piminus1_Pz");
 

     //write latex-style captions to the axis     
     var_data2->GetXaxis()->SetTitle("p_{z}  [GeV]");
     var_data2->GetXaxis()->SetRangeUser(0.,7.);     



     TLegend *leg = new TLegend(0.6,0.5,0.9,0.9); //0.74, 0.75
     leg->AddEntry(var_data2,"  Slow #pi^{-}, full range ","pf");
     leg->AddEntry(var_data2cut,"  Slow #pi^{-}, after cut ","pf");
     leg->AddEntry(var_data1,"  Fast #pi^{-}, full range ","pf");
     leg->AddEntry(var_data1cut,"  Fast #pi^{-}, after cut ","pf");

     
     setstyle(var_data2,var_data2cut,var_data1,var_data1cut);      
     gStyle->SetOptStat(0);


     
     TCanvas* canva = new TCanvas("c","c",15,10,400,300);
     //    gPad->SetLogy();
     //    var_data->SetMinimum(100.);
     gPad->SetBottomMargin(0.12);
     var_data2->Draw("*H"); 
     var_data2cut->Draw("*H F SAME");
     var_data1->Draw("*H F SAME");
     var_data1cut->Draw("*H F SAME");
     leg->Draw("SAME");
     
     canva->Update();
     canva->Print("PiminusBothPz.pdf","pdf");    
     canva->Close();



    data1file->Close();
    data2file->Close();
    
}
