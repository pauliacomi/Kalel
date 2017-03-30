#ifndef PARAMETRES_H
#define PARAMETRES_H
#pragma once

#include <string>

class MachineSettings;

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

// Instruments
int GetNumberInstruments();
int GetInstruments();
int GetTypeInstrument(int num);
int GetPortInstrument(int num);
int GetFunctionInstrument(int num);

// Readers
int GetNumberReaderPressure();
int GetNumberReaderTemperature();
int GetNumberReaderCalorimeter();
int GetNumberReaderValves();
int GetNumberReaderPumps();
int GetIdentifierReader(int num);
int GetSensitivityReader(int num);


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
void SetPortVannes(int PortValves);
void SetPortTemperatures(int PortTemperatures);

void SetNomCalo(std::wstring NomCalo);		
void SetEnteteCalo(std::wstring EnteteCalo);

void SetCheminFichierGeneral(std::wstring CheminFichierGeneral);

void SetMesureCalo(bool bMesure);
void SetMesureBassePression(bool bMesure);
void SetMesureHautePression(bool bMesure);


#endif