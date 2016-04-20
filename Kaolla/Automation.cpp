#include "stdafx.h"
#include "Automation.h"



Automation::Automation()
{
	// Initialise class members
	Initialisation();
}


Automation::~Automation()
{
	//CloseHandle
}



void Automation::SetKeithley(Keithley* Keith)
{
	g_pKeithley = Keith;
}

void Automation::SetMensor(Mensor* Mens)
{
	g_pMensor = Mens;
}

void Automation::SetVannes(CVannes* pVannes)
{
	g_pVanne = pVannes;
}

void Automation::SetTemperature(CTemperature* pTemperature)
{
	g_pTemperature = pTemperature;
}

void Automation::SetDataPointer(ExperimentData * eData)	// Put this into a critical section
{
	// Store the pointer
	experimentData = eData;
}

void Automation::SetData()
{
	// Copy the data from the main thread
	EnterCriticalSection(&experimentData->criticalSection);			// Is the critical section defined well? might have to be defined in the main thread
	experimentLocalData = experimentData;
	LeaveCriticalSection(&experimentData->criticalSection);
}

void Automation::SendData()
{
	// Copy the data to the main thread
	EnterCriticalSection(&experimentData->criticalSection);
	experimentData = & experimentLocalData;
	LeaveCriticalSection(&experimentData->criticalSection);
}


void Automation::Execution()
{
	// Send start messages
	messageHandler.DisplayMessage(MESSAGE_FILLLINE);
	messageHandler.DisplayMeasurement(MESSAGE_EXPSTART);

	// Record start in global bool
	experimentLocalData.experimentInProgress = TRUE;
	experimentLocalData.experimentMeasurement = 1;
	
	// Create open and write the columns in the:
	EcritureEntete();				// Entete TXT
	EcritureEnteteCSV();			// Entete CSV
	FileMeasurementOpen();			// Measurement file
	
	// Start global experiment timer
	timerExperiment.TopChrono();


	if (experimentLocalData.experimentType == EXPERIMENT_TYPE_MANUAL)
		ExecutionManual();
	if (experimentLocalData.experimentType == EXPERIMENT_TYPE_AUTO)
		ExecutionAuto();

	FinishExperiment(false);
}

void Automation::ExecutionManual()
{
	// Infinite loop, it is broken from the inside
	while (TRUE)
	{
		// Start the timer to record time between measurements
		timerMeasurement.TopChrono();

		// Start threads and read the data
		ThreadMeasurement();

		// Do the security checks
		SecuriteTemperatures();
		SecuriteHautePression();

		// Save the time at which the measurement took place
		experimentLocalData.experimentTime = timerExperiment.TempsActuel();

		// Send the data to be saved outside of the function - ?
		messageHandler.ExchangeData();

		// Save the data to the file
		EnregistrementFichierMesures();

		// On regarde si g_eventKill est activé sans attendre
		//	- S'il est activé, alors on arrête cette boucle while
		//	- Sinon on refait un tour dans cette boucle while

		// Increment the measurement number
		experimentLocalData.experimentMeasurement ++;

		// Now check if the experiment has not been stopped from the main thread
		// This is an optimisation hack, first checking a boolean then doing an expensive kernel call only in the case of 
		if (experimentLocalData.experimentInProgress == FALSE)
		{
			DWORD TempsAttente = 1000; // (ms) Poll once a seccond
			switch (::WaitForSingleObject(h_eventShutdown, TempsAttente))
			{
			case WAIT_OBJECT_0:
				FinishExperiment(false);	// Finish the experiment
				break;
			case WAIT_TIMEOUT:
				continue;
			}
		}

		// Wait some time between two individual measurements
		do {
			if (timerMeasurement.TempsActuel() < T_BETWEEN_MEASURE)
			{
				if (experimentLocalData.experimentInProgress == FALSE)
					break;
				Sleep(100);
			}
			else
				break;
		} while (TRUE);
	}
}

void Automation::ExecutionAuto()
{
	while (true)
	{
		switch (g_flagAskShutdown)		// We look at the main flag
		{
		case INACTIVE:					// In case the experiment is not started
			break;
		case PAUSE:						// In case the experiment is set as paused
			Pause();
			break;
		case STOP:						// In case the experiment is asked to stop
			ShutdownDisplay();			// then look at possible causes
			break;
		case ACTIVE:
		
			switch (experimentLocalData.experimentStage)
			{
			default:
				break;
			}

			break;
		
		default:
			ASSERT(0); // Should never reach this
			break;
		}
	}


	// If the the verifications result in cancellation, call the experiment end
	if (Verifications() == IDCANCEL)
	{
		FinishExperiment(true);
		return;
	}

	// Initialise automatic variables
	experimentLocalData.experimentDose = 0;
	//injection = 0;
	experimentLocalData.experimentTime = 0;
	experimentLocalData.experimentMeasurement = 1;
	g_flagAskShutdown = INACTIF;
	experimentLocalData.experimentStage = STAGE_UNDEF;


	//// Equilibrate
	//LigneBaseEtEquilibre(pParam);


	//// Start going through doses and perform the required ones

	//if (adsorption_continue.a_effectuer && ContinuerExperience())
	//	AdsorptionContinue(pParam);

	////if (petites_doses.a_effectuer && ContinuerExperience())
	//if (PetitesDosesAEffectuer())
	//	PetitesDoses(pParam);

	////if (moyennes_doses.a_effectuer && ContinuerExperience())
	//if (MoyennesDosesAEffectuer())
	//	MoyennesDoses(pParam);

	////if (grandes_doses.a_effectuer && ContinuerExperience())
	//if (GrandesDosesAEffectuer())
	//	GrandesDoses(pParam);

	////if (desorption.a_effectuer && ContinuerExperience())
	//if (DesorptionAEffectuer())
	//	Desorption(pParam);

	//if (experimentData->dataDivers.mise_sous_vide_fin_experience || demande_arret == ARRET_SOUSVIDE ||
	//	demande_arret == ARRET_URGENCE_HP || demande_arret == ARRET_URGENCE_TCH || demande_arret == ARRET_URGENCE_TCB)
	//	MiseSousVide(pParam);
}


void Automation::Initialisation()
{
	// Initialise threads
	h_MeasurementThread[0] = NULL;
	h_MeasurementThread[1] = NULL;
	h_MeasurementThread[2] = NULL;
	h_MeasurementThread[3] = NULL;

	// Initialise data
	SetData();

	// Initialise instruments
	InitialisationInstruments();

	// Initialise security
	InitialisationSecurityManual();

	// Open instruments
	InstrumentsOpen();

	// Initialisation of the critical section
	InitializeCriticalSection(&criticalSection);

	// Create the events
	//   - Non signalled by default
	//   - With manual reinitiallisation
	h_MeasurementThreadStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	h_eventShutdown = CreateEvent(NULL, TRUE, FALSE, NULL);
}


// The instruments which the calorimeter uses are defined here
// Ihave no clue how this works, copied it from the other code
void Automation::InitialisationInstruments()
{
	// Create instruments
	for (int i = 0; i < NB_OF_INSTRUMENTS; i++) {
		instrument[i] = new CInstrument();
	}

	int index_instr = 0;

	AppareilCalo.voie_mesure = VOIE_INDEF;
	AppareilCalo.index = INDEX_INDEF;
	AppareilHP.voie_mesure = VOIE_INDEF;
	AppareilHP.index = INDEX_INDEF;
	AppareilBP.voie_mesure = VOIE_INDEF;
	AppareilBP.index = INDEX_INDEF;

	synchCalo = PassageNul;
	synchHP = PassageNul;
	synchBP = PassageNul;

	for (int i = 1; i <= NB_OF_INSTRUMENTS; i++)
	{
		switch (GetTypeInstrument(i))
		{
		case INSTRUMENT_KEITHLEY:
			instrument[index_instr]->SetParametresInstrument(GetCOMInstrument(i), INSTRUMENT_KEITHLEY);
			switch (GetFonctionInstrument(i))
			{
			case CALO_V1_BP_V2_KEITHLEY:
				AppareilCalo.voie_mesure = INSTRUMENT_KEYTHLEY_V1;
				AppareilCalo.index = index_instr;
				AppareilBP.voie_mesure = INSTRUMENT_KEYTHLEY_V2;
				AppareilBP.index = index_instr;
				if (instrument[index_instr]->COM == 1)
				{
					synchCalo = PassageCOM1;
					synchBP = PassageCOM1;
				}
				else
				{
					synchCalo = PassageCOMs;
					synchBP = PassageCOMs;
				}
				break;

			case CALO_V1_HP_V2_KEITHLEY:
				AppareilCalo.voie_mesure = INSTRUMENT_KEYTHLEY_V1;
				AppareilCalo.index = index_instr;
				AppareilHP.voie_mesure = INSTRUMENT_KEYTHLEY_V2;
				AppareilHP.index = index_instr;
				if (instrument[index_instr]->COM == 1)
				{
					synchCalo = PassageCOM1;
					synchHP = PassageCOM1;
				}
				else
				{
					synchCalo = PassageCOMs;
					synchHP = PassageCOMs;
				}
				break;

			case CALO_V1_KEITHLEY:
				AppareilCalo.voie_mesure = INSTRUMENT_KEYTHLEY_V1;
				AppareilCalo.index = index_instr;
				if (instrument[index_instr]->COM == 1)
					synchCalo = PassageCOM1;
				else
					synchCalo = PassageCOMs;
				break;

			case INSTRUMENT_KEYTHLEY_LP_V2:
				AppareilBP.voie_mesure = INSTRUMENT_KEYTHLEY_V2;
				AppareilBP.index = index_instr;
				if (instrument[index_instr]->COM == 1)
					synchBP = PassageCOM1;
				else
					synchBP = PassageCOMs;
				break;

			case INSTRUMENT_KEYTHLEY_HP_V2:
				AppareilHP.voie_mesure = INSTRUMENT_KEYTHLEY_V2;
				AppareilHP.index = index_instr;
				if (instrument[index_instr]->COM == 1)
					synchHP = PassageCOM1;
				else
					synchHP = PassageCOMs;
				break;

			default:
				ASSERT(0); // Should never be reached
				break;
			}
			index_instr++;
			break;

		case INSTRUMENT_MENSOR:
			instrument[index_instr]->SetParametresInstrument(GetCOMInstrument(i), INSTRUMENT_MENSOR);
			if (GetFonctionInstrument(i) == INSTRUMENT_MENSOR_LP)
			{
				AppareilBP.voie_mesure = MENSOR_VOIE;
				AppareilBP.index = index_instr;
				if (instrument[index_instr]->COM == 1)
					synchBP = PassageCOM1;
				else
					synchBP = PassageCOMs;
			}
			else
			{
				AppareilHP.voie_mesure = MENSOR_VOIE;
				AppareilHP.index = index_instr;
				if (instrument[index_instr]->COM == 1)
					synchHP = PassageCOM1;
				else
					synchHP = PassageCOMs;
			}
			index_instr++;
			break;
		case INSTRUMENT_NONE:
		case INSTRUMENT_UNDEF:
		case INSTRUMENT_INEXIST:
		default:
			ASSERT(0); // Should never be reached
			break;
		}
	}
}

void Automation::InstrumentsOpen()
{
	for (int i = 0; i<NB_OF_INSTRUMENTS; i++)
		instrument[i]->OuvrirPortInstrument();
}

void Automation::InstrumentsClose()
{
	for (int i = 0; i<NB_OF_INSTRUMENTS; i++)
		instrument[i]->FermerPortInstrument();
}

// Function which makes sure everything is cancelled
void Automation::FinishExperiment(bool premature)
{
	// Destroy the events
	CloseHandle(h_MeasurementThreadStartEvent);

	// Destroy the critical sections
	DeleteCriticalSection(&criticalSection);

	// Close instruments
	InstrumentsClose();

	// Close valves/pump
	ControlMechanismsCloseAll();

	// Close measurement file
	FileMeasurementClose();

	if (premature) {
		// Experiment has been cancelled
		messageHandler.DisplayMessage(MESSAGE_EXPCANCEL);
	}
	else {
		// Experiment has been finished normally
		messageHandler.DisplayMessage(MESSAGE_EXPFINISH);
	}

	// Unlock the menu
	messageHandler.UnlockMenu();

	// Enable start button
	messageHandler.EnableStartButton();
}