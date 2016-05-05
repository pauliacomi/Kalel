#pragma once
#ifndef THREADS_H
#define THREADS_H

#include "afxwin.h"
#include <afxmt.h>

#include "ExperimentSettings.h"

class ThreadManager
{
public:
	ThreadManager(LPVOID pParam, ExperimentSettings * expD);
	~ThreadManager();

	// Pointer to settings storage
	ExperimentSettings * experimentSettings;

	// Public interface methods
public:
	HRESULT CreateMainThread(LPVOID pParam);
	HRESULT SetStartEvent();
	HRESULT ShutdownThread();
	void ChangementDev(int dev_vanne, int dev_temp);
	void ManualAction(LPVOID pParam);						// When a manual command is issued

	// Private fields
private:
	static UINT ThreadMainWorkerStarter(LPVOID pParam);		// Main worker thread starter
	void ThreadMainWorker();								// Main worker thread function

	static UINT ThreadManualActionStarter(LPVOID pParam);	// Manual command thread starter
	void ThreadManualAction();								// Manual command thread worker
	

	CWinThread * m_threadMainControlLoop;					// Reference for main thread
	CWinThread * m_threadManualAction;						// Reference for manual thread

	HANDLE      m_hShutdownEvent;							// Shutdown Event handle
															// (causes thread to exit)

	HANDLE		m_hStartMainThreadEvent;					// Event handle for giving the main thread the go-ahead.

	HWND   m_hWnd;    // Window handle to the UI dialog (used to
					  // send progress and completed messages)


};






//
///// Old implementation
//
//// --------- Changing the instrument parameters -------
//
//void ChangementDev(int dev_vanne, int dev_temp);
//
//// --------- Functions to return experiment information -------
//
//int GetEtapeEnCours();
//CString GetDonneesExperience();
//
//// --------- Thread start functions --------------
//
//void ManualAction(LPVOID pParam);
//void LancementThreads(LPVOID pParam);
//void ArretThreads(LPVOID pParam);
//void MiseSousVideAmpoule(LPVOID pParam);
//void MiseSousVideBouteille(LPVOID pParam);
//void ChangementBouteille(LPVOID pParam);
//
//// --------- Threads ----------------------
//
//UINT ThreadManualAction(LPVOID pParam);
//UINT LancerThreadProc(LPVOID pParam);
//UINT ArretThreadProc(LPVOID pParam);
//UINT ThreadMenuMiseSousVideAmpoule(LPVOID pParam);
//UINT ThreadMenuMiseSousVideBouteille(LPVOID pParam);
//UINT ThreadMenuChangementBouteille(LPVOID pParam);
//
//// --------- Thread interaction / modification functions -------
//
//void DemandeModificationExperience();
//void FinAffichageMesure();
//void PauseThreads();
//void ArretSousVideThreads();
//void ProchaineCommandeThreads();
//void ProchaineDoseThreads();
//void ProchaineEtapeThreads();
//void RepriseThreads();
//
//// --------- End ------------

#endif