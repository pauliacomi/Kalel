
// This file should be rewritten as a class in the future

#include "Parametres.h"
#include "../../Kalel Shared/Resources/DefineInstruments.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>

#include <iostream>
#include <fstream>
#include <sstream>

#define nSizeInt 6
#define nSizeFloat 20
#define nSizeString 100
#define nSizeFichier 255
#define Fichier_parametres_char "./Parametres.ini"
#define Fichier_parametres _T(Fichier_parametres_char)


void VerifParametres()
{
	std::ifstream file_stream(Fichier_parametres_char, std::ifstream::in);
	bool ans = file_stream.is_open();
	file_stream.close();

	// si ans=TRUE , alors le fichier existe, pas besoin de le créer
	// si ans=FALSE, alors le fichier n'existe pas, on le crée et on initialise les parametres
	if(!ans)
	{
		Initialisation_parametres();
	}
}




void Initialisation_parametres()
{
	/*
		BOOL WritePrivateProfileString(
		  LPCTSTR lpAppName,
		  LPCTSTR lpKeyName,
		  LPCTSTR lpString,
		  LPCTSTR lpFileName
		);

	lpAppName [in] : le nom de la section
	lpKeyName [in] : le nom de la clé
	lpString [in] : la valeur à sauvegarder (le système ne supporte pas l'utilisation du caractères \t-tabulations dans cette chaîne)
	lpFileName [in] : le nom du fichier d'initialisation
	
	*/


	WritePrivateProfileString(_T("Appareil"),_T("Sensibilite_calo"),_T("0.05470197"),Fichier_parametres);
	WritePrivateProfileString(_T("Appareil"),_T("Sensibilite_capteur_basse_pression"),_T("7.6"),Fichier_parametres);
	WritePrivateProfileString(_T("Appareil"),_T("Sensibilite_capteur_haute_pression"),_T("1"),Fichier_parametres);
	WritePrivateProfileString(_T("Appareil"),_T("Activation_Securite"),_T("Vrai"),Fichier_parametres);
	WritePrivateProfileString(_T("Appareil"),_T("Pression_securite_capteur_basse_pression"),_T("1.1"),Fichier_parametres);
	WritePrivateProfileString(_T("Appareil"),_T("Pression_securite_capteur_haute_pression"),_T("5.1"),Fichier_parametres);
	WritePrivateProfileString(_T("Appareil"),_T("Pression_limite_vide"),_T("0.02"),Fichier_parametres);
	WritePrivateProfileString(_T("Appareil"),_T("Presence_Tuyere_Sonique"),_T("Vrai"),Fichier_parametres);
	WritePrivateProfileString(_T("Appareil"),_T("Volume_ref"),_T("1"),Fichier_parametres);
	WritePrivateProfileString(_T("Appareil"),_T("Volume_P6"),_T("1"),Fichier_parametres);
	WritePrivateProfileString(_T("Appareil"),_T("Instrument_number"), _T("3"), Fichier_parametres);

	WritePrivateProfileString(_T("Connection"),_T("USB_Vannes"),_T("1"),Fichier_parametres);
	WritePrivateProfileString(_T("Connection"),_T("USB_Temperature"),_T("2"),Fichier_parametres);

	for (int i = 1; i <= NB_OF_INSTRUMENTS; i++)
	{
		std::wstring title = L"Instrument" + std::to_wstring(i);
		std::wstring type = std::to_wstring(INSTRUMENT_UNDEF);
		std::wstring function = std::to_wstring(FUNCTION_UNDEF);

		WritePrivateProfileString(title.c_str(), _T("type"), type.c_str(), Fichier_parametres);
		WritePrivateProfileString(title.c_str(), _T("COM"), _T("-1"), Fichier_parametres);
		WritePrivateProfileString(title.c_str(), _T("fonction"), function.c_str(), Fichier_parametres);
	}
	
	WritePrivateProfileString(_T("Calorimetre"),_T("Nom"),_T("Indéfini"),Fichier_parametres);
	WritePrivateProfileString(_T("Calorimetre"),_T("Entete"),_T("Calo"),Fichier_parametres);

	WritePrivateProfileString(_T("General"),_T("Chemin_Fichier"),_T("C:/"),Fichier_parametres);

	WritePrivateProfileString(_T("Mesure"),_T("Calo"),_T("Faux"),Fichier_parametres);
	WritePrivateProfileString(_T("Mesure"),_T("BP"),_T("Faux"),Fichier_parametres);
	WritePrivateProfileString(_T("Mesure"),_T("HP"),_T("Faux"),Fichier_parametres);
}



// Lecture des paramètres de l'appareil


/*
	GetPrivateProfileString

	DWORD GetPrivateProfileString(
	  LPCTSTR lpAppName,
	  LPCTSTR lpKeyName,
	  LPCTSTR lpDefault,
	  LPTSTR lpReturnedString,
	  DWORD nSize,
	  LPCTSTR lpFileName
	);

	lpAppName [in] : le nom de la section
	lpKeyName [in] : le nom de la clé
	lpDefault [in] : la valeur à retourner par défaut dans lpReturnedString si la section ou bien la clé n'existe pas
	lpReturnedString [out] : la valeur demandée (cette chaîne ne peut pas contenir des caractères de control - code inférieur à 32)
	nSize [in] : Taille du buffer lpReturnedString
	lpFileName [in] : le nom du fichier d'initialisation
*/


long double GetSensibiliteCalo()
{
	TCHAR StrSensibiliteCalo[nSizeFloat];
	GetPrivateProfileString(_T("Appareil"), _T("Sensibilite_calo"),
		_T("0.05470197"), StrSensibiliteCalo, 11, Fichier_parametres);
	std::wstring ConvertSensibiliteCalo(StrSensibiliteCalo);
	
	return _wtof(ConvertSensibiliteCalo.c_str());
}


float GetSensibiliteCapteurBassePression()
{
	TCHAR StrSensibiliteCapteurBassePression[nSizeFloat];
	GetPrivateProfileString(_T("Appareil"),_T("Sensibilite_capteur_basse_pression"),
							_T("1"),StrSensibiliteCapteurBassePression,nSizeFloat,Fichier_parametres);
	std::wstring ConvertSensibiliteBasse(StrSensibiliteCapteurBassePression);
	return _wtof(ConvertSensibiliteBasse.c_str());
}


float GetSensibiliteCapteurHautePression()
{
	TCHAR StrSensibiliteCapteurHautePression[nSizeFloat];
	GetPrivateProfileString(_T("Appareil"),_T("Sensibilite_capteur_haute_pression"),
							_T("7.6"),StrSensibiliteCapteurHautePression,nSizeFloat,Fichier_parametres);
	std::wstring ConvertSensibiliteHaute(StrSensibiliteCapteurHautePression);
	return _wtof(ConvertSensibiliteHaute.c_str());
}

bool GetActivationSecurite()
{
	TCHAR StrActivationSecurite[nSizeString];
	GetPrivateProfileString(_T("Appareil"),_T("Activation_Securite"),
							_T("Vrai"),StrActivationSecurite,nSizeString,Fichier_parametres);
	std::wstring ConvertActivationSecurite(StrActivationSecurite);
	if (ConvertActivationSecurite == L"Vrai")
		return true;
	return false;
}

float GetPressionSecuriteBassePression()
{
	TCHAR StrPressionSecuriteBassePression[nSizeFloat];
	GetPrivateProfileString(_T("Appareil"),_T("Pression_securite_capteur_basse_pression"),
							_T("1.1"),StrPressionSecuriteBassePression,nSizeFloat,Fichier_parametres);
	std::wstring ConvertPressionSecuriteBassePression(StrPressionSecuriteBassePression);
	return _wtof(ConvertPressionSecuriteBassePression.c_str());
}


float GetPressionSecuriteHautePression()
{
	TCHAR StrPressionSecuriteHautePression[nSizeFloat];
	GetPrivateProfileString(_T("Appareil"),_T("Pression_securite_capteur_haute_pression"),
							_T("5.1"),StrPressionSecuriteHautePression,nSizeFloat,Fichier_parametres);
	std::wstring ConvertPressionSecuriteHautePression(StrPressionSecuriteHautePression);
	return _wtof(ConvertPressionSecuriteHautePression.c_str());
}


float GetPressionLimiteVide()
{
	TCHAR StrPressionLimiteVide[nSizeFloat];
	GetPrivateProfileString(_T("Appareil"),_T("Pression_limite_vide"),
							_T("0.02"),StrPressionLimiteVide,nSizeFloat,Fichier_parametres);
	std::wstring ConvertPressionLimiteVide(StrPressionLimiteVide);
	return _wtof(ConvertPressionLimiteVide.c_str());
}


bool GetPresenceTuyereSonique()
{
	TCHAR StrPresenceTuyereSonique[nSizeString];
	GetPrivateProfileString(_T("Appareil"),_T("Presence_Tuyere_Sonique"),
							_T("Vrai"),StrPresenceTuyereSonique,nSizeString,Fichier_parametres);
	std::wstring ConvertPresenceTuyereSonique(StrPresenceTuyereSonique);
	if (ConvertPresenceTuyereSonique == L"Vrai")
		return true;
	return false;
}


float GetVolumeRef()
{
	TCHAR StrVolumeRef[nSizeFloat];
	GetPrivateProfileString(_T("Appareil"),_T("Volume_ref"),
							_T("1"),StrVolumeRef,nSizeFloat,Fichier_parametres);
	std::wstring ConvertVolumeRef(StrVolumeRef);
	return _wtof(ConvertVolumeRef.c_str());
}


float GetVolumeP6()
{
	TCHAR StrVolumeP6[nSizeFloat];
	GetPrivateProfileString(_T("Appareil"),_T("Volume_P6"),
							_T("1"),StrVolumeP6,nSizeFloat,Fichier_parametres);
	std::wstring ConvertVolumeP6(StrVolumeP6);
	return _wtof(ConvertVolumeP6.c_str());
}

int GetNumberInstruments()
{
	TCHAR StrNumberInstruments[nSizeInt];
	GetPrivateProfileString(_T("Appareil"), _T("Instrument_number"),
		_T("1"), StrNumberInstruments, nSizeInt, Fichier_parametres);
	return _ttoi(StrNumberInstruments);
}

int GetTypeInstrument(int num)
{
	std::wstring title = L"Instrument" + std::to_wstring(num);
	std::wstring type = std::to_wstring(INSTRUMENT_INEXIST);

	TCHAR StrTypeInstrument[nSizeInt];

	GetPrivateProfileString(title.c_str(), _T("Type"), type.c_str(), StrTypeInstrument, nSizeInt, Fichier_parametres);
	return _ttoi(StrTypeInstrument);	
}

int GetTypeInstrument1()
{return GetTypeInstrument(1);}

int GetTypeInstrument2()
{return GetTypeInstrument(2);}

int GetTypeInstrument3()
{return GetTypeInstrument(3);}



int GetCOMInstrument(int num)
{
	std::wstring title = L"Instrument" + std::to_wstring(num);

	TCHAR StrCOMInstrument[nSizeInt];

	GetPrivateProfileString(title.c_str(), _T("COM"), _T("-1"), StrCOMInstrument, nSizeInt, Fichier_parametres);
	return _ttoi(StrCOMInstrument);	
}

int GetCOMInstrument1()
{return GetCOMInstrument(1);}

int GetCOMInstrument2()
{return GetCOMInstrument(2);}

int GetCOMInstrument3()
{return GetCOMInstrument(3);}



int GetFonctionInstrument(int num)
{
	std::wstring title = L"Instrument" + std::to_wstring(num);
	std::wstring function = std::to_wstring(FUNCTION_INEXIST);

	TCHAR StrFonctionInstrument[nSizeInt];

	GetPrivateProfileString(title.c_str(), _T("Fonction"), function.c_str(), StrFonctionInstrument, nSizeInt, Fichier_parametres);
	return _ttoi(StrFonctionInstrument);	
}

int GetFonctionInstrument1()
{return GetFonctionInstrument(1);}

int GetFonctionInstrument2()
{return GetFonctionInstrument(2);}

int GetFonctionInstrument3()
{return GetFonctionInstrument(3);}


// Lecture des connections de ports

int GetPortKeithley()
{
	TCHAR StrPortKeithley[nSizeInt];
	GetPrivateProfileString(_T("Connection"),_T("RS232_Keithley"),
							_T("1"),StrPortKeithley,nSizeInt,Fichier_parametres);
	return _ttoi(StrPortKeithley);
}

int GetPortMensor()
{
	TCHAR StrPortMensor[nSizeInt];
	GetPrivateProfileString(_T("Connection"),_T("RS232_Mensor"),
							_T("3"),StrPortMensor,nSizeInt,Fichier_parametres);
	return _ttoi(StrPortMensor);
}

int GetPortVannes()
{
	TCHAR StrPortVannes[nSizeInt];
	GetPrivateProfileString(_T("Connection"),_T("USB_Vannes"),
							_T("1"),StrPortVannes,nSizeInt,Fichier_parametres);
	return _ttoi(StrPortVannes);
}

int GetPortTemperatures()
{
	TCHAR StrPortTemperatures[nSizeInt];
	GetPrivateProfileString(_T("Connection"),_T("USB_Temperature"),
							_T("2"),StrPortTemperatures,nSizeInt,Fichier_parametres);
	return _ttoi(StrPortTemperatures);
}


// Lecture des données du calo

std::wstring GetNomCalo()
{
	TCHAR StrNomCalo[nSizeString];
	GetPrivateProfileString(__T("Calorimetre"),_T("Nom"),_T("Indéfini"),
							StrNomCalo,nSizeString,Fichier_parametres);
	std::wstring ConvertNomCalo(StrNomCalo);
	return ConvertNomCalo;
}

std::wstring GetEnteteCalo()
{
	TCHAR StrEnteteCalo[nSizeString];
	GetPrivateProfileString(__T("Calorimetre"),_T("Entete"),_T("Calo"),
							StrEnteteCalo,nSizeString,Fichier_parametres);
	std::wstring ConvertEnteteCalo(StrEnteteCalo);
	return ConvertEnteteCalo;
}

// Lecture des données de General

std::wstring GetCheminFichierGeneral()
{
	TCHAR StrCheminFichierGeneral[nSizeFichier];
	GetPrivateProfileString(__T("General"),_T("Chemin_Fichier"),_T("C:/"),
							StrCheminFichierGeneral,nSizeString,Fichier_parametres);
	std::wstring ConvertCheminFichierGeneral(StrCheminFichierGeneral);
	return ConvertCheminFichierGeneral;
}

// Lecture des données de Mesure

bool GetMesureCalo()
{
	TCHAR StrMesureCalo[nSizeString];
	GetPrivateProfileString(_T("Mesure"),_T("Calo"),
							_T("Faux"),StrMesureCalo,nSizeString,Fichier_parametres);
	std::wstring ConvertMesureCalo(StrMesureCalo);
	if (ConvertMesureCalo == L"Vrai")
		return true;
	return false;
}

bool GetMesureBassePression()
{
	TCHAR StrMesureBassePression[nSizeString];
	GetPrivateProfileString(_T("Mesure"),_T("BP"),
							_T("Faux"),StrMesureBassePression,nSizeString,Fichier_parametres);
	std::wstring ConvertMesureBassePression(StrMesureBassePression);
	if (ConvertMesureBassePression == L"Vrai")
		return true;
	return false;
}

bool GetMesureHautePression()
{
	TCHAR StrMesureHautePression[nSizeString];
	GetPrivateProfileString(_T("Mesure"),_T("HP"),
							_T("Faux"),StrMesureHautePression,nSizeString,Fichier_parametres);
	std::wstring ConvertMesureHautePression(StrMesureHautePression);
	if (ConvertMesureHautePression == L"Vrai")
		return true;
	return false;
}









// Ecriture des paramètres de l'appareil

void SetSensibiliteCalo(long double fSensCalo)
{
	std::wstringstream temp;
	temp.precision(10);
	temp << fSensCalo;
	WritePrivateProfileString(_T("Appareil"), _T("Sensibilite_calo"), temp.str().c_str(), Fichier_parametres);
}

void SetSensibiliteCapteurBassePression(float fSensBP)
{
	WritePrivateProfileString(_T("Appareil"),_T("Sensibilite_capteur_basse_pression"), std::to_wstring(fSensBP).c_str(),Fichier_parametres);
}

void SetSensibiliteCapteurHautePression(float fSensHP)
{
	WritePrivateProfileString(_T("Appareil"),_T("Sensibilite_capteur_haute_pression"), std::to_wstring(fSensHP).c_str(),Fichier_parametres);
}


void SetActivationSecurite(bool bSecurite)
{
	TCHAR TCHARSecurite[nSizeString];
	if (bSecurite == true)
		wsprintf(TCHARSecurite,_T("Vrai"));
	else
		wsprintf(TCHARSecurite,_T("Faux"));
	WritePrivateProfileString(_T("Appareil"),_T("Activation_Securite"),TCHARSecurite,Fichier_parametres);
}

void SetPressionSecuriteBassePression(float fSecuBP)
{
	WritePrivateProfileString(_T("Appareil"),_T("Pression_securite_capteur_basse_pression"), std::to_wstring(fSecuBP).c_str(),Fichier_parametres);
}

void SetPressionSecuriteHautePression(float fSecuHP)
{
	WritePrivateProfileString(_T("Appareil"),_T("Pression_securite_capteur_haute_pression"), std::to_wstring(fSecuHP).c_str(),Fichier_parametres);
}

void SetPressionLimiteVide(float fPressionVide)
{
	WritePrivateProfileString(_T("Appareil"),_T("Pression_limite_vide"), std::to_wstring(fPressionVide).c_str(),Fichier_parametres);
}


void SetPresenceTuyereSonique(bool bTuyere)
{
	TCHAR TCHARTuyere[nSizeString];
	if (bTuyere == true)
		wsprintf(TCHARTuyere,_T("Vrai"));
	else
		wsprintf(TCHARTuyere,_T("Faux"));
	WritePrivateProfileString(_T("Appareil"),_T("Presence_Tuyere_Sonique"),TCHARTuyere,Fichier_parametres);
}

void SetVolumeRef(float fVolumeRef)
{
	WritePrivateProfileString(_T("Appareil"),_T("Volume_ref"), std::to_wstring(fVolumeRef).c_str(),Fichier_parametres);
}

void SetVolumeP6(float fVolumeP6)
{
	WritePrivateProfileString(_T("Appareil"),_T("Volume_P6"), std::to_wstring(fVolumeP6).c_str(),Fichier_parametres);
}


void SetNumberInstruments(int numberInstruments)
{
	WritePrivateProfileString(_T("Appareil"), _T("Instrument_number"), std::to_wstring(numberInstruments).c_str(), Fichier_parametres);
}




void SetTypeInstrument(int num, int TypeInstrument)
{
	std::wstring title = L"Instrument" + std::to_wstring(num);
	std::wstring value = std::to_wstring(TypeInstrument);
	WritePrivateProfileString(title.c_str(), _T("Type"), value.c_str(), Fichier_parametres);
}

void SetTypeInstrument1(int TypeInstrument)
{SetTypeInstrument(1,TypeInstrument);}

void SetTypeInstrument2(int TypeInstrument)
{SetTypeInstrument(2,TypeInstrument);}

void SetTypeInstrument3(int TypeInstrument)
{SetTypeInstrument(3,TypeInstrument);}




void SetCOMInstrument(int num, int COMInstrument)
{
	std::wstring title = L"Instrument" + std::to_wstring(num);
	std::wstring value = std::to_wstring(COMInstrument);
	WritePrivateProfileString(title.c_str(), _T("COM"), value.c_str(), Fichier_parametres);
}

void SetCOMInstrument1(int COMInstrument)
{SetCOMInstrument(1,COMInstrument);}

void SetCOMInstrument2(int COMInstrument)
{SetCOMInstrument(2,COMInstrument);}

void SetCOMInstrument3(int COMInstrument)
{SetCOMInstrument(3,COMInstrument);}




void SetFonctionInstrument(int num, int FonctionInstrument)
{
	std::wstring title = L"Instrument" + std::to_wstring(num);
	std::wstring value = std::to_wstring(FonctionInstrument);
	WritePrivateProfileString(title.c_str(), _T("Fonction"), value.c_str(), Fichier_parametres);
}

void SetFonctionInstrument1(int FonctionInstrument)
{SetFonctionInstrument(1,FonctionInstrument);}

void SetFonctionInstrument2(int FonctionInstrument)
{SetFonctionInstrument(2,FonctionInstrument);}

void SetFonctionInstrument3(int FonctionInstrument)
{SetFonctionInstrument(3,FonctionInstrument);}

void SetInstrument(int num,int TypeInstrument,int COMInstrument,int FonctionInstrument)
{
	SetTypeInstrument(num,TypeInstrument);
	SetCOMInstrument(num,COMInstrument);
	SetFonctionInstrument(num,FonctionInstrument);
}

// Ecriture des connections des ports

void SetPortKeithley(int PortKeithley)
{
	std::wstring value = std::to_wstring(PortKeithley);
	WritePrivateProfileString(_T("Connection"), _T("RS232_Keithley"), value.c_str(), Fichier_parametres);
}

void SetPortMensor(int PortMensor)
{
	std::wstring value = std::to_wstring(PortMensor);
	WritePrivateProfileString(_T("Connection"), _T("RS232_Mensor"), value.c_str(), Fichier_parametres);
}

void SetPortVannes(int PortVannes)
{
	std::wstring value = std::to_wstring(PortVannes);
	WritePrivateProfileString(_T("Connection"), _T("USB_Vannes"), value.c_str(), Fichier_parametres);
}

void SetPortTemperatures(int PortTemperatures)
{
	std::wstring value = std::to_wstring(PortTemperatures);
	WritePrivateProfileString(_T("Connection"), _T("USB_Temperature"), value.c_str(), Fichier_parametres);
}


// Ecriture du calo

void SetNomCalo(std::wstring NomCalo)
{
	WritePrivateProfileString(_T("Calorimetre"), _T("Nom"), NomCalo.c_str(), Fichier_parametres);
}
		
void SetEnteteCalo(std::wstring EnteteCalo)
{
	WritePrivateProfileString(_T("Calorimetre"), _T("Entete"), EnteteCalo.c_str(), Fichier_parametres);
}


// Ecriture de General

void SetCheminFichierGeneral(std::wstring CheminFichierGeneral)
{
	WritePrivateProfileString(_T("General"), _T("Chemin_Fichier"), CheminFichierGeneral.c_str(), Fichier_parametres);
}

// Ecriture de Mesure

void SetMesureCalo(bool bMesure)
{
	TCHAR TCHARMesure[nSizeString];
	if (bMesure == true)
		wsprintf(TCHARMesure,_T("Vrai"));
	else
		wsprintf(TCHARMesure,_T("Faux"));
	WritePrivateProfileString(_T("Mesure"),_T("Calo"),TCHARMesure,Fichier_parametres);
}

void SetMesureBassePression(bool bMesure)
{
	TCHAR TCHARMesure[nSizeString];
	if (bMesure == true)
		wsprintf(TCHARMesure,_T("Vrai"));
	else
		wsprintf(TCHARMesure,_T("Faux"));
	WritePrivateProfileString(_T("Mesure"),_T("BP"),TCHARMesure,Fichier_parametres);
}

void SetMesureHautePression(bool bMesure)
{
	TCHAR TCHARMesure[nSizeString];
	if (bMesure == true)
		wsprintf(TCHARMesure,_T("Vrai"));
	else
		wsprintf(TCHARMesure,_T("Faux"));
	WritePrivateProfileString(_T("Mesure"),_T("HP"),TCHARMesure,Fichier_parametres);
}
