#include "StdAfx.h"
#include "Manip.h"


void CManip::OuvrirInstruments()
{
	for(int i=0;i<NB_OF_INSTRUMENTS;i++)
		instrument[i]->OuvrirPortInstrument();
}

void CManip::FermerInstruments()
{
	for(int i=0;i<NB_OF_INSTRUMENTS;i++)
		instrument[i]->FermerPortInstrument();
}


double CManip::LectureMesure(ConnectionMesure Appareil)
{
	if(Appareil.voie_mesure == INSTRUMENT_KEYTHLEY_V1)
		return instrument[Appareil.index]->LireKeithley_Voie1();
	if(Appareil.voie_mesure == INSTRUMENT_KEYTHLEY_V2)
		return instrument[Appareil.index]->LireKeithley_Voie2();
	if(Appareil.voie_mesure == MENSOR_VOIE)
		return instrument[Appareil.index]->LireMensor();
	return ERROR_MESURE;
}

void CManip::LectureCalo()
{
	resultat_calo= fluxConverter.ConversionCalo(LectureMesure(AppareilCalo));
}


void CManip::LectureBassePression()
{
	resultat_bp= fluxConverter.ConversionBP(LectureMesure(AppareilBP));
}


void CManip::LectureHautePression()
{
	resultat_hp= fluxConverter.ConversionHP(LectureMesure(AppareilHP));
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
	// Initialisation of the critical section
	for (int i=0;i<NB_OF_INSTRUMENTS+1;i++)
		InitializeCriticalSection(&Sync_view_instrument[i]);


	// Create an event
	//   - Non signalled by default
	//   - With manual reinitiallisation
	hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	// Start the 4 threads and get their handles
	// The functions wait for the hEvent
	hThread[0]=CreateThread(NULL,NULL,ThreadProc_LectureCalo,this,NULL,&ThreadId);
	hThread[1]=CreateThread(NULL,NULL,ThreadProc_LectureHautePression,this,NULL,&ThreadId);
	hThread[2]=CreateThread(NULL,NULL,ThreadProc_LectureBassePression,this,NULL,&ThreadId);
	hThread[3]=CreateThread(NULL,NULL,ThreadProc_LectureTemperature,this,NULL,&ThreadId);

	// Set the event to start the threads
	SetEvent(hEvent);

	// Wait for the threads to finish
	WaitForMultipleObjects(4,hThread,TRUE,INFINITE);

	// Destroy the critical sections
	for(int i=0;i<NB_OF_INSTRUMENTS;i++)
		DeleteCriticalSection(&Sync_view_instrument[i]);

	// Destroy the event
	CloseHandle(hEvent);
}



DWORD WINAPI CManip::ThreadProc_LectureCalo(LPVOID lpParam)
{

	CManip *manipulation = reinterpret_cast<CManip *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->hEvent,INFINITE);

	// Block the corresponding variable
	EnterCriticalSection(&manipulation->Sync_view_instrument[manipulation->synchCalo]);
	manipulation->LectureCalo();
	LeaveCriticalSection(&manipulation->Sync_view_instrument[manipulation->synchCalo]);

	return 0;
}

DWORD WINAPI CManip::ThreadProc_LectureHautePression(LPVOID lpParam)
{
	CManip *manipulation = reinterpret_cast<CManip *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->hEvent,INFINITE);

	// Block the corresponding variable
	EnterCriticalSection(&manipulation->Sync_view_instrument[manipulation->synchHP]);
	manipulation->LectureHautePression();
	LeaveCriticalSection(&manipulation->Sync_view_instrument[manipulation->synchHP]);

	return 0;
}

DWORD WINAPI CManip::ThreadProc_LectureBassePression(LPVOID lpParam)
{
	CManip *manipulation = reinterpret_cast<CManip *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->hEvent,INFINITE);

	// Block the corresponding variable
	EnterCriticalSection(&manipulation->Sync_view_instrument[manipulation->synchBP]);
	manipulation->LectureBassePression();
	LeaveCriticalSection(&manipulation->Sync_view_instrument[manipulation->synchBP]);

	return 0;
}

DWORD WINAPI CManip::ThreadProc_LectureTemperature(LPVOID lpParam)
{
	CManip *manipulation = reinterpret_cast<CManip *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->hEvent,INFINITE);
	
	manipulation->LectureTemperatures();

	return 0;
}

