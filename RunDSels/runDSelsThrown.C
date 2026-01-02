void runDSelsThrown()
{
 
  const char *folder = "";
  const char *input1 = "/work/halld/home/ilbelov/MCphspSpring2017/SourceTrees/tree_thrown_gen_amp_V2_tslope1__sum_030274_031057.root";
  const char *input2 = "/work/halld/home/ilbelov/MCphspSpring2017/SourceTrees/tree_thrown_gen_amp_V2_tslope1p5__sum_030274_031057.root";
  const char *input3 = "/work/halld/home/ilbelov/MCphspSpring2017/SourceTrees/tree_thrown_gen_amp_V2_tslope2__sum_030274_031057.root";
  const char *input4 = "/work/halld/home/ilbelov/MCphspSpring2017/SourceTrees/tree_thrown_gen_amp_V2_tslope3__sum_030274_031057.root";
  const char *input5 = "/work/halld/home/ilbelov/MCphspSpring2017/SourceTrees/tree_thrown_gen_amp_V2_tslope4__sum_030274_031057.root";
  const char *input6 = "/work/halld/home/ilbelov/MCphspSpring2017/SourceTrees/tree_thrown_gen_amp_V2_tslope6__sum_030274_031057.root";
  const char *input7 = "/work/halld/home/ilbelov/MCphspSpring2017/SourceTrees/tree_thrown_gen_amp_V2_tslope8__sum_030274_031057.root";
  const char *input8 = "/work/halld/home/ilbelov/MCphspSpring2017/SourceTrees/tree_thrown_gen_amp_V2_tslope10__sum_030274_031057.root";
  const char *input9 = "/work/halld/home/ilbelov/MCphspSpring2017/SourceTrees/tree_thrown_gen_amp_V2_tslope12__sum_030274_031057.root";
  const char *input10 = "/work/halld/home/ilbelov/MCphspSpring2017/SourceTrees/tree_thrown_gen_amp_V2_tslope14__sum_030274_031057.root";


  
  //    TFile *file0 = TFile::Open(input0,"READ");
  //    TTree *intree = (TTree*)file0->Get("Thrown_Tree"); //pippippimpim__B4_Tree

  

  TFile *file1 = TFile::Open(input1,"READ");
  TFile *file2 = TFile::Open(input2,"READ");
  TFile *file3 = TFile::Open(input3,"READ");
  TFile *file4 = TFile::Open(input4,"READ");
  TFile *file5 = TFile::Open(input5,"READ");
  TFile *file6 = TFile::Open(input6,"READ");
  TFile *file7 = TFile::Open(input7,"READ");
  TFile *file8 = TFile::Open(input8,"READ");
  TFile *file9 = TFile::Open(input9,"READ");
  TFile *file10 = TFile::Open(input10,"READ");
 

  
  if ((!file1) || (!file2) || (!file3)) return;

  

  TChain *intree = new TChain("Thrown_Tree");

  intree->Add(input1);
  intree->Add(input2);
  intree->Add(input3);
  intree->Add(input4);
  intree->Add(input5);  
  intree->Add(input6);
  intree->Add(input7);
  intree->Add(input8);
  intree->Add(input9);
  intree->Add(input10);  



  
  std::cout << "The \'intree\' TChain is loaded" << endl;

  gROOT->ProcessLine(".x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C");
  std::cout << "The DSelectors library is loaded" << endl;

  std::cout << "Now, running the \'DSelThrown_2pip2pim\' script " << endl;  
  intree->Process("DSelThrown_2pip2pim.C+");

  
  file1->TFile::Close();
  file2->TFile::Close();
  file3->TFile::Close();
  file4->TFile::Close();
  file5->TFile::Close();
  file6->TFile::Close();
  file7->TFile::Close();
  file8->TFile::Close();
  file9->TFile::Close();
  file10->TFile::Close();



  
}
