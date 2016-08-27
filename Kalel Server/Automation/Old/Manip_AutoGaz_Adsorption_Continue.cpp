#include "StdAfx.h"
#include "Manip_AutoGaz.h"

using namespace std;


HANDLE hEventDemandeOuverture, hEventReceptionDemande, hEventConfirmationFin;
HANDLE hThreadDemandeOuverture;
DWORD ThreadDemandeOuvertureId;



void CManip_AutoGaz::AdsorptionContinue(LPVOID pParam)
{
	//FermetureDeToutesLesVannes();
	ToutFermer();
	// Fermer EV1, EV2 et la pompe
	messageHandler.DisplayMessage(_T("Début de l'adsorption continue\r\n"));
	messageHandler.DisplayStep(_T("Adsorption Continue"));
	CString MessageInBox;
	MessageInBox = _T("Vérifier que toutes les électrovannes sont en automatique\t\n");
	MessageInBox += _T("et que LA VANNE DE L'AMPOULE EST FERMEE\t");

	AfxMessageBox(MessageInBox, MB_ICONWARNING | MB_OK);

	EquilibreInitialeDuVideDynamique(pParam);
	PointArretSansDoseFonction();
	CalibrationDuDebit(pParam);
	PointArretSansDoseFonction();
	CalibrationDuVolumeMort(pParam);
	PointArretSansDoseFonction();
	EquilibreDuVideDynamiqueAvantAdsorption(pParam);
	PointArretSansDoseFonction();
	DemandeOuvertureVanneAmpoule(pParam);
	PointArretSansDoseFonction();
	ExperienceAdsorption(pParam);
	PointArretSansDoseFonction();
	EquilibreFinAdsorption(pParam);
	PointArretSansDoseFonction();

	messageHandler.DisplayStep(_T("Adsorption Continue : Fin"));
	messageHandler.DisplayMessage(_T("Fin de l'adsorption continue\r\n"));
}



void CManip_AutoGaz::EquilibreInitialeDuVideDynamique(LPVOID pParam)
{
	messageHandler.DisplayMessage(_T("Equilibre initiale du volume du vide dynamique\r\n"));
	CString nom_etape = _T("Adsorption Continue : Equilibre initiale du volume du vide dynamique");

	// Mettre en route la pompe
	MettreEnRouteLaPompe();

	messageHandler.DisplayMessage(_T("Attente de 5 secondes\r\n"));
	//Sleep(5000);
	AttenteSecondes(5);
	Ouverture_Vanne(8); Sleep(500);
	Ouverture_Vanne(7); Sleep(500);
	Ouverture_Vanne(4); Sleep(500);
	Ouverture_Vanne(3); Sleep(500);
	Ouverture_Vanne(2);
	// Attendre 5 secondes
	messageHandler.DisplayMessage(_T("Attente de 5 secondes\r\n"));
	//Sleep(5000);
	AttenteSecondes(5);

	LireAfficherHautePression();
	if (resultat_hp < GetPressionSecuriteBassePression()) // mettre message
		Ouverture_Vanne(6);
	LireEcrireAfficher(adsorption_continue.temps_equilibre_continue, nom_etape, pParam);
}



void CManip_AutoGaz::CalibrationDuDebit(LPVOID pParam)
{
	messageHandler.DisplayMessage(_T("Calibration du débit\r\n"));
	CString nom_etape = _T("Adsorption Continue : Calibration du débit");

	Fermeture_Vanne(7); Sleep(500);
	Fermeture_Vanne(8);
	ArreterLaPompe();


	LireEcrireAfficher(adsorption_continue.temps_etalonnage_debit, nom_etape, pParam);
}



void CManip_AutoGaz::CalibrationDuVolumeMort(LPVOID pParam)
{
	messageHandler.DisplayMessage(_T("Calibration du volume mort\r\n"));
	CString nom_etape = _T("Adsorption Continue : Calibration du volume mort");

	Ouverture_Vanne(5);

	LireEcrireAfficher(adsorption_continue.temps_etalonnage_volume_inter, nom_etape, pParam);
}



void CManip_AutoGaz::EquilibreDuVideDynamiqueAvantAdsorption(LPVOID pParam)
{
	messageHandler.DisplayMessage(_T("Equilibre du vide dynamique avant adsorption\r\n"));
	CString nom_etape = _T("Adsorption Continue : Equilibre du vide dynamique avant adsorption");

	// Mettre en route la pompe
	MettreEnRouteLaPompe();

	messageHandler.DisplayMessage(_T("Attente de 5 secondes\r\n"));
	//Sleep(5000);
	AttenteSecondes(5);

	Ouverture_Vanne(8); Sleep(500);
	Ouverture_Vanne(7);

	// Attendre 5 secs
	messageHandler.DisplayMessage(_T("Attente de 5 secondes\r\n"));
	//Sleep(5000);
	AttenteSecondes(5);


	LireEcrireAfficher(adsorption_continue.temps_equilibre_continue, nom_etape, pParam);
}



void CManip_AutoGaz::DemandeOuvertureVanneAmpoule(LPVOID pParam)
{
	messageHandler.DisplayMessage(_T("Demande d'ouverture de la vanne de l'ampoule\r\n"));
	CString nom_etape = _T("Adsorption Continue : Demande d'ouverture de la vanne de l'ampoule");
	messageHandler.DisplayStep(nom_etape);


	hEventDemandeOuverture = CreateEvent(NULL, TRUE, FALSE, NULL);
	hEventReceptionDemande = CreateEvent(NULL, TRUE, FALSE, NULL);
	hEventConfirmationFin = CreateEvent(NULL, TRUE, FALSE, NULL);

	hThreadDemandeOuverture = CreateThread(NULL, NULL, ThreadAttenteDemande, this, NULL, &ThreadDemandeOuvertureId);
	SetEvent(hEventDemandeOuverture);




	AfxMessageBox("Ouvrir doucement la vanne de l'ampoule.\t\n", MB_OK | MB_ICONWARNING);

	// Lancer Event pour arrêter le thread
	SetEvent(hEventReceptionDemande);

	WaitForSingleObject(hEventConfirmationFin, INFINITE);
}



void CManip_AutoGaz::ExperienceAdsorption(LPVOID pParam)
{
	messageHandler.DisplayMessage(_T("Expérience d'adsorption\r\n"));
	CString nom_etape = _T("Adsorption Continue : Expérience d'adsorption");
	messageHandler.DisplayStep(nom_etape);

	Fermeture_Vanne(7); Sleep(500);
	Fermeture_Vanne(8);
	// Arrêter la pompe
	ArreterLaPompe();

	bool continuer;
	do
	{
		continuer = LireEcrireAfficher(pParam);
		// Point arret
		// PointArretSousFonction();
	} while (resultat_hp < adsorption_continue.pression_finale_adsorption_continue
		&& continuer == TRUE);

}



void CManip_AutoGaz::EquilibreFinAdsorption(LPVOID pParam)
{
	messageHandler.DisplayMessage(_T("Equilibre en fin d'adsorption\r\n"));
	CString nom_etape = _T("Adsorption Continue : Equilibre en fin d'adsorption");
	Fermeture_Vanne(4); Sleep(500);
	Fermeture_Vanne(3); Sleep(500);
	Fermeture_Vanne(2);
	LireEcrireAfficher(adsorption_continue.temps_final_equilibre, nom_etape, pParam);
	FermetureDeToutesLesVannes();
}



DWORD WINAPI CManip_AutoGaz::ThreadAttenteDemande(LPVOID lpParam)
{
	// Attend l'activation de hEventAuto indéfiniment
	WaitForSingleObject(hEventDemandeOuverture, INFINITE);

	CManip_AutoGaz *manipulation = reinterpret_cast<CManip_AutoGaz *>(lpParam);

	do {
		manipulation->LireEcrireAfficher(lpParam);
	} while (WaitForSingleObject(hEventReceptionDemande, 100) == WAIT_TIMEOUT);
	SetEvent(hEventConfirmationFin);

	return 0;
}
