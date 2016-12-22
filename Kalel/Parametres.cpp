#include "stdafx.h"
#include "Parametres.h"

#include <iostream>
#include <fstream>


const int nSizeString{ 100 };


Parameters::Parameters()
	: fileLocation { L"./ParametresClient.ini" }
{
	std::ifstream file_stream(fileLocation, std::ifstream::in);
	bool ans = file_stream.is_open();
	file_stream.close();

	// si ans=TRUE , alors le fichier existe, pas besoin de le créer
	// si ans=FALSE, alors le fichier n'existe pas, on le crée et on initialise les parametres
	if (!ans)
	{
		Initialisation();
	}
}

Parameters::~Parameters()
{
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

	WritePrivateProfileString(_T("refreshinterval"), _T("data"), _T("100"), fileLocation);
	WritePrivateProfileString(_T("refreshinterval"), _T("graph"), _T("1000"), fileLocation);
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

void Parameters::SetServerAddress(std::wstring address)
{
	WritePrivateProfileString(_T("server"), _T("address"), address.c_str(), fileLocation);
}


std::wstring Parameters::GetServerPort()
{
	TCHAR ch[nSizeString];
	GetPrivateProfileString(__T("server"), _T("port"), _T(""), ch, nSizeString, fileLocation);
	std::wstring convertStr(ch);
	return convertStr;
}

void Parameters::SetServerPort(std::wstring port)
{
	WritePrivateProfileString(_T("server"), _T("port"), port.c_str(), fileLocation);
}


int Parameters::GetDataRefreshInterval()
{
	TCHAR ch[nSizeString];
	GetPrivateProfileString(__T("refreshinterval"), _T("data"), _T(""), ch, nSizeString, fileLocation);
	return _ttoi(ch);
}

void Parameters::SetDataRefreshInterval(int interval)
{
	WritePrivateProfileString(_T("refreshinterval"), _T("data"), std::to_wstring(interval).c_str(), fileLocation);
}


int Parameters::GetGraphRefreshInterval()
{
	TCHAR ch[nSizeString];
	GetPrivateProfileString(__T("refreshinterval"), _T("graph"), _T(""), ch, nSizeString, fileLocation);
	return _ttoi(ch);
}

void Parameters::SetGraphRefreshInterval(int interval)
{
	WritePrivateProfileString(_T("refreshinterval"), _T("graph"), std::to_wstring(interval).c_str(), fileLocation);
}