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
    dhisto->SetMarkerColor(kBlack);
    dhisto->SetTitleOffset(1.0,"X");
    dhisto->SetTitleSize(0.05,"X");
    dhisto->SetTitleOffset(1.2,"Y");
    dhisto->SetTitleSize(0.04,"Y"); 
    dhisto->SetLineColor(kBlack);
    dhisto->SetLineWidth(2);    
    dhisto->SetFillColor(kBlack);
    dhisto->SetFillStyle(0);//3016

    
    
    dhistocut->SetMarkerSize(0.4);
    dhistocut->SetMarkerStyle(20);
    dhistocut->SetMarkerColor(kRed);
    dhistocut->SetTitleOffset(1.0,"X");
    dhistocut->SetTitleSize(0.05,"X");
    dhistocut->SetTitleOffset(1.2,"Y");
    dhistocut->SetTitleSize(0.04,"Y"); 
    dhistocut->SetLineColor(kRed);
    dhistocut->SetLineWidth(2);    
    //    dhistocut->SetFillColor(kRed);
    //    dhistocut->SetFillStyle(3015);

    
    dhistocut2nd->SetMarkerSize(0.4);
    dhistocut2nd->SetMarkerStyle(20);
    dhistocut2nd->SetMarkerColor(kGreen);
    dhistocut2nd->SetTitleOffset(1.0,"X");
    dhistocut2nd->SetTitleSize(0.05,"X");
    dhistocut2nd->SetTitleOffset(1.2,"Y");
    dhistocut2nd->SetTitleSize(0.04,"Y"); 
    dhistocut2nd->SetLineColor(kGreen);
    dhistocut2nd->SetLineWidth(2);    
    dhistocut2nd->SetFillColor(kGreen);
    dhistocut2nd->SetFillStyle(0);//3015


    dhistocut3rd->SetMarkerSize(0.4);
    dhistocut3rd->SetMarkerStyle(20);
    dhistocut3rd->SetMarkerColor(kBlue);
    dhistocut3rd->SetTitleOffset(1.0,"X");
    dhistocut3rd->SetTitleSize(0.05,"X");
    dhistocut3rd->SetTitleOffset(1.2,"Y");
    dhistocut3rd->SetTitleSize(0.04,"Y"); 
    dhistocut3rd->SetLineColor(kBlue);
    dhistocut3rd->SetLineWidth(2);    
    //    dhistocut3rd->SetFillColor(kBlue);
    //    dhistocut3rd->SetFillStyle(3005);
    
    
}







void printToposContributions()
{


  const char *data1filename = "histos_bggen_topo0_allcuts.root";
  const char *data2filename = "histos_bggen_topo1_allcuts.root";
  const char *data3filename = "histos_bggen_topo2_allcuts.root";
  const char *data4filename = "histos_bggen_topo3_allcuts.root";


  
  Double_t posl,posr;

  
   
   TFile *data1file = TFile::Open(data1filename,"READ");
   TFile *data2file = TFile::Open(data2filename,"READ");
   TFile *data3file = TFile::Open(data3filename,"READ");
   TFile *data4file = TFile::Open(data4filename,"READ");
   if (!data1file || !data2file || !data3file || !data4file) return;

   
   std::string variables[25] = {"minustvar","Piminus1Proton_M","Piminus2Proton_M","Piplus1Piplus2Piminus1_M","Piplus1Piplus2Piminus2_M","ProtonPiplus1Piminus1_M","ProtonPiplus1Piminus2_M","ProtonPiplus2Piminus1_M","ProtonPiplus2Piminus2_M","ProtonPiplus1Piminus1Piminus2_M","ProtonPiplus2Piminus1Piminus2_M","ProtonPiplus1Piplus2Piminus1_M","ProtonPiplus1Piplus2Piminus2_M","FourPions_M","DeltaPhi_LV_UV","DeltaTheta_LV_UV","Miss_M2","Piplus2_P","Piminus2_Pz","Piplus2Piminus1_M","Piplus2Piminus2_M","Piplus2Proton_M","Piplus1Piminus1_M","Piplus1Piminus2_M","Piplus1Piminus1Piminus2_M"};
   std::string outpdf;

   
   TH1D *var_data = new TH1D();
   TH1D *var_datacut = new TH1D();
   TH1D *var_datacut2nd = new TH1D();
   TH1D *var_datacut3rd = new TH1D();

  
   for (int i=0; i<25; i++){
   
     const char *title = variables[i].c_str();      
     outpdf = "ShowImpactTopos/pic_"+variables[i]+".pdf";

 
     var_data = (TH1D*)data1file->TFile::Get(title);
     var_datacut = (TH1D*)data2file->TFile::Get(title);
     var_datacut2nd = (TH1D*)data3file->TFile::Get(title);
     var_datacut3rd = (TH1D*)data4file->TFile::Get(title);
  

     //write latex-style captions to the axis     
     if (variables[i]=="minustvar")                var_data->GetXaxis()->SetTitle("-t  [GeV^{2}]");
     if (variables[i]=="Piminus1Proton_M")         var_data->GetXaxis()->SetTitle("m(p#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="Piminus2Proton_M")         var_data->GetXaxis()->SetTitle("m(p#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="Piplus1Piplus2Piminus1_M") var_data->GetXaxis()->SetTitle("m(#pi^{+}#pi^{+}#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="Piplus1Piplus2Piminus2_M")  var_data->GetXaxis()->SetTitle("m(#pi^{+}#pi^{+}#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="ProtonPiplus1Piminus1_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}(uv)#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="ProtonPiplus1Piminus2_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}(uv)#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="ProtonPiplus2Piminus1_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}(lv)#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="ProtonPiplus2Piminus2_M")   var_data->GetXaxis()->SetTitle("m(p#pi^{+}(lv)#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="ProtonPiplus1Piminus1Piminus2_M"){
                                                    var_data->GetXaxis()->SetTitle("m(p#pi^{+}(uv)#pi^{-}#pi^{-})  [GeV]");
                                                    var_data->GetXaxis()->SetRangeUser(2.5,4.5);
     }
     if (variables[i]=="ProtonPiplus2Piminus1Piminus2_M") var_data->GetXaxis()->SetTitle("m(p#pi^{+}(lv)#pi^{-}#pi^{-})  [GeV]");
     if (variables[i]=="ProtonPiplus1Piplus2Piminus1_M"){
                                                     var_data->GetXaxis()->SetTitle("m(p#pi^{+}#pi^{+}#pi^{-}_{fast})  [GeV]");
                                                     var_data->GetXaxis()->SetRangeUser(1.5,4.5);
     }
     if (variables[i]=="ProtonPiplus1Piplus2Piminus2_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}#pi^{+}#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="FourPions_M"){                    var_data->GetXaxis()->SetTitle("m(4#pi)  [GeV]");
                                                          var_data->GetXaxis()->SetRangeUser(1.0,3.5); 
                                      }
     if (variables[i]=="Piplus2Piminus1_M")      var_data->GetXaxis()->SetTitle("m(#pi^{+}(lv)#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="Piplus2Piminus2_M")      var_data->GetXaxis()->SetTitle("m(#pi^{+}(lv)#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="DeltaPhi_LV_UV")         var_data->GetXaxis()->SetTitle("#Delta#phi(lv-uv)  [deg]");
     if (variables[i]=="DeltaTheta_LV_UV")       var_data->GetXaxis()->SetTitle("#Delta#Theta(lv-uv)  [deg]");
     if (variables[i]=="Miss_M2")                var_data->GetXaxis()->SetTitle("m_{miss}^{2}  [GeV^{2}]");
     if (variables[i]=="Piplus2_P")              var_data->GetXaxis()->SetTitle("|p(#pi^{+}(lv))|  [GeV]");
     if (variables[i]=="Piminus2_Pz")            var_data->GetXaxis()->SetTitle("p_{z}(#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="Piplus2Proton_M")        var_data->GetXaxis()->SetTitle("m(p#pi^{+}(lv))  [GeV]");
     if (variables[i]=="Piplus1Piminus1_M")      var_data->GetXaxis()->SetTitle("m(#pi^{+}(uv)#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="Piplus1Piminus2_M")      var_data->GetXaxis()->SetTitle("m(#pi^{+}(uv)#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="Piplus1Piminus1Piminus2_M")     var_data->GetXaxis()->SetTitle("m(#pi^{+}(uv)#pi^{-}#pi^{-})  [GeV]");
       
 


     ///Drawing   
     if (strncmp(title,"ProtonPiplus1Piminus1Piminus2_M",31)==0 || strncmp(title,"ProtonPiplus2Piminus1Piminus2_M",31)==0 ||
	 strncmp(title,"ProtonPiplus1Piminus1_M",23)==0 || strncmp(title,"ProtonPiplus1Piplus2Piminus1_M",30)==0){
       posl = 0.1;
       posr = 0.3;
     }
     else {
       posl = 0.7;
       posr = 0.9;
     }	 
     

     


     TLegend *leg = new TLegend(posl,0.75,posr,0.9); //0.74, 0.75
     setstyle(var_data,var_datacut,var_datacut2nd,var_datacut3rd);      
     gStyle->SetOptStat(0);


     
     TCanvas* canva = new TCanvas(title,title,15,10,400,300);
     gPad->SetLogy();
     //    var_data->SetMinimum(100.);
     gPad->SetBottomMargin(0.11);
     var_data->SetMinimum(1.);
     var_data->Draw("*H"); 
     var_datacut->Draw("*H F SAME");
     var_datacut2nd->Draw("*H F SAME");
     var_datacut3rd->Draw("*H F SAME");
     //     gfill->Draw("same,lf");   


     
     canva->Update();
     canva->Print(outpdf.c_str(),"pdf");    
     canva->Close();
     
}

    data1file->Close();
    data2file->Close();
    data3file->Close();
    
}
