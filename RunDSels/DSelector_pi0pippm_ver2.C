#include "DSelector_pi0pippm_ver2.h"
#include "TParameter.h"

void DSelector_pi0pippm_ver2::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.
	 // Pfad anpassen
	
	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "pi0pippm_ver2.root"; //"" for none
	dOutputTreeFileName = ""; //"" for none
	dFlatTreeFileName = "flat_tree.root"; //output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = "kin"; //if blank, default name will be chosen
	//dSaveDefaultFlatBranches = true; // False: don't save default branches, reduce disk footprint.
	//dSaveTLorentzVectorsAsFundamentaFlatTree = false; // Default (or false): save particles as TLorentzVector objects. True: save as four doubles instead.

	//Because this function gets called for each TTree in the TChain, we must be careful:
		//We need to re-initialize the tree interface & branch wrappers, but don't want to recreate histograms
	bool locInitializedPriorFlag = dInitializedFlag; //save whether have been initialized previously
	DSelector::Init(locTree); //This must be called to initialize wrappers for each new TTree
	//gDirectory now points to the output file with name dOutputFileName (if any)
	if(locInitializedPriorFlag)
		return; //have already created histograms, etc. below: exit

	Get_ComboWrappers();

	dPreviousRunNumber = 0;

	/*********************************** EXAMPLE USER INITIALIZATION: ANALYSIS ACTIONS **********************************/

	// EXAMPLE: Create deque for histogramming particle masses:
	// // For histogramming the phi mass in phi -> K+ K-
	// // Be sure to change this and dAnalyzeCutActions to match reaction
	std::deque<Particle_t> MyParticles;
	MyParticles.push_back(Gamma);
	MyParticles.push_back(Gamma);

	//ANALYSIS ACTIONS: //Executed in order if added to dAnalysisActions
	//false/true below: use measured/kinfit data

	//PID
	dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, false));
	//below: value: +/- N ns, UnknownParticle: All PIDs, SYS_NULL: all timing systems
	//dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.5, KPlus, SYS_BCAL));

	//PIDFOM (for charged tracks)
	dAnalysisActions.push_back(new DHistogramAction_PIDFOM(dComboWrapper));
	//dAnalysisActions.push_back(new DCutAction_PIDFOM(dComboWrapper, KPlus, 0.1));
	//dAnalysisActions.push_back(new DCutAction_EachPIDFOM(dComboWrapper, 0.1));

	//MASSES
	//dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, Lambda, 1000, 1.0, 1.2, "Lambda"));
	//dAnalysisActions.push_back(new DHistogramAction_MissingMassSquared(dComboWrapper, false, 1000, -0.1, 0.1));

	//KINFIT RESULTS
	dAnalysisActions.push_back(new DHistogramAction_KinFitResults(dComboWrapper));

	//CUT MISSING MASS
	//dAnalysisActions.push_back(new DCutAction_MissingMassSquared(dComboWrapper, false, -0.03, 0.02));

	//CUT ON SHOWER QUALITY
	//dAnalysisActions.push_back(new DCutAction_ShowerQuality(dComboWrapper, SYS_FCAL, 0.5));

	//BEAM ENERGY
	dAnalysisActions.push_back(new DHistogramAction_BeamEnergy(dComboWrapper, false));
	//dAnalysisActions.push_back(new DCutAction_BeamEnergy(dComboWrapper, false, 8.2, 8.8));  

	//KINEMATICS
	dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, false));

	// ANALYZE CUT ACTIONS
	// // Change MyPhi to match reaction
	dAnalyzeCutActions = new DHistogramAction_AnalyzeCutActions( dAnalysisActions, dComboWrapper, false, 1, MyParticles, 1000, 0.9, 2.4, "CutActionEffect" );

	//INITIALIZE ACTIONS
	//If you create any actions that you want to run manually (i.e. don't add to dAnalysisActions), be sure to initialize them here as well
	Initialize_Actions();
	dAnalyzeCutActions->Initialize(); // manual action, must call Initialize()

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/

	//Define t bins
	double tbins[17] =  {0.025,0.075,0.1,0.125,0.150,0.175,0.2,0.24,0.28,0.34,0.40,0.46,0.54,0.64,0.85,1.05,1.4}; 
	//EXAMPLE MANUAL HISTOGRAMS:
	
	dHist_2D_Mass_3pi_vs_pi0pim = new TH2F("Mass_3pi_vs_pi0pim", ";M(#pi^{0}#pi^{-}) [GeV];M(#pi^{0}#pi^{+}#pi^{-}) [GeV]", 200, 0, 11, 200, 0, 17);
	dHist_2D_Mass_3pi_vs_ppip = new TH2F("Mass_3pi_vs_ppip", ";M(#pi^{+}p) [GeV];M(#pi^{0}#pi^{+}#pi^{-}) [GeV]", 200, 0, 4, 200, 0, 4);
	dHist_2D_Mass_pi0pim_vs_ppip = new TH2F("Mass_pi0pim_vs_ppip", ";M(#pi^{+}p) [GeV];M(#pi^{0}#pi^{-}) [GeV]", 200, 0, 4, 200, 0, 4);
	dHist_2D_Mass_pippim_vs_ppi0 = new TH2F("Mass_pippim_vs_ppi0", ";M(#pi^{0}p) [GeV];M(#pi^{+}#pi^{-}) [GeV]", 200, 0, 4, 200, 0, 4);
	dHist_2D_Mass_pi0pip_vs_ppim = new TH2F("Mass_pi0pip_vs_ppim", ";M(#pi^{-}p) [GeV];M(#pi^{+}#pi^{0}) [GeV]", 200, 0, 4, 200, 0, 4);
	
	//not per combo! per event!
	dHist_BeamEnergy = new TH1F("beam_energy", "E_{beam};E_{beam} (GeV);Events", 100, 6.0, 12.0);
	dHist_MissingMassSquared = new TH1F("MissingMassSqu", "Missing Mass Squared Distribution;(M_{Miss})^2 (GeV)^{2};Events", 600, -0.06, 0.06);
	dHist_combo_before = new TH1F("combo_before", "combos / event;combos / event", 50, 0, 50);
	dHist_combo_after = new TH1F("combo_after", "combos / event;combos / event", 50, 0, 50);
	



   // angles
	dHist_Proton_Angle = new TH2F("proton_angle", "p angle distribution;#phi (deg);#theta (deg)", 360, -180, 180, 180, 0, 180);
	dHist_Pi0_Angle = new TH2F("pi0_angle", "#pi^{0} angle distribution;#phi (deg);#theta (deg)", 360, -180, 180, 180, 0, 180);
	dHist_PiPlus_Angle = new TH2F("piplus_angle", "#pi^{+} angle distribution;#phi (deg);#theta (deg)", 360, -180, 180, 180, 0, 180);
	dHist_PiMinus_Angle = new TH2F("piminus_angle", "#pi^{-} angle distribution;#phi (deg);#theta (deg)", 360, -180, 180, 180, 0, 180);
	dHist_Photon1_Angle = new TH2F("gamma_1_angle", "#gamma_{1} angle distribution;#phi (deg);#theta (deg)", 360, -180, 180, 180, 0, 180);
	dHist_Photon2_Angle = new TH2F("gamma_2_angle", "#gamma_{2} angle distribution;#phi (deg);#theta (deg)", 360, -180, 180, 180, 0, 180);
	dHist_RhoMinus_Angle = new TH2F("rho_minus_angle", "#pi^{-}#pi^{0} angle distribution;#phi (deg);#theta (deg)", 360, -180, 180, 180, 0, 180);
	dHist_RhoPlus_Angle = new TH2F("rho_plus_angle", "#pi^{0}#pi^{+} angle distribution;#phi (deg);#theta (deg)", 360, -180, 180, 180, 0, 180);
	dHist_Rho0_Angle = new TH2F("rho0_angle", "#pi^{+}#pi^{-} angle distribution;#phi (deg);#theta (deg)", 360, -180, 180, 180, 0, 180);
	dHist_Omega_Angle = new TH2F("omega_angle", "#pi^{+}#pi^{-}#pi^{0} angle distribution;#phi (deg);#theta (deg)", 360, -180, 180, 180, 0, 180);
	dHist_Delta_Angle = new TH2F("delta_angle", "p#pi^{+} angle distribution;#phi (deg);#theta (deg)", 360, -180, 180, 180, 0, 180);
	
    /***************Histogram Impact of each cut on variables ********************/
	std::vector<HistConfig> histConfigs = {
		{"hBeamEnergy", ";E_{Beam} (GeV);Counts", 100, 6.0, 12.0},
		{"hChi2_kinfit", ";#chi^{2}/NDF;Counts", 100, 0, 20},
		{"hMissingMassSquared", ";(M_{Miss})^2 (GeV)^{2};Counts", 600, -0.06, 0.06},
		{"hz_vertex", ";z_{vertex} [cm];z_{vertex} [cm];Counts", 200, 0.0, 120.0},
		{"hR_vertex", ";Vertex R;R_{vertex} [cm];Counts", 500, 0.0, 2.0},
		{"hShower_Quality", ";Shower Quality;{qf}_{#gamma1};Counts", 100, 0, 1},
		{"hNumShowers", ";N_{unusedshowers};Counts", 20, 0, 20},
		{"hNumChargedTracks", ";N_{unusedchargedtracks};Counts", 10, 0, 10},
		{"hPi0_mass", ";M(#gamma_{1} #gamma_{2}) [GeV];Counts", 100, 0.05, 0.25},
		{"ht", ";-t [GeV^{2}]; -t [GeV^{2}];Counts", 200, 0.0, 3},
		{"hOmega_mass", ";M(#pi^+ #pi^- #pi^0) [GeV];Counts", 200, 0.0, 3},
		{"hDelta", ";M(#pi^+ #p) [GeV];Counts", 200, 1.0, 5.0},
		{"hrho_mass", ";M(#pi^- #pi^0) [GeV];Counts", 200, 0.0, 3},
		   //last variable which is also cut variable
		{"hrho_zero_mass",";M(#pi^+ #pi^-) [GeV];Counts", 200, 0.0, 3.0},
		{"hrho_plus_mass",";M(#pi^0 #pi^+) [GeV];Counts", 200, 0.0, 3.0},
		{"hrho_diff_angle",";#phi_{#Delta^{++}, meas} - #phi_{#rho^{-},meas}(deg);Counts", 360, -360, 360},
		{"c", ";M(#pi^- #pi^0,meas) [GeV];Counts", 200, 0.0, 3}

	};



	const int nCuts = 13;

	histMatrix.resize(histConfigs.size());
	
	for (size_t v = 0; v < histConfigs.size(); ++v) {
		const auto& cfg = histConfigs[v];
	
		histMatrix[v].resize(nCuts + 1); // +1 für alle Cuts, +1 für n-1 Plots
	
		for (int c = 0; c <= nCuts; ++c) {
			std::string hname = Form("%s_cut%d", cfg.name.c_str(), c);
			std::string htitle = Form("%s (after cut %d);%s", cfg.title.c_str(), c, cfg.title.c_str());
	
			histMatrix[v][c] = new TH1F(
				hname.c_str(),
				htitle.c_str(),
				cfg.nbins,
				cfg.xmin,
				cfg.xmax
			);
		}
	}


	
	std::vector<Hist2DConfig> hist2DConfigs = {
	{"tchannel_ProtonPiPlus", "p #pi^{+} vs. t channel;-t (GeV^{2});m_{p #pi^{+}} (GeV)", 16, 500, 0, 5},
    {"tchannel_Pi0PiMinus", "#pi^{0} #pi^{-} vs. t channel;-t (GeV^{2});m_{#pi^{0} #pi^{-}} (GeV)", 16, 500, 0, 5},
    {"tchannel_Pi0PiPlus", "#pi^{0} #pi^{+} vs. t channel;-t (GeV^{2});m_{#pi^{0} #pi^{+}} (GeV)", 16, 500, 0, 5},
    {"tchannel_PiPlusPiMinus", "#pi^{+} #pi^{-} vs. t channel;-t (GeV^{2});m_{#pi^{+} #pi^{-}} (GeV)", 16, 500, 0, 5},
    {"tchannel_ProtonPiMinus", "p #pi^{-} vs. t channel;-t (GeV^{2});m_{p #pi^{-}} (GeV)", 16, 500, 0, 5},
    {"tchannel_ProtonPi0", "p #pi^{0} vs. t channel;-t (GeV^{2});m_{p #pi^{0}} (GeV)", 16, 500, 0, 5}
	};
	const int nMassCuts = 4;
	
	hist2DMatrix.resize(hist2DConfigs.size());

	for (size_t v = 0; v < hist2DConfigs.size(); ++v) {
		const auto& cfg = hist2DConfigs[v];

		hist2DMatrix[v].resize(nMassCuts + 2); 

		for (int c = 0; c <= nMassCuts + 1; ++c) {
			std::string hname  = Form("%s_cut%d", cfg.name.c_str(), c);
			std::string htitle = Form("%s (after cut %d)", cfg.title.c_str(), c);

			hist2DMatrix[v][c] = new TH2F(
				hname.c_str(),
				htitle.c_str(),
				cfg.nbinsX, tbins,   
				cfg.nbinsY, cfg.ymin, cfg.ymax  
			);
		}
	}
	

	/************************** EXAMPLE USER INITIALIZATION: CUSTOM OUTPUT BRANCHES - MAIN TREE *************************/

	//EXAMPLE MAIN TREE CUSTOM BRANCHES (OUTPUT ROOT FILE NAME MUST FIRST BE GIVEN!!!! (ABOVE: TOP)):
	//The type for the branch must be included in the brackets
	//1st function argument is the name of the branch
	//2nd function argument is the name of the branch that contains the size of the array (for fundamentals only)
	/*
	dTreeInterface->Create_Branch_Fundamental<Int_t>("my_int"); //fundamental = char, int, float, double, etc.
	dTreeInterface->Create_Branch_FundamentalArray<Int_t>("my_int_array", "my_int");
	dTreeInterface->Create_Branch_FundamentalArray<Float_t>("my_combo_array", "NumCombos");
	dTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("my_p4");
	dTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("my_p4_array");
	*/

	/************************** EXAMPLE USER INITIALIZATION: CUSTOM OUTPUT BRANCHES - FLAT TREE *************************/

	// RECOMMENDED: CREATE ACCIDENTAL WEIGHT BRANCH
	// dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("accidweight");

	//EXAMPLE FLAT TREE CUSTOM BRANCHES (OUTPUT ROOT FILE NAME MUST FIRST BE GIVEN!!!! (ABOVE: TOP)):
	//The type for the branch must be included in the brackets
	//1st function argument is the name of the branch
	//2nd function argument is the name of the branch that contains the size of the array (for fundamentals only)
	/*
	dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("flat_my_int"); //fundamental = char, int, float, double, etc.
	dFlatTreeInterface->Create_Branch_FundamentalArray<Int_t>("flat_my_int_array", "flat_my_int");
	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("flat_my_p4");
	dFlatTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("flat_my_p4_array");
	*/

	SetupAmpTools_FlatTree();
	dFlatTreeInterface->Create_Branch_Fundamental<float>("p_proton");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("p_pim");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("p_pip");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("p_pi0");


	// invariant masses of combinations
	dFlatTreeInterface->Create_Branch_Fundamental<float>("m_proton_pim");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("m_proton_pip");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("m_proton_pi0");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("m_pi0_pip");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("m_pi0_pim");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("m_pip_pim");

	// additional information
	dFlatTreeInterface->Create_Branch_Fundamental<float>("mandel_t");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("Weight_user");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("chi2ndf");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("missingMassSquared");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("unusedShowers");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("unusedTracks");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("showerQualityPhoton1");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("showerQualityPhoton2");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("polstate");
	
	/************************************* ADVANCED EXAMPLE: CHOOSE BRANCHES TO READ ************************************/

	//TO SAVE PROCESSING TIME
		//If you know you don't need all of the branches/data, but just a subset of it, you can speed things up
		//By default, for each event, the data is retrieved for all branches
		//If you know you only need data for some branches, you can skip grabbing data from the branches you don't need
		//Do this by doing something similar to the commented code below

	//dTreeInterface->Clear_GetEntryBranches(); //now get none
	//dTreeInterface->Register_GetEntryBranch("Proton__P4"); //manually set the branches you want

	/************************************** DETERMINE IF ANALYZING SIMULATED DATA *************************************/
	

	dIsMC = (dTreeInterface->Get_Branch("MCWeight") != NULL);

	if(dIsMC)
	{
		
		
		dHistThrownTopologies = new TH1F("hThrownTopologies","hThrownTopologies", 10, -0.5, 9.5);

		histMatrix3D.resize(histConfigs.size());
		topologien = {
			"2#gamma#pi^{#plus}#pi^{#minus}p[#pi^{0}]",
			"4#gamma#pi^{#plus}#pi^{#minus}p[2#pi^{0}]",
			"2#gamma#pi^{#plus}#pi^{#minus}p[#pi^{0},#omega]",
			"6#gamma#pi^{#plus}#pi^{#minus}p[3#pi^{0},#eta]",
			"2#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0}]",
			"4#gamma#pi^{#plus}#pi^{#minus}p[2#pi^{0},#omega]",
			"2#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#omega]",
			"6#gamma#pi^{#plus}#pi^{#minus}p[3#pi^{0}]",
			"4#gamma2#pi^{#plus}2#pi^{#minus}p[2#pi^{0},#omega]",
			"4#gamma2#pi^{#plus}2#pi^{#minus}p[2#pi^{0}]",
			"other"
		};
		for (size_t v = 0; v < histConfigs.size(); ++v) {
			histMatrix3D[v].resize(nCuts + 1);

			for (int c = 0; c <= nCuts; ++c) {
				for (const auto& topo : topologien) {
					std::string hname = Form("%s_cut%d_topo_%s", histConfigs[v].name.c_str(), c, topo.c_str());
					std::string htitle = Form("%s (cut %d, topo %s)", histConfigs[v].title.c_str(), c, topo.c_str());

					histMatrix3D[v][c][topo] = new TH1F(
						hname.c_str(),
						htitle.c_str(),
						histConfigs[v].nbins,
						histConfigs[v].xmin,
						histConfigs[v].xmax
					);
				}
			}
		}
		Nsurvived.resize(nCuts + 1);
		
		
		

	};
	
	//Write Cut values in tree
	float sigmafitpi0mass = 0.0054;
	float mufitpi0mass = 0.1349;
	float breitePi0 = sigmafitpi0mass * 2.355;
	 
	dCut_chi2perndf = 5.0;
    dCut_Ebeam = {8.2, 8.8};
    dCut_MissingMass = {-0.01, 0.01};
    dCut_shower = 5;
    dCut_qf = 0.5;
    dCut_R = 1.0;
    dCut_z = {50.0, 80.0};
    dCut_Delta = {1.1, 1.35};
    dCut_tracks = 0;
    dCut_rho_mass = {0.62, 0.93};
    dCut_t = 1.4;
	dCut_Pi0_mass = {mufitpi0mass-breitePi0,mufitpi0mass+breitePi0};
	dCut_Omega_mass = 0.9;


	
    TDirectory* cutDir = gDirectory->mkdir("Cuts");
	cutDir->cd();

	auto p = new TParameter<double>("Cut_BeamEnergy_min", dCut_Ebeam.first); p->Write(); delete p;
	p = new TParameter<double>("Cut_BeamEnergy_max", dCut_Ebeam.second); p->Write(); delete p;
	p = new TParameter<double>("Cut_Chi2_kinfit_max", dCut_chi2perndf); p->Write(); delete p;
	p = new TParameter<double>("Cut_MissingMassSquared_min", dCut_MissingMass.first); p->Write(); delete p;
	p = new TParameter<double>("Cut_MissingMassSquared_max", dCut_MissingMass.second); p->Write(); delete p;
	p = new TParameter<double>("Cut_NumShowers_max", dCut_shower); p->Write(); delete p;
	p = new TParameter<double>("Cut_Shower_Quality_min", dCut_qf); p->Write(); delete p;
	p = new TParameter<double>("Cut_R_vertex_max", dCut_R); p->Write(); delete p;
	p = new TParameter<double>("Cut_z_primary_vertex_min", dCut_z.first); p->Write(); delete p;
	p = new TParameter<double>("Cut_z_primary_vertex_max", dCut_z.second); p->Write(); delete p;
	p = new TParameter<double>("Cut_Delta_min", dCut_Delta.first); p->Write(); delete p;
	p = new TParameter<double>("Cut_Delta_max", dCut_Delta.second); p->Write(); delete p;
	p = new TParameter<double>("Cut_NumChargedTracks_max", dCut_tracks); p->Write(); delete p;
	p = new TParameter<double>("Cut_rho_mass_min", dCut_rho_mass.first); p->Write(); delete p;
	p = new TParameter<double>("Cut_rho_mass_max", dCut_rho_mass.second); p->Write(); delete p;
	p = new TParameter<double>("Cut_t_max", dCut_t); p->Write(); delete p;
	p = new TParameter<double>("Cut_Pi0_min", dCut_Pi0_mass.first); p->Write(); delete p;
	p = new TParameter<double>("Cut_Pi0_max", dCut_Pi0_mass.second); p->Write(); delete p;
	p = new TParameter<double>("Cut_Omega_mass_min", dCut_Omega_mass); p->Write(); delete p;

	gDirectory->cd("/");


	
}



Bool_t DSelector_pi0pippm_ver2::Process(Long64_t locEntry)
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
	//TLorentzVector locProductionX4 = Get_X4_Production();

	/******************************************** GET POLARIZATION ORIENTATION ******************************************/

	//Only if the run number changes
	//RCDB environment must be setup in order for this to work! (Will return false otherwise)
	UInt_t locRunNumber = Get_RunNumber();
	
	// if(locRunNumber != dPreviousRunNumber)
	// {
	// 	dIsPolarizedFlag = dAnalysisUtilities.Get_IsPolarizedBeam(locRunNumber, dIsPARAFlag);
	// 	dPreviousRunNumber = locRunNumber;
	// }
	// locRunNumber = Get_RunNumber();
	if(locRunNumber != dPreviousRunNumber)
	{
		dAnalysisUtilities.Get_PolarizationAngle(locRunNumber, dBeamPolarisationAngle);
		dPreviousRunNumber = locRunNumber;
		cout << "Beam polarisation angle is: " << dBeamPolarisationAngle << " degrees." << endl;
	}
	
	if(dBeamPolarisationAngle == (-1.0)) // 0 = amo, 1 = para 0, 2 = perp 90, 3 = para 135, 4 = perp 45
		polarisationState = 0;
	else if (dBeamPolarisationAngle == (0.0))
		polarisationState = 1;
	else if (dBeamPolarisationAngle == (90.0))
		polarisationState = 2;
	else if (dBeamPolarisationAngle == (135.0))
		polarisationState = 3;
	else if (dBeamPolarisationAngle == (45.0))
		polarisationState = 4;


	/********************************************* SETUP UNIQUENESS TRACKING ********************************************/

	//ANALYSIS ACTIONS: Reset uniqueness tracking for each action
	//For any actions that you are executing manually, be sure to call Reset_NewEvent() on them here
	Reset_Actions_NewEvent();
	dAnalyzeCutActions->Reset_NewEvent(); // manual action, must call Reset_NewEvent()

	//PREVENT-DOUBLE COUNTING WHEN HISTOGRAMMING
		//Sometimes, some content is the exact same between one combo and the next
			//e.g. maybe two combos have different beam particles, but the same data for the final-state
		//When histogramming, you don't want to double-count when this happens: artificially inflates your signal (or background)
		//So, for each quantity you histogram, keep track of what particles you used (for a given combo)
		//Then for each combo, just compare to what you used before, and make sure it's unique

	//EXAMPLE 0: Event-specific info:
	Bool_t locUsedSoFar_Event = false; // Flag used to mark if the best chi-squared combo is filled in the histogram

	//EXAMPLE 1: Particle-specific info:
	set<Int_t> locUsedSoFar_BeamEnergy; //Int_t: Unique ID for beam particles. set: easy to use, fast to search. This container is used for the "hybrid" method dealing with combinatorics.

	//EXAMPLE 2: Combo-specific info:
		//In general: Could have multiple particles with the same PID: Use a set of Int_t's
		//In general: Multiple PIDs, so multiple sets: Contain within a map
		//Multiple combos: Contain maps within a set (easier, faster to search)
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MissingMass;

	//INSERT USER ANALYSIS UNIQUENESS TRACKING HERE

	/**************************************** EXAMPLE: FILL CUSTOM OUTPUT BRANCHES **************************************/

	/*
	Int_t locMyInt = 7;
	dTreeInterface->Fill_Fundamental<Int_t>("my_int", locMyInt);

	TLorentzVector locMyP4(4.0, 3.0, 2.0, 1.0);
	dTreeInterface->Fill_TObject<TLorentzVector>("my_p4", locMyP4);

	for(int loc_i = 0; loc_i < locMyInt; ++loc_i)
		dTreeInterface->Fill_Fundamental<Int_t>("my_int_array", 3*loc_i, loc_i); //2nd argument = value, 3rd = array index
	*/

	/************************************************* LOOP OVER COMBOS *************************************************/

	// Vector to store combo information
	std::vector<std::pair<UInt_t, Double_t>> loc_combos;
	int combos = 0;
	// Pre-loop to gather kinfit ComboIndex-chiSq pairing and sort by chiSq value ascendingly
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i)
	{
		dComboWrapper->Set_ComboIndex(loc_i);
		Double_t locChiSq = dComboWrapper->Get_ChiSq_KinFit("");
		loc_combos.push_back(std::make_pair(loc_i, locChiSq));
	}
	// Sort the combos by ChiSq
	std::sort(loc_combos.begin(), loc_combos.end(), [](const std::pair<UInt_t, Double_t>& a, const std::pair<UInt_t, Double_t>& b) {
		return a.second < b.second;
	});
	std::string locThrownTopology;
	float MCweight = 1.0;

	
	if(dIsMC)
    {locThrownTopology = Get_ThrownTopologyString();
	//MCweight= Get_MCWeight();
	
	
	}
	//MCweight= Get_MCWeight();
	//float MCweight = 1.0;
	//Loop over combos
	bool isFirst = true;
	for(const auto& loc_combo : loc_combos)
	{
		UInt_t loc_i = loc_combo.first;
		
		//Set branch array indices for combo and all combo particles
		dComboWrapper->Set_ComboIndex(loc_i);

		// Is used to indicate when combos have been cut
		if(dComboWrapper->Get_IsComboCut()) // Is false when tree originally created
			continue; // Combo has been cut previously
		
		/********************************************** GET PARTICLE INDICES *********************************************/

		//Used for tracking uniqueness when filling histograms, and for determining unused particles

		//Step 0
		Int_t locBeamID = dComboBeamWrapper->Get_BeamID();
		Int_t locPiPlusTrackID = dPiPlusWrapper->Get_TrackID();
		Int_t locPiMinusTrackID = dPiMinusWrapper->Get_TrackID();
		Int_t locProtonTrackID = dProtonWrapper->Get_TrackID();

		//Step 1
		Int_t locPhoton1NeutralID = dPhoton1Wrapper->Get_NeutralID();
		Int_t locPhoton2NeutralID = dPhoton2Wrapper->Get_NeutralID();

		/*********************************************** GET FOUR-MOMENTUM **********************************************/

		// Get P4's: //is kinfit if kinfit performed, else is measured
		//dTargetP4 is target p4
		//Step 0
		TLorentzVector locBeamP4 = dComboBeamWrapper->Get_P4();
		TLorentzVector locPiPlusP4 = dPiPlusWrapper->Get_P4();
		TLorentzVector locPiMinusP4 = dPiMinusWrapper->Get_P4();
		TLorentzVector locProtonP4 = dProtonWrapper->Get_P4();
		//Step 1
		TLorentzVector locPhoton1P4 = dPhoton1Wrapper->Get_P4();
		TLorentzVector locPhoton2P4 = dPhoton2Wrapper->Get_P4();
		TLorentzVector locPi0P4 = locPhoton1P4 + locPhoton2P4;
		// Get Measured P4's:
		//Step 0
		TLorentzVector locBeamP4_Measured = dComboBeamWrapper->Get_P4_Measured();
		TLorentzVector locPiPlusP4_Measured = dPiPlusWrapper->Get_P4_Measured();
		TLorentzVector locPiMinusP4_Measured = dPiMinusWrapper->Get_P4_Measured();
		TLorentzVector locProtonP4_Measured = dProtonWrapper->Get_P4_Measured();
		//Step 1
		TLorentzVector locPhoton1P4_Measured = dPhoton1Wrapper->Get_P4_Measured();
		TLorentzVector locPhoton2P4_Measured = dPhoton2Wrapper->Get_P4_Measured();

		/********************************************* GET COMBO RF TIMING INFO *****************************************/
		TLorentzVector locBeamX4 = dComboBeamWrapper->Get_X4();
		TLorentzVector locBeamX4_Measured = dComboBeamWrapper->Get_X4_Measured();
		TVector3 locTargetCenter = dComboWrapper->Get_TargetCenter();
		// Double_t locBunchPeriod = dAnalysisUtilities.Get_BeamBunchPeriod(Get_RunNumber());
		// Double_t locDeltaT_RF = dAnalysisUtilities.Get_DeltaT_RF(Get_RunNumber(), locBeamX4_Measured, dComboWrapper);
		//Int_t locRelBeamBucket = dAnalysisUtilities.Get_RelativeBeamBucket(Get_RunNumber(), locBeamX4_Measured, dComboWrapper); // 0 for in-time events, non-zero integer for out-of-time photons
		// Int_t locNumOutOfTimeBunchesInTree = XXX; //YOU need to specify this number
			//Number of out-of-time beam bunches in tree (on a single side, so that total number out-of-time bunches accepted is 2 times this number for left + right bunches) 

		// Bool_t locSkipNearestOutOfTimeBunch = true; // True: skip events from nearest out-of-time bunch on either side (recommended).
		// Int_t locNumOutOfTimeBunchesToUse = locSkipNearestOutOfTimeBunch ? locNumOutOfTimeBunchesInTree-1:locNumOutOfTimeBunchesInTree; 
		// Double_t locAccidentalScalingFactor = dAnalysisUtilities.Get_AccidentalScalingFactor(Get_RunNumber(), locBeamP4.E(), dIsMC); // Ideal value would be 1, but deviations require added factor, which is different for data and MC.
		// Double_t locAccidentalScalingFactorError = dAnalysisUtilities.Get_AccidentalScalingFactorError(Get_RunNumber(), locBeamP4.E()); // Ideal value would be 1, but deviations observed, need added factor.
		// Double_t locHistAccidWeightFactor = locRelBeamBucket==0 ? 1 : -locAccidentalScalingFactor/(2*locNumOutOfTimeBunchesToUse) ; // Weight by 1 for in-time events, ScalingFactor*(1/NBunches) for out-of-time
		// if(locSkipNearestOutOfTimeBunch && abs(locRelBeamBucket)==1) { // Skip nearest out-of-time bunch: tails of in-time distribution also leak in
		// 	dComboWrapper->Set_IsComboCut(true); 
		// 	continue; 
		// } 

		/********************************************* COMBINE FOUR-MOMENTUM ********************************************/

		// DO YOUR STUFF HERE

		// Combine 4-vectors
		TLorentzVector locMissingP4_Measured = locBeamP4_Measured + dTargetP4;
		locMissingP4_Measured -= locPiPlusP4_Measured + locPiMinusP4_Measured + locProtonP4_Measured + locPhoton1P4_Measured + locPhoton2P4_Measured;
		TLorentzVector locVertex = dComboBeamWrapper->Get_X4_Measured();
		TLorentzVector locPiMinusPi0P4_Measured = locPiMinusP4_Measured + locPhoton1P4_Measured + locPhoton2P4_Measured;

		float locRFTime = dComboWrapper->Get_RFTime();
		float DT_RF = locVertex.T() - (locRFTime + (locVertex.Z() - dTargetCenter.Z())/29.9792458);
		//
		TLorentzVector rho_measured = locPhoton1P4_Measured + locPhoton2P4_Measured + locPiMinusP4_Measured;
		TLorentzVector delta_measured = locPiPlusP4_Measured + locProtonP4_Measured;
		float phi_delta_measured = delta_measured.Phi()*TMath::RadToDeg();
		float phi_rho_measured = rho_measured.Phi()*TMath::RadToDeg();
		float diff_phi_measured_finalstate = phi_delta_measured - phi_rho_measured;
		/******************************************** EXECUTE ANALYSIS ACTIONS *******************************************/
		// Loop through the analysis actions, executing them in order for the active particle combo
		dAnalyzeCutActions->Perform_Action(); // Must be executed before Execute_Actions()
		if(!Execute_Actions()) //if the active combo fails a cut, IsComboCutFlag automatically set
			continue;

		//if you manually execute any actions, and it fails a cut, be sure to call:
			//dComboWrapper->Set_IsComboCut(true);

		/**************************************** EXAMPLE: FILL CUSTOM OUTPUT BRANCHES **************************************/

		/*
		TLorentzVector locMyComboP4(8.0, 7.0, 6.0, 5.0);
		//for arrays below: 2nd argument is value, 3rd is array index
		//NOTE: By filling here, AFTER the cuts above, some indices won't be updated (and will be whatever they were from the last event)
			//So, when you draw the branch, be sure to cut on "IsComboCut" to avoid these.
		dTreeInterface->Fill_Fundamental<Float_t>("my_combo_array", -2*loc_i, loc_i);
		dTreeInterface->Fill_TObject<TLorentzVector>("my_p4_array", locMyComboP4, loc_i);
		*/

		/**************************************** EXAMPLE: BEST chi2 METHOD *****************************************/

        // Need to uncomment the section computing combo timing info before running this block of code
		//if(locUsedSoFar_Event == false)
		//{
			// Fill the histogram only when the beam bunch is in-time. 
			//if(!locRelBeamBucket)
			//{
			//	dHist_BeamEnergy_BestChiSq->Fill(locBeamP4.E());
			//	locUsedSoFar_Event = true;
			//}
		//}

		double weight = 1.0;

		if (abs(DT_RF)>2)
		weight = -1 / (8);
		
		
		
		if(dIsMC)
		{dHistThrownTopologies->Fill(locThrownTopology.c_str(),1);
		 weight = MCweight;
		};
		if(isFirst)
	{   dHist_combo_before->Fill(Get_NumCombos(),weight);
		
		isFirst = false;
	}
			
	
		
		/**************************************** EXAMPLE: Chi2 *****************************************/
		double chi2 = dComboWrapper->Get_ChiSq_KinFit("");
		double ndf  = dComboWrapper->Get_NDF_KinFit("");
		double chi2perndf = chi2 / ndf;
		
		/**************************************** EXAMPLE: Omega mass plot*****************************************/
		//locMissingP4_Measured = locPiPlusP4_Measured + locPiMinusP4_Measured + locPhoton1P4_Measured + locPhoton2P4_Measured;
		TLorentzVector locPiPlusPiMinusPi0P4 = locPi0P4 + locPiMinusP4 + locPiPlusP4;
		TLorentzVector locPiMinusPi0P4 = locPi0P4 + locPiMinusP4;
		TLorentzVector locPiPlusPiMinusP4 = locPiMinusP4 + locPiPlusP4;
		TLorentzVector locProtonPi0P4 = locProtonP4 + locPi0P4;
		TLorentzVector locProtonPiMinusP4 = locProtonP4 + locPiMinusP4;
		TLorentzVector locProtonPiPlusP4 = locProtonP4 + locPiPlusP4;
		TLorentzVector locPi0PiPlusP4 = locPi0P4 + locPiPlusP4;

		//Mandelstam t
		TLorentzVector mandelstam_t_P4 = (locBeamP4 - locPiMinusPi0P4);
		double mandelstam_t = mandelstam_t_P4*mandelstam_t_P4;
		
		
		


        TLorentzVector locPiPlusProtonP4 = locPiPlusP4 + locProtonP4;
		

		double locVertexX = locVertex.X();
		if(locUsedSoFar_BeamEnergy.find(locBeamID) == locUsedSoFar_BeamEnergy.end())
		{
			dHist_BeamEnergy->Fill(locBeamP4.E(),weight);
														 // Fills in-time and out-of-time beam photon combos
			//dHist_BeamEnergy->Fill(locBeamP4.E(),locHistAccidWeightFactor); // Alternate version with accidental subtraction

			locUsedSoFar_BeamEnergy.insert(locBeamID);
		}
		/************************************ EXAMPLE: HISTOGRAM z-Vertex (all combos) ************************************/
		

		double momentum_transfer_t = -mandelstam_t;
		
		/************************************ EXAMPLE: HISTOGRAM MISSING MASS SQUARED ************************************/

		//Missing Mass Squared
		double locMissingMassSquared = locMissingP4_Measured.M2();

		//Uniqueness tracking: Build the map of particles used for the missing mass
			//For beam: Don't want to group with final-state photons. Instead use "UnknownParticle" PID (not ideal, but it's easy).
		map<Particle_t, set<Int_t> > locUsedThisCombo_MissingMass;
		locUsedThisCombo_MissingMass[UnknownParticle].insert(locBeamID); //beam
		locUsedThisCombo_MissingMass[PiPlus].insert(locPiPlusTrackID);
		locUsedThisCombo_MissingMass[PiMinus].insert(locPiMinusTrackID);
		locUsedThisCombo_MissingMass[Proton].insert(locProtonTrackID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton1NeutralID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton2NeutralID);
		/**************************************** EXAMPLE: Angles *****************************************/

		double locVertexY = locVertex.Y();
		double R_vertex = sqrt(locVertexX*locVertexX + locVertexY*locVertexY);
		
		/**************************************** EXAMPLE: HISTOGRAM number of showers and charged tracks *****************************************/
		int locNumChargedTracks = dComboWrapper->Get_NumUnusedTracks(); 
		int locNumShowers = dComboWrapper->Get_NumUnusedShowers();
		
		/**************************************** Get angles in labsystem of all particles *****************************************/
		double theta_Proton_LBS = locProtonP4.Theta()*TMath::RadToDeg();
		double theta_Pi0_LBS = locPi0P4.Theta()*TMath::RadToDeg();
		double theta_PiPlus_LBS = locPiPlusP4.Theta()*TMath::RadToDeg();
		double theta_PiMinus_LBS = locPiMinusP4.Theta()*TMath::RadToDeg();
		double theta_Photon1_LBS = locPhoton1P4.Theta()*TMath::RadToDeg();
		double theta_Photon2_LBS = locPhoton2P4.Theta()*TMath::RadToDeg();
		double theta_RhoMinus_LBS = locPiMinusPi0P4.Theta()*TMath::RadToDeg();
		double theta_RhoPlus_LBS = locPi0PiPlusP4.Theta()*TMath::RadToDeg();
		double theta_Rho0_LBS = locPiPlusPiMinusP4.Theta()*TMath::RadToDeg();
		double theta_Omega_LBS = locPiPlusPiMinusPi0P4.Theta()*TMath::RadToDeg();
		double theta_Delta_LBS = locProtonPiPlusP4.Theta()*TMath::RadToDeg();

		double phi_Proton_LBS = locProtonP4.Phi()*TMath::RadToDeg();
		double phi_Pi0_LBS = locPi0P4.Phi()*TMath::RadToDeg();
		double phi_PiPlus_LBS = locPiPlusP4.Phi()*TMath::RadToDeg();
		double phi_PiMinus_LBS = locPiMinusP4.Phi()*TMath::RadToDeg();
		double phi_Photon1_LBS = locPhoton1P4.Phi()*TMath::RadToDeg();
		double phi_Photon2_LBS = locPhoton2P4.Phi()*TMath::RadToDeg();
		double phi_RhoMinus_LBS = locPiMinusPi0P4.Phi()*TMath::RadToDeg();
		double phi_RhoPlus_LBS = locPi0PiPlusP4.Phi()*TMath::RadToDeg();
		double phi_Rho0_LBS = locPiPlusPiMinusP4.Phi()*TMath::RadToDeg();
		double phi_Omega_LBS = locPiPlusPiMinusPi0P4.Phi()*TMath::RadToDeg();
		double phi_Delta_LBS = locProtonPiPlusP4.Phi()*TMath::RadToDeg();



		/**************************************** Shower quality *****************************************/
		float qf1 = dPhoton1Wrapper->Get_Shower_Quality();
		float qf2 = dPhoton2Wrapper->Get_Shower_Quality();
		


		//compare to what's been used so far
		if(locUsedSoFar_MissingMass.find(locUsedThisCombo_MissingMass) == locUsedSoFar_MissingMass.end())
		{
			//unique missing mass combo: histogram it, and register this combo of particles
			dHist_MissingMassSquared->Fill(locMissingMassSquared,weight); // Fills in-time and out-of-time beam photon combos (with weight)
			//dHist_MissingMassSquared->Fill(locMissingMassSquared,locHistAccidWeightFactor); // Alternate version with accidental subtraction

			locUsedSoFar_MissingMass.insert(locUsedThisCombo_MissingMass);
		}
		
		// Plots Impact of cuts on variables 
		std::vector<std::function<bool()>> cutFunctions;
		cutFunctions.push_back([&]() {
			return (locBeamP4.E() > dCut_Ebeam.first && locBeamP4.E() < dCut_Ebeam.second);   //cut1
		});
		
		cutFunctions.push_back([&]() {                         //cut2
			return chi2perndf < dCut_chi2perndf;
		});
		
		cutFunctions.push_back([&]() {
			return (locMissingMassSquared > dCut_MissingMass.first && locMissingMassSquared < dCut_MissingMass.second);  //cut3
		});
		
		cutFunctions.push_back([&]() {
			return (locBeamX4.Z() > dCut_z.first && locBeamX4.Z() < dCut_z.second); //cut4
		});
		
		cutFunctions.push_back([&]() {    //cut5
			return R_vertex < dCut_R;
		});
		
		cutFunctions.push_back([&]() {
			return (qf1 > dCut_qf && qf2 > dCut_qf);  //cut6 
		});
		
		cutFunctions.push_back([&]() {           //cut7
			return locNumShowers <= dCut_shower;
		});

		cutFunctions.push_back([&]() {              //cut8
			return locNumChargedTracks <= dCut_tracks;
		});
		
		cutFunctions.push_back([&]() {
			return (locPi0P4.M() > dCut_Pi0_mass.first && locPi0P4.M() < dCut_Pi0_mass.second);   //cut9
		});
		
		cutFunctions.push_back([&]() {                 //cut10
			return momentum_transfer_t <= dCut_t;
		});

		cutFunctions.push_back([&]() {
			return locPiPlusPiMinusPi0P4.M() >= dCut_Omega_mass;  //cut11
		});

		
		cutFunctions.push_back([&]() {
			return (locPiPlusProtonP4.M() > dCut_Delta.first && locPiPlusProtonP4.M() < dCut_Delta.second);
		});
		
		cutFunctions.push_back([&]() {
			return (locPiMinusPi0P4.M() > dCut_rho_mass.first && locPiMinusPi0P4.M() < dCut_rho_mass.second);
		});
		
		
		
		

		std::vector<float> values = {
			static_cast<float>(locBeamP4.E()),                      // value 0
			static_cast<float>(chi2perndf),                         // value 1
			static_cast<float>(locMissingMassSquared),             
			static_cast<float>(locBeamX4.Z()),                      // value 3
			static_cast<float>(R_vertex),                           
			static_cast<float>(std::min(qf1, qf2)),                 // value 5
			static_cast<float>(locNumShowers),
			static_cast<float>(locNumChargedTracks),                // value 7   
			static_cast<float>(locPi0P4.M()),    
			static_cast<float>(momentum_transfer_t),                // value 9  
			static_cast<float>(locPiPlusPiMinusPi0P4.M()),          
			static_cast<float>(locPiPlusProtonP4.M()),              // value 11
			static_cast<float>(locPiMinusPi0P4.M()),                              
                                                          // letzte cut variable        
			static_cast<float>(locPiPlusPiMinusP4.M()),
			static_cast<float>(locPi0PiPlusP4.M()),
			static_cast<float>(diff_phi_measured_finalstate),
			static_cast<float>(locPiMinusPi0P4_Measured.M())       
		};
		
		for (size_t var = 0; var < values.size(); ++var)
		{
			histMatrix[var][cutFunctions.size()]->Fill(values[var], weight); // Vor allen Cuts
			if (dIsMC){  
				if (histMatrix3D[var][cutFunctions.size()].find(locThrownTopology) != histMatrix3D[var][cutFunctions.size()].end()) {
					histMatrix3D[var][cutFunctions.size()][locThrownTopology.c_str()]->Fill(values[var], MCweight); 
				}
				else {
					histMatrix3D[var][cutFunctions.size()]["other"]->Fill(values[var], MCweight);  //Others
				}
			}
			bool passed = true;
			for (size_t cut = 0; cut < cutFunctions.size(); ++cut)
			{	
				if (cut == var) continue;
				if (!cutFunctions[cut]()) {
					passed = false;
					break;
				}

				if (passed)
				{
					histMatrix[var][cut]->Fill(values[var], weight);
					if (dIsMC){  
						if (histMatrix3D[var][cut].find(locThrownTopology) != histMatrix3D[var][cut].end()) {
							histMatrix3D[var][cut][locThrownTopology.c_str()]->Fill(values[var], MCweight); 
						}
						 else {
							histMatrix3D[var][cut]["other"]->Fill(values[var],MCweight);  //Others
						}
					}
				}

			}
		}
		
			
			 // Vor allen Cuts
			if (dIsMC){  
			Nsurvived[cutFunctions.size()][locThrownTopology.c_str()]++;
			
			bool passed = true;
			for (size_t cut = 0; cut < cutFunctions.size(); ++cut)
			{	
				
				if (cutFunctions[cut]()) {
					Nsurvived[cut][locThrownTopology.c_str()]++;
				}
			}
			}
		


		//t resonance
		std::vector<float> tmass = {     
			static_cast<float>(locPiPlusProtonP4.M()),              // value 11
			static_cast<float>(locPiMinusPi0P4.M()),                              
			static_cast<float>(locPi0PiPlusP4.M()),
			static_cast<float>(locPiPlusPiMinusP4.M()),
			static_cast<float>(locProtonPiMinusP4.M()),
			static_cast<float>(locProtonPi0P4.M()),
			       
		};

		for (size_t var = 0; var < tmass.size(); ++var)
		{	
		bool passed = true;

		for (size_t i = 0; i < cutFunctions.size(); ++i)
		{
			if (i == var+11 && var<13) continue; // überspringe zugehörigen Cut
			if (!cutFunctions[i]()) {
				passed = false;
				break;
			}
			if (passed && i>8) 
					hist2DMatrix[var][i-9]->Fill(momentum_transfer_t,tmass[var], weight);
			if (passed && i==8){ 
			dHist_2D_Mass_3pi_vs_pi0pim->Fill(locPiMinusPi0P4.M(), locPiPlusPiMinusPi0P4.M(), weight);
			dHist_2D_Mass_3pi_vs_ppip->Fill(locPiPlusProtonP4.M(), locPiPlusPiMinusPi0P4.M(), weight);
			dHist_2D_Mass_pi0pim_vs_ppip->Fill(locPiPlusProtonP4.M(), locPiMinusPi0P4.M(), weight);
			dHist_2D_Mass_pippim_vs_ppi0->Fill(locProtonPi0P4.M(), locPiPlusPiMinusP4.M(), weight);
			dHist_2D_Mass_pi0pip_vs_ppim->Fill(locProtonPiMinusP4.M(), locPi0PiPlusP4.M(), weight);
			}
		}

		}
		/**************************************** EXAMPLE: HISTOGRAM BEAM ENERGY *****************************************/

		
			

		bool passed = true;
		for (size_t cut = 0; cut < cutFunctions.size(); ++cut)
		{	
			if (!cutFunctions[cut]()) {
				passed = false;
				break;
			}
		}
		
		if (passed){
			if (dIsMC){
			
		 	for (size_t var = 0; var < cutFunctions.size(); ++var)
			 {
			 	if (histMatrix3D[var][var].find(locThrownTopology) != histMatrix3D[var][var].end()) {
					histMatrix3D[var][var][locThrownTopology.c_str()]->Fill(values[var], MCweight); 
			 	} // Vor allen Cuts
				
			 	else
			 	{
			 		histMatrix3D[var][var]["other"]->Fill(values[var], MCweight);
				}
			 }
			 }
			combos = combos +1;
			
			
			vector<TLorentzVector> locFinalStateP4; // should be in the same order as PID_FinalState
			locFinalStateP4.push_back(locProtonP4);
			locFinalStateP4.push_back(locPiPlusP4);
			locFinalStateP4.push_back(locPiMinusP4); 
			locFinalStateP4.push_back(locPi0P4); 
		

			// momenta
			dFlatTreeInterface->Fill_Fundamental<Float_t>("p_proton", locProtonP4.P());
			dFlatTreeInterface->Fill_Fundamental<Float_t>("p_pim", locPiMinusP4.P());
			dFlatTreeInterface->Fill_Fundamental<Float_t>("p_pip", locPiPlusP4.P());
			dFlatTreeInterface->Fill_Fundamental<Float_t>("p_pi0", locPi0P4.P());

			// invariant masses of combinations
			dFlatTreeInterface->Fill_Fundamental<float>("m_proton_pim", locProtonPiMinusP4.M());
			dFlatTreeInterface->Fill_Fundamental<float>("m_proton_pip", locProtonPiPlusP4.M());
			dFlatTreeInterface->Fill_Fundamental<float>("m_proton_pi0", locProtonPi0P4.M());
			dFlatTreeInterface->Fill_Fundamental<float>("m_pi0_pip", locPi0PiPlusP4.M());
			dFlatTreeInterface->Fill_Fundamental<float>("m_pi0_pim", locPiMinusPi0P4.M());
			dFlatTreeInterface->Fill_Fundamental<float>("m_pip_pim", locPiPlusPiMinusP4.M());

			// additional information
			dFlatTreeInterface->Fill_Fundamental<float>("mandel_t", momentum_transfer_t);
			dFlatTreeInterface->Fill_Fundamental<float>("Weight_user", weight);
			dFlatTreeInterface->Fill_Fundamental<float>("chi2ndf", chi2perndf);
			dFlatTreeInterface->Fill_Fundamental<float>("missingMassSquared", locMissingMassSquared);
			dFlatTreeInterface->Fill_Fundamental<float>("unusedShowers", locNumShowers);
			dFlatTreeInterface->Fill_Fundamental<float>("unusedTracks", locNumChargedTracks);
			dFlatTreeInterface->Fill_Fundamental<float>("showerQualityPhoton1", qf1);
			dFlatTreeInterface->Fill_Fundamental<float>("showerQualityPhoton2", qf2);
			dFlatTreeInterface->Fill_Fundamental<float>("polstate", polarisationState);
			// RECOMMENDED: FILL ACCIDENTAL WEIGHT
			// dFlatTreeInterface->Fill_Fundamental<Double_t>("accidweight",locHistAccidWeightFactor);
			
			/*
			//FILL ANY CUSTOM BRANCHES FIRST!!
			Int_t locMyInt_Flat = 7;
			dFlatTreeInterface->Fill_Fundamental<Int_t>("flat_my_int", locMyInt_Flat);

			TLorentzVector locMyP4_Flat(4.0, 3.0, 2.0, 1.0);
			dFlatTreeInterface->Fill_TObject<TLorentzVector>("flat_my_p4", locMyP4_Flat);

			for(int loc_j = 0; loc_j < locMyInt_Flat; ++loc_j)
			{
				dFlatTreeInterface->Fill_Fundamental<Int_t>("flat_my_int_array", 3*loc_j, loc_j); //2nd argument = value, 3rd = array index
				TLorentzVector locMyComboP4_Flat(8.0, 7.0, 6.0, 5.0);
				dFlatTreeInterface->Fill_TObject<TLorentzVector>("flat_my_p4_array", locMyComboP4_Flat, loc_j);
			}
			*/

			//FILL FLAT TREE
			FillAmpTools_FlatTree(locBeamP4, locFinalStateP4);
			Fill_FlatTree(); //for the active combo
			
					

			dHist_Proton_Angle->Fill(phi_Proton_LBS, theta_Proton_LBS, weight);
			dHist_Pi0_Angle->Fill(phi_Pi0_LBS, theta_Pi0_LBS, weight);
			dHist_PiPlus_Angle->Fill(phi_PiPlus_LBS, theta_PiPlus_LBS, weight);
			dHist_PiMinus_Angle->Fill(phi_PiMinus_LBS, theta_PiMinus_LBS, weight);
			dHist_Photon1_Angle->Fill(phi_Photon1_LBS, theta_Photon1_LBS, weight);
			dHist_Photon2_Angle->Fill(phi_Photon2_LBS, theta_Photon2_LBS, weight);
			dHist_RhoMinus_Angle->Fill(phi_RhoMinus_LBS, theta_RhoMinus_LBS, weight);
			dHist_RhoPlus_Angle->Fill(phi_RhoPlus_LBS, theta_RhoPlus_LBS, weight);
			dHist_Rho0_Angle->Fill(phi_Rho0_LBS, theta_Rho0_LBS, weight);
			dHist_Omega_Angle->Fill(phi_Omega_LBS, theta_Omega_LBS, weight);
			dHist_Delta_Angle->Fill(phi_Delta_LBS, theta_Delta_LBS, weight);
			
			
			
			
		}





		


		/****************************************** FILL FLAT TREE (IF DESIRED) ******************************************/
		
		/****************************************** FILL FLAT TREE (IF DESIRED) ******************************************/
		
		

		// Final state vector
	dHist_combo_after->Fill(combos,weight);
	} // end of combo loop

	//FILL HISTOGRAMS: Num combos / events surviving actions
	Fill_NumCombosSurvivedHists();

	/******************************************* LOOP OVER THROWN DATA (OPTIONAL) ***************************************/
/*
	//Thrown beam: just use directly
	if(dThrownBeam != NULL)
		double locEnergy = dThrownBeam->Get_P4().E();

	//Loop over throwns
	for(UInt_t loc_i = 0; loc_i < Get_NumThrown(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dThrownWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}
*/
	/****************************************** LOOP OVER OTHER ARRAYS (OPTIONAL) ***************************************/
/*
	//Loop over beam particles (note, only those appearing in combos are present)
	for(UInt_t loc_i = 0; loc_i < Get_NumBeam(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dBeamWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}

	//Loop over charged track hypotheses
	for(UInt_t loc_i = 0; loc_i < Get_NumChargedHypos(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dChargedHypoWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}

	//Loop over neutral particle hypotheses
	for(UInt_t loc_i = 0; loc_i < Get_NumNeutralHypos(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dNeutralHypoWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}
*/

	/************************************ EXAMPLE: FILL CLONE OF TTREE HERE WITH CUTS APPLIED ************************************/
/*
	Bool_t locIsEventCut = true;
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i) {
		//Set branch array indices for combo and all combo particles
		dComboWrapper->Set_ComboIndex(loc_i);
		// Is used to indicate when combos have been cut
		if(dComboWrapper->Get_IsComboCut())
			continue;
		locIsEventCut = false; // At least one combo succeeded
		break;
	}
	if(!locIsEventCut && dOutputTreeFileName != "")
		Fill_OutputTree();
*/

	return kTRUE;
}


void DSelector_pi0pippm_ver2::Finalize(void)
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
