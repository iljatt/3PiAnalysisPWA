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




void setstyle(TH1D *dhisto, TH1D *mchisto, TH1D *mcbg2histo, TH1D *mcbg3histo, TH1D *mcbg4histo, TH1D *mcbg5histo, TLegend *leg)
{

    leg->AddEntry(dhisto,"  Data","p");
    leg->AddEntry(mchisto,"  MC sig LV(p#pi^{+})","l");
    leg->AddEntry(mcbg2histo,"  MC bg LV(p)","l");
    leg->AddEntry(mcbg3histo,"  MC bg LV(p#pi^{+}#pi^{-})","l");
    leg->AddEntry(mcbg4histo,"  MC bg LV(p#pi^{+}#pi^{-}#pi^{-})","l");
    leg->AddEntry(mcbg5histo,"  MC bg LV(p#pi^{+}#pi^{+}#pi^{-})","l");

    
    dhisto->Scale(1./dhisto->GetMaximum());
    mchisto->Scale(1./mchisto->GetMaximum());
    mcbg2histo->Scale(1./mcbg2histo->GetMaximum());
    mcbg3histo->Scale(1./mcbg3histo->GetMaximum());
    mcbg4histo->Scale(1./mcbg4histo->GetMaximum());
    mcbg5histo->Scale(1./mcbg5histo->GetMaximum());

    
    Double_t height = 1.1*std::max(dhisto->GetMaximum(),mchisto->GetMaximum());//1.5
    dhisto->SetMaximum(height);
    mchisto->SetMaximum(height);
    //    mcbg2histo->SetMaximum(height);
    
    
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

    
    mcbg2histo->SetMarkerSize(0.5);
    mcbg2histo->SetMarkerStyle(20);
    mcbg2histo->SetMarkerColor(kAzure);
    mcbg2histo->SetLineColor(kAzure);
    mcbg2histo->SetLineWidth(2);    
    //    mcbg2histo->SetFillStyle(3020);
    //    mcbg2histo->SetFillColor(kAzure);

    mcbg3histo->SetMarkerSize(0.5);
    mcbg3histo->SetMarkerStyle(20);
    mcbg3histo->SetMarkerColor(kGreen+2);
    mcbg3histo->SetLineColor(kGreen+2);
    mcbg3histo->SetLineWidth(2);    
    //    mcbg3histo->SetFillStyle(3004);
    //    mcbg3histo->SetFillColor(kGreen+2);

    mcbg4histo->SetMarkerSize(0.5);
    mcbg4histo->SetMarkerStyle(20);
    mcbg4histo->SetMarkerColor(kOrange);
    mcbg4histo->SetLineColor(kOrange);
    mcbg4histo->SetLineWidth(2);    
    //    mcbg4histo->SetFillStyle(3022);
    //    mcbg4histo->SetFillColor(kOrange);

    mcbg5histo->SetMarkerSize(0.5);
    mcbg5histo->SetMarkerStyle(20);
    mcbg5histo->SetMarkerColor(kMagenta+2);
    mcbg5histo->SetLineColor(kMagenta+2);
    mcbg5histo->SetLineWidth(2);    
    //    mcbg5histo->SetFillStyle(3005);
    //    mcbg5histo->SetFillColor(kMagenta+2);
    
    
}







void print1DimCompDataMCbgs()
{

  const char *datafilename = "histos_data_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut1p0.root";
  const char *mcfilename = "histos_mcw_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut1p0.root";
  const char *mcbg2filename = "CombBGs/histos_mc_bgcomb2_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut1p0.root";
  const char *mcbg3filename = "CombBGs/histos_mc_bgcomb3_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut1p0.root";
  const char *mcbg4filename = "CombBGs/histos_mc_bgcomb4_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut1p0.root";
  const char *mcbg5filename = "CombBGs/histos_mc_bgcomb5_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut1p0.root";

  Double_t posl,posr;
  
   
   TFile *datafile = TFile::Open(datafilename,"READ");
   TFile *mcfile = TFile::Open(mcfilename,"READ");
   TFile *mcbg2file = TFile::Open(mcbg2filename,"READ");
   TFile *mcbg3file = TFile::Open(mcbg3filename,"READ");
   TFile *mcbg4file = TFile::Open(mcbg4filename,"READ");
   TFile *mcbg5file = TFile::Open(mcbg5filename,"READ");
   
   if (!datafile || !mcfile || !mcbg2filename || !mcbg3filename || !mcbg4filename || !mcbg5filename) return;

     std::string variables[21] = {"minustvar","Piminus1Proton_M","Piminus2Proton_M","PiplusLPiplusUPiminus1_M","PiplusLPiplusUPiminus2_M","ProtonPiplusUPiminus1_M","ProtonPiplusUPiminus2_M","ProtonPiplusLPiminus1_M","ProtonPiplusLPiminus2_M","ProtonPiplusUPiminus1Piminus2_M","ProtonPiplusLPiminus1Piminus2_M","ProtonPiplusLPiplusUPiminus1_M","ProtonPiplusLPiplusUPiminus2_M","FourPions_M","DeltaPhi_LV_UV","DeltaTheta_LV_UV","Miss_M2","PiplusL_P","Piminus2_Pz","PiplusLPiminus1_M","PiplusLPiminus2_M"};
   std::string outpdf;

   
   TH1D *var_data = new TH1D();
   TH1D *var_mc = new TH1D();
   TH1D *var_mcbg2 = new TH1D();
   TH1D *var_mcbg3 = new TH1D();
   TH1D *var_mcbg4 = new TH1D();
   TH1D *var_mcbg5 = new TH1D();

   TH1D *check = new TH1D();
  
   for (int i=0; i<21; i++){
   
     const char *title = variables[i].c_str();      
     outpdf = "Comp_DataMCbgs/pic_"+variables[i]+".pdf";

 
     var_data = (TH1D*)datafile->TFile::Get(title);
     var_mc = (TH1D*)mcfile->TFile::Get(title);
     var_mcbg2 = (TH1D*)mcbg2file->TFile::Get(title);
     var_mcbg3 = (TH1D*)mcbg3file->TFile::Get(title);
     var_mcbg4 = (TH1D*)mcbg4file->TFile::Get(title);
     var_mcbg5 = (TH1D*)mcbg5file->TFile::Get(title);

     

     //write latex-style captions to the axis     
     if (variables[i]=="minustvar")                var_mc->GetXaxis()->SetTitle("-t  [GeV^{2}]");
     if (variables[i]=="Piminus1Proton_M")         var_mc->GetXaxis()->SetTitle("m(p#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="Piminus2Proton_M")         var_mc->GetXaxis()->SetTitle("m(p#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="PiplusLPiplusUPiminus1_M") var_mc->GetXaxis()->SetTitle("m(#pi^{+}#pi^{+}#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="PiplusLPiplusUPiminus2_M")  var_mc->GetXaxis()->SetTitle("m(#pi^{+}#pi^{+}#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="ProtonPiplusUPiminus1_M")  var_mc->GetXaxis()->SetTitle("m(p#pi^{+}(uv)#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="ProtonPiplusUPiminus2_M")  var_mc->GetXaxis()->SetTitle("m(p#pi^{+}(uv)#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiminus1_M")  var_mc->GetXaxis()->SetTitle("m(p#pi^{+}(lv)#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiminus2_M")   var_mc->GetXaxis()->SetTitle("m(p#pi^{+}(lv)#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="ProtonPiplusUPiminus1Piminus2_M") var_mc->GetXaxis()->SetTitle("m(p#pi^{+}(uv)#pi^{-}#pi^{-})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiminus1Piminus2_M") var_mc->GetXaxis()->SetTitle("m(p#pi^{+}(lv)#pi^{-}#pi^{-})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiplusUPiminus1_M")  var_mc->GetXaxis()->SetTitle("m(p#pi^{+}#pi^{+}#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiplusUPiminus2_M")  var_mc->GetXaxis()->SetTitle("m(p#pi^{+}#pi^{+}#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="FourPions_M"){                    var_mc->GetXaxis()->SetTitle("m(4#pi)  [GeV]");
                                                          var_mc->GetXaxis()->SetRangeUser(1.5,3.5);
                                      }
     if (variables[i]=="PiplusLPiminus1_M")      var_mc->GetXaxis()->SetTitle("m(#pi^{+}(lv)#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="PiplusLPiminus2_M")      var_mc->GetXaxis()->SetTitle("m(#pi^{+}(lv)#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="DeltaPhi_LV_UV")         var_mc->GetXaxis()->SetTitle("#Delta#phi(lv-uv)  [deg]");
     if (variables[i]=="DeltaTheta_LV_UV")       var_mc->GetXaxis()->SetTitle("#Delta#Theta(lv-uv)  [deg]");
     if (variables[i]=="Miss_M2")                var_mc->GetXaxis()->SetTitle("m_{miss}^{2}  [GeV^{2}]");
     if (variables[i]=="PiplusL_P")              var_mc->GetXaxis()->SetTitle("|p(#pi^{+}(lv))|  [GeV]");
     if (variables[i]=="Piminus2_Pz")            var_mc->GetXaxis()->SetTitle("p_{z}(#pi^{-}_{slow})  [GeV]");
        
 


     ///Drawing   
     if (strncmp(title,"ProtonPiplusUPiminus1Piminus2_M",31)==0 || strncmp(title,"ProtonPiplusLPiminus1Piminus2_M",31)==0 ||
	 strncmp(title,"ProtonPiplusUPiminus1_M",23)==0 || strncmp(title,"ProtonPiplusLPiplusUPiminus1_M",30)==0){
       posl = 0.1;
       posr = 0.3;
     }
     else {
       posl = 0.7;
       posr = 0.9;
     }	 
     

     

     TLegend *leg = new TLegend(posl,0.7,posr,0.9); //0.74, 0.75
     setstyle(var_data,var_mc,var_mcbg2,var_mcbg3,var_mcbg4,var_mcbg5,leg);      
     gStyle->SetOptStat(0);

     /*
     check = var_mc;
     check->Add(var_mcbg4);
     check->SetLineColor(kRed);
     */     
     
     TCanvas* canva = new TCanvas(title,title,15,10,400,300);
     //    gPad->SetLogy();
     //    var_data->SetMinimum(100.);
     var_mc->Draw("HIST F");
     var_mcbg2->Draw("HIST F SAME");
     var_mcbg3->Draw("HIST F SAME");
     var_mcbg4->Draw("HIST F SAME");
     var_mcbg5->Draw("HIST F SAME");
     check->Draw("HIST SAME");
     var_data->Draw("P SAME"); 
     leg->Draw("SAME");

     
     canva->Update();
     canva->Print(outpdf.c_str(),"pdf");    
     canva->Close();
     
}

    datafile->Close();
    mcfile->Close();
    
}
