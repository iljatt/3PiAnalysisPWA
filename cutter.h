#include <string>
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TChain.h"
#include "TMath.h"


/* 
  Example of how to write a reduced ntuple
  Use: .L ApplySelection.C+
        ApplySelection("theFile.root")
 Giving extra arguments allows to change ntuple and output name
        ApplySelection("EtaDown.root", "rhoGammaDown/rhoGammaDown", "_Selected.root");
*/ 



/* First some helpers to avoid duplicating code */
std::string createOutputName(const std::string& name, const std::string& suffix){
  std::string outputName = name.substr(0,name.size() - 5);
  outputName += suffix;
  return outputName;
}

TFile* openOutput(std::string& dir, std::string& tree, std::string& input, std::string& output) {
  std::string path = dir +  '/'  + output;
  TFile* outFile  =new TFile(path.c_str(),"RECREATE");
  std::cout << "**************" <<std::endl;
  std::cout << "Reading: " << tree << " from " << input  << " to " << path << std::endl;
  return outFile;
}

void finalize(TTree* cutTree, TFile* output){
  TTree*  newtree = cutTree->CloneTree(-1);
  newtree->Write();
  output->Close();
}


/* Now the main business */
std::string cutter(std::string fileName, std::string treeName, std::string suffix, std::string idir, std::string odir, const char *cuts){


  // get the input
  TChain* decaytree = new TChain(treeName.c_str());
  std::string fullname = idir + "/" + fileName;
  cout << fullname << endl;
  decaytree->Add(fullname.c_str());

  //TTree *decaytree = (TTree*) fullname->Get(treeName.c_str());
  // make the output file name 
  std::string outputName = createOutputName(fileName, suffix);
 
  // make the output file
  TFile* outFile = openOutput(odir,treeName,fileName,outputName); 
  TCut cut = cuts;

  cout << "Copying..."<<endl;
  TTree* smalltree = decaytree->CopyTree(cut);

  //decaytree->Print();
  //TTree* smalltree = decaytree->CloneTree(50); 
  //cout << "Finishedx"<<endl;
  //double val = 100*smalltree->GetEntries()/float(decaytree->GetEntries());
  //double error = sqrt(val*(1-val)/float(decaytree->GetEntries()));
  //std::cout << " Selected " <<  decaytree->GetEntries() << " " << smalltree->GetEntries() <<  " " << val << " +/- " << error << std::endl;
  //std::cout << "**************" <<std::endl;

  cut.Print();

  finalize(smalltree,outFile);  

  
  return outputName;
}
