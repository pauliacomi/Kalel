#include "StdAfx.h"
#include <iostream>
#include "Keithley.h"
//Rajout 
#include <sstream>
#include <math.h>


using namespace std;


Keithley::Keithley(void) : LiaisonRS232()
{
	g_dcb.BaudRate = 19200;				// On a le moyen d'augmenter le BaudRate du keithley.

	// Initialize the critical section
	InitializeCriticalSection(&Sync_keithley);
}

Keithley::~Keithley(void)
{
	// Close the connection
	LiaisonRS232::CloseCOM();

	// Delete the critical section
	DeleteCriticalSection(&Sync_keithley);
}

//============================
// Fonctions Ouverture, fermeture, initialisation...


bool Keithley::OpenCOM(int nId)
{
	bool ouverture = LiaisonRS232::OpenCOM(nId);

	if (ouverture)
	{
		errorKeep = "Keithley open: COM" + to_string(nId);
		return true;
	}
	else
	{
		errorKeep += "\nKeithley opening failed: COM" + to_string(nId);
		return false;
	}
}

bool Keithley::CloseCOM()
{
	bool fermeture = LiaisonRS232::CloseCOM();

	if (fermeture)
	{
		errorKeep = "Keithley closed";
		return true;
	}
	else
	{
		errorKeep += "\nKeithley closing failed";
		return false;
	}
}

bool Keithley::InitKeithley()
{
	int nBytesWritten = 0;
	int nbOctetsLus = 0;
	char buffer[256];
	
	//On met dans 'buffer' les instructions a donne au keithley pour qu'il puisse s'initialiser.
	//
	//	*RST => Reset command : Returns the 2182 to the *RST default conditions 
	//							EXPERIMENT_TYPE_MANUAL du Keithley, page 12-12
	//	*CLS => Clear status : Clears all event registers and Error Queue 
	//						   EXPERIMENT_TYPE_MANUAL du Keithley, page 12-2
	//	:SENS:VOLT:CHAN1:LPAS OFF => :SENS:VOLT     Path to configure DC Volts
	//								  :CHAN1        Channel 1 voltage commands
	//								   :LPAS OFF    Control analog filter for DCV1
	//								 EXPERIMENT_TYPE_MANUAL du Keithley 14-8
	//	:SENS:VOLT:CHAN1:DFIL:STAT OFF => :DFIL      Configure and control digital filter
	//									   :STAT OFF Enable or disable digital filter
	//									  EXPERIMENT_TYPE_MANUAL du Keithley 14-8
	//
	//On fait les 2 dernieres instructions egalement pour le channel 2.
	//Le caractere '\n' represente le saut de ligne et permet de signaler la fin d'une execution

	sprintf_s(buffer,"*RST\n*CLS\n:SENS:VOLT:CHAN1:LPAS OFF\n:SENS:VOLT:CHAN1:DFIL:STAT OFF\n:SENS:VOLT:CHAN2:LPAS OFF\n:SENS:VOLT:CHAN2:DFIL:STAT OFF\n");


	//On va ecrire dans le port COM pour initialiser le Keithley.
	//Si on a pu ecrire au COM - WriteCOM retournant 'true' - l'initialisation a ete effectue,
	//on retourne 'true', sinon, on retourne 'false'
	EnterCriticalSection(&Sync_keithley);
	bool success = WriteCOM(buffer, (int)strlen(buffer), &nBytesWritten);
	LeaveCriticalSection(&Sync_keithley);

	if(success)
	{
		errorKeep = "Keithley initialised";
		return true;
	}
	else
	{
		errorKeep += "\nKeithley initialisation failure";
		return false;
	}
}



// La fonction renverra si la lecture s'est faite ou pas (true ou false)
// Elle donnera aussi le retour de lecture du Keithley qu'on mettra 
// dans 'result' (mis en parametre)

bool Keithley::ReadChannel(int chanNo, double* result)
{
	int nBytesWritten = 0;
	int nbOctetsLus = 0;
	char buffer[256];
	bool success;

	// Start by sending message to ask for the data

	//On met dans 'buffer' les instructions a donne au keithley pour qu'il puisse nous donner
	//la mesure du Channel 1
	//
	//	*CLS => Clear status : Clears all event registers and Error Queue 
	//						   EXPERIMENT_TYPE_MANUAL du Keithley, page 12-2
	//	:SENS:CHAN1 => Select channel to measure; 0,1 or 2 (0=internal temperature sensor).
	//				   EXPERIMENT_TYPE_MANUAL du Keithley 14-8

	sprintf_s(buffer,"*CLS\nSENS:CHAN %d\n:READ?\n", chanNo);

	EnterCriticalSection(&Sync_keithley);
	success = WriteCOM(buffer, (int)strlen(buffer), &nBytesWritten);
	LeaveCriticalSection(&Sync_keithley);

	if (!success)
		return false;

	// Then receive the data in an array

	// Si tout va bien, nbOctetsLus = 16
	// Le Keithley nous renvoie 16 octets qui represente une chaine de caracteres donnant
	// la valeur scientifique de la mesure
	//
	// Exemples : 
	//	-1.16305416E-01(13)
	//  +1.23456753E+02(13)
	//
	// Le premier caract�re est le signe de la valeur
	// Du 2eme au 11eme caractere, c'est la valeur d�cimal de la mesure
	// le 12eme caractere est 'E'
	// le 13eme caractere esr le signe de la puissance 10
	// le 14eme et 15eme caractere sont la puissance 10
	// le 16eme caractere est le caractere 13 representant le retour a la ligne :
	//							<cr> Carriage return en code ascii
	//
	// On rajoute a buffer[16] (representant le 17eme caractere)le caractere '\0' qui est
	// la fin d'une chaine.
	//
	// On pourra voir si on rajoute une fonction pour verifier que la valeur renvoyer est la bonne.

	EnterCriticalSection(&Sync_keithley);
	success = ReadCOM(buffer, 256);
	LeaveCriticalSection(&Sync_keithley);

	// On ne va garder de 'buffer' que les 15 premiers caracteres. On elimine le retour a la ligne
	// On mettra cette chaine de caractere dans 'resultat'.
	string temp = buffer;
	temp = temp.substr(0,15);
	*result = atof(temp.c_str());

	errorKeep = "Channel " + to_string(chanNo) + " read";
	return true;
}


// Cette Fonction reprendra le retour de celle avec un string en parametre
// le '(string)resultat' sera converti en double et sera attribu� au 
// '(double)resultat' qui est mis en parametre.

bool Keithley::ReadChannel1(double* resultat)
{
	return ReadChannel(1, resultat);
}

bool Keithley::ReadChannel2(double* resultat)
{
	return ReadChannel(2, resultat);
}
