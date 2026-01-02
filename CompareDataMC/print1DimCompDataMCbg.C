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




void setstyle(TH1D *dhisto, TH1D *mchisto, TH1D *mcbghisto, TLegend *leg)
{

    leg->AddEntry(dhisto,"  Data","p");
    leg->AddEntry(mchisto,"  MC phsp BW(#Delta)","l");
    //    leg->AddEntry(mcbghisto,"  MC bg#1","l");
    leg->AddEntry(mcbghisto,"  MC phsp","l");

    
    dhisto->Scale(1./dhisto->GetMaximum());
    mchisto->Scale(1./mchisto->GetMaximum());
    mcbghisto->Scale(1./mcbghisto->GetMaximum());

    
    Double_t height = 1.1*std::max(dhisto->GetMaximum(),mchisto->GetMaximum());//1.5
    dhisto->SetMaximum(height);
    mchisto->SetMaximum(height);
    mcbghisto->SetMaximum(height);
    
    
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
    mchisto->SetFillStyle(3016);
    mchisto->SetFillColor(kCyan);

    mcbghisto->SetMarkerSize(0.5);
    mcbghisto->SetMarkerStyle(20);
    mcbghisto->SetMarkerColor(kAzure);
    mcbghisto->SetTitleOffset(0.9,"X");
    mcbghisto->SetTitleSize(0.05,"X");
    mcbghisto->SetTitleOffset(1.2,"Y");
    mcbghisto->SetTitleSize(0.04,"Y"); 
    mcbghisto->SetLineColor(kAzure);
    mcbghisto->SetLineWidth(2);    
    mcbghisto->SetFillStyle(3020);
    mcbghisto->SetFillColor(kAzure);

    
}







void print1DimCompDataMCbg()
{

  const char *datafilename = "../HistosDataMC/histos_data_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root";
  const char *mcfilename = "../HistosDataMC/histos_mc_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root";
  const char *mcbgfilename = "../HistosDataMC/histos_mcweighted_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root";
  //const char *mcbgfilename = "histos_mc_bgcomb1_spring2017_precuts_diffpipluscut_piminuscut_deltacut.root";

  Double_t posl,posr;
  
   
   TFile *datafile = TFile::Open(datafilename,"READ");
   TFile *mcfile = TFile::Open(mcfilename,"READ");
   TFile *mcbgfile = TFile::Open(mcbgfilename,"READ");
   if (!datafile || !mcfile || !mcbgfilename) return;

   
   std::string variables[17] = {"minustvar","Piminus1Proton_M","Piminus2Proton_M","PiplusLPiplusUPiminus1_M","ProtonPiplusUPiminus1_M","ProtonPiplusUPiminus2_M","ProtonPiplusLPiminus1_M","ProtonPiplusLPiminus2_M","ProtonPiplusUPiminus1Piminus2_M","ProtonPiplusLPiminus1Piminus2_M","ProtonPiplusLPiplusUPiminus1_M","ProtonPiplusLPiplusUPiminus2_M","PiplusL_P","FourPions_M","DeltaPhi_LV_UV","DeltaTheta_LV_UV","Miss_M2"};
   std::string outpdf;

   
   TH1D *var_data = new TH1D();
   TH1D *var_mc = new TH1D();
   TH1D *var_mcbg = new TH1D();



   
   for (int i=0; i<17; i++){
   
     const char *title = variables[i].c_str();      
     outpdf = "Comparison_DataMC_weighted/pic_"+variables[i]+".pdf";

     
     var_data = (TH1D*)datafile->TFile::Get(title);
     var_mc = (TH1D*)mcfile->TFile::Get(title);
     var_mcbg = (TH1D*)mcbgfile->TFile::Get(title);
     var_data->GetXaxis()->SetTitle(title);

     
     ///Drawing   
     if (strncmp(title,"ProtonPiplusUPiminus1Piminus2_M",31)==0 || strncmp(title,"ProtonPiplusLPiminus1Piminus2_M",31)==0 ||
	 strncmp(title,"ProtonPiplusUPiminus1_M",23)==0 || strncmp(title,"ProtonPiplusLPiplusUPiminus1_M",30)==0 || strncmp(title,"FourPions_M",11)==0){
       posl = 0.1;
       posr = 0.35;
     }
     else {
       posl = 0.65;
       posr = 0.9;
     }	 
     
     

     TLegend *leg = new TLegend(posl,0.7,posr,0.9); //0.74, 0.75
     setstyle(var_data,var_mc,var_mcbg,leg);      
     gStyle->SetOptStat(0);
     
     TCanvas* canva = new TCanvas(title,title,15,10,400,300);
     //    gPad->SetLogy();
     //    var_data->SetMinimum(100.);
     var_data->Draw("P"); 
     var_mc->Draw("HIST F SAME");
     var_mcbg->Draw("HIST F SAME");
     leg->Draw("SAME");

     
     canva->Update();
     canva->Print(outpdf.c_str(),"pdf");    
     canva->Close();
     
}

    datafile->Close();
    mcfile->Close();
    
}
