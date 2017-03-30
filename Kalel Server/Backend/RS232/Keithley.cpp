#include "Keithley.h"

#include <iostream>
#include <sstream>



Keithley::Keithley(int comport) : LiaisonRS232()
{
	g_dcb.BaudRate = 19200;								// On a le moyen d'augmenter le BaudRate du keithley.

	connectionOpen = LiaisonRS232::OpenCOM(comport);	// Open port
	if (connectionOpen)	{
		errorKeep = "Keithley open: COM" + std::to_string(comport);

		InitKeithley();									// Init Keithley
	}
	else {
		errorKeep += "\nKeithley opening failed: COM" + std::to_string(comport);
	}
}

Keithley::~Keithley(void)
{
	if (connectionOpen) {
		// Close the connection
		LiaisonRS232::CloseCOM();
	}
}

//============================
// Fonctions Ouverture, fermeture, initialisation...


bool Keithley::OpenCOM(int nId)
{
	connectionOpen = LiaisonRS232::OpenCOM(nId);

	if (connectionOpen)
	{
		errorKeep = "Keithley open: COM" + std::to_string(nId);
		return true;
	}
	else
	{
		errorKeep += "\nKeithley opening failed: COM" + std::to_string(nId);
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
	
	// On met dans 'buffer' les instructions a donne au keithley pour qu'il puisse s'initialiser.
	//
	//		*RST => Reset command : Returns the 2182 to the *RST default conditions 
	//								EXPERIMENT_TYPE_MANUAL du Keithley, page 12-12
	//		*CLS => Clear status : Clears all event registers and Error Queue 
	//							   EXPERIMENT_TYPE_MANUAL du Keithley, page 12-2
	//		:SENS:VOLT:CHANx:LPAS OFF => :SENS:VOLT     Path to configure DC Volts
	//									 :CHAN1			Channel x voltage commands
	//									 :LPAS OFF		Control analog filter for DCV1
	//									 EXPERIMENT_TYPE_MANUAL du Keithley 14-8
	//		:SENS:VOLT:CHANx:DFIL:STAT OFF => :DFIL			Configure and control digital filter
	//										  :STAT OFF		Enable or disable digital filter
	//										  EXPERIMENT_TYPE_MANUAL du Keithley 14-8
	//
	// On fait les 2 dernieres instructions egalement pour le channel 2.
	// Le caractere '\n' represente le saut de ligne et permet de signaler la fin d'une execution

	sprintf_s(buffer, sizeof(buffer),
		"%s%s%s%s%s%s",		// six strings
		"*RST\n",
		"*CLS\n",
		":SENS:VOLT:CHAN1:LPAS OFF\n",
		":SENS:VOLT:CHAN1:DFIL:STAT OFF\n",
		":SENS:VOLT:CHAN2:LPAS OFF\n"
		":SENS:VOLT:CHAN2:DFIL:STAT OFF\n");


	//On va ecrire dans le port COM pour initialiser le Keithley.
	//Si on a pu ecrire au COM - WriteCOM retournant 'true' - l'initialisation a ete effectue,
	//on retourne 'true', sinon, on retourne 'false'
	bool success = WriteCOM(buffer, (int)strlen(buffer), &nBytesWritten);

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

	// The reading and writing has to be successive, otherwise a thread can ask for the read of one channel 
	// and actually read the result of another channel read which was requested from another thread.
	// Normally critical section SHOULD NOT be used with potentially blocking parts of code
	// Due to the OVERLAPPED reading and writing of the serial port, we are guaranteed not to block the thread for more than MAX_READ + MAX_WRITE (about 500 ms) 
	
	std::unique_lock<std::mutex> lock(mutex_keithley);

	// Start by sending message to ask for the data

	//On met dans 'buffer' les instructions a donne au keithley pour qu'il puisse nous donner
	//la mesure du Channel 1
	//
	//	*CLS => Clear status : Clears all event registers and Error Queue 
	//						   EXPERIMENT_TYPE_MANUAL du Keithley, page 12-2
	//	:SENS:CHAN1 => Select channel to measure; 0,1 or 2 (0=internal temperature sensor).
	//				   EXPERIMENT_TYPE_MANUAL du Keithley 14-8

	sprintf_s(buffer, sizeof(buffer),
		"%s%s%d%s%s",
		"*CLS\n",
		":SENS:CHAN ", chanNo,"\n",
		":READ?\n");

	success = WriteCOM(buffer, (int)strlen(buffer), &nBytesWritten);
	if (!success) {
		lock.unlock();
		return false;
	}

	// Then receive the data in an array

	// Si tout va bien, nbOctetsLus = 16
	// Le Keithley nous renvoie 16 octets qui represente une chaine de caracteres donnant
	// la valeur scientifique de la mesure
	//
	// Exemples : 
	//	-1.16305416E-01(13)
	//  +1.23456753E+02(13)
	//
	// Le premier caractère est le signe de la valeur
	// Du 2eme au 11eme caractere, c'est la valeur décimal de la mesure
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

	success = ReadCOM(buffer, 256);
	if (!success) {
		lock.unlock();
		return false;
	}

	// Can now leave the critical section 
	lock.unlock();
	
	// On ne va garder de 'buffer' que les 15 premiers caracteres. On elimine le retour a la ligne
	// On mettra cette chaine de caractere dans 'resultat'.
	std::string temp = buffer;
	temp = temp.substr(0,15);
	*result = atof(temp.c_str());

	errorKeep = "Channel " + std::to_string(chanNo) + " read";
	return true;
}


// Cette Fonction reprendra le retour de celle avec un string en parametre
// le '(string)resultat' sera converti en double et sera attribué au 
// '(double)resultat' qui est mis en parametre.

bool Keithley::ReadChannel1(double* resultat)
{
	return ReadChannel(1, resultat);
}

bool Keithley::ReadChannel2(double* resultat)
{
	return ReadChannel(2, resultat);
}
