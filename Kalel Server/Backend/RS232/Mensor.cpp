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



Mensor::Mensor(int comport)
{
	portRS = comport;
	OpenCOM(portRS);
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
		LOG(logDEBUG2) << "Mensor open: COM" << std::to_string(nId);
		return true;
	}
	else
	{
		LOG(logDEBUG1) << "\nMensor opening failed: COM" << std::to_string(nId);
		return false;
	}
}

bool Mensor::CloseCOM()
{
	bool fermeture = RS232::CloseCOM();

	if (fermeture)
	{
		LOG(logDEBUG2) << "Mensor closed";
		return true;
	}
	else
	{
		LOG(logDEBUG1) << "Mensor closing failed";
		return false;
	}
}

void Mensor::SetComPort(int comport)
{
	if (CloseCOM())
	{
		portRS = comport;
		OpenCOM(comport);
	}
}

int Mensor::GetComPort()
{
	return portRS;
}

//=============================================================================
//On vérifie qu'on peut lire le Mensor. Si c'est le cas, on instanciera la valeur
//dans le double pression et on renverra TRUE. Sinon on instancie le message d'erreur
//et on renvoie FALSE.

double Mensor::Read()
{
	int nBytesWritten = 0;
	int nbOctetsLus = 0;
	bool success;

	// Start by sending message to ask for the data
	
	//la commande "#1?\n" permet de récupérer la pression du mensor.
	//le 1 correspond à l'adresse appareil utilisé
	//le ? correspond à la recherche de la valeur
	
	success = WriteCOM(query_template, (int)strlen(query_template), &nBytesWritten);
	if (!success)
		return false;

	// Then receive the data in an array

	//le mensor nous retourne 13 caractères.
	//le premier est l'adresse de l'appareil
	//les 3 suivants sont des espaces
	//les 7 prochains nous retourne la valeur de la pression
	//ex : 1   1.00631<cr><lf>
	//Puis les 2 derniers sont le caractère 13 et 10

	char buffer[256] = { "\0" };

	success = ReadCOM(buffer, sizeof(buffer));
	if (!success)
		return false;


	double result;
	if (buffer[0] == '\0') {
		LOG(logDEBUG2) << "Mensor read nothing";
		result = 0;
	}
	else {
		LOG(logDEBUG2) << "Mensor read: " << buffer;
		result = std::stod(std::string(buffer).substr(1, std::string::npos));
	}

	buffer[0] = 0;

	return result;
}