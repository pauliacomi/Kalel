#include "StdAfx.h"
#include "Automation.h"



void Automation::Shutdown()
{
	switch (shutdownReason)
	{

	case STOP_CANCEL:		// This option is used if the experiment is cancelled
							// It then resets everything

		//messageHandler.DisplayMessageBox(PROMPT_CANCELEXP, MB_ICONQUESTION | MB_OKCANCEL, true);
		//::SetEvent(h_eventPause);

		//When thread finishes, let main window know to unlock menu
		messageHandler.ExperimentEnd();

		// Close measurement file
		FileMeasurementClose();

		// Stop all timers 
		timerExperiment.ArretTemps();
		timerMeasurement.ArretTemps();
		timerWaiting.ArretTemps();

		// Reset all data from the experiment
		experimentLocalData.ResetData();

		// Experiment has been cancelled
		messageHandler.DisplayMessage(MESSAGE_EXPCANCEL);

		break;

	case STOP_NORMAL:		// This option is used if the experiment finishes correctly
							// It then resets everything

		//When thread finishes, let main window know to unlock menu
		messageHandler.ExperimentEnd();

		// Close measurement file
		FileMeasurementClose();

		// Stop all timers 
		timerExperiment.ArretTemps();
		timerMeasurement.ArretTemps();
		timerWaiting.ArretTemps();

		// Reset all data from the experiment
		experimentLocalData.ResetData();

		// Experiment has been finished normally
		messageHandler.DisplayMessage(MESSAGE_EXPFINISH);

		break;

	default:
		ASSERT(0);		// Error, should never be reached
		break;
	}

	/*case ARRET_IMMEDIAT:
	case ARRET_SOUSVIDE:
		if (etape_perturbe != etape_en_cours)
		{
			message.Format(_T("Etape %s non effectuée\r\n"), etape);
			break;
		}
	case ARRET_ETAPE:
		message.Format(_T("Etape %s interrompue\r\n"), etape);
		break;
	case ARRET_DOSE:
		message.Format(_T("Dose interrompue dans l'étape %s\r\n"), etape);
		break;
	case ARRET_COMMANDE:
		message.Format(_T("Commande interrompue\r\n"));
		break;
	case ARRET_URGENCE_HP:
		if (etape_perturbe == etape_en_cours)
		{
			message.Format(_T("ARRET D'URGENCE : HAUTE PRESSION DEPASSANT LA PRESSION DE SECURITE : %f\r\n"),
				GetPressionSecuriteHautePression());
		}
		break;
	case ARRET_URGENCE_TCH:
		if (etape_perturbe == etape_en_cours)
		{
			message.Format(_T("ARRET D'URGENCE : TEMPERATURE DU CALORIMETRE TROP ELEVE PAR RAPPORT A LA TEMPERATURE D'EXPERIENCE : %d\r\n"),
				general.temperature_experience);
		}
		break;
	case ARRET_URGENCE_TCB:
		if (etape_perturbe == etape_en_cours)
		{
			message.Format(_T("ARRET D'URGENCE : TEMPERATURE DU CALORIMETRE TROP FAIBLE PAR RAPPORT A LA TEMPERATURE D'EXPERIENCE : %d\r\n"),
				general.temperature_experience);
		}
		break;
*/
	//	// Emmettre un son peut-être
	//	messageHandler.DisplayMessage(message);
	//}
}