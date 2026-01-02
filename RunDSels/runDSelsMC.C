
void runDSelsMC()
{
 
  const char *folder = "";

  //        const char *input0 = "/work/halld/home/ilbelov/MCPHSPSpring2017/tree_pippippimpim__B4_gen_amp_V2_mask011__sum_30274_31057.root";
       const char *input0 = "/work/halld/home/ilbelov/MCphspSpring2017/SourceTrees/tree_pippippimpim__B4_gen_amp_V2_tslope1p5__sum_30274_31057.root";

  
    TFile *file0 = TFile::Open(input0,"READ");
    TTree *intree = (TTree*)file0->Get("pippippimpim__B4_Tree"); 
 
  
  /*
  TFile *file1 = TFile::Open(input1,"READ");
  TFile *file2 = TFile::Open(input2,"READ");
  TFile *file3 = TFile::Open(input3,"READ");
  
  if ((!file1) || (!file2) || (!file3)) return;
  */
  
  /*
  TChain *intree = new TChain("pippippimpim__B4_Tree");

  intree->Add(input1);
  intree->Add(input2);
  intree->Add(input3);
  intree->Add(input4);
  intree->Add(input5);  
  */


  
  std::cout << "The \'intree\' TChain is loaded" << endl;

  gROOT->ProcessLine(".x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C");
  std::cout << "The DSelectors library is loaded" << endl;

  std::cout << "Now, running the \'DSelMC_2pip2pim\' script " << endl;  
  intree->Process("DSelMC_2pip2pim.C+");

  /*
  file1->TFile::Close();
  file2->TFile::Close();
  file3->TFile::Close();
  file4->TFile::Close();
  file5->TFile::Close();
  */
}
