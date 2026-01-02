
{
 
  const char *folder = "";
  const char *input1 = "/cache/halld/RunPeriod-2017-01/analysis/ver20/tree_pippippimpim__B4/merged/tree_pippippimpim__B4_031050.root";
  const char *input2 = "/cache/halld/RunPeriod-2017-01/analysis/ver20/tree_pippippimpim__B4/merged/tree_pippippimpim__B4_030981.root";
  const char *input3 = "/cache/halld/RunPeriod-2017-01/analysis/ver20/tree_pippippimpim__B4/merged/tree_pippippimpim__B4_030982.root";
  const char *input4 = "/cache/halld/RunPeriod-2017-01/analysis/ver20/tree_pippippimpim__B4/merged/tree_pippippimpim__B4_030980.root";
  const char *input5 = "/cache/halld/RunPeriod-2017-01/analysis/ver20/tree_pippippimpim__B4/merged/tree_pippippimpim__B4_031049.root";


  TFile *file1 = TFile::Open(input1,"READ");
  TFile *file2 = TFile::Open(input2,"READ");
  TFile *file3 = TFile::Open(input3,"READ");
  TFile *file4 = TFile::Open(input4,"READ");
  TFile *file5 = TFile::Open(input5,"READ");
  
  if ((!file1) || (!file2) || (!file3) || (!file4) || (!file5)) return;

  

  TChain *intree = new TChain("pippippimpim__B4_Tree");

  intree->Add(input1);
  intree->Add(input2);
  intree->Add(input3);
  intree->Add(input4);
  intree->Add(input5);
  
  std::cout << "The \'intree\' TChain is loaded" << endl;

  gROOT->ProcessLine(".x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C");

  std::cout << "The DSelectors library is loaded" << endl;

}
