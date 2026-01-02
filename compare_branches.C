void compare_branches()
{

  // What do we get from rection filter
  //  const char *filename1 = "/cache/halld/RunPeriod-2017-01/analysis/ver20/tree_pippippimpim__B4/merged/tree_pippippimpim__B4_031050.root";
  //  const char *treename1 = "pippippimpim__B4_Tree"; 
  //  const char *filename2 = "/volatile/halld/home/ilbelov/MC_ppippimRho_pippim/root/trees/tree_pippippimpim__B4_gen_amp_V2_030300_000_decay_evtgen.root";
  //  const char *treename2 = "pippippimpim__B4_Tree"; 


  // What do we save in flat trees
    const char *filename1 = "DataSpring2017/flattree_2pip2pim_sum_30274_31057_precuts.root";
    const char *treename1 = "kin";
    const char *filename2 = "DataSpring2017/flattree_2pip2pim_sum_30274_31057_precuts_deltacut_1st.root";
    const char *treename2 = "kin";


  
  std::cout << "Compare the files:\n";
  std::cout << filename1 << "\t" << treename1 << endl;
  std::cout << filename2 << "\t" << treename2 << endl;

    
  
  TFile *file0ne = TFile::Open(filename1,"READ");
  TFile *fileTwo = TFile::Open(filename2,"READ");
  if (!file0ne || !fileTwo) return;

  TTree *dctrOne = (TTree*)file0ne->Get(treename1);
  TTree *dctrTwo = (TTree*)fileTwo->Get(treename2);


  TObjArray *brslistOne = dctrOne->TTree::GetListOfBranches();
      brslistOne->SetOwner(kFALSE);
      brslistOne->Sort();
  TObjArray *brslistTwo = dctrTwo->TTree::GetListOfBranches();
      brslistTwo->SetOwner(kFALSE);
      brslistTwo->Sort();
      
      std::string branch_name, branch_title;
      Bool_t isMember;
      UInt_t brs_max = 0;
      TObjArray brslistMax, brslistMin;    
      

      if (brslistOne->GetEntries() > brslistTwo->GetEntries()){
 	brslistMax = TObjArray(*brslistOne);
        brslistMin = TObjArray(*brslistTwo);
        brs_max = brslistOne->GetEntries();
      }
      else {
	brslistMax = TObjArray(*brslistTwo);
	brslistMin = TObjArray(*brslistOne);
        brs_max = brslistTwo->GetEntries();
      }
	

      
      
      for(int ii=0, jj=0; ii < brs_max; ii++){
	branch_name = brslistMax.At(ii)->GetName();
	branch_title = brslistMax.At(ii)->GetTitle();	
	isMember  = brslistMin.TObjArray::FindObject(branch_name.c_str());

	if (isMember==0)  { printf("BR %d\t\t%-100s\t\tNot found\n",jj, branch_title.c_str()); jj++;}
      }


      
      file0ne->Close();
      fileTwo->Close();

}
