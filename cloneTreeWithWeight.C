void printListOfBranches(TTree *decaytree)
{
  TList:TObjArray *brslist = decaytree->TTree::GetListOfBranches();
  brslist->SetOwner(kFALSE);
  //brslist->Sort();
  cout << "New branches are added" << endl;
  for(int ii = brslist->GetEntries()-1; ii > brslist->GetEntries()-2; ii--)
    cout << "BR #"<< ii << "\t\t" << brslist->At(ii)->GetTitle() << endl;
}



void cloneTreeWithWeight(const char *infile, const char *outfile, double weight)
{
  const char *treename = "kin"; //remains intact 
  std::string wname = "TSlopeWeight";
  std::string wtitle = wname+"/D";
  Double_t value = weight;

  
  TFile *fileIn  =TFile::Open(infile,"READ");
  if (!fileIn) return;
  TTree *intree = (TTree*)fileIn->Get(treename);

  //One can activate only the needed branches here
  
  TFile *fileOut = TFile::Open(outfile,"RECREATE");
  TTree *outtree = (TTree*) intree->TTree::CloneTree(0);
  //  outtree->SetName("kin");
  //  outtree->CopyEntries(dctrChain);
  outtree->Branch(wname.c_str(),&value,wtitle.c_str()); 

  
  TTreeReader *myReader = new TTreeReader(intree);
  Long64_t nevents = myReader->TTreeReader::GetEntries(true);
  Long64_t ientry;
  

  //copy event by event
  while (myReader->Next()){
    ientry = myReader->GetCurrentEntry();
    intree->GetEntry(ientry);
    outtree->Fill();
    }


  
   fileIn->Close();
  
   printListOfBranches(outtree);
   outtree->Write();
   fileOut->Close();
     
}
