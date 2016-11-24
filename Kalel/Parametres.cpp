#include "stdafx.h"
#include "Parametres.h"

#include <iostream>
#include <fstream>


const int nSizeString{ 100 };


Parameters::Parameters()
	: fileLocation { L"./ParametresClient.ini" }
{
}

Parameters::~Parameters()
{
}

void Parameters::VerifParametres()
{
	std::ifstream file_stream(fileLocation, std::ifstream::in);
	bool ans = file_stream.is_open();
	file_stream.close();

	// si ans=TRUE , alors le fichier existe, pas besoin de le créer
	// si ans=FALSE, alors le fichier n'existe pas, on le crée et on initialise les parametres
	if(!ans)
	{
		Initialisation();
	}
}




void Parameters::Initialisation()
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


	WritePrivateProfileString(_T("server"),_T("address"),_T(""), fileLocation);
	WritePrivateProfileString(_T("server"), _T("port"), _T(""), fileLocation);
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

std::wstring Parameters::GetServerAddress()
{
	TCHAR ch[nSizeString];
	GetPrivateProfileString(__T("server"), _T("address"), _T(""), ch, nSizeString, fileLocation);
	std::wstring convertStr(ch);
	return convertStr;
}


std::wstring Parameters::GetServerPort()
{
	TCHAR ch[nSizeString];
	GetPrivateProfileString(__T("server"), _T("port"), _T(""), ch, nSizeString, fileLocation);
	std::wstring convertStr(ch);
	return convertStr;
}

void Parameters::SetServerAddress(std::wstring address)
{
	WritePrivateProfileString(_T("server"), _T("address"), address.c_str(), fileLocation);
}

void Parameters::SetServerPort(std::wstring port)
{
	WritePrivateProfileString(_T("server"), _T("address"), port.c_str(), fileLocation);
}