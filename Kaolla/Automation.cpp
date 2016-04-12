

#include "stdafx.h"
#include "Automation.h"



Automation::Automation()
{
}


Automation::~Automation()
{
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

void Automation::SetData(ExperimentData * eData)
{
	experimentData = eData;
}

void Automation::Execution()
{
	if (experimentData->experimentType == EXPERIMENT_TYPE_MANUAL)
		ExecutionManual();
	if (experimentData->experimentType == EXPERIMENT_TYPE_AUTO)
		ExecutionAuto();
}

void Automation::ExecutionManual()
{
	// Send start message
	messageHandler.DisplayMessage(MESSAGE_FILLLINE);
	messageHandler.DisplayMeasurement(MESSAGE_EXPSTART);

	// Initialise
	Initialisation();

	// Record start in global bool
	manip_en_cours = TRUE;
	experimentData->experimentMeasurement = 1;

	// Create files
	EcritureEntete();				// Entete
	EcritureEnteteCSV();			// Entete CSV
	OuvertureFichierMesures();		// Measurement file
	
	// Start timer
	timerExperiment.TopChrono();

	// Boucle indéfinie, elle est 'breakée' de l'intérieure
	while (TRUE)
	{
		// Start the timer to record time between measurements
		timerMeasurement.TopChrono();

		// Start threads and read the data
		ThreadMesures();

		// Save the time at which the measurement took place
		experimentData->experimentTime = timerExperiment.TempsActuel();

		// Send the data to be saved outside of the function
		messageHandler.ExchangeData();

		// Display to the
		GraphAddMeasurement();	//graph
		AffichageMesures();		// textbox

		// Save the data to the file
		EnregistrementFichierMesures();

		DonneesManuelleGrapheEtape();	// saves the data to the graph again??

		// Launches a tread in the main function
		::PostMessage((HWND)pParam, WM_THREADAFFICHAGE, 0, 0);


		// On regarde si g_eventKill est activé sans attendre
		//	- S'il est activé, alors on arrête cette boucle while
		//	- Sinon on refait un tour dans cette boucle while

		// Increment the measurement number
		experimentData->experimentMeasurement ++;

		// Now check if the experiment has not been stopped from the main thread
		if (manip_en_cours == FALSE)
		{
			DWORD TempsAttente = 20000; // ms
			::WaitForSingleObject(g_eventFinAffichage, TempsAttente);	// wtf? really?
			InstrumentsClose();
			ToutFermer();
			ReinitialisationManuelle();
			FermetureFichierMesures();
			messageHandler.UnlockMenu();
			messageHandler.DisplayMessage(_T("Expérience terminée\r\n"));
			messageHandler.EnableStartButton();
			break;
		}

		// Wait some time between two individual measurements
		do {
			if (timerMeasurement.TempsActuel() < T_BETWEEN_MEASURE)
			{
				if (manip_en_cours == FALSE)
					break;
				Sleep(100); // mettre if (manip_en_cours == FALSE) {...}
			}
			else
				break;
		} while (TRUE);
	}
}

void Automation::ExecutionAuto()
{
	// Initialise the instruments, might be redundant if it is already called at the start of the creation
	InitialisationManip();

	// Create, open and write columns to the measurement file
	OuvertureFichierMesures();

	// Create, open and write instrument parameters to CSV file
	EcritureEntete();
	EcritureEnteteCSV();

	// Initialise automatic variables
	experimentData->experimentDose = 0;
	injection = 0;
	experimentData->experimentTime = 0;
	experimentData->experimentMeasurement = 1;
	demande_arret = INACTIF;
	etape_en_cours = STAGE_UNDEF;


	if (Verifications() == IDCANCEL)
	{
		messageHandler.DisplayMessage(_T("Expérience annulée\r\n"));
		//GREY_OUT les boutons

		InstrumentsClose();

		FermetureFichierMesures();
		messageHandler.UnlockMenu();

		return;
	}

	// Start timer
	timerExperiment.TopChrono();

	// Equilibrate
	LigneBaseEtEquilibre(pParam);


	// Start going through doses and perform the required ones

	if (adsorption_continue.a_effectuer && ContinuerExperience())
		AdsorptionContinue(pParam);

	//if (petites_doses.a_effectuer && ContinuerExperience())
	if (PetitesDosesAEffectuer())
		PetitesDoses(pParam);

	//if (moyennes_doses.a_effectuer && ContinuerExperience())
	if (MoyennesDosesAEffectuer())
		MoyennesDoses(pParam);

	//if (grandes_doses.a_effectuer && ContinuerExperience())
	if (GrandesDosesAEffectuer())
		GrandesDoses(pParam);

	//if (desorption.a_effectuer && ContinuerExperience())
	if (DesorptionAEffectuer())
		Desorption(pParam);

	if (divers.mise_sous_vide_fin_experience || demande_arret == ARRET_SOUSVIDE ||
		demande_arret == ARRET_URGENCE_HP || demande_arret == ARRET_URGENCE_TCH || demande_arret == ARRET_URGENCE_TCB)
		MiseSousVide(pParam);


	InstrumentsClose();

	ToutFermer();

	FermetureFichierMesures();

	ReinitialisationAuto();

	messageHandler.UnlockMenu();
	messageHandler.DisplayMessage(MESSAGE_EXPFINISH);
}


void Automation::Initialisation()
{
	InitialisationInstruments();
	InitialisationSecurityManual();
	InstrumentsOpen();
}


// The instruments which the calorimeter uses are defined here
// It looks good, might need some error detection.
void Automation::InitialisationInstruments()
{
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