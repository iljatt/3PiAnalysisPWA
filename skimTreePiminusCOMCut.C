#include "HistosDataMC/calc_angles.h"
#define PI TMath::Pi()
const Float_t ProtonMass = 0.938272;



void skimTreePiminusCOMCut(const char *infilename, const char *outfilename)
{

  const char *treename = "kin"; //remains intact
  
  TFile *fileIn = TFile::Open(infilename,"READ");
  if (!fileIn) return;
  TTree *intree = (TTree*)fileIn->Get(treename);
  TFile *fileOut = TFile::Open(outfilename,"RECREATE");

  //Local variables
  TLorentzVector LV_P4, UV_P4;
  Double_t bz,long_comp;
  ULong64_t nevents, ientry;


  
  
  TTree *outtree = (TTree*) intree->TTree::CloneTree(0);
  //  dectr->CopyEntries(dctrChain);


  
 //******************Usage of TTreeReader*************************                                                                                                                 
  TTreeReader *myReader = new TTreeReader(intree);
  nevents = myReader->TTreeReader::GetEntries(true);


  //MOMENTA
  TTreeReaderValue <TLorentzVector> Beam_Meas_P4(*myReader, "Beam_Meas_P4");
  TTreeReaderValue <TLorentzVector> PiplusL_P4(*myReader, "PiplusL_P4");
  TTreeReaderValue <TLorentzVector> PiplusU_P4(*myReader, "PiplusU_P4");
  TTreeReaderValue <TLorentzVector> Piminus1_P4(*myReader, "Piminus1_P4");
  TTreeReaderValue <TLorentzVector> Piminus2_P4(*myReader, "Piminus2_P4");
  TTreeReaderValue <TLorentzVector> Proton_P4(*myReader, "Proton_P4");
  TLorentzVector P4_com;



   cout << "LOOP OVER " << nevents/1000000. << "M EVENTS STARTED\n";

  
    while (myReader->Next()){
    ientry = myReader->GetCurrentEntry();

    bz = -Beam_Meas_P4->E()/(Beam_Meas_P4->E()+ProtonMass);

    P4_com = *Piminus2_P4;        
    P4_com.Boost(0,0,bz);
    
    long_comp = P4_com.Pz();

    

      if (long_comp >= 0.1){
	intree->GetEntry(ientry);
	outtree->Fill();
      }
      else continue;

      
    }
  
    
   fileIn->Close();   
   outtree->Write();
   fileOut->Close();
}


