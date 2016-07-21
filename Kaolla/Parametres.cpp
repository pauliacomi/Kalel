
// This file should be rewritten as a class in the future

#include "stdafx.h"
#include "Parametres.h"

#include "DefineInstruments.h"

using namespace std;

#define nSizeInt 6
#define nSizeFloat 20
#define nSizeString 100
#define nSizeFichier 255
#define Fichier_parametres_char "./Parametres.ini"
#define Fichier_parametres _T(Fichier_parametres_char)


void VerifParametres()
{
	ifstream file_stream(Fichier_parametres_char,ifstream::in);
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

	WritePrivateProfileString(_T("Connection"),_T("USB_Vannes"),_T("1"),Fichier_parametres);
	WritePrivateProfileString(_T("Connection"),_T("USB_Temperature"),_T("2"),Fichier_parametres);

	for(int i=1;i<=NB_OF_INSTRUMENTS;i++)
	{
		CString strTitre,strType,strFonction;
		strTitre.Format(_T("Instrument%d"),i);
		strType.Format(_T("%d"),INSTRUMENT_UNDEF);
		strFonction.Format(_T("%d"),FUNCTION_UNDEF);

		WritePrivateProfileString(strTitre,_T("type"),strType,Fichier_parametres);
		WritePrivateProfileString(strTitre,_T("COM"),_T("-1"),Fichier_parametres);
		WritePrivateProfileString(strTitre,_T("fonction"),strFonction,Fichier_parametres);
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


//float GetSensibiliteCalo()
//long double GetSensibiliteCalo()
long double GetSensibiliteCalo()
{
	TCHAR StrSensibiliteCalo[nSizeFloat];
	GetPrivateProfileString(_T("Appareil"),_T("Sensibilite_calo"),
							_T("0.05470197"),StrSensibiliteCalo,11,Fichier_parametres);
	string ConvertSensibiliteCalo(StrSensibiliteCalo);
	
	//return atof(ConvertSensibiliteCalo.c_str());
	return atof(ConvertSensibiliteCalo.c_str());
}


float GetSensibiliteCapteurBassePression()
{
	TCHAR StrSensibiliteCapteurBassePression[nSizeFloat];
	GetPrivateProfileString(_T("Appareil"),_T("Sensibilite_capteur_basse_pression"),
							_T("1"),StrSensibiliteCapteurBassePression,nSizeFloat,Fichier_parametres);
	string ConvertSensibiliteBasse(StrSensibiliteCapteurBassePression);
	return atof(ConvertSensibiliteBasse.c_str());
}


float GetSensibiliteCapteurHautePression()
{
	TCHAR StrSensibiliteCapteurHautePression[nSizeFloat];
	GetPrivateProfileString(_T("Appareil"),_T("Sensibilite_capteur_haute_pression"),
							_T("7.6"),StrSensibiliteCapteurHautePression,nSizeFloat,Fichier_parametres);
	string ConvertSensibiliteHaute(StrSensibiliteCapteurHautePression);
	return atof(ConvertSensibiliteHaute.c_str());
}

bool GetActivationSecurite()
{
	TCHAR StrActivationSecurite[nSizeString];
	GetPrivateProfileString(_T("Appareil"),_T("Activation_Securite"),
							_T("Vrai"),StrActivationSecurite,nSizeString,Fichier_parametres);
	string ConvertActivationSecurite(StrActivationSecurite);
	if (ConvertActivationSecurite == "Vrai")
		return true;
	return false;
}

float GetPressionSecuriteBassePression()
{
	TCHAR StrPressionSecuriteBassePression[nSizeFloat];
	GetPrivateProfileString(_T("Appareil"),_T("Pression_securite_capteur_basse_pression"),
							_T("1.1"),StrPressionSecuriteBassePression,nSizeFloat,Fichier_parametres);
	string ConvertPressionSecuriteBassePression(StrPressionSecuriteBassePression);
	return atof(ConvertPressionSecuriteBassePression.c_str());
}


float GetPressionSecuriteHautePression()
{
	TCHAR StrPressionSecuriteHautePression[nSizeFloat];
	GetPrivateProfileString(_T("Appareil"),_T("Pression_securite_capteur_haute_pression"),
							_T("5.1"),StrPressionSecuriteHautePression,nSizeFloat,Fichier_parametres);
	string ConvertPressionSecuriteHautePression(StrPressionSecuriteHautePression);
	return atof(ConvertPressionSecuriteHautePression.c_str());
}


float GetPressionLimiteVide()
{
	TCHAR StrPressionLimiteVide[nSizeFloat];
	GetPrivateProfileString(_T("Appareil"),_T("Pression_limite_vide"),
							_T("0.02"),StrPressionLimiteVide,nSizeFloat,Fichier_parametres);
	string ConvertPressionLimiteVide(StrPressionLimiteVide);
	return atof(ConvertPressionLimiteVide.c_str());
}


bool GetPresenceTuyereSonique()
{
	TCHAR StrPresenceTuyereSonique[nSizeString];
	GetPrivateProfileString(_T("Appareil"),_T("Presence_Tuyere_Sonique"),
							_T("Vrai"),StrPresenceTuyereSonique,nSizeString,Fichier_parametres);
	string ConvertPresenceTuyereSonique(StrPresenceTuyereSonique);
	if (ConvertPresenceTuyereSonique == "Vrai")
		return true;
	return false;
}


float GetVolumeRef()
{
	TCHAR StrVolumeRef[nSizeFloat];
	GetPrivateProfileString(_T("Appareil"),_T("Volume_ref"),
							_T("1"),StrVolumeRef,nSizeFloat,Fichier_parametres);
	string ConvertVolumeRef(StrVolumeRef);
	return atof(ConvertVolumeRef.c_str());
}


float GetVolumeP6()
{
	TCHAR StrVolumeP6[nSizeFloat];
	GetPrivateProfileString(_T("Appareil"),_T("Volume_P6"),
							_T("1"),StrVolumeP6,nSizeFloat,Fichier_parametres);
	string ConvertVolumeP6(StrVolumeP6);
	return atof(ConvertVolumeP6.c_str());
}


int GetTypeInstrument(int num)
{
	TCHAR StrTypeInstrument[nSizeInt];
	CString strTitre,strType;
	strTitre.Format(_T("Instrument%d"),num);
	strType.Format(_T("%d"),INSTRUMENT_INEXIST);
	GetPrivateProfileString(strTitre,_T("Type"),
							strType,StrTypeInstrument,nSizeInt,Fichier_parametres);
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
	TCHAR StrCOMInstrument[nSizeInt];
	CString strTitre;
	strTitre.Format(_T("Instrument%d"),num);
	GetPrivateProfileString(strTitre,_T("COM"),
							_T("-1"),StrCOMInstrument,nSizeInt,Fichier_parametres);
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
	TCHAR StrFonctionInstrument[nSizeInt];
	CString strTitre,strFonction;
	strTitre.Format(_T("Instrument%d"),num);
	strFonction.Format(_T("%d"),FUNCTION_INEXIST);
	GetPrivateProfileString(strTitre,_T("Fonction"),
							strFonction,StrFonctionInstrument,nSizeInt,Fichier_parametres);
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

string GetNomCalo()
{
	TCHAR StrNomCalo[nSizeString];
	GetPrivateProfileString(__T("Calorimetre"),_T("Nom"),_T("Indéfini"),
							StrNomCalo,nSizeString,Fichier_parametres);
	string ConvertNomCalo(StrNomCalo);
	return ConvertNomCalo;
}

string GetEnteteCalo()
{
	TCHAR StrEnteteCalo[nSizeString];
	GetPrivateProfileString(__T("Calorimetre"),_T("Entete"),_T("Calo"),
							StrEnteteCalo,nSizeString,Fichier_parametres);
	string ConvertEnteteCalo(StrEnteteCalo);
	return ConvertEnteteCalo;
}

// Lecture des données de General

string GetCheminFichierGeneral()
{
	TCHAR StrCheminFichierGeneral[nSizeFichier];
	GetPrivateProfileString(__T("General"),_T("Chemin_Fichier"),_T("C:/"),
							StrCheminFichierGeneral,nSizeString,Fichier_parametres);
	string ConvertCheminFichierGeneral(StrCheminFichierGeneral);
	return ConvertCheminFichierGeneral;
}

// Lecture des données de Mesure

bool GetMesureCalo()
{
	TCHAR StrMesureCalo[nSizeString];
	GetPrivateProfileString(_T("Mesure"),_T("Calo"),
							_T("Faux"),StrMesureCalo,nSizeString,Fichier_parametres);
	string ConvertMesureCalo(StrMesureCalo);
	if (ConvertMesureCalo == "Vrai")
		return true;
	return false;
}

bool GetMesureBassePression()
{
	TCHAR StrMesureBassePression[nSizeString];
	GetPrivateProfileString(_T("Mesure"),_T("BP"),
							_T("Faux"),StrMesureBassePression,nSizeString,Fichier_parametres);
	string ConvertMesureBassePression(StrMesureBassePression);
	if (ConvertMesureBassePression == "Vrai")
		return true;
	return false;
}

bool GetMesureHautePression()
{
	TCHAR StrMesureHautePression[nSizeString];
	GetPrivateProfileString(_T("Mesure"),_T("HP"),
							_T("Faux"),StrMesureHautePression,nSizeString,Fichier_parametres);
	string ConvertMesureHautePression(StrMesureHautePression);
	if (ConvertMesureHautePression == "Vrai")
		return true;
	return false;
}









// Ecriture des paramètres de l'appareil

void SetSensibiliteCalo(float fSensCalo)
{
	char charSensCalo[nSizeFloat];
	TCHAR TCHARSensCalo[nSizeFloat];
	sprintf_s(charSensCalo,"%1.8f",fSensCalo);
	wsprintf(TCHARSensCalo,_T(charSensCalo));
	WritePrivateProfileString(_T("Appareil"),_T("Sensibilite_calo"),TCHARSensCalo,Fichier_parametres);
}

void SetSensibiliteCapteurBassePression(float fSensBP)
{
	char charSensBP[nSizeFloat];
	TCHAR TCHARSensBP[nSizeFloat];
	sprintf_s(charSensBP,"%1.4f",fSensBP);
	wsprintf(TCHARSensBP,_T(charSensBP));
	WritePrivateProfileString(_T("Appareil"),_T("Sensibilite_capteur_basse_pression"),TCHARSensBP,Fichier_parametres);
}

void SetSensibiliteCapteurHautePression(float fSensHP)
{
	char charSensHP[nSizeFloat];
	TCHAR TCHARSensHP[nSizeFloat];
	sprintf_s(charSensHP,"%1.4f",fSensHP);
	wsprintf(TCHARSensHP,_T(charSensHP));
	WritePrivateProfileString(_T("Appareil"),_T("Sensibilite_capteur_haute_pression"),TCHARSensHP,Fichier_parametres);
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
	char charSecuBP[nSizeFloat];
	TCHAR TCHARSecuBP[nSizeFloat];
	sprintf_s(charSecuBP,"%1.4f",fSecuBP);
	wsprintf(TCHARSecuBP,_T(charSecuBP));
	WritePrivateProfileString(_T("Appareil"),_T("Pression_securite_capteur_basse_pression"),TCHARSecuBP,Fichier_parametres);
}

void SetPressionSecuriteHautePression(float fSecuHP)
{
	char charSecuHP[nSizeFloat];
	TCHAR TCHARSecuHP[nSizeFloat];
	sprintf_s(charSecuHP,"%1.4f",fSecuHP);
	wsprintf(TCHARSecuHP,_T(charSecuHP));
	WritePrivateProfileString(_T("Appareil"),_T("Pression_securite_capteur_haute_pression"),TCHARSecuHP,Fichier_parametres);
}

void SetPressionLimiteVide(float fPressionVide)
{
	char charPressionVide[nSizeFloat];
	TCHAR TCHARPressionVide[nSizeFloat];
	sprintf_s(charPressionVide,"%1.6f",fPressionVide);
	wsprintf(TCHARPressionVide,_T(charPressionVide));
	WritePrivateProfileString(_T("Appareil"),_T("Pression_limite_vide"),TCHARPressionVide,Fichier_parametres);
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
	char charVolumeRef[nSizeFloat];
	TCHAR TCHARVolumeRef[nSizeFloat];
	sprintf_s(charVolumeRef,"%1.4f",fVolumeRef);
	wsprintf(TCHARVolumeRef,_T(charVolumeRef));
	WritePrivateProfileString(_T("Appareil"),_T("Volume_ref"),TCHARVolumeRef,Fichier_parametres);
}

void SetVolumeP6(float fVolumeP6)
{
	char charVolumeP6[nSizeFloat];
	TCHAR TCHARVolumeP6[nSizeFloat];
	sprintf_s(charVolumeP6,"%1.4f",fVolumeP6);
	wsprintf(TCHARVolumeP6,_T(charVolumeP6));
	WritePrivateProfileString(_T("Appareil"),_T("Volume_P6"),TCHARVolumeP6,Fichier_parametres);
}




void SetTypeInstrument(int num, int TypeInstrument)
{
	char charTypeInstrument[nSizeInt];
	TCHAR TCHARTypeInstrument[nSizeInt];
	sprintf_s(charTypeInstrument,"%d",TypeInstrument);
	wsprintf(TCHARTypeInstrument,_T(charTypeInstrument));
	CString strTitre;
	strTitre.Format(_T("Instrument%d"),num);
	WritePrivateProfileString(strTitre,_T("Type"),TCHARTypeInstrument,Fichier_parametres);
}

void SetTypeInstrument1(int TypeInstrument)
{SetTypeInstrument(1,TypeInstrument);}

void SetTypeInstrument2(int TypeInstrument)
{SetTypeInstrument(2,TypeInstrument);}

void SetTypeInstrument3(int TypeInstrument)
{SetTypeInstrument(3,TypeInstrument);}




void SetCOMInstrument(int num, int COMInstrument)
{
	char charCOMInstrument[nSizeInt];
	TCHAR TCHARCOMInstrument[nSizeInt];
	sprintf_s(charCOMInstrument,"%d",COMInstrument);
	wsprintf(TCHARCOMInstrument,_T(charCOMInstrument));
	CString strTitre;
	strTitre.Format(_T("Instrument%d"),num);
	WritePrivateProfileString(strTitre,_T("COM"),TCHARCOMInstrument,Fichier_parametres);
}

void SetCOMInstrument1(int COMInstrument)
{SetCOMInstrument(1,COMInstrument);}

void SetCOMInstrument2(int COMInstrument)
{SetCOMInstrument(2,COMInstrument);}

void SetCOMInstrument3(int COMInstrument)
{SetCOMInstrument(3,COMInstrument);}




void SetFonctionInstrument(int num, int FonctionInstrument)
{
	char charFonctionInstrument[nSizeInt];
	TCHAR TCHARFonctionInstrument[nSizeInt];
	sprintf_s(charFonctionInstrument,"%d",FonctionInstrument);
	wsprintf(TCHARFonctionInstrument,_T(charFonctionInstrument));
	CString strTitre;
	strTitre.Format(_T("Instrument%d"),num);
	WritePrivateProfileString(strTitre,_T("Fonction"),TCHARFonctionInstrument,Fichier_parametres);
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
	char charPortKeithley[nSizeInt];
	TCHAR TCHARPortKeithley[nSizeInt];
	sprintf_s(charPortKeithley,"%d",PortKeithley);
	wsprintf(TCHARPortKeithley,_T(charPortKeithley));
	WritePrivateProfileString(_T("Connection"),_T("RS232_Keithley"),TCHARPortKeithley,Fichier_parametres);
}

void SetPortMensor(int PortMensor)
{
	char charPortMensor[nSizeInt];
	TCHAR TCHARPortMensor[nSizeInt];
	sprintf_s(charPortMensor,"%d",PortMensor);
	wsprintf(TCHARPortMensor,_T(charPortMensor));
	WritePrivateProfileString(_T("Connection"),_T("RS232_Mensor"),TCHARPortMensor,Fichier_parametres);
}

void SetPortVannes(int PortVannes)
{
	char charPortVannes[nSizeInt];
	TCHAR TCHARPortVannes[nSizeInt];
	sprintf_s(charPortVannes,"%d",PortVannes);
	wsprintf(TCHARPortVannes,_T(charPortVannes));
	WritePrivateProfileString(_T("Connection"),_T("USB_Vannes"),TCHARPortVannes,Fichier_parametres);
}

void SetPortTemperatures(int PortTemperatures)
{
	char charPortTemperatures[nSizeInt];
	TCHAR TCHARPortTemperatures[nSizeInt];
	sprintf_s(charPortTemperatures,"%d",PortTemperatures);
	wsprintf(TCHARPortTemperatures,_T(charPortTemperatures));
	WritePrivateProfileString(_T("Connection"),_T("USB_Temperature"),TCHARPortTemperatures,Fichier_parametres);
}


// Ecriture du calo

void SetNomCalo(string NomCalo)
{
	TCHAR TCHARNomCalo[nSizeString];
	wsprintf(TCHARNomCalo,_T(NomCalo.c_str()));
	WritePrivateProfileString(_T("Calorimetre"),_T("Nom"),TCHARNomCalo,Fichier_parametres);
}
		
void SetEnteteCalo(string EnteteCalo)
{
	TCHAR TCHAREnteteCalo[nSizeString];
	wsprintf(TCHAREnteteCalo,_T(EnteteCalo.c_str()));
	WritePrivateProfileString(_T("Calorimetre"),_T("Entete"),TCHAREnteteCalo,Fichier_parametres);
}


// Ecriture de General

void SetCheminFichierGeneral(string CheminFichierGeneral)
{
	TCHAR TCHARCheminFichierGeneral[nSizeFichier];
	wsprintf(TCHARCheminFichierGeneral,_T(CheminFichierGeneral.c_str()));
	WritePrivateProfileString(_T("General"),_T("Chemin_Fichier"),TCHARCheminFichierGeneral,Fichier_parametres);
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
