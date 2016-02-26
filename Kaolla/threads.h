#pragma once

#ifndef THREADS_H
#define THREADS_H

#include "afxwin.h"
#include <afxmt.h>

#include "Dialogue_TypeExperience.h"


void InitialisationManip();
void DeleteManip();
void ChangementDev(int dev_vanne, int dev_temp);

UINT LancerThreadProc(LPVOID pParam);
UINT ArretThreadProc(LPVOID pParam);

int GetEtapeEnCours();
CString GetDonneesExperience();

// --------- Menus ------------------
void MiseSousVideAmpoule();
UINT ThreadMenuMiseSousVideAmpoule(LPVOID pParam);

void MiseSousVideBouteille();
UINT ThreadMenuMiseSousVideBouteille(LPVOID pParam);

void ChangementBouteille();
UINT ThreadMenuChangementBouteille(LPVOID pParam);

// --------- Boutons ----------------

void DemandeModificationExperience();

// Boutons de commande de la manip

void LancementThreads();
void ArretThreads();
void FinAffichageMesure();
void PauseThreads();
void ArretSousVideThreads();
void ProchaineCommandeThreads();
void ProchaineDoseThreads();
void ProchaineEtapeThreads();
void RepriseThreads();

// Boutons des Vannes

bool DemandeOuvertureVanne(int num_vanne);
bool DemandeFermetureVanne(int num_vanne);
bool DemandeActivationEV1();
bool DemandeDesactivationEV1();
bool DemandeActivationEV2();
bool DemandeDesactivationEV2();
bool DemandeActivationPompe();
bool DemandeDesactivationPompe();

// --------- Fin boutons ------------

#endif