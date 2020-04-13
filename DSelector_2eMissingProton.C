#include "DSelector_2eMissingProton.h"

void DSelector_2eMissingProton::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
        dOutputFileName = "2eMissingProton_4_2020_And_10milMC.root"; //"" for none
	dOutputTreeFileName = ""; //"" for none
	dFlatTreeFileName = "";// "Flat_2eMissingProton_HILO_2017JustE1P1cut.root"; //output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = ""; //if blank, default name will be chosen

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
	//std::deque<Particle_t> MyPhi;
	//MyPhi.push_back(KPlus); MyPhi.push_back(KMinus);

	//ANALYSIS ACTIONS: //Executed in order if added to dAnalysisActions
	//false/true below: use measured/kinfit data

	//PID
	//	dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, false)); //This was DEFAULT setting. I copied from pi file whats below
        dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, false)); //false: use measured data                                                  
        dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, true, "KinFit")); //true: use kinfit data 
	//below: value: +/- N ns, Unknown: All PIDs, SYS_NULL: all timing systems
	//dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.5, KPlus, SYS_BCAL));

	//MASSES
	//dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, Lambda, 1000, 1.0, 1.2, "Lambda"));
	//dAnalysisActions.push_back(new DHistogramAction_MissingMassSquared(dComboWrapper, false, 1000, -0.1, 0.1));

	//KINFIT RESULTS
	dAnalysisActions.push_back(new DHistogramAction_KinFitResults(dComboWrapper));

	//CUT MISSING MASS
	//dAnalysisActions.push_back(new DCutAction_MissingMassSquared(dComboWrapper, false, -0.03, 0.02));

	//BEAM ENERGY
	dAnalysisActions.push_back(new DHistogramAction_BeamEnergy(dComboWrapper, false));
	//dAnalysisActions.push_back(new DCutAction_BeamEnergy(dComboWrapper, false, 8.4, 9.05));

	//KINEMATICS
	dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, false));

	// ANALYZE CUT ACTIONS
	 // Change MyPhi to match reaction
	//dAnalyzeCutActions = new DHistogramAction_AnalyzeCutActions( dAnalysisActions, dComboWrapper, false, 0, MyPhi, 1000, 0.9, 2.4, "CutActionEffect" );

	//INITIALIZE ACTIONS
	//If you create any actions that you want to run manually (i.e. don't add to dAnalysisActions), be sure to initialize them here as well
	Initialize_Actions();
	//	dAnalyzeCutActions->Initialize(); // manual action, must call Initialize()

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/

	//EXAMPLE MANUAL HISTOGRAMS:
       
	dHist_BeamEnergy = new TH1I("BeamEnergy", ";Beam Energy (GeV)", 600, 0.0, 12.0);
	
	dHist_MyCuts = new TH1I("MyCutActions", "Cut Numbers", 21, 0, 20);
	dHist_TaggerAccidentals = new TH1I("dHist_TaggerAccidentals", "Vertex time - RF (ns)", 400,-20,20);
		
	dHist_MissingMassSquared = new TH1D("MissingMassSquared", ";Missing Mass Squared (GeV/c^{2})^{2}", 600, -1, 5);
	dHist_MissingMassSquared_Measured = new TH1D("MissingMassSquaredMeasured", ";Missing Mass Squared  (GeV/c^{2})^{2}", 600, -1, 5);
	
	/*dHist_MissingMassSquared_Thrown = new TH1D("MissingMassSquaredThrown" , ";Missing Mass Squared (GeV/c^{2})^{2}", 300, -1, 5);
        dHist_MissingMassSquaredTK = new TH1D("MissingMassSquaredTK", ";#Delta Missing Mass Squared  (GeV/c^{2})^{2}", 600, -1, 5);
        dHist_MissingMassSquaredMK = new TH1D("MissingMassSquaredMK", "; #Delta Missing Mass Squared  (GeV/c^{2})^{2}", 600, -1, 5);
        dHist_MissingMassSquaredTM = new TH1D("MissingMassSquaredTM", ";#Delta Missing Mass Squared (GeV/c^{2})^{2}", 600, -5, 1);
	*/
	dHist_MissingMass = new TH1D("MissingMassKin", "; Missing Mass (GeV/c^{2})", 200, .936, .94);
        dHist_MissingMass_Measured = new TH1D("MissingMassMeas", "; Missing Mass (GeV/c^{2})", 200, -0.8, 2.2);
	//dHist_MissingMass_Thrown = new TH1D("MissingMassThrown", "; Missing Mass (GeV/c^{2})", 200, 0.936, .94);

	dHist_MissingEnergy = new TH1D("MissingEnergy", ";Missing Energy (GeV)", 200, -0.1, 0.7);
        dHist_MissingEnergy_Measured = new TH1D("MissingEnergyMeasured", ";Missing Energy (GeV)", 200, 0, 3);
        dHist_MissingEnergyMK = new TH1D("MissingEnergyMK", ";Missing Energy (GeV)", 200, 0, 2.5);
	/*
	dHist_MissingEnergy_Thrown = new TH1D("MissingEnergyThrown", ";Missing Energy (GeV)", 200, 0.9, 1.6);
	dHist_MissingEnergyTK = new TH1D("MissingEnergyTK", ";Missing Energy (GeV)", 200, 0.8, 1.1);
        dHist_MissingEnergyTM = new TH1D("MissingEnergyTM", ";Missing Energy (GeV)", 200, -2.5, 1);
	*/

	dHist_E2e = new TH1D("2e pair total energy kin fit", "; Energy (GeV)", 300,0,10);
	dHist_E2e_Measured = new TH1D("e+/e- pair total energy Measured", "; Energy (GeV)", 300,0,10);
	/*
	dHist_E2e_Thrown = new TH1D("2e pair total energy Thrown", "; Energy (GeV)", 300,0,10);
	dHist_E2e_TM = new TH1D("total energy 2e Thrown - Measured", "; Energy (GeV)", 300,0,10);
	dHist_E2e_TK = new TH1D("total energy 2e Thrown - Kin", "; Energy (GeV)", 300,0,10);
	*/

	
        //Mass of the 2 e system                                                                                                                                      
        dHist_M2ekin = new TH1D("M2ekin", ";M_{e^{+}e^{-}} Kin (GeV/c^{2})", 238, 0.0, 1.4); 
        dHist_M2e_Measured = new TH1D("M2eMeas", ";M_{e^{+}e^{-}} Gen (GeV/c^{2})", 238, 0.0, 1.4);
        dHist_M2eMK = new TH1D("M2eMK", ";M_{e^{+}e^{-}} Kin - Gen (GeV/c^{2})", 238, -0.15, 0.15);
       
	dHist_M2egen = new TH1D("M2egen", ";M_{e^{+}e^{-}} Gen (GeV/c^{2})", 200, 0.0, 1.4);
	dHist_M2eTK = new TH1D("M2eTK", ";M_{e^{+}e^{-}} Kin - Gen (GeV/c^{2})", 200, -0.05, 0.05);
        dHist_M2eTM = new TH1D("M2eTM", ";M_{e^{+}e^{-}} Kin - Gen (GeV/c^{2})", 200, -0.05, 0.05);
       
	dHist_M2eTK_vs_M2egen = new TH2D("M2eTK_vs_M2egen", ";GeV/c^{2} ;M_{e^{+}e^{-}} Kin - Gen (GeV/c^{2})", 238, 0.0, 1.4, 100, -0.05, 0.05);
	

	//	dHist_Theta_epem = new TH1D("Theta_epem", "Theta between 2 tracks in deg", 182, 0, 181);
	//	dHist_Dist_epem = new TH1D("Dist_epem", "Dist between 2 tracks (cm)", 200, 0, 20);
	

	dHist_Theta1 = new TH1D("Theta1", ";Lab Theta (deg)", 120, 0, 20);
	dHist_Theta2 = new TH1D("Theta2", ";Lab Theta (deg)", 120, 0, 20);
	dHist_Theta1TK_vs_Theta1T = new TH2D("ThetaTK1_vs_Theta1Thrown", ";Lab Theta Thrown (deg) ;theta res (thrown - kin)", 120, 0, 20, 100, -0.5, 0.5);
	dHist_Theta2TK_vs_Theta2T = new TH2D("ThetaTK2_vs_Theta2Thrown", ";Lab Theta Thrown (deg) ;theta res (thrown - kin)", 120, 0, 20, 100, -1, 1);


	//FCAL
	dHist_Energy_FCAL = new TH1D("Sumof2eTracksFCAL", ";Energy (GeV)", 300, 0, 10);
	dHist_Energy_FCALbyPeak = new TH1D("FCAL_Elasticity", ";(FCAL_1 + FCAL_2)/BeamE", 300, 0.4, 2.0);
	//here
	dHist_ElasticityKinFit = new TH1D("Elasticity_KinFit", ";KinFit(E_1 + E_2)/BeamE", 300, 0, 1.5);
	dHist_ElasticityMeasured = new TH1D("Elasticity_Measured", ";Measured(E_1 + E_2)/BeamE", 300, 0, 1.5);
	//here
	dHist_Energy_FCALbyPeak_vs_EoverP_electronKF = new TH2D("FCALElas_VS_EoverPElectronKF", ";(FCAL_1 +FCAL_2)/BeamE ;Electron E over PKin", 200,0,1.5,200,0,3);
	dHist_Energy_FCALbyPeak_vs_EoverP_electronMeas = new TH2D("FCALElas_VS_EoverPElectronMeas", ";(FCAL_1 +FCAL_2)/BeamE ;Electron E over PMeas", 200,0,1.5,200,0,3);
	//Nothere
	dHist_EoverP_Electron = new TH1D("EFCAL_over_P_electron", ";FCAL Energy/Track Momentum", 100,0.47,1.97);
	dHist_EoverP_Positron = new TH1D("EFCAL_over_P_positron", ";FCAL Energy/Track Momentum", 200,-0.1,2);
	//here
	dHist_EoverP_vs_P_Electron = new TH2D("EoverP_vs_P_Electron", ";KinFit p_{e-} (GeV) ;E FCAL/KinFit p_{e-}", 200,0,9,200,-0.1,2);
	dHist_EoverP_vs_P_ElectronMeas = new TH2D("EoverP_vs_P_ElectronMeas", ";Meas p_{e-} (GeV; E FCAL/Meas p_{e-}", 200,0,9,200,-0.1,2);

	dHist_EoverP_Electron_Measured = new TH1D("EFCAL_over_P_electron_meas", ";FCAL Energy/Track Momentum", 200,-0.1,2);
	dHist_EoverP_Positron_Measured = new TH1D("EFCAL_over_P_positron_meas", ";FCAL Energy/Track Momentum", 200,-0.1,2);
	
//nothere
	dHist_PoverE_Electron = new TH1D("p/E_Electron", ";Track Momentum/FCAL Energy", 400,-0.1,2);
	dHist_PoverE_Positron = new TH1D("p/E_Positron", ";Track Momentum/FCAL Energy", 400, -0.1,2);

	dHist_Energy_FCALbyPeak_vs_electronP = new TH2D("FCALElas_VS_electronP", ";(FCAL_1 + FCAL_2)/BeamE ;Electron Momentum", 200,0,1.1,200,0,9);


	dHist_KinFitChiSq = new TH1D("KinFitChiSq", ";Kinematic Fit #chi^{2}/NDF", 250, 0., 25.);
        dHist_KinFitCL = new TH1D("KinFitCL", ";Kinematic Fit Confidence Level", 100, 0., 1.);
        dHist_KinFitCLlogx = new TH1D("KinFitCLlogx", ";log10 of Kinematic Fit Confidence Level", 100, -20., 1.);

	dHist_Energy_UnusedShowers = new TH1D("Energy_UnusedShowers", ";Energy (GeV)", 400,0,10);
	dHist_EUnSh_VS_Elas = new TH2D("Elas_EUnusedShowers", ";E Unused Showers (GeV) ;Elasticity", 300,0,5,300,0,1.1);
	dHist_EUnSh_VS_W = new TH2D("W_EUnusedShowers", ";E Unused Showers (GeV) ;Invariant Mass (GeV/c^{2})", 300, 0,5, 238,0,1.4);

	dHist_M2ekin_VS_NumUnusedTracks = new TH2D("NumUnusedTracks_vs_M2ekin", ";Invariant Mass (GeV/c^{2}) ;NumUnusedTracks", 238, 0, 1.4, 5, 0, 4);
	//	dHist_M2ekin_VS_NumUnusedTracks->Draw("LEGO1");
	dHist_M2ekin_VS_NumUnusedTracks->GetYaxis()->SetNdivisions(4);
	dHist_NumUnusedTracks = new TH1I("NumUnusedTracks",";NumUnusedTracks", 5,0,4);
	dHist_NumUnusedTracks->GetXaxis()->SetNdivisions(4);
	dHist_Elasticity_VS_NumUnusedTracks = new TH2D("Elasticity_VS_NumUnusedTracks", ";Elasticity ;NumUnusedTracks", 300, 0, 1.5, 5,0,4);
	dHist_Elasticity_VS_NumUnusedTracks->GetYaxis()->SetNdivisions(4);

	dHist_Elasticity_VS_PoverEtot = new TH2D("FCALElasticity_VS_PoverEtot", ";(P_{1} + P_{2})/(E_{1} + E_{2}) ;(E_{1} + E_{2})/E_{#gamma}", 300, 0, 2, 300, 0, 1.5);
	dHist_Elas_PoE_rotate = new TH1D("Elas_PoE_rotate", ";2^{-1/2}((P_{1} + P_{2})/(E_{1} + E_{2}) - (E_{1} + E_{2})/E_{#gamma})", 300, -2, 2);
	dHist_PoEtot = new TH1D("PoEtot", ";(P_{1} + P_{2})/(E_{1} + E_{2})", 300, 0, 2);

	dHist_EoP1_VS_EoP2 = new TH2D("EoP1_VS_EoP2", "; E_{1}/p_{1} ;E_{2}/p_{2}", 300,0.7,1.5,300,0.7,1.5);
	dHist_EoP1_Plus_EoP2 = new TH1D("EoP1_Plus_Eop2", "; E_{1}/p_{1} + E_{2}/p_{2}", 300, 1.5, 2.5);
	dHist_Elas_VS_EoP1 = new TH2D("Elas_VS_EoP1", "; E_{1}/p_{1} ;FCAL Elasticity", 300, 0.7, 1.5, 300, 0.6, 1.5);
	dHist_Elas_VS_EoP2 = new TH2D("Elas_VS_EoP2", "; E_{2}/p_{2} ;FCAL Elasticity", 300, 0.7, 1.4, 300, 0.6, 1.5);
	dHist_Elas_VS_EoP1_Plus_EoP2 = new TH2D("Elas_VS_EoP1plus2", ";E_{1}/p_{1} + E_{2}/p_{2} ; FCAL Elasticity", 300, 1.5, 3, 300, 0.6, 1.5);

	dHist_Elas_EoP1_rotate = new TH1D("Elas_EoP1_rotate", ";2^{-1/2}( E_{1}/p_{1}n_{1} - FCAL Elas/n_{3})", 300, -0.4, 0.4);
	dHist_Elas_EoP2_rotate = new TH1D("Elas_EoP2_rotate", ";2^{-1/2}(E_{2}/p_{2}n_{2} - FCAL Elas/n_{3})", 300, -0.4, 0.4);
	dHist_Elas_EoP1_Plus_EoP2norm_rotate = new TH1D("Elas_EoP1_Plus_EoP2norm_rotate", ";2^{-1/2}( E_{1}/p_{1}n_{1} + E_{2}/p_{2}n_{2} - FCAL Elas/n_{3})", 300, -0.4, 0.4);


	dHist_EoP2_chop1 = new TH1D("EoP2_chop1",";FCAL energy/kinFit P", 238, 0, 1.6); 
	dHist_EoP2_chop2 = new TH1D("EoP2_chop2",";FCAL energy/kinFit P", 238, 0, 1.6);
	dHist_EoP2_chop3 = new TH1D("EoP2_chop3",";FCAL energy/kinFit P", 238, 0, 1.6);
	dHist_EoP2_chop4 = new TH1D("EoP2_chop4",";FCAL energy/kinFit P", 238, 0, 1.6);
	dHist_EoP2_chop5 = new TH1D("EoP2_chop5",";FCAL energy/kinFit P", 238, 0, 1.6);




	dHist_OpeningAngle = new TH1D("OpeningAngle", ";opening angle of e+ e- tracks (deg)", 30, 0, 29);
	dHist_OpeningAngle_VS_W = new TH2D("OpeningAngle_VS_W", ";W (GeV/c^{2}) ;opening angle of e+ e- tracks (deg)", 238, 0, 1.4, 30, 0, 29);
       	dHist_Elasticity_VS_W = new TH2D("Elasticity_VS_W", ";Invariant Mass ;FCAL Elasticity", 238, 0, 1.4, 238, 0.6, 1.5);

	dHist_ShowerQuality_ep = new TH1D("Neutral_Shower_Quality", ";Neutral Shower Quality", 300, 0, 1.1);

	/*
	
	//MOMENTA                                                                                                                                                      
	dHist_2eMomentum = new TH1I("2eMomentumkin", ";Summed Momentum magnitude (GeV)", 200, 7.5, 9);
	dHist_2eMomentum_Thrown = new TH1I("2eMomentumThrown", ";Summed Momentum magnitude (GeV)", 200, 7.5, 9);
	dHist_2eMomentum_Measured = new TH1I("2eMomentumMeas", ";Summed Momentum magnitude (GeV)", 200,0,16);                                                      
	dHist_2eMomentumTKoT = new TH1I("2eMomentumTKoT", ";(Thrown - Kin)/Thrown ", 100, -0.5, 0.5);
        dHist_2eMomentumTMoT = new TH1I("2eMomentumTMoT", ";(Thrown - Meas)/Thrown ", 100, -5,5);                                                                  
        //eplus                                                                                                                                                       

	*/
        //dHist_missingProtonM2 = new TH1I("missingProtonM2", ";proton mag squared (GeV^{2})",200,0,9);

        dHist_eplusMomentum = new TH1D("eplusMomentumkin", ";e^{+} Momentum (GeV)", 200, 0.0, 9);
	dHist_eminusMomentum = new TH1D("eminusMomentumkin", ";e^{-} Momentum (GeV)", 200, 0.0, 9);
	
	dHist_epMomentum_VS_theta = new TH2D("eplusMomentumVStheta", ";e^{+} theta in degrees ;e^{+} momentum magnitude", 200,0,20,200,0.0,9);
	
	dHist_ep_phi = new TH1D("ep_phi", ";e+ lab phi (deg)", 180, -180, 180);
	dHist_em_phi = new TH1D("em_phi", ";e- lab phi (deg)", 180, -180, 180);


	//        dHist_eplusMomentum_Thrown = new TH1I("eplusMomentumThrown", ";e^{+} Momentum (GeV)", 200, 0.0, 9);
	//        dHist_eminusMomentum_Thrown = new TH1I("minusMomentumThrown", ";e^{-} Momentum (GeV)", 200, 0.0, 9);

	//        dHist_eplusMomentum_Measured = new TH1I("plusMomentumMeasured", ";e^{+} Momentum Measured (GeV)", 200, 0.0, 9);
	//        dHist_eminusMomentum_Measured = new TH1I("eminusMomentumMeasured", ";e^{-} Momentum Measured (GeV)",200, 0.0, 9);

	/*
        dHist_epTKoT = new TH1I("epTKoT","; #e^{+}: Thrown p - KinFit p/Thrown p",300,-0.4,0.4);
        dHist_mTKoT = new TH1I("emTKoT","; e^{-}: Thrown p - KinFit p/ Thrown p",300,-0.4,0.4);

        dHist_epTMoT = new TH1I("epTMoT","; e^{+}: Thrown p - Measured p/Thrown p",300,-0.4,0.6);
        dHist_emTMoT = new TH1I("emTMoT","; e^{-}: Thrown p - Measured p/ Thrown p",300,-0.4,0.6);

        dHist_epMK = new TH1I("epMK","; e^{+}: Measured p - Kin p",300,-2,1);
        dHist_emMK =  new TH1I("emMK","; e^{-}: Measured p - Kin p",300,-2,1);


        dHist_Elasticity_Measured = new TH1I("ElasticityMeas", ";E_{#gamma} - E_{e +} - E_{e -}/ E_{#gamma}",200,-0.15,0.3);
        dHist_Elasticity_Thrown = new TH1I("ElasticityThrown", ";E_{#gamma} - E_{e +} - E_{e -}/ E_{#gamma} ",200,-0.002,0.07);
        dHist_ElasticityTMoT = new TH1I("ElasticityTMoT", ";(Thrown-Meas)/Thrown",200,-10,10);
        dHist_epemETMoT = new TH1I("epemETMoT",";(Thrown-Meas)/Thrown",200,-0.1,0.3);
        dHist_eminusETMoT = new TH1I("emETMoT",";(Thrown-Meas)/Thrown",200,-0.4,0.4);
        dHist_eplusETMoT = new TH1I("epETMoT", ";(Thrown-Meas)/Thrown ", 200, -0.4, 0.4);
	*/
	dHist_Ilia = new TH1D("ilia", ";p_{1} + p_{2}/E_{#gamma}", 300,0,1.5);
	dHist_t_transverse = new TH1D("transverse momentum transfer", ";|k_t|", 600,0.003, .15);

	dHist_MissingProtonP3Mag_VS_W = new TH2D("MissProtP3_VS_W", ";W ;Proton P Magnitude (GeV)", 238, 0, 1.4, 300, -0.1, 2);
	dHist_MissingProtonTheta_VS_W= new TH2D("MissProtTheta_VS_W", ";W ;Calculated Missing Proton Theta (deg)", 238,0,1.4,100,0,100);
	dHist_MissingProtonPhi_VS_W= new TH2D("MissProtPhi_VS_W", ";W ;Calculated Missing Proton Phi (deg)", 238,0,1.4,180,-180,180);

	dHist_UnusedTrackP3Mag_VS_W = new TH2D("UnusedTrackP3Mag_VS_W", ";W (GeV/c^{2}) ; P Magnitude of Unused Track", 238, 0, 1.4, 200,  0, 2);
	dHist_UnusedTrackTheta_VS_W = new TH2D("UnusedTrackTheta_VS_W", ";W (GeV/c^{2}) ; Unused Track theta (deg)", 238, 0, 1.4, 90, 0, 180);
	dHist_UnusedTrackTheta_VS_MissingProtonTheta = new TH2D("UnusedTheta_VS_misspTheta", ";Missing proton theta (deg) ;Unused track theta (deg)", 90, 0, 180, 90, 0, 180);

	dHist_UnusedTrackPX = new TH1D("UnusedTrackPX", ";Momentum x component (GeV/c)", 100, -0.5, 0.5);
	dHist_UnusedTrackPY = new TH1D("UnusedTrackPY", ";Momentum y component (GeV/c)", 100, -0.5, 0.5);
	dHist_UnusedTrackPZ = new TH1D("UnusedTrackPZ", ";Momentum z component (GeV/c)", 100, 0, 1);

	dHist_MissingProtonP3Mag_VS_SumPMag_UnusedTracks = new TH2D("MissProtP3_VS_SumPMag_UnusedTracks", ";SumPMag_UnusedTracks (GeV) ;Proton P Magnitude (GeV)", 300,0,2,300,0,2);

	dHist_SumP3Mag_UnusedTracks = new TH1D("SumP3Mag_UnusedTracks", ";SumPMag_UnusedTracks (GeV)", 200, 0, 1);
	dHist_P_unused = new TH1D("P_unusedTrack", ";Magnitude of Momentum of Unused Track (GeV)", 200, 0, 1);
	dHist_SumP3Mag_OneUnusedTrack = new TH1D("P3Mag_OneUnusedTrack", ";P magnitude of Unused Track", 200, 0, 0.0000001);
	dHist_UnusedTrackTheta = new TH1D("UnusedTrackTheta", "Unused Track Theta (rad)", 180, 0, 4);
	dHist_UnusedTrackThetaRad = new TH1D("UnusedTrackThetaDeg", "Unused Track Theta (deg)", 180, 0, 180);

       	dHist_TrackFCAL_DOCA_Charged = new TH1D("TrackFCAL_DOCA_Charged", ";FCAL DOCA", 300,0,10);
	dHist_TrackFCAL_DOCA_ep = new TH1D("TrackFCAL_DOCA_ep", ";FCAL  DOCA", 300,0,10);
	dHist_TrackFCAL_DOCA_em = new TH1D("TrackFCAL_DOCA_em",";FCAL  DOCA", 300,0,10);
	dHist_TrackFCAL_DOCA_Neutral = new TH1D("TrackFCAL_DOCA_Neutral", ";FCAL DOCA",300,0,10);
	dHist_TrackFCAL_DOCA_Charged_VS_W = new TH2D("TrackFCAL_DOCA_Charged_VS_W", ";W ;FCAL DOCA", 238,0,1.4,238,0,10);
	dHist_TrackFCAL_DOCA_ep_VS_W = new TH2D("TrackFCAL_DOCA_ep_VS_W", ";W ;FCAL DOCA Positron", 238,0,1.4,238,0,10);
	dHist_TrackFCAL_DOCA_em_VS_W = new TH2D("TrackFCAL_DOCA_em_VS_W", ";W ;FCAL DOCA Electron", 238,0,1.4,238,0,10);
	dHist_TrackFCAL_DOCA_Neutral_VS_W = new TH2D("TrackFCAL_DOCA_Neutral_VS_W", ";W, ;FCAL DOCA", 238,0,1.4,238,0,10);
	// Distance of Closest approach to the nearest track. It's 999.0 if no tracks on FCAL.

       	dHist_E9E25_FCAL_all_VS_W = new TH2D("E9E25_FCAL_all", ";5x5 Sq. E Sum about Crystal w/ Shower, all ch.tr.", 238, 0, 1.4, 238, 0, 1.1);
       	dHist_E9E25_FCAL_ep_VS_W = new TH2D("E9E25_FCAL_ep",";W ;5x5 Sq. E Sum about Crystal w/ Shower, e+", 238, 0, 1.4, 238, 0, 1.1);
       	dHist_E9E25_FCAL_em_VS_W = new TH2D("E9E25_FCAL_em", ";W ;5x5 Sq. E Sum about Crystal w/ Shower, e-", 238, 0, 1.4, 238, 0, 1.1);

	//dE/dx should always be on the y axis for particle identification.

	dHist_DetectorTiming_ep = new TH1D("DetectorTiming_ep", ";Positron Hit Time", 200, 0, 1);
	dHist_DetectorTiming_em = new TH1D("DetectorTiming_em", ";Electron Hit Time", 200, 0, 1);

	dHist_dEdx_TOF_ep = new TH1D("dEdxTOFep", ";Positron dE/dx in TOF", 200, 0, 0.01);
	dHist_dEdx_TOF_em  = new TH1D("dEdxTOFem", ";Electron dE/dx in TOF", 200, 0, 0.01);

	dHist_dEdx_TOF_VS_P_Positron = new TH2D("dEdxTOFPos_VS_P_Positron", ";Positron Kin Fit P magnitude (GeV) ;dE/dx TOF Positron", 300, 0, 10, 300, 0, 0.01);
	dHist_dEdx_TOF_VS_P_Electron = new TH2D("dEdxTOFPos_VS_P_Electron", ";Electron Kin Fit P magnitude (GeV) ;dE/dx TOF Electron", 300, 0, 10, 300, 0, 0.01);

	dHist_dEdx_TOF_ep_VS_P_missp = new TH2D("dEdxTOFPos_VS_P_missp", ";Missing Proton P Magnitude (GeV) ;dE/dx TOF Positron", 300, 0, 2, 300, 0, 0.01);
	dHist_dEdx_TOF_em_VS_P_missp = new TH2D("dEdxTOFElec_VS_P_missp", ";Missing Proton P Magnitude (GeV) ;dE/dx TOF Electron", 300, 0, 2, 300, 0, 0.01);
        dHist_dEdx_TOF_all_VS_P_missp = new TH2D("dEdxTOFAll_VS_P_missp", ";Missing Proton P Magnitude (GeV) ;dE/dx TOF All", 300, 0, 2, 300, 0, 0.01);
        dHist_dEdx_TOF_ep_VS_W = new TH2D("dEdxTOFPos_VS_W", ";Invariant Mass (GeV/c^{2}) ;dE/dx TOF Positron", 238, 0, 1.4, 300, 0, 0.01);
        dHist_dEdx_TOF_em_VS_W = new TH2D("dEdxTOFElec_VS_W", ";Invariant Mass (GeV/c^{2}) ;dE/dx TOF Electron", 238, 0,1.4, 300, 0, 0.01);
	dHist_dEdx_TOF_all_VS_W = new TH2D("dEdxTOFAll_VS_W", ";Invariant Mass (GeV/c^{2}) ;dE/dx TOF All", 238, 0, 1.4, 300, 0, 0.01);

	dHist_dEdx_TOF_ep_VS_P_unused = new TH2D("dEdxTOFep_VS_P_unused", ";Unused Track Momentum Magnitude (GeV/c) ;dEdx TOF Positron",  200, 0, 1, 200, 0, 0.01);
	dHist_dEdx_TOF_em_VS_P_unused = new TH2D("dEdxTOFem_VS_P_unused", ";Unused Track Momentum Magnitude (GeV/c) ;dEdx TOF Electron", 200, 0, 1, 200, 0, 0.01);
	dHist_dEdx_TOF_all_VS_P_unused = new TH2D("dEdxTOFall_VS_P_unused", ";Unused Track Momentum Magnitude (GeV/c) ;dEdx TOF all charged tracks", 200, 0, 1, 300, 0, 0.01);
	
	dHist_dEdx_CDC_ep = new TH1D("dEdxCDCep", ";Positron dE/dx in CDC", 200, 0,25e-6);
	dHist_dEdx_CDC_em = new TH1D("dEdxCDCem", ";Electron dE/dx in CDC", 200, 0,25e-6);

	dHist_dEdx_CDC_Pos_VS_P_Positron = new TH2D("dEdxCDCPos_VS_P_Positron", ";Positron Kin Fit P magnitude (GeV) ;dE/dx CDC Positron", 300, 0, 10, 300, 0,25e-6);
	dHist_dEdx_CDC_Elec_VS_P_Electron = new TH2D("dEdxCDCElec_VS_P_Electron", ";Electron Kin Fit P magnitude (GeV) ;dE/dx CDC Electron", 300, 0, 10, 300, 0,25e-6);
	dHist_dEdx_CDC_All_VS_P_Positron = new TH2D("dEdxCDCAll_VS_P_Positron", ";Positron Kin Fit P magnitude (GeV) ;dE/dx CDC All", 300, 0, 10, 300, 0, 25e-6);
	dHist_dEdx_CDC_Pos_VS_P_MissP = new TH2D("dEdxCDCPos_VS_P_MissProtP", ";Missing Proton P Magnitude (GeV) ;dE/dx CDC Positron", 300, 0, 2, 300, 0, 25e-6);
	dHist_dEdx_CDC_em_VS_P_missp = new TH2D("dEdxCDCem_VS_P_missp", ";Missing Proton P Magnitude (GeV) ;dE/dx CDC Electron", 300, 0, 2, 300, 0, 25e-6);
	dHist_dEdx_CDC_VS_MissingProtonP3Mag = new TH2D("MissProtP3_VS_CDCdEdx", ";Proton P Magnitude (GeV) ;CDC dE/dx", 200,-0.1,2,200,0,25e-6);

	dHist_dEdx_CDC_ep_VS_P_unused = new TH2D("dEdxCDCep_VS_P_unused", ";Unused Track Momentum Magnitude (GeV/c) ;dEdx CDC Positron", 200, 0, 5, 200, 0,25e-6);
	dHist_dEdx_CDC_em_VS_P_unused = new TH2D("dEdxCDCem_VS_P_unused", ";Unused Track Momentum Magnitude (GeV/c) ;dEdx CDC Electron", 200, 0, 5, 200, 0,25e-6);
	dHist_dEdx_CDC_all_VS_P_unused = new TH2D("dEdxCDCall_VS_P_unused", ";Unused Track Momentum Magnitude (GeV/c) ;dEdx CDC all charged tracks", 200, 0, 5, 200, 0,25e-6);

	dHist_dEdx_FDC_ep = new TH1D("dEdxFDCep", ";Positron dE/dx in FDC", 200, 0, 4e-6);
	dHist_dEdx_FDC_em = new TH1D("dEdxFDCem", ";Electron dE/dx in FDC", 200, 0, 4e-6);

	dHist_dEdx_FDC_ep_VS_P_ep = new TH2D("dEdxFDCep_VS_epP", ";Positron Kin Fit P magnitude (GeV) ;dE/dx FDC Positron", 300, 0, 10, 300, 0, 4e-6);
	dHist_dEdx_FDC_em_VS_P_em = new TH2D("dEdxFDCem_VS_emP", ";Electron Kin Fit P magnitude (GeV) ;dE/dx FDC Electron", 300, 0, 10, 300, 0, 4e-6); 
	dHist_dEdx_FDC_all_VS_P_missp = new TH2D("dEdxFDCall_VS_misspP", ";Proton P Magnitude (GeV) ;dE/dx FDC charged hypos", 300, 0, 2, 300, 0, 4e-6);
	dHist_dEdx_FDC_all_VS_P_ep = new TH2D("dEdxFDCall_VS_epP", ";Positron Kin Fit P magnitude (GeV) ;dE/dx FDC charged hypos", 300, 0, 10, 300, 0, 4e-6);
	dHist_dEdx_FDC_ep_VS_P_missp = new TH2D("dEdxFDCep_VS_missP", ";Missing Proton P Magnitude (GeV) ;dE/dx FDC Positron", 300, 0, 2, 300, 0, 4e-6);
	dHist_dEdx_FDC_em_VS_P_missp = new TH2D("dEdxFDCem_VS_missP", ";Missing Proton P Magnitude (GeV) ;dE/dx FDC Electron", 300, 0, 2, 300, 0, 4e-6);

	dHist_dEdx_FDC_ep_VS_P_unused = new TH2D("dEdxFDCep_VS_P_unused", ";Unused Track Momentum Magnitude (GeV/c) ;dEdx FDC Positron", 200, 0, 5, 200, 0, 4e-6);
	dHist_dEdx_FDC_em_VS_P_unused = new TH2D("dEdxFDCem_VS_P_unused", ";Unused Track Momentum Magnitude (GeV/c) ;dEdx FDC Electron", 200, 0, 5, 200, 0, 4e-6);
	dHist_dEdx_FDC_all_VS_P_unused = new TH2D("dEdxFDCall_VS_P_unused", ";Unused Track Momentum Magnitude (GeV/c) ;dEdx FDC all charged tracks", 200, 0, 5, 200, 0, 4e-6);

	dHist_dEdx_ST_ep = new TH1D("dEdxSTep", ";Positron dE/dx in ST", 200, 0,  0.015);
	dHist_dEdx_ST_em = new TH1D("dEdxSTem", ";Electron dE/dx in ST", 200, 0,  0.015); 

	dHist_dEdx_ST_ep_VS_P_ep = new TH2D("dEdxSTep_VS_epP", ";Positron Kin Fit P magnitude (GeV) ;dE/dx ST Positron", 300, 0, 10, 300, 0, 0.015);
	dHist_dEdx_ST_em_VS_P_em = new TH2D("dEdxSTem_VS_emP", ";Electron Kin Fit P magnitude (GeV) ;dE/dx ST Electron", 300, 0, 10, 300, 0, 0.015);
	dHist_dEdx_ST_all_VS_P_missp = new TH2D("dEdxSTall_VS_misspP", ";Missing Proton P Magnitude (GeV) ;dE/dx ST charged hypos", 300, 0, 2, 300, 0, 0.015);
	dHist_dEdx_ST_all_VS_P_ep = new TH2D("dEdxSTall_VS_epP", ";Positron Kin Fit P magnitude (GeV) ;dE/dx ST charged hypos", 300, 0, 10, 300, 0, 0.015);
	dHist_dEdx_ST_ep_VS_P_missp = new TH2D("dEdxSTep_VS_misspP", ";Missing Proton P Magnitude (GeV) ;dE/dx ST Positron", 300, 0, 2, 300, 0, 0.015);
	dHist_dEdx_ST_em_VS_P_missp = new TH2D("dEdxSTem_VS_misspP", ";Missing Proton P Magnitude (GeV) ;dE/dx ST Electron", 300, 0, 2, 300, 0, 0.015);

	dHist_dEdx_ST_ep_VS_P_unused = new TH2D("dEdxSTep_VS_P_unused", ";Unused Track Momentum Magnitude (GeV/c) ;dEdx ST Positron", 200, 0, 5, 200, 0, 0.015);
	dHist_dEdx_ST_em_VS_P_unused = new TH2D("dEdxSTem_VS_P_unused",";Unused Track Momentum Magnitude (GeV/c) ;dEdx ST Electron", 200, 0, 5, 200, 0, 0.015);
	dHist_dEdx_ST_all_VS_P_unused = new TH2D("dEdxSTall_VS_P_unused", ";Unused Track Momentum Magnitude (GeV/c) ;dEdx ST all charged tracks", 200, 0, 5, 200, 0, 0.015);

	
       
        //Momentum Transfer Squared                                                                                                                                    
        dHist_tkin = new TH1D("tkin", ";|t| Kin (GeV/c)^{2}", 30, 0.000000, 0.015);  //0.15 max x for data
        dHist_tmeas  = new TH1D("tmeas", ";|t| meas (GeV/c)^{2}", 300, 0.000000, 0.15);
        dHist_tTM = new TH1D("GenMeas t res", "; (GeV/c)^{2}", 300, -.005, .005);
	dHist_t_VS_W = new TH2D("t_VS_W", ";Invariant Mass (GeV/c^{2}) ;  -t kinfit (GeV/c)^{2}", 238, 0, 1.5, 300,0,.15); 
	
        dHist_tgen = new TH1D("tgen", ";|t| Gen (GeV/c)^{2}", 300, -0.03, 0.00);
        dHist_tTK = new TH1D("GenKin t res", "; (GeV/c)^{2}", 100, -0.006, 0.006);
	dHist_tgen_vs_tresTM = new TH2D("tgen_vs_tresTM", ";t thrown (GeV)^{2} ;t resolution (thrown - measured) (GeV)^{2}", 400, -0.5, 0.05, 400, -.2,.2);
	dHist_tgen_vs_tresTK = new TH2D("tgen_vs_tresTK", ";t thrown (GeV)^{2} ;t resolution (thrown - kin fit) (GeV)^{2}", 30, 0.00000, 0.015, 200, -.005,.005);

	dHist_tgen_vs_tresTMoT = new TH2D("tgen_vs_FractresTK", ";t thrown (GeV)^{2} ;t frac resolution (thrown - kin fit)/thrown (GeV)^{2}", 30, 0.00000, 0.015, 200, -5,2);

	dHist_Jphi = new TH1D("Azimuthal_phi_of_JT_0pol", ";Degrees", 180, -180, 180);
	dHist_phi = new TH1D("Lab phi", ";Degrees", 180, -180, 180);
	dHist_Jphi_VS_phi = new TH2D("PhiJT0_vs_LabPHI", ";Lab phi (Degrees) ; Phi of JT (Degrees)", 45, -180, 180, 45, -180, 180);
        
	dHist_Jphi_Thrown = new TH1D("Azimuthal_phi_JT_Thrown", ";Degrees", 300, -180, 180);
        dHist_JphiTK_vs_JphiThrown = new TH2D("phi_JT_res_vs_JTThrown", ";JPhi thrown Degrees ;Jphi res (thrown - kin)", 45, -180, 180, 100, -30, 30);
        // CUT PARAMETERS:                                                                                                                                             
        //      fMaxPion_dEdx = new TF1("fMaxPion_dEdx", "exp(-1.*[0]*x +[1]) + [2]", 0., 100.);                                                                       
        //      fMaxPion_dEdx->SetParameters(4.0, 2.0, 2.5); //First number goes into [0], second into [1], etc... What is x?                                          

        dMinKinFitCL = 1e-6; //5.73303e-7; //Where does this confidence level come from?                                                                                  
        dMaxKinFitChiSq = 5;
        dMinBeamEnergy  = 8.12;
        dMaxBeamEnergy  = 8.88;
        dMin2eMass     = 0.0;
        dMax2eMass     = 2.5;
        dMinMissingMassSquared = -0.5;
        dMaxMissingMassSquared = 5;
	dMinPositronP3 = 0;
	dMinElectronP3 = 0;
	dMaxNumUnusedTracks = 0;
	dMinPoverE = 0;//0.82;
	dMaxPoverE =20; //1.22;

	dMinEoverP = 0.5; //0.8;

        dMinEoverPkin =  0.7660974; //Calculation 9.48789e-01 - 3*6.08972e-02  = 0.7660974      
	//new for ver27 0.5 cut: 0.75498740
	dMaxEoverPkin = 1.1314806;  //Calculation 9.48789e-01 + 3*6.08972e-02 = 1.1314806            for positron
	//new for ver 27 0.5 cut: 1.1336066

	dMinEoverPkin2 = 0.7404814;// 9.36598e-01 - 3*6.53722e-02     Electron
	dMaxEoverPkin2 = 1.1327146; //9.36598e-01 + 3*6.53722e-02     Electron

	dMaxFCALElasticity = 1.12; //1.05; Was 1.12 for the .85thingie.
	dMaxMissingProtonP3Mag = 15;// 0.115;

        dMaxFCAL_DOCA_em = 1.67783; //8.68472e-01 + 3 * 2.69786e-01
	dMaxFCAL_DOCA_ep = 1.661351; //8.68472e-01 + 3* 2.64293e-01 
	//1.7470320 ver 27
	
	dMaxTOF_dEdx_ep = 0.0056336200;
	dMaxTOF_dEdx_em = 0.0055915270;

        ABeamCut                    = 1;
	ATOF2HitCut                 = 1;
	AFCAL2HitCut                = 1;

	AFCAL2TrackEoverPmeasCut    = 1;

        AKinFitCLCut                = 0;
	AFCALElasticityCut          = 1;
        AMissingMassSqCut           = 0;
	AInvariantMassCut           = 0;


	AFCALE1overP1kinCut         = 1;
	AFCALE2overP2kinCut         = 1;
	ATOFdEdxPositronCut         = 0;
	ATOFdEdxElectronCut         = 0;
	ADOCAPositronCut            = 1;
	ADOCAElectronCut            = 1;
	AUnusedShowerEnergyCut      = 1;

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
}

Bool_t DSelector_2eMissingProton::Process(Long64_t locEntry)
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
	cout << "RUN " << Get_RunNumber() << ", EVENT " << Get_EventNumber() << endl;
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
	//dAnalyzeCutActions->Reset_NewEvent(); // manual action, must call Reset_NewEvent()

	//PREVENT-DOUBLE COUNTING WHEN HISTOGRAMMING
	//Sometimes, some content is the exact same between one combo and the next
	//e.g. maybe two combos have different beam particles, but the same data for the final-state
	//When histogramming, you don't want to double-count when this happens: artificially inflates your signal (or background)
	//So, for each quantity you histogram, keep track of what particles you used (for a given combo)
	//Then for each combo, just compare to what you used before, and make sure it's unique

	//EXAMPLE 1: Particle-specific info:
	set<Int_t> locUsedSoFar_BeamEnergy; //Int_t: Unique ID for beam particles. set: easy to use, fast to search
	set<Int_t> locUsedSoFar_Positron, locUsedSoFar_Electron;

	//EXAMPLE 2: Combo-specific info:
	//In general: Could have multiple particles with the same PID: Use a set of Int_t's
	//In general: Multiple PIDs, so multiple sets: Contain within a map
	//Multiple combos: Contain maps within a set (easier, faster to search)
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MissingMass, locUsedSoFar_2e, locUsedSoFar_Angles;

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


	/******************************************* LOOP OVER THROWN DATA (OPTIONAL) ***************************************/
	                                                                
        double locEbeam_Thrown = 0;
        if(dThrownBeam != NULL)
	  locEbeam_Thrown = dThrownBeam->Get_P4().T();
        TLorentzVector locBeamP4_Thrown = dThrownBeam->Get_P4();
        //Loop over throwns                                                                                                                                                            
        TLorentzVector locProton_Thrown;
        TLorentzVector locPositronP4_Thrown;
        TLorentzVector locElectronP4_Thrown;
  

        for(UInt_t loc_i = 0; loc_i < Get_NumThrown(); ++loc_i)
	  {
	    //Set branch array indices corresponding to this particle                                                                                                         
	    dThrownWrapper->Set_ArrayIndex(loc_i);
	    Particle_t thrown_pid = dThrownWrapper->Get_PID();
	    cout << " loc_i=" << loc_i << " thrown_pid=" << thrown_pid << endl;
	    TLorentzVector locP4_Thrown = dThrownWrapper->Get_P4();
	    if (loc_i == 2) locProton_Thrown = locP4_Thrown;    // Assume recoil is index 3                                                                                   
	    if (loc_i == 1) locElectronP4_Thrown = locP4_Thrown;                                                                                                              
	    if (loc_i == 0) locPositronP4_Thrown = locP4_Thrown;
	  }
	cout << "After thrown PID assignment " << endl;
	TLorentzVector loc2eP4_Thrown = locPositronP4_Thrown + locElectronP4_Thrown;

        double tgen = (dThrownBeam->Get_P4() - locPositronP4_Thrown - locElectronP4_Thrown).M2();    // use beam and 2pi momenta                                                          

        double eplusE_Thrown = locPositronP4_Thrown.E();
        double eminusE_Thrown = locElectronP4_Thrown.E();
        double E2e_Thrown = eplusE_Thrown + eminusE_Thrown;

        TLorentzVector locMissingP4_Thrown = dThrownBeam->Get_P4() + dTargetP4;
	locMissingP4_Thrown -= locPositronP4_Thrown + locElectronP4_Thrown;

	double JWeight1_Thrown = locPositronP4_Thrown.X()*locPositronP4_Thrown.X() + locPositronP4_Thrown.Y()*locPositronP4_Thrown.Y() + ElectronMass*ElectronMass;
        double JWeight2_Thrown = locElectronP4_Thrown.X()*locElectronP4_Thrown.X() + locElectronP4_Thrown.Y()*locElectronP4_Thrown.Y() + ElectronMass*ElectronMass;
	double Jx_Thrown = locPositronP4_Thrown.X()/JWeight1_Thrown + locElectronP4_Thrown.X()/JWeight2_Thrown;
	double Jy_Thrown = locPositronP4_Thrown.Y()/JWeight1_Thrown + locElectronP4_Thrown.Y()/JWeight2_Thrown;        
	double Jphi_Thrown = atan2(Jy_Thrown,Jx_Thrown)*180/PI;

       	double Elasticity_Thrown = (locBeamP4_Thrown.E() - locElectronP4_Thrown.E() - locPositronP4_Thrown.E())/locBeamP4_Thrown.E();
	
	

	/************************************************* LOOP OVER COMBOS *************************************************/

	//Loop over combos
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i)
	{
	  //Set branch array indices for combo and all combo particles
	  dComboWrapper->Set_ComboIndex(loc_i);
	  
	  // Is used to indicate when combos have been cut
	  if(dComboWrapper->Get_IsComboCut()) // Is false when tree originally created
	    continue; // Combo has been cut previously

	  /********************************************** GET PARTICLE INDICES *********************************************/

	  //Used for tracking uniqueness when filling histograms, and for determining unused particles
	  
	  //Step 0
	  Int_t locBeamID = dComboBeamWrapper->Get_BeamID();
	  Int_t locPositronTrackID = dPositronWrapper->Get_TrackID();
	  Int_t locElectronTrackID = dElectronWrapper->Get_TrackID();

	  /*********************************************** GET FOUR-MOMENTUM **********************************************/

	  //Get P4's: //is kinfit if kinfit performed, else is measured
	  //dTargetP4 is target p4
	  //Step 0
	  TLorentzVector locBeamP4 = dComboBeamWrapper->Get_P4();
	  TLorentzVector locPositronP4 = dPositronWrapper->Get_P4();
	  TLorentzVector locPositronX4 = dPositronWrapper->Get_X4();
	  TLorentzVector locElectronP4 = dElectronWrapper->Get_P4();
	  TLorentzVector locElectronX4 = dElectronWrapper->Get_X4();
	  TLorentzVector locMissingProtonP4 = dMissingProtonWrapper->Get_P4();
	  

	  double OpeningAngle = acos((locPositronP4.Vect().Dot( locElectronP4.Vect() ) )/( locPositronP4.Vect().Mag() * locElectronP4.Vect().Mag()) ) * 180.0/PI ;
	  double MissingProtonP3Mag = locMissingProtonP4.Vect().Mag();



	  TLorentzVector loc2eP4 = locPositronP4 + locElectronP4;
	  
	  double E2e = loc2eP4.E();
	 
	  double JWeight1 = locPositronP4.X()*locPositronP4.X() + locPositronP4.Y()*locPositronP4.Y() + ElectronMass*ElectronMass;
	  double JWeight2 = locElectronP4.X()*locElectronP4.X() + locElectronP4.Y()*locElectronP4.Y() + ElectronMass*ElectronMass;
	  double Jx = (locPositronP4.X()/JWeight1) + (locElectronP4.X()/JWeight2);
	  double Jy = (locPositronP4.Y()/JWeight1) + (locElectronP4.Y()/JWeight2);
	  double Jphi = atan2(Jy,Jx)*180/PI;

	  	  double phi = atan2(locPositronP4.Y() + locElectronP4.Y(), locPositronP4.X() + locElectronP4.X())*180/PI; 

	  // double Theta_epem = fabs(atan2(locPositronX4.Y(),locPositronX4.X())*180/PI - atan2(locElectronX4.Y(),locElectronX4.X())*180/PI);
	  //	  double Dist_epem = sqrt( (locPositronX4.X() - locElectronX4.X())*(locPositronX4.X() - locElectronX4.X()) + (locPositronX4.Y() - locElectronX4.Y())* (locPositronX4.Y() - locElectronX4.Y()));


	  
	  TLorentzVector locMissingP4 = locBeamP4 + dTargetP4;
	  locMissingP4 -= locPositronP4 + locElectronP4;

	  // Get Measured P4's:
	  //Step 0
	  TLorentzVector locBeamP4_Measured = dComboBeamWrapper->Get_P4_Measured();
	  TLorentzVector locPositronP4_Measured = dPositronWrapper->Get_P4_Measured();
	  TLorentzVector locElectronP4_Measured = dElectronWrapper->Get_P4_Measured();
	  
	  TLorentzVector loc2eP4_Measured = locPositronP4_Measured + locElectronP4_Measured;
	  
	  double eplusE_Measured = locPositronP4_Measured.E();
	  double eminusE_Measured = locElectronP4_Measured.E();
	  double E2e_Measured = eplusE_Measured + eminusE_Measured;
	  //  double Elasticity_Measured = (locBeamP4_Measured.E() - locElectronP4_Measured.E() - locPositronP4_Measured.E())/(locBeamP4_Measured.E());	  

	  //double epemE_TMoT = (epemE_Thrown - epemE_Measured)/epemE_Thrown;
	  //double Elasticity_TMoT = (Elasticity_Thrown - Elasticity_Measured)/Elasticity_Thrown;
	  
	  /********************************************* COMBINE FOUR-MOMENTUM ********************************************/
	  cout << " " <<endl;
	  cout << "Combining 4 momentum" << endl;
	  // DO YOUR STUFF HERE
	  
	  // Combine 4-vectors
	  TLorentzVector locMissingP4_Measured = locBeamP4_Measured + dTargetP4;
	  locMissingP4_Measured -= locPositronP4_Measured + locElectronP4_Measured;
	 

	   //Missing Mass Squared
	  double locMissingMassSquared = locMissingP4.M2();
	  double locMissingEnergy = locMissingP4.E();
	  double locMissingMass = sqrt(locMissingP4.M2());
	  
	  double locMissingMassSquared_Measured = locMissingP4_Measured.M2();
	  double locMissingEnergy_Measured = locMissingP4_Measured.E();
	  double locMissingMass_Measured = sqrt(locMissingP4_Measured.M2());
	       
	  /*
	  double locMissingMassSquared_Thrown = locMissingP4_Thrown.M2();
	  double locMissingEnergy_Thrown = locMissingP4_Thrown.E();
	  double locMissingMass_Thrown = locMissingP4.M();
	  */
	  
	  double dEdx_TOF_all = dChargedHypoWrapper->Get_dEdx_TOF(); //2.169
	  double Positron_dEdx_TOF = (dPositronWrapper->Get_dEdx_TOF());
	  double Electron_dEdx_TOF = (dElectronWrapper->Get_dEdx_TOF());

	  double dEdx_CDC = dChargedHypoWrapper->Get_dEdx_CDC();
	  double dEdx_CDC_Positron = dPositronWrapper->Get_dEdx_CDC();
	  double dEdx_CDC_Electron = dElectronWrapper->Get_dEdx_CDC();

	  double dEdx_FDC_all = dChargedHypoWrapper->Get_dEdx_FDC();
	  double dEdx_FDC_ep = dPositronWrapper->Get_dEdx_FDC();
	  double dEdx_FDC_em = dElectronWrapper->Get_dEdx_FDC();

	  double dEdx_ST_all = dChargedHypoWrapper->Get_dEdx_ST();
	  double dEdx_ST_ep = dPositronWrapper->Get_dEdx_ST();
	  double dEdx_ST_em = dElectronWrapper->Get_dEdx_ST();

	  double TrackFCAL_DOCA_Charged = dChargedHypoWrapper->Get_TrackFCAL_DOCA();
	  double TrackFCAL_DOCA_ep = dPositronWrapper->Get_TrackFCAL_DOCA();
	  double TrackFCAL_DOCA_em = dElectronWrapper->Get_TrackFCAL_DOCA();
	  double TrackFCAL_DOCA_Neutral = dNeutralHypoWrapper->Get_TrackFCAL_DOCA();

	  /*	  	  
	  double E9E25_FCAL_all = dChargedHypoWrapper->Get_E9E25_FCAL();
	  double E9E25_FCAL_ep = dPositronWrapper->Get_E9E25_FCAL();
	  double E9E25_FCAL_em = dElectronWrapper->Get_E9E25_FCAL();
	  double ShowerQuality_ep = dNeutralHypoWrapper->Get_Shower_Quality();
	  */
	  double PositronEnergy_FCAL = dPositronWrapper->Get_Energy_FCAL();
	  double ElectronEnergy_FCAL = dElectronWrapper->Get_Energy_FCAL();
	  double PositronEnergy_BCAL = dPositronWrapper->Get_Energy_BCAL();
	  double ElectronEnergy_BCAL = dElectronWrapper->Get_Energy_BCAL();

	  double Energy_UnusedShowers = dComboWrapper->Get_Energy_UnusedShowers();
	  double locSumPMag_UnusedTracks = dComboWrapper->Get_Fundamental<double>("SumPMag_UnusedTracks"); 
	  TVector3 locSumP3_UnusedTracks = dComboWrapper->Get_TObject<TVector3>("SumP3_UnusedTracks");
	  double UnusedTrackTheta = locSumP3_UnusedTracks.Theta();
	  double UnusedTrackThetaRad = UnusedTrackTheta*180/PI;

	  double DetectorTime_ep = dPositronWrapper->Get_Detector_System_Timing();
	  double DetectorTime_em = dElectronWrapper->Get_Detector_System_Timing();

		  dHist_UnusedTrackTheta->Fill(UnusedTrackTheta);
		  dHist_UnusedTrackThetaRad->Fill(UnusedTrackThetaRad);
		  dHist_SumP3Mag_UnusedTracks->Fill(locSumPMag_UnusedTracks);
		  dHist_P_unused->Fill(locSumP3_UnusedTracks.Mag());
		  dHist_UnusedTrackPX->Fill(locSumP3_UnusedTracks.Px());
		  dHist_UnusedTrackPY->Fill(locSumP3_UnusedTracks.Py());
		  dHist_UnusedTrackPZ->Fill(locSumP3_UnusedTracks.Pz());

     
	  int NumUnusedTracks = dComboWrapper->Get_NumUnusedTracks();
	  if(NumUnusedTracks == 1)
	    {
	     dHist_SumP3Mag_OneUnusedTrack->Fill(locSumPMag_UnusedTracks);
	    }
	  
	  double EoverP_plus = PositronEnergy_FCAL/locPositronP4.Vect().Mag();
	  double EoverP_minus = ElectronEnergy_FCAL/locElectronP4.Vect().Mag();
	  double EoverP_plus_Measured = PositronEnergy_FCAL/locPositronP4_Measured.Vect().Mag();
	  double EoverP_minus_Measured = ElectronEnergy_FCAL/locElectronP4_Measured.Vect().Mag();
	  
	  double EoP1_Plus_EoP2 = EoverP_plus + EoverP_minus;

	  double PoverE_plus = locPositronP4.Vect().Mag()/PositronEnergy_FCAL;
	  double PoverE_minus = locElectronP4.Vect().Mag()/ElectronEnergy_FCAL;
	  double PoEtot = ((locPositronP4.Vect().Mag() + locElectronP4.Vect().Mag())/(PositronEnergy_FCAL + ElectronEnergy_FCAL));
	  double Energy_FCAL = PositronEnergy_FCAL + ElectronEnergy_FCAL;
	  double FCAL_Elas = Energy_FCAL/locBeamP4.E();
	  double FCAL_Elasnorm = FCAL_Elas/0.943906;
	  double Elas_PoE_rotate = (1/sqrt(2)) * ((PoEtot/1.05107) - (FCAL_Elas/.943906));

	  double EoP1norm = EoverP_plus/0.946057;
	  double EoP2norm = EoverP_minus/0.945564;
	  double Elas_EoP1_rotate = (1/sqrt(2)) * ( EoP1norm - FCAL_Elasnorm);
	  double Elas_EoP2_rotate = (1/sqrt(2)) * ( EoP2norm - FCAL_Elasnorm);
	  double Elas_EoP1_Plus_EoP2norm_rotate = (1/sqrt(2)) * (EoP1_Plus_EoP2/1.90402 - FCAL_Elasnorm);
	  
	  
	  double tkin = (locBeamP4 - loc2eP4).M2();    // use beam and 2pi momenta, (and I think we also want proton, right?)                                             
	  double tmeas = (locBeamP4_Measured - loc2eP4_Measured).M2();    // use beam and 2pi momenta    

	  dHist_MyCuts->Fill(0);

	  /******************************************** EXECUTE ANALYSIS ACTIONS *******************************************/
	  //	  cout << "analysis action" << endl;
	  //	  cout << " " << endl;
	  
	  // Loop through the analysis actions, executing them in order for the active particle combo
	  //dAnalyzeCutActions->Perform_Action(); // Must be executed before Execute_Actions()
	  if(!Execute_Actions()) //if the active combo fails a cut, IsComboCutFlag automatically set
	    continue;

	  //if you manually execute any actions, and it fails a cut, be sure to call:
	  //dComboWrapper->Set_IsComboCut(true);

	  //MISSING MASS CUT                                                                                                                        

	  if(ABeamCut == 1){
	    if((locBeamP4.E() < dMinBeamEnergy) || (locBeamP4.E() > dMaxBeamEnergy)){
	      dComboWrapper->Set_IsComboCut(true);
	      cout << " failed beam cut" << endl;
	      cout << " " << endl;
	      continue;
	    }
	  }
	  //	  cout << "passed beam cut" << endl;
	  // cout << " " << endl;
	  dHist_MyCuts->Fill(1);
   
	  if(AMissingMassSqCut == 1){
	    if((locMissingMassSquared_Measured < dMinMissingMassSquared ) || (locMissingMassSquared_Measured > dMaxMissingMassSquared)){
	      dComboWrapper->Set_IsComboCut(true);
	      cout << "Failed MM cut" << endl;
	      cout << " " << endl;
	      continue;
	    }
	  }
	 
	  //	  cout << " passed MM cut" << endl;
	  //	  cout << " " << endl;

	  dHist_MyCuts->Fill(2);

	  if(AFCAL2HitCut == 1){
	    if(PositronEnergy_FCAL <= 0 || ElectronEnergy_FCAL <= 0)
	      {
		dComboWrapper->Set_IsComboCut(true);
		cout << " Failed FCAL 2 hit cut" << endl;
		cout << " " << endl;
		continue;
	      }
	  }
	  //	  cout << " Passed FCAL E cut" << endl;
	  //	  cout << " " << endl;
	  
	  dHist_MyCuts->Fill(3);	 
	
	  if(ATOF2HitCut == 1){
	    if(Positron_dEdx_TOF <= 0 || Electron_dEdx_TOF <= 0)
	      {
		dComboWrapper->Set_IsComboCut(true);
		cout << "Failed TOF 2 hit cut" <<endl;
		cout << " " << endl;
		continue;
	      }
	  }

	  dHist_MyCuts->Fill(4);

	  if(ATOFdEdxPositronCut == 1){
	    if(Positron_dEdx_TOF > dMaxTOF_dEdx_ep)
	      {
		dComboWrapper->Set_IsComboCut(true);
		cout << "Failed Positron dEdx_TOF cut" << endl;
		continue;
		//mean = 1.80580e-03                 sigma = 2.75226e-04
		// mean + 3*sigma = 0.002631478      mean - 3*sigma = 0.000980122
	      }
	  }
	  
	  dHist_MyCuts->Fill(5);


	  if(ATOFdEdxElectronCut == 1){
	    if(Electron_dEdx_TOF > dMaxTOF_dEdx_em)
	      {
		dComboWrapper->Set_IsComboCut(true);
		cout << "Failed Electron dEdx_TOF cut" << endl;
		continue;
		//mean = 1.80757e-03                 sigma = 2.75054e-04
		//mean + 3*sigma = 0.002632732       mean - 3*sigma = 0.000982
	      }
	  }

	  dHist_MyCuts->Fill(6);

	  //  	  cout << "Passed dEdx_TOF cut" << "\n";
	  
	  if(AFCALElasticityCut == 1){
	    if(FCAL_Elas > dMaxFCALElasticity || FCAL_Elas < 0.84)
	      {
		dComboWrapper->Set_IsComboCut(true);
		cout << "Failed FCAL_Elas cut" << endl;
		continue;
	      }
	  }
	  dHist_MyCuts->Fill(7); 

	  //KINEMATIC FIT CL CUT                                                                               
	  //-20 to 1...2D 10 bins in the y dimension 0 to 1. The x is the missing mass difference.             

	  if(AKinFitCLCut == 1){
	    if(dComboWrapper->Get_ConfidenceLevel_KinFit("") <= dMinKinFitCL) {
	      dComboWrapper->Set_IsComboCut(true);
	      cout << "Failed CL cut" << endl;
	      cout << " " <<endl;
	      continue;
	    }
	  }
	  dHist_MyCuts->Fill(8);
	  //	  cout << " Passed kinematic Fit CL cut " << endl;

	  
	  if(NumUnusedTracks > dMaxNumUnusedTracks) {
	    dComboWrapper->Set_IsComboCut(true);
	    cout << "failed NumUnusedTracks cut" << endl;
	    continue;
	  }
	  
	  dHist_MyCuts->Fill(9);
	  dHist_NumUnusedTracks->Fill(NumUnusedTracks);


	  if(AInvariantMassCut == 1){
	    if( loc2eP4.M() > dMax2eMass || loc2eP4.M() < dMin2eMass) {
	      dComboWrapper->Set_IsComboCut(true);
	      continue;
	    }
	  }
	   dHist_MyCuts->Fill(10);
	  
	  if(AFCAL2TrackEoverPmeasCut == 1){
	    if(EoverP_plus_Measured < dMinEoverP) {
	      dComboWrapper->Set_IsComboCut(true);
	      continue;
	    }
	     
            dHist_MyCuts->Fill(11);
	     
	     if(EoverP_minus_Measured < dMinEoverP) {
	       dComboWrapper->Set_IsComboCut(true);
	       continue;
	     }

	     dHist_MyCuts->Fill(12);
	  }

	   
	   //E1/P1 cut
	  if(AFCALE1overP1kinCut == 1){
	    if(EoverP_plus < dMinEoverPkin || EoverP_plus > dMaxEoverPkin)
	      {
		dComboWrapper->Set_IsComboCut(true);
		continue;
	      }	   
	    dHist_MyCuts->Fill(13);
	  }

	   //E2/P2 cut
	  if(AFCALE2overP2kinCut == 1){
	    if(EoverP_minus < dMinEoverPkin2 || EoverP_minus > dMaxEoverPkin2)
	      {
		dComboWrapper->Set_IsComboCut(true);
		continue;
	      }
	    dHist_MyCuts->Fill(14);
	  }

	    

	  if(ADOCAElectronCut == 1){
	    if(TrackFCAL_DOCA_em >  dMaxFCAL_DOCA_em)
	      {
		dComboWrapper->Set_IsComboCut(true);
		continue;
	      }
	    dHist_MyCuts->Fill(15);
	  }


	  if(ADOCAPositronCut == 1){
	    if(TrackFCAL_DOCA_ep > dMaxFCAL_DOCA_ep)
	      {
		dComboWrapper->Set_IsComboCut(true);
		continue;
	      }
	    dHist_MyCuts->Fill(16);
	  }


	   
	  /******************************************** Accidental Subtraction ***************************************************/
	  double AccWeight = 0.;

	  // measured tagger time for combo                                                                                                                 
                                                                                                                                                                             
	  TLorentzVector locBeam_X4_Measured = dComboBeamWrapper->Get_X4_Measured();

	  // measured RF time for combo                                                                                                                     
                                                                                                                                                                             
	  double locRFTime = dComboWrapper->Get_RFTime_Measured();


	  // time difference between tagger and RF (corrected for production vertex position relative to target center)                                               
                                                                                                                                                                             
	  double locBeamDeltaT = locBeam_X4_Measured.T() - (locRFTime + (locBeam_X4_Measured.Z() - dTargetCenter.Z())/29.9792458);

	  if(fabs(locBeamDeltaT) < 0.5*4.008) { // prompt signal recieves a weight of 1                                                                               
                                                                                                                                                                             
	    AccWeight = 1.;
	  }
	  else { // accidentals recieve a weight of 1/# RF bunches included in TTree (??? in this case)                                                               
                                                                                                                                                                             
	    AccWeight = -1./4.; //need to have the total number of buckets. Make the histogram wider to find out how many                                              
	  }

	  //  cout << " Tagger Accidentals: dTargetCenter=" <<  dTargetCenter.Z() << " locRFTime=" << locRFTime << " locBeamDeltaT=" << locBeamDeltaT << " AccWeight=" << 
	  //  AccWeight << endl;
	  //  cout << " locBeam_X4_Measured="; locBeam_X4_Measured.Print();
	  dHist_TaggerAccidentals->Fill(locBeamDeltaT);

	  
	  if(AUnusedShowerEnergyCut == 1){
	    if(Energy_UnusedShowers > 0)
	      {
		dComboWrapper->Set_IsComboCut(true);
		continue;
	      }
	    dHist_MyCuts->Fill(17);
	  }

	  dHist_KinFitChiSq->Fill(dComboWrapper->Get_ChiSq_KinFit("")/dComboWrapper->Get_NDF_KinFit(""));
	  dHist_KinFitCL->Fill(dComboWrapper->Get_ConfidenceLevel_KinFit(""));
	  dHist_KinFitCLlogx->Fill(  log10(dComboWrapper->Get_ConfidenceLevel_KinFit("")   ) );                                                                           
	  //dHist_CLvsMissingMassDiff->Fill(locMissingMassSquared, dComboWrapper-> Get_ConfidenceLevel_KinFit(""));                                                              

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
	  if(locUsedSoFar_BeamEnergy.find(locBeamID) == locUsedSoFar_BeamEnergy.end())
	    {
	      dHist_BeamEnergy->Fill(locBeamP4.E());
	      locUsedSoFar_BeamEnergy.insert(locBeamID);
	    }




	  /************************************ EXAMPLE: HISTOGRAM MISSING MASS SQUARED ************************************/
	  
	 

	  //Uniqueness tracking: Build the map of particles used for the missing mass
	  //For beam: Don't want to group with final-state photons. Instead use "Unknown" PID (not ideal, but it's easy).
	  map<Particle_t, set<Int_t> > locUsedThisCombo_MissingMass;
	  locUsedThisCombo_MissingMass[Unknown].insert(locBeamID); //beam
	  locUsedThisCombo_MissingMass[Positron].insert(locPositronTrackID);
	  locUsedThisCombo_MissingMass[Electron].insert(locElectronTrackID);

	  //compare to what's been used so far
	  if(locUsedSoFar_MissingMass.find(locUsedThisCombo_MissingMass) == locUsedSoFar_MissingMass.end())
	    {
	      //unique missing mass combo: histogram it, and register this combo of particles      
	      //Missing Mass Squared Histograms

	      if(AccWeight == 1)
		{ 
		  //		  dHist_MissingMassSquared->Fill(locMissingMassSquared,1);
		  dHist_MissingMassSquared_Measured->Fill(locMissingMassSquared_Measured,1);

	      // dHist_MissingMassSquaredMK->Fill(locMissingMassSquared_Measured - locMissingMassSquared);
	      // cout <<" and differences" << endl;

	      /*
	      dHist_MissingMassSquaredTM->Fill(locMissingMassSquared_Thrown - locMissingMassSquared_Measured);
	      dHist_MissingMassSquared_Thrown->Fill(locMissingMassSquared_Thrown);
	      dHist_MissingMassSquaredTK->Fill(locMissingMassSquared_Thrown - locMissingMassSquared);
	      */

		  //		  dHist_MissingMass->Fill(locMissingMass,1);
		  //		  dHist_MissingEnergy->Fill(locMissingEnergy,1);
		}
	      else
		{
		  dHist_MissingMassSquared_Measured->Fill(locMissingMassSquared_Measured,-1./4.);
		}
	      locUsedSoFar_MissingMass.insert(locUsedThisCombo_MissingMass);
	      
	     
	    }



	  map<Particle_t, set<Int_t> > locUsedThisCombo_2e;
	  locUsedThisCombo_2e[Unknown].insert(locBeamID); //beam
	  locUsedThisCombo_2e[Positron].insert(locPositronTrackID);
	  locUsedThisCombo_2e[Electron].insert(locElectronTrackID);

	  dHist_DetectorTiming_ep->Fill(DetectorTime_ep, AccWeight);
	  dHist_DetectorTiming_em->Fill(DetectorTime_em, AccWeight);

	  dHist_dEdx_CDC_VS_MissingProtonP3Mag->Fill( MissingProtonP3Mag, dEdx_CDC, AccWeight);
	  dHist_dEdx_CDC_All_VS_P_Positron->Fill(locPositronP4.Vect().Mag(), dEdx_CDC, AccWeight);
	  dHist_dEdx_FDC_all_VS_P_missp->Fill(MissingProtonP3Mag, dEdx_FDC_all, AccWeight);
	  dHist_dEdx_FDC_all_VS_P_ep->Fill(locPositronP4.Vect().Mag(), dEdx_FDC_all, AccWeight);
	  dHist_dEdx_ST_all_VS_P_missp->Fill(MissingProtonP3Mag, dEdx_ST_all, AccWeight);
	  dHist_dEdx_ST_all_VS_P_ep->Fill(locPositronP4.Vect().Mag(), dEdx_ST_all, AccWeight);

	  dHist_TrackFCAL_DOCA_Neutral_VS_W->Fill(loc2eP4.M(), TrackFCAL_DOCA_Neutral, AccWeight);
	  dHist_TrackFCAL_DOCA_Neutral->Fill(TrackFCAL_DOCA_Neutral, AccWeight);
	  dHist_TrackFCAL_DOCA_Charged_VS_W->Fill(loc2eP4.M(), TrackFCAL_DOCA_Charged, AccWeight);
	  dHist_TrackFCAL_DOCA_Charged->Fill(TrackFCAL_DOCA_Charged, AccWeight);
	  /*  
	  dHist_ShowerQuality_ep->Fill(ShowerQuality_ep, AccWeight);
	  dHist_E9E25_FCAL_all_VS_W->Fill(loc2eP4.M(), E9E25_FCAL_all, AccWeight);
	  dHist_E9E25_FCAL_ep_VS_W->Fill(loc2eP4.M(), E9E25_FCAL_ep, AccWeight);
	  dHist_E9E25_FCAL_em_VS_W->Fill(loc2eP4.M(), E9E25_FCAL_em, AccWeight);
	  */
	  dHist_phi->Fill(phi, AccWeight);
	  dHist_Jphi_VS_phi->Fill(phi, Jphi, AccWeight);
	  dHist_JphiTK_vs_JphiThrown->Fill(Jphi_Thrown, Jphi_Thrown - Jphi, AccWeight);

	      if(NumUnusedTracks == 1)
		{
                  dHist_MissingProtonP3Mag_VS_SumPMag_UnusedTracks->Fill(locSumPMag_UnusedTracks, MissingProtonP3Mag, AccWeight);

      	          dHist_UnusedTrackP3Mag_VS_W->Fill(loc2eP4.M(), locSumPMag_UnusedTracks, AccWeight);
	 	  dHist_UnusedTrackTheta_VS_W->Fill(loc2eP4.M(), UnusedTrackTheta, AccWeight);
		  dHist_UnusedTrackTheta_VS_MissingProtonTheta->Fill(MissingProtonP3Mag, UnusedTrackTheta, AccWeight);

		  dHist_dEdx_TOF_ep_VS_P_unused->Fill(locSumPMag_UnusedTracks, Positron_dEdx_TOF,AccWeight);
		  dHist_dEdx_TOF_em_VS_P_unused->Fill(locSumPMag_UnusedTracks, Electron_dEdx_TOF,AccWeight);
		  dHist_dEdx_TOF_all_VS_P_unused->Fill(locSumPMag_UnusedTracks, dEdx_TOF_all, AccWeight);

		  dHist_dEdx_CDC_ep_VS_P_unused->Fill(locSumPMag_UnusedTracks, dEdx_CDC_Positron,AccWeight);
		  dHist_dEdx_CDC_em_VS_P_unused->Fill(locSumPMag_UnusedTracks, dEdx_CDC_Electron,AccWeight);
		  dHist_dEdx_CDC_all_VS_P_unused->Fill(locSumPMag_UnusedTracks, dEdx_CDC, AccWeight);

		  dHist_dEdx_FDC_ep_VS_P_unused->Fill(locSumPMag_UnusedTracks, dEdx_FDC_ep, AccWeight);
		  dHist_dEdx_FDC_em_VS_P_unused->Fill(locSumPMag_UnusedTracks, dEdx_FDC_em, AccWeight);
		  dHist_dEdx_FDC_all_VS_P_unused->Fill(locSumPMag_UnusedTracks, dEdx_FDC_all, AccWeight);

		  dHist_dEdx_ST_ep_VS_P_unused->Fill(locSumPMag_UnusedTracks, dEdx_ST_ep, AccWeight);
		  dHist_dEdx_ST_em_VS_P_unused->Fill(locSumPMag_UnusedTracks, dEdx_ST_em, AccWeight);
		  dHist_dEdx_ST_all_VS_P_unused->Fill(locSumPMag_UnusedTracks, dEdx_ST_all, AccWeight);

		}

	  if(locUsedSoFar_2e.find(locUsedThisCombo_2e) == locUsedSoFar_2e.end())
	    {
	      //Two Particle Invariant Mass Histograms
	      dHist_Energy_UnusedShowers->Fill(Energy_UnusedShowers,AccWeight);
	      dHist_EUnSh_VS_W->Fill(Energy_UnusedShowers,loc2eP4.M(),AccWeight);
	      dHist_EUnSh_VS_Elas->Fill(Energy_UnusedShowers,Energy_FCAL/locBeamP4.E(),AccWeight);
	      dHist_M2ekin_VS_NumUnusedTracks->Fill(loc2eP4.M(), NumUnusedTracks, AccWeight);


	      dHist_Elasticity_VS_NumUnusedTracks->Fill(Energy_FCAL/locBeamP4.E(), NumUnusedTracks, AccWeight);
	      
	      dHist_EoverP_Positron_Measured->Fill(EoverP_plus_Measured,AccWeight);
	      dHist_EoverP_Electron_Measured->Fill(EoverP_minus_Measured,AccWeight);
	      dHist_ElasticityKinFit->Fill((locPositronP4.E() + locElectronP4.E())/locBeamP4.E(),AccWeight);
      	      dHist_ElasticityMeasured->Fill((locPositronP4_Measured.E() + locElectronP4_Measured.E())/locBeamP4.E(),AccWeight);
	      dHist_Energy_FCALbyPeak_vs_EoverP_electronKF->Fill(Energy_FCAL/locBeamP4.E(),EoverP_minus,AccWeight);
	      dHist_Energy_FCALbyPeak_vs_EoverP_electronMeas->Fill(Energy_FCAL/locBeamP4.E(),EoverP_minus_Measured,AccWeight);


	      dHist_Theta1->Fill(locPositronP4.Theta()*(180.0/PI), AccWeight);
	      dHist_Theta2->Fill(locElectronP4.Theta()*(180.0/PI), AccWeight);
	      dHist_Theta1TK_vs_Theta1T->Fill(locPositronP4_Thrown.Theta()*(180.0/PI), (locPositronP4_Thrown.Theta() - locPositronP4.Theta())*(180.0/PI), AccWeight);
	      dHist_Theta2TK_vs_Theta2T->Fill(locElectronP4_Thrown.Theta()*(180.0/PI), (locElectronP4_Thrown.Theta() - locElectronP4.Theta())*(180.0/PI), AccWeight);

	      dHist_ep_phi->Fill(locPositronP4.Phi()*(180.0/PI), AccWeight);
	      dHist_em_phi->Fill(locElectronP4.Phi()*(180.0/PI), AccWeight);


	      
	      dHist_EoverP_vs_P_ElectronMeas->Fill(locElectronP4_Measured.Vect().Mag(),EoverP_minus_Measured,AccWeight);
	      dHist_EoverP_vs_P_Electron->Fill(locElectronP4.Vect().Mag(),EoverP_minus,AccWeight);
	      dHist_Elasticity_VS_PoverEtot->Fill(PoEtot, FCAL_Elas, AccWeight);
	      dHist_Elas_PoE_rotate->Fill(Elas_PoE_rotate, AccWeight);
	      dHist_PoEtot->Fill(PoEtot,AccWeight);
	      dHist_Ilia->Fill(FCAL_Elas, AccWeight);
	      dHist_EoP1_VS_EoP2->Fill(EoverP_plus, EoverP_minus,AccWeight);
	      dHist_EoP1_Plus_EoP2->Fill(EoP1_Plus_EoP2,AccWeight);
	      dHist_Elas_VS_EoP1->Fill(EoverP_plus, FCAL_Elas, AccWeight);
	      dHist_Elas_VS_EoP2->Fill(EoverP_minus, FCAL_Elas, AccWeight);
	      dHist_Elas_VS_EoP1_Plus_EoP2->Fill(EoP1_Plus_EoP2, FCAL_Elas, AccWeight);

	      dHist_Elas_EoP1_rotate->Fill(Elas_EoP1_rotate, AccWeight);
	      dHist_Elas_EoP2_rotate->Fill(Elas_EoP2_rotate, AccWeight);
	      dHist_Elas_EoP1_Plus_EoP2norm_rotate->Fill(Elas_EoP1_Plus_EoP2norm_rotate, AccWeight);


	      dHist_MissingProtonP3Mag_VS_W->Fill(loc2eP4.M(), MissingProtonP3Mag, AccWeight);
	      dHist_MissingProtonTheta_VS_W->Fill(loc2eP4.M(), locMissingProtonP4.Vect().Theta()*180/PI, AccWeight);
	      dHist_MissingProtonPhi_VS_W->Fill(loc2eP4.M(), locMissingProtonP4.Vect().Phi()*180/PI, AccWeight);
	     
	  
	      dHist_eplusMomentum->Fill(locPositronP4.Vect().Mag(), AccWeight);
	      dHist_eminusMomentum->Fill(locElectronP4.Vect().Mag(), AccWeight);
	      dHist_epMomentum_VS_theta->Fill(locPositronP4.Theta()*180/PI, locPositronP4.Vect().Mag(), AccWeight);
    			      
	      //	      dHist_Energy_FCALbyPeak_vs_EoverP_electron->Fill(Energy_FCAL/locBeamP4.E(),EoverP_minus,AccWeight);
	      //	      dHist_Energy_FCALbyPeak_vs_electronP->Fill(Energy_FCAL/locBeamP4.E(),locElectronP4_Measured.Vect().Mag(),AccWeight);


	      dHist_PoverE_Positron->Fill(PoverE_plus,AccWeight);
	      dHist_PoverE_Electron->Fill(PoverE_minus,AccWeight);
	      
	      if(loc2eP4.M() <= 0.2){
		dHist_EoP2_chop1->Fill(EoverP_minus,AccWeight);
	      }else if(loc2eP4.M() > 0.2 && loc2eP4.M() <= 0.4){
		dHist_EoP2_chop2->Fill(EoverP_minus,AccWeight);
	      }else if(loc2eP4.M() > 0.4 && loc2eP4.M() <= 0.6){
		dHist_EoP2_chop3->Fill(EoverP_minus,AccWeight);
	      }else if(loc2eP4.M() > 0.6 && loc2eP4.M() <= 0.8){
		dHist_EoP2_chop4->Fill(EoverP_minus,AccWeight);
	      }else {
		dHist_EoP2_chop5->Fill(EoverP_minus,AccWeight);
	      }

	      


	      dHist_dEdx_TOF_ep->Fill(Positron_dEdx_TOF,AccWeight);
	      dHist_dEdx_TOF_em->Fill(Electron_dEdx_TOF,AccWeight);

	      dHist_dEdx_TOF_VS_P_Positron->Fill(locPositronP4.Vect().Mag(),Positron_dEdx_TOF,AccWeight);
	      dHist_dEdx_TOF_VS_P_Electron->Fill(locElectronP4.Vect().Mag(),Electron_dEdx_TOF,AccWeight);
	      dHist_dEdx_TOF_ep_VS_P_missp->Fill(MissingProtonP3Mag, Positron_dEdx_TOF,AccWeight);
	      dHist_dEdx_TOF_em_VS_P_missp->Fill(MissingProtonP3Mag,Electron_dEdx_TOF,AccWeight);
	      dHist_dEdx_TOF_all_VS_P_missp->Fill(MissingProtonP3Mag, dEdx_TOF_all, AccWeight);
	      dHist_dEdx_TOF_ep_VS_W->Fill(loc2eP4.M(), Positron_dEdx_TOF,AccWeight);
	      dHist_dEdx_TOF_em_VS_W->Fill(loc2eP4.M(), Electron_dEdx_TOF,AccWeight);
	      dHist_dEdx_TOF_all_VS_W->Fill(loc2eP4.M(), dEdx_TOF_all, AccWeight);

	      dHist_dEdx_CDC_ep->Fill(dEdx_CDC_Positron, AccWeight);
	      dHist_dEdx_CDC_em->Fill(dEdx_CDC_Electron, AccWeight);

	      dHist_dEdx_CDC_Pos_VS_P_Positron->Fill(locPositronP4.Vect().Mag(),dEdx_CDC_Positron,AccWeight);
	      dHist_dEdx_CDC_Elec_VS_P_Electron->Fill(locElectronP4.Vect().Mag(),dEdx_CDC_Electron,AccWeight);
	      dHist_dEdx_CDC_Pos_VS_P_MissP->Fill(MissingProtonP3Mag, dEdx_CDC_Positron, AccWeight);

	      dHist_dEdx_FDC_ep->Fill(dEdx_FDC_ep, AccWeight);
	      dHist_dEdx_FDC_em->Fill(dEdx_FDC_em, AccWeight);

	      dHist_dEdx_FDC_ep_VS_P_ep->Fill(locPositronP4.Vect().Mag(), dEdx_FDC_ep, AccWeight);
	      dHist_dEdx_FDC_em_VS_P_em->Fill(locElectronP4.Vect().Mag(), dEdx_FDC_em, AccWeight);
	      dHist_dEdx_FDC_ep_VS_P_missp->Fill(MissingProtonP3Mag, dEdx_FDC_ep, AccWeight);

	      dHist_dEdx_ST_ep->Fill(dEdx_ST_ep, AccWeight);
	      dHist_dEdx_ST_em->Fill(dEdx_ST_em, AccWeight);

	      dHist_dEdx_ST_ep_VS_P_ep->Fill(locPositronP4.Vect().Mag(), dEdx_ST_ep, AccWeight);
	      dHist_dEdx_ST_em_VS_P_em->Fill(locElectronP4.Vect().Mag(), dEdx_ST_em, AccWeight);
	      dHist_dEdx_ST_ep_VS_P_missp->Fill(MissingProtonP3Mag, dEdx_ST_ep, AccWeight);

	      dHist_TrackFCAL_DOCA_ep->Fill(TrackFCAL_DOCA_ep, AccWeight);
	      dHist_TrackFCAL_DOCA_em->Fill(TrackFCAL_DOCA_em, AccWeight);
	      dHist_TrackFCAL_DOCA_ep_VS_W->Fill(loc2eP4.M(), TrackFCAL_DOCA_ep, AccWeight);
	      dHist_TrackFCAL_DOCA_em_VS_W->Fill(loc2eP4.M(), TrackFCAL_DOCA_em, AccWeight);
	  

	      dHist_Elasticity_VS_W->Fill(loc2eP4.M(),FCAL_Elas, AccWeight);
	      dHist_OpeningAngle->Fill(OpeningAngle, AccWeight);
	      dHist_OpeningAngle_VS_W->Fill(loc2eP4.M(), OpeningAngle, AccWeight);

	      dHist_t_VS_W->Fill(loc2eP4.M(), -tkin, AccWeight);

	      if(AccWeight == 1)
		{
		  dHist_M2ekin->Fill(loc2eP4.M(),1);
		  dHist_M2e_Measured->Fill(loc2eP4_Measured.M(),1);
		  dHist_M2eMK->Fill(loc2eP4_Measured.M()-loc2eP4.M(),1);
		  	      
	      dHist_M2egen->Fill(loc2eP4_Thrown.M());
	      dHist_M2eTK->Fill(loc2eP4_Thrown.M() - loc2eP4.M());
	      dHist_M2eTM->Fill(loc2eP4_Thrown.M() - loc2eP4_Measured.M());
	      dHist_M2eTK_vs_M2egen->Fill(loc2eP4_Thrown.M(), loc2eP4_Thrown.M()  - loc2eP4.M());
	      
	      //Two Particle Energy Histograms
		  dHist_E2e->Fill(E2e,1);	    
		  dHist_E2e_Measured->Fill(E2e_Measured,1);
	       /*
	      dHist_E2e_Thrown->Fill(E2e_Thrown);
	      dHist_E2e_TM->Fill(E2e_Thrown - E2e_Measured);
	      dHist_E2e_TK->Fill(E2e_Thrown - E2e);
	      */
	      
	      
		  dHist_tkin->Fill(-tkin,1);
		  dHist_tmeas->Fill(tmeas,1);
	      
		  if(-tkin < 0.015){
	      dHist_tgen_vs_tresTM->Fill(tgen, (tgen - tmeas));
	      dHist_tgen_vs_tresTK->Fill(-tgen, (tgen - tkin));
	      dHist_tgen_vs_tresTMoT->Fill(-tgen, (tgen - tkin)/tgen);
	      dHist_tgen->Fill(tgen);
	      dHist_tTK->Fill(tgen-tkin);
	      dHist_tTM->Fill(tgen-tmeas);
		  }
	      
		  dHist_EoverP_Positron->Fill(EoverP_plus,1);
		  dHist_EoverP_Electron->Fill(EoverP_minus,1);
		  dHist_Energy_FCAL->Fill(Energy_FCAL,1);
		  dHist_Energy_FCALbyPeak->Fill(Energy_FCAL/locBeamP4.E(),1);
		  dHist_Jphi->Fill(Jphi,1);
		}
	      else
		{
		  dHist_M2ekin->Fill(loc2eP4.M(),-1./4.);
                  dHist_M2e_Measured->Fill(loc2eP4_Measured.M(),-1./4.);
                  dHist_M2eMK->Fill(loc2eP4_Measured.M()-loc2eP4.M(),-1./4.);
		  dHist_E2e->Fill(E2e,-1./4.);
                  dHist_E2e_Measured->Fill(E2e_Measured,-1./4.);
		  dHist_tkin->Fill(-tkin,-1./4.);
                  dHist_tmeas->Fill(tmeas,-1./4.);
		  dHist_EoverP_Positron->Fill(EoverP_plus,-1./4.);
                  dHist_EoverP_Electron->Fill(EoverP_minus,-1./4.);
                  dHist_Energy_FCAL->Fill(Energy_FCAL,-1./4.);
                  dHist_Energy_FCALbyPeak->Fill(Energy_FCAL/locBeamP4.E(),-1./4.);
		  dHist_Jphi->Fill(Jphi,-1./4.);
		}


	      locUsedSoFar_2e.insert(locUsedThisCombo_2e);
	    }
	
	  

	  /****************************************** PHYSICS FOR KINEMATIC AND  ANGULAR VARIABLES **********************************************/
	  // calculate kinematic and angular variables                                                                                                                           	  
	 

	  //	  dHist_Jphi_Thrown->Fill(Jphi_Thrown);


	  //E.g. Cut
	  //if((locMissingMassSquared < -0.04) || (locMissingMassSquared > 0.04))
	  //{
	  //	dComboWrapper->Set_IsComboCut(true);
	  //	continue;
	  //}
	  
	  /****************************************** FILL FLAT TREE (IF DESIRED) ******************************************/
	  
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
	  //  Fill_FlatTree(); //for the active combo
	} // end of combo loop

	//FILL HISTOGRAMS: Num combos / events surviving actions
	//	Fill_NumCombosSurvivedHists();

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


	return kTRUE;
}

void DSelector_2eMissingProton::Finalize(void)
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
