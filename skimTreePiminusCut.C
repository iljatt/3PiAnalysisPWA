#include "cutter.h"




void skimTreePiminusCut(){
  

  //  gROOT->ProcessLine(".L cutter.C+");

  
  std::string datadir = "/work/halld/home/ilbelov/DataSpring2017";
  std::string mcsigdir = "/work/halld/home/ilbelov/MCphspSpring2017";


  
  //Add cuts on the additional tracks allowed in the reaction filter to precuts.  
  const char *piminuscuts = "Piminus1_P4.Pz() >= 1. && Piminus2_P4.Pz() >= 1.";
  const char *suffix = "_piminuscut.root";

  
  //     cutter("flattree_2pip2pim_sum_30274_31057_precuts_deltacut_ab.root", "kin", suffix, datadir, datadir, piminuscuts);
      cutter("mctree_2pip2pim_3slopes__sum_30274_31057_deltacut.root", "kin", suffix, mcsigdir, mcsigdir, piminuscuts);
  

}


