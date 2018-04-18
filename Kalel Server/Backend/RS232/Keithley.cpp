#include "Keithley.h"

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
//						Baud		19200
//						Data Bits	  8
//						Parity		 None
//						Stop Bits	  1
//=====================================================================


Keithley::Keithley(int comport)
{
	g_dcb.BaudRate = 19200;								// On a le moyen d'augmenter le BaudRate du keithley.
	portRS = comport;

	if (OpenCOM(comport))								// Open port
	{
		InitKeithley();
	}
}

Keithley::~Keithley(void)
{
	if (connectionOpen) {
		// Close the connection
		RS232::CloseCOM();
	}
}

//============================
// Fonctions Ouverture, fermeture, initialisation...


bool Keithley::OpenCOM(int nId)
{
	connectionOpen = RS232::OpenCOM(nId);

	if (connectionOpen)
	{
		LOG(logDEBUG2) << "Keithley open: COM" << std::to_string(nId);
		return true;
	}
	else
	{
		LOG(logDEBUG1) << "Keithley opening failed: COM" << std::to_string(nId);
		return false;
	}
}

bool Keithley::CloseCOM()
{
	bool fermeture = RS232::CloseCOM();

	if (fermeture)
	{
		LOG(logDEBUG2) << "Keithley closed";
		return true;
	}
	else
	{
		LOG(logDEBUG1) << "Keithley closing failed";
		return false;
	}
}

void Keithley::SetComPort(int comport)
{
	if (CloseCOM())
	{
		portRS = comport;
		if (OpenCOM(comport))
		{
			// Finally init keithley
			InitKeithley();									// Init Keithley
		}
	}
}

int Keithley::GetComPort()
{
	return portRS;
}

bool Keithley::InitKeithley()
{
	int nBytesWritten = 0;
	char init_buf[256];
	
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

	sprintf_s(init_buf, sizeof(init_buf),
		"%s%s%s%s%s%s",		// six strings
		mens_rst,
		mens_cls,
		mens_chan1_lpas,
		mens_chan1_dfil,
		mens_chan2_lpas,
		mens_chan2_dfil);

	//On va ecrire dans le port COM pour initialiser le Keithley.
	//Si on a pu ecrire au COM - WriteCOM retournant 'true' - l'initialisation a ete effectue,
	//on retourne 'true', sinon, on retourne 'false'
	bool success = WriteCOM(init_buf, (int)strlen(init_buf), &nBytesWritten);

	if(success)
	{
		LOG(logDEBUG2) << "Keithley initialised";
		return true;
	}
	else
	{
		LOG(logDEBUG1) << "Keithley initialisation failure";
		return false;
	}
}



// La fonction renverra si la lecture s'est faite ou pas (true ou false)
// Elle donnera aussi le retour de lecture du Keithley qu'on mettra 
// dans 'result' (mis en parametre)

bool Keithley::ReadChan(int chanNo, double* result)
{
	int nBytesWritten = 0;
	char write_buf[256] = { 0 };
	bool success;

	// Start by sending message to ask for the data

	//On met dans 'buffer' les instructions a donne au keithley pour qu'il puisse nous donner
	//la mesure du Channel 1
	//
	//	*CLS => Clear status : Clears all event registers and Error Queue 
	//						   EXPERIMENT_TYPE_MANUAL du Keithley, page 12-2
	//	:SENS:CHAN1 => Select channel to measure; 0,1 or 2 (0=internal temperature sensor).
	//				   EXPERIMENT_TYPE_MANUAL du Keithley 14-8

	sprintf_s(write_buf, sizeof(write_buf),
		"%s%s%d%s%s",
		mens_cls,
		":SENS:CHAN ", chanNo,"\n",
		mens_read);

	// The reading and writing has to be successive, otherwise a thread can ask for the read of one channel 
	// and actually read the result of another channel read which was requested from another thread.
	// Normally critical section SHOULD NOT be used with potentially blocking parts of code
	// Due to the OVERLAPPED reading and writing of the serial port, we are guaranteed not to block the thread for more than MAX_READ + MAX_WRITE (about 500 ms) 

	std::lock_guard<std::mutex> lock(mutex_keithley);

	success = WriteCOM(write_buf, (int)strlen(write_buf), &nBytesWritten);
	if (!success) {
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

	char read_buf[256] = { 0 };

	success = ReadCOM(read_buf, sizeof(read_buf));
	if (!success) {
		return false;
	}
	
	if (read_buf[0] == '\0') {
		LOG(logDEBUG2) << "Keithley channel " << chanNo << " read nothing";
		*result = 0;
	}
	else {
		LOG(logDEBUG3) << "Keithley channel " << chanNo << " read: " << read_buf;
		*result = std::stod(std::string(read_buf));
	}
	return true;
}


// Cette Fonction reprendra le retour de celle avec un string en parametre
// le '(string)resultat' sera converti en double et sera attribué au 
// '(double)resultat' qui est mis en parametre.

double Keithley::Read(unsigned int chanNo)
{
	double result;
	ReadChan(chanNo, &result);
	return result;
}
