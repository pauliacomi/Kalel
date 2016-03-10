#ifndef PARAMETRES_H
#define PARAMETRES_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>


#include <iostream>
#include <fstream>
#include <string>
//#include "NIDAQmxBase.h"

#include "Define_Instrument.h"


#define Dev_USB_Temp 2
#define Dev_USB_Vannes 1

void VerifParametres();
void Initialisation_parametres();



//float GetSensibiliteCalo();
//long double GetSensibiliteCalo();
long double GetSensibiliteCalo();
float GetSensibiliteCapteurBassePression();
float GetSensibiliteCapteurHautePression();
bool GetActivationSecurite();
float GetPressionSecuriteBassePression();
float GetPressionSecuriteHautePression();
float GetPressionLimiteVide();
bool GetPresenceTuyereSonique();
int GetPourcentageAvertissement();
float GetVolumeRef();
float GetVolumeP6();

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

std::string GetNomCalo();
std::string GetEnteteCalo();

std::string GetCheminFichierGeneral();

bool GetMesureCalo();
bool GetMesureBassePression();
bool GetMesureHautePression();




void SetSensibiliteCalo(float fSensCalo);
void SetSensibiliteCapteurBassePression(float fSensBP);
void SetSensibiliteCapteurHautePression(float fSensHP);
void SetActivationSecurite(bool bSecurite);
void SetPressionSecuriteBassePression(float fSecuBP);
void SetPressionSecuriteHautePression(float fSecuHP);
void SetPressionLimiteVide(float fPressionVide);
void SetPresenceTuyereSonique(bool bTuyere);
void SetPourcentageAvertissement(int fAvertissement);
void SetVolumeRef(float fVolumeRef);
void SetVolumeP6(float fVolumeP6);

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

void SetNomCalo(std::string NomCalo);		
void SetEnteteCalo(std::string EnteteCalo);

void SetCheminFichierGeneral(std::string CheminFichierGeneral);

void SetMesureCalo(bool bMesure);
void SetMesureBassePression(bool bMesure);
void SetMesureHautePression(bool bMesure);


#endif