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
  TTreeReaderValue <TLorentzVector> PiplusL_P4(*myReader, "PiplusL_P4");
  TTreeReaderValue <TLorentzVector> PiplusU_P4(*myReader, "PiplusU_P4");
  TTreeReaderValue <TLorentzVector> Piminus1_P4(*myReader, "Piminus1_P4");
  TTreeReaderValue <TLorentzVector> Piminus2_P4(*myReader, "Piminus2_P4");
  TTreeReaderValue <TLorentzVector> Proton_P4(*myReader, "Proton_P4");

  TTreeReaderValue <TLorentzVector> Beam_Meas_P4(*myReader, "Beam_Meas_P4");
  TTreeReaderValue <TLorentzVector> PiplusL_MP4(*myReader, "PiplusL_Meas_P4");
  TTreeReaderValue <TLorentzVector> PiplusU_MP4(*myReader, "PiplusU_Meas_P4");
  TTreeReaderValue <TLorentzVector> Piminus1_MP4(*myReader, "Piminus1_Meas_P4");
  TTreeReaderValue <TLorentzVector> Piminus2_MP4(*myReader, "Piminus2_Meas_P4");
  TTreeReaderValue <TLorentzVector> Proton_MP4(*myReader, "Proton_Meas_P4");
  
  
  // MASSES
  TTreeReaderValue <Double_t> PiplusLPiminus1_M(*myReader,"PiplusLPiminus1_M");
  TTreeReaderValue <Double_t> PiplusLPiminus2_M(*myReader,"PiplusLPiminus2_M");    
  TTreeReaderValue <Double_t> PiplusUPiminus1_M(*myReader,"PiplusUPiminus1_M");
  TTreeReaderValue <Double_t> PiplusUPiminus2_M(*myReader,"PiplusUPiminus2_M");

  TTreeReaderValue <Double_t> PiplusLPiplusUPiminus1_M(*myReader,"PiplusLPiplusUPiminus1_M");
  TTreeReaderValue <Double_t> PiplusLPiplusUPiminus2_M(*myReader,"PiplusLPiplusUPiminus2_M");
  TTreeReaderValue <Double_t> PiplusLPiminus1Piminus2_M(*myReader,"PiplusLPiminus1Piminus2_M");
  TTreeReaderValue <Double_t> PiplusUPiminus1Piminus2_M(*myReader,"PiplusUPiminus1Piminus2_M");

  TTreeReaderValue <Double_t> PiplusLProton_M(*myReader,"PiplusLProton_M");
  TTreeReaderValue <Double_t> PiplusUProton_M(*myReader,"PiplusUProton_M");
  TTreeReaderValue <Double_t> Piminus1Proton_M(*myReader,"Piminus1Proton_M");
  TTreeReaderValue <Double_t> Piminus2Proton_M(*myReader,"Piminus2Proton_M");

  TTreeReaderValue <Double_t> ProtonPiplusLPiminus1_M(*myReader,"ProtonPiplusLPiminus1_M");
  TTreeReaderValue <Double_t> ProtonPiplusLPiminus2_M(*myReader,"ProtonPiplusLPiminus2_M");    
  TTreeReaderValue <Double_t> ProtonPiplusUPiminus1_M(*myReader,"ProtonPiplusUPiminus1_M");
  TTreeReaderValue <Double_t> ProtonPiplusUPiminus2_M(*myReader,"ProtonPiplusUPiminus2_M");
  TTreeReaderValue <Double_t> ProtonPiplusLPiplusUPiminus1_M(*myReader,"ProtonPiplusLPiplusUPiminus1_M");
  TTreeReaderValue <Double_t> ProtonPiplusLPiplusUPiminus2_M(*myReader,"ProtonPiplusLPiplusUPiminus2_M");
  TTreeReaderValue <Double_t> ProtonPiplusLPiminus1Piminus2_M(*myReader,"ProtonPiplusLPiminus1Piminus2_M");
  TTreeReaderValue <Double_t> ProtonPiplusUPiminus1Piminus2_M(*myReader,"ProtonPiplusUPiminus1Piminus2_M");
  TTreeReaderValue <Double_t> FourPions_M(*myReader,"FourPions_M");


  //ANGLES
  TTreeReaderValue <Double_t> CosThetaPiminus1Bachelor_GJ(*myReader,"CosTheta_Piminus1Bachelor_GJ");
  TTreeReaderValue <Double_t> CosThetaPiminus2Bachelor_GJ(*myReader,"CosTheta_Piminus2Bachelor_GJ");
  TTreeReaderValue <Double_t> CosThetaPiminus1Isobar_HF(*myReader,"CosTheta_Piminus1Isobar_HF");
  TTreeReaderValue <Double_t> CosThetaPiminus2Isobar_HF(*myReader,"CosTheta_Piminus2Isobar_HF");
  TTreeReaderValue <Double_t> PhiPiminus1Bachelor_GJ(*myReader,"Phi_Piminus1Bachelor_GJ");
  TTreeReaderValue <Double_t> PhiPiminus2Bachelor_GJ(*myReader,"Phi_Piminus2Bachelor_GJ");
  TTreeReaderValue <Double_t> PhiPiminus1Isobar_HF(*myReader,"Phi_Piminus1Isobar_HF");
  TTreeReaderValue <Double_t> PhiPiminus2Isobar_HF(*myReader,"Phi_Piminus2Isobar_HF");
 
  TTreeReaderValue <Double_t> CosThetaThrownPiminus1Bachelor_GJ(*myReader,"CosThetaThrown_Piminus1Bachelor_GJ");
  TTreeReaderValue <Double_t> CosThetaThrownPiminus2Bachelor_GJ(*myReader,"CosThetaThrown_Piminus2Bachelor_GJ");
  TTreeReaderValue <Double_t> CosThetaThrownPiminus1Isobar_HF(*myReader,"CosThetaThrown_Piminus1Isobar_HF");
  TTreeReaderValue <Double_t> CosThetaThrownPiminus2Isobar_HF(*myReader,"CosThetaThrown_Piminus2Isobar_HF");
  TTreeReaderValue <Double_t> PhiThrownPiminus1Bachelor_GJ(*myReader,"PhiThrown_Piminus1Bachelor_GJ");
  TTreeReaderValue <Double_t> PhiThrownPiminus2Bachelor_GJ(*myReader,"PhiThrown_Piminus2Bachelor_GJ");
  TTreeReaderValue <Double_t> PhiThrownPiminus1Isobar_HF(*myReader,"PhiThrown_Piminus1Isobar_HF");
  TTreeReaderValue <Double_t> PhiThrownPiminus2Isobar_HF(*myReader,"PhiThrown_Piminus2Isobar_HF");

  
  
  
  //OTHER
  TTreeReaderValue <Double_t> TSlopeWeight(*myReader,"TSlopeWeight");
  TTreeReaderValue <Double_t> tvar(*myReader,"tvar");
  TTreeReaderValue <Double_t> Miss_M2(*myReader,"Miss_M2");
  TTreeReaderValue <Float_t> E_Beam(*myReader,"E_Beam");
  TTreeReaderValue <Double_t> Chi2Ndof(*myReader,"Chi2Ndof");

  
  
  //*****************Declaring histograms to be filled********************


  //Angles 
  TH2D *anglesRecoGJ[NN];
  TH2D *anglesRecoHF[NN];


  for (int jj=0; jj< NN; jj++){
    anglesRecoGJ[jj] = new TH2D(Form("AnglesRecoGJ_bin%d",jj),"",3*DEFBINS,-1.,1.,3*DEFBINS,-180.,180.);
    anglesRecoHF[jj] = new TH2D(Form("AnglesRecoHF_bin%d",jj),"",3*DEFBINS,-1.,1.,3*DEFBINS,-180.,180.);  
  }  
  

  //3-particle van Hove plot 
  TH2D *vanHoveXY_ppipLX = new TH2D("vanHoveXY_for_ppipLX","",3*DEFBINS,-2.5,2.5,3*DEFBINS,-2.5,2.5);

    
  
  //Masses against momentum transfer
  TH2D *pipUpimpim_mass_vs_minust = new TH2D("PiplusUPiminusPiminus_mass_vs_minust","",2*DEFBINS,0.2,3.2,DEFTBINS,0.,2.);
  TH2D *pipUpim_mass_vs_minust = new TH2D("PiplusUPiminus_mass_vs_minust","",2*DEFBINS,0.,3.,DEFTBINS,0.,2.);
  TH2D *ppipL_mass_vs_minust = new TH2D("ProtonPiplusL_mass_vs_minust","",2*DEFBINS,0.8,3.8,DEFTBINS,0.,2.);
  TH2D *fourpion_mass_vs_minust = new TH2D("FourPion_mass_vs_minust","",2*DEFBINS,0.5,3.5,DEFTBINS,0.,2.);

  
  

  
    
  cout << "\n\nMAIN EVENT LOOP OVER ~" << nevents/1000000 << "M EVENTS" << endl;
  wt = 1.;
  
  //************Main event loop********************************************
  while (myReader->Next()){
    ientry = myReader->GetCurrentEntry(); //if (ientry==10000) break;
    if (ientry%1000000==0){
      cout << "Arrived at event #" << ientry/1000000 << "M in the current file:" << tchain->GetCurrentFile()->GetName() << endl;
    ////dectree->Show(ientry);
    }

    //FILLING DESIRED HISTOGRAMS      
    wt = *TSlopeWeight;
    

    
    pipUpimpim_mass_vs_minust->Fill(*PiplusUPiminus1Piminus2_M,-*tvar,wt); 
    ppipL_mass_vs_minust->Fill(*PiplusLProton_M,-*tvar,wt);
    fourpion_mass_vs_minust->Fill(*FourPions_M,-*tvar,wt);

    pipUpim_mass_vs_minust->Fill(*PiplusUPiminus1_M,-*tvar,wt); 
    pipUpim_mass_vs_minust->Fill(*PiplusUPiminus2_M,-*tvar,wt); 

    
    
    UV_P4 = *PiplusU_P4+*Piminus1_P4+*Piminus2_P4;      
    vanHoveXY_ppipLX->Fill(vanHoveX(*Proton_P4,*PiplusL_P4,UV_P4),vanHoveY(*Proton_P4,*PiplusL_P4,UV_P4),wt);    


    anglesRecoGJ[0]->Fill(*CosThetaPiminus1Bachelor_GJ,*PhiPiminus1Bachelor_GJ,wt);
    anglesRecoGJ[0]->Fill(*CosThetaPiminus2Bachelor_GJ,*PhiPiminus2Bachelor_GJ,wt);

    anglesRecoHF[0]->Fill(*CosThetaPiminus2Isobar_HF,*PhiPiminus2Isobar_HF,wt);
    anglesRecoHF[0]->Fill(*CosThetaPiminus1Isobar_HF,*PhiPiminus1Isobar_HF,wt);

    
    if (*tvar > -0.25 && *tvar <=0){
      anglesRecoGJ[1]->Fill(*CosThetaPiminus1Bachelor_GJ,*PhiPiminus1Bachelor_GJ,wt);
      anglesRecoGJ[1]->Fill(*CosThetaPiminus2Bachelor_GJ,*PhiPiminus2Bachelor_GJ,wt);
      anglesRecoHF[1]->Fill(*CosThetaPiminus2Isobar_HF,*PhiPiminus2Isobar_HF,wt);
      anglesRecoHF[1]->Fill(*CosThetaPiminus1Isobar_HF,*PhiPiminus1Isobar_HF,wt);    
    }
    if (*tvar > -0.5 && *tvar <= -0.25){
      anglesRecoGJ[2]->Fill(*CosThetaPiminus1Bachelor_GJ,*PhiPiminus1Bachelor_GJ,wt);
      anglesRecoGJ[2]->Fill(*CosThetaPiminus2Bachelor_GJ,*PhiPiminus2Bachelor_GJ,wt);
      anglesRecoHF[2]->Fill(*CosThetaPiminus2Isobar_HF,*PhiPiminus2Isobar_HF,wt);
      anglesRecoHF[2]->Fill(*CosThetaPiminus1Isobar_HF,*PhiPiminus1Isobar_HF,wt);    
    }
    if (*tvar > -0.75 && *tvar <= -0.5){
      anglesRecoGJ[3]->Fill(*CosThetaPiminus1Bachelor_GJ,*PhiPiminus1Bachelor_GJ,wt);
      anglesRecoGJ[3]->Fill(*CosThetaPiminus2Bachelor_GJ,*PhiPiminus2Bachelor_GJ,wt);
      anglesRecoHF[3]->Fill(*CosThetaPiminus2Isobar_HF,*PhiPiminus2Isobar_HF,wt);
      anglesRecoHF[3]->Fill(*CosThetaPiminus1Isobar_HF,*PhiPiminus1Isobar_HF,wt);    
    }
    if (*tvar > -1. && *tvar <= -0.75){
      anglesRecoGJ[4]->Fill(*CosThetaPiminus1Bachelor_GJ,*PhiPiminus1Bachelor_GJ,wt);
      anglesRecoGJ[4]->Fill(*CosThetaPiminus2Bachelor_GJ,*PhiPiminus2Bachelor_GJ,wt);
      anglesRecoHF[4]->Fill(*CosThetaPiminus2Isobar_HF,*PhiPiminus2Isobar_HF,wt);
      anglesRecoHF[4]->Fill(*CosThetaPiminus1Isobar_HF,*PhiPiminus1Isobar_HF,wt);    
    }
    if (*tvar >= -2. && *tvar <= -1.){
      anglesRecoGJ[5]->Fill(*CosThetaPiminus1Bachelor_GJ,*PhiPiminus1Bachelor_GJ,wt);
      anglesRecoGJ[5]->Fill(*CosThetaPiminus2Bachelor_GJ,*PhiPiminus2Bachelor_GJ,wt);
      anglesRecoHF[5]->Fill(*CosThetaPiminus2Isobar_HF,*PhiPiminus2Isobar_HF,wt);
      anglesRecoHF[5]->Fill(*CosThetaPiminus1Isobar_HF,*PhiPiminus1Isobar_HF,wt);    
    }

      
    }


  pipUpimpim_mass_vs_minust->Write();
  pipUpim_mass_vs_minust->Write();
  ppipL_mass_vs_minust->Write();
  fourpion_mass_vs_minust->Write();
    
      
  vanHoveXY_ppipLX->Write();

  for (int jj=0; jj< NN; jj++){
    anglesRecoGJ[jj]->Write();
    anglesRecoHF[jj]->Write();
  }

    
    
}




void write_correlations_mcwt(const char *input0, const char *output)
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



