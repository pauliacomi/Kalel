#include "StdAfx.h"
#include "Manip.h"


using namespace std;



CRITICAL_SECTION Sync_view_instrument[nb_instrument + 1];
HANDLE hEvent;



void CManip::RajoutMesure()
{
	m_Doc->RajoutMesure(numero_mesure, 
						temps_manip, 
						resultat_calo, 
						resultat_bp, 
						resultat_hp, 
						TemperatureCalo, 
						TemperatureCage, 
						TemperaturePiece);
}

void CManip::OuvrirInstruments()
{
	for(int i=0;i<nb_instrument;i++)
		instrument[i]->OuvrirPortInstrument();
}

void CManip::FermerInstruments()
{
	for(int i=0;i<nb_instrument;i++)
		instrument[i]->FermerPortInstrument();
}


double CManip::LectureMesure(ConnectionMesure Appareil)
{
	if(Appareil.voie_mesure == KEITHLEY_V1)
		return instrument[Appareil.index]->LireKeithley_Voie1();
	if(Appareil.voie_mesure == KEITHLEY_V2)
		return instrument[Appareil.index]->LireKeithley_Voie2();
	if(Appareil.voie_mesure == MENSOR_VOIE)
		return instrument[Appareil.index]->LireMensor();
	return ERROR_MESURE;
}

void CManip::LectureCalo()
{
	resultat_calo=ConversionCalo(LectureMesure(AppareilCalo));
}


void CManip::LectureBassePression()
{
	resultat_bp=ConversionBP(LectureMesure(AppareilBP));
}


void CManip::LectureHautePression()
{
	resultat_hp=ConversionHP(LectureMesure(AppareilHP));
	SecuriteHautePression();
}


void CManip::LectureTemperatures()
{
	double dTemperatureCalo, dTemperatureCage, dTemperaturePiece;
	g_pTemperature->Temperature(&dTemperatureCalo, &dTemperatureCage, &dTemperaturePiece);
	TemperatureCalo=(float)dTemperatureCalo;
	TemperatureCage= (float)dTemperatureCage;
	TemperaturePiece=(float)dTemperaturePiece;

	SecuriteTemperatures();
}



void CManip::ThreadMesures()
{
	// Initialisation de la section critique
	for (int i=0;i<nb_instrument+1;i++)
		InitializeCriticalSection(&Sync_view_instrument[i]);


	// On cr�e un �v�nement
	//   - Non signal� par d�faut
	//   - A r�initialisation manuelle
	hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	// On d�marre 4 threads et on r�cup�re leurs HANDLES
	// Les fonctions utilis�es attendent l'activation de hEventAuto
	hThread[0]=CreateThread(NULL,NULL,ThreadProc_LectureCalo,this,NULL,&ThreadId);
	hThread[1]=CreateThread(NULL,NULL,ThreadProc_LectureHautePression,this,NULL,&ThreadId);
	hThread[2]=CreateThread(NULL,NULL,ThreadProc_LectureBassePression,this,NULL,&ThreadId);
	hThread[3]=CreateThread(NULL,NULL,ThreadProc_LectureTemperature,this,NULL,&ThreadId);

	// Les threads sont lib�r�s de leur attente
	SetEvent(hEvent);
	// On attend que les 4 threads soient termin�s
	WaitForMultipleObjects(4,hThread,TRUE,INFINITE);
	// On peut d�truire la section critique
	for(int i=0;i<nb_instrument;i++)
		DeleteCriticalSection(&Sync_view_instrument[i]);

	// On d�truit l'�v�nement
	CloseHandle(hEvent);
}



DWORD WINAPI CManip::ThreadProc_LectureCalo(LPVOID lpParam)
{
	// Attend l'activation de hEventAuto ind�finiment
	WaitForSingleObject(hEvent,INFINITE);
	
	CManip *manipulation = reinterpret_cast<CManip *>(lpParam);
	
	EnterCriticalSection(&Sync_view_instrument[manipulation->synchCalo]);
	manipulation->LectureCalo();
	LeaveCriticalSection(&Sync_view_instrument[manipulation->synchCalo]);

	return 0;
}

DWORD WINAPI CManip::ThreadProc_LectureHautePression(LPVOID lpParam)
{
	// Attend l'activation de hEventAuto ind�finiment
	WaitForSingleObject(hEvent,INFINITE);
	
	CManip *manipulation = reinterpret_cast<CManip *>(lpParam);
	
	EnterCriticalSection(&Sync_view_instrument[manipulation->synchHP]);
	manipulation->LectureHautePression();
	LeaveCriticalSection(&Sync_view_instrument[manipulation->synchHP]);

	return 0;
}

DWORD WINAPI CManip::ThreadProc_LectureBassePression(LPVOID lpParam)
{
	// Attend l'activation de hEventAuto ind�finiment
	WaitForSingleObject(hEvent,INFINITE);
	
	CManip *manipulation = reinterpret_cast<CManip *>(lpParam);
	
	int cool = manipulation->AppareilBP.index;

	EnterCriticalSection(&Sync_view_instrument[manipulation->synchBP]);
	manipulation->LectureBassePression();
	LeaveCriticalSection(&Sync_view_instrument[manipulation->synchBP]);

	return 0;
}

DWORD WINAPI CManip::ThreadProc_LectureTemperature(LPVOID lpParam)
{
	// Attend l'activation de hEvent ind�finiment
	WaitForSingleObject(hEvent,INFINITE);
	
	CManip *manipulation = reinterpret_cast<CManip *>(lpParam);
	
	manipulation->LectureTemperatures();

	return 0;
}

