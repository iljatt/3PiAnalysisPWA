

void parsefile(const char *filename, Double_t arr[221][8])
{

  int ii,jj;
  ULong64_t tmp;
  FILE *fo = fopen(filename,"r");
  
  for (ii = 0; ii < 221; ii++){
    for (jj = 0; jj < 8; jj++){
      fscanf(fo,"%llu\t",&tmp); arr[ii][jj] = (double) tmp;}
    fscanf(fo,"\n");
  }

  fclose(fo);  
}  



void normalize(Double_t arr[221][8])
{

  Double_t sum[8];
  for (int jj=0; jj < 8; jj++)
    sum[jj] = 0.;

  
  for (int jj=0; jj < 8; jj++)
   for (int ii=0; ii < 221; ii++)
     sum[jj] += arr[ii][jj];

  for (int jj=0; jj < 8; jj++)
   for (int ii=0; ii < 221; ii++)
     arr[ii][jj] /= sum[jj];
       
}




void initgraphs(TGraph *gr[221], Double_t arr[221][8])
{

  Double_t xx[8], yy[8];
  
  for (int ii=0; ii < 221; ii++){
    for (int jj=0; jj < 8; jj++) {
      yy[jj] = arr[ii][jj]; 
      xx[jj] = (double) jj; 
    } 
    gr[ii] = new TGraph(8,xx,yy); 
  }
  
}



  







void setstyle(TGraph *gr[221], const char *ticks[8])
{

   gr[0]->SetMinimum(1.e-4);
   gr[0]->SetMaximum(5.e+0);
   
   gr[0]->SetTitle("Relative yield change under the cuts                        ");
   gr[0]->GetYaxis()->SetTitleSize(0.05);
   gr[0]->GetYaxis()->SetTitleOffset(1.);
   gr[0]->GetXaxis()->SetLabelSize(0.05);   

   for (int jj = 0; jj < 8; jj++)
     gr[0]->GetXaxis()->SetBinLabel(gr[0]->GetXaxis()->FindBin(jj), ticks[jj]); 


   for (int ii=0; ii < 221; ii++){
     gr[ii]->SetMarkerSize(0.5);
     gr[ii]->SetMarkerStyle(20);
     gr[ii]->SetMarkerColor(1+ii);
     gr[ii]->SetLineColor(1+ii);
     gr[ii]->SetLineWidth(3);
   }

   gr[9]->SetMarkerColor(14);
   gr[9]->SetLineColor(14);
  

}







void draw_yields_normalized()
{

  const char *labels[8] = {"Initial", "N_{unused tracks} <= 1", "#chi^{2}/N_{d.o.f.} <= 5", "-t <= 2 GeV^{2}", "#pi^{+} fast -- slow cut", "#Delta^{++} mass cut", "m(4#pi) cut", "p_{z} cut"};
  Double_t yields[221][8];  
  TGraph *gtopos[221];


  
  parsefile("yields.txt",yields); //checked
  normalize(yields);

  printf("Signal topology gives %.4f of the total yield\n",yields[0][7]);
  
  initgraphs(gtopos,yields); // checked for  the Double type
  setstyle(gtopos,labels);
  

    
  //Drawing starts here
   TCanvas *canva = new TCanvas("c","c",15,10,700,500);
   gPad->SetTopMargin(0.1);
   gPad->SetBottomMargin(0.26);   
   gPad->SetLeftMargin(0.08);
   gPad->SetRightMargin(0.3);
   gPad->SetLogy();
   gPad->SetGrid();
     

   TLegend *leg = new TLegend(0.72, 0.25, 0.98, 0.9); 
   leg->SetBorderSize(0);
   leg->SetFillStyle(0); 
   leg->AddEntry(gtopos[0], "2#pi^{#plus}2#pi^{#minus}p", "l");
   leg->AddEntry(gtopos[1], "2#pi^{#plus}2#pi^{#minus}p[#omega]", "l");
   leg->AddEntry(gtopos[2], "2#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0}]", "l");
   leg->AddEntry(gtopos[3], "2#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#omega]", "l");
   leg->AddEntry(gtopos[4], "#pi^{#plus}#pi^{#minus}K^{#plus}K^{#minus}p", "l");
   leg->AddEntry(gtopos[5], "#gamma2#pi^{#plus}2#pi^{#minus}p[#eta']", "l");
   leg->AddEntry(gtopos[6], "2#pi^{#plus}#pi^{#minus}K^{#minus}p[K^{0}_{S}]", "l");
   leg->AddEntry(gtopos[7], "2#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#eta]", "l");
   leg->AddEntry(gtopos[8], "2#gamma#pi^{#plus}#pi^{#minus}p[#pi^{0}]", "l");
   leg->AddEntry(gtopos[9], "#gamma2#pi^{#plus}2#pi^{#minus}p[#eta]", "l");

   
   
   gtopos[0]->Draw("ALP");
   for (int ii=0; ii<10; ii++)  gtopos[ii]->Draw("LP SAME");


   for (int ii = 0; ii < 8; ii++){
     TLine * line = new TLine((double) ii, 1.e-4, (double) ii, 5.e+0);
     line->SetLineStyle(3);
     line->SetLineColor(kBlack);
     line->Draw("SAME");
}


   canva->Update();
   leg->Draw("SAME");
     
   canva->Print("yields_normalized.pdf","pdf");    
   //    canva->Close();


  

}
