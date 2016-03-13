#pragma once
#ifndef THREADS_H
#define THREADS_H

#include "afxwin.h"
#include <afxmt.h>


// --------- Initialisation and destruction (kind of) -------
void InitialisationManip();
void InitializeObjects();
void DeleteManip();


// --------- Changing the instrument parameters -------

void ChangementDev(int dev_vanne, int dev_temp);


// --------- Functions to return experiment information -------

int GetEtapeEnCours();
CString GetDonneesExperience();

// --------- Thread start functions --------------

void ManualAction(LPVOID pParam);
void LancementThreads(LPVOID pParam);
void ArretThreads(LPVOID pParam);
void MiseSousVideAmpoule(LPVOID pParam);
void MiseSousVideBouteille(LPVOID pParam);
void ChangementBouteille(LPVOID pParam);

// --------- Threads ----------------------


UINT ThreadManualAction(LPVOID pParam);
UINT LancerThreadProc(LPVOID pParam);
UINT ArretThreadProc(LPVOID pParam);
UINT ThreadMenuMiseSousVideAmpoule(LPVOID pParam);
UINT ThreadMenuMiseSousVideBouteille(LPVOID pParam);
UINT ThreadMenuChangementBouteille(LPVOID pParam);

// --------- Thread interaction / modification functions -------

void DemandeModificationExperience();
void FinAffichageMesure();
void PauseThreads();
void ArretSousVideThreads();
void ProchaineCommandeThreads();
void ProchaineDoseThreads();
void ProchaineEtapeThreads();
void RepriseThreads();

// --------- End ------------

#endif