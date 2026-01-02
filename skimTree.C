#include "cutter.h"




void skimTree(){
  

  //  gROOT->ProcessLine(".L cutter.C+");

  
  std::string datadir = "/work/halld/home/ilbelov/DataSpring2017";
  std::string data_tbinsdir = "/work/halld/home/ilbelov/DataSpring2017/tbins";
  std::string mcsigdir = "/work/halld/home/ilbelov/MCphspSpring2017";
  std::string mcsig_tslopesdir = "/work/halld/home/ilbelov/MCphspSpring2017/Different_tslopes_best_chi2";
  std::string mccombdir = "/work/halld/home/ilbelov/MCcombSpring2017";

 
  const char *ntracks = "NumUnusedTracks == 1";
  const char *precuts = "Chi2Ndof <= 5. && E_Beam >= 8.2 && E_Beam <= 8.8 && NumUnusedTracks <= 1";
  
  
  const char *allcuts = "DiffMomPiplus >= 0.4 && PiplusLProton_M >= 1.15 && PiplusLProton_M <= 1.35 && tvar >= -2. && FourPions_M >= 1.8 && Piminus2_P4.Pz() >= 0.7";
  const char *cuts = "DiffMomPiplus >= 0.4 && tvar >= -2. && FourPions_M >= 1.8 && Piminus2_P4.Pz() >= 0.7";
  const char *diffpipluscut = "DiffMomPiplus >= 0.4";
  const char *deltacut = "PiplusLProton_M >= 1.15 && PiplusLProton_M <= 1.35 && tvar >= -2.";
  const char *fourpionscut = "FourPions_M >= 1.8";
  const char *piminuscut = "Piminus2_P4.Pz() >= 0.7";
  const char *tcut = "tvar >= -2.";

  

  //     cutter("flattree_2pip2pim_sideband_chi2__sum_30274_31057_precuts.root", "kin", "_diffpipluscut_fourpionscut_piminuscut.root", datadir, datadir, cuts);
     cutter("flattree_2pip2pim_chi2__sum_30274_31057_precuts_diffpipluscut_fourpionscut_piminuscut.root", "kin","_copy.root",datadir,datadir, tcut);


  
  
  //  cutter("flattree_2pip2pim_sum_30274_31057_precuts_diffpipluscut_deltacut.root","kin","_bin5.root",datadir,data_tbinsdir,tcut);
  //  cutter("flattree_2pip2pim_sum_30274_31057_precuts_diffpipluscut_deltacut_piminuscut.root","kin","_bin1.root",datadir,data_tbinsdir,tcut);
  //    cutter("flattree_2pip2pim_sum_30274_31057_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut.root","kin","_chi2ndof4.root",datadir,datadir,cut);


  
    //  cutter("bggentree_pippippimpim__B2__sum_30274_31057.root","kin","_allcuts_nounusedtrackscut.root","/work/halld/home/ilbelov/MCbggenSpring2017","/work/halld/home/ilbelov/MCbggenSpring2017",allcuts);
 


  
  /*
  cutter("mctree_2pip2pim_bgcomb1_3slopes__sum_30274_31057_precuts_diffpipluscut.root","kin","_fourpionscut_piminuscut.root",mccombdir,mccombdir,cut);
  cutter("mctree_2pip2pim_bgcomb2_3slopes__sum_30274_31057_precuts_diffpipluscut.root","kin","_fourpionscut_piminuscut.root",mccombdir,mccombdir,cut);
  cutter("mctree_2pip2pim_bgcomb3_3slopes__sum_30274_31057_precuts_diffpipluscut.root","kin","_fourpionscut_piminuscut.root",mccombdir,mccombdir,cut);
  cutter("mctree_2pip2pim_bgcomb4_3slopes__sum_30274_31057_precuts_diffpipluscut.root","kin","_fourpionscut_piminuscut.root",mccombdir,mccombdir,cut);
  cutter("mctree_2pip2pim_bgcomb5_3slopes__sum_30274_31057_precuts_diffpipluscut.root","kin","_fourpionscut_piminuscut.root",mccombdir,mccombdir,cut);
  */

  
  //  cutter("mctree_2pip2pim_10slopes__sum_30274_31057.root","kin","_precuts.root",mcsigdir,mcsigdir,precuts);
  //    cutter("mctree_2pip2pim_10slopes__sum_30274_31057_precuts_diffpipluscut_deltacut_fourpionscut_piminuscut.root","kin","_ntracks1.root",mcsigdir,mcsigdir,ntracks);

  
  /*
  cutter("mctree_2pip2pim_tslope14__sum_30274_31057.root", "kin", "_diffpipluscut_deltacut_fourpionscut_piminuscut.root",mcsig_tslopesdir,mcsig_tslopesdir,cut);
  cutter("mctree_2pip2pim_tslope12__sum_30274_31057.root", "kin", "_diffpipluscut_deltacut_fourpionscut_piminuscut.root",mcsig_tslopesdir,mcsig_tslopesdir,cut);
  cutter("mctree_2pip2pim_tslope10__sum_30274_31057.root", "kin", "_diffpipluscut_deltacut_fourpionscut_piminuscut.root",mcsig_tslopesdir,mcsig_tslopesdir,cut);
  cutter("mctree_2pip2pim_tslope8__sum_30274_31057.root", "kin", "_diffpipluscut_deltacut_fourpionscut_piminuscut.root",mcsig_tslopesdir,mcsig_tslopesdir,cut);
  cutter("mctree_2pip2pim_tslope6__sum_30274_31057.root", "kin", "_diffpipluscut_deltacut_fourpionscut_piminuscut.root",mcsig_tslopesdir,mcsig_tslopesdir,cut);
  cutter("mctree_2pip2pim_tslope4__sum_30274_31057.root", "kin", "_diffpipluscut_deltacut_fourpionscut_piminuscut.root",mcsig_tslopesdir,mcsig_tslopesdir,cut);
  cutter("mctree_2pip2pim_tslope3__sum_30274_31057.root", "kin", "_diffpipluscut_deltacut_fourpionscut_piminuscut.root",mcsig_tslopesdir,mcsig_tslopesdir,cut);
  cutter("mctree_2pip2pim_tslope2__sum_30274_31057.root", "kin", "_diffpipluscut_deltacut_fourpionscut_piminuscut.root",mcsig_tslopesdir,mcsig_tslopesdir,cut);
  cutter("mctree_2pip2pim_tslope1p5__sum_30274_31057.root", "kin","_diffpipluscut_deltacut_fourpionscut_piminuscut.root",mcsig_tslopesdir,mcsig_tslopesdir,cut);
  cutter("mctree_2pip2pim_tslope1__sum_30274_31057.root", "kin", "_diffpipluscut_deltacut_fourpionscut_piminuscut.root",mcsig_tslopesdir,mcsig_tslopesdir,cut);
  */
   
}


