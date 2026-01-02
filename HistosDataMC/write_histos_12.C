#include "calc_angles.h"



void printListOfBranches(TChain *dectree)
{
  TList:TObjArray *brslist = dectree->TChain::GetListOfBranches();
  brslist->SetOwner(kFALSE); 
  brslist->Sort();  
   for(int ii = 0; ii < brslist->GetEntries(); ii++) 
     cout << "BR #"<< ii << "\t\t" << brslist->At(ii)->GetTitle() << endl; 
}




void LoopOverTree(TChain *tchain)
{
   
  //  tchain->SetBranchStatus("*",0);
  //  tchain->SetBranchStatus("*M",1);  
  
  const Int_t DEFBINS = 100;
  const Float_t ProtonMass = 0.938272;

  
  //*******************local variables****************************
  Double_t  wt, bz, deltatheta, deltaphi,diff_piplus12,diff_piminus12;
  Long64_t ientry;
  Bool_t  isFile1, isFile2; 

  TLorentzVector LV1_P4,LV2_P4,UV1_P4,UV2_P4,LV_P4,UV_P4;
  
  
  //********************************Reading the tree using TTreeReader**************************************
  TTreeReader *myReader = new TTreeReader(tchain);
  Long64_t nevents = myReader->TTreeReader::GetEntries(true);


  //MOMENTA
  TTreeReaderValue <TLorentzVector> Piplus2_P4(*myReader, "Piplus2_P4");
  TTreeReaderValue <TLorentzVector> Piplus1_P4(*myReader, "Piplus1_P4");
  TTreeReaderValue <TLorentzVector> Piminus1_P4(*myReader, "Piminus1_P4");
  TTreeReaderValue <TLorentzVector> Piminus2_P4(*myReader, "Piminus2_P4");
  TTreeReaderValue <TLorentzVector> Proton_P4(*myReader, "Proton_P4");

  TTreeReaderValue <TLorentzVector> Beam_Meas_P4(*myReader, "Beam_Meas_P4");
  TTreeReaderValue <TLorentzVector> Piplus2_MP4(*myReader, "Piplus2_Meas_P4");
  TTreeReaderValue <TLorentzVector> Piplus1_MP4(*myReader, "Piplus1_Meas_P4");
  TTreeReaderValue <TLorentzVector> Piminus1_MP4(*myReader, "Piminus1_Meas_P4");
  TTreeReaderValue <TLorentzVector> Piminus2_MP4(*myReader, "Piminus2_Meas_P4");
  TTreeReaderValue <TLorentzVector> Proton_MP4(*myReader, "Proton_Meas_P4");
  
  
  // MASSES
  TTreeReaderValue <Double_t> Piplus2Piminus1_M(*myReader,"Piplus2Piminus1_M");
  TTreeReaderValue <Double_t> Piplus2Piminus2_M(*myReader,"Piplus2Piminus2_M");    
  TTreeReaderValue <Double_t> Piplus1Piminus1_M(*myReader,"Piplus1Piminus1_M");
  TTreeReaderValue <Double_t> Piplus1Piminus2_M(*myReader,"Piplus1Piminus2_M");

  TTreeReaderValue <Double_t> Piplus1Piplus2Piminus1_M(*myReader,"Piplus1Piplus2Piminus1_M");
  TTreeReaderValue <Double_t> Piplus1Piplus2Piminus2_M(*myReader,"Piplus1Piplus2Piminus2_M");
  TTreeReaderValue <Double_t> Piplus2Piminus1Piminus2_M(*myReader,"Piplus2Piminus1Piminus2_M");
  TTreeReaderValue <Double_t> Piplus1Piminus1Piminus2_M(*myReader,"Piplus1Piminus1Piminus2_M");

  TTreeReaderValue <Double_t> Piplus2Proton_M(*myReader,"Piplus2Proton_M");
  TTreeReaderValue <Double_t> Piplus1Proton_M(*myReader,"Piplus1Proton_M");
  TTreeReaderValue <Double_t> Piminus1Proton_M(*myReader,"Piminus1Proton_M");
  TTreeReaderValue <Double_t> Piminus2Proton_M(*myReader,"Piminus2Proton_M");

  TTreeReaderValue <Double_t> ProtonPiplus2Piminus1_M(*myReader,"ProtonPiplus2Piminus1_M");
  TTreeReaderValue <Double_t> ProtonPiplus2Piminus2_M(*myReader,"ProtonPiplus2Piminus2_M");    
  TTreeReaderValue <Double_t> ProtonPiplus1Piminus1_M(*myReader,"ProtonPiplus1Piminus1_M");
  TTreeReaderValue <Double_t> ProtonPiplus1Piminus2_M(*myReader,"ProtonPiplus1Piminus2_M");

  TTreeReaderValue <Double_t> ProtonPiplus1Piplus2Piminus1_M(*myReader,"ProtonPiplus1Piplus2Piminus1_M");
  TTreeReaderValue <Double_t> ProtonPiplus1Piplus2Piminus2_M(*myReader,"ProtonPiplus1Piplus2Piminus2_M");
  TTreeReaderValue <Double_t> ProtonPiplus2Piminus1Piminus2_M(*myReader,"ProtonPiplus2Piminus1Piminus2_M");
  TTreeReaderValue <Double_t> ProtonPiplus1Piminus1Piminus2_M(*myReader,"ProtonPiplus1Piminus1Piminus2_M");

  TTreeReaderValue <Double_t> FourPions_M(*myReader,"FourPions_M");


  
  
  //MASSES SQUARED
  TTreeReaderValue <Double_t> Piplus2Piminus1_M2(*myReader,"Piplus2Piminus1_M2");
  TTreeReaderValue <Double_t> Piplus2Piminus2_M2(*myReader,"Piplus2Piminus2_M2");    
  TTreeReaderValue <Double_t> Piplus1Piminus1_M2(*myReader,"Piplus1Piminus1_M2");
  TTreeReaderValue <Double_t> Piplus1Piminus2_M2(*myReader,"Piplus1Piminus2_M2");

  TTreeReaderValue <Double_t> Piplus1Piplus2Piminus1_M2(*myReader,"Piplus1Piplus2Piminus1_M2");
  TTreeReaderValue <Double_t> Piplus1Piplus2Piminus2_M2(*myReader,"Piplus1Piplus2Piminus2_M2");
  TTreeReaderValue <Double_t> Piplus2Piminus1Piminus2_M2(*myReader,"Piplus2Piminus1Piminus2_M2");
  TTreeReaderValue <Double_t> Piplus1Piminus1Piminus2_M2(*myReader,"Piplus1Piminus1Piminus2_M2");

  TTreeReaderValue <Double_t> Piplus2Proton_M2(*myReader,"Piplus2Proton_M2");
  TTreeReaderValue <Double_t> Piplus1Proton_M2(*myReader,"Piplus1Proton_M2");
  TTreeReaderValue <Double_t> Piminus1Proton_M2(*myReader,"Piminus1Proton_M2");
  TTreeReaderValue <Double_t> Piminus2Proton_M2(*myReader,"Piminus2Proton_M2");

  TTreeReaderValue <Double_t> ProtonPiplus2Piminus1_M2(*myReader,"ProtonPiplus2Piminus1_M2");
  TTreeReaderValue <Double_t> ProtonPiplus2Piminus2_M2(*myReader,"ProtonPiplus2Piminus2_M2");    
  TTreeReaderValue <Double_t> ProtonPiplus1Piminus1_M2(*myReader,"ProtonPiplus1Piminus1_M2");
  TTreeReaderValue <Double_t> ProtonPiplus1Piminus2_M2(*myReader,"ProtonPiplus1Piminus2_M2");

  TTreeReaderValue <Double_t> ProtonPiplus1Piplus2Piminus1_M2(*myReader,"ProtonPiplus1Piplus2Piminus1_M2");
  TTreeReaderValue <Double_t> ProtonPiplus1Piplus2Piminus2_M2(*myReader,"ProtonPiplus1Piplus2Piminus2_M2");
  TTreeReaderValue <Double_t> ProtonPiplus2Piminus1Piminus2_M2(*myReader,"ProtonPiplus2Piminus1Piminus2_M2");
  TTreeReaderValue <Double_t> ProtonPiplus1Piminus1Piminus2_M2(*myReader,"ProtonPiplus1Piminus1Piminus2_M2");

  TTreeReaderValue <Double_t> FourPions_M2(*myReader,"FourPions_M2");

  
  
  //OTHER
  TTreeReaderValue <Double_t> RFTimeWeight(*myReader,"RFTimeWeight");
  TTreeReaderValue <Double_t> tvarU1(*myReader,"tvar_U1");
  TTreeReaderValue <Double_t> tvarU2(*myReader,"tvar_U2");
  TTreeReaderValue <Double_t> Miss_M2(*myReader,"Miss_M2");
  TTreeReaderValue <Float_t> E_Beam(*myReader,"E_Beam");
  TTreeReaderValue <Double_t> Chi2Ndof(*myReader,"Chi2Ndof");

  

  
  
  //*****************Declaring histograms to be filled********************

  TH1D *piplus2piminus1_mass = new TH1D("Piplus2Piminus1_M","",DEFBINS,0.,3.);
  TH1D *piplus2piminus2_mass = new TH1D("Piplus2Piminus2_M","",DEFBINS,0.,3.);
  TH1D *piplus1piminus1_mass = new TH1D("Piplus1Piminus1_M","",DEFBINS,0.,3.);
  TH1D *piplus1piminus2_mass = new TH1D("Piplus1Piminus2_M","",DEFBINS,0.,3.);

  TH1D *piplus1piplus2piminus1_mass = new TH1D("Piplus1Piplus2Piminus1_M","",DEFBINS,0.2,3.2);
  TH1D *piplus1piplus2piminus2_mass = new TH1D("Piplus1Piplus2Piminus2_M","",DEFBINS,0.2,3.2);
  TH1D *piplus2piminus1piminus2_mass = new TH1D("Piplus2Piminus1Piminus2_M","",DEFBINS,0.2,3.2);
  TH1D *piplus1piminus1piminus2_mass = new TH1D("Piplus1Piminus1Piminus2_M","",DEFBINS,0.2,3.2);
  
  TH1D *piplus2proton_mass = new TH1D("Piplus2Proton_M","",DEFBINS,0.8,3.8);
  TH1D *piplus1proton_mass = new TH1D("Piplus1Proton_M","",DEFBINS,0.8,3.8);
  TH1D *piminus1proton_mass = new TH1D("Piminus1Proton_M","",DEFBINS,0.8,3.8);
  TH1D *piminus2proton_mass = new TH1D("Piminus2Proton_M","",DEFBINS,0.8,3.8);

  TH1D *protonpiplus2piminus1_mass = new TH1D("ProtonPiplus2Piminus1_M","",DEFBINS,1.,4.);
  TH1D *protonpiplus2piminus2_mass = new TH1D("ProtonPiplus2Piminus2_M","",DEFBINS,1.,4.);
  TH1D *protonpiplus1piminus1_mass = new TH1D("ProtonPiplus1Piminus1_M","",DEFBINS,1.,4.);
  TH1D *protonpiplus1piminus2_mass = new TH1D("ProtonPiplus1Piminus2_M","",DEFBINS,1.,4.);

  TH1D *protonpiplus1piplus2piminus1_mass = new TH1D("ProtonPiplus1Piplus2Piminus1_M","",DEFBINS,1.,4.5);
  TH1D *protonpiplus1piplus2piminus2_mass = new TH1D("ProtonPiplus1Piplus2Piminus2_M","",DEFBINS,1.,4.5);
  TH1D *protonpiplus2piminus1piminus2_mass = new TH1D("ProtonPiplus2Piminus1Piminus2_M","",DEFBINS,1.5,4.5);
  TH1D *protonpiplus1piminus1piminus2_mass = new TH1D("ProtonPiplus1Piminus1Piminus2_M","",DEFBINS,2.0,4.5);

  TH1D *fourpions_mass = new TH1D("FourPions_M","",DEFBINS,0.5,3.5);

  
  TH1D *tslope = new TH1D("minustvar","",DEFBINS,0.,2.);
  TH1D *missm2 = new TH1D("Miss_M2","",DEFBINS,-0.04,0.04);
  TH1D *EnBeam  = new TH1D("E_Beam","",DEFBINS,8.0,9.0);
  TH1D *chi2ndof  = new TH1D("Chi2Ndof","",DEFBINS,0.,5.);


  TH1D *piminus1_p = new TH1D("Piminus1_P","",DEFBINS,0.,6.);
  TH1D *piplus2_p = new TH1D("Piplus2_P","",DEFBINS,0.,6.);
  TH1D *piminus2_p = new TH1D("Piminus2_P","",DEFBINS,0.,6.);
  TH1D *piplus1_p = new TH1D("Piplus1_P","",DEFBINS,0.,6.);
  TH1D *proton_p = new TH1D("Proton_P","",DEFBINS,0.,4.);
 
  TH1D *piminus1_pz = new TH1D("Piminus1_Pz","",DEFBINS,0.,6.);
  TH1D *piplus2_pz = new TH1D("Piplus2_Pz","",DEFBINS,0.,6.);
  TH1D *piminus2_pz = new TH1D("Piminus2_Pz","",DEFBINS,0.,6.);
  TH1D *piplus1_pz = new TH1D("Piplus1_Pz","",DEFBINS,0.,6.);
  TH1D *proton_pz = new TH1D("Proton_Pz","",DEFBINS,0.,4.);

  TH1D *deltaphi_lv_uv = new TH1D("DeltaPhi_LV_UV","",DEFBINS,170.,190.);
  TH1D *deltaphi_lv_uv_wrong = new TH1D("DeltaPhi_LV_UV_wrong","",DEFBINS,170.,190.);

  TH1D *deltatheta_lv_uv = new TH1D("DeltaTheta_LV_UV","",DEFBINS,170.,190.);
  TH1D *deltatheta_lv_uv_wrong = new TH1D("DeltaTheta_LV_UV_wrong","",DEFBINS,170.,190.);

  TH1D *pdiff_piplus12 = new TH1D("DiffMomPiplus12","",DEFBINS,0.,4.);
  TH1D *pdiff_piminus12 = new TH1D("DiffMomPiminus12","",DEFBINS,0.,4.);

  
  
  //2-DIM HISTOGRAMS
  
  TH2D *pippim_vs_ppip_masses = new TH2D("Dalitz_pippim_vs_ppip","",5*DEFBINS,0.,6.,5*DEFBINS,0.,12.);
  TH2D *pippimpim_vs_ppip_masses = new TH2D("Dalitz_pippimpim_vs_ppip","",5*DEFBINS,0.,9.,5*DEFBINS,0.,12.);
  TH2D *pippim_vs_ppip_masses_wrong = new TH2D("Dalitz_pippim_vs_ppip_wrong","",5*DEFBINS,0.,6.,5*DEFBINS,0.,12.);
  TH2D *pippimpim_vs_ppip_masses_wrong = new TH2D("Dalitz_pippimpim_vs_ppip_wrong","",5*DEFBINS,0.,9.,5*DEFBINS,0.,12.);

  
  TH2D *pip1_mom_vs_pip2_mom = new TH2D("Piplus1_P_vs_Piplus2_P","",5*DEFBINS,0.,7.,5*DEFBINS,0.,7.);
  TH2D *pim1_mom_vs_pim2_mom = new TH2D("Piminus1_P_vs_Piminus2_P","",5*DEFBINS,0.,7.,5*DEFBINS,0.,7.);


  
  //masses against difference in modulus of momentum: piminus1 (fast) - piminus2 (slow)
  TH2D *ppip1pimpim_mass_vs_diffpip12_mom = new TH2D("DiffPiplus12_P_vs_ProtonPiplus1Piminus1Piminus2_mass","",5*DEFBINS,2.5,4.5,5*DEFBINS,0.,7.);
  TH2D *ppip2pimpim_mass_vs_diffpip12_mom = new TH2D("DiffPiplus12_P_vs_ProtonPiplus2Piminus1Piminus2_mass","",5*DEFBINS,1.5,4.5,5*DEFBINS,0.,7.);
  TH2D *ppip2pip1pim1_mass_vs_diffpip12_mom = new TH2D("DiffPiplus12_P_vs_ProtonPiplus1Piplus2Piminus1","",5*DEFBINS,1.5,4.5,5*DEFBINS,0.,7.);
  TH2D *pip2pip1pim1_mass_vs_diffpip12_mom = new TH2D("DiffPiplus12_P_vs_Piplus1Piplus2Piminus1_mass","",5*DEFBINS,0,3.5,5*DEFBINS,0.,7.);
  TH2D *ppip2pim1_mass_vs_diffpip12_mom = new TH2D("DiffPiplus12_P_vs_ProtonPiplus2Piminus1_mass","",5*DEFBINS,1.,4.,5*DEFBINS,0.,7.);
  TH2D *ppip1pim1_mass_vs_diffpip12_mom = new TH2D("DiffPiplus12_P_vs_ProtonPiplus1Piminus1_mass","",5*DEFBINS,1.,4.,5*DEFBINS,0.,7.);
  TH2D *ppim1_mass_vs_diffpip12_mom = new TH2D("DiffPiplus12_P_vs_ProtonPiminus1_mass","",5*DEFBINS,0.5,4.,5*DEFBINS,0.,7.);


  //masses against difference in modulus of momentum: piminus1 (fast) - piminus2 (slow)
  TH2D *ppip1pimpim_mass_vs_diffpim12_mom = new TH2D("DiffPiminus12_P_vs_ProtonPiplus1Piminus1Piminus2_mass","",5*DEFBINS,2.5,4.5,5*DEFBINS,0.,7.);
  TH2D *ppip2pimpim_mass_vs_diffpim12_mom = new TH2D("DiffPiminus12_P_vs_ProtonPiplus2Piminus1Piminus2_mass","",5*DEFBINS,1.5,4.5,5*DEFBINS,0.,7.);
  TH2D *ppip2pip1pim1_mass_vs_diffpim12_mom = new TH2D("DiffPiminus12_P_vs_ProtonPiplus1Piplus2Piminus1_mass","",5*DEFBINS,1.5,4.5,5*DEFBINS,0.,7.);
  TH2D *pip2pip1pim1_mass_vs_diffpim12_mom = new TH2D("DiffPiminus12_P_vs_Piplus1Piplus2Piminus1_mass","",5*DEFBINS,0,3.5,5*DEFBINS,0.,7.);
  TH2D *ppip2pim1_mass_vs_diffpim12_mom = new TH2D("DiffPiminus12_P_vs_ProtonPiplus2Piminus1_mass","",5*DEFBINS,1.,4.,5*DEFBINS,0.,7.);
  TH2D *ppip1pim1_mass_vs_diffpim12_mom = new TH2D("DiffPiminus12_P_vs_ProtonPiplus1Piminus1_mass","",5*DEFBINS,1.,4.,5*DEFBINS,0.,7.);
  TH2D *ppim1_mass_vs_diffpim12_mom = new TH2D("DiffPiminus12_P_vs_ProtonPiminus1_mass","",5*DEFBINS,0.5,4.,5*DEFBINS,0.,7.);


   //masses against momentum transfer
  TH2D *ppip1pimpim_mass_vs_minust = new TH2D("minust_vs_ProtonPiplus1Piminus1Piminus2_mass","",5*DEFBINS,2.5,4.5,5*DEFBINS,0.,2.);
  TH2D *ppip2pimpim_mass_vs_minust = new TH2D("minust_vs_ProtonPiplus2Piminus1Piminus2_mass","",5*DEFBINS,1.5,4.5,5*DEFBINS,0.,2.);
  TH2D *ppip2pip1pim1_mass_vs_minust = new TH2D("minust_vs_ProtonPiplus1Piplus2Piminus1_mass","",5*DEFBINS,1.5,4.5,5*DEFBINS,0.,2.);
  TH2D *pip2pip1pim1_mass_vs_minust = new TH2D("minust_vs_Piplus1Piplus2Piminus1_mass","",5*DEFBINS,0,3.5,5*DEFBINS,0.,2.);
  TH2D *ppip2pim1_mass_vs_minust = new TH2D("minust_vs_ProtonPiplus2Piminus1_mass","",5*DEFBINS,1.,4.,5*DEFBINS,0.,2.);
  TH2D *ppip1pim1_mass_vs_minust = new TH2D("minust_vs_ProtonPiplus1Piminus1_mass","",5*DEFBINS,1.,4.,5*DEFBINS,0.,2.);
  TH2D *ppim1_mass_vs_minust = new TH2D("minust_vs_ProtonPiminus1_mass","",5*DEFBINS,0.5,4.,5*DEFBINS,0.,2.);
 
  
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

    diff_piplus12 = Piplus1_P4->Vect().Mag()-Piplus2_P4->Vect().Mag();
    diff_piminus12 = Piplus1_P4->Vect().Mag()-Piplus2_P4->Vect().Mag();


    
    //FILLING DESIRED HISTOGRAMS      
    wt = *RFTimeWeight; 
    piplus2piminus1_mass->Fill(*Piplus2Piminus1_M,wt);
    piplus2piminus2_mass->Fill(*Piplus2Piminus2_M,wt);
    piplus1piminus1_mass->Fill(*Piplus1Piminus1_M,wt);
    piplus1piminus2_mass->Fill(*Piplus1Piminus2_M,wt);

    piplus1piplus2piminus1_mass->Fill(*Piplus1Piplus2Piminus1_M,wt);
    piplus1piplus2piminus2_mass->Fill(*Piplus1Piplus2Piminus2_M,wt);
    piplus2piminus1piminus2_mass->Fill(*Piplus2Piminus1Piminus2_M,wt);
    piplus1piminus1piminus2_mass->Fill(*Piplus1Piminus1Piminus2_M,wt);
  
    piplus2proton_mass->Fill(*Piplus2Proton_M,wt);
    piplus1proton_mass->Fill(*Piplus1Proton_M,wt);
    piminus1proton_mass->Fill(*Piminus1Proton_M,wt);
    piminus2proton_mass->Fill(*Piminus2Proton_M,wt);


    protonpiplus2piminus1_mass->Fill(*ProtonPiplus2Piminus1_M,wt);
    protonpiplus2piminus2_mass->Fill(*ProtonPiplus2Piminus2_M,wt);
    protonpiplus1piminus1_mass->Fill(*ProtonPiplus1Piminus1_M,wt);
    protonpiplus1piminus2_mass->Fill(*ProtonPiplus1Piminus2_M,wt);
                                      
    protonpiplus1piplus2piminus1_mass->Fill(*ProtonPiplus1Piplus2Piminus1_M,wt);
    protonpiplus1piplus2piminus2_mass->Fill(*ProtonPiplus1Piplus2Piminus2_M,wt);
    protonpiplus2piminus1piminus2_mass->Fill(*ProtonPiplus2Piminus1Piminus2_M,wt);
    protonpiplus1piminus1piminus2_mass->Fill(*ProtonPiplus1Piminus1Piminus2_M,wt);

    fourpions_mass->Fill(*FourPions_M,wt);
    

    tslope->Fill(-*tvarU1,wt);
    missm2->Fill(*Miss_M2,wt);
    EnBeam->Fill(*E_Beam,wt);
    chi2ndof->Fill(*Chi2Ndof,1.);


    piminus1_p->Fill(Piminus1_P4->Vect().Mag(),wt);
    piplus2_p->Fill(Piplus2_P4->Vect().Mag(),wt);
    piminus2_p->Fill(Piminus2_P4->Vect().Mag(),wt);
    piplus1_p->Fill(Piplus1_P4->Vect().Mag(),wt);
    proton_p->Fill(Proton_P4->Vect().Mag(),wt);

    piminus1_pz->Fill(Piminus1_P4->Pz(),wt);
    piplus2_pz->Fill(Piplus2_P4->Pz(),wt);
    piminus2_pz->Fill(Piminus2_P4->Pz(),wt);
    piplus1_pz->Fill(Piplus1_P4->Pz(),wt);
    proton_pz->Fill(Proton_P4->Pz(),wt);

    pdiff_piplus12->Fill(diff_piplus12,wt);
    pdiff_piminus12->Fill(diff_piminus12,wt);


    
    //Continue with angles
        
    deltaphi=DeltaPhiLab(*Piplus2_MP4,*Piplus1_MP4,*Piminus1_MP4,*Piminus2_MP4,*Proton_MP4);
    deltaphi_lv_uv->Fill(deltaphi,wt);
    deltaphi=DeltaPhiLab(*Piplus1_MP4,*Piplus2_MP4,*Piminus1_MP4,*Piminus2_MP4,*Proton_MP4); 
    deltaphi_lv_uv_wrong->Fill(deltaphi,wt);

   
    bz = -Beam_Meas_P4->E()/(Beam_Meas_P4->E()+ProtonMass);

    deltatheta=DeltaThetaCOM(*Piplus2_MP4,*Piplus1_MP4,*Piminus1_MP4,*Piminus2_MP4,*Proton_MP4,bz);
    deltatheta_lv_uv->Fill(deltatheta,wt); 
    deltatheta=DeltaThetaCOM(*Piplus1_MP4,*Piplus2_MP4,*Piminus1_MP4,*Piminus2_MP4,*Proton_MP4,bz);  
    deltatheta_lv_uv_wrong->Fill(deltatheta,wt); 


    //Continue with 2dim histograms

    if (wt==1.){
      
	
      pippim_vs_ppip_masses->Fill(*Piplus1Piminus1_M2,*Piplus2Proton_M2,1);
      pippim_vs_ppip_masses->Fill(*Piplus1Piminus2_M2,*Piplus2Proton_M2,1);
      pippimpim_vs_ppip_masses->Fill(*Piplus1Piminus1Piminus2_M2,*Piplus2Proton_M2,1);
      
      pippim_vs_ppip_masses_wrong->Fill(*Piplus2Piminus1_M2,*Piplus1Proton_M2,1);
      pippim_vs_ppip_masses_wrong->Fill(*Piplus2Piminus2_M2,*Piplus1Proton_M2,1);
      pippimpim_vs_ppip_masses_wrong->Fill(*Piplus2Piminus1Piminus2_M2,*Piplus1Proton_M2,1);


      pip1_mom_vs_pip2_mom->Fill(Piplus2_P4->Vect().Mag(),Piplus1_P4->Vect().Mag(),1);
      pim1_mom_vs_pim2_mom->Fill(Piminus2_P4->Vect().Mag(),Piminus1_P4->Vect().Mag(),1);

      
      
      ppip1pimpim_mass_vs_diffpip12_mom->Fill(*ProtonPiplus1Piminus1Piminus2_M,diff_piplus12,1);
      ppip2pimpim_mass_vs_diffpip12_mom->Fill(*ProtonPiplus2Piminus1Piminus2_M,diff_piplus12,1);
      ppip2pip1pim1_mass_vs_diffpip12_mom->Fill(*ProtonPiplus1Piplus2Piminus1_M,diff_piplus12,1);
      pip2pip1pim1_mass_vs_diffpip12_mom->Fill(*Piplus1Piplus2Piminus1_M,diff_piplus12,1);
      ppip2pim1_mass_vs_diffpip12_mom->Fill(*ProtonPiplus2Piminus1_M,diff_piplus12,1);
      ppip1pim1_mass_vs_diffpip12_mom->Fill(*ProtonPiplus1Piminus1_M,diff_piplus12,1);
      ppim1_mass_vs_diffpip12_mom->Fill(*Piminus1Proton_M,diff_piplus12,1);
	

      ppip1pimpim_mass_vs_diffpim12_mom->Fill(*ProtonPiplus1Piminus1Piminus2_M,diff_piminus12,1);
      ppip2pimpim_mass_vs_diffpim12_mom->Fill(*ProtonPiplus2Piminus1Piminus2_M,diff_piminus12,1);
      ppip2pip1pim1_mass_vs_diffpim12_mom->Fill(*ProtonPiplus1Piplus2Piminus1_M,diff_piminus12,1);
      pip2pip1pim1_mass_vs_diffpim12_mom->Fill(*Piplus1Piplus2Piminus1_M,diff_piminus12,1);
      ppip2pim1_mass_vs_diffpim12_mom->Fill(*ProtonPiplus2Piminus1_M,diff_piminus12,1);
      ppip1pim1_mass_vs_diffpim12_mom->Fill(*ProtonPiplus1Piminus1_M,diff_piminus12,1);
      ppim1_mass_vs_diffpim12_mom->Fill(*Piminus1Proton_M,diff_piminus12,1);

      
      ppip1pimpim_mass_vs_minust->Fill(*ProtonPiplus1Piminus1Piminus2_M,-*tvarU1,1);
      ppip2pimpim_mass_vs_minust->Fill(*ProtonPiplus2Piminus1Piminus2_M,-*tvarU1,1);
      ppip2pip1pim1_mass_vs_minust->Fill(*ProtonPiplus1Piplus2Piminus1_M,-*tvarU1,1);
      pip2pip1pim1_mass_vs_minust->Fill(*Piplus1Piplus2Piminus1_M,-*tvarU1,1);
      ppip2pim1_mass_vs_minust->Fill(*ProtonPiplus2Piminus1_M,-*tvarU1,1);
      ppip1pim1_mass_vs_minust->Fill(*ProtonPiplus1Piminus1_M,-*tvarU1,1);
      ppim1_mass_vs_minust->Fill(*Piminus1Proton_M,-*tvarU1,1);


      UV_P4 = *Piplus1_P4+*Piminus1_P4+*Piminus2_P4;      
      vanHoveXY_ppipLX->Fill(vanHoveX(*Proton_P4,*Piplus2_P4,UV_P4),vanHoveY(*Proton_P4,*Piplus2_P4,UV_P4),1);


    }    
    
    
    }


  
    piplus2piminus1_mass->Write();
    piplus2piminus2_mass->Write();
    piplus1piminus1_mass->Write();
    piplus1piminus2_mass->Write();

    piplus1piplus2piminus1_mass->Write();
    piplus1piplus2piminus2_mass->Write();
    piplus2piminus1piminus2_mass->Write();
    piplus1piminus1piminus2_mass->Write();
  
    piplus2proton_mass->Write();
    piplus1proton_mass->Write();
    piminus1proton_mass->Write();
    piminus2proton_mass->Write();

    protonpiplus2piminus1_mass->Write();
    protonpiplus2piminus2_mass->Write();
    protonpiplus1piminus1_mass->Write();
    protonpiplus1piminus2_mass->Write();

    protonpiplus1piplus2piminus1_mass->Write();
    protonpiplus1piplus2piminus2_mass->Write();
    protonpiplus2piminus1piminus2_mass->Write();
    protonpiplus1piminus1piminus2_mass->Write();    

    fourpions_mass->Write();
    
    tslope->Write();
    missm2->Write();
    EnBeam->Write();
    chi2ndof->Write();

    piminus1_p->Write();
    piplus2_p->Write();
    piminus2_p->Write();
    piplus1_p->Write();
    proton_p->Write();

    piminus1_pz->Write();
    piplus2_pz->Write();
    piminus2_pz->Write();
    piplus1_pz->Write();
    proton_pz->Write();

    pdiff_piplus12->Write();
    pdiff_piminus12->Write();

    
    
    deltaphi_lv_uv->Write();
    deltaphi_lv_uv_wrong->Write();
    deltatheta_lv_uv->Write();
    deltatheta_lv_uv_wrong->Write();
    
        
    pippim_vs_ppip_masses->Write(); 
    pippimpim_vs_ppip_masses->Write();
    pippim_vs_ppip_masses_wrong->Write();
    pippimpim_vs_ppip_masses_wrong->Write();


    pip1_mom_vs_pip2_mom->Write();
    pim1_mom_vs_pim2_mom->Write();

      
      
    ppip1pimpim_mass_vs_diffpip12_mom->Write();
    ppip2pimpim_mass_vs_diffpip12_mom->Write();
    ppip2pip1pim1_mass_vs_diffpip12_mom->Write();
    pip2pip1pim1_mass_vs_diffpip12_mom->Write();
    ppip2pim1_mass_vs_diffpip12_mom->Write();
    ppip1pim1_mass_vs_diffpip12_mom->Write();
    ppim1_mass_vs_diffpip12_mom->Write();
	

    ppip1pimpim_mass_vs_diffpim12_mom->Write();
    ppip2pimpim_mass_vs_diffpim12_mom->Write();
    ppip2pip1pim1_mass_vs_diffpim12_mom->Write();
    pip2pip1pim1_mass_vs_diffpim12_mom->Write();
    ppip2pim1_mass_vs_diffpim12_mom->Write();
    ppip1pim1_mass_vs_diffpim12_mom->Write();
    ppim1_mass_vs_diffpim12_mom->Write();


   ppip1pimpim_mass_vs_minust->Write();
   ppip2pimpim_mass_vs_minust->Write();
   ppip2pip1pim1_mass_vs_minust->Write();
   pip2pip1pim1_mass_vs_minust->Write();
   ppip2pim1_mass_vs_minust->Write();
   ppip1pim1_mass_vs_minust->Write();
   ppim1_mass_vs_minust->Write();


   vanHoveXY_ppipLX->Write();
    
}




void write_histos_12(const char *input0, const char *output)
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
