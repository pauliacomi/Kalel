#include "stdafx.h"
#include "Automation.h"



Automation::Automation()
	:running(true)
	,waiting(false)
{
}


Automation::~Automation()
{
}


void Automation::Execution()
{
	// Initialise class members
	Initialisation();
	
	// Infinite loop, it is broken from the inside
	while (running)
	{
		switch (g_flagState)		// We look at the main flag
		{
		case STOP:						// In case the experiment is asked to stop completely
			Shutdown();
			break;

		case INACTIVE:					// In case the experiment is not doing anything
			Inactive();
			break;

		case PAUSE:						// In case the experiment is measuring in the background
			Pause();
			break;
			
		case ACTIVE:					// In case the experiment is started and underway
			
			// Check if the data is the same as the old one, if so then get it
			if (DataIsNew()) {
				SetData();
				RecordDataChange();
			}

			switch (experimentLocalSettings.experimentType)		// We look at the type of experiment
			{
				case EXPERIMENT_TYPE_MANUAL:					// in case it is manual
					if (ExecutionManual())						// run the manual loop
						continue;
					break;
				case EXPERIMENT_TYPE_AUTO:						// in case it is automatic
					if (ExecutionAuto())						// run the automatic loop
						continue;
					break;
				default:
					ASSERT(0); // Error
					break;
			}
			break;
		
		default:
			ASSERT(0); // Error
			break;
		}
	}

	FinishExperiment(false);
}



bool Automation::ExecutionManual()
{
	switch (experimentLocalData.experimentStepStatus)
	{
	case STEP_STATUS_START:

		// Send start messages
		messageHandler.DisplayMessage(MESSAGE_FILLLINE);
		messageHandler.DisplayMessage(MESSAGE_EXPSTART);

		// Record start
		experimentLocalData.experimentInProgress = true; 
		experimentLocalData.experimentRecording = true;

		// Create open and write the columns in the:
		EcritureEntete();				// Entete TXT
		EcritureEnteteCSV();			// Entete CSV
		FileMeasurementOpen();			// Measurement file

		timerExperiment.TopChrono();	// Start global experiment timer	
		timerMeasurement.TopChrono();	// Start the timer to record time between measurements

		// Continue experiment
		experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;
		g_flagState = INACTIVE;

		break;

	case STEP_STATUS_INPROGRESS:
		g_flagState = INACTIVE;
		break;

	case STEP_STATUS_END:
		break;

	default:
		ASSERT(0);
		break;
	}
	
	return true;
}

bool Automation::ExecutionAuto()
{
	switch (experimentLocalData.experimentStage)
	{
	case STAGE_VERIFICATIONS:
		Verifications();
		break;
	case STAGE_EQUILIBRATION:
		StageEquilibration();
		break;
	case STAGE_ADSORPTION:
		StageAdsorption();
		break;
	case STAGE_DESORPTION:
		StageDesorption();
		break;
	case STAGE_CONTINUOUS_ADSORPTION:
		break;
	case STAGE_END_AUTOMATIC:
		break;
	default:
		ASSERT(0); // Error
		break;
	}
	return true;
}


void Automation::Initialisation()
{
	// Initialise threads
	h_MeasurementThread[0] = NULL;
	h_MeasurementThread[1] = NULL;
	h_MeasurementThread[2] = NULL;
	h_MeasurementThread[3] = NULL;

	// Initialise automatic variables
	experimentLocalData.experimentDose = 0;
	experimentLocalData.experimentTime = 0;
	experimentLocalData.experimentMeasurements = 1;
	experimentLocalData.experimentStage = STAGE_VERIFICATIONS;
	experimentLocalData.experimentSubstepStage = STEP_STATUS_START;

	// Initialise data
	SetData();
	messageHandler.SetHandle(experimentLocalSettings.GUIhandle);

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
	h_eventResume = CreateEvent(NULL, TRUE, FALSE, NULL);
	h_eventPause = CreateEvent(NULL, TRUE, FALSE, NULL);

	// Put the thread in a paused state
	g_flagState = PAUSE;
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
	CloseHandle(h_eventShutdown);
	CloseHandle(h_eventResume);
	CloseHandle(h_eventPause);
	
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

void Automation::SetDataPointer(ExperimentSettings * eSettings)
{
	// Store the pointer
	experimentSettings = eSettings;
}

void Automation::SetData()
{
	// Copy the data from the main thread
	EnterCriticalSection(&experimentSettings->criticalSection);
	experimentLocalSettings = experimentSettings;
	experimentSettings->dataModified = false;
	LeaveCriticalSection(&experimentSettings->criticalSection);
}

bool Automation::DataIsNew()
{
	// Copy the data from the main thread
	bool check = false;

	EnterCriticalSection(&experimentSettings->criticalSection);
	if (experimentSettings->dataModified == false)
		check = false;
	else
		check = true;
	LeaveCriticalSection(&experimentSettings->criticalSection);

	return check;
}