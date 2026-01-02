#include "DSelector_RhomDeltapp.h"
#include <cmath>
#include "particleType.h"
void DSelector_RhomDeltapp::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "RhomDeltapp.root"; //"" for none
	dOutputTreeFileName = ""; //"" for none
	dFlatTreeFileName = "flattree_RhomDeltapp.root"; //output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = "kin"; //if blank, default name will be chosen
	dSaveDefaultFlatBranches = false; // False: don't save default branches, reduce disk footprint.
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

	//Creating new branchses for flat tree
	SetupAmpTools_FlatTree(); //sets most branches needed for AmpTools
	// dFlatTreeInterface->Create_Branch_Fundamental<float>("Target_Mass"); 
 	// dFlatTreeInterface->Create_Branch_FundamentalArray<int>("PID_FinalState","NumFinalState");
 	// dFlatTreeInterface->Create_Branch_Fundamental<int>("pol_angle");
 	// dFlatTreeInterface->Create_Branch_Fundamental<float>("mandel_t"); 
	// dFlatTreeInterface->Create_Branch_Fundamental<float>("m_pippim"); 
	// dFlatTreeInterface->Create_Branch_Fundamental<float>("m_pipp"); 
	// dFlatTreeInterface->Create_Branch_Fundamental<float>("m_pimp");
	// dFlatTreeInterface->Create_Branch_Fundamental<float>("chi2ndf");
	// dFlatTreeInterface->Create_Branch_Fundamental<float>("cosTpipGJ");
	// dFlatTreeInterface->Create_Branch_Fundamental<float>("phipipGJ");
	// dFlatTreeInterface->Create_Branch_Fundamental<float>("phi_lab_PiMinus");
	// dFlatTreeInterface->Create_Branch_Fundamental<float>("phi_lab_PiPlus");
	// dFlatTreeInterface->Create_Branch_Fundamental<float>("phi_lab_Proton");
	// dFlatTreeInterface->Create_Branch_Fundamental<float>("theta_lab_PiMinus");
	// dFlatTreeInterface->Create_Branch_Fundamental<float>("theta_lab_PiPlus");
	// dFlatTreeInterface->Create_Branch_Fundamental<float>("theta_lab_Proton");
	// dFlatTreeInterface->Create_Branch_Fundamental<float>("momentum_pip");
	// dFlatTreeInterface->Create_Branch_Fundamental<float>("momentum_pim");
	// dFlatTreeInterface->Create_Branch_Fundamental<float>("momentum_p");
	// dFlatTreeInterface->Create_Branch_Fundamental<float>("mandel_t_beampiplus");

	// momenta
	dFlatTreeInterface->Create_Branch_Fundamental<float>("p_proton");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("p_pim");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("p_pip");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("p_pi0");

	// theta angles
	dFlatTreeInterface->Create_Branch_Fundamental<float>("theta_proton");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("theta_pim");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("theta_pip");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("theta_pi0");
	
	// phi angles
	dFlatTreeInterface->Create_Branch_Fundamental<float>("phi_proton");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("phi_pim");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("phi_pip");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("phi_pi0");

	// invariant masses of combinations
	dFlatTreeInterface->Create_Branch_Fundamental<float>("m_proton_pim");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("m_proton_pip");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("m_proton_pi0");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("m_pi0_pip");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("m_pi0_pim");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("m_pip_pim");

	// additional information
	dFlatTreeInterface->Create_Branch_Fundamental<float>("mandel_t");
	//dFlatTreeInterface->Create_Branch_Fundamental<float>("Weight");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("chi2ndf");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("missingMassSquared");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("unusedShowers");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("unusedTracks");
	dFlatTreeInterface->Create_Branch_Fundamental<float>("showerQuality");


	/*********************************** EXAMPLE USER INITIALIZATION: ANALYSIS ACTIONS **********************************/

	// EXAMPLE: Create deque for histogramming particle masses:
	// // For histogramming the phi mass in phi -> K+ K-
	// // Be sure to change this and dAnalyzeCutActions to match reaction
	std::deque<Particle_t> MyPhi;
	MyPhi.push_back(KPlus); MyPhi.push_back(KMinus);

	//ANALYSIS ACTIONS: //Executed in order if added to dAnalysisActions
	//false/true below: use measured/kinfit data

	//PID
	dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, false));
	//below: value: +/- N ns, Unknown: All PIDs, SYS_NULL: all timing systems
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
	//dAnalysisActions.push_back(new DCutAction_BeamEnergy(dComboWrapper, false, 8.2, 8.8));  // Coherent peak for runs in the range 30000-59999

	//KINEMATICS
	dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, false));

	// INVARIANT MASSES
	// dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, 0, {Proton, Pi0}, 500, 0.0, 5.0, "ProtonPi0"));
	// dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, 0, {Proton, PiPlus}, 500, 0.0, 5.0, "ProtonPiPlus"));
	// dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, 0, {Proton, PiMinus}, 500, 0.0, 5.0, "ProtonPiMinus"));
	// dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, 0, {Pi0, PiPlus}, 500, 0.0, 5.0, "Pi0PiPlus"));
	// dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, 0, {Pi0, PiMinus}, 500, 0.0, 5.0, "Pi0PiMinus"));
	// dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, 0, {PiPlus, PiMinus}, 500, 0.0, 5.0, "PiPlusPiMinus"));

	// ANALYZE CUT ACTIONS
	// // Change MyPhi to match reaction
	dAnalyzeCutActions = new DHistogramAction_AnalyzeCutActions( dAnalysisActions, dComboWrapper, false, 0, MyPhi, 1000, 0.9, 2.4, "CutActionEffect" );

	//INITIALIZE ACTIONS
	//If you create any actions that you want to run manually (i.e. don't add to dAnalysisActions), be sure to initialize them here as well
	Initialize_Actions();
	dAnalyzeCutActions->Initialize(); // manual action, must call Initialize()

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/

	// basic information
	dHist_Chi2NDF = new TH1F("Chi2NDF", ";#chi^{2}/ndf", 500, 0, 50);
	dHist_MissingMassSquared = new TH1F("MissingMassSquared", ";Missing Mass Squared (GeV/c^{2})^{2}", 600, -0.06, 0.06);
	dHist_MissingMassSquaredWoWeight = new TH1F("MissingMassSquaredWoWeight", ";Missing Mass Squared (GeV/c^{2})^{2}", 600, -0.06, 0.06);
	dHist_BeamEnergy = new TH1I("BeamEnergy", ";Beam Energy (GeV)", 600, 0.0, 12.0);
	dHist_TimeCut = new TH1I("TimeCut", "Time cut for Handling Accidental Beam Photon Contributions;#DeltaT_{RF} (ns)", 500, -25.0, 25.0);
	dHist_ZVertex = new TH1F("dHist_ZVertex", "Z Vertex of particles after cut; Z vertex (cm)", 200, 0, 100);
	dHist_XYVertex = new TH1F("dHist_XYVertex", "XY Vertex of particles after cut: XY vertex (cm)", 100, -10, 10);
	dHist_unusedShowers = new TH1F("unusedShowers", "Unused showers;# unused showers", 11, -0.5, 10.5);
	dHist2D_unusedShowers_Pi0PiMinus = new TH2F("unusedShowers_Pi0PiMinus", "#pi^{0} #pi^{-} vs. Unused showers ;# unused showers; m_{#pi^{0} #pi^{-}} (GeV/c^{2})", 11, -0.5, 10.5, 500, 0.0, 5.0);
	dHist_unusedTracks = new TH1F("unusedTracks", "Unused tracks;# unused tracks", 11, -0.5, 10.5);
	dHist2D_unusedTracks_Pi0PiMinus = new TH2F("unusedTracks_Pi0PiMinus", "#pi^{0} #pi^{-} vs. Unused tracks ;# unused tracks; m_{#pi^{0} #pi^{-}} (GeV/c^{2})", 11, -0.5, 10.5, 500, 0.0, 5.0);
	dHist_showerQuality = new TH1F("showerQuality", "Shower Quality;Shower Quality", 100, 0, 1);
	dHist2D_showerQuality_Pi0PiMinus = new TH2F("showerQuality_Pi0PiMinus", "#pi^{0} #pi^{-} vs. Shower Quality;Shower Quality; m_{#pi^{0} #pi^{-}} (GeV/c^{2})", 100, 0, 1, 500, 0.0, 5.0);


	// pi0
	dHist_InvMass_GammaGammaMeas = new TH1F("InvMass_GammaGammaMeas", "Invariant mass of measured #gamma #gamma; m_{#gamma #gamma} (GeV/c^{2})", 500, 0.0, 0.5);

	// 1-baryon 1-meson combinations
	dHist_InvMass_ProtonPi0 = new TH1F("InvMass_ProtonPi0", "Invariant mass of p #pi^{0};m_{p #pi^{0}} (GeV/c^{2})", 500, 0.0, 5.0);
	dHist_InvMass_ProtonPiPlus = new TH1F("InvMass_ProtonPiPlus", "Invariant mass of p #pi^{+};m_{p #pi^{+}} (GeV/c^{2})", 500, 0.0, 5.0);
	dHist_InvMass_ProtonPiMinus = new TH1F("InvMass_ProtonPiMinus", "Invariant mass of p #pi^{-};m_{p #pi^{-}} (GeV/c^{2})", 500, 0.0, 5.0);

	// 2-meson combinations
	dHist_InvMass_Pi0PiPlus = new TH1F("InvMass_Pi0PiPlus", "Invariant mass of #pi^{0} #pi^{+};m_{#pi^{0} #pi^{+}} (GeV/c^{2})", 500, 0.0, 5.0);
	dHist_InvMass_Pi0PiMinus = new TH1F("InvMass_Pi0PiMinus", "Invariant mass of #pi^{0} #pi^{-};m_{#pi^{0} #pi^{-}} (GeV/c^{2})", 500, 0.0, 5.0);
	dHist_InvMass_PiPlusPiMinus = new TH1F("InvMass_PiPlusPiMinus", "Invariant mass of #pi^{+} #pi^{-};m_{#pi^{+} #pi^{-}} (GeV/c^{2})", 500, 0.0, 5.0);

	// 3-meson combination after different cuts
	dHist_InvMass_Pi0PiPlusPiMinus = new TH1F("InvMass_Pi0PiPlusPiMinus", "Invariant mass of #pi^{0} #pi^{+} #pi^{-};m_{#pi^{0} #pi^{+} #pi^{-}} (GeV/c^{2})", 500, 0.0, 5.0);
	//dHist_InvMass_Pi0PiPlusPiMinus_1 = new TH1F("InvMass_Pi0PiPlusPiMinus_1", "Invariant mass of #pi^{0} #pi^{+} #pi^{-} after Missing mass, #chi^{2}/ndf cut;m_{#pi^{0} #pi^{+} #pi^{-}} (GeV/c^{2})", 500, 0.0, 5.0);


	// Delta++ vs different variables
	dHist_InvMass2D_ProtonPiPlus_vs_BeamEnergy = new TH2F("InvMass2D_ProtonPiPlus_vs_BeamEnergy", "Beam Energy vs. invariant mass of p #pi^{+};m_{p #pi^{+}} (GeV/c^{2});Beam Energy (GeV)", 500, 0.0, 5.0, 120, 0.0, 12.0);
	dHist_InvMass2D_ProtonPiPlus_vs_ProtonPiMinus = new TH2F("InvMass2D_ProtonPiPlus_vs_ProtonPiMinus", "p #pi^{-} vs. p #pi^{+} ;m_{p #pi^{+}} (GeV/c^{2});m_{p #pi^{-}} (GeV/c^{2})", 500, 0.0, 5.0, 500, 0.0, 5.0);
	dHist_InvMass2D_ProtonPiPlus_vs_deltaTheta = new TH2F("InvMass2D_ProtonPiPlus_vs_deltaTheta", "Invariant mass of #theta_{p} - #theta_{#pi^{+}} vs. p #pi^{+};m_{p #pi^{+}} (GeV/c^{2});#Delta#theta_{p #pi^{+}} (deg)", 500, 0.0, 5.0, 720, -360, 360);
	//dHist_InvMass2D_ProtonPiPlus_vs_Pi0PiPlusPiMinus = new TH2F("InvMass2D_ProtonPiPlus_vs_Pi0PiPlusPiMinus", "p #pi^{+} vs. #pi^{0} #pi^{+} #pi^{-};m_{p #pi^{+}} (GeV/c^{2});m_{#pi^{0} #pi^{+} #pi^{-}} (GeV/c^{2})", 500, 0.0, 5.0, 500, 0.0, 5.0);
	
	// rho- vs different variables
	dHist_InvMass2D_Pi0PiMinus_vs_deltaTheta = new TH2F("InvMass2D_Pi0PiMinus_vs_deltaTheta", "Invariant mass of #Delta#theta_{#pi^{0} #pi^{-}} vs. #pi^{0} #pi^{-};m_{#pi^{0} #pi^{-}} (GeV/c^{2});#Delta#theta_{#pi^{0} #pi^{-}} (deg)", 500, 0.0, 5.0, 720, -360, 360);

	// Delta++ vs Rho- after different cuts
	dHist_InvMass2D_ProtonPiPlus_Pi0PiMinus = new TH2F("InvMass2D_ProtonPiPlus_Pi0PiMinus", " #pi^{0} #pi^{-} vs. p #pi^{+};m_{p #pi^{+}} (GeV/c^{2});m_{#pi^{0} #pi^{-}} (GeV/c^{2})", 500, 0.0, 5.0, 500, 0.0, 5.0);
	//dHist_InvMass2D_ProtonPiPlus_Pi0PiMinus_1 = new TH2F("InvMass2D_ProtonPiPlus_Pi0PiMinus_1", "p #pi^{+} vs. #pi^{0} #pi^{-} after Missing mass, #chi^{2}/ndf cut;m_{p #pi^{+}} (GeV/c^{2});m_{#pi^{0} #pi^{-}} (GeV/c^{2})", 500, 0.0, 5.0, 500, 0.0, 5.0);

	// Different E-Beam bins
	dHist_InvMass2D_ProtonPiPlus_Pi0PiMinus_Ebeam0 = new TH2F("InvMass2D_ProtonPiPlus_Pi0PiMinus_Ebeam0", "#pi^{0} #pi^{-} vs. p #pi^{+} for 3 Gev < E_{beam} < 5 GeV;m_{p #pi^{+}} (GeV/c^{2});m_{#pi^{0} #pi^{-}} (GeV/c^{2})", 500, 0.0, 5.0, 500, 0.0, 5.0);
	dHist_InvMass2D_ProtonPiPlus_Pi0PiMinus_Ebeam1 = new TH2F("InvMass2D_ProtonPiPlus_Pi0PiMinus_Ebeam1", "#pi^{0} #pi^{-} vs. p #pi^{+} for 5 Gev < E_{beam} < 8 GeV;m_{p #pi^{+}} (GeV/c^{2});m_{#pi^{0} #pi^{-}} (GeV/c^{2})", 500, 0.0, 5.0, 500, 0.0, 5.0);
	dHist_InvMass2D_ProtonPiPlus_Pi0PiMinus_Ebeam2 = new TH2F("InvMass2D_ProtonPiPlus_Pi0PiMinus_Ebeam2", "#pi^{0} #pi^{-} vs. p #pi^{+} for 8.2 Gev < E_{beam} < 8.8 GeV;m_{p #pi^{+}} (GeV/c^{2});m_{#pi^{0} #pi^{-}} (GeV/c^{2})", 500, 0.0, 5.0, 500, 0.0, 5.0);


	// Dalitz plots
	dHist_Dalitz_ProtonPiPlusRhoMinus = new TH2F("Dalitz_ProtonPiPlusRhoMinus", "Dalitz plot of p #pi^{+} #rho^{-};m_{p #pi^{+}}^{2} (GeV/c^{2})^{2};m_{#pi^{+} #rho^{-}}^{2} (GeV/c^{2})^{2}", 1500, 0, 15, 1500, 0 , 15);
	dHist_Dalitz_ProtonPiMinusRhoPlus = new TH2F("Dalitz_ProtonPiMinusRhoPlus", "Dalitz plot of p #pi^{-} #rho^{+};m_{p #pi^{-}}^{2} (GeV/c^{2})^{2};m_{#pi^{-} #rho^{+}}^{2} (GeV/c^{2})^{2}", 1500, 0, 15, 1500, 0 , 15);
	dHist_Dalitz_ProtonPi0Rho0 = new TH2F("Dalitz_ProtonPi0Rho0", "Dalitz plot of p #pi^{0} #rho^{0};m_{p #pi^{0}}^{2} (GeV/c^{2})^{2};m_{#pi^{0} #rho^{0}}^{2} (GeV/c^{2})^{2}", 1500, 0, 15, 1500, 0 , 15);
	dHist_Dalitz_ProtronRho0Pi0 = new TH2F("Dalitz_ProtronRho0Pi0", "Dalitz plot of p #rho^{0} #pi^{0};m_{p #rho^{0}}^{2} (GeV/c^{2})^{2};m_{#rho^{0} #pi^{0}}^{2} (GeV/c^{2})^{2}", 1500, 0, 15, 1500, 0 , 15);
	dHist_Dalitz_ProtonRhoMinusPiPlus = new TH2F("Dalitz_ProtonRhoMinusPiPlus", "Dalitz plot of p #rho^{-} #pi^{+};m_{p #rho^{-}}^{2} (GeV/c^{2})^{2};m_{#rho^{-} #pi^{+}}^{2} (GeV/c^{2})^{2}", 1500, 0, 15, 1500, 0 , 15);
	dHist_Dalitz_ProtonRhoPlusPiMinus = new TH2F("Dalitz_ProtonRhoPlusPiMinus", "Dalitz plot of p #rho^{+} #pi^{-};m_{p #rho^{+}}^{2} (GeV/c^{2})^{2};m_{#rho^{+} #pi^{-}}^{2} (GeV/c^{2})^{2}", 1500, 0, 15, 1500, 0 , 15);


	// t channel histos
	dHist2D_tchannel_Pi0PiMinus = new TH2F("tchannel_Pi0PiMinus", "#pi^{0} #pi^{-} vs. t channel;#sqrt{-t} (GeV);m_{#pi^{0} #pi^{-}} (GeV/c^{2})", 500, 0, 5, 500, 0 , 5);
	dHist2D_tchannel_Pi0PiPlus = new TH2F("tchannel_Pi0PiPlus", "#pi^{0} #pi^{+} vs. t channel;#sqrt{-t} (GeV);m_{#pi^{0} #pi^{+}} (GeV/c^{2})", 500, 0, 5, 500, 0 , 5);
	dHist2D_tchannel_PiPlusPiMinus = new TH2F("tchannel_PiPlusPiMinus", "#pi^{+} #pi^{-} vs. t channel;#sqrt{-t} (GeV);m_{#pi^{+} #pi^{-}} (GeV/c^{2})", 500, 0, 5, 500, 0 , 5);
	dHist2D_tchannel_ProtonPiPlus = new TH2F("tchannel_ProtonPiPlus", "p #pi^{+} vs. t channel;#sqrt{-t} (GeV);m_{p #pi^{+}} (GeV/c^{2})", 500, 0, 5, 500, 0 , 5);
	dHist2D_tchannel_ProtonPiMinus = new TH2F("tchannel_ProtonPiMinus", "p #pi^{-} vs. t channel;#sqrt{-t} (GeV);m_{p #pi^{-}} (GeV/c^{2})", 500, 0, 5, 500, 0 , 5);
	dHist2D_tchannel_ProtonPi0 = new TH2F("tchannel_ProtonPi0", "p #pi^{0} vs. t channel;#sqrt{-t} (GeV);m_{p #pi^{0}} (GeV/c^{2})", 500, 0, 5, 500, 0 , 5);


	// angles
	dHist_CM_theta_Rhom = new TH1F("dHist_CM_theta_Rhom", "Measured Theta angle of #rho^{-} in CM-frame;#theta_{#rho^{-}} (deg)", 180, 0, 180);
	dHist_CM_theta_Deltapp = new TH1F("dHist_CM_theta_Deltapp", "Measured Theta angle of #Delta^{++} in CM-frame;#theta_{#Delta^{++}} (deg)", 180, 0, 180);
	dHist_CM_deltaTheta = new TH1F("deltaTheta_CM", "Measured Delta theta of #rho^{-} and #Delta^{++};#Delta#theta_{#rho^{-} #Delta^{++}} (deg)", 400, 160, 200);
	dHist_CM_phi_Rhom = new TH1F("dHist_CM_phi_Rhom", "Measured Phi angle of #rho^{-} in CM-frame;#phi_{#rho^{-}} (deg)", 360, -180, 180);
	dHist_CM_phi_Deltapp = new TH1F("dHist_CM_phi_Deltapp", "Measured Phi angle of #Delta^{++} in CM-frame;#phi_{#Delta^{++}} (deg)", 360, -180, 180);
	dHist_CM_deltaPhi = new TH1F("deltaPhi_CM", "Measured Delta phi of #rho^{-} and #Delta^{++};#Delta#phi_{#rho^{-} #Delta^{++}} (deg)", 400, 160, 200);


	// final histograms, after all cuts (AAC)
	dHist_InvMass_ProtonPi0_AAC = new TH1F("InvMass_ProtonPi0_AAC", "Invariant mass of p #pi^{0}: #Delta^{++} cut, #rho^{-} cut, #omega anticut;m_{p #pi^{0}} (GeV/c^{2})", 500, 0.0, 5.0);
	dHist_InvMass_ProtonPiPlus_AAC = new TH1F("InvMass_ProtonPiPlus_AAC", "Invariant mass of p #pi^{+}: #rho^{-} cut, #omega anticut;m_{p #pi^{+}} (GeV/c^{2})", 500, 0.0, 5.0);
	dHist_InvMass_ProtonPiMinus_AAC = new TH1F("InvMass_ProtonPiMinus_AAC", "Invariant mass of p #pi^{-}: #Delta^{++} cut, #rho^{-} cut, #omega anticut;m_{p #pi^{-}} (GeV/c^{2})", 500, 0.0, 5.0);
	dHist_InvMass_Pi0PiPlus_AAC = new TH1F("InvMass_Pi0PiPlus_AAC", "Invariant mass of #pi^{0} #pi^{+}: #Delta^{++} cut, #rho^{-} cut, #omega anticut;m_{#pi^{0} #pi^{+}} (GeV/c^{2})", 500, 0.0, 5.0);
	dHist_InvMass_Pi0PiMinus_AAC = new TH1F("InvMass_Pi0PiMinus_AAC", "Invariant mass of #pi^{0} #pi^{-}: #Delta^{++} cut, #omega anticut;m_{#pi^{0} #pi^{-}} (GeV/c^{2})", 500, 0.0, 5.0);
	dHist_InvMass_PiPlusPiMinus_AAC = new TH1F("InvMass_PiPlusPiMinus_AAC", "Invariant mass of #pi^{+} #pi^{-}: #Delta^{++} cut, #rho^{-} cut, #omega anticut;m_{#pi^{+} #pi^{-}} (GeV/c^{2})", 500, 0.0, 5.0);

	dHist_InvMass_Pi0PiPlusPiMinus_AAC = new TH1F("InvMass_Pi0PiPlusPiMinus_AAC", "Invariant mass of #pi^{0} #pi^{+} #pi^{-}: #Delta^{++} cut, #rho^{-} cut;m_{#pi^{0} #pi^{+} #pi^{-}} (GeV/c^{2})", 500, 0.0, 5.0);

	

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



	// Cut Limits, these are all very broad cuts, they will be refined later
	dChi2Cut = 8.0; // 5 -> 8
	dMissingMassSquaredCut = 0.01; // GeV^2  0.005 -> 0.01
	dUnusedShowersCut = 5; // 2 -> 5
	dUnusedTracksCut = 5; // 1 -> 5
	dShowerQualityCut = 0; // 0.5 -> 0


	dZVertexCutLow = 0.; //cm 51.0 -> 0
	dZVertexCutHigh = 100.; //cm 78.0 -> 100
	dXYVertexCut = 5.; //cm 1.0 -> 5.0

}

Bool_t DSelector_RhomDeltapp::Process(Long64_t locEntry)
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
	if(locRunNumber != dPreviousRunNumber)
	{
		dIsPolarizedFlag = dAnalysisUtilities.Get_IsPolarizedBeam(locRunNumber, dIsPARAFlag);
		dPreviousRunNumber = locRunNumber;
	}

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

	//EXAMPLE 1: Particle-specific info:
	set<Int_t> locUsedSoFar_BeamEnergy; //Int_t: Unique ID for beam particles. set: easy to use, fast to search

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

	//Loop over combos
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i)
	{
		//Set branch array indices for combo and all combo particles
		dComboWrapper->Set_ComboIndex(loc_i);

		// Is used to indicate when combos have been cut
		if (dComboWrapper->Get_IsComboCut()) // Is false when tree originally created
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
		TLorentzVector locDecayingPi0P4 = dDecayingPi0Wrapper->Get_P4();
		TLorentzVector locPhoton1P4 = dPhoton1Wrapper->Get_P4();
		TLorentzVector locPhoton2P4 = dPhoton2Wrapper->Get_P4();

		// Get Measured P4's:
		//Step 0
		TLorentzVector locBeamP4_Measured = dComboBeamWrapper->Get_P4_Measured();
		TLorentzVector locPiPlusP4_Measured = dPiPlusWrapper->Get_P4_Measured();
		TLorentzVector locPiMinusP4_Measured = dPiMinusWrapper->Get_P4_Measured();
		TLorentzVector locProtonP4_Measured = dProtonWrapper->Get_P4_Measured();
		//Step 1
		TLorentzVector locPhoton1P4_Measured = dPhoton1Wrapper->Get_P4_Measured();
		TLorentzVector locPhoton2P4_Measured = dPhoton2Wrapper->Get_P4_Measured();

		TLorentzVector locBeamX4_Measured = dComboBeamWrapper->Get_X4_Measured();
		TLorentzVector locPiMinusX4_Measured = dPiMinusWrapper->Get_X4_Measured();
		TLorentzVector locPiPlusX4_Measured = dPiPlusWrapper->Get_X4_Measured();
		TLorentzVector locProtonX4_Measured = dProtonWrapper->Get_X4_Measured();


		double locProtonXVertex = locProtonX4_Measured.X();
		double locPiPlusXVertex = locPiPlusX4_Measured.X();
		double locPiMinusXVertex = locPiMinusX4_Measured.X();

		double locProtonYVertex = locProtonX4_Measured.Y();
		double locPiPlusYVertex = locPiPlusX4_Measured.Y();
		double locPiMinusYVertex = locPiMinusX4_Measured.Y();

		double locProtonZVertex = locProtonX4_Measured.Z();
		double locPiPlusZVertex = locPiPlusX4_Measured.Z();
		double locPiMinusZVertex = locPiMinusX4_Measured.Z();

		double locRVertexProton = TMath::Sqrt(TMath::Power(locProtonXVertex,2) + TMath::Power(locProtonYVertex,2));
		double locRVertexPiPlus = TMath::Sqrt(TMath::Power(locPiPlusXVertex,2) + TMath::Power(locPiPlusYVertex,2));
		double locRVertexPiMinus = TMath::Sqrt(TMath::Power(locPiMinusXVertex,2) + TMath::Power(locPiMinusYVertex,2));

		/********************************************* GET COMBO RF TIMING INFO *****************************************/

		// Double_t locBunchPeriod = dAnalysisUtilities.Get_BeamBunchPeriod(Get_RunNumber());
		// Double_t locDeltaT_RF = dAnalysisUtilities.Get_DeltaT_RF(Get_RunNumber(), locBeamX4_Measured, dComboWrapper);
		// Int_t locRelBeamBucket = dAnalysisUtilities.Get_RelativeBeamBucket(Get_RunNumber(), locBeamX4_Measured, dComboWrapper); // 0 for in-time events, non-zero integer for out-of-time photons
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

		TLorentzVector locVertex = dComboBeamWrapper->Get_X4_Measured();
		float locRFTime = dComboWrapper->Get_RFTime();
		float DT_RF = locVertex.T() - (locRFTime + (locVertex.Z() - dTargetCenter.Z())/29.9792458);
		
		// Now an event weight can be assigned:
		// if DT_RF = +/- 2ns within zero the beam photon is in time
		// within +-4, +-8, +-12, ... the beam photon is out of time

		double weight = 1.;
		if (abs(DT_RF) > 2.0) { // TOFIX: should actually pull the real beam bunch spacing for this
			weight = -0.125; // -1/8
			//weight *= dAnalysisUtilities.Get_AccidentalScalingFactor(); // correct for correlation, doc: An additional run-dependent multiplicitive factor should be applied, to correct for non-uniformities in the out-of-time peaks, which can be acccessed through DAnalysisUtilities::Get AccidentalScalingFactor().
		}

		dHist_TimeCut->Fill(DT_RF);


		dHist_ZVertex->Fill(locProtonZVertex, weight);
		dHist_ZVertex->Fill(locPiPlusZVertex, weight);
		dHist_ZVertex->Fill(locPiMinusZVertex, weight);

		dHist_XYVertex->Fill(locRVertexProton, weight);
		dHist_XYVertex->Fill(locRVertexPiPlus, weight);
		dHist_XYVertex->Fill(locRVertexPiMinus, weight);

		// Vertex Cut
		if (locProtonZVertex < dZVertexCutLow || locProtonZVertex > dZVertexCutHigh) continue;
		if (locPiPlusZVertex < dZVertexCutLow || locPiPlusZVertex > dZVertexCutHigh) continue;
		if (locPiMinusZVertex < dZVertexCutLow || locPiMinusZVertex > dZVertexCutHigh) continue;
		if (locRVertexProton > dXYVertexCut || locRVertexPiPlus > dXYVertexCut || locRVertexPiMinus > dXYVertexCut) continue;







		/********************************************* COMBINE FOUR-MOMENTUM ********************************************/

		// DO YOUR STUFF HERE

		// Combine 4-vectors
		TLorentzVector locMissingP4_Measured = locBeamP4_Measured + dTargetP4;
		locMissingP4_Measured -= locPiPlusP4_Measured + locPiMinusP4_Measured + locProtonP4_Measured + locPhoton1P4_Measured + locPhoton2P4_Measured;

		// Measured 4-vectors
		TLorentzVector locPi0P4_Measured = locPhoton1P4_Measured + locPhoton2P4_Measured;

		// TLorentzVector locProtonPi0P4_Measured = locProtonP4_Measured + locPi0P4_Measured;
		// TLorentzVector locProtonPiPlusP4_Measured = locProtonP4_Measured + locPiPlusP4_Measured;
		// TLorentzVector locProtonPiMinusP4_Measured = locProtonP4_Measured + locPiMinusP4_Measured;
		// TLorentzVector locPi0PiPlusP4_Measured = locPi0P4_Measured + locPiPlusP4_Measured;
		// TLorentzVector locPi0PiMinusP4_Measured = locPi0P4_Measured + locPiMinusP4_Measured;
		// TLorentzVector locPiPlusPiMinusP4_Measured = locPiPlusP4_Measured + locPiMinusP4_Measured;

		// TLorentzVector locPi0PiPlusPiMinusP4_Measured = locPi0P4_Measured + locPiPlusP4_Measured + locPiMinusP4_Measured;



		// fitted 4-vectors
		TLorentzVector locPi0P4 = locPhoton1P4 + locPhoton2P4;

		TLorentzVector locProtonPi0P4 = locProtonP4 + locPi0P4;
		TLorentzVector locProtonPiPlusP4 = locProtonP4 + locPiPlusP4;
		TLorentzVector locProtonPiMinusP4 = locProtonP4 + locPiMinusP4;
		TLorentzVector locPi0PiPlusP4 = locPi0P4 + locPiPlusP4;
		TLorentzVector locPi0PiMinusP4 = locPi0P4 + locPiMinusP4;
		TLorentzVector locPiPlusPiMinusP4 = locPiPlusP4 + locPiMinusP4;

		TLorentzVector locPi0PiPlusPiMinusP4 = locPi0P4 + locPiPlusP4 + locPiMinusP4;

		TLorentzVector locSumOfAllP4 = locPi0P4 + locPiPlusP4 + locPiMinusP4 + locProtonP4;

		TLorentzVector locProtonRho0P4 = locProtonP4 + locPiPlusP4 + locPiMinusP4;
		TLorentzVector locProtonRhoMinusP4 = locProtonP4 + locPi0P4 + locPiMinusP4;
		TLorentzVector locProtonRhoPlusP4 = locProtonP4 + locPi0P4 + locPiPlusP4;

		TLorentzVector loctChannelP4 = locBeamP4 - locPi0P4 - locPiMinusP4; // p_gamma - p_rho-
		double momentum_transfer_t = -1 * loctChannelP4.M2();



		/******************************************** EXECUTE ANALYSIS ACTIONS *******************************************/

		// Loop through the analysis actions, executing them in order for the active particle combo
		dAnalyzeCutActions->Perform_Action(); // Must be executed before Execute_Actions()
		if(!Execute_Actions()) continue; //if the active combo fails a cut, IsComboCutFlag automatically set
			

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

		/**************************************** EXAMPLE: HISTOGRAM BEAM ENERGY *****************************************/

		//Histogram beam energy (if haven't already)
		if(locUsedSoFar_BeamEnergy.find(locBeamID) == locUsedSoFar_BeamEnergy.end()) {
			dHist_BeamEnergy->Fill(locBeamP4.E()); // Fills in-time and out-of-time beam photon combos
			//dHist_BeamEnergy->Fill(locBeamP4.E(),locHistAccidWeightFactor); // Alternate version with accidental subtraction
			locUsedSoFar_BeamEnergy.insert(locBeamID);
		}

		/************************************ EXAMPLE: HISTOGRAM MISSING MASS SQUARED ************************************/

		//Missing Mass Squared
		double locMissingMassSquared = locMissingP4_Measured.M2();

		//Uniqueness tracking: Build the map of particles used for the missing mass
			//For beam: Don't want to group with final-state photons. Instead use "Unknown" PID (not ideal, but it's easy).
		map<Particle_t, set<Int_t> > locUsedThisCombo_MissingMass;
		locUsedThisCombo_MissingMass[Unknown].insert(locBeamID); //beam
		locUsedThisCombo_MissingMass[PiPlus].insert(locPiPlusTrackID);
		locUsedThisCombo_MissingMass[PiMinus].insert(locPiMinusTrackID);
		locUsedThisCombo_MissingMass[Proton].insert(locProtonTrackID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton1NeutralID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton2NeutralID);

		//compare to what's been used so far
		if(locUsedSoFar_MissingMass.find(locUsedThisCombo_MissingMass) == locUsedSoFar_MissingMass.end()) {
			//unique missing mass combo: histogram it, and register this combo of particles
			dHist_MissingMassSquared->Fill(locMissingMassSquared, weight); // Fills in-time and out-of-time beam photon combos
			dHist_MissingMassSquaredWoWeight->Fill(locMissingMassSquared);
			//dHist_MissingMassSquared->Fill(locMissingMassSquared,locHistAccidWeightFactor); // Alternate version with accidental subtraction
			locUsedSoFar_MissingMass.insert(locUsedThisCombo_MissingMass);
		}

		// reject events with a poor chi^2 for this reaction
		double chi2dof = dComboWrapper->Get_ChiSq_KinFit("") / dComboWrapper->Get_NDF_KinFit("");
		dHist_Chi2NDF->Fill(chi2dof, weight);
		if(chi2dof > dChi2Cut) continue; //{dComboWrapper->Set_IsComboCut(true); continue;} // reject this combo for future consideration

		// reject events with a missing mass squared far from zero
		if (abs(locMissingMassSquared) > dMissingMassSquaredCut) continue; //{dComboWrapper->Set_IsComboCut(true); continue;}
		


		// Unused showers cut
		int locNumUnusedShowers = int(dComboWrapper->Get_NumUnusedShowers());
        if (locNumUnusedShowers > dUnusedShowersCut) continue;
		dHist_unusedShowers->Fill(locNumUnusedShowers, weight);
		dHist2D_unusedShowers_Pi0PiMinus->Fill(locNumUnusedShowers, locPi0PiMinusP4.M(), weight);

		// Unused tracks cut
		int locNumUnusedTracks = int(dComboWrapper->Get_NumUnusedTracks());
        if (locNumUnusedTracks > dUnusedTracksCut) continue;
		dHist_unusedTracks->Fill(locNumUnusedTracks, weight);
		dHist2D_unusedTracks_Pi0PiMinus->Fill(locNumUnusedTracks, locPi0PiMinusP4.M(), weight);


		// Shower quality cut
		float qf1 = dPhoton1Wrapper->Get_Shower_Quality();
		float qf2 = dPhoton2Wrapper->Get_Shower_Quality();
		if (qf1 < dShowerQualityCut || qf2 < dShowerQualityCut) continue;
		dHist_showerQuality->Fill(qf1, weight);
		dHist_showerQuality->Fill(qf2, weight);
		dHist2D_showerQuality_Pi0PiMinus->Fill(qf1, locPi0PiMinusP4.M(), weight);
		dHist2D_showerQuality_Pi0PiMinus->Fill(qf2, locPi0PiMinusP4.M(), weight);


		/******************************************** BOOST TO DIFFERENT FRAMES *********************************************/

		// calculate delta Theta in p gamma CM frame
		TLorentzVector p4_pgamma_meas = locBeamP4_Measured + dTargetP4;
        // TLorentzVector p4_proton_CM(locProtonP4); p4_proton_CM.Boost(-p4_pgamma.BoostVector());
        // TLorentzVector p4_pip_CM(locPiPlusP4); p4_pip_CM.Boost(-p4_pgamma.BoostVector());
        // TLorentzVector p4_pi0_CM(locPi0P4); p4_pi0_CM.Boost(-p4_pgamma.BoostVector());
        // TLorentzVector p4_pim_CM(locPiMinusP4); p4_pim_CM.Boost(-p4_pgamma.BoostVector());
        // TLorentzVector p4_proton_pip_CM = p4_proton_CM + p4_pip_CM;
        // TLorentzVector p4_pi0_pim_CM = p4_pi0_CM + p4_pim_CM;
		TLorentzVector p4_proton_pip_CM_meas(locProtonP4_Measured + locPiPlusP4_Measured); p4_proton_pip_CM_meas.Boost(-p4_pgamma_meas.BoostVector());
		TLorentzVector p4_pi0_pim_CM_meas(locPi0P4_Measured + locPiMinusP4_Measured); p4_pi0_pim_CM_meas.Boost(-p4_pgamma_meas.BoostVector());

		double theta_proton_pip_CM = p4_proton_pip_CM_meas.Theta()*TMath::RadToDeg();
        double theta_pi0_pim_CM = p4_pi0_pim_CM_meas.Theta()*TMath::RadToDeg();
        double phi_proton_pip_CM = p4_proton_pip_CM_meas.Phi()*TMath::RadToDeg();
        double phi_pi0_pim_CM = p4_pi0_pim_CM_meas.Phi()*TMath::RadToDeg();
        if (phi_proton_pip_CM >= -180 && phi_proton_pip_CM < 0) theta_proton_pip_CM *= -1;
        if (phi_pi0_pim_CM >= -180 && phi_pi0_pim_CM < 0) theta_pi0_pim_CM *= -1;
		double deltaTheta_CM =  abs(theta_pi0_pim_CM - theta_proton_pip_CM);
		double deltaPhi_CM = abs(phi_pi0_pim_CM - phi_proton_pip_CM);


		// boost vector to Delta++ frame 
		TLorentzVector locDeltaPlusPlusP4 = locProtonP4 + locPiPlusP4;
		TVector3 locBoostVector_DeltaPlusPlus = -1.0*(locDeltaPlusPlusP4.BoostVector());

		TLorentzVector locProtonP4_DeltaSystem(locProtonP4);
		TLorentzVector locPiPlusP4_DeltaSystem(locPiPlusP4);
		TLorentzVector locBeamP4_DeltaSystem(locBeamP4);
		TLorentzVector locTargetP4_DeltaSystem(dTargetP4);

		locProtonP4_DeltaSystem.Boost(locBoostVector_DeltaPlusPlus);
		locPiPlusP4_DeltaSystem.Boost(locBoostVector_DeltaPlusPlus);
		locBeamP4_DeltaSystem.Boost(locBoostVector_DeltaPlusPlus);
		locTargetP4_DeltaSystem.Boost(locBoostVector_DeltaPlusPlus);

		//define Delta++ coordinate system
		TVector3 vz_Delta = -locBeamP4_DeltaSystem.Vect().Unit();
		TVector3 vy_Delta = (locProtonP4_DeltaSystem.Vect().Cross(locBeamP4_DeltaSystem.Vect())).Unit();
		TVector3 vx_Delta = (vy_Delta.Cross(vz_Delta)).Unit();

		TVector3 ProtonP3_DeltaSystem(locProtonP4_DeltaSystem.Vect().Dot(vx_Delta), locProtonP4_DeltaSystem.Vect().Dot(vy_Delta), locProtonP4_DeltaSystem.Vect().Dot(vz_Delta));
		TVector3 PiPlusP3_DeltaSystem(locPiPlusP4_DeltaSystem.Vect().Dot(vx_Delta), locPiPlusP4_DeltaSystem.Vect().Dot(vy_Delta), locPiPlusP4_DeltaSystem.Vect().Dot(vz_Delta));

		double deltaPhi_DeltaSystem = (ProtonP3_DeltaSystem.Phi() - PiPlusP3_DeltaSystem.Phi())*TMath::RadToDeg();
		double sumTheta_DeltaSystem = (ProtonP3_DeltaSystem.Theta() + PiPlusP3_DeltaSystem.Theta())*TMath::RadToDeg();
		double deltaTheta_DeltaSystem = (ProtonP3_DeltaSystem.Theta() - PiPlusP3_DeltaSystem.Theta())*TMath::RadToDeg();



		// bost vector to rho- frame
		TLorentzVector locRhoMinusP4 = locPi0P4 + locPiMinusP4;
		TVector3 locBoostVector_RhoMinus = -1.0*(locRhoMinusP4.BoostVector());

		TLorentzVector locPi0P4_RhoSystem(locPi0P4);
		TLorentzVector locPiMinusP4_RhoSystem(locPiMinusP4);
		TLorentzVector locBeamP4_RhoSystem(locBeamP4);
		TLorentzVector locTargetP4_RhoSystem(dTargetP4);

		locPi0P4_RhoSystem.Boost(locBoostVector_RhoMinus);
		locPiMinusP4_RhoSystem.Boost(locBoostVector_RhoMinus);
		locBeamP4_RhoSystem.Boost(locBoostVector_RhoMinus);
		locTargetP4_RhoSystem.Boost(locBoostVector_RhoMinus);

		//define rho- coordinate system
		TVector3 vz_Rho = -locBeamP4_RhoSystem.Vect().Unit();
		TVector3 vy_Rho = (locPi0P4_RhoSystem.Vect().Cross(locBeamP4_RhoSystem.Vect())).Unit();
		TVector3 vx_Rho = (vy_Rho.Cross(vz_Rho)).Unit();

		TVector3 Pi0P3_RhoSystem(locPi0P4_RhoSystem.Vect().Dot(vx_Rho), locPi0P4_RhoSystem.Vect().Dot(vy_Rho), locPi0P4_RhoSystem.Vect().Dot(vz_Rho));
		TVector3 PiMinusP3_RhoSystem(locPiMinusP4_RhoSystem.Vect().Dot(vx_Rho), locPiMinusP4_RhoSystem.Vect().Dot(vy_Rho), locPiMinusP4_RhoSystem.Vect().Dot(vz_Rho));

		double deltaPhi_RhoSystem = (Pi0P3_RhoSystem.Phi() - PiMinusP3_RhoSystem.Phi())*TMath::RadToDeg();
		double sumTheta_RhoSystem = (Pi0P3_RhoSystem.Theta() + PiMinusP3_RhoSystem.Theta())*TMath::RadToDeg();
		double deltaTheta_RhoSystem = (Pi0P3_RhoSystem.Theta() - PiMinusP3_RhoSystem.Theta())*TMath::RadToDeg();



		/********************************************** CUTS AND HISTOGRAMS *************************************************/

		dHist_InvMass_GammaGammaMeas->Fill(locPi0P4_Measured.M(), weight);

		dHist_InvMass_ProtonPi0->Fill(locProtonPi0P4.M(), weight);
		dHist_InvMass_ProtonPiPlus->Fill(locProtonPiPlusP4.M(), weight);
		dHist_InvMass_ProtonPiMinus->Fill(locProtonPiMinusP4.M(), weight);

		dHist_InvMass_Pi0PiPlus->Fill(locPi0PiPlusP4.M(), weight);
		dHist_InvMass_Pi0PiMinus->Fill(locPi0PiMinusP4.M(), weight);
		dHist_InvMass_PiPlusPiMinus->Fill(locPiPlusPiMinusP4.M(), weight);

		dHist_InvMass2D_ProtonPiPlus_vs_BeamEnergy->Fill(locProtonPiPlusP4.M(), locBeamP4.E(), weight);
		dHist_InvMass2D_ProtonPiPlus_vs_ProtonPiMinus->Fill(locProtonPiPlusP4.M(), locProtonPiMinusP4.M(), weight);

		dHist_InvMass2D_ProtonPiPlus_vs_deltaTheta->Fill(locProtonPiPlusP4.M(), deltaTheta_DeltaSystem, weight);
		dHist_InvMass2D_Pi0PiMinus_vs_deltaTheta->Fill(locPi0PiMinusP4.M(), deltaTheta_RhoSystem, weight);

		dHist_InvMass_Pi0PiPlusPiMinus->Fill(locPi0PiPlusPiMinusP4.M(), weight);
		dHist_InvMass2D_ProtonPiPlus_Pi0PiMinus->Fill(locProtonPiPlusP4.M(), locPi0PiMinusP4.M(), weight);


		dHist_Dalitz_ProtonPiPlusRhoMinus->Fill(locProtonPiPlusP4.M2(), locPi0PiPlusPiMinusP4.M2(), weight);
		dHist_Dalitz_ProtonPiMinusRhoPlus->Fill(locProtonPiMinusP4.M2(), locPi0PiPlusPiMinusP4.M2(), weight);
		dHist_Dalitz_ProtonPi0Rho0->Fill(locProtonPi0P4.M2(), locPi0PiPlusPiMinusP4.M2(), weight);
		dHist_Dalitz_ProtronRho0Pi0->Fill(locProtonRho0P4.M2(), locPi0PiPlusPiMinusP4.M2(), weight);
		dHist_Dalitz_ProtonRhoMinusPiPlus->Fill(locProtonRhoMinusP4.M2(), locPi0PiPlusPiMinusP4.M2(), weight);
		dHist_Dalitz_ProtonRhoPlusPiMinus->Fill(locProtonRhoPlusP4.M2(), locPi0PiPlusPiMinusP4.M2(), weight);


		// different E-Beam bins
		if ((locBeamP4.E() - 4) < 1.0)        dHist_InvMass2D_ProtonPiPlus_Pi0PiMinus_Ebeam0->Fill(locProtonPiPlusP4.M(), locPi0PiMinusP4.M(), weight); // 3 - 5 GeV
		else if ((locBeamP4.E() - 6.5) < 1.5) dHist_InvMass2D_ProtonPiPlus_Pi0PiMinus_Ebeam1->Fill(locProtonPiPlusP4.M(), locPi0PiMinusP4.M(), weight); // 5 - 8 GeV
		else if ((locBeamP4.E() - 8.5) < 0.3) dHist_InvMass2D_ProtonPiPlus_Pi0PiMinus_Ebeam2->Fill(locProtonPiPlusP4.M(), locPi0PiMinusP4.M(), weight); // 8.2 - 8.8 GeV

		// t channel histos
		dHist2D_tchannel_Pi0PiMinus->Fill(momentum_transfer_t, locPi0PiMinusP4.M(), weight);
		dHist2D_tchannel_Pi0PiPlus->Fill(momentum_transfer_t, locPi0PiPlusP4.M(), weight);
		dHist2D_tchannel_PiPlusPiMinus->Fill(momentum_transfer_t, locPiPlusPiMinusP4.M(), weight);
		dHist2D_tchannel_ProtonPi0->Fill(momentum_transfer_t, locProtonPi0P4.M(), weight);
		dHist2D_tchannel_ProtonPiPlus->Fill(momentum_transfer_t, locProtonPiPlusP4.M(), weight);
		dHist2D_tchannel_ProtonPiMinus->Fill(momentum_transfer_t, locProtonPiMinusP4.M(), weight);


		// angles
		dHist_CM_theta_Rhom->Fill(theta_pi0_pim_CM, weight);
		dHist_CM_theta_Deltapp->Fill(theta_proton_pip_CM, weight);
		dHist_CM_deltaTheta->Fill(deltaTheta_CM, weight);
		dHist_CM_phi_Rhom->Fill(phi_pi0_pim_CM, weight);
		dHist_CM_phi_Deltapp->Fill(phi_proton_pip_CM, weight);
		dHist_CM_deltaPhi->Fill(deltaPhi_CM, weight);

		// reject events which do not have a proper Delta++ system with a proton and a pi+
		//if (deltaTheta_DeltaSystem < 0. || deltaTheta_DeltaSystem > 100.) continue; //{dComboWrapper->Set_IsComboCut(true); continue;}
		//if (abs(deltaTheta_RhoSystem) > 70.) continue; //{dComboWrapper->Set_IsComboCut(true); continue;}

		// All cuts
		if (chi2dof < 5.0 && abs(locMissingMassSquared) < 0.005 && locNumUnusedShowers < 3 && locNumUnusedTracks < 2 ) {
			if ((abs(locProtonPiPlusP4.M() - 1.232) < 0.100) && (abs(locPi0PiMinusP4.M() - 0.770) < 0.100) && (locPi0PiPlusPiMinusP4.M() > 1.000)) {
				dHist_InvMass_ProtonPi0_AAC->Fill(locProtonPi0P4.M(), weight);
				dHist_InvMass_ProtonPiMinus_AAC->Fill(locProtonPiMinusP4.M(), weight);
				dHist_InvMass_Pi0PiPlus_AAC->Fill(locPi0PiPlusP4.M(), weight);
				dHist_InvMass_PiPlusPiMinus_AAC->Fill(locPiPlusPiMinusP4.M(), weight);

				dHist_InvMass_ProtonPiPlus_AAC->Fill(locProtonPiPlusP4.M(), weight);
				dHist_InvMass_Pi0PiMinus_AAC->Fill(locPi0PiMinusP4.M(), weight);
				dHist_InvMass_Pi0PiPlusPiMinus_AAC->Fill(locPi0PiPlusPiMinusP4.M(), weight);
			}
		
			if ((abs(locProtonPiPlusP4.M() - 1.232) > 0.100) && (abs(locPi0PiMinusP4.M() - 0.770) < 0.100) && (locPi0PiPlusPiMinusP4.M() > 1.000)) {
				dHist_InvMass_ProtonPiPlus_AAC->Fill(locProtonPiPlusP4.M(), weight);
			}

			if ((abs(locProtonPiPlusP4.M() - 1.232) < 0.100) && (abs(locPi0PiMinusP4.M() - 0.770) > 0.100) && (locPi0PiPlusPiMinusP4.M() > 1.000)) {
				dHist_InvMass_Pi0PiMinus_AAC->Fill(locPi0PiMinusP4.M(), weight);
			}

			if ((abs(locProtonPiPlusP4.M() - 1.232) < 0.100) && (abs(locPi0PiMinusP4.M() - 0.770) < 0.100) && (locPi0PiPlusPiMinusP4.M() < 1.000)) {
				dHist_InvMass_Pi0PiPlusPiMinus_AAC->Fill(locPi0PiPlusPiMinusP4.M(), weight);
			}


		}





	



		/****************************************** FILL FLAT TREE (IF DESIRED) ******************************************/
		
		// Final state vector
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

		// theta angles
		dFlatTreeInterface->Fill_Fundamental<float>("theta_proton", locProtonP4.Theta()*TMath::RadToDeg());
		dFlatTreeInterface->Fill_Fundamental<float>("theta_pim", locPiMinusP4.Theta()*TMath::RadToDeg());
		dFlatTreeInterface->Fill_Fundamental<float>("theta_pip", locPiPlusP4.Theta()*TMath::RadToDeg());
		dFlatTreeInterface->Fill_Fundamental<float>("theta_pi0", locPi0P4.Theta()*TMath::RadToDeg());
		
		// phi angles
		dFlatTreeInterface->Fill_Fundamental<float>("phi_proton", locProtonP4.Phi()*TMath::RadToDeg());
		dFlatTreeInterface->Fill_Fundamental<float>("phi_pim", locPiMinusP4.Phi()*TMath::RadToDeg());
		dFlatTreeInterface->Fill_Fundamental<float>("phi_pip", locPiPlusP4.Phi()*TMath::RadToDeg());
		dFlatTreeInterface->Fill_Fundamental<float>("phi_pi0", locPi0P4.Phi()*TMath::RadToDeg());

		// invariant masses of combinations
		dFlatTreeInterface->Fill_Fundamental<float>("m_proton_pim", locProtonPiMinusP4.M());
		dFlatTreeInterface->Fill_Fundamental<float>("m_proton_pip", locProtonPiPlusP4.M());
		dFlatTreeInterface->Fill_Fundamental<float>("m_proton_pi0", locProtonPi0P4.M());
		dFlatTreeInterface->Fill_Fundamental<float>("m_pi0_pip", locPi0PiPlusP4.M());
		dFlatTreeInterface->Fill_Fundamental<float>("m_pi0_pim", locPi0PiMinusP4.M());
		dFlatTreeInterface->Fill_Fundamental<float>("m_pip_pim", locPiPlusPiMinusP4.M());

		// additional information
		dFlatTreeInterface->Fill_Fundamental<float>("mandel_t", momentum_transfer_t);
		dFlatTreeInterface->Fill_Fundamental<float>("Weight", weight);
		dFlatTreeInterface->Fill_Fundamental<float>("chi2ndf", chi2dof);
		dFlatTreeInterface->Fill_Fundamental<float>("missingMassSquared", locMissingMassSquared);
		dFlatTreeInterface->Fill_Fundamental<float>("unusedShowers", locNumUnusedShowers);
		dFlatTreeInterface->Fill_Fundamental<float>("unusedTracks", locNumUnusedTracks);
		dFlatTreeInterface->Fill_Fundamental<float>("showerQuality", qf1);
		dFlatTreeInterface->Fill_Fundamental<float>("showerQuality", qf2);

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
		Fill_FlatTree(); // for the active combo
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

void DSelector_RhomDeltapp::Finalize(void)
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
