#include <string>
#include <iostream>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TDirectory.h"


 /*
int runlocally()
{
	    gROOT->ProcessLine(".x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C");
	    std::cout << "The DSelectors library is loaded" << endl;

	    std::cout << "Now, running the \'DSelData_2pip2pim\' script " << endl;  
	    intree->Process("DSelData_2pip2pim.C+");
	    
	    return 0;
}
*/




int Run_Selector(string locInputFileName, string locTreeName, string locSelectorName, unsigned int locNThreads)
{
	//tell it to compile selector (if user did not)
	if(locSelectorName[locSelectorName.size() - 1] != '+')
	  locSelectorName += '+';

	std::cout << "file name, tree name, selector name, #threads = " << locInputFileName << ", " << locTreeName << ", " << locSelectorName << ", " << locNThreads << endl;

	if(locNThreads > 1) //USE PROOF
	{
	  gEnv->SetValue("ProofLite.Sandbox", "$PWD/.proof/"); // write all intermediate files to the local directory
	  DPROOFLiteManager::Set_SandBox("./"); // that does not work, as the proof session was already started
	  
	  return (DPROOFLiteManager::Process_Tree(locInputFileName, locTreeName, locSelectorName, locNThreads) ? 0 : 999); //0 = success
	}

	//process tree directly
	TFile* locFile = new TFile(locInputFileName.c_str(), "READ");
	TTree* locTree = (TTree*)gDirectory->Get(locTreeName.c_str());
	Long64_t locStatus = locTree->Process(locSelectorName.c_str());
	std::cout << "tree status = " << locStatus << endl;
	return ((locStatus >= Long64_t(0)) ? 0 : 999); //0 = success
}
