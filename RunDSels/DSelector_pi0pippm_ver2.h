#ifndef DSelector_pi0pippm_ver2_h
#define DSelector_pi0pippm_ver2_h

#include <iostream>

#include "DSelector/DSelector.h"
#include "DSelector/DHistogramActions.h"
#include "DSelector/DCutActions.h"

#include "TH1I.h"
#include "TH2I.h"
#include <utility>

struct HistConfig {
    std::string name;         // Histogram name
    std::string title;        // Titel 
    int nbins;                
    float xmin;               
    float xmax;               
};
struct Hist2DConfig {
    std::string name;
    std::string title;
    int nbinsX;
    int nbinsY;
    double ymin;
    double ymax;
};
class DSelector_pi0pippm_ver2 : public DSelector
{
	public:
		float dCut_chi2perndf;
		std::pair<float, float> dCut_Ebeam;
		std::pair<float, float> dCut_MissingMass;
		int dCut_shower;
		float dCut_qf;
		float dCut_R;
		std::pair<float, float> dCut_z;
		std::pair<float, float> dCut_Delta;
		int dCut_tracks;
		std::pair<float, float> dCut_rho_mass;
		float dCut_t;
		std::pair<float, float> dCut_Pi0_mass;
		float dCut_Omega_mass = 0.9;
		std::vector<std::string> topologien;
		
		std::vector<std::vector<TH1F*>> histMatrix;
		std::vector<std::vector<TH2F*>> hist2DMatrix;
		std::vector<std::vector<std::map<std::string, TH1F*>>> histMatrix3D;
		std::vector<std::map<std::string, int>> Nsurvived;




	
		

		
		DSelector_pi0pippm_ver2(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_pi0pippm_ver2(){}

		void Init(TTree *tree);
		Bool_t Process(Long64_t entry);

	private:

		void Get_ComboWrappers(void);
		void Finalize(void);

		// BEAM POLARIZATION INFORMATION
		UInt_t dPreviousRunNumber;
		bool dIsPolarizedFlag; //else is AMO
		bool dIsPARAFlag; //else is PERP or AMO
		Int_t dBeamPolarisationAngle;
        Int_t polarisationState; // 0 = amo, 1 = para 0, 2 = perp 90, 3 = para 135, 4 = perp 45
		bool dIsMC;

		
		// ANALYZE CUT ACTIONS
		// // Automatically makes mass histograms where one cut is missing
		DHistogramAction_AnalyzeCutActions* dAnalyzeCutActions;

		//CREATE REACTION-SPECIFIC PARTICLE ARRAYS

		//Step 0
		DParticleComboStep* dStep0Wrapper;
		DBeamParticle* dComboBeamWrapper;
		DChargedTrackHypothesis* dPiPlusWrapper;
		DChargedTrackHypothesis* dPiMinusWrapper;
		DChargedTrackHypothesis* dProtonWrapper;

		//Step 1
		DParticleComboStep* dStep1Wrapper;
		DNeutralParticleHypothesis* dPhoton1Wrapper;
		DNeutralParticleHypothesis* dPhoton2Wrapper;

		// DEFINE YOUR HISTOGRAMS HERE
		// EXAMPLES:
		
		TH1F* dHist_BeamEnergy;
		TH1F* dHist_MissingMassSquared;
		TH1F* dHistThrownTopologies;
		
		TH2F* dHist_2D_Mass_3pi_vs_pi0pim;
		TH2F* dHist_2D_Mass_3pi_vs_ppip;
		TH2F* dHist_2D_Mass_pi0pim_vs_ppip;
		TH2F* dHist_2D_Mass_pippim_vs_ppi0;
		
		
		
		


		TH2F* dHist_2D_Mass_pi0pip_vs_ppim;

		TH2F* dHist_Proton_Angle;
		TH2F* dHist_Pi0_Angle;
		TH2F* dHist_PiPlus_Angle;
		TH2F* dHist_PiMinus_Angle;
		TH2F* dHist_Photon1_Angle;
		TH2F* dHist_Photon2_Angle;
		TH2F* dHist_RhoMinus_Angle;
		TH2F* dHist_RhoPlus_Angle;
		TH2F* dHist_Rho0_Angle;
		TH2F* dHist_Omega_Angle;
		TH2F* dHist_Delta_Angle;
		



		TH1F* dHist_deltaPhi_DeltaPlusPlus_System;
		TH1F* dHist_CM_theta_Rhom;
		TH1F* dHist_combo_before;
		TH1F* dHist_combo_after;

		
		
		

	ClassDef(DSelector_pi0pippm_ver2, 0);
};

void DSelector_pi0pippm_ver2::Get_ComboWrappers(void)
{
	//Step 0
	dStep0Wrapper = dComboWrapper->Get_ParticleComboStep(0);
	dComboBeamWrapper = static_cast<DBeamParticle*>(dStep0Wrapper->Get_InitialParticle());
	dPiPlusWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(1));
	dPiMinusWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(2));
	dProtonWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(3));

	//Step 1
	dStep1Wrapper = dComboWrapper->Get_ParticleComboStep(1);
	dPhoton1Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep1Wrapper->Get_FinalParticle(0));
	dPhoton2Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep1Wrapper->Get_FinalParticle(1));
}

#endif // DSelector_pi0pippm_ver2_h
