
// This file should be rewritten as a class in the future

#include "Parametres.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>

#include <iostream>
#include <fstream>

#define nSizeInt 6
#define nSizeFloat 20
#define nSizeString 100
#define nSizeFichier 255
#define Fichier_parametres_char "./ParametresClient.ini"
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
		Initialisation();
	}
}




void Initialisation()
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


	WritePrivateProfileString(_T("server"),_T("address"),_T(""),Fichier_parametres);
	WritePrivateProfileString(_T("server"), _T("port"), _T(""), Fichier_parametres);
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

std::wstring GetServerAddress()
{
	TCHAR ch[nSizeString];
	GetPrivateProfileString(__T("server"), _T("address"), _T(""),
		ch, nSizeString, Fichier_parametres);
	std::wstring convertStr(ch);
	return convertStr;
}


std::wstring GetServerPort()
{
	TCHAR ch[nSizeString];
	GetPrivateProfileString(__T("server"), _T("port"), _T(""),
		ch, nSizeString, Fichier_parametres);
	std::wstring convertStr(ch);
	return convertStr;
}

void SetServerAddress(std::wstring address)
{
	WritePrivateProfileString(_T("server"), _T("address"), address.c_str(), Fichier_parametres);
}

void SetServerPort(std::wstring port)
{
	WritePrivateProfileString(_T("server"), _T("address"), port.c_str(), Fichier_parametres);
}