#ifndef DSelector_2eMissingProton_h
#define DSelector_2eMissingProton_h

#include <iostream>

#include "DSelector/DSelector.h"
#include "DSelector/DHistogramActions.h"
#include "DSelector/DCutActions.h"

#include "TH1I.h"
#include "TH2I.h"
#include "TH1D.h"
#include "TH2D.h"

class DSelector_2eMissingProton : public DSelector
{
	public:

		DSelector_2eMissingProton(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_2eMissingProton(){}

		void Init(TTree *tree);
		Bool_t Process(Long64_t entry);

	private:

		void Get_ComboWrappers(void);
		void Finalize(void);

		// BEAM POLARIZATION INFORMATION
		UInt_t dPreviousRunNumber;
		bool dIsPolarizedFlag; //else is AMO
		bool dIsPARAFlag; //else is PERP or AMO

		// ANALYZE CUT ACTIONS
		// // Automatically makes mass histograms where one cut is missing
		DHistogramAction_AnalyzeCutActions* dAnalyzeCutActions;

		//CREATE REACTION-SPECIFIC PARTICLE ARRAYS

		//Step 0
		DParticleComboStep* dStep0Wrapper;
		DBeamParticle* dComboBeamWrapper;
		DChargedTrackHypothesis* dPositronWrapper;
		DChargedTrackHypothesis* dElectronWrapper;
		DKinematicData* dMissingProtonWrapper;

		// DEFINE YOUR HISTOGRAMS HERE
		// EXAMPLES:
		TH1I* dHist_BeamEnergy;
		TH1I* dHist_MyCuts;
		TH1I* dHist_TaggerAccidentals;

		TH1D* dHist_KinFitChiSq;
		TH1D* dHist_KinFitCL;
		TH1D* dHist_KinFitCLlogx;

		TH1D* dHist_MissingMassSquared;
                TH1D* dHist_MissingMassSquared_Measured;
                TH1D* dHist_MissingMassSquared_Thrown;
                TH1D* dHist_MissingMassSquaredTK;
                TH1D* dHist_MissingMassSquaredMK;
                TH1D* dHist_MissingMassSquaredTM;

		TH1D* dHist_MissingMass;
                TH1D* dHist_MissingMass_Measured;
                TH1D* dHist_MissingMass_Thrown;
                TH1D* dHist_MissingMass_TK;
                TH1D* dHist_MissingMass_TM;
                TH1D* dHist_MissingMass_MK;

		TH1D* dHist_MissingEnergy;
                TH1D* dHist_MissingEnergy_Thrown;
                TH1D* dHist_MissingEnergy_Measured;
                TH1D* dHist_MissingEnergyTK;
                TH1D* dHist_MissingEnergyTM;
                TH1D* dHist_MissingEnergyMK;

		//Histograms about 2pi system                                   
                TH1D* dHist_M2ekin;
		TH1D* dHist_M2ekin_b4BCALyP3;
		TH1D* dHist_M2ekin_b4P3;
                TH1D* dHist_M2egen;
                TH1D* dHist_M2e_Measured;
                TH1D* dHist_M2eTK;
                TH1D* dHist_M2eTM;
                TH1D* dHist_M2eMK;
		TH2D* dHist_M2eTK_vs_M2egen;
		
                //diff is kin -  gen or TK 
		
		TH1D* dHist_t_transverse;

		TH1D* dHist_tkin;
                TH1D* dHist_tgen;
                TH1D* dHist_t_measured;
                TH1D* dHist_tTK;
                TH1D* dHist_tmeas;
                TH1D* dHist_tTM;
                TH2D* dHist_tkin_tmeasured;
                TH2D* dHist_tgen_tmeasured;

		TH2D* dHist_tgen_vs_tresTM;
		TH2D* dHist_tgen_vs_tresTK;
		TH2D* dHist_tgen_vs_tresTMoT;

		TH1D* dHist_Energy_FCAL;
		TH1D* dHist_Energy_FCALbyPeak;
		TH1D* dHist_FCAL_Elasticity_b4BCALyP3;
		TH1D* dHist_FCAL_Elasticity_b4P3;
		TH2D* dHist_Energy_FCALbyPeak_vs_EoverP_electron;
		TH2D* dHist_Energy_FCALbyPeak_vs_electronP;
		TH1D* dHist_EoverP_Positron;
		TH1D* dHist_EoverP_Electron;
		TH1D* dHist_PoverE_Electron;
		TH1D* dHist_PoverE_Positron;


		TH1D* dHist_eplusMomentum;
		TH1D* dHist_eminusMomentum;
		TH2D* dHist_epMomentum_VS_theta;

		TH1D* dHist_Theta1;
		TH1D* dHist_Theta2;
		TH2D* dHist_Theta1TK_vs_Theta1T;
		TH2D* dHist_Theta2TK_vs_Theta2T;
		TH1D* dHist_ep_phi;
		TH1D* dHist_em_phi;

		TH1D* dHist_E2e;
		TH1D* dHist_E2e_Measured;
		TH1D* dHist_E2e_Thrown;
		TH1D* dHist_E2e_TM;
		TH1D* dHist_E2e_TK;

		TH1D* dHist_Jphi;
		TH1D* dHist_Jphi_Thrown;
		TH1D* dHist_phi;
		TH2D* dHist_Jphi_VS_phi;
		TH2D* dHist_JphiTK_vs_JphiThrown;


		TH1D* dHist_Energy_UnusedShowers;
		TH2D* dHist_EUnSh_VS_Elas;
		TH2D* dHist_EUnSh_VS_W;
		TH2D* dHist_EUnSh_VS_Elas_NZb4P3;
		TH2D* dHist_EUnSh_VS_W_NZb4P3;
		TH2D* dHist_M2ekin_VS_NumUnusedTracks;
		TH2D* dHist_Elasticity_VS_NumUnusedTracks;
		TH1I* dHist_NumUnusedTracks;
		
		TH1D* dHist_EoverP_Positron_Measured;
		TH1D* dHist_EoverP_Electron_Measured;
		TH1D* dHist_ElasticityKinFit;
		TH1D* dHist_ElasticityMeasured;
		
		TH2D* dHist_Energy_FCALbyPeak_vs_EoverP_electronKF;
		TH2D* dHist_Energy_FCALbyPeak_vs_EoverP_electronMeas;
		TH2D* dHist_EoverP_vs_P_ElectronMeas;
		TH2D* dHist_EoverP_vs_P_Electron;
		
		TH2D* dHist_Elasticity_VS_PoverEtot;
		TH1D* dHist_Elas_PoE_rotate;
		TH1D* dHist_PoEtot;
		TH1D* dHist_Ilia;

		TH2D* dHist_EoP1_VS_EoP2;
		TH1D* dHist_EoP1_Plus_EoP2;
		TH2D* dHist_Elas_VS_EoP1;
		TH2D* dHist_Elas_VS_EoP2;
		TH2D* dHist_Elas_VS_EoP1_Plus_EoP2;

	        TH1D* dHist_Elas_EoP1_rotate;
		TH1D* dHist_Elas_EoP2_rotate;
		TH1D* dHist_Elas_EoP1_Plus_EoP2norm_rotate;

		TH1D* dHist_EoP2_chop1;
		TH1D* dHist_EoP2_chop2;
		TH1D* dHist_EoP2_chop3;
		TH1D* dHist_EoP2_chop4;
		TH1D* dHist_EoP2_chop5;

		TH1D* dHist_DetectorTiming_ep;
		TH1D* dHist_DetectorTiming_em;

		TH2D* dHist_MissingProtonP3Mag_VS_W;

		TH2D* dHist_MissingProtonP3Mag_VS_SumPMag_UnusedTracks;
		TH2D* dHist_MissingProtonTheta_VS_W;
		TH2D* dHist_MissingProtonPhi_VS_W;

		TH2D* dHist_UnusedTrackP3Mag_VS_W;
		TH2D* dHist_UnusedTrackTheta_VS_W;
		TH2D* dHist_UnusedTrackTheta_VS_MissingProtonTheta;

		TH1D* dHist_SumP3Mag_UnusedTracks;
		TH1D* dHist_P_unused;
		TH1D* dHist_SumP3Mag_OneUnusedTrack;
		TH1D* dHist_UnusedTrackTheta;
		TH1D* dHist_UnusedTrackThetaRad;
	     
		TH1D* dHist_UnusedTrackPX;
		TH1D* dHist_UnusedTrackPY;
		TH1D* dHist_UnusedTrackPZ;



		TH1D* dHist_ShowerQuality_ep;


		TH1D* dHist_dEdx_TOF_ep;
		TH1D* dHist_dEdx_TOF_em;

		TH2D* dHist_dEdx_TOF_VS_P_Positron;
		TH2D* dHist_dEdx_TOF_VS_P_Electron;
		TH2D* dHist_dEdx_TOF_ep_VS_P_missp;
		TH2D* dHist_dEdx_TOF_em_VS_P_missp;
		TH2D* dHist_dEdx_TOF_all_VS_P_missp;
		TH2D* dHist_dEdx_TOF_ep_VS_W;
		TH2D* dHist_dEdx_TOF_em_VS_W;
		TH2D* dHist_dEdx_TOF_all_VS_W;
     	
		TH2D* dHist_dEdx_TOF_ep_VS_P_unused;
		TH2D* dHist_dEdx_TOF_em_VS_P_unused;
		TH2D* dHist_dEdx_TOF_all_VS_P_unused;

		TH1D* dHist_dEdx_CDC_ep;
		TH1D* dHist_dEdx_CDC_em;

		TH2D* dHist_dEdx_CDC_Pos_VS_P_Positron;
		TH2D* dHist_dEdx_CDC_Elec_VS_P_Electron;
		TH2D* dHist_dEdx_CDC_All_VS_P_Positron;
		TH2D* dHist_dEdx_CDC_Pos_VS_P_MissP;
		TH2D* dHist_dEdx_CDC_em_VS_P_missp;
		TH2D* dHist_dEdx_CDC_VS_MissingProtonP3Mag;

		TH2D* dHist_dEdx_CDC_ep_VS_P_unused;
		TH2D* dHist_dEdx_CDC_em_VS_P_unused;
		TH2D* dHist_dEdx_CDC_all_VS_P_unused;

		TH1D* dHist_dEdx_FDC_ep;
		TH1D* dHist_dEdx_FDC_em;

		TH2D* dHist_dEdx_FDC_ep_VS_P_ep;
		TH2D* dHist_dEdx_FDC_em_VS_P_em;
		TH2D* dHist_dEdx_FDC_all_VS_P_missp;
		TH2D* dHist_dEdx_FDC_all_VS_P_ep;
		TH2D* dHist_dEdx_FDC_ep_VS_P_missp;	       
		TH2D* dHist_dEdx_FDC_em_VS_P_missp;
	
		TH2D* dHist_dEdx_FDC_ep_VS_P_unused;
		TH2D* dHist_dEdx_FDC_em_VS_P_unused;
		TH2D* dHist_dEdx_FDC_all_VS_P_unused;

		TH1D* dHist_dEdx_ST_ep;
		TH1D* dHist_dEdx_ST_em;

		TH2D* dHist_dEdx_ST_ep_VS_P_ep;
		TH2D* dHist_dEdx_ST_em_VS_P_em;
		TH2D* dHist_dEdx_ST_all_VS_P_missp;
		TH2D* dHist_dEdx_ST_all_VS_P_ep;
		TH2D* dHist_dEdx_ST_ep_VS_P_missp;
		TH2D* dHist_dEdx_ST_em_VS_P_missp;

		TH2D* dHist_dEdx_ST_ep_VS_P_unused;
		TH2D* dHist_dEdx_ST_em_VS_P_unused;
		TH2D* dHist_dEdx_ST_all_VS_P_unused;

		TH1D* dHist_TrackFCAL_DOCA_Charged;
		TH1D* dHist_TrackFCAL_DOCA_ep;
		TH1D* dHist_TrackFCAL_DOCA_em;
		TH1D* dHist_TrackFCAL_DOCA_Neutral;
		TH2D* dHist_TrackFCAL_DOCA_Charged_VS_W;
		TH2D* dHist_TrackFCAL_DOCA_ep_VS_W;
		TH2D* dHist_TrackFCAL_DOCA_em_VS_W;
		TH2D* dHist_TrackFCAL_DOCA_Neutral_VS_W;

		TH2D* dHist_E9E25_FCAL_all_VS_W;
		TH2D* dHist_E9E25_FCAL_ep_VS_W;
		TH2D* dHist_E9E25_FCAL_em_VS_W;

		TH2D* dHist_Elasticity_VS_W;
		TH1D* dHist_OpeningAngle;
		TH2D* dHist_OpeningAngle_VS_W;
		TH2D* dHist_t_VS_W;
		


		Double_t dMinKinFitCL;
                Double_t dMaxKinFitChiSq;
                Double_t dMinBeamEnergy;
                Double_t dMaxBeamEnergy;
                Double_t dMin2eMass;
                Double_t dMax2eMass;
                Double_t dMinMissingMassSquared;
                Double_t dMaxMissingMassSquared;
		Double_t dMinPositronP3;
		Double_t dMinElectronP3;
		Double_t dMinPoverE;
		Double_t dMaxPoverE;
		Double_t dMinEoverP;
		Double_t dMinEoverPkin;
		Double_t dMaxEoverPkin;
		Double_t dMinEoverPkin2;
                Double_t dMaxEoverPkin2;

		Double_t dMaxTOF_dEdx_ep;
		Double_t dMaxTOF_dEdx_em;

		Double_t dMaxFCAL_DOCA_em;
		Double_t dMaxFCAL_DOCA_ep;
		Double_t dMaxMissingProtonP3Mag;
		Double_t dMaxFCALElasticity;
		Int_t dMaxNumUnusedTracks;

		//Andrew's Cut Options
		Int_t ABeamCut;
		Int_t AKinFitCLCut;
		Int_t AMissingMassSqCut;
		Int_t AInvariantMassCut;
		Int_t ATOF2HitCut;
		Int_t ATOFdEdxPositronCut;
		Int_t ATOFdEdxElectronCut;
		Int_t AFCAL2HitCut;
		Int_t AFCAL2TrackEoverPmeasCut;
		Int_t AFCALE1overP1kinCut;
		Int_t AFCALE2overP2kinCut;
		Int_t AFCALElasticityCut;
		Int_t ADOCAPositronCut;
		Int_t ADOCAElectronCut;
		Int_t AUnusedShowerEnergyCut;

		// Constants                                                    
                Double_t PI = 3.14159;
                Double_t MProton = 0.93827231;  //(GeV/c^2)   
		Double_t ElectronMass = 0.000511;
	ClassDef(DSelector_2eMissingProton, 0);
};

void DSelector_2eMissingProton::Get_ComboWrappers(void)
{
	//Step 0
	dStep0Wrapper = dComboWrapper->Get_ParticleComboStep(0);
	dComboBeamWrapper = static_cast<DBeamParticle*>(dStep0Wrapper->Get_InitialParticle());
	dPositronWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(0));
	dElectronWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(1));
	dMissingProtonWrapper = dStep0Wrapper->Get_FinalParticle(2);
}

#endif // DSelector_2eMissingProton_h
