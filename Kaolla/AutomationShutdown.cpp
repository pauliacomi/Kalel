#include "StdAfx.h"
#include "Automation.h"

int Automation::TemperatureStop() {
//	if (g_flagState == ARRET_ETAPE) { ShutdownDisplay(); g_flagState = INACTIF; return 0; }
//	if (g_flagState == ARRET_IMMEDIAT) { ShutdownDisplay(); return 0; }
//	if (g_flagState == ARRET_SOUSVIDE) { ShutdownDisplay(); return 0; }
//	if (g_flagState == ARRET_URGENCE_HP) { ShutdownDisplay(); return 0; }
//	if (g_flagState == ARRET_URGENCE_TCH) { ShutdownDisplay(); return 0; }
//	if (g_flagState == ARRET_URGENCE_TCB) { ShutdownDisplay(); return 0; }
//	Pause();
	return 0;
}

void Automation::Shutdown()
{
	//CString etape, message;
	//if (demande_arret != INACTIF) // Alors �criture dans editMessage pour pr�venir d'un arret
	//{
	//	int i;
	//	for (i = 0; i<nb_etape; i++)
	//	{
	//		if (etape_en_cours == ListeEtape[i].Index)
	//		{
	//			etape = ListeEtape[i].Nom;
	//			break;
	//		}
	//	}

	//	if (i == nb_etape)
	//		etape = _T("non d�finie");

	switch (shutdownReason)
	{
	/*case ARRET_IMMEDIAT:
	case ARRET_SOUSVIDE:
		if (etape_perturbe != etape_en_cours)
		{
			message.Format(_T("Etape %s non effectu�e\r\n"), etape);
			break;
		}
	case ARRET_ETAPE:
		message.Format(_T("Etape %s interrompue\r\n"), etape);
		break;
	case ARRET_DOSE:
		message.Format(_T("Dose interrompue dans l'�tape %s\r\n"), etape);
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
	default:
		running = false;
		//message.Format(_T("Arret non d�fini de %s\r\n"), etape);
		break;
	}

	//	// Emmettre un son peut-�tre
	//	messageHandler.DisplayMessage(message);
	//}
}

void Automation::Pause()
{
	// Start threads and read the data
	ThreadMeasurement();

	// Do the security checks
	SecuriteTemperatures();
	SecuriteHautePression();

	// Send the data to be saved outside of the function
	messageHandler.ExchangeData(experimentLocalData);

	// Switch to see if the thread is still inactive
	HANDLE objects[2];
	objects[0] = h_eventShutdown;
	objects[1] = h_eventResume;

	switch (::WaitForMultipleObjects(2, objects, FALSE, T_BETWEEN_MEASURE)) // (ms) Poll time
	{
	case WAIT_TIMEOUT:
		g_flagState = PAUSE;
		break;
	case WAIT_OBJECT_0:
		g_flagState = STOP;
		break;
	case WAIT_OBJECT_0 + 1:
		g_flagState = ACTIVE;
		if (experimentLocalData.experimentInProgress)
		{
			messageHandler.DisplayMessage(MESSAGE_EXPRESUME);
			timerExperiment.RepriseTemps();
		}
		::ResetEvent(h_eventResume);	// Reset the event
		break;
	default:
		ASSERT(FALSE); // unknown error
		break;
	}
}

void Automation::Inactive()
{
	// Start threads and read the data
	ThreadMeasurement();

	// Do the security checks
	SecuriteTemperatures();
	SecuriteHautePression();

	if (experimentLocalData.experimentRecording									// If we started recording
		&& timerMeasurement.TempsActuel() > T_BETWEEN_RECORD)					// and the enough time between measurements
	{
		// Restart the timer to record time between measurements
		timerMeasurement.TopChrono();

		// Save the time at which the measurement took place
		experimentLocalData.experimentTime = timerExperiment.TempsActuel();

		// Save the data to the file
		EnregistrementFichierMesures();

		// Increment the measurement number
		experimentLocalData.experimentMeasurements++;
	}

	// Send the data to be saved outside of the function
	messageHandler.ExchangeData(experimentLocalData);

	// If the wait functionality is requested, check to see if we can continue
	if (timerWaiting.TempsActuel() > experimentLocalData.timeToEquilibrate) {
		waiting = false;
	}

	// Switch to see if the thread is still inactive
	HANDLE objects[2];
	objects[0] = h_eventShutdown;
	objects[1] = h_eventPause;

	switch (::WaitForMultipleObjects(2, objects, FALSE, T_BETWEEN_MEASURE)) // (ms) Poll time
	{
	case WAIT_TIMEOUT:
		if (waiting) {
			g_flagState = INACTIVE;
		}
		else{
			g_flagState = ACTIVE;
		}
		break;
	case WAIT_OBJECT_0:
		g_flagState = STOP;
		break;
	case WAIT_OBJECT_0 + 1:
		g_flagState = PAUSE;
		if (experimentLocalData.experimentInProgress)
		{
			messageHandler.DisplayMessage(MESSAGE_EXPPAUSE);
			timerExperiment.ArretTemps();
		}
		::ResetEvent(h_eventPause);	// Reset the event
		break;
	default:
		ASSERT(FALSE); // unknown error
		break;
	}
}
