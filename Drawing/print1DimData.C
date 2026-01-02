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




void setstyle(TH1D *dhisto, TLegend *leg)
{

    leg->AddEntry(dhisto,"  Data","p");
    

    
    dhisto->SetMarkerSize(0.5);
    dhisto->SetMarkerStyle(20);
    dhisto->SetTitleOffset(0.9,"X");
    dhisto->SetTitleSize(0.05,"X");
    dhisto->SetTitleOffset(1.2,"Y");
    dhisto->SetTitleSize(0.04,"Y"); 
    dhisto->SetLineColor(kBlack);
    dhisto->SetLineWidth(2);    

    
}







void print1DimData()
{


  //  const char *datafilename = "Different_com_cuts/histos_data_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_3picomcut1p0.root";
  //  const char *datafilename = "histos_data_spring2017_precuts_deltacut.root";
  //  const char *datafilename = "histos_data_spring2017_precuts.root";
  //  const char *datafilename = "histos_data_spring2017_precuts_diffpipluscut_fourpionscut_piminuscut.root";
  //  const char *datafilename = "histos_data_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut1p0.root";
  const char *datafilename = "histos_data_spring2017_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut.root";
  //  const char *datafilename = "histos_data_spring2017_precuts_diffpipluscut_deltacut.root";
  //  const char *datafilename = "histos_data_spring2017_precuts_diffpipluscut_deltacut_fourpionscut.root";
  std::string dirname = "Data_precuts_diffpipluscut_fourpionscut_piminuscut";

  

  Double_t posl,posr;
  
   
   TFile *datafile = TFile::Open(datafilename,"READ");
   if (!datafile) return;

   
   std::string variables[25] = {"minustvar","Piminus1Proton_M","Piminus2Proton_M","PiplusLPiplusUPiminus1_M","PiplusLPiplusUPiminus2_M","ProtonPiplusUPiminus1_M","ProtonPiplusUPiminus2_M","ProtonPiplusLPiminus1_M","ProtonPiplusLPiminus2_M","ProtonPiplusUPiminus1Piminus2_M","ProtonPiplusLPiminus1Piminus2_M","ProtonPiplusLPiplusUPiminus1_M","ProtonPiplusLPiplusUPiminus2_M","FourPions_M","DeltaPhi_LV_UV","DeltaTheta_LV_UV","Miss_M2","PiplusL_P","Piminus2_Pz","PiplusLPiminus1_M","PiplusLPiminus2_M","PiplusLProton_M","PiplusUPiminus1_M","PiplusUPiminus2_M","PiplusUPiminus1Piminus2_M"};
   std::string outpdf;

   
   TH1D *var_data = new TH1D();

  
   for (int i=0; i<25; i++){
   
     outpdf = dirname+"/pic_"+variables[i]+".pdf";
     const char *title = variables[i].c_str();      

 
     var_data = (TH1D*)datafile->TFile::Get(title);


     //write latex-style captions to the axis     
     if (variables[i]=="minustvar")                var_data->GetXaxis()->SetTitle("-t  [GeV^{2}]");
     if (variables[i]=="Piminus1Proton_M")         var_data->GetXaxis()->SetTitle("m(p#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="Piminus2Proton_M")         var_data->GetXaxis()->SetTitle("m(p#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="PiplusLPiplusUPiminus1_M") var_data->GetXaxis()->SetTitle("m(#pi^{+}#pi^{+}#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="PiplusLPiplusUPiminus2_M")  var_data->GetXaxis()->SetTitle("m(#pi^{+}#pi^{+}#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="ProtonPiplusUPiminus1_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}(uv)#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="ProtonPiplusUPiminus2_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}(uv)#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiminus1_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}(lv)#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiminus2_M")   var_data->GetXaxis()->SetTitle("m(p#pi^{+}(lv)#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="ProtonPiplusUPiminus1Piminus2_M") var_data->GetXaxis()->SetTitle("m(p#pi^{+}(uv)#pi^{-}#pi^{-})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiminus1Piminus2_M") var_data->GetXaxis()->SetTitle("m(p#pi^{+}(lv)#pi^{-}#pi^{-})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiplusUPiminus1_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}#pi^{+}#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiplusUPiminus2_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}#pi^{+}#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="FourPions_M"){                    var_data->GetXaxis()->SetTitle("m(4#pi)  [GeV]");
                                                          var_data->GetXaxis()->SetRangeUser(1.,3.5); //1.5
                                      }
     if (variables[i]=="PiplusLPiminus1_M")      var_data->GetXaxis()->SetTitle("m(#pi^{+}(lv)#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="PiplusLPiminus2_M")      var_data->GetXaxis()->SetTitle("m(#pi^{+}(lv)#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="DeltaPhi_LV_UV")         var_data->GetXaxis()->SetTitle("#Delta#phi(lv-uv)  [deg]");
     if (variables[i]=="DeltaTheta_LV_UV")       var_data->GetXaxis()->SetTitle("#Delta#Theta(lv-uv)  [deg]");
     if (variables[i]=="Miss_M2")                var_data->GetXaxis()->SetTitle("m_{miss}^{2}  [GeV^{2}]");
     if (variables[i]=="PiplusL_P")              var_data->GetXaxis()->SetTitle("|p(#pi^{+}(lv))|  [GeV]");
     if (variables[i]=="Piminus2_Pz")            var_data->GetXaxis()->SetTitle("p_{z}(#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="PiplusLProton_M")        var_data->GetXaxis()->SetTitle("m(p#pi^{+}_{lv})  [GeV]");
     if (variables[i]=="PiplusUPiminus1_M")      var_data->GetXaxis()->SetTitle("m(#pi^{+}_{uv}#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="PiplusUPiminus2_M")      var_data->GetXaxis()->SetTitle("m(#pi^{+}_{uv}#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="PiplusUPiminus1Piminus2_M")     var_data->GetXaxis()->SetTitle("m(#pi^{+}_{uv}#pi^{-}#pi^{-})  [GeV]");
        
 


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
     
     

     TLegend *leg = new TLegend(posl,0.75,posr,0.9); //0.74, 0.75
     setstyle(var_data,leg);      
     gStyle->SetOptStat(0);
     
     TCanvas* canva = new TCanvas(title,title,15,10,400,300);
     //    gPad->SetLogy();
     //    var_data->SetMinimum(100.);
     var_data->Draw("P"); 
     //     leg->Draw("SAME");

     
     canva->Update();
     canva->Print(outpdf.c_str(),"pdf");    
     canva->Close();
     
}

    datafile->Close();
    
}
