#include "StdAfx.h"
#include "Automation.h"

int Automation::TemperatureStop() {
	if (g_flagAskShutdown == ARRET_ETAPE) { ShutdownDisplay(); g_flagAskShutdown = INACTIF; return 0; }
	if (g_flagAskShutdown == ARRET_IMMEDIAT) { ShutdownDisplay(); return 0; }
	if (g_flagAskShutdown == ARRET_SOUSVIDE) { ShutdownDisplay(); return 0; }
	if (g_flagAskShutdown == ARRET_URGENCE_HP) { ShutdownDisplay(); return 0; }
	if (g_flagAskShutdown == ARRET_URGENCE_TCH) { ShutdownDisplay(); return 0; }
	if (g_flagAskShutdown == ARRET_URGENCE_TCB) { ShutdownDisplay(); return 0; }
	Pause();
}

void Automation::ShutdownDisplay()
{
	//CString etape, message;
	//if (demande_arret != INACTIF) // Alors écriture dans editMessage pour prévenir d'un arret
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
	//		etape = _T("non définie");

	//	switch (demande_arret)
	//	{
	//	case ARRET_IMMEDIAT:
	//	case ARRET_SOUSVIDE:
	//		if (etape_perturbe != etape_en_cours)
	//		{
	//			message.Format(_T("Etape %s non effectuée\r\n"), etape);
	//			break;
	//		}
	//	case ARRET_ETAPE:
	//		message.Format(_T("Etape %s interrompue\r\n"), etape);
	//		break;
	//	case ARRET_DOSE:
	//		message.Format(_T("Dose interrompue dans l'étape %s\r\n"), etape);
	//		break;
	//	case ARRET_COMMANDE:
	//		message.Format(_T("Commande interrompue\r\n"));
	//		break;
	//	case ARRET_URGENCE_HP:
	//		if (etape_perturbe == etape_en_cours)
	//		{
	//			message.Format(_T("ARRET D'URGENCE : HAUTE PRESSION DEPASSANT LA PRESSION DE SECURITE : %f\r\n"),
	//				GetPressionSecuriteHautePression());
	//		}
	//		break;
	//	case ARRET_URGENCE_TCH:
	//		if (etape_perturbe == etape_en_cours)
	//		{
	//			message.Format(_T("ARRET D'URGENCE : TEMPERATURE DU CALORIMETRE TROP ELEVE PAR RAPPORT A LA TEMPERATURE D'EXPERIENCE : %d\r\n"),
	//				general.temperature_experience);
	//		}
	//		break;
	//	case ARRET_URGENCE_TCB:
	//		if (etape_perturbe == etape_en_cours)
	//		{
	//			message.Format(_T("ARRET D'URGENCE : TEMPERATURE DU CALORIMETRE TROP FAIBLE PAR RAPPORT A LA TEMPERATURE D'EXPERIENCE : %d\r\n"),
	//				general.temperature_experience);
	//		}
	//		break;
	//	default:
	//		message.Format(_T("Arret non défini de %s\r\n"), etape);
	//		break;
	//	}

	//	// Emmettre un son peut-être
	//	messageHandler.DisplayMessage(message);
	//}
}

void Automation::Pause()
{
	//if (g_flagAskShutdown == PAUSE)
	//{
	//	messageHandler.DisplayMessage(_T("Pause\r\n"));
	//	messageHandler.DisplaAddStep(_T("  |  Pause"));
	//	temps_intermediaire.ArretTemps();
	//	WaitForSingleObject(EventReprise, INFINITE);
	//	//UN_GREY_OUT
	//	temps_intermediaire.RepriseTemps();
	//	messageHandler.DisplayMessage(_T("Reprise\r\n"));
	//	messageHandler.DisplayPreviousStep();
	//}
}