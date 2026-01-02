#include "calc_angles.h"



void printListOfBranches(TChain *dectree)
{
  TList:TObjArray *brslist = dectree->TChain::GetListOfBranches();
  brslist->SetOwner(kFALSE); 
  brslist->Sort();  
   for(int ii = 0; ii < brslist->GetEntries(); ii++) 
     cout << "BR #"<< ii << "\t\t" << brslist->At(ii)->GetTitle() << endl; 
}



void RemoveNegativeBins(TH2* histo)
{
  for (int ix=0; ix<=histo->GetNbinsX()+1; ++ix)
    for (int iy=0; iy<=histo->GetNbinsY()+1; ++iy)
     if (histo->GetBinContent(ix,iy) < 0) histo->SetBinContent(ix,iy,0);
}




void LoopOverTree(TChain *tchain)
{
   
  //  tchain->SetBranchStatus("*",0);
  //  tchain->SetBranchStatus("*M",1);  
  
  const Int_t DEFBINS = 100;
  const Int_t DEFTBINS = 80;
  const Int_t NN = 6;
  const Float_t ProtonMass = 0.938272;

  
  //*******************local variables****************************
  Double_t  wt, bz, deltatheta, deltaphi, mean;
  Long64_t ientry;
  Bool_t  isFile1, isFile2;
  TLorentzVector LV_P4, UV_P4;
  
  
  //********************************Reading the tree using TTreeReader**************************************
  TTreeReader *myReader = new TTreeReader(tchain);
  Long64_t nevents = myReader->TTreeReader::GetEntries(true);


  //MOMENTA
  TTreeReaderValue <TLorentzVector> PiplusL_P4(*myReader, "Thrown_PiplusL_P4");
  TTreeReaderValue <TLorentzVector> PiplusU_P4(*myReader, "Thrown_PiplusU_P4");
  TTreeReaderValue <TLorentzVector> Piminus1_P4(*myReader, "Thrown_Piminus1_P4");
  TTreeReaderValue <TLorentzVector> Piminus2_P4(*myReader, "Thrown_Piminus2_P4");
  TTreeReaderValue <TLorentzVector> Proton_P4(*myReader, "Thrown_Proton_P4");

  
  

  //ANGLES
  TTreeReaderValue <Double_t> CosThetaThrownPiminus1Bachelor_GJ(*myReader,"CosThetaThrown_Piminus1Bachelor_GJ");
  TTreeReaderValue <Double_t> CosThetaThrownPiminus2Bachelor_GJ(*myReader,"CosThetaThrown_Piminus2Bachelor_GJ");
  TTreeReaderValue <Double_t> CosThetaThrownPiminus1Isobar_HF(*myReader,"CosThetaThrown_Piminus1Isobar_HF");
  TTreeReaderValue <Double_t> CosThetaThrownPiminus2Isobar_HF(*myReader,"CosThetaThrown_Piminus2Isobar_HF");
  TTreeReaderValue <Double_t> PhiThrownPiminus1Bachelor_GJ(*myReader,"PhiThrown_Piminus1Bachelor_GJ");
  TTreeReaderValue <Double_t> PhiThrownPiminus2Bachelor_GJ(*myReader,"PhiThrown_Piminus2Bachelor_GJ");
  TTreeReaderValue <Double_t> PhiThrownPiminus1Isobar_HF(*myReader,"PhiThrown_Piminus1Isobar_HF");
  TTreeReaderValue <Double_t> PhiThrownPiminus2Isobar_HF(*myReader,"PhiThrown_Piminus2Isobar_HF");

  
  
  
  //OTHER
  TTreeReaderValue <Double_t> MCWeight(*myReader,"MCWeight");
  TTreeReaderValue <Double_t> tvar(*myReader,"tvar");
  TTreeReaderValue <Double_t> E_Beam(*myReader,"EnBeam");

  
  
  //*****************Declaring histograms to be filled********************


  //Angles 
  TH2D *anglesThrownGJ[NN];
  TH2D *anglesThrownHF[NN];


  for (int jj=0; jj< NN; jj++){
    anglesThrownGJ[jj] = new TH2D(Form("AnglesThrownGJ_bin%d",jj),"",3*DEFBINS,-1.,1.,3*DEFBINS,-180.,180.);
    anglesThrownHF[jj] = new TH2D(Form("AnglesThrownHF_bin%d",jj),"",3*DEFBINS,-1.,1.,3*DEFBINS,-180.,180.);  
  }  

  
  //3-particle van Hove plot 
  TH2D *vanHoveXY_ppipLX = new TH2D("vanHoveXY_for_ppipLX","",3*DEFBINS,-2.5,2.5,3*DEFBINS,-2.5,2.5);

    
  
  

  
    
  cout << "\n\nMAIN EVENT LOOP OVER ~" << nevents/1000000 << "M EVENTS" << endl;
  wt = 1.;
  
  //************Main event loop********************************************
  while (myReader->Next()){
    ientry = myReader->GetCurrentEntry(); //if (ientry==100000) break;
    if (ientry%1000000==0){
      cout << "Arrived at event #" << ientry/1000000 << "M in the current file:" << tchain->GetCurrentFile()->GetName() << endl;
    ////dectree->Show(ientry);
    }

    //FILLING DESIRED HISTOGRAMS      
    
    
    
    UV_P4 = *PiplusU_P4+*Piminus1_P4+*Piminus2_P4;      
    vanHoveXY_ppipLX->Fill(vanHoveX(*Proton_P4,*PiplusL_P4,UV_P4),vanHoveY(*Proton_P4,*PiplusL_P4,UV_P4),wt);    


    anglesThrownGJ[0]->Fill(*CosThetaThrownPiminus1Bachelor_GJ,*PhiThrownPiminus1Bachelor_GJ,wt);
    anglesThrownGJ[0]->Fill(*CosThetaThrownPiminus2Bachelor_GJ,*PhiThrownPiminus2Bachelor_GJ,wt);

    anglesThrownHF[0]->Fill(*CosThetaThrownPiminus2Isobar_HF,*PhiThrownPiminus2Isobar_HF,wt);
    anglesThrownHF[0]->Fill(*CosThetaThrownPiminus1Isobar_HF,*PhiThrownPiminus1Isobar_HF,wt);

    
    if (*tvar > -0.25 && *tvar <=0){
      anglesThrownGJ[1]->Fill(*CosThetaThrownPiminus1Bachelor_GJ,*PhiThrownPiminus1Bachelor_GJ,wt);
      anglesThrownGJ[1]->Fill(*CosThetaThrownPiminus2Bachelor_GJ,*PhiThrownPiminus2Bachelor_GJ,wt);
      anglesThrownHF[1]->Fill(*CosThetaThrownPiminus2Isobar_HF,*PhiThrownPiminus2Isobar_HF,wt);
      anglesThrownHF[1]->Fill(*CosThetaThrownPiminus1Isobar_HF,*PhiThrownPiminus1Isobar_HF,wt);    
    }
    if (*tvar > -0.5 && *tvar <= -0.25){
      anglesThrownGJ[2]->Fill(*CosThetaThrownPiminus1Bachelor_GJ,*PhiThrownPiminus1Bachelor_GJ,wt);
      anglesThrownGJ[2]->Fill(*CosThetaThrownPiminus2Bachelor_GJ,*PhiThrownPiminus2Bachelor_GJ,wt);
      anglesThrownHF[2]->Fill(*CosThetaThrownPiminus2Isobar_HF,*PhiThrownPiminus2Isobar_HF,wt);
      anglesThrownHF[2]->Fill(*CosThetaThrownPiminus1Isobar_HF,*PhiThrownPiminus1Isobar_HF,wt);    
    }
    if (*tvar > -0.75 && *tvar <= -0.5){
      anglesThrownGJ[3]->Fill(*CosThetaThrownPiminus1Bachelor_GJ,*PhiThrownPiminus1Bachelor_GJ,wt);
      anglesThrownGJ[3]->Fill(*CosThetaThrownPiminus2Bachelor_GJ,*PhiThrownPiminus2Bachelor_GJ,wt);
      anglesThrownHF[3]->Fill(*CosThetaThrownPiminus2Isobar_HF,*PhiThrownPiminus2Isobar_HF,wt);
      anglesThrownHF[3]->Fill(*CosThetaThrownPiminus1Isobar_HF,*PhiThrownPiminus1Isobar_HF,wt);    
    }
    if (*tvar > -1. && *tvar <= -0.75){
      anglesThrownGJ[4]->Fill(*CosThetaThrownPiminus1Bachelor_GJ,*PhiThrownPiminus1Bachelor_GJ,wt);
      anglesThrownGJ[4]->Fill(*CosThetaThrownPiminus2Bachelor_GJ,*PhiThrownPiminus2Bachelor_GJ,wt);
      anglesThrownHF[4]->Fill(*CosThetaThrownPiminus2Isobar_HF,*PhiThrownPiminus2Isobar_HF,wt);
      anglesThrownHF[4]->Fill(*CosThetaThrownPiminus1Isobar_HF,*PhiThrownPiminus1Isobar_HF,wt);    
    }
    if (*tvar >= -2. && *tvar <= -1.){
      anglesThrownGJ[5]->Fill(*CosThetaThrownPiminus1Bachelor_GJ,*PhiThrownPiminus1Bachelor_GJ,wt);
      anglesThrownGJ[5]->Fill(*CosThetaThrownPiminus2Bachelor_GJ,*PhiThrownPiminus2Bachelor_GJ,wt);
      anglesThrownHF[5]->Fill(*CosThetaThrownPiminus2Isobar_HF,*PhiThrownPiminus2Isobar_HF,wt);
      anglesThrownHF[5]->Fill(*CosThetaThrownPiminus1Isobar_HF,*PhiThrownPiminus1Isobar_HF,wt);    
    }


    
    }


      
  vanHoveXY_ppipLX->Write();

  for (int jj=0; jj< NN; jj++){
    anglesThrownGJ[jj]->Write();
    anglesThrownHF[jj]->Write();
  }

    
    
}




void write_correlations_thrown(const char *input0, const char *output)
{

  const char *treename = "kin"; // remains intact


 //*******************Root files: the tree chain and its branches********************
  TFile *file0 = TFile::Open(input0,"READ");
  /*
  TFile *file1 = TFile::Open(input1,"READ");
  TFile *file2 = TFile::Open(input2,"READ");
  TFile *file3 = TFile::Open(input3,"READ");
  TFile *file4 = TFile::Open(input4,"READ");
  TFile *file5 = TFile::Open(input5,"READ");

  if (!file1 || !file2 || !file3 || !file4 || !file5) return;
  */
  
  TFile *fileOut = TFile::Open(output,"RECREATE");

  
  TChain *dectree = new TChain(treename);
  dectree->Add(input0);


  //    printListOfBranches(dectree);
    LoopOverTree(dectree);


  
    fileOut->Close();
    file0->Close();
    //    file1->Close();
    //    file2->Close();
    //    file3->Close();
    //    file4->Close();
    //    file5->Close();

}



