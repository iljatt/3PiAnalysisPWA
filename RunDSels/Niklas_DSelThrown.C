#include "DSelector_thrown.h"

void DSelector_thrown::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "/w/halld-scshelf2101/home/herrmann/root_trees/DSelector_output/thrown.root"; //"" for none
	dFlatTreeFileName = "/w/halld-scshelf2101/home/herrmann/root_trees/DSelector_output/flat_tree_thrown.root"; //output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = "kin"; //if blank, default name will be chosen

	//USERS: SET OUTPUT TREE FILES/NAMES //e.g. binning into separate files for AmpTools
	//dOutputTreeFileNameMap["Bin1"] = "mcgen_bin1.root"; //key is user-defined, value is output file name
	//dOutputTreeFileNameMap["Bin2"] = "mcgen_bin2.root"; //key is user-defined, value is output file name
	//dOutputTreeFileNameMap["Bin3"] = "mcgen_bin3.root"; //key is user-defined, value is output file name

	//Because this function gets called for each TTree in the TChain, we must be careful:
		//We need to re-initialize the tree interface & branch wrappers, but don't want to recreate histograms
	bool locInitializedPriorFlag = dInitializedFlag; //save whether have been initialized previously
	DSelector::Init(locTree); //This must be called to initialize wrappers for each new TTree
	//gDirectory now points to the output file with name dOutputFileName (if any)
	if(locInitializedPriorFlag)
		return; //have already created histograms, etc. below: exit

	dPreviousRunNumber = 0;

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/
	dHist_Delta = new TH1F("hDelta", ";M(#pi^+ #p) [GeV];Counts", 200, 1.0, 5.0);
	dHist_rho_mass = new TH1F("hrho_mass", ";M(#pi^- #pi^0) [GeV];Counts", 200, 0.0, 3);
	dHist_Delta_unweight = new TH1F("hDelta_unweight", ";M(#pi^+ #p) [GeV];Counts", 200, 1.0, 5.0);
	dHist_rho_mass_unweight = new TH1F("hrho_mass_unweight", ";M(#pi^- #pi^0) [GeV];Counts", 200, 0.0, 3);
	dHist_t_unweight = new TH1F("ht_unweight", ";-t [GeV^{2}]; -t [GeV^{2}];Counts", 200, 0.0, 3);
	dHist_t = new TH1F("ht", ";-t [GeV^{2}]; -t [GeV^{2}];Counts", 200, 0.0, 3);
	dHist_RhoMinus_Angle = new TH2F("rho_minus_angle", "#pi^{-}#pi^{0} angle distribution;#phi (deg);#theta (deg)", 360, -180, 180, 180, 0, 180);

	/*******+++++++++++++Angles */

	dHist_PiPlus_Angle_GFJ = new TH2F("PiPlus_angle_GFJ", "#pi^{+} angle distribution;#phi_{GFJ,#pi^{+}} (deg);cos(#theta_{GFJ,#pi^{+}})", 90, -180, 180, 100, -1, 1);
	dHist_PiMinus_Angle_GFJ = new TH2F("PiMinus_angle_GFJ", "#pi^{-} angle distribution;#phi_{GFJ,#pi^{-}} (deg);cos(#theta_{GFJ,#pi^{-}})", 90, -180, 180, 100, -1, 1);

	dHist_PiPlus_Angle_HF = new TH2F("PiPlus_angle_HF", "#pi^{+} angle distribution;#phi_{HF,#pi^{+}} (deg);cos(#theta_{HF,#pi^{+}})",90, -180, 180, 100, -1, 1);
	dHist_PiMinus_Angle_HF = new TH2F("PiMinus_angle_HF", "#pi^{-} angle distribution;#phi_{HF,#pi^{-}} (deg);cos(#theta_{HF,#pi^{-}})", 90, -180, 180, 100, -1, 1);
	

	dHist_PiPlus_Angle_HF_Meson = new TH2F("PiPlus_angle_HF_Meson", "#pi^{+} angle distribution;#phi_{HF,#pi^{+}} (deg);cos(#theta_{HF,#pi^{+}})",90, -180, 180, 100, -1, 1);
	dHist_PiMinus_Angle_HF_Meson = new TH2F("PiMinus_angle_HF_Meson", "#pi^{-} angle distribution;#phi_{HF,#pi^{-}} (deg);cos(#theta_{HF,#pi^{-}})", 90, -180, 180, 100, -1, 1);
	dHist_PiMinusPi0_Angle_HF_Meson = new TH2F("PiMinusPi0_angle_HF_Meson", "#pi^{-} angle distribution;#phi_{HF,#pi^{-}#pi^{0}} (deg);cos(#theta_{HF,#pi^{-}#pi^{0}})", 90, -180, 180, 100, -1, 1);

	dHist_PiPlus_Angle_GFJ_Meson = new TH2F("PiPlus_angle_GFJ_Meson", "#pi^{+} angle distribution;#phi_{GFJ,#pi^{+}} (deg);cos(#theta_{GFJ,#pi^{+}})",90, -180, 180, 100, -1, 1);
	dHist_PiMinus_Angle_GFJ_Meson = new TH2F("PiMinus_angle_GFJ_Meson", "#pi^{-} angle distribution;#phi_{GFJ,#pi^{-}} (deg);cos(#theta_{GFJ,#pi^{-}})", 90, -180, 180, 100, -1, 1);
	dHist_PiMinusPi0_Angle_GFJ_Meson = new TH2F("PiMinusPi0_angle_GFJ_Meson", "#pi^{-} angle distribution;#phi_{GFJ,#pi^{-}#pi^{0}} (deg);cos(#theta_{GFJ,#pi^{-}#pi^{0}})", 90, -180, 180, 100, -1, 1);
	SetupAmpTools_FlatTree(); //sets most branches needed for AmpTools
	SetupAmpTools_FlatTree();

	dFlatTreeInterface->Create_Branch_Fundamental<float>("p_pip");
	
	

     

	// invariant masses of combinations
	dFlatTreeInterface->Create_Branch_Fundamental<float>("m_proton_pip");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("m_pi0_pim");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("m_pi0_pip_pim");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("m_pi0");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("z_pi0");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("z_pip");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("z_pim");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("z_proton");

	// additional information
	dFlatTreeInterface->Create_Branch_Fundamental<float>("E_beam");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("mandel_t");
	
	/************************************* ADVANCED EXAMPLE: CHOOSE BRANCHES TO READ ************************************/

	//TO SAVE PROCESSING TIME
		//If you know you don't need all of the branches/data, but just a subset of it, you can speed things up
		//By default, for each event, the data is retrieved for all branches
		//If you know you only need data for some branches, you can skip grabbing data from the branches you don't need
		//Do this by doing something similar to the commented code below

	//dTreeInterface->Clear_GetEntryBranches(); //now get none
	//dTreeInterface->Register_GetEntryBranch("Proton__P4"); //manually set the branches you want
}

Bool_t DSelector_thrown::Process(Long64_t locEntry)
{
	// The Process() function is called for each entry in the tree. The entry argument
	// specifies which entry in the currently loaded tree is to be processed.
	//
	// This function should contain the "body" of the analysis. It can contain
	// simple or elaborate selection criteria, run algorithms on the data
	// of the event and typically fill histograms.
	//
	// The processing can be stopped by calling Abort().
	// Use fStatus to set the return value of TTree::Process().
	// The return value is currently not used.

	//CALL THIS FIRST
	DSelector::Process(locEntry); //Gets the data from the tree for the entry
	//cout << "RUN " << Get_RunNumber() << ", EVENT " << Get_EventNumber() << endl;

	/******************************************** GET POLARIZATION ORIENTATION ******************************************/

	//Only if the run number changes
	//RCDB environment must be setup in order for this to work! (Will return false otherwise)
	UInt_t locRunNumber = Get_RunNumber();
	if(locRunNumber != dPreviousRunNumber)
	{
		dIsPolarizedFlag = dAnalysisUtilities.Get_IsPolarizedBeam(locRunNumber, dIsPARAFlag);
		dPreviousRunNumber = locRunNumber;
	}

	/********************************************* SETUP UNIQUENESS TRACKING ********************************************/

	//INSERT USER ANALYSIS UNIQUENESS TRACKING HERE

	/******************************************* LOOP OVER THROWN DATA ***************************************/

	//Thrown beam: just use directly
	TLorentzVector locThrownBeamP4;
	
	if(dThrownBeam != NULL){
	Bool_t dIsBeamEnergyCut =kFALSE;
	locThrownBeamP4 = dThrownBeam->Get_P4();
	if(locThrownBeamP4.E()<=8.8 && locThrownBeamP4.E()>=8.2) dIsBeamEnergyCut =kTRUE;

	if(dIsBeamEnergyCut==kTRUE){
		TLorentzVector locThrownProtonP4, locThrownPiPlusP4, locThrownPiMinusP4, locThrownPi0P4;
	//Loop over throwns
	for(UInt_t loc_i = 0; loc_i < Get_NumThrown(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dThrownWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
		Particle_t locPID = dThrownWrapper->Get_PID();
		TLorentzVector locThrownP4 = dThrownWrapper->Get_P4();
		if(locPID==14) locThrownProtonP4 = locThrownP4;
		if(locPID==8) locThrownPiPlusP4 = locThrownP4;
		if(locPID==9) locThrownPiMinusP4 = locThrownP4;
		if(locPID==7) locThrownPi0P4 = locThrownP4;
		
		cout << "Thrown " << Get_NumThrown() << loc_i << ": " << locPID << ", " << locThrownP4.Px() << ", " << locThrownP4.Py() << ", " << locThrownP4.Pz() << ", " << locThrownP4.E() << endl;
	}
    TLorentzVector locThrownPiMinusPi0P4 = locThrownPi0P4 + locThrownPiMinusP4;
	TLorentzVector locThrownPiPlusProtonP4 = locThrownProtonP4 + locThrownPiPlusP4;
	TLorentzVector locThrownOmegaP4 = locThrownPi0P4 + locThrownPiMinusP4+ locThrownPiPlusP4;

	TLorentzVector mandelstam_t_P4 = (locThrownBeamP4 - locThrownPiMinusPi0P4);
	double phi_RhoMinus_LBS = locThrownPiMinusPi0P4.Phi()*TMath::RadToDeg();
	double theta_RhoMinus_LBS = locThrownPiMinusPi0P4.Theta()*TMath::RadToDeg();
	double mandelstam_t = mandelstam_t_P4*mandelstam_t_P4;
	double momentum_transfer_t = -mandelstam_t;
	double MCweight;
	MCweight= Get_MCWeight();
	double weight = MCweight;   //Just that I dont have to change the reco code
	dHist_rho_mass->Fill(locThrownPiMinusPi0P4.M(),MCweight);
	dHist_Delta->Fill(locThrownPiPlusProtonP4.M(),MCweight);
	dHist_rho_mass_unweight->Fill(locThrownPiMinusPi0P4.M());
	dHist_Delta_unweight->Fill(locThrownPiPlusProtonP4.M());
	dHist_t->Fill(momentum_transfer_t,MCweight);
	dHist_t_unweight->Fill(momentum_transfer_t,1);
	dHist_RhoMinus_Angle->Fill(phi_RhoMinus_LBS,theta_RhoMinus_LBS,MCweight);

	//OR Manually:
	//BEWARE: Do not expect the particles to be at the same array indices from one event to the next!!!!
	//Why? Because while your channel may be the same, the pions/kaons/etc. will decay differently each event.

	//BRANCHES: https://halldweb.jlab.org/wiki/index.php/Analysis_TTreeFormat#TTree_Format:_Simulated_Data
	
	TClonesArray** locP4Array = dTreeInterface->Get_PointerToPointerTo_TClonesArray("Thrown__P4");
	TBranch* locPIDBranch = dTreeInterface->Get_Branch("Thrown__PID");
	/********************** Calculate Angles */
	TLorentzVector locPi0P4_CM(locThrownPi0P4);
	TLorentzVector locPiMinusP4_CM(locThrownPiMinusP4);
	TLorentzVector locBeamP4_CM(locThrownBeamP4);
	TLorentzVector locPiMinusPi0P4_CM(locThrownPiMinusPi0P4);
	TLorentzVector locTargetP4_CM(dTargetP4);
	TLorentzVector locPiPlusProtonP4_CM(locThrownPiPlusProtonP4);

	// CM-system moves with momentum P_CM in Lab

	TLorentzVector P_CM_P4 = locThrownBeamP4 + dTargetP4;
	TVector3 locBoostVector_CM = -1.0*(P_CM_P4.BoostVector());
	locPi0P4_CM.Boost(locBoostVector_CM);
	locPiMinusP4_CM.Boost(locBoostVector_CM);
	locPiMinusPi0P4_CM.Boost(locBoostVector_CM);
	locPiPlusProtonP4_CM.Boost(locBoostVector_CM);
	locBeamP4_CM.Boost(locBoostVector_CM);
	locTargetP4_CM.Boost(locBoostVector_CM);
	




	// bost vector to rho- frame
	
	TVector3 locBoostVector_RhoMinus = -1.0*(locThrownPiMinusPi0P4.BoostVector());

	TLorentzVector locPi0P4_RhoSystem(locThrownPi0P4);
	TLorentzVector locPiMinusP4_RhoSystem(locThrownPiMinusP4);
	TLorentzVector locBeamP4_RhoSystem(locThrownBeamP4);
	TLorentzVector locPiMinusPi0P4_RhoSystem(locThrownPiMinusPi0P4);
	TLorentzVector locTargetP4_RhoSystem(dTargetP4);
	TLorentzVector locPiPlusProtonP4_RhoSystem(locThrownPiPlusProtonP4);
	TLorentzVector locPiPlusP4_RhoSystem(locThrownPiPlusP4);


	locPi0P4_RhoSystem.Boost(locBoostVector_RhoMinus);
	locPiMinusP4_RhoSystem.Boost(locBoostVector_RhoMinus);
	locPiMinusPi0P4_RhoSystem.Boost(locBoostVector_RhoMinus);
	locPiPlusProtonP4_RhoSystem.Boost(locBoostVector_RhoMinus);
	locBeamP4_RhoSystem.Boost(locBoostVector_RhoMinus);
	locTargetP4_RhoSystem.Boost(locBoostVector_RhoMinus);
	locPiPlusP4_RhoSystem.Boost(locBoostVector_RhoMinus);

	TVector3 vz_HF_Meson = locPiMinusPi0P4_CM.Vect().Unit();
	TVector3 vy_HF_Meson = (locBeamP4_CM.Vect().Cross(locPiMinusPi0P4_CM.Vect())).Unit();
	TVector3 vx_HF_Meson = (vy_HF_Meson.Cross(vz_HF_Meson)).Unit();

	TVector3 locPiMinusPi0P3_HF_Meson(
		locPiMinusPi0P4_RhoSystem.Vect().Dot(vx_HF_Meson),
		locPiMinusPi0P4_RhoSystem.Vect().Dot(vy_HF_Meson),
		locPiMinusPi0P4_RhoSystem.Vect().Dot(vz_HF_Meson)
	);
	
	TVector3 locPiPlusProtonP3_HF_Meson(
		locPiPlusProtonP4_RhoSystem.Vect().Dot(vx_HF_Meson),
		locPiPlusProtonP4_RhoSystem.Vect().Dot(vy_HF_Meson),
		locPiPlusProtonP4_RhoSystem.Vect().Dot(vz_HF_Meson)
	);
	
	TVector3 locPiPlusP3_HF_Meson(
		locPiPlusP4_RhoSystem.Vect().Dot(vx_HF_Meson),
		locPiPlusP4_RhoSystem.Vect().Dot(vy_HF_Meson),
		locPiPlusP4_RhoSystem.Vect().Dot(vz_HF_Meson)
	);
	
	TVector3 locPiMinusP3_HF_Meson(
		locPiMinusP4_RhoSystem.Vect().Dot(vx_HF_Meson),
		locPiMinusP4_RhoSystem.Vect().Dot(vy_HF_Meson),
		locPiMinusP4_RhoSystem.Vect().Dot(vz_HF_Meson)
	);

	double PiMinusPi0_Phi_HF_Meson = (locPiMinusPi0P3_HF_Meson.Phi())*TMath::RadToDeg();
	double PiMinusPi0_Theta_HF_Meson = locPiMinusPi0P3_HF_Meson.CosTheta();

	double PiMinus_Phi_HF_Meson = (locPiMinusP3_HF_Meson.Phi())*TMath::RadToDeg();
	double PiMinus_Theta_HF_Meson = locPiMinusP3_HF_Meson.CosTheta();

	double PiPlus_Phi_HF_Meson = (locPiPlusP3_HF_Meson.Phi())*TMath::RadToDeg();
	double PiPlus_Theta_HF_Meson = locPiPlusP3_HF_Meson.CosTheta();

	// Define system GFJ to describe Pi Minus (described here: https://halldweb.jlab.org/DocDB/0048/004829/005/coordinates.pdf)
	TVector3 vz_GFJ_Meson = locBeamP4_RhoSystem.Vect().Unit();
	TVector3 vy_GFJ_Meson = (locBeamP4_CM.Vect().Cross(locPiMinusPi0P4_CM.Vect())).Unit();
	TVector3 vx_GFJ_Meson = (vy_GFJ_Meson.Cross(vz_GFJ_Meson)).Unit();

	TVector3 locPiMinusPi0P3_GFJ_Meson(
		locPiMinusPi0P4_RhoSystem.Vect().Dot(vx_GFJ_Meson),
		locPiMinusPi0P4_RhoSystem.Vect().Dot(vy_GFJ_Meson),
		locPiMinusPi0P4_RhoSystem.Vect().Dot(vz_GFJ_Meson)
	);
	
	TVector3 locPiPlusProtonP3_GFJ_Meson(
		locPiPlusProtonP4_RhoSystem.Vect().Dot(vx_GFJ_Meson),
		locPiPlusProtonP4_RhoSystem.Vect().Dot(vy_GFJ_Meson),
		locPiPlusProtonP4_RhoSystem.Vect().Dot(vz_HF_Meson)
	);
	
	TVector3 locPiPlusP3_GFJ_Meson(
		locPiPlusP4_RhoSystem.Vect().Dot(vx_GFJ_Meson),
		locPiPlusP4_RhoSystem.Vect().Dot(vy_GFJ_Meson),
		locPiPlusP4_RhoSystem.Vect().Dot(vz_GFJ_Meson)
	);
	
	TVector3 locPiMinusP3_GFJ_Meson(
		locPiMinusP4_RhoSystem.Vect().Dot(vx_GFJ_Meson),
		locPiMinusP4_RhoSystem.Vect().Dot(vy_GFJ_Meson),
		locPiMinusP4_RhoSystem.Vect().Dot(vz_GFJ_Meson)
	);

	double PiMinusPi0_Phi_GFJ_Meson = (locPiMinusPi0P3_GFJ_Meson.Phi())*TMath::RadToDeg();
	double PiMinusPi0_Theta_GFJ_Meson = locPiMinusPi0P3_GFJ_Meson.CosTheta();

	double PiMinus_Phi_GFJ_Meson = (locPiMinusP3_GFJ_Meson.Phi())*TMath::RadToDeg();
	double PiMinus_Theta_GFJ_Meson = locPiMinusP3_GFJ_Meson.CosTheta();

	double PiPlus_Phi_GFJ_Meson = (locPiPlusP3_GFJ_Meson.Phi())*TMath::RadToDeg();
	double PiPlus_Theta_GFJ_Meson = locPiPlusP3_GFJ_Meson.CosTheta();



	// bost vector to delta++ frame
	
	TVector3 locBoostVector_DeltaPlusPlus = -1.0*(locThrownPiPlusProtonP4.BoostVector());

	TLorentzVector locPi0P4_DeltaSystem(locThrownPi0P4);
	TLorentzVector locPiMinusP4_DeltaSystem(locThrownPiMinusP4);
	TLorentzVector locBeamP4_DeltaSystem(locThrownBeamP4);
	TLorentzVector locPiMinusPi0P4_DeltaSystem(locThrownPiMinusPi0P4);
	TLorentzVector locTargetP4_DeltaSystem(dTargetP4);
	TLorentzVector locPiPlusProtonP4_DeltaSystem(locThrownPiPlusProtonP4);
	TLorentzVector locPiPlusP4_DeltaSystem(locThrownPiPlusP4);



	locPi0P4_DeltaSystem.Boost(locBoostVector_DeltaPlusPlus);
	locPiMinusP4_DeltaSystem.Boost(locBoostVector_DeltaPlusPlus);
	locPiMinusPi0P4_DeltaSystem.Boost(locBoostVector_DeltaPlusPlus);
	locPiPlusProtonP4_DeltaSystem.Boost(locBoostVector_DeltaPlusPlus);
	locBeamP4_DeltaSystem.Boost(locBoostVector_DeltaPlusPlus);
	locTargetP4_DeltaSystem.Boost(locBoostVector_DeltaPlusPlus);
	locPiPlusP4_DeltaSystem.Boost(locBoostVector_DeltaPlusPlus);
	//define  coordinate system GFJ
	TVector3 vz_GFJ = locBeamP4_DeltaSystem.Vect().Unit();
	TVector3 vy_GFJ = (locBeamP4_DeltaSystem.Vect().Cross(locPiMinusPi0P4_DeltaSystem.Vect())).Unit();
	TVector3 vx_GFJ = (vy_GFJ.Cross(vz_GFJ)).Unit();
	//now calculate coordinates in new system by calculating dot product with new basis
	TVector3 locPiMinusPi0P3_GFJ(locPiMinusPi0P4_DeltaSystem.Vect().Dot(vx_GFJ), locPiMinusPi0P4_DeltaSystem.Vect().Dot(vy_GFJ), locPiMinusPi0P4_DeltaSystem.Vect().Dot(vz_GFJ));
	TVector3 locPiPlusProtonP3_GFJ(locPiPlusProtonP4_DeltaSystem.Vect().Dot(vx_GFJ), locPiPlusProtonP4_DeltaSystem.Vect().Dot(vy_GFJ), locPiPlusProtonP4_DeltaSystem.Vect().Dot(vz_GFJ));
	TVector3 locPiPlusP3_GFJ(locPiPlusP4_DeltaSystem.Vect().Dot(vx_GFJ), locPiPlusP4_DeltaSystem.Vect().Dot(vy_GFJ), locPiPlusP4_DeltaSystem.Vect().Dot(vz_GFJ));
	TVector3 locPiMinusP3_GFJ(locPiMinusP4_DeltaSystem.Vect().Dot(vx_GFJ), locPiMinusP4_DeltaSystem.Vect().Dot(vy_GFJ), locPiMinusP4_DeltaSystem.Vect().Dot(vz_GFJ));

	
	double PiPlusPi0_Phi_GFJ = (locPiPlusProtonP3_GFJ.Phi())*TMath::RadToDeg();
	double PiPlusPi0_Theta_GFJ = locPiPlusProtonP3_GFJ.CosTheta();

	double PiPlus_Phi_GFJ = (locPiPlusP3_GFJ.Phi())*TMath::RadToDeg();
	double PiPlus_Theta_GFJ = locPiPlusP3_GFJ.CosTheta();
	
	double PiMinus_Phi_GFJ = (locPiMinusP3_GFJ.Phi())*TMath::RadToDeg();
	double PiMinus_Theta_GFJ = locPiMinusP3_GFJ.CosTheta();;

	TVector3 vz_HF = locPiMinusPi0P4_DeltaSystem.Vect().Unit();
	TVector3 vy_HF = (locBeamP4_DeltaSystem.Vect().Cross(locPiMinusPi0P4_DeltaSystem.Vect())).Unit();
	TVector3 vx_HF = (vy_HF.Cross(vz_HF)).Unit();
	
	TVector3 locPiMinusPi0P3_HF(locPiMinusPi0P4_DeltaSystem.Vect().Dot(vx_HF), locPiMinusPi0P4_DeltaSystem.Vect().Dot(vy_HF), locPiMinusPi0P4_DeltaSystem.Vect().Dot(vz_HF));
	TVector3 locPiPlusProtonP3_HF(locPiPlusProtonP4_DeltaSystem.Vect().Dot(vx_HF), locPiPlusProtonP4_DeltaSystem.Vect().Dot(vy_HF), locPiPlusProtonP4_DeltaSystem.Vect().Dot(vz_HF));
	TVector3 locPiPlusP3_HF(locPiPlusP4_DeltaSystem.Vect().Dot(vx_HF), locPiPlusP4_DeltaSystem.Vect().Dot(vy_HF), locPiPlusP4_DeltaSystem.Vect().Dot(vz_HF));
	TVector3 locPiMinusP3_HF(locPiMinusP4_DeltaSystem.Vect().Dot(vx_HF), locPiMinusP4_DeltaSystem.Vect().Dot(vy_HF), locPiMinusP4_DeltaSystem.Vect().Dot(vz_HF));
	
	
	double PiMinusPi0_Phi_HF = locPiMinusPi0P3_HF.Phi();
	double PiMinusPi0_Theta_HF = locPiMinusPi0P3_HF.CosTheta();

	double PiMinus_Phi_HF = locPiMinusP3_HF.Phi();
	double PiMinus_Theta_HF = locPiMinusP3_HF.CosTheta();

	double PiPlus_Phi_HF = locPiPlusP3_HF.Phi();
	double PiPlus_Theta_HF = locPiPlusP3_HF.CosTheta();
	dHist_PiPlus_Angle_HF->Fill(PiPlus_Phi_HF,PiPlus_Theta_HF,weight);
	dHist_PiMinus_Angle_HF->Fill(PiMinus_Phi_HF,PiMinus_Theta_HF,weight);
	
	dHist_PiPlus_Angle_GFJ->Fill(PiPlus_Phi_GFJ,PiPlus_Theta_GFJ,weight);
	dHist_PiMinus_Angle_GFJ->Fill(PiMinus_Phi_GFJ,PiMinus_Theta_GFJ,weight);
	dHist_PiPlus_Angle_HF_Meson->Fill(PiPlus_Phi_HF_Meson,PiPlus_Theta_HF_Meson,weight);
	dHist_PiMinus_Angle_HF_Meson->Fill(PiMinus_Phi_HF_Meson,PiMinus_Theta_HF_Meson,weight);
	dHist_PiMinusPi0_Angle_HF_Meson->Fill(PiMinusPi0_Phi_HF_Meson,PiMinusPi0_Theta_HF_Meson,weight);


	dHist_PiPlus_Angle_GFJ_Meson->Fill(PiPlus_Phi_GFJ_Meson,PiPlus_Theta_GFJ_Meson,weight);
	dHist_PiMinus_Angle_GFJ_Meson->Fill(PiMinus_Phi_GFJ_Meson,PiMinus_Theta_GFJ_Meson,weight);
	dHist_PiMinusPi0_Angle_GFJ_Meson->Fill(PiMinusPi0_Phi_GFJ_Meson,PiMinusPi0_Theta_GFJ_Meson,weight);

	if (momentum_transfer_t > 0.025 && momentum_transfer_t < 1.4 && locThrownPiPlusProtonP4.M() > 0.6 && locThrownPiPlusProtonP4.M() < 1.6 && locThrownPiMinusPi0P4.M() > 0.3 && locThrownPiMinusPi0P4.M() < 1.1 ){
	vector<TLorentzVector> locThrownFinalStateP4; // should be in the same order as PID_FinalState
	locThrownFinalStateP4.push_back(locThrownProtonP4);
	locThrownFinalStateP4.push_back(locThrownPiPlusP4);
	locThrownFinalStateP4.push_back(locThrownPiMinusP4); 
	locThrownFinalStateP4.push_back(locThrownPi0P4);


	dFlatTreeInterface->Fill_Fundamental<float>("Weight", weight);
	dFlatTreeInterface->Fill_Fundamental<float>("p_pip", locThrownPiPlusP4.P());
	dFlatTreeInterface->Fill_Fundamental<float>("m_proton_pip", locThrownPiPlusProtonP4.M());
	dFlatTreeInterface->Fill_Fundamental<float>("mandel_t", momentum_transfer_t);
	dFlatTreeInterface->Fill_Fundamental<float>("m_pi0_pim", locThrownPiMinusPi0P4.M());
	dFlatTreeInterface->Fill_Fundamental<float>("m_pi0_pip_pim",locThrownOmegaP4.M());
	dFlatTreeInterface->Fill_Fundamental<float>("m_pi0", locThrownPi0P4.M());
	dFlatTreeInterface->Fill_Fundamental<float>("E_beam", locThrownBeamP4.E());
	dFlatTreeInterface->Fill_Fundamental<float>("z_pi0", locThrownPi0P4.Z());
	dFlatTreeInterface->Fill_Fundamental<float>("z_pim", locThrownPiMinusP4.Z());
	dFlatTreeInterface->Fill_Fundamental<float>("z_pip", locThrownPiPlusP4.Z());
	dFlatTreeInterface->Fill_Fundamental<float>("z_proton", locThrownProtonP4.Z());

	FillAmpTools_FlatTree(locThrownBeamP4, locThrownFinalStateP4);
	Fill_FlatTree(); }

	/******************************************* BIN THROWN DATA INTO SEPARATE TREES FOR AMPTOOLS ***************************************/

/*
	//THESE KEYS MUST BE DEFINED IN THE INIT SECTION (along with the output file names)
	if((locBeamEnergyUsedForBinning >= 8.0) && (locBeamEnergyUsedForBinning < 9.0))
		Fill_OutputTree("Bin1"); //your user-defined key
	else if((locBeamEnergyUsedForBinning >= 9.0) && (locBeamEnergyUsedForBinning < 10.0))
		Fill_OutputTree("Bin2"); //your user-defined key
	else if((locBeamEnergyUsedForBinning >= 10.0) && (locBeamEnergyUsedForBinning < 11.0))
		Fill_OutputTree("Bin3"); //your user-defined key
*/
}}
	return kTRUE;
}

void DSelector_thrown::Finalize(void)
{
	//Save anything to output here that you do not want to be in the default DSelector output ROOT file.

	//Otherwise, don't do anything else (especially if you are using PROOF).
		//If you are using PROOF, this function is called on each thread,
		//so anything you do will not have the combined information from the various threads.
		//Besides, it is best-practice to do post-processing (e.g. fitting) separately, in case there is a problem.

	//DO YOUR STUFF HERE

	//CALL THIS LAST
	DSelector::Finalize(); //Saves results to the output file
}
