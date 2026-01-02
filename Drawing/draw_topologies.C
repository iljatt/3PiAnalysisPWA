#define NN 4



void setstyle(TH1D *hist[NN])
{

  
  for (int ii=0; ii< NN; ii++){

   hist[ii]->SetMinimum(0.1);
   //   hist[ii]->SetMaximum(10000.);    
    hist[ii]->SetTitleOffset(1.0,"X");
    hist[ii]->SetTitleSize(0.05,"X");
   
    hist[ii]->SetMarkerSize(0.8);
    hist[ii]->SetMarkerStyle(20);
    hist[ii]->SetMarkerColor(1+ii);
    hist[ii]->SetLineColor(1+ii);
    hist[ii]->SetFillColor(1+ii);
    hist[ii]->SetLineWidth(2);

  }

  //   hist[9]->SetMarkerColor(14);
  //   hist[9]->SetLineColor(14);
  //   hist[9]->SetFillColor(14);
  
}




void LoopOverTree(TTree *ttree, TH1D *hchi2[NN], TH1D *henergy[NN], TH1D *htracks[NN], TH1D *hm2miss[NN], int flag)
{
       
  //*******************local variables****************************                                   
  Double_t  wt;
  Long64_t ientry;
  //********************************Reading the tree using TTreeReader**************************************                                                                                                  
  TTreeReader *myReader = new TTreeReader(ttree);
  Long64_t nevents = myReader->TTreeReader::GetEntries(true);


  //Enumerated topologies and weight                                                                                                                                                         
  TTreeReaderValue <Int_t> ThrownTopoIndex(*myReader,"ThrownTopoIndex");
  TTreeReaderValue <Double_t> RFTimeWeight(*myReader,"RFTimeWeight");

  
  //A group of basic cuts                                                                                                                                                                                    
  TTreeReaderValue <Double_t> tvar(*myReader,"tvar_U1");
  TTreeReaderValue <Double_t> Chi2Ndof(*myReader,"Chi2Ndof");
  TTreeReaderValue <UInt_t> NumUnusedTracks(*myReader,"NumUnusedTracks");
  TTreeReaderValue <Double_t> EnUnusedShowers(*myReader,"EnUnusedShowers");
  TTreeReaderValue <Double_t> Miss_M2(*myReader,"Miss_M2");
  

  cout << "\n\nMAIN EVENT LOOP OVER ~" << nevents/1000000 << "M EVENTS" << endl;


  //************Main event loop********************************************                                                                                                                                   
  while (myReader->Next()){
    ientry = myReader->GetCurrentEntry();
    if (ientry%1000000==0)   cout << "Arrived at event #" << ientry/1000000 << "M in the current file:" << ttree->GetCurrentFile()->GetName() << endl;

     wt = *RFTimeWeight;

     for (int ii=0; ii < NN; ii++){
       if (ii == *ThrownTopoIndex) {
	 switch (flag) {
	    case 0:  henergy[ii]->Fill(*EnUnusedShowers,wt);    hm2miss[ii]->Fill(*Miss_M2,wt); break;
	    case 1:  hchi2[ii]->Fill(*Chi2Ndof,wt); break;
	    case 2:  htracks[ii]->Fill((double) *NumUnusedTracks,wt); break;
	 }
	 break;
       }       
     }

  }

}





void draw_topologies()
{

  const char *input0 = "bggentree_pippippimpim__B2__sum_30274_31057_allcuts.root";
  const char *input1 = "bggentree_pippippimpim__B2__sum_30274_31057_allcuts_nochi2cut.root";
  const char *input2 = "bggentree_pippippimpim__B2__sum_30274_31057_allcuts_nounusedtrackscut.root";
  const char *treename = "kin"; // remains intact                                                                                                                                                            


  //*****************Declaring histograms to be filled********************                                                                                                                                
  TH1D *hchi2[NN],*henergy[NN],*htracks[NN],*hm2miss[NN];
  for (int ii=0; ii<NN; ii++){
    hchi2[ii] = new TH1D(Form("hchi2_top%d",ii),"",100,0.,10.);
    henergy[ii] = new TH1D(Form("henergy_top%d",ii),"",100,0.,2.);
    htracks[ii] = new TH1D(Form("htracks_top%d",ii),"",4,0.,4.);
    hm2miss[ii] = new TH1D(Form("hm2miss_top%d",ii),"",100,-0.02,0.02);
  }

  
  //*******************Preparing the histograms***************************** 
  TFile *file0 = TFile::Open(input0,"READ");
  TFile *file1 = TFile::Open(input1,"READ");
  TFile *file2 = TFile::Open(input2,"READ");

  if (!file0 || !file1 || !file2) return;                                                                                                                                                 
  TTree *dectree0 = (TTree*) file0->TFile::Get(treename);
  TTree *dectree1 = (TTree*) file1->TFile::Get(treename);
  TTree *dectree2 = (TTree*) file2->TFile::Get(treename);


  LoopOverTree(dectree0,hchi2,henergy,htracks,hm2miss,0);
  LoopOverTree(dectree1,hchi2,henergy,htracks,hm2miss,1);
  LoopOverTree(dectree2,hchi2,henergy,htracks,hm2miss,2);

  //  std::cout << "Nentries " << hchi2[0]->GetEntries() << "\t" << henergy[0]->GetEntries() << "\t" << htracks[0]->GetEntries() << "\t" << hm2miss[0]->GetEntries() << endl;  

  setstyle(hchi2);
  setstyle(henergy);
  setstyle(htracks);
  setstyle(hm2miss);
 
  hchi2[0]->GetXaxis()->SetTitle("#chi^{2}/#it{N}_{d.o.f.}");
  henergy[0]->GetXaxis()->SetTitle("#it{E}_{unused showers}  [GeV]");
  htracks[0]->GetXaxis()->SetTitle("#it{N}_{unused tracks}");
  hm2miss[0]->GetXaxis()->SetTitle("#it{M}_{miss}^{2}  [GeV^{2}]");


  for (int ii=0; ii<NN; ii++) htracks[ii]->SetFillColor(kWhite);

  //**********************************************************************

  
  
  //Drawing starts here
  gStyle->SetOptStat(0);

  TLegend *leg1 = new TLegend(0.3, 0.8, 0.6, 0.98); 
  leg1->SetBorderSize(1);
  leg1->AddEntry(htracks[0], "2#pi^{#plus}2#pi^{#minus}p", "l");
  leg1->AddEntry(htracks[1], "2#pi^{#plus}2#pi^{#minus}p[#omega]", "l");

  TLegend *leg2 = new TLegend(0.3, 0.8, 0.7, 0.98); 
  leg2->SetBorderSize(1);
  leg2->AddEntry(htracks[2], "2#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0}]", "l");
  leg2->AddEntry(htracks[3], "2#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#omega]", "l");  


  
 
  TCanvas* canva = new TCanvas("ctot","ctot",15,10,1000,750);
  canva->Divide(2,2);

  canva->cd(1);
  gPad->SetLogy();
  gPad->SetTopMargin(0.3);
  gPad->SetBottomMargin(0.11);
  hchi2[0]->Draw("HIST P");
  for (int ii=1; ii<NN; ii++) hchi2[ii]->Draw("HIST SAME");
  TLine *line = new TLine(5.,0.,5.,1.e+3);
  line->SetLineColor(kRed); line->SetLineWidth(3); line->SetLineStyle(kDotted);
  line->Draw("SAME");
  leg1->Draw("SAME");
  canva->Update();
  
  
  canva->cd(2);
  gPad->SetLogy();
  gPad->SetTopMargin(0.3);
  gPad->SetBottomMargin(0.11);
  htracks[0]->Draw("HIST");
  for (int ii=1; ii<NN; ii++) htracks[ii]->Draw("HIST SAME");
  gPad->Update();
  TLine *vline = new TLine(2.,0.,2.,1.2e+4);
  vline->SetLineColor(kRed); vline->SetLineWidth(3); vline->SetLineStyle(kDotted);
  vline->Draw("SAME");
  leg2->Draw("SAME");
  canva->Update();
 
  

  canva->cd(3);
  gPad->SetLogy();
  gPad->SetBottomMargin(0.11);
  henergy[0]->Draw("HIST P");
  for (int ii=1; ii<NN; ii++) henergy[ii]->Draw("HIST  SAME");
  canva->Update();


  
  canva->cd(4);
  gPad->SetLogy();
  gPad->SetBottomMargin(0.11);
  hm2miss[0]->Draw("HIST P");
  for (int ii=1; ii<NN; ii++) hm2miss[ii]->Draw("HIST SAME");


  canva->Print("topos.pdf","pdf");
  canva->Close();

  

  

  TCanvas* canvaL = new TCanvas("cleg","cleg",15,10,1200,750);
  canvaL->Divide(2);  
  canvaL->cd(1);
  leg1->Draw();  
  canvaL->cd(2);
  leg2->Draw();

  canvaL->Print("topos_leg.pdf","pdf");
  canvaL->Close();

  


  


 file0->Close();
 file1->Close();
 file2->Close();
  
}
