#include "Mensor.h"

#include "../../../Kalel Shared/log.h"

#include <iostream>
#include <sstream>


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



Mensor::Mensor(void)
{
}

Mensor::Mensor(int comport) : RS232()
{ 
	OpenCOM(comport);
}


Mensor::~Mensor(void)
{
	if (connectionOpen) {
		// Close the connection
		CloseCOM();
	}
}

bool Mensor::OpenCOM(int nId)
{
	connectionOpen = RS232::OpenCOM(nId);
	
	if (connectionOpen)
	{
		LOG(logDEBUG) << "Mensor open: COM" << std::to_string(nId);
		return true;
	}
	else
	{
		LOG(logERROR) << "\nMensor opening failed: COM" << std::to_string(nId);
		return false;
	}
}

bool Mensor::CloseCOM()
{
	bool fermeture = RS232::CloseCOM();

	if (fermeture)
	{
		LOG(logDEBUG) << "Mensor closed";
		return true;
	}
	else
	{
		LOG(logDEBUG) << "Mensor closing failed";
		return false;
	}
}

bool Mensor::ChangeCOM(int nId)
{
	if (CloseCOM())
	{
		return OpenCOM(nId);
	}
	return false;
}

//=============================================================================
//On vérifie qu'on peut lire le Mensor. Si c'est le cas, on instanciera la valeur
//dans le double pression et on renverra TRUE. Sinon on instancie le message d'erreur
//et on renvoie FALSE.

double Mensor::ReadMensor()
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
	resultat = resultat.substr(4, nbOctetsLus - 6);

	LOG(logDEBUG1) << "Mensor read";

	return atof(resultat.c_str()); //conversion du string en float;
}