#include "calc_angles.h"



void printListOfBranches(TChain *dectree)
{

  dectree->SetBranchStatus("*",0);
  dectree->SetBranchStatus("*_P4",1);  

  
  TList:TObjArray *brslist = dectree->TChain::GetListOfBranches();
  brslist->SetOwner(kFALSE); 
  brslist->Sort();  
  for(int ii = 0; ii < brslist->GetEntries(); ii++)
    cout << "BR #"<< ii << "\t\t" << brslist->At(ii)->GetTitle() << endl; 

}




void LoopOverTree(TChain *tchain)
{
   
  
  const Int_t DEFBINS = 100;
  const Float_t ProtonMass = 0.938272;

  
  //*******************local variables****************************
  Double_t  wt,ntracks;
  Long64_t ientry;
  Bool_t  isFile1, isFile2; 
  TLorentzVector *Piminus1_TP4,*Piminus2_TP4;
  
  
  //********************************Reading the tree using TTreeReader**************************************
  TTreeReader *myReader = new TTreeReader(tchain);
  Long64_t nevents = myReader->TTreeReader::GetEntries(true);

  

  //MOMENTA
  TTreeReaderValue <TLorentzVector> PiplusL_TP4(*myReader, "Thrown_PiplusL_P4");
  TTreeReaderValue <TLorentzVector> PiplusU_TP4(*myReader, "Thrown_PiplusU_P4");
  TTreeReaderValue <TLorentzVector> Piminus1_TP4_unsorted(*myReader, "Thrown_Piminus1_P4");
  TTreeReaderValue <TLorentzVector> Piminus2_TP4_unsorted(*myReader, "Thrown_Piminus2_P4");
  TTreeReaderValue <TLorentzVector> Proton_TP4(*myReader, "Thrown_Proton_P4");


  TTreeReaderValue <TLorentzVector> Piplus2_P4(*myReader, "PiplusL_P4");
  TTreeReaderValue <TLorentzVector> Piplus1_P4(*myReader, "PiplusU_P4");
  TTreeReaderValue <TLorentzVector> Piminus1_P4(*myReader, "Piminus1_P4");
  TTreeReaderValue <TLorentzVector> Piminus2_P4(*myReader, "Piminus2_P4");
  TTreeReaderValue <TLorentzVector> Proton_P4(*myReader, "Proton_P4");

  
  
  //OTHER
  TTreeReaderValue <Double_t> RFTimeWeight(*myReader,"RFTimeWeight");
  TTreeReaderValue <Double_t> tvar(*myReader,"tvar");
  TTreeReaderValue <UInt_t> NumUnusedTracks(*myReader,"NumUnusedTracks");
 



  
  
  //*****************Declaring histograms to be filled********************

  //Illustration for the pi+ separation
  TH1D *theta_piplus1 = new TH1D("Theta_piplus1","",DEFBINS,0.,180.);
  TH1D *theta_piplus2 = new TH1D("Theta_piplus2","",DEFBINS,0.,180.);
  TH1D *theta_piplusU = new TH1D("Theta_piplusU","",DEFBINS,0.,180.);
  TH1D *theta_piplusL = new TH1D("Theta_piplusL","",DEFBINS,0.,180.);
  TH1D *mag_piplus1 = new TH1D("Mag_piplus1","",DEFBINS,0.,7.);
  TH1D *mag_piplus2 = new TH1D("Mag_piplus2","",DEFBINS,0.,7.);
  TH1D *mag_piplusU = new TH1D("Mag_piplusU","",DEFBINS,0.,7.);
  TH1D *mag_piplusL = new TH1D("Mag_piplusL","",DEFBINS,0.,7.);


  //Resolutions
  TH1D *deltaMag_proton = new TH1D("DeltaMag_proton","",DEFBINS,-0.4,0.4);
  TH1D *deltaMag_piplusU = new TH1D("DeltaMag_piplusU","",DEFBINS,-0.4,0.4);
  TH1D *deltaMag_piplusL = new TH1D("DeltaMag_piplusL","",DEFBINS,-0.4,0.4);
  TH1D *deltaMag_piminus1 = new TH1D("DeltaMag_piminus1","",DEFBINS,-0.4,0.4);
  TH1D *deltaMag_piminus2 = new TH1D("DeltaMag_piminus2","",DEFBINS,-0.4,0.4);


  TH1D *deltaTheta_proton = new TH1D("DeltaTheta_proton","",DEFBINS,-5.,5.);
  TH1D *deltaTheta_piplusU = new TH1D("DeltaTheta_piplusU","",DEFBINS,-5.,5.);
  TH1D *deltaTheta_piplusL = new TH1D("DeltaTheta_piplusL","",DEFBINS,-5.,5.);
  TH1D *deltaTheta_piminus1 = new TH1D("DeltaTheta_piminus1","",DEFBINS,-5.,5.);
  TH1D *deltaTheta_piminus2 = new TH1D("DeltaTheta_piminus2","",DEFBINS,-5.,5.);
 

  TH1D *deltaPhi_proton = new TH1D("DeltaPhi_proton","",DEFBINS,-5.,5.);  
  TH1D *deltaPhi_piminus1 = new TH1D("DeltaPhi_piminus1","",DEFBINS,-5.,5.);  
  TH1D *deltaPhi_piminus2 = new TH1D("DeltaPhi_piminus2","",DEFBINS,-5.,5.);  
  TH1D *deltaPhi_piplusU = new TH1D("DeltaPhi_piplusU","",DEFBINS,-5.,5.);
  TH1D *deltaPhi_piplusL = new TH1D("DeltaPhi_piplusL","",DEFBINS,-5.,5.);

  

    
  
    
  cout << "\n\nMAIN EVENT LOOP OVER ~" << nevents/1000000 << "M EVENTS" << endl;
  wt = 1.;
  
  //************Main event loop********************************************
  while (myReader->Next()){
    ientry = myReader->GetCurrentEntry(); //if (ientry==100000) break;
    if (ientry%1000000==0){
      cout << "Arrived at event #" << ientry/1000000 << "M in the current file:" << tchain->GetCurrentFile()->GetName() << endl;
    ////dectree->Show(ientry);
    }

   
    
    
    //FILLING HISTOGRAMS ILLUSTRATING THE PI+'S SEPARATION      
    theta_piplus1->Fill(TMath::RadToDeg()*Piplus1_P4->Vect().Theta(),wt);
    theta_piplus2->Fill(TMath::RadToDeg()*Piplus2_P4->Vect().Theta(),wt);
    theta_piplusU->Fill(TMath::RadToDeg()*PiplusU_TP4->Vect().Theta(),wt);
    theta_piplusL->Fill(TMath::RadToDeg()*PiplusL_TP4->Vect().Theta(),wt);
    mag_piplus1->Fill(Piplus1_P4->Vect().Mag(),wt);
    mag_piplus2->Fill(Piplus2_P4->Vect().Mag(),wt);
    mag_piplusU->Fill(PiplusU_TP4->Vect().Mag(),wt);
    mag_piplusL->Fill(PiplusL_TP4->Vect().Mag(),wt);



    //SORT THE TWO THROWN PI-'S
    if (Piminus1_TP4_unsorted->Vect().Mag() < Piminus2_TP4_unsorted->Vect().Mag())
      { Piminus2_TP4 = &(*Piminus1_TP4_unsorted); Piminus1_TP4 = &(*Piminus2_TP4_unsorted); }
    else
      {Piminus1_TP4 = &(*Piminus1_TP4_unsorted); Piminus2_TP4 = &(*Piminus2_TP4_unsorted); }
      

    //FILLING HISTOGRAMS WITH RESOLUTIONS
   deltaMag_proton->Fill(Proton_P4->Vect().Mag()-Proton_TP4->Vect().Mag(),wt);
   deltaMag_piplusU->Fill(Piplus1_P4->Vect().Mag()-PiplusU_TP4->Vect().Mag(),wt);
   deltaMag_piplusL->Fill(Piplus2_P4->Vect().Mag()-PiplusL_TP4->Vect().Mag(),wt);
   deltaMag_piminus1->Fill(Piminus1_P4->Vect().Mag()-Piminus1_TP4->Vect().Mag(),wt);
   deltaMag_piminus2->Fill(Piminus2_P4->Vect().Mag()-Piminus2_TP4->Vect().Mag(),wt);
 
   
   deltaTheta_proton->Fill(TMath::RadToDeg()*(Proton_P4->Vect().Theta()-Proton_TP4->Vect().Theta()),wt);   
   deltaTheta_piplusU->Fill(TMath::RadToDeg()*(Piplus1_P4->Vect().Theta()-PiplusU_TP4->Vect().Theta()),wt);
   deltaTheta_piplusL->Fill(TMath::RadToDeg()*(Piplus2_P4->Vect().Theta()-PiplusL_TP4->Vect().Theta()),wt);
   deltaTheta_piminus1->Fill(TMath::RadToDeg()*(Piminus1_P4->Vect().Theta()-Piminus1_TP4->Vect().Theta()),wt);
   deltaTheta_piminus2->Fill(TMath::RadToDeg()*(Piminus2_P4->Vect().Theta()-Piminus2_TP4->Vect().Theta()),wt);

   
   
   deltaPhi_proton->Fill(TMath::RadToDeg()*(Proton_P4->Vect().Phi()-Proton_TP4->Vect().Phi()),wt);
   deltaPhi_piplusU->Fill(TMath::RadToDeg()*(Piplus1_P4->Vect().Phi()-PiplusU_TP4->Vect().Phi()),wt);
   deltaPhi_piplusL->Fill(TMath::RadToDeg()*(Piplus2_P4->Vect().Phi()-PiplusL_TP4->Vect().Phi()),wt);
   deltaPhi_piminus1->Fill(TMath::RadToDeg()*(Piminus1_P4->Vect().Phi()-Piminus1_TP4->Vect().Phi()),wt);
   deltaPhi_piminus2->Fill(TMath::RadToDeg()*(Piminus2_P4->Vect().Phi()-Piminus2_TP4->Vect().Phi()),wt);

   
    }

  
    theta_piplus1->Write();
    theta_piplus2->Write();
    theta_piplusL->Write();
    theta_piplusU->Write();

    mag_piplus1->Write();
    mag_piplus2->Write();
    mag_piplusL->Write();
    mag_piplusU->Write();


    //Normalize each histogram by its integral value  
    deltaMag_proton->Scale(1./deltaMag_proton->GetMaximum());
    deltaMag_piplusU->Scale(1./deltaMag_piplusU->GetMaximum());
    deltaMag_piplusL->Scale(1./deltaMag_piplusL->GetMaximum());
    deltaMag_piminus1->Scale(1./deltaMag_piminus1->GetMaximum());
    deltaMag_piminus2->Scale(1./deltaMag_piminus2->GetMaximum());
  
    deltaTheta_proton->Scale(1./deltaTheta_proton->GetMaximum());
    deltaTheta_piplusU->Scale(1./deltaTheta_piplusU->GetMaximum());
    deltaTheta_piplusL->Scale(1./deltaTheta_piplusL->GetMaximum());
    deltaTheta_piminus1->Scale(1./deltaTheta_piminus1->GetMaximum());
    deltaTheta_piminus2->Scale(1./deltaTheta_piminus2->GetMaximum());

    deltaPhi_proton->Scale(1./deltaPhi_proton->GetMaximum());
    deltaPhi_piplusU->Scale(1./deltaPhi_piplusU->GetMaximum());
    deltaPhi_piplusL->Scale(1./deltaPhi_piplusL->GetMaximum());
    deltaPhi_piminus1->Scale(1./deltaPhi_piminus1->GetMaximum());
    deltaPhi_piminus2->Scale(1./deltaPhi_piminus2->GetMaximum());

    
    
    deltaMag_proton->Write();
    deltaMag_piplusU->Write();
    deltaMag_piplusL->Write();
    deltaMag_piminus1->Write();
    deltaMag_piminus2->Write();
  
    deltaTheta_proton->Write();
    deltaTheta_piplusU->Write();
    deltaTheta_piplusL->Write();
    deltaTheta_piminus1->Write();
    deltaTheta_piminus2->Write();

    deltaPhi_proton->Write();
    deltaPhi_piplusU->Write();
    deltaPhi_piplusL->Write();
    deltaPhi_piminus1->Write();
    deltaPhi_piminus2->Write();

      
}




void write_resolutions(const char *input0, const char *output)
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


    printListOfBranches(dectree);
    LoopOverTree(dectree);


  
    fileOut->Close();
    file0->Close();
    //    file1->Close();
    //    file2->Close();
    //    file3->Close();
    //    file4->Close();
    //    file5->Close();

}
