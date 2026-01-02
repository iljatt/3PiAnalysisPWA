#include <cstring>
#include <iostream>



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



void replaceNamesOfBranches(TTree *dectree)
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


      replace(buffer_title, "Piplus1Piplus2", "PiplusLPiplusU");
      replace(buffer_title, "Piplus2", "PiplusL");
      replace(buffer_title, "Piplus1", "PiplusU");
      replace(buffer_title, "_U1", "");
      replace(buffer_title, "_U2", "_wrong");

      replace(buffer_name, "Piplus1Piplus2", "PiplusLPiplusU");
      replace(buffer_name, "Piplus2", "PiplusL");
      replace(buffer_name, "Piplus1", "PiplusU");
      replace(buffer_name, "_U1", "");
      replace(buffer_name, "_U2", "_wrong");

      
      printf("%-40s\n", buffer_title.c_str());
    
      branch->SetName(buffer_name.c_str());
      branch->SetTitle(buffer_title.c_str());
  }
  
}



void copy4vectors(TTree *intree, TTree *outtree)
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








void skimTreeDeltaCut(const char *infilename, const char *outfilename)
{

  
  const char *treename = "kin"; //remains intact
  const char *deltacut  = "Piplus2Proton_M >= 1.15 && Piplus2Proton_M <= 1.35 && tvar_U1 >= -2.";


  
  TFile *fileIn = TFile::Open(infilename,"READ");
  if (!fileIn) return;
  TTree *tree = (TTree*)fileIn->Get(treename);
  TFile *fileOut = TFile::Open(outfilename,"RECREATE");


  //*******************Create two reduced copies of the tree : one with and one without the Piplus TLorentzVectors************************* 
  tree->SetBranchStatus("Piplus*P4",0);
  TTree *copiedtree_a = tree->CopyTree(deltacut);

  
  tree->SetBranchStatus("*",0);
  tree->SetBranchStatus("Piplus2Proton_M",1);
  tree->SetBranchStatus("tvar_U1",1);
  tree->SetBranchStatus("Piplus*P4",1);
  TTree *copiedtree_b = tree->CopyTree(deltacut);
  
  //********************Rename simple flat branches in the copiedtree_a and clone it*******************
  replaceNamesOfBranches(copiedtree_a);     
  copiedtree_a->SetBranchStatus("*_wrong",0);  
  TTree *newtree = (TTree*) copiedtree_a->TTree::CloneTree(-1);


  //*****************Copy the Piplus four-vectors from the copiedtree_b through the explicit loop over entries*****
 
  TTree *bufftree = (TTree*)copiedtree_b->TTree::CloneTree(-1);
  copy4vectors(bufftree,newtree);   


  
  
  printListOfBranches(newtree);
  newtree->Write();
  fileOut->Close();
  fileIn->Close();


}





