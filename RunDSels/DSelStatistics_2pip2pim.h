#ifndef DSelStatistics_2pip2pim_h
#define DSelStatistics_2pip2pim_h

#include <iostream>

#include "DSelector/DSelector.h"
#include "DSelector/DHistogramActions.h"
#include "DSelector/DCutActions.h"

#include "TH1I.h"
#include "TH2I.h"

class DSelStatistics_2pip2pim : public DSelector
{
	public:

		DSelStatistics_2pip2pim(TTree* locTree = NULL) : DSelector(locTree){}
                virtual ~DSelStatistics_2pip2pim(){}

		void Init(TTree *tree);
		Bool_t Process(Long64_t entry);

	private:

		void Get_ComboWrappers(void);
		void Finalize(void);

		// BEAM POLARIZATION INFORMATION
		UInt_t dPreviousRunNumber;
		bool dIsPolarizedFlag; //else is AMO
		bool dIsPARAFlag; //else is PERP or AMO
		Int_t dBeamPolarizationAngle;
                Int_t dDiamondSet; // 0=amo, 1=para 0, 2=perp 90, 3=para 135, 4=perp 45
		bool dIsMC;

		// ANALYZE CUT ACTIONS
		// // Automatically makes mass histograms where one cut is missing
		DHistogramAction_AnalyzeCutActions* dAnalyzeCutActions;

		//CREATE REACTION-SPECIFIC PARTICLE ARRAYS
  
		//Step 0
		DParticleComboStep* dStep0Wrapper;
		DBeamParticle* dComboBeamWrapper;
		DChargedTrackHypothesis* dPiPlus1Wrapper;
		DChargedTrackHypothesis* dPiPlus2Wrapper;
		DChargedTrackHypothesis* dPiMinus1Wrapper;
		DChargedTrackHypothesis* dPiMinus2Wrapper;
		DChargedTrackHypothesis* dProtonWrapper;

  
               //Containers for counting unique track IDs
               std::set<Int_t> BeamID_List;	
               std::set<Int_t> Proton_TrackID_List, PiPlusU_TrackID_List, PiPlusL_TrackID_List, PiMinusFast_TrackID_List, PiMinusSlow_TrackID_List;

  
		// DEFINE YOUR HISTOGRAMS HERE
		// EXAMPLES:
		TH1D* dHist_MissingMassSquared;
		TH1D* dHist_BeamEnergy;
		TH1D* dHist_BeamEnergy_BestChiSq;
                TH2I* dHist2D_NumUniqueParticles;
  
  
	ClassDef(DSelStatistics_2pip2pim, 0);
};

void DSelStatistics_2pip2pim::Get_ComboWrappers(void)
{
	//Step 0
	dStep0Wrapper = dComboWrapper->Get_ParticleComboStep(0);
	dComboBeamWrapper = static_cast<DBeamParticle*>(dStep0Wrapper->Get_InitialParticle());
	dPiPlus1Wrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(0));
	dPiPlus2Wrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(1));
	dPiMinus1Wrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(2));
	dPiMinus2Wrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(3));
	dProtonWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(4));
}

#endif // DSelStatistics_2pip2pim_h
