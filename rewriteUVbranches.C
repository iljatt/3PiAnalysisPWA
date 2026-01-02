#include <cstring>


bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}



void printListOfBranches(TTree *dectree)
{
  TList:TObjArray *brslist = dectree->TTree::GetListOfBranches();
  brslist->SetOwner(kFALSE);
  brslist->Sort();
   for(int ii = 0; ii < brslist->GetEntries(); ii++)
     cout << "BR #"<< ii << "\t\t" << brslist->At(ii)->GetTitle() << endl;
}




void replaceNamesOfBranches(TTree *dectree, const char *mode)
{

  std::string buffer_name, buffer_title;
  
  TList:TObjArray *brslist = dectree->TTree::GetListOfBranches();
  brslist->SetOwner(kFALSE);
  brslist->Sort();


  for (UInt_t ii = 0; ii < brslist->GetEntries(); ii++) {

      TBranch *branch = (TBranch*) brslist->At(ii);
      buffer_title =  branch->GetTitle();
      buffer_name =  branch->GetName();
    
      printf("%-40s=>\t",buffer_title.c_str());

      if (strncmp(mode,"1st",3)==0 || strncmp(mode,"both",4)==0){
	
	replace(buffer_title, "Piplus1Piplus2", "PiplusLPiplusU");
	replace(buffer_title, "Piplus1", "PiplusL");
	replace(buffer_title, "Piplus2", "PiplusU");
	replace(buffer_title, "tvar2", "tvar");
	replace(buffer_title, "tvar1", "trash");
	replace(buffer_name, "Piplus1Piplus2", "PiplusLPiplusU");
	replace(buffer_name, "Piplus1", "PiplusL");
	replace(buffer_name, "Piplus2", "PiplusU");
	replace(buffer_name, "tvar2", "tvar");
	replace(buffer_name, "tvar1", "trash");

      }
      else if (strncmp(mode,"2nd",3)==0){

	replace(buffer_title, "Piplus1Piplus2", "PiplusLPiplusU");
	replace(buffer_title, "Piplus2", "PiplusL");
	replace(buffer_title, "Piplus1", "PiplusU");
	replace(buffer_title, "tvar1", "tvar");
	replace(buffer_title, "tvar2", "trash");
	replace(buffer_name, "Piplus1Piplus2", "PiplusLPiplusU");
	replace(buffer_name, "Piplus2", "PiplusL");
	replace(buffer_name, "Piplus1", "PiplusU");
	replace(buffer_name, "tvar1", "tvar");
	replace(buffer_name, "tvar2", "trash");
 }
      
      printf("%-40s\n", buffer_title.c_str());
    
      branch->SetName(buffer_name.c_str());
      branch->SetTitle(buffer_title.c_str());
  }
  
}



void copy4vectors(TTree *intree, TTree *outtree, const char *mode)
{
  Long64_t ii, nentries = intree->GetEntries();

  TLorentzVector *piplus1 = new TLorentzVector();
  TLorentzVector *piplus1_meas = new TLorentzVector();
  TLorentzVector *piplus2 = new TLorentzVector();
  TLorentzVector *piplus2_meas = new TLorentzVector();

  TLorentzVector *piplusL = new TLorentzVector();
  TLorentzVector *piplusU = new TLorentzVector();
  TLorentzVector *piplusL_meas = new TLorentzVector();
  TLorentzVector *piplusU_meas = new TLorentzVector();


  intree->SetBranchAddress("Piplus1_P4", &piplus1);
  intree->SetBranchAddress("Piplus2_P4", &piplus2);
  intree->SetBranchAddress("Piplus1_Meas_P4", &piplus1_meas);
  intree->SetBranchAddress("Piplus2_Meas_P4", &piplus2_meas);
  
  TBranch *b1 = outtree->Branch("PiplusU_P4",&piplusU);
  TBranch *b2 = outtree->Branch("PiplusL_P4",&piplusL);
  TBranch *b3 = outtree->Branch("PiplusU_Meas_P4",&piplusU_meas);
  TBranch *b4 = outtree->Branch("PiplusL_Meas_P4",&piplusL_meas);

      
  if (strncmp(mode,"1st",3)==0 || strncmp(mode,"both",4)==0){
    for (ii = 0; ii < nentries; ii++) {
      intree->GetEntry(ii);

      piplusL->SetPxPyPzE(piplus1->Px(),piplus1->Py(),piplus1->Pz(),piplus1->E());
      piplusU->SetPxPyPzE(piplus2->Px(),piplus2->Py(),piplus2->Pz(),piplus2->E());
      piplusL_meas->SetPxPyPzE(piplus1_meas->Px(),piplus1_meas->Py(),piplus1_meas->Pz(),piplus1_meas->E());
      piplusU_meas->SetPxPyPzE(piplus2_meas->Px(),piplus2_meas->Py(),piplus2_meas->Pz(),piplus2_meas->E());
        
      b1->Fill();
      b2->Fill();
      b3->Fill();
      b4->Fill();
    }
  }
  
  else if (strncmp(mode,"2nd",3)==0){
    for (ii = 0; ii < nentries; ii++) {
      intree->GetEntry(ii);

      piplusU->SetPxPyPzE(piplus1->Px(),piplus1->Py(),piplus1->Pz(),piplus1->E());
      piplusL->SetPxPyPzE(piplus2->Px(),piplus2->Py(),piplus2->Pz(),piplus2->E());
      piplusU_meas->SetPxPyPzE(piplus1_meas->Px(),piplus1_meas->Py(),piplus1_meas->Pz(),piplus1_meas->E());
      piplusL_meas->SetPxPyPzE(piplus2_meas->Px(),piplus2_meas->Py(),piplus2_meas->Pz(),piplus2_meas->E());
        
      b1->Fill();
      b2->Fill();
      b3->Fill();
      b4->Fill();
    }
  }  
}











void rewriteUVbranches(const char *infilename, const char *outfilename, const char *mode)
{

  const char *treename = "kin"; //remains intact
  
  TFile *fileIn = TFile::Open(infilename,"READ");
  if (!fileIn) return;
  TTree *tree = (TTree*)fileIn->Get(treename);
  TFile *fileOut = TFile::Open(outfilename,"RECREATE");


  //*******************Create a copy of the tree, rename simple flat branches and clone the tree*******************
  tree->SetBranchStatus("Piplus*P4",0);
  TTree *copiedtree = tree->CopyTree("");
  
  replaceNamesOfBranches(copiedtree,mode);     

  copiedtree->SetBranchStatus("trash*",0);  
  TTree *newtree = (TTree*) copiedtree->TTree::CloneTree(-1);


  //*****************Copy the PiplusL and PiplusU four-vectors through the explicit loop over entries*****

  tree->SetBranchStatus("*",0);
  tree->SetBranchStatus("Piplus*_P4",1);
  copy4vectors(tree,newtree,mode);   


  
  
  printListOfBranches(newtree);
  newtree->Write();
  fileOut->Close();
  fileIn->Close();


}
