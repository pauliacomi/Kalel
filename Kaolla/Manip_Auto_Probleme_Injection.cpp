#include "StdAfx.h"
#include "Manip_Auto.h"
#include "Define_Manip_Auto_Probleme_Injection.h"

HANDLE hEventInjection;

using namespace std;


void CManip_Auto::ReinitialisationVerificationInjection()
{
	precedent_injection = -1;
	compteur_injection = 0;
}




void CManip_Auto::VerificationInjection()
{
	if((precedent_injection - marge_injection < resultat_hp) &&
		(resultat_hp < precedent_injection + marge_injection))
	{
		compteur_injection++;
	}
	if(compteur_injection >= nb_injection)
	{
		// Mettre en pause et message
		///tentative de fen�tre et pause en thread
		if(demande_arret == INACTIF)
			demande_arret = PAUSE;

		HANDLE hThreadInjection;
		DWORD ThreadInjectionId;
		hEventInjection=CreateEvent(NULL,TRUE,FALSE,NULL);
		hThreadInjection=CreateThread(NULL,NULL,PauseInjection,this,NULL,&ThreadInjectionId);
		SetEvent(hEventInjection);
		//WaitForSingleObject(hThreadPression,INFINITE);
		messageHandler.DisplayMessage(_T("Probl�me d'injection - demande de pause\r\n"));
		CString message;
		message = _T("ATTENTION !!! probl�me de gaz !!!\t\n");
		message+= _T("(gaz moteur ou gaz d'analyse)\t\n");
		message+= _T("Exp�rience en pause !\r\n");
		message+= _T("N'oubliez pas de 'reprendre' l'exp�rience !\t\n");
		AfxMessageBox(message,MB_ICONERROR | MB_OK);
		
		WaitForSingleObject(hThreadInjection,INFINITE);
		CloseHandle(hEventInjection);
		

		//fin tentative
		compteur_injection = 0;
	}
	precedent_injection = resultat_hp;
}




DWORD WINAPI CManip_Auto::PauseInjection(LPVOID lpParam)
{

	WaitForSingleObject(hEventInjection,INFINITE);

	CManip_Auto *manipulation = reinterpret_cast<CManip_Auto *>(lpParam);
	manipulation->MiseEnPause();

	return 0;
}