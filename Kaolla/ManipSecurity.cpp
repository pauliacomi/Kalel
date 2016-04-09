
// This function contains checks for high pressure and other security related checks
// Threads in here are really badly implemented 

#include "StdAfx.h"
#include "Manip.h"
#include "Define_Manip_Securite.h"


void CManip::SecuriteHautePression()
{
	if(GenreExperience == EXPERIMENT_TYPE_MANUAL)
		SecuriteHautePressionManuelle();
	if(GenreExperience == EXPERIMENT_TYPE_AUTO)
		SecuriteHautePressionAuto();
}

void CManip::InitialisationMesureSecuriteManuelle()
{
	PressionTropHaute = FALSE;
	TemperatureElevee = FALSE;
	TemperatureFaible = FALSE;
}


void CManip::SecuriteHautePressionManuelle()
{
	// Mesure de sécurité
	if(resultat_hp >= GetPressionSecuriteHautePression())
	{
		HANDLE hThreadPression;
		DWORD ThreadPressionId;
		hEventPression=CreateEvent(NULL,TRUE,FALSE,NULL);
		hThreadPression=CreateThread(NULL,NULL,MesureSecuriteHautePression,NULL,NULL,&ThreadPressionId);
		SetEvent(hEventPression);
		//WaitForSingleObject(hThreadPression,INFINITE);
		messageHandler.DisplayMessage(_T("Haute Pression trop élevée\r\n"));

		CloseHandle(hEventPression);
	}
	else
		if(PressionTropHaute)
		{
			messageHandler.DisplayMessage(_T("Haute Pression revenue à la normale\r\n"));
			MessageBeep(MB_ICONINFORMATION);
			PressionTropHaute = FALSE;
		}
}

DWORD WINAPI CManip::MesureSecuriteHautePression(LPVOID)
{

	WaitForSingleObject(hEventPression,INFINITE);

	if (!PressionTropHaute)
	{
		PressionTropHaute = TRUE;
		CString message;
		message.Format(_T("Attention ! Haute pression trop élevé\t\nVeuillez la mettre en dessous de : %f bar\t\n"),
			GetPressionSecuriteHautePression());
		AfxMessageBox(message,MB_ICONERROR);
	}
	else
	{
		//emmetre un son
		//MessageBeep(MB_ICONWARNING);
		MessageBeep(MB_ICONERROR);
		//PlaySound( (LPCTSTR)SND_ALIAS_SYSTEMHAND, NULL, SND_ALIAS_ID | SND_ASYNC );
	}

	return 0;
}


void CManip::SecuriteHautePressionAuto()
{
	bool mesure_de_securite = GetActivationSecurite();
	// Mesure de sécurité : 
	if (mesure_de_securite)
	{
		if(etape_en_cours != STAGE_UNDER_VACUUM)
		{
			if(resultat_hp > GetPressionSecuriteBassePression())
			{
				if(EstOuvert_Vanne(6))
				{
					CString mess;
					mess.Format(_T("HAUTE PRESSION (%f) DEPASSANT LA SECURITE DE LA BASSE PRESSION (%f)\r\n"),
						resultat_hp,GetPressionSecuriteBassePression());
					mess +=_T("PAR MESURE DE SECURITE, FERMETURE DE LA VANNE 6\r\n");

					messageHandler.DisplayMessage(mess);
					Fermeture_Vanne(6);
				}
			}
			if(resultat_hp >= GetPressionSecuriteHautePression())
				demande_arret = ARRET_URGENCE_HP;
		}
	}
}


void CManip::SecuriteTemperatures()
{
	if(GenreExperience == EXPERIMENT_TYPE_MANUAL)
		SecuriteTemperaturesManuelle();
	if(GenreExperience == EXPERIMENT_TYPE_AUTO)
		SecuriteTemperaturesAuto();
}

void CManip::SecuriteTemperaturesManuelle()
{

	if(TemperatureCalo >= (float)general.temperature_experience + securite_temperature)//2)//5)
	{
		HANDLE hThreadTemperatureElevee;
		DWORD ThreadTemperatureEleveeId;
		hEventTemperatureElevee=CreateEvent(NULL,TRUE,FALSE,NULL);
		hThreadTemperatureElevee=CreateThread(NULL,NULL,MesureSecuriteTemperatureElevee,NULL,NULL,&ThreadTemperatureEleveeId);
		SetEvent(hEventTemperatureElevee);
		messageHandler.DisplayMessage(_T("Température du Calorimètre trop élevée\r\n"));

		CloseHandle(hEventTemperatureElevee);
	}
	else
		if(TemperatureElevee)
		{
			messageHandler.DisplayMessage(_T("Température du Calorimètre redescendue à la normale\r\n"));
			MessageBeep(MB_ICONINFORMATION);
			TemperatureElevee = FALSE;
		}

	if(TemperatureCalo <= (float)general.temperature_experience - securite_temperature)//2)//5)
	{
		HANDLE hThreadTemperatureFaible;
		DWORD ThreadTemperatureFaibleId;
		hEventTemperatureFaible=CreateEvent(NULL,TRUE,FALSE,NULL);
		hThreadTemperatureFaible=CreateThread(NULL,NULL,MesureSecuriteTemperatureFaible,NULL,NULL,&ThreadTemperatureFaibleId);
		SetEvent(hEventTemperatureFaible);
		messageHandler.DisplayMessage(_T("Température du Calorimètre trop faible\r\n"));

		CloseHandle(hEventTemperatureFaible);
	}
	else
		if(TemperatureFaible)
		{
			messageHandler.DisplayMessage(_T("Température du Calorimètre remontée à la normale\r\n"));
			MessageBeep(MB_ICONINFORMATION);
			TemperatureFaible = FALSE;
		}
	
}



DWORD WINAPI CManip::MesureSecuriteTemperatureElevee(LPVOID)
{
	WaitForSingleObject(hEventTemperatureElevee,INFINITE);
	if (!TemperatureElevee)
	{
		TemperatureElevee = TRUE;
		CString message;
		message.Format(_T("Attention ! Température du calo trop élevé par rapport à la température d'expérience\t\n"));
		AfxMessageBox(message,MB_ICONERROR);
	}
	else
	{
		//emmetre un son
		//MessageBeep(MB_ICONWARNING);
		MessageBeep(MB_ICONERROR);
		//PlaySound( (LPCTSTR)SND_ALIAS_SYSTEMHAND, NULL, SND_ALIAS_ID | SND_ASYNC );
	}

	return 0;
}

DWORD WINAPI CManip::MesureSecuriteTemperatureFaible(LPVOID)
{
	WaitForSingleObject(hEventTemperatureFaible,INFINITE);
	if (!TemperatureFaible)
	{
		TemperatureFaible = TRUE;
		CString message;
		message.Format(_T("Attention ! Température du calo trop faible par rapport à la température d'expérience\t\n"));
		AfxMessageBox(message,MB_ICONERROR);
	}
	else
	{
		//emmetre un son
		//MessageBeep(MB_ICONWARNING);
		MessageBeep(MB_ICONERROR);
		//PlaySound( (LPCTSTR)SND_ALIAS_SYSTEMHAND, NULL, SND_ALIAS_ID | SND_ASYNC );
	}

	return 0;
}

void CManip::SecuriteTemperaturesAuto()
{
	bool mesure_de_securite = GetActivationSecurite();
	// Mesure de sécurité : 
	if (mesure_de_securite)
	{
		if(etape_en_cours != STAGE_UNDER_VACUUM)
		{
			CString mess;
			if(TemperatureCalo >= (float)general.temperature_experience + securite_temperature)//2)//5)
			{
				mess.Format(_T("Arrêt de l'expérience : La température du Calo étant trop élevée (supérieure à %d°C)\r\n"),
																			general.temperature_experience + securite_temperature);
				messageHandler.DisplayMessage(mess);
				demande_arret = ARRET_URGENCE_TCH;
			}
			if(TemperatureCalo <= (float)general.temperature_experience - securite_temperature)//2)//5)
			{
				mess.Format(_T("Arrêt de l'expérience : La température du Calo étant trop faible (inférieure à %d°C)\r\n"),
																			general.temperature_experience - securite_temperature);
				messageHandler.DisplayMessage(mess);
				demande_arret = ARRET_URGENCE_TCB;
			}
		}
	}
}