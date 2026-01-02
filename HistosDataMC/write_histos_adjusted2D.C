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
  const Float_t ProtonMass = 0.938272;

  
  //*******************local variables****************************
  Double_t  wt, bz, deltatheta, deltaphi;
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


  
  
  //MASSES SQUARED
  TTreeReaderValue <Double_t> PiplusLPiminus1_M2(*myReader,"PiplusLPiminus1_M2");
  TTreeReaderValue <Double_t> PiplusLPiminus2_M2(*myReader,"PiplusLPiminus2_M2");    
  TTreeReaderValue <Double_t> PiplusUPiminus1_M2(*myReader,"PiplusUPiminus1_M2");
  TTreeReaderValue <Double_t> PiplusUPiminus2_M2(*myReader,"PiplusUPiminus2_M2");

  TTreeReaderValue <Double_t> PiplusLPiplusUPiminus1_M2(*myReader,"PiplusLPiplusUPiminus1_M2");
  TTreeReaderValue <Double_t> PiplusLPiplusUPiminus2_M2(*myReader,"PiplusLPiplusUPiminus2_M2");
  TTreeReaderValue <Double_t> PiplusLPiminus1Piminus2_M2(*myReader,"PiplusLPiminus1Piminus2_M2");
  TTreeReaderValue <Double_t> PiplusUPiminus1Piminus2_M2(*myReader,"PiplusUPiminus1Piminus2_M2");

  TTreeReaderValue <Double_t> PiplusLProton_M2(*myReader,"PiplusLProton_M2");
  TTreeReaderValue <Double_t> PiplusUProton_M2(*myReader,"PiplusUProton_M2");
  TTreeReaderValue <Double_t> Piminus1Proton_M2(*myReader,"Piminus1Proton_M2");
  TTreeReaderValue <Double_t> Piminus2Proton_M2(*myReader,"Piminus2Proton_M2");

  TTreeReaderValue <Double_t> ProtonPiplusLPiminus1_M2(*myReader,"ProtonPiplusLPiminus1_M2");
  TTreeReaderValue <Double_t> ProtonPiplusLPiminus2_M2(*myReader,"ProtonPiplusLPiminus2_M2");    
  TTreeReaderValue <Double_t> ProtonPiplusUPiminus1_M2(*myReader,"ProtonPiplusUPiminus1_M2");
  TTreeReaderValue <Double_t> ProtonPiplusUPiminus2_M2(*myReader,"ProtonPiplusUPiminus2_M2");

  TTreeReaderValue <Double_t> ProtonPiplusLPiplusUPiminus1_M2(*myReader,"ProtonPiplusLPiplusUPiminus1_M2");
  TTreeReaderValue <Double_t> ProtonPiplusLPiplusUPiminus2_M2(*myReader,"ProtonPiplusLPiplusUPiminus2_M2");
  TTreeReaderValue <Double_t> ProtonPiplusLPiminus1Piminus2_M2(*myReader,"ProtonPiplusLPiminus1Piminus2_M2");
  TTreeReaderValue <Double_t> ProtonPiplusUPiminus1Piminus2_M2(*myReader,"ProtonPiplusUPiminus1Piminus2_M2");

  TTreeReaderValue <Double_t> FourPions_M2(*myReader,"FourPions_M2");

  
  
  //OTHER
  TTreeReaderValue <Double_t> RFTimeWeight(*myReader,"RFTimeWeight");
  TTreeReaderValue <Double_t> tvar(*myReader,"tvar");
  TTreeReaderValue <Double_t> Miss_M2(*myReader,"Miss_M2");
  TTreeReaderValue <Float_t> E_Beam(*myReader,"E_Beam");
  TTreeReaderValue <Double_t> Chi2Ndof(*myReader,"Chi2Ndof");

  

  
  
  //*****************Declaring histograms to be filled********************

  TH1D *piplusLpiminus1_mass = new TH1D("PiplusLPiminus1_M","",DEFBINS,0.,3.);
  TH1D *piplusLpiminus2_mass = new TH1D("PiplusLPiminus2_M","",DEFBINS,0.,3.);
  TH1D *piplusUpiminus1_mass = new TH1D("PiplusUPiminus1_M","",DEFBINS,0.,3.);
  TH1D *piplusUpiminus2_mass = new TH1D("PiplusUPiminus2_M","",DEFBINS,0.,3.);

  TH1D *piplusLpiplusUpiminus1_mass = new TH1D("PiplusLPiplusUPiminus1_M","",DEFBINS,0.2,3.2);
  TH1D *piplusLpiplusUpiminus2_mass = new TH1D("PiplusLPiplusUPiminus2_M","",DEFBINS,0.2,3.2);
  TH1D *piplusLpiminus1piminus2_mass = new TH1D("PiplusLPiminus1Piminus2_M","",DEFBINS,0.2,3.2);
  TH1D *piplusUpiminus1piminus2_mass = new TH1D("PiplusUPiminus1Piminus2_M","",DEFBINS,0.2,3.2);
  
  TH1D *piplusLproton_mass = new TH1D("PiplusLProton_M","",DEFBINS,0.8,3.8);
  TH1D *piplusUproton_mass = new TH1D("PiplusUProton_M","",DEFBINS,0.8,3.8);
  TH1D *piminus1proton_mass = new TH1D("Piminus1Proton_M","",DEFBINS,0.8,3.8);
  TH1D *piminus2proton_mass = new TH1D("Piminus2Proton_M","",DEFBINS,0.8,3.8);

  TH1D *protonpiplusLpiminus1_mass = new TH1D("ProtonPiplusLPiminus1_M","",DEFBINS,1.,4.);
  TH1D *protonpiplusLpiminus2_mass = new TH1D("ProtonPiplusLPiminus2_M","",DEFBINS,1.,4.);
  TH1D *protonpiplusUpiminus1_mass = new TH1D("ProtonPiplusUPiminus1_M","",DEFBINS,1.,4.);
  TH1D *protonpiplusUpiminus2_mass = new TH1D("ProtonPiplusUPiminus2_M","",DEFBINS,1.,4.);

  TH1D *protonpiplusLpiplusUpiminus1_mass = new TH1D("ProtonPiplusLPiplusUPiminus1_M","",DEFBINS,1.,4.5);
  TH1D *protonpiplusLpiplusUpiminus2_mass = new TH1D("ProtonPiplusLPiplusUPiminus2_M","",DEFBINS,1.,4.5);
  TH1D *protonpiplusLpiminus1piminus2_mass = new TH1D("ProtonPiplusLPiminus1Piminus2_M","",DEFBINS,1.5,4.5);
  TH1D *protonpiplusUpiminus1piminus2_mass = new TH1D("ProtonPiplusUPiminus1Piminus2_M","",DEFBINS,2.0,4.5);

  TH1D *fourpions_mass = new TH1D("FourPions_M","",DEFBINS,0.5,3.5);

  
  TH1D *tslope = new TH1D("minustvar","",DEFBINS,0.,2.);
  TH1D *missm2 = new TH1D("Miss_M2","",DEFBINS,-0.04,0.04);
  TH1D *EnBeam  = new TH1D("E_Beam","",DEFBINS,8.0,9.0);
  TH1D *chi2ndof  = new TH1D("Chi2Ndof","",DEFBINS,0.,5.);


  TH1D *piminus1_p = new TH1D("Piminus1_P","",DEFBINS,0.,8.);
  TH1D *piplusL_p = new TH1D("PiplusL_P","",DEFBINS,0.,8.);
  TH1D *piminus2_p = new TH1D("Piminus2_P","",DEFBINS,0.,8.);
  TH1D *piplusU_p = new TH1D("PiplusU_P","",DEFBINS,0.,8.);
  TH1D *proton_p = new TH1D("Proton_P","",DEFBINS,0.,4.);
 
  TH1D *piminus1_pz = new TH1D("Piminus1_Pz","",DEFBINS,0.,8.);
  TH1D *piplusL_pz = new TH1D("PiplusL_Pz","",DEFBINS,0.,8.);
  TH1D *piminus2_pz = new TH1D("Piminus2_Pz","",DEFBINS,0.,8.);
  TH1D *piplusU_pz = new TH1D("PiplusU_Pz","",DEFBINS,0.,8.);
  TH1D *proton_pz = new TH1D("Proton_Pz","",DEFBINS,0.,4.);

  TH1D *deltaphi_lv_uv = new TH1D("DeltaPhi_LV_UV","",DEFBINS,170.,190.);
  TH1D *deltaphi_lv_uv_wrong = new TH1D("DeltaPhi_LV_UV_wrong","",DEFBINS,170.,190.);

  TH1D *deltatheta_lv_uv = new TH1D("DeltaTheta_LV_UV","",DEFBINS,170.,190.);
  TH1D *deltatheta_lv_uv_wrong = new TH1D("DeltaTheta_LV_UV_wrong","",DEFBINS,170.,190.);


  
  //2-DIM HISTOGRAMS
  
  TH2D *pippim_vs_ppip_masses = new TH2D("Dalitz_pippim_vs_ppip","",5*DEFBINS,0.,6.,5*DEFBINS,0.,12.);
  TH2D *pippimpim_vs_ppip_masses = new TH2D("Dalitz_pippimpim_vs_ppip","",5*DEFBINS,0.,9.,5*DEFBINS,0.,12.);
  TH2D *pippim_vs_ppip_masses_wrong = new TH2D("Dalitz_pippim_vs_ppip_wrong","",5*DEFBINS,0.,6.,5*DEFBINS,0.,12.);
  TH2D *pippimpim_vs_ppip_masses_wrong = new TH2D("Dalitz_pippimpim_vs_ppip_wrong","",5*DEFBINS,0.,9.,5*DEFBINS,0.,12.);

  
  TH2D *pipL_mom_vs_pipU_mom = new TH2D("PiplusU_P_vs_PiplusL_P","",5*DEFBINS,0.,2.,5*DEFBINS,0.,8.);
  TH2D *pim1_mom_vs_pim2_mom = new TH2D("Piminus1_P_vs_Piminus2_P","",5*DEFBINS,0.,8.,5*DEFBINS,0.,8.);


  //masses against piplus from the Upper vertex momentum
  TH2D *ppipUpimpim_mass_vs_pipU_mom = new TH2D("PiplusU_P_vs_ProtonPiplusUPiminus1Piminus2_mass","",5*DEFBINS,2.5,4.5,5*DEFBINS,0.,8.);
  TH2D *ppipLpimpim_mass_vs_pipU_mom = new TH2D("PiplusU_P_vs_ProtonPiplusLPiminus1Piminus2_mass","",5*DEFBINS,1.5,4.5,5*DEFBINS,0.,8.);
  TH2D *pipLpipUpim1_mass_vs_pipU_mom = new TH2D("PiplusU_P_vs_PiplusLPiplusUPiminus1_mass","",5*DEFBINS,0,3.5,5*DEFBINS,0.,8.);
  TH2D *pipLpipUpim2_mass_vs_pipU_mom = new TH2D("PiplusU_P_vs_PiplusLPiplusUPiminus2_mass","",5*DEFBINS,0,3.5,5*DEFBINS,0.,8.);
  TH2D *fourpion_mass_vs_pipU_mom = new TH2D("PiplusU_P_vs_FourPion_mass","",5*DEFBINS,0.5,3.5,5*DEFBINS,0.,8.);

  
  //masses against piminus1 momentum
  TH2D *ppipUpimpim_mass_vs_pim1_mom = new TH2D("Piminus1_P_vs_ProtonPiplusUPiminus1Piminus2_mass","",5*DEFBINS,2.5,4.5,5*DEFBINS,0.,8.);
  TH2D *ppipLpimpim_mass_vs_pim1_mom = new TH2D("Piminus1_P_vs_ProtonPiplusLPiminus1Piminus2_mass","",5*DEFBINS,1.5,4.5,5*DEFBINS,0.,8.);
  TH2D *pipLpipUpim1_mass_vs_pim1_mom = new TH2D("Piminus1_P_vs_PiplusLPiplusUPiminus1_mass","",5*DEFBINS,0,3.5,5*DEFBINS,0.,8.);
  TH2D *pipLpipUpim2_mass_vs_pim1_mom = new TH2D("Piminus1_P_vs_PiplusLPiplusUPiminus2_mass","",5*DEFBINS,0,3.5,5*DEFBINS,0.,8.);
  TH2D *fourpion_mass_vs_pim1_mom = new TH2D("Piminus1_P_vs_FourPion_mass","",5*DEFBINS,0.5,3.5,5*DEFBINS,0.,8.);

   //masses against piminus2 momentum
  TH2D *ppipUpimpim_mass_vs_pim2_mom = new TH2D("Piminus2_P_vs_ProtonPiplusUPiminus1Piminus2_mass","",5*DEFBINS,2.5,4.5,5*DEFBINS,0.,8.);
  TH2D *ppipLpimpim_mass_vs_pim2_mom = new TH2D("Piminus2_P_vs_ProtonPiplusLPiminus1Piminus2_mass","",5*DEFBINS,1.5,4.5,5*DEFBINS,0.,8.);
  TH2D *pipLpipUpim1_mass_vs_pim2_mom = new TH2D("Piminus2_P_vs_PiplusLPiplusUPiminus1_mass","",5*DEFBINS,0,3.5,5*DEFBINS,0.,8.);
  TH2D *pipLpipUpim2_mass_vs_pim2_mom = new TH2D("Piminus2_P_vs_PiplusLPiplusUPiminus2_mass","",5*DEFBINS,0,3.5,5*DEFBINS,0.,8.);
  TH2D *fourpion_mass_vs_pim2_mom = new TH2D("Piminus2_P_vs_FourPion_mass","",5*DEFBINS,0.5,3.5,5*DEFBINS,0.,8.);

  
  //masses against momentum transfer
  TH2D *ppipUpimpim_mass_vs_minust = new TH2D("minust_vs_ProtonPiplusUPiminus1Piminus2_mass","",5*DEFBINS,2.5,4.5,5*DEFBINS,0.,2.);
  TH2D *ppipLpimpim_mass_vs_minust = new TH2D("minust_vs_ProtonPiplusLPiminus1Piminus2_mass","",5*DEFBINS,1.5,4.5,5*DEFBINS,0.,2.);
  TH2D *pipLpipUpim1_mass_vs_minust = new TH2D("minust_vs_PiplusLPiplusUPiminus1_mass","",5*DEFBINS,0,3.5,5*DEFBINS,0.,2.);
  TH2D *pipLpipUpim2_mass_vs_minust = new TH2D("minust_vs_PiplusLPiplusUPiminus2_mass","",5*DEFBINS,0,3.5,5*DEFBINS,0.,2.);
  TH2D *fourpion_mass_vs_minust = new TH2D("minust_vs_FourPion_mass","",5*DEFBINS,0.5,3.5,5*DEFBINS,0.,2.);
 

  //van Hove plots 
  TH2D *vanHoveXY_ppipLX = new TH2D("vanHoveXY_for_ppipLX","",5*DEFBINS,-2.5,2.5,5*DEFBINS,-2.5,2.5);
  
  
    
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
    wt = *RFTimeWeight; 
    piplusLpiminus1_mass->Fill(*PiplusLPiminus1_M,wt);
    piplusLpiminus2_mass->Fill(*PiplusLPiminus2_M,wt);
    piplusUpiminus1_mass->Fill(*PiplusUPiminus1_M,wt);
    piplusUpiminus2_mass->Fill(*PiplusUPiminus2_M,wt);

    piplusLpiplusUpiminus1_mass->Fill(*PiplusLPiplusUPiminus1_M,wt);
    piplusLpiplusUpiminus2_mass->Fill(*PiplusLPiplusUPiminus2_M,wt);
    piplusLpiminus1piminus2_mass->Fill(*PiplusLPiminus1Piminus2_M,wt);
    piplusUpiminus1piminus2_mass->Fill(*PiplusUPiminus1Piminus2_M,wt);
  
    piplusLproton_mass->Fill(*PiplusLProton_M,wt);
    piplusUproton_mass->Fill(*PiplusUProton_M,wt);
    piminus1proton_mass->Fill(*Piminus1Proton_M,wt);
    piminus2proton_mass->Fill(*Piminus2Proton_M,wt);


    protonpiplusLpiminus1_mass->Fill(*ProtonPiplusLPiminus1_M,wt);
    protonpiplusLpiminus2_mass->Fill(*ProtonPiplusLPiminus2_M,wt);
    protonpiplusUpiminus1_mass->Fill(*ProtonPiplusUPiminus1_M,wt);
    protonpiplusUpiminus2_mass->Fill(*ProtonPiplusUPiminus2_M,wt);
                                      
    protonpiplusLpiplusUpiminus1_mass->Fill(*ProtonPiplusLPiplusUPiminus1_M,wt);
    protonpiplusLpiplusUpiminus2_mass->Fill(*ProtonPiplusLPiplusUPiminus2_M,wt);
    protonpiplusLpiminus1piminus2_mass->Fill(*ProtonPiplusLPiminus1Piminus2_M,wt);
    protonpiplusUpiminus1piminus2_mass->Fill(*ProtonPiplusUPiminus1Piminus2_M,wt);

    fourpions_mass->Fill(*FourPions_M,wt);
    

    tslope->Fill(-*tvar,wt);
    missm2->Fill(*Miss_M2,wt);
    EnBeam->Fill(*E_Beam,wt);
    chi2ndof->Fill(*Chi2Ndof,1.);


    piminus1_p->Fill(Piminus1_P4->Vect().Mag(),wt);
    piplusL_p->Fill(PiplusL_P4->Vect().Mag(),wt);
    piminus2_p->Fill(Piminus2_P4->Vect().Mag(),wt);
    piplusU_p->Fill(PiplusU_P4->Vect().Mag(),wt);
    proton_p->Fill(Proton_P4->Vect().Mag(),wt);

    piminus1_pz->Fill(Piminus1_P4->Pz(),wt);
    piplusL_pz->Fill(PiplusL_P4->Pz(),wt);
    piminus2_pz->Fill(Piminus2_P4->Pz(),wt);
    piplusU_pz->Fill(PiplusU_P4->Pz(),wt);
    proton_pz->Fill(Proton_P4->Pz(),wt);

    
    //Continue with angles
        
    deltaphi=DeltaPhiLab(*PiplusL_MP4,*PiplusU_MP4,*Piminus1_MP4,*Piminus2_MP4,*Proton_MP4);
    deltaphi_lv_uv->Fill(deltaphi,wt);
    deltaphi=DeltaPhiLab(*PiplusU_MP4,*PiplusL_MP4,*Piminus1_MP4,*Piminus2_MP4,*Proton_MP4); 
    deltaphi_lv_uv_wrong->Fill(deltaphi,wt);

   
    bz = -Beam_Meas_P4->E()/(Beam_Meas_P4->E()+ProtonMass);

    deltatheta=DeltaThetaCOM(*PiplusL_MP4,*PiplusU_MP4,*Piminus1_MP4,*Piminus2_MP4,*Proton_MP4,bz);
    deltatheta_lv_uv->Fill(deltatheta,wt); 
    deltatheta=DeltaThetaCOM(*PiplusU_MP4,*PiplusL_MP4,*Piminus1_MP4,*Piminus2_MP4,*Proton_MP4,bz);  
    deltatheta_lv_uv_wrong->Fill(deltatheta,wt); 


    //Continue with 2dim histograms      
	
      pippim_vs_ppip_masses->Fill(*PiplusUPiminus1_M2,*PiplusLProton_M2,wt);
      pippim_vs_ppip_masses->Fill(*PiplusUPiminus2_M2,*PiplusLProton_M2,wt);
      pippimpim_vs_ppip_masses->Fill(*PiplusUPiminus1Piminus2_M2,*PiplusLProton_M2,wt);
      
      pippim_vs_ppip_masses_wrong->Fill(*PiplusLPiminus1_M2,*PiplusUProton_M2,wt);
      pippim_vs_ppip_masses_wrong->Fill(*PiplusLPiminus2_M2,*PiplusUProton_M2,wt);
      pippimpim_vs_ppip_masses_wrong->Fill(*PiplusLPiminus1Piminus2_M2,*PiplusUProton_M2,wt);


      pipL_mom_vs_pipU_mom->Fill(PiplusL_P4->Vect().Mag(),PiplusU_P4->Vect().Mag(),wt);
      pim1_mom_vs_pim2_mom->Fill(Piminus2_P4->Vect().Mag(),Piminus1_P4->Vect().Mag(),wt);

      
      
      ppipUpimpim_mass_vs_pipU_mom->Fill(*ProtonPiplusUPiminus1Piminus2_M,PiplusU_P4->Vect().Mag(),wt);
      ppipLpimpim_mass_vs_pipU_mom->Fill(*ProtonPiplusLPiminus1Piminus2_M,PiplusU_P4->Vect().Mag(),wt);
      pipLpipUpim1_mass_vs_pipU_mom->Fill(*PiplusLPiplusUPiminus1_M,PiplusU_P4->Vect().Mag(),wt);
      pipLpipUpim2_mass_vs_pipU_mom->Fill(*PiplusLPiplusUPiminus2_M,PiplusU_P4->Vect().Mag(),wt);
      fourpion_mass_vs_pipU_mom->Fill(*FourPions_M,PiplusU_P4->Vect().Mag(),wt);


      ppipUpimpim_mass_vs_pim1_mom->Fill(*ProtonPiplusUPiminus1Piminus2_M,Piminus1_P4->Vect().Mag(),wt);
      ppipLpimpim_mass_vs_pim1_mom->Fill(*ProtonPiplusLPiminus1Piminus2_M,Piminus1_P4->Vect().Mag(),wt);
      pipLpipUpim1_mass_vs_pim1_mom->Fill(*PiplusLPiplusUPiminus1_M,Piminus1_P4->Vect().Mag(),wt);
      pipLpipUpim2_mass_vs_pim1_mom->Fill(*PiplusLPiplusUPiminus2_M,Piminus1_P4->Vect().Mag(),wt);
      fourpion_mass_vs_pim1_mom->Fill(*FourPions_M,Piminus1_P4->Vect().Mag(),wt);

      
      ppipUpimpim_mass_vs_pim2_mom->Fill(*ProtonPiplusUPiminus1Piminus2_M,Piminus2_P4->Vect().Mag(),wt);
      ppipLpimpim_mass_vs_pim2_mom->Fill(*ProtonPiplusLPiminus1Piminus2_M,Piminus2_P4->Vect().Mag(),wt);
      pipLpipUpim1_mass_vs_pim2_mom->Fill(*PiplusLPiplusUPiminus1_M,Piminus2_P4->Vect().Mag(),wt);
      pipLpipUpim2_mass_vs_pim2_mom->Fill(*PiplusLPiplusUPiminus2_M,Piminus2_P4->Vect().Mag(),wt);
      fourpion_mass_vs_pim2_mom->Fill(*FourPions_M,Piminus2_P4->Vect().Mag(),wt);

      
      ppipUpimpim_mass_vs_minust->Fill(*ProtonPiplusUPiminus1Piminus2_M,-*tvar,wt);
      ppipLpimpim_mass_vs_minust->Fill(*ProtonPiplusLPiminus1Piminus2_M,-*tvar,wt);
      pipLpipUpim1_mass_vs_minust->Fill(*PiplusLPiplusUPiminus1_M,-*tvar,wt);
      pipLpipUpim2_mass_vs_minust->Fill(*PiplusLPiplusUPiminus2_M,-*tvar,wt);
      fourpion_mass_vs_minust->Fill(*FourPions_M,-*tvar,wt);


      UV_P4 = *PiplusU_P4+*Piminus1_P4+*Piminus2_P4;      
      vanHoveXY_ppipLX->Fill(vanHoveX(*Proton_P4,*PiplusL_P4,UV_P4),vanHoveY(*Proton_P4,*PiplusL_P4,UV_P4),wt);    

      
    }



  
  
    piplusLpiminus1_mass->Write();
    piplusLpiminus2_mass->Write();
    piplusUpiminus1_mass->Write();
    piplusUpiminus2_mass->Write();

    piplusLpiplusUpiminus1_mass->Write();
    piplusLpiplusUpiminus2_mass->Write();
    piplusLpiminus1piminus2_mass->Write();
    piplusUpiminus1piminus2_mass->Write();
  
    piplusLproton_mass->Write();
    piplusUproton_mass->Write();
    piminus1proton_mass->Write();
    piminus2proton_mass->Write();

    protonpiplusLpiminus1_mass->Write();
    protonpiplusLpiminus2_mass->Write();
    protonpiplusUpiminus1_mass->Write();
    protonpiplusUpiminus2_mass->Write();

    protonpiplusLpiplusUpiminus1_mass->Write();
    protonpiplusLpiplusUpiminus2_mass->Write();
    protonpiplusLpiminus1piminus2_mass->Write();
    protonpiplusUpiminus1piminus2_mass->Write();    

    fourpions_mass->Write();
    
    tslope->Write();
    missm2->Write();
    EnBeam->Write();
    chi2ndof->Write();

    piminus1_p->Write();
    piplusL_p->Write();
    piminus2_p->Write();
    piplusU_p->Write();
    proton_p->Write();

    piminus1_pz->Write();
    piplusL_pz->Write();
    piminus2_pz->Write();
    piplusU_pz->Write();
    proton_pz->Write();

    deltaphi_lv_uv->Write();
    deltaphi_lv_uv_wrong->Write();
    deltatheta_lv_uv->Write();
    deltatheta_lv_uv_wrong->Write();
    
        
    RemoveNegativeBins(pippim_vs_ppip_masses); pippim_vs_ppip_masses->Write(); 
    RemoveNegativeBins(pippimpim_vs_ppip_masses); pippimpim_vs_ppip_masses->Write();
    RemoveNegativeBins(pippim_vs_ppip_masses_wrong); pippim_vs_ppip_masses_wrong->Write();
    RemoveNegativeBins(pippimpim_vs_ppip_masses_wrong); pippimpim_vs_ppip_masses_wrong->Write();


    RemoveNegativeBins(pipL_mom_vs_pipU_mom); pipL_mom_vs_pipU_mom->Write();
    RemoveNegativeBins(pim1_mom_vs_pim2_mom); pim1_mom_vs_pim2_mom->Write();

      
      
    RemoveNegativeBins(ppipUpimpim_mass_vs_pipU_mom); ppipUpimpim_mass_vs_pipU_mom->Write();
    RemoveNegativeBins(ppipLpimpim_mass_vs_pipU_mom); ppipLpimpim_mass_vs_pipU_mom->Write();
    RemoveNegativeBins(pipLpipUpim1_mass_vs_pipU_mom); pipLpipUpim1_mass_vs_pipU_mom->Write();
    RemoveNegativeBins(pipLpipUpim2_mass_vs_pipU_mom); pipLpipUpim2_mass_vs_pipU_mom->Write();
    RemoveNegativeBins(fourpion_mass_vs_pipU_mom); fourpion_mass_vs_pipU_mom->Write();
	

    RemoveNegativeBins(ppipUpimpim_mass_vs_pim1_mom); ppipUpimpim_mass_vs_pim1_mom->Write();
    RemoveNegativeBins(ppipLpimpim_mass_vs_pim1_mom); ppipLpimpim_mass_vs_pim1_mom->Write();
    RemoveNegativeBins(pipLpipUpim1_mass_vs_pim1_mom); pipLpipUpim1_mass_vs_pim1_mom->Write();
    RemoveNegativeBins(pipLpipUpim2_mass_vs_pim1_mom); pipLpipUpim2_mass_vs_pim1_mom->Write();
    RemoveNegativeBins(fourpion_mass_vs_pim1_mom); fourpion_mass_vs_pim1_mom->Write();

    RemoveNegativeBins(ppipUpimpim_mass_vs_pim2_mom); ppipUpimpim_mass_vs_pim2_mom->Write();
    RemoveNegativeBins(ppipLpimpim_mass_vs_pim2_mom); ppipLpimpim_mass_vs_pim2_mom->Write();
    RemoveNegativeBins(pipLpipUpim1_mass_vs_pim2_mom); pipLpipUpim1_mass_vs_pim2_mom->Write();
    RemoveNegativeBins(pipLpipUpim2_mass_vs_pim2_mom); pipLpipUpim2_mass_vs_pim2_mom->Write();
    RemoveNegativeBins(fourpion_mass_vs_pim2_mom); fourpion_mass_vs_pim2_mom->Write();
    

    RemoveNegativeBins(ppipUpimpim_mass_vs_minust); ppipUpimpim_mass_vs_minust->Write();
    RemoveNegativeBins(ppipLpimpim_mass_vs_minust); ppipLpimpim_mass_vs_minust->Write();
    RemoveNegativeBins(pipLpipUpim1_mass_vs_minust); pipLpipUpim1_mass_vs_minust->Write();
    RemoveNegativeBins(pipLpipUpim2_mass_vs_minust); pipLpipUpim2_mass_vs_minust->Write();
    RemoveNegativeBins(fourpion_mass_vs_minust); fourpion_mass_vs_minust->Write();

    RemoveNegativeBins(vanHoveXY_ppipLX); vanHoveXY_ppipLX->Write();  
    
    
}




void write_histos_adjusted2D(const char *input0, const char *output)
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



