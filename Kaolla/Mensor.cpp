#include "StdAfx.h"
#include <iostream>
#include "Mensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <windows.h>

using namespace std;

CRITICAL_SECTION Sync_mensor;
//=====================================================================
//                    Serial Port Configuration
//
//						Item		Value
//						----		-----
//						Type		RS-232
//						Address		  1
//						Baud		 9600
//						Data Bits	  8
//						Parity		 None
//						Stop Bits	  1
//=====================================================================



Mensor::Mensor(void) : LiaisonRS232()
{ 
	message = "Mensor non connecté";
}



Mensor::~Mensor(void)
{
	LiaisonRS232::CloseCOM();
}

bool Mensor::OpenCOM(int nId)
{
	bool ouverture = LiaisonRS232::OpenCOM(nId);
	// créer un flux de sortie
    ostringstream nPort;
    // écrire un nombre dans le flux
    nPort << nId;
    // récupérer une chaîne de caractères
    string port = nPort.str();

	if (ouverture)
	{
		message = "Le Mensor est ouvert au port COM" + port;
		return true;
	}
	else
	{
		message = "Erreur : Ouverture du Mensor échoué au port COM" + port;
		return false;
	}
}

bool Mensor::OpenCOM(int nId,string* info)
{
	bool ouverture = OpenCOM(nId);
	*info = message;
	return ouverture;
}

bool Mensor::CloseCOM()
{
	bool fermeture = LiaisonRS232::CloseCOM();

	if (fermeture)
	{
		message = "Le Mensor est fermé";
		return true;
	}
	else
	{
		message = "Erreur : La fermeture du Mensor a échoué";
		return false;
	}
}

bool Mensor::CloseCOM(string *info)
{
	bool fermeture = CloseCOM();
	*info = message;
	return fermeture;
}

//=============================================================================
//On vérifie qu'on peut lire le Mensor. Si c'est le cas, on instanciera la valeur
//dans le double pression et on renverra TRUE. Sinon on instancie le message d'erreur
//et on renvoie FALSE.

//void Mensor::ReadMensor()
bool Mensor::ReadMensor(double* pression)
{
	int nBytesWritten = 0;
	int nbOctetsLus = 0;

	//fgets(buffer,256,":READ?");	//acquisition de la données a envoyer
	sprintf_s(buffer,"#1?<cr>\n");
	//la commande "#1?<cr>\n" permet de récupérer la pression du mensor.
	//le 1 correspond à l'adresse appareil utilisé
	//le ? correspond à la recherche de la valeur
	//le <cr> correspond au "carriage return", un retour à la ligne, le caractère 13
	//			dans le code ascii

	InitializeCriticalSection(&Sync_mensor);

	EnterCriticalSection(&Sync_mensor);

	WriteCOM(buffer, (int)strlen(buffer), &nBytesWritten);

	/*
	if(WriteCOM(buffer, (int)strlen(buffer), &nBytesWritten)) 
	{
	*/

	LeaveCriticalSection(&Sync_mensor);

	EnterCriticalSection(&Sync_mensor);

	while(ReadCOM(buffer, 256)==-1){}


	LeaveCriticalSection(&Sync_mensor);

	DeleteCriticalSection(&Sync_mensor);

			//le mensor nous retourne 13 caractères.
			//le premier est l'adresse de l'appareil
			//les 3 suivants sont des espaces
			//les 7 prochains nous retourne la valeur de la pression
			//ex : 1   1.00631<cr><lf>
			//Puis les 2 derniers sont le caractère 13 et 10
			//On ignore donc les 4 premiers caractères et on prend les nbOctetsLus-6
			// suivants qui représentent la pression

			string resultat = buffer;
			resultat = resultat.substr(4,nbOctetsLus-6);
			*pression = atof(resultat.c_str()); //conversion du string en float
			message = "Lecture du Mensor effectué";
			return true;
	/*
		}
		else //de ReadCOM, si on n'arrive pas à lire
		{
			message = "Erreur : Pas de reception";
			return false;
		}
	}
	else //de WriteCOM, si on n'arrive pas à écrire
	{
		message = "Erreur : Envoi non effectue";
		return false;
	}
	*/
}

bool Mensor::ReadMensor(double* pression, string* info)
{
	bool read = ReadMensor(pression);
	*info = message;
	return read;
}

double Mensor::ReadMensor()
{
	double pression;
	ReadMensor(&pression);
	return pression;
}

//================================================================================
//Renvoie les messages que ce soit d'erreur ou d'information
string Mensor::getMessage()
{return message;}

