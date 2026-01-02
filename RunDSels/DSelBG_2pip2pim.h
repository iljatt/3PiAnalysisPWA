#ifndef DSelBG_2pip2pim_h
#define DSelBG_2pip2pim_h

#include <iostream>

#include "DSelector/DSelector.h"
#include "DSelector/DHistogramActions.h"
#include "DSelector/DCutActions.h"

#include "TH1I.h"
#include "TH2I.h"

class DSelBG_2pip2pim : public DSelector
{
	public:

		DSelBG_2pip2pim(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelBG_2pip2pim(){}

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

		// DEFINE YOUR HISTOGRAMS HERE
		// EXAMPLES:
		TH1I* dHist_MissingMassSquared;
		TH1I* dHist_BeamEnergy;
		TH1I* dHist_BeamEnergy_BestChiSq;

                //A LIST WITH KNOWN TOPOLOGIES 
                Int_t NTOP = 221;
                char const *dKnownTopologies[221] = {
		  "2#pi^{#plus}2#pi^{#minus}p",
		  "2#pi^{#plus}2#pi^{#minus}p[#omega]",
		  "2#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0}]",
	  	  "2#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#omega]",
		  "#pi^{#plus}#pi^{#minus}K^{#plus}K^{#minus}p",
		  "#gamma2#pi^{#plus}2#pi^{#minus}p[#eta']",
		  "2#pi^{#plus}#pi^{#minus}K^{#minus}p[K^{0}_{S}]",
		  "2#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#eta]",
		  "2#gamma#pi^{#plus}#pi^{#minus}p[#pi^{0}]",
		  "#gamma2#pi^{#plus}2#pi^{#minus}p[#eta]",
		  "4#gamma2#pi^{#plus}2#pi^{#minus}p[2#pi^{0},#omega]",
		  "2#gamma#pi^{#plus}#pi^{#minus}p[#pi^{0},#omega]",
		  "#gammae^{#plus}e^{#minus}2#pi^{#plus}2#pi^{#minus}p[#pi^{0}]",
		  "#pi^{#plus}2#pi^{#minus}K^{#plus}p[K^{0}_{S}]",
		  "#pi^{#plus}2#pi^{#minus}K^{#plus}p[#Lambda]",
		  "2#gamma#pi^{#plus}#pi^{#minus}K^{#plus}K^{#minus}p[#pi^{0},#eta]",
		  "2#gamma2#pi^{#plus}3#pi^{#minus}K^{#plus}p[#pi^{0},K^{0}_{S},#omega]",
		  "2#gamma2#pi^{#plus}2#pi^{#minus}K^{0}_{L}p[#pi^{0},K^{0}_{S}]",
		  "2#gamma2#pi^{#plus}2#pi^{#minus}K^{#plus}K^{#minus}p[#pi^{0}]",
		  "2#gamma2#pi^{#plus}2#pi^{#minus}K^{#plus}K^{#minus}p[#pi^{0},#omega]",
		  "2#gamma2#pi^{#plus}2#pi^{#minus}K^{#plus}K^{#minus}p[#pi^{0},#phi]",
		  "2#gamma2#pi^{#plus}2#pi^{#minus}K^{#plus}n[#pi^{0},#Lambda]",
		  "2#gamma2#pi^{#plus}2#pi^{#minus}p[#eta]",
		  "2#gamma2#pi^{#plus}2#pi^{#minus}p[#eta,#eta']",
		  "2#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#eta,#eta']",
		  "2#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#eta,#omega]",
		  "2#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},K^{0}_{S}]",
		  "2#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},K^{0}_{S},#Sigma^{#plus}]",
		  "2#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#phi]",
		  "2#gamma2#pi^{#plus}3#pi^{#minus}K^{#plus}p[#Lambda,#Sigma^{0},#eta']",
		  "2#gamma2#pi^{#plus}3#pi^{#minus}K^{#plus}p[#pi^{0},K^{0}_{S}]",
		  "2#gamma2#pi^{#plus}3#pi^{#minus}K^{#plus}p[#pi^{0},#Lambda]",
		  "2#gamma2#pi^{#plus}3#pi^{#minus}K^{#plus}p[#pi^{0},#Lambda,#omega]",
		  "2#gamma2#pi^{#plus}#pi^{#minus}K^{#minus}p[#pi^{0},K^{0}_{S}]",
		  "2#gamma2#pi^{#plus}#pi^{#minus}K^{#plus}K^{#minus}n[#pi^{0}]",
		  "2#gamma2#pi^{#plus}#pi^{#minus}n[#pi^{0}]",
		  "2#gamma3#pi^{#plus}2#pi^{#minus}K^{0}_{L}n[#pi^{0},#Sigma^{#plus},#omega]",
		  "2#gamma3#pi^{#plus}2#pi^{#minus}n[#pi^{0}]",
		  "2#gamma3#pi^{#plus}2#pi^{#minus}n[#pi^{0},#eta]",
		  "2#gamma3#pi^{#plus}2#pi^{#minus}n[#pi^{0},#omega]",
		  "2#gamma3#pi^{#plus}3#pi^{#minus}K^{#plus}n[#pi^{0},K^{0}_{S}]",
		  "2#gamma3#pi^{#plus}3#pi^{#minus}p[#pi^{0}]",
		  "2#gamma3#pi^{#plus}3#pi^{#minus}p[#pi^{0},#eta]",
		  "2#gamma3#pi^{#plus}3#pi^{#minus}p[#pi^{0},#eta,#eta']",
		  "2#gamma3#pi^{#plus}3#pi^{#minus}p[#pi^{0},#eta,#omega]",
		  "2#gamma3#pi^{#plus}3#pi^{#minus}p[#pi^{0},#omega]",
		  "2#gamma4#pi^{#plus}3#pi^{#minus}K^{#minus}p[#pi^{0},K^{0}_{S},#eta]",
		  "2#gamma4#pi^{#plus}3#pi^{#minus}n[#pi^{0},K^{0}_{S},#Sigma^{#plus},#omega]",
		  "2#gamma4#pi^{#plus}3#pi^{#minus}n[#pi^{0},#omega]",
		  "2#gamma4#pi^{#plus}4#pi^{#minus}p[#pi^{0},K^{0}_{S},#eta,#Lambda]",
		  "2#gamma4#pi^{#plus}4#pi^{#minus}p[#pi^{0},K^{0}_{S},#Lambda,#phi]",
		  "2#gammae^{#plus}e^{#minus}#pi^{#plus}#pi^{#minus}p[#pi^{0},#omega]",
		  "2#gammaK^{#plus}K^{#minus}p[#pi^{0}]",
		  "2#gamma#pi^{#plus}2#pi^{#minus}K^{#plus}p[#pi^{0},K^{0}_{S}]",
		  "2#gamma#pi^{#plus}2#pi^{#minus}K^{#plus}p[#pi^{0},#Lambda]",
		  "2#gamma#pi^{#plus}2#pi^{#minus}K^{#plus}p[#pi^{0},#Lambda,#omega]",
		  "2#gamma#pi^{#plus}2#pi^{#minus}K^{#plus}p[#pi^{0},#Sigma^{#plus}]",
		  "2#gamma#pi^{#plus}K^{#plus}K^{#minus}n[#pi^{0}]",
		  "2#gamma#pi^{#plus}#pi^{#minus}2p#bar{p}[#pi^{0}]",
		  "2#gamma#pi^{#plus}#pi^{#minus}K^{#plus}K^{#minus}p[#pi^{0}]",
		  "2#gamma#pi^{#plus}#pi^{#minus}K^{#plus}K^{#minus}p[#pi^{0},#omega]",
		  "2#gamma#pi^{#plus}#pi^{#minus}K^{#plus}n[#pi^{0},#Sigma^{#plus}]",
		  "2#gamma#pi^{#plus}#pi^{#minus}p[#eta']",
		  "2#gamma#pi^{#plus}#pi^{#minus}p[#eta]",
		  "2#gamma#pi^{#plus}#pi^{#minus}p[#pi^{0},#phi]",
		  "2K^{#plus}2K^{#minus}p[#phi]",
		  "2#pi^{#plus}2#pi^{#minus}K^{0}_{L}p[K^{0}_{S}]",
		  "2#pi^{#plus}2#pi^{#minus}K^{#plus}K^{#minus}p",
		  "2#pi^{#plus}2#pi^{#minus}K^{#plus}K^{#minus}p[K^{0}_{S},#Lambda,#phi]",
		  "2#pi^{#plus}2#pi^{#minus}K^{#plus}n[K^{0}_{S}]",
		  "2#pi^{#plus}2#pi^{#minus}K^{#plus}n[#Sigma^{#plus}]",
		  "2#pi^{#plus}2#pi^{#minus}p[K^{0}_{S}]",
		  "2#pi^{#plus}2#pi^{#minus}p[K^{0}_{S},#Lambda]",
		  "2#pi^{#plus}3#pi^{#minus}K^{0}_{L}K^{#plus}p",
		  "2#pi^{#plus}3#pi^{#minus}K^{0}_{L}K^{#plus}p[K^{0}_{S},#Lambda,#phi]",
		  "2#pi^{#plus}3#pi^{#minus}K^{#plus}p[K^{0}_{S}]",
		  "2#pi^{#plus}3#pi^{#minus}K^{#plus}p[K^{0}_{S},#omega]",
		  "2#pi^{#plus}3#pi^{#minus}K^{#plus}p[#Lambda]",
		  "2#pi^{#plus}#pi^{#minus}K^{0}_{L}K^{#minus}p",
		  "2#pi^{#plus}#pi^{#minus}K^{0}_{L}K^{#plus}K^{#minus}n[K^{0}_{S},#phi]",
		  "2#pi^{#plus}#pi^{#minus}K^{#plus}K^{#minus}n",
		  "2#pi^{#plus}#pi^{#minus}n",
		  "3#gamma2#pi^{#plus}2#pi^{#minus}p[#eta]",
		  "3#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0}]",
		  "3#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#eta']",
		  "3#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#eta]",
		  "3#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#omega,#eta']",
		  "3#gamma2#pi^{#plus}3#pi^{#minus}K^{#plus}p[#pi^{0},#eta,#Lambda,#omega]",
		  "3#gamma3#pi^{#plus}2#pi^{#minus}n[#pi^{0},#eta]",
		  "3#gamma3#pi^{#plus}3#pi^{#minus}K^{#plus}n[#pi^{0},#Lambda,#eta']",
		  "3#gamma3#pi^{#plus}3#pi^{#minus}p[#pi^{0},#eta']",
		  "3#gamma3#pi^{#plus}3#pi^{#minus}p[#pi^{0},#eta]",
		  "3#gamma3#pi^{#plus}3#pi^{#minus}p[#pi^{0},#eta,#eta']",
		  "3#gammae^{#plus}e^{#minus}2#pi^{#plus}2#pi^{#minus}p[2#pi^{0}]",
		  "3#gammae^{#plus}e^{#minus}2#pi^{#plus}2#pi^{#minus}p[2#pi^{0},#omega]",
		  "3#gammae^{#plus}e^{#minus}2#pi^{#plus}#pi^{#minus}n[2#pi^{0}]",
		  "3#gammae^{#plus}e^{#minus}p[2#pi^{0}]",
		  "3#gammae^{#plus}e^{#minus}#pi^{#plus}#pi^{#minus}p[2#pi^{0}]",
		  "3#gamma#pi^{#plus}#pi^{#minus}p[#pi^{0},#eta]",
		  "3#gamma#pi^{#plus}#pi^{#minus}p[#pi^{0},#omega]",
		  "3#pi^{#plus}2#pi^{#minus}K^{0}_{L}n[K^{0}_{S}]",
		  "3#pi^{#plus}2#pi^{#minus}K^{#minus}p[K^{0}_{S}]",
		  "3#pi^{#plus}2#pi^{#minus}n",
		  "3#pi^{#plus}2#pi^{#minus}n[K^{0}_{S},#Sigma^{#plus}]",
		  "3#pi^{#plus}2#pi^{#minus}n[#omega]",
		  "3#pi^{#plus}3#pi^{#minus}K^{0}_{L}p[K^{0}_{S}]",
		  "3#pi^{#plus}3#pi^{#minus}K^{#plus}n[K^{0}_{S}]",
		  "3#pi^{#plus}3#pi^{#minus}p",
		  "3#pi^{#plus}3#pi^{#minus}p[K^{0}_{S}]",
		  "3#pi^{#plus}3#pi^{#minus}p[K^{0}_{S},#Lambda]",
		  "3#pi^{#plus}3#pi^{#minus}p[#omega]",
		  "3#pi^{#plus}#pi^{#minus}K^{0}_{L}K^{#minus}n",
		  "4#gamma2#pi^{#plus}2#pi^{#minus}p[2#pi^{0}]",
		  "4#gamma2#pi^{#plus}2#pi^{#minus}p[2#pi^{0},#eta]",
		  "4#gamma2#pi^{#plus}2#pi^{#minus}p[2#pi^{0},#eta,#eta']",
		  "4#gamma2#pi^{#plus}2#pi^{#minus}p[2#pi^{0},K^{0}_{S},#Lambda]",
		  "4#gamma#pi^{#plus}2#pi^{#minus}K^{#plus}p[2#pi^{0},#Sigma^{#plus}]",
		  "4#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#eta]",
		  "4#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#eta,#eta']",
		  "4#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#eta,#omega]",
		  "4#gamma2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#omega,#eta']",
		  "4#gamma2#pi^{#plus}3#pi^{#minus}K^{#plus}p[2#pi^{0},#eta,#Lambda]",
		  "4#gamma2#pi^{#plus}#pi^{#minus}n[2#pi^{0}]",
		  "4#gamma3#pi^{#plus}2#pi^{#minus}n[2#pi^{0}]",
		  "4#gamma3#pi^{#plus}2#pi^{#minus}n[2#pi^{0},#eta]",
		  "4#gamma3#pi^{#plus}2#pi^{#minus}n[2#pi^{0},K^{0}_{S},#Lambda]",
		  "4#gamma3#pi^{#plus}2#pi^{#minus}n[2#pi^{0},#omega]",
		  "4#gamma3#pi^{#plus}2#pi^{#minus}n[#pi^{0},#eta,#omega,#eta']",
		  "4#gamma3#pi^{#plus}3#pi^{#minus}K^{#plus}n[2#pi^{0},K^{0}_{S},#omega]",
		  "4#gamma3#pi^{#plus}3#pi^{#minus}p[2#pi^{0}]",
		  "4#gamma3#pi^{#plus}3#pi^{#minus}p[2#pi^{0},#eta]",
		  "4#gamma3#pi^{#plus}3#pi^{#minus}p[2#pi^{0},#eta,#omega]",
		  "4#gamma3#pi^{#plus}3#pi^{#minus}p[2#pi^{0},#omega]",
		  "4#gamma#pi^{#plus}2#pi^{#minus}K^{#plus}p[2#pi^{0},K^{0}_{S}]",
		  "4#gamma#pi^{#plus}2#pi^{#minus}K^{#plus}p[2#pi^{0},#Sigma^{#plus},#omega]",
		  "4#gamma#pi^{#plus}#pi^{#minus}K^{#plus}K^{#minus}p[2#pi^{0}]",
		  "4#gamma#pi^{#plus}#pi^{#minus}K^{#plus}K^{#minus}p[2#pi^{0},#eta]",
		  "4#gamma#pi^{#plus}#pi^{#minus}p[2#pi^{0}]",
		  "4#gamma#pi^{#plus}#pi^{#minus}p[2#pi^{0},#omega]",
		  "4#gamma#pi^{#plus}#pi^{#minus}p[#pi^{0},#eta]",
		  "4#pi^{#plus}2#pi^{#minus}K^{#minus}n[K^{0}_{S}]",
		  "4#pi^{#plus}3#pi^{#minus}n",
		  "4#pi^{#plus}4#pi^{#minus}p[#omega]",
		  "5#gamma2#pi^{#plus}2#pi^{#minus}K^{#plus}n[2#pi^{0},#eta,#Lambda]",
		  "5#gamma2#pi^{#plus}2#pi^{#minus}p[2#pi^{0},#eta]",
		  "5#gamma2#pi^{#plus}2#pi^{#minus}p[2#pi^{0},#omega]",
		  "5#gamma2#pi^{#plus}2#pi^{#minus}p[2#pi^{0},#omega,#eta']",
		  "5#gamma3#pi^{#plus}3#pi^{#minus}p[2#pi^{0},#eta]",
		  "5#gamma3#pi^{#plus}3#pi^{#minus}p[2#pi^{0},#eta,#eta']",
		  "5#gammae^{#plus}e^{#minus}2#pi^{#plus}2#pi^{#minus}p[3#pi^{0},#omega]",
		  "5#gammae^{#plus}e^{#minus}2#pi^{#plus}#pi^{#minus}n[3#pi^{0}]",
		  "5#gammae^{#plus}e^{#minus}#pi^{#plus}#pi^{#minus}p[3#pi^{0}]",
		  "6#gamma2#pi^{#plus}2#pi^{#minus}p[3#pi^{0}]",
		  "6#gamma2#pi^{#plus}2#pi^{#minus}p[3#pi^{0},#eta]",
		  "6#gamma2#pi^{#plus}2#pi^{#minus}p[3#pi^{0},#eta,#eta']",
		  "6#gamma2#pi^{#plus}2#pi^{#minus}p[3#pi^{0},#eta,#omega]",
		  "6#gamma2#pi^{#plus}2#pi^{#minus}p[3#pi^{0},#omega]",
		  "6#gamma2#pi^{#plus}#pi^{#minus}K^{#minus}p[3#pi^{0},K^{0}_{S}]",
		  "6#gamma3#pi^{#plus}2#pi^{#minus}n[2#pi^{0},#eta,#omega]",
		  "6#gamma3#pi^{#plus}2#pi^{#minus}n[3#pi^{0}]",
		  "6#gamma3#pi^{#plus}2#pi^{#minus}n[3#pi^{0},#eta]",
		  "6#gamma3#pi^{#plus}2#pi^{#minus}n[3#pi^{0},#omega]",
		  "6#gamma3#pi^{#plus}3#pi^{#minus}p[3#pi^{0},#eta]",
		  "6#gamma3#pi^{#plus}3#pi^{#minus}p[3#pi^{0},#eta,#omega]",
		  "6#gamma3#pi^{#plus}3#pi^{#minus}p[3#pi^{0},#omega]",
		  "6#gamma#pi^{#plus}2#pi^{#minus}K^{#plus}p[3#pi^{0},K^{0}_{S}]",
		  "6#gamma#pi^{#plus}2#pi^{#minus}K^{#plus}p[3#pi^{0},#Lambda]",
		  "6#gamma#pi^{#plus}K^{#minus}p[3#pi^{0},K^{0}_{S}]",
		  "6#gamma#pi^{#plus}#pi^{#minus}K^{#plus}K^{#minus}p[3#pi^{0}]",
		  "6#gamma#pi^{#plus}#pi^{#minus}K^{#plus}K^{#minus}p[3#pi^{0},#omega]",
		  "6#gamma#pi^{#plus}#pi^{#minus}p[3#pi^{0}]",
		  "6#gamma#pi^{#plus}#pi^{#minus}p[3#pi^{0},#eta]",
		  "7#gamma2#pi^{#plus}2#pi^{#minus}p[3#pi^{0},#eta']",
		  "8#gamma2#pi^{#plus}2#pi^{#minus}p[4#pi^{0}]",
		  "8#gamma2#pi^{#plus}2#pi^{#minus}p[4#pi^{0},#eta]",
		  "8#gamma2#pi^{#plus}2#pi^{#minus}p[4#pi^{0},#eta,#omega]",
		  "8#gamma#pi^{#plus}#pi^{#minus}p[4#pi^{0}]",
		  "8#gamma#pi^{#plus}#pi^{#minus}p[4#pi^{0},#omega]",
		  "e^{#plus}e^{#minus}#pi^{#plus}#pi^{#minus}p",
		  "e^{#plus}e^{#minus}#pi^{#plus}#pi^{#minus}p[#omega]",
		  "e^{#plus}e^{#minus}#pi^{#plus}#pi^{#minus}p[#phi]",
		  "#gamma2#pi^{#plus}2#pi^{#minus}K^{0}_{L}p[#Lambda,#eta']",
		  "#gamma2#pi^{#plus}2#pi^{#minus}p",
		  "#gamma2#pi^{#plus}2#pi^{#minus}p[#eta,#eta']",
		  "#gamma2#pi^{#plus}2#pi^{#minus}p[#eta,#omega]",
		  "#gamma2#pi^{#plus}2#pi^{#minus}p[#omega,#eta']",
		  "#gamma3#pi^{#plus}2#pi^{#minus}K^{#minus}p[K^{0}_{S}]",
		  "#gamma3#pi^{#plus}2#pi^{#minus}n[#eta']",
		  "#gamma3#pi^{#plus}2#pi^{#minus}n[#eta]",
		  "#gamma3#pi^{#plus}3#pi^{#minus}p[#eta']",
		  "#gamma3#pi^{#plus}3#pi^{#minus}p[#eta]",
		  "#gamma3#pi^{#plus}3#pi^{#minus}p[#eta,#eta']",
		  "#gamma4#pi^{#plus}4#pi^{#minus}p[#eta]",
		  "#gamma4#pi^{#plus}4#pi^{#minus}p[#eta,#eta']",
		  "#gammae^{#plus}e^{#minus}2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#eta]",
		  "#gammae^{#plus}e^{#minus}2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#eta,#omega]",
		  "#gammae^{#plus}e^{#minus}2#pi^{#plus}2#pi^{#minus}p[#pi^{0},#omega]",
		  "#gammae^{#plus}e^{#minus}2#pi^{#plus}#pi^{#minus}K^{#plus}K^{#minus}n[#pi^{0}]",
		  "#gammae^{#plus}e^{#minus}2#pi^{#plus}#pi^{#minus}K^{#plus}K^{#minus}n[#pi^{0},#omega]",
		  "#gammae^{#plus}e^{#minus}K^{#plus}K^{#minus}p[#pi^{0}]",
		  "#gammae^{#plus}e^{#minus}K^{#plus}K^{#minus}p[#pi^{0},#phi]",
		  "#gammae^{#plus}e^{#minus}#pi^{#plus}#pi^{#minus}p[#pi^{0}]",
		  "#gammae^{#plus}e^{#minus}#pi^{#plus}#pi^{#minus}p[#pi^{0},#omega]",
		  "#gammae^{#plus}e^{#minus}#pi^{#plus}#pi^{#minus}p[#pi^{0},#phi]",
		  "#gamma#pi^{#plus}2#pi^{#minus}2K^{#plus}K^{#minus}p[#Lambda,#Sigma^{0}]",
		  "#gamma#pi^{#plus}2#pi^{#minus}K^{#plus}p[K^{0}_{S}]",
		  "#gamma#pi^{#plus}2#pi^{#minus}K^{#plus}p[#Lambda,#Sigma^{0}]",
		  "#gamma#pi^{#plus}3#pi^{#minus}2K^{#plus}p[K^{0}_{S},#Lambda,#Sigma^{0}]",
		  "#gamma#pi^{#plus}#pi^{#minus}K^{#plus}K^{#minus}p[#eta']",
		  "#gamma#pi^{#plus}#pi^{#minus}K^{#plus}K^{#minus}p[#eta]",
		  "#gamma#pi^{#plus}#pi^{#minus}p[#eta']",
		  "#gamma#pi^{#plus}#pi^{#minus}p[#eta]",
		  "K^{#plus}K^{#minus}p[#phi]",
		  "#mu^{#plus}#mu^{#minus}#pi^{#plus}#pi^{#minus}p",
		  "#pi^{#minus}2K^{#plus}K^{#minus}p[#Lambda,#phi]",
		  "#pi^{#plus}2#pi^{#minus}K^{0}_{L}K^{#plus}p",
		  "#pi^{#plus}2#pi^{#minus}K^{#plus}p[#Lambda,#omega]",
		  "#pi^{#plus}#pi^{#minus}2p#bar{p}",
		  "#pi^{#plus}#pi^{#minus}K^{#plus}K^{#minus}p[#omega,#phi]",
		  "#pi^{#plus}#pi^{#minus}K^{#plus}K^{#minus}p[#phi]",
		  "#pi^{#plus}#pi^{#minus}p"
		};

  
	ClassDef(DSelBG_2pip2pim, 0);
};

void DSelBG_2pip2pim::Get_ComboWrappers(void)
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

#endif // DSelBG_2pip2pim_h
