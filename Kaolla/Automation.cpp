#include "stdafx.h"
#include "Automation.h"



Automation::Automation()
	: running(true)
	, checking(true)
{
	g_pTemperature = new CTemperature;

}


Automation::~Automation()
{
	DeInitialise();
}

////////////////////////////////////////////////////////
//	Main execution function
//
//	This is an infinite loop which has to be broken form inside
//	The general flowchart goes as follows:
//	
//	initialize everything
//	loop
//	{
//		1. Get the experiment settings if they are new
//		2. Run through the automation algorithm for the chosen program (nothing, manual, automatic, vacuum, etc)
//		3. Measure values from instruments
//		4. Do a security and safety check on the values
//		5. Record the time the measurement was done / the time between measurments
//		6. IF RECORDING, save the data to the file, restart timer between measurements and increment measurement number
//		7. IF WAITING, check whether the wait is complete and reset the wait
//		8. Display the data to the GUI
//		9. 
//	}
//
//
//
//
//
//
////////////////////////////////////////////////////////
void Automation::Execution()
{
	// Initialise class members
	Initialisation();

	// Infinite loop, it is broken from the inside
	while (running)
	{
		// Check if there is any change in the experiment settings
		if (DataIsNew()) {
			SetData();
			RecordDataChange();
		}

		if (experimentLocalData.experimentCommandsRequested) {
			switch (experimentLocalSettings.experimentType)		// We look at the type of experiment
			{
			case EXPERIMENT_TYPE_MANUAL:						// in case it is manual
				ExecutionManual();								// run the manual loop
				break;
			case EXPERIMENT_TYPE_AUTO:							// in case it is automatic
				ExecutionAuto();								// run the automatic loop
				break;
			case EXPERIMENT_TYPE_SAMPLE_VACUUM:					// in case we want to vacuum up to the sample
				SampleVacuum();									// run the functionality
				break;
			case EXPERIMENT_TYPE_BOTTLE_VACUUM:					// in case we want to vacuum up to the bottle
				BottleVacuum();									// run the functionality
				break;
			case EXPERIMENT_TYPE_UNDEF:							// in case no experiment has been set yet
				break;											// just continue
			default:
				ASSERT(0); // Error
				break;
			}
		}

		// Start threads and read the data
		ThreadMeasurement();

		// Do the security checks
		SecuriteTemperatures();
		SecuriteHautePression();

		// Record time
		experimentLocalData.experimentTime = timerExperiment.TempsActuel();			// Save the time at which the measurement took place
		experimentLocalData.timeToEquilibrateCurrent = timerWaiting.TempsActuel();	// Save the waiting time if it exists

		// Write data
		if (experimentLocalData.experimentRecording	&&								// If we started recording
			timerMeasurement.TempsActuel() > T_BETWEEN_RECORD)						// and the enough time between measurements
		{
			// Save the data to the file
			EnregistrementFichierMesures();

			// Restart the timer to record time between measurements
			timerMeasurement.TopChrono();
			
			// Increment the measurement number
			experimentLocalData.experimentMeasurements++;
		}

		// If waiting complete
		if (experimentLocalData.experimentWaiting &&														// If the wait functionality is requested																					
			experimentLocalData.timeToEquilibrateCurrent > experimentLocalData.timeToEquilibrate) {			//and the time has been completed

			// Stop the timer
			timerWaiting.ArretTemps();

			// Reset the flag
			experimentLocalData.experimentWaiting = false;
		}

		// Send the data to be displayed to the GUI
		messageHandler.ExchangeData(experimentLocalData);
		
		if (checking)
		{
			// Switch to see if the thread is still inactive
			switch (::WaitForMultipleObjects(4, events, FALSE, T_BETWEEN_MEASURE)) // (ms) Poll time
			{

			case WAIT_OBJECT_0:					// Complete stop of thread, might need extra things
				running = false;
				::ResetEvent(h_eventShutdown);	// Reset the event
				break;

			case WAIT_OBJECT_0 + 1:
				if (experimentLocalData.experimentInProgress)
				{
					timerExperiment.ArretTemps();
					timerMeasurement.ArretTemps();
					timerWaiting.ArretTemps();
					experimentLocalData.experimentRecording = false;

					messageHandler.DisplayMessage(MESSAGE_EXPPAUSE);
				}
				experimentLocalData.experimentCommandsRequested = false;
				::ResetEvent(h_eventPause);	// Reset the event
				break;

			case WAIT_OBJECT_0 + 2:
				if (experimentLocalData.experimentInProgress)
				{
					timerExperiment.RepriseTemps();
					timerMeasurement.RepriseTemps();
					timerWaiting.RepriseTemps();
					experimentLocalData.experimentRecording = true;

					messageHandler.DisplayMessage(MESSAGE_EXPRESUME);
				}
				experimentLocalData.experimentCommandsRequested = true;
				::ResetEvent(h_eventResume);	// Reset the event
				break;

			case WAIT_OBJECT_0 + 3:
				Shutdown();
				::ResetEvent(h_eventReset);	// Reset the event
				break;

			case WAIT_TIMEOUT:
				break;

			default:
				ASSERT(FALSE); // unknown error
				break;
			}
		}
	}
}


bool Automation::ExecutionManual()
{
	if (experimentLocalData.experimentStepStatus == STEP_STATUS_UNDEF) {

		// Send start messages
		messageHandler.DisplayMessage(MESSAGE_FILLLINE);
		messageHandler.DisplayMessage(MESSAGE_EXPSTART);
		messageHandler.ExperimentStart();

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
		experimentLocalData.experimentStage = STAGE_MANUAL;
		experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;
		experimentLocalData.experimentCommandsRequested = false;
		
		return true;
	}

	return false;
}



bool Automation::ExecutionAuto()
{
	if (experimentLocalData.experimentStepStatus == STEP_STATUS_UNDEF){

		// Send start messages
		messageHandler.DisplayMessage(MESSAGE_FILLLINE);
		messageHandler.DisplayMessage(MESSAGE_EXPSTART);

		experimentLocalData.experimentInProgress = true;
		experimentLocalData.experimentStage = STAGE_VERIFICATIONS;
		experimentLocalData.experimentStepStatus = STEP_STATUS_START;
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_START;
		experimentLocalData.verificationStep = STEP_VERIFICATIONS_SECURITY;
	}

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

		// If the experiment has finished
		shutdownReason = STOP_NORMAL;	// set a normal shutdown
		::SetEvent(h_eventReset);	// end then set the event

		break;
	default:
		ASSERT(0); // Error
		break;
	}

	return true;
}

// Initialisation of all variables
void Automation::Initialisation()
{
	// Initialise threads
	h_MeasurementThread[0] = NULL;
	h_MeasurementThread[1] = NULL;
	h_MeasurementThread[2] = NULL;
	h_MeasurementThread[3] = NULL;

	// Initialise automatic variables
	experimentLocalData.ResetData();
	experimentLocalSettings.experimentType = EXPERIMENT_TYPE_UNDEF;	// reset function?

	// Initialise data
	SetData();
	messageHandler.SetHandle(experimentLocalSettings.GUIhandle);

	// Initialise instruments
	InitialisationInstruments();

	// Open instruments
	InstrumentsOpen();

	// Initialise security
	InitialisationSecurityManual();

	// Initialisation of the critical section
	InitializeCriticalSection(&criticalSection);

	// Create the events
	//   - Non signalled by default
	//   - With manual reinitiallisation
	h_MeasurementThreadStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	h_eventShutdown = CreateEvent(NULL, TRUE, FALSE, NULL);
	h_eventResume = CreateEvent(NULL, TRUE, FALSE, NULL);
	h_eventPause = CreateEvent(NULL, TRUE, FALSE, NULL);
	h_eventReset = CreateEvent(NULL, TRUE, FALSE, NULL);
	events[0] = h_eventShutdown;
	events[1] = h_eventPause;
	events[2] = h_eventResume;
	events[3] = h_eventReset;

	// If the shutdown event is called externally, it will default to a cancel
	// Otherwise the flag will be changed from inside the code
	shutdownReason = STOP_CANCEL;
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

// Function which makes sure everything is shutdown gracefully
void Automation::DeInitialise()
{
	// Close valves/pump
	ControlMechanismsCloseAll();

	// Close instruments
	InstrumentsClose();

	// Delete instruments
	for (int i = 0; i < NB_OF_INSTRUMENTS; i++) {
		delete instrument[i];
	}

	// Destroy the events
	CloseHandle(h_MeasurementThreadStartEvent);
	CloseHandle(h_eventShutdown);
	CloseHandle(h_eventResume);
	CloseHandle(h_eventPause);
	CloseHandle(h_eventReset);
	
	// Destroy the critical sections
	DeleteCriticalSection(&criticalSection);

	// Delete temperature class
	delete g_pTemperature;
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
	//g_pTemperature = pTemperature;
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
	experimentSettings->continueAnyway = false;
	LeaveCriticalSection(&experimentSettings->criticalSection);
}

bool Automation::DataIsNew()
{
	// Copy the data from the main thread
	bool check = false;

	EnterCriticalSection(&experimentSettings->criticalSection);
	if (experimentSettings->dataModified == false)
		check = false;
	else {
		check = true;
	}
	LeaveCriticalSection(&experimentSettings->criticalSection);

	return check;
}