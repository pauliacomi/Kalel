#include "StdAfx.h"
#include "Manip.h"


using namespace std;


CEvent g_eventFinAffichage;



void CManip::ExecutionManuelle(LPVOID pParam)
{
	AffichageMessages(_T("D�but de l'exp�rience\r\n"));
	AffichageMesures(_T("***************************************************************\r\n"));

	InitialisationManip();

	RecuperationDonnees();

	InitialisationMesureSecuriteManuelle();

	manip_en_cours = TRUE;
	numero_mesure=1;


	RecuperationDonnees();

	OuvertureFichierMesures();

	EcritureEntete();
	EcritureEnteteCSV();

	//temps_manip = 0; // On initialise le temps � 0?
	temps_experience.TopChrono();


	// Boucle ind�finie, elle est 'break�e' de l'int�rieure
	while (TRUE)
	{	
		temps_intermediaire.TopChrono();

		ThreadMesures();

		temps_manip=temps_experience.TempsActuel();

		EchangeDonnees();
		RajoutMesure();
		AffichageMesures();
		EnregistrementFichierMesures();
		DonneesManuelleGrapheEtape();

		// Lancement du thread : OnThreadAffichage
		::PostMessage((HWND) pParam, WM_THREADAFFICHAGE, 0, 0);


		// On regarde si g_eventKill est activ� sans attendre
		//	- S'il est activ�, alors on arr�te cette boucle while
		//	- Sinon on refait un tour dans cette boucle while

		numero_mesure++;


		if (manip_en_cours == FALSE) 
		{
			DWORD TempsAttente = 20000; // ms
			::WaitForSingleObject(g_eventFinAffichage, TempsAttente);
			FermerInstruments();
			//FermetureDeToutesLesVannes();
			ToutFermer();
			ReinitialisationManuelle();
			FermetureFichierMesures();
			DebloqueMenu();
			AffichageMessages(_T("Exp�rience termin�e\r\n"));
			RemettreBoutonLancer();
			break;
		}

		
		do{
			if(temps_intermediaire.TempsActuel() < T_BETWEEN_MEASURE)
			{
				if (manip_en_cours == FALSE)
					break;
				Sleep(100); // mettre if (manip_en_cours == FALSE) {...}
			}
			else
				break;
		}while(TRUE);
	}
}

void CManip::FinAffichage()
{
	g_eventFinAffichage.SetEvent();
}