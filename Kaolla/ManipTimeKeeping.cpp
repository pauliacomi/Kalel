
// This file contains Manip functions that are used to make the system wait
// The functions here also build the string that is returned to the main display - perhaps a good idea to move it to the GUI


#include "StdAfx.h"
#include "Manip.h"


#define jour_sec 86400
#define heure_sec 3600
#define minute_sec 60

#define message_jour 	0
#define message_heure 	1
#define message_minute	2
#define message_seconde	3


// Wait a number of minutes
void CManip::AttenteMinutes(int nbminutes)
{
	AttenteSecondes(nbminutes * 60);
}

// Wait a number of seconds
void CManip::AttenteSecondes (int nbsecondes)
{
	CChrono chrono_attente;
	chrono_attente.TopChrono();
	int temps_effectue = chrono_attente.TempsActuel();
	CString textAttente,message;
	textAttente = MessageAttente(nbsecondes);
	int compteur = 0;
	while(temps_effectue<nbsecondes)
	{
		message  = textAttente;
		message += MessageTemps(nbsecondes - temps_effectue);
		
		for(int i=0;i<compteur;i++)
		{
			message += _T(".");
		}
		
		if(compteur==10)
			compteur=0;
		else
			compteur++;
		message += _T("\r\n");
		
		RajoutAffichageMessages(message);
		
		Sleep(995); // why?
		temps_effectue = chrono_attente.TempsActuel();
	}
	
}

CString CManip::MessageAttente(int nbsecondes) // should build just a time object to be passed 
{
	CString result;
	if (nbsecondes >= jour_sec)
	{
		if(nbsecondes/jour_sec ==1)
			result = _T("Attente : 1 jour\r\n");
		else
			result.Format(_T("Attente : %d jours\r\n"),nbsecondes/jour_sec);
	}
	else
		if (nbsecondes >= heure_sec)
		{
			if(nbsecondes/jour_sec ==1)
				result = _T("Attente : 1 heure\r\n");
			else
				result.Format(_T("Attente : %d heures\r\n"),nbsecondes/heure_sec);
		}
	else
		if (nbsecondes >= minute_sec)
		{
			if(nbsecondes/jour_sec ==1)
				result = _T("Attente : 1 minute\r\n");
			else
				result.Format(_T("Attente : %d minutes\r\n"),nbsecondes/minute_sec);
		}
	else
	{
		result.Format(_T("Attente : %d secondes\r\n"),nbsecondes);
	}
	
	return result;

}

CString CManip::MessageTemps(int duree)
{
	int TypeMessage;
	
	if (duree >= jour_sec)
		TypeMessage = message_jour;
	else
		if (duree >= heure_sec)
			TypeMessage = message_heure;
	else
		if (duree >= minute_sec)
			TypeMessage = message_minute;
	else
		TypeMessage = message_seconde;
		
	CString temps_attente,tmp;
	temps_attente = _T("");
	int duree_restante = duree;
	switch(TypeMessage)
	{
		case message_jour :
			tmp.Format(_T("%dj"),duree_restante/jour_sec);
			duree_restante = duree_restante%jour_sec;
			temps_attente += tmp;
		case message_heure :
			if(TypeMessage == message_heure)
				tmp.Format(_T("%dh"),duree_restante/heure_sec);
			else
				tmp.Format(_T("%02dh"),duree_restante/heure_sec);
			duree_restante = duree_restante%heure_sec;
			temps_attente += tmp;		
		case message_minute :
			if(TypeMessage == message_minute)		
				tmp.Format(_T("%dmin"),duree_restante/minute_sec);
			else
				tmp.Format(_T("%02dmin"),duree_restante/minute_sec);
			duree_restante = duree_restante%minute_sec;
			temps_attente += tmp;		
		case message_seconde :
			if(TypeMessage == message_seconde)		
				tmp.Format(_T("%ds"),duree_restante);
			else
				tmp.Format(_T("%02ds"),duree_restante);
			temps_attente += tmp;
	}
	return temps_attente;
}