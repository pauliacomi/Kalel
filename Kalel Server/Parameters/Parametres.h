#pragma once
#ifndef PARAMETRES_H
#define PARAMETRES_H

#include <string>
#include "../../Kalel Shared/Com Classes/MachineSettings.h"

#define Dev_USB_Temp 2
#define Dev_USB_Vannes 1

bool ParametersCheck();
void ParametersInit();

void ParametersGet(MachineSettings & settings);
void ParametersReplace(const MachineSettings & settings);

// Get

long double GetSensibiliteCalo();
float GetSensibiliteCapteurBassePression();
float GetSensibiliteCapteurHautePression();
bool GetActivationSecurite();
float GetPressionSecuriteBassePression();
float GetPressionSecuriteHautePression();
float GetPressionLimiteVide();
bool GetPresenceTuyereSonique();
float GetVolumeRef();
float GetVolumeP6();

int GetNumberInstruments();

int GetTypeInstrument(int num);
int GetTypeInstrument1();
int GetTypeInstrument2();
int GetTypeInstrument3();

int GetCOMInstrument(int num);
int GetCOMInstrument1();
int GetCOMInstrument2();
int GetCOMInstrument3();

int GetFonctionInstrument(int num);
int GetFonctionInstrument1();
int GetFonctionInstrument2();
int GetFonctionInstrument3();

int GetPortKeithley();
int GetPortMensor();
int GetPortVannes();
int GetPortTemperatures();

std::wstring GetNomCalo();
std::wstring GetEnteteCalo();

std::wstring GetCheminFichierGeneral();

bool GetMesureCalo();
bool GetMesureBassePression();
bool GetMesureHautePression();

// Set

void SetSensibiliteCalo(long double fSensCalo);
void SetSensibiliteCapteurBassePression(float fSensBP);
void SetSensibiliteCapteurHautePression(float fSensHP);
void SetActivationSecurite(bool bSecurite);
void SetPressionSecuriteBassePression(float fSecuBP);
void SetPressionSecuriteHautePression(float fSecuHP);
void SetPressionLimiteVide(float fPressionVide);
void SetPresenceTuyereSonique(bool bTuyere);
void SetVolumeRef(float fVolumeRef);
void SetVolumeP6(float fVolumeP6);

void SetNumberInstruments(int numberInstruments);

void SetTypeInstrument(int num, int TypeInstrument);
void SetTypeInstrument1(int TypeInstrument);
void SetTypeInstrument2(int TypeInstrument);
void SetTypeInstrument3(int TypeInstrument);

void SetCOMInstrument(int num, int COMInstrument);
void SetCOMInstrument1(int COMInstrument);
void SetCOMInstrument2(int COMInstrument);
void SetCOMInstrument3(int COMInstrument);

void SetFonctionInstrument(int num, int FonctionInstrument);
void SetFonctionInstrument1(int FonctionInstrument);
void SetFonctionInstrument2(int FonctionInstrument);
void SetFonctionInstrument3(int FonctionInstrument);

void SetInstrument(int num,int TypeInstrument,int COMInstrument,int FonctionInstrument);


void SetPortKeithley(int PortKeithley);
void SetPortMensor(int PortMensor);
void SetPortVannes(int PortVannes);
void SetPortTemperatures(int PortTemperatures);

void SetNomCalo(std::wstring NomCalo);		
void SetEnteteCalo(std::wstring EnteteCalo);

void SetCheminFichierGeneral(std::wstring CheminFichierGeneral);

void SetMesureCalo(bool bMesure);
void SetMesureBassePression(bool bMesure);
void SetMesureHautePression(bool bMesure);


#endif