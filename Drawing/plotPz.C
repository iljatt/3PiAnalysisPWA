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
    dhisto->SetFillStyle(3016);

    
    
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
    dhistocut2nd->SetMarkerColor(kRed+1);
    dhistocut2nd->SetTitleOffset(1.0,"X");
    dhistocut2nd->SetTitleSize(0.05,"X");
    dhistocut2nd->SetTitleOffset(1.2,"Y");
    dhistocut2nd->SetTitleSize(0.04,"Y"); 
    dhistocut2nd->SetLineColor(kRed+1);
    dhistocut2nd->SetLineWidth(2);    
    dhistocut2nd->SetFillColor(kRed-9);
    dhistocut2nd->SetFillStyle(3015);


    dhistocut3rd->SetMarkerSize(0.4);
    dhistocut3rd->SetMarkerStyle(20);
    dhistocut3rd->SetMarkerColor(kRed);
    dhistocut3rd->SetTitleOffset(1.0,"X");
    dhistocut3rd->SetTitleSize(0.05,"X");
    dhistocut3rd->SetTitleOffset(1.2,"Y");
    dhistocut3rd->SetTitleSize(0.04,"Y"); 
    dhistocut3rd->SetLineColor(kRed);
    dhistocut3rd->SetLineWidth(2);    
    //    dhistocut3rd->SetFillColor(kRed-7);
    //    dhistocut3rd->SetFillStyle(3005);
    
    
}







void plotPz()
{


  const char *data1filename = "histos_data_spring2017_precuts_diffpipluscut_deltacut_fourpionscut.root";
  const char *data2filename = "histos_data_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut.root";
  const char *data3filename = "histos_data_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_comcut0.root";
  const char *data4filename = "histos_data_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_comcut0p1.root";


  
  Double_t posl,posr;

  
   
   TFile *data1file = TFile::Open(data1filename,"READ");
   TFile *data2file = TFile::Open(data2filename,"READ");
   TFile *data3file = TFile::Open(data3filename,"READ");
   TFile *data4file = TFile::Open(data4filename,"READ");
   if (!data1file || !data2file || !data3file) return;

   
   std::string variables[1] = {"Piminus2_Pz"};
   std::string outpdf;

   
   TH1D *var_data = new TH1D();
   TH1D *var_datacut = new TH1D();
   TH1D *var_datacut2nd = new TH1D();
   TH1D *var_datacut3rd = new TH1D();

  
   for (int i=0; i<1; i++){
   
     const char *title = variables[i].c_str();      
     outpdf = variables[i]+".pdf";

 
     var_data = (TH1D*)data1file->TFile::Get(title);
     var_datacut = (TH1D*)data2file->TFile::Get(title);
     var_datacut2nd = (TH1D*)data3file->TFile::Get(title);
     var_datacut3rd = (TH1D*)data4file->TFile::Get(title);
 

     //write latex-style captions to the axis     
     if (variables[i]=="Piminus2_Pz") {
       var_data->GetXaxis()->SetTitle("p_{z}(#pi^{-}_{slow})  [GeV]");
       var_data->GetXaxis()->SetRangeUser(0.,6.);
     
     }     


     TLegend *leg = new TLegend(0.65,0.6,0.9,0.9); //0.74, 0.75
     leg->AddEntry(var_data,"  Full range in p_{z} ","pf");
     leg->AddEntry(var_datacut,"  p_{z} #geq 700 MeV ","pf");
     leg->AddEntry(var_datacut2nd,"  p_{z}^{[c.o.m.]} #geq 0 MeV ","pf");
     
     setstyle(var_data,var_datacut,var_datacut2nd,var_datacut3rd);      
     gStyle->SetOptStat(0);


     
     TCanvas* canva = new TCanvas(title,title,15,10,400,300);
     //    gPad->SetLogy();
     //    var_data->SetMinimum(100.);
     gPad->SetBottomMargin(0.12);
     var_data->Draw("*H"); 
     var_datacut->Draw("*H F SAME");
     var_datacut2nd->Draw("*H F SAME");
     //     var_datacut3rd->Draw("*H F SAME");
     leg->Draw("SAME");
     //     gfill->Draw("same,lf");   


     
     canva->Update();
     canva->Print(outpdf.c_str(),"pdf");    
     canva->Close();
   }     


    data1file->Close();
    data2file->Close();
    data3file->Close();
    data4file->Close();
    
}
