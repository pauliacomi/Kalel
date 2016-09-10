#include "Mensor.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <windows.h>


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
}


Mensor::~Mensor(void)
{
	if (connectionOpen) {
		// Close the connection
		LiaisonRS232::CloseCOM();
	}
}

bool Mensor::OpenCOM(int nId)
{
	connectionOpen = LiaisonRS232::OpenCOM(nId);
	
	if (connectionOpen)
	{
		errorKeep = "Mensor open: COM" + std::to_string(nId);
		return true;
	}
	else
	{
		errorKeep += "\nMensor opening failed: COM" + std::to_string(nId);
		return false;
	}
}

bool Mensor::CloseCOM()
{
	bool fermeture = LiaisonRS232::CloseCOM();

	if (fermeture)
	{
		errorKeep = "Mensor closed";
		return true;
	}
	else
	{
		errorKeep += "\nMensor closing failed";
		return false;
	}
}


//=============================================================================
//On vérifie qu'on peut lire le Mensor. Si c'est le cas, on instanciera la valeur
//dans le double pression et on renverra TRUE. Sinon on instancie le message d'erreur
//et on renvoie FALSE.

bool Mensor::ReadMensor(double* pression)
{
	int nBytesWritten = 0;
	int nbOctetsLus = 0;
	bool success;

	// Start by sending message to ask for the data
	
	//la commande "#1?<cr>\n" permet de récupérer la pression du mensor.
	//le 1 correspond à l'adresse appareil utilisé
	//le ? correspond à la recherche de la valeur
	//le <cr> correspond au "carriage return", un retour à la ligne, le caractère 13
	// dans le code ascii
	sprintf_s(buffer,"#1?<cr>\n");
	
	success = WriteCOM(buffer, (int)strlen(buffer), &nBytesWritten);
	if (!success)
		return false;

	// Then receive the data in an array

	//le mensor nous retourne 13 caractères.
	//le premier est l'adresse de l'appareil
	//les 3 suivants sont des espaces
	//les 7 prochains nous retourne la valeur de la pression
	//ex : 1   1.00631<cr><lf>
	//Puis les 2 derniers sont le caractère 13 et 10
	//On ignore donc les 4 premiers caractères et on prend les nbOctetsLus-6
	// suivants qui représentent la pression

	success = ReadCOM(buffer, 256);
	if (!success)
		return false;

	std::string resultat = buffer;
	resultat = resultat.substr(4,nbOctetsLus-6);
	*pression = atof(resultat.c_str()); //conversion du string en float
	errorKeep = "Mensor read";
	return true;
}