#include "StdAfx.h"
#include <iostream>
#include "Keithley.h"
//Rajout 
#include <sstream>
#include <math.h>


using namespace std;

CRITICAL_SECTION Sync;

Keithley::Keithley(void) : LiaisonRS232()
{
	g_dcb.BaudRate = 19200; // On a le moyen d'augmenter le BaudRate du keithley.
	message = "Keithley non connect�";
}

Keithley::~Keithley(void)
{
	LiaisonRS232::CloseCOM();
}

//============================
// Fonctions Ouverture, fermeture, initialisation...


bool Keithley::OpenCOM(int nId)
{
	bool ouverture = LiaisonRS232::OpenCOM(nId);
	// cr�er un flux de sortie
    ostringstream nPort;
    // �crire un nombre dans le flux
    nPort << nId;
    // r�cup�rer une cha�ne de caract�res
    string port = nPort.str();

	if (ouverture)
	{
		message = "Keithley ouvert au port COM" + port;
		return true;
	}
	else
	{
		message = "Erreur : Ouverture du Keithley �chou� au port COM" + port;
		return false;
	}
}

bool Keithley::OpenCOM(int nId,string* info)
{
	bool ouverture = OpenCOM(nId);
	*info = message;
	return ouverture;
}

bool Keithley::CloseCOM()
{
	bool fermeture = LiaisonRS232::CloseCOM();

	if (fermeture)
	{
		message = "Le Keithley est ferm�";
		return true;
	}
	else
	{
		message = "Erreur : La fermeture du Keithley a �chou�";
		return false;
	}
}

bool Keithley::CloseCOM(string *info)
{
	bool fermeture = CloseCOM();
	*info = message;
	return fermeture;
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
	if(WriteCOM(buffer, (int)strlen(buffer), &nBytesWritten))
	{
		message = "Initialisation du Keithley r�ussie";
		return true;
	}
	else
	{
		message = "Initialisation �chou�e";
		return false;
	}
}

bool Keithley::InitKeithley(string* info)
{
	bool initialisation = InitKeithley();
	*info = message;
	return initialisation;
}


//==================================================
// Fonctions du channel1


// La fonction renverra si la lecture s'est faite ou pas (true ou false)
// Elle donnera aussi le retour de lecture du Keithley qu'on mettra 
// dans 'result' (mis en parametre)

bool Keithley::ReadChannel1(string* result)
{
	int nBytesWritten = 0;
	int nbOctetsLus = 0;
	char buffer[256];

	//fgets(buffer,256,":READ?");	//acquisition de la donn�es a envoyer

	//On met dans 'buffer' les instructions a donne au keithley pour qu'il puisse nous donner
	//la mesure du Channel 1
	//
	//	*CLS => Clear status : Clears all event registers and Error Queue 
	//						   EXPERIMENT_TYPE_MANUAL du Keithley, page 12-2
	//	:SENS:CHAN1 => Select channel to measure; 0,1 or 2 (0=internal temperature sensor).
	//				   EXPERIMENT_TYPE_MANUAL du Keithley 14-8

	sprintf_s(buffer,"*CLS\nSENS:CHAN 1\n:READ?\n");

	//printf("Envoi des donnes...\n");

	InitializeCriticalSection(&Sync);

	EnterCriticalSection(&Sync);

	WriteCOM(buffer, (int)strlen(buffer), &nBytesWritten);

	/*if(WriteCOM(buffer, (int)strlen(buffer), &nBytesWritten)) 
	{*/

	LeaveCriticalSection(&Sync);

	EnterCriticalSection(&Sync);

	/*
		if(ReadCOM(buffer,256 , &nbOctetsLus))
		{
	*/

	//ReadCOM(buffer,256 , &nbOctetsLus);

	while(ReadCOM(buffer, 256)==-1){}

	LeaveCriticalSection(&Sync);

	DeleteCriticalSection(&Sync);

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


			// On ne va garder de 'buffer' que les 15 premiers caracteres. On elimine le retour a la ligne
			// On mettra cette chaine de caractere dans 'resultat'.
			string temp = buffer;
			temp = temp.substr(0,15);
			*result = temp;

			message = "Lecture du Channel 1 effectu�";
			return true;

/*
		}
		else
		{
			message = "Erreur : Pas de reception";
			return false;
		}
	}
	else //de WriteCOM
	{
		message = "Erreur : Envoi non effectue";
		return false;
	}
*/
}

bool Keithley::ReadChannel1(string* result, string* info)
{
	bool read = ReadChannel1(result);
	*info = message;
	return read;
}


// Cette Fonction reprendra le retour de celle avec un string en parametre
// le '(string)resultat' sera converti en double et sera attribu� au 
// '(double)resultat' qui est mis en parametre.

bool Keithley::ReadChannel1(double* resultat)
{
	string temp;
	bool read = ReadChannel1(&temp);
	*resultat = atof(temp.c_str());
	return read;
}

bool Keithley::ReadChannel1(double* resultat, string* info)
{
	bool read = ReadChannel1(resultat);
	*info = message;
	return read;
}


bool Keithley::ReadChannel1(string* result, double* resultat)
{
	string temp;
	bool read = ReadChannel1(&temp);
	*result = temp;
	*resultat = atof(temp.c_str());
	return read;
}

bool Keithley::ReadChannel1(string* result, double* resultat, string* info)
{
	bool read = ReadChannel1(result,resultat);
	*info = message;
	return read;
}

string Keithley::ReadChannel1()
{
	string result;
	ReadChannel1(&result);
	return result;
}

double Keithley::ReadChannel1_double()
{
	double resultat;
	ReadChannel1(&resultat);
	return resultat;
}

//=============================================
// Fonctions du Channel2

bool Keithley::ReadChannel2(string* result)
{
	int nBytesWritten = 0;
	int nbOctetsLus = 0;
	char buffer[256];

	sprintf_s(buffer,"*CLS\nSENS:CHAN 2\n:READ?\n");

//printf("Envoi des donnes...\n");

	InitializeCriticalSection(&Sync);

	EnterCriticalSection(&Sync);

	WriteCOM(buffer, (int)strlen(buffer), &nBytesWritten);

	/*if(WriteCOM(buffer, (int)strlen(buffer), &nBytesWritten)) 
	{*/

	LeaveCriticalSection(&Sync);

	EnterCriticalSection(&Sync);

	/*
		if(ReadCOM(buffer,256 , &nbOctetsLus))
		{
	*/

	//ReadCOM(buffer,256 , &nbOctetsLus);

	while(ReadCOM(buffer, 256)==-1){}

	LeaveCriticalSection(&Sync);

	DeleteCriticalSection(&Sync);

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


			// On ne va garder de 'buffer' que les 15 premiers caracteres. On elimine le retour a la ligne
			// On mettra cette chaine de caractere dans 'resultat'.
			string temp = buffer;
			temp = temp.substr(0,15);
			*result = temp;

			message = "Lecture du Channel 2 effectu�";
			return true;
}

bool Keithley::ReadChannel2(string* result, string* info)
{
	bool read = ReadChannel2(result);
	*info = message;
	return read;
}



bool Keithley::ReadChannel2(double* resultat)
{
	string temp;
	bool read = ReadChannel2(&temp);
	*resultat = atof(temp.c_str());
	return read;
}

bool Keithley::ReadChannel2(double* resultat, string* info)
{
	bool read = ReadChannel2(resultat);
	*info = message;
	return read;
}


bool Keithley::ReadChannel2(string* result, double* resultat)
{
	string temp;
	bool read = ReadChannel2(&temp);
	*result = temp;
	*resultat = atof(temp.c_str());
	return read;
}

bool Keithley::ReadChannel2(string* result, double* resultat, string* info)
{
	bool read = ReadChannel2(result,resultat);
	*info = message;
	return read;
}

string Keithley::ReadChannel2()
{
	string result;
	ReadChannel2(&result);
	return result;
}

double Keithley::ReadChannel2_double()
{
	double resultat;
	ReadChannel2(&resultat);
	return resultat;
}

//==================================
// Autres fonctions

string Keithley::getMessage()
{return message;}

double Keithley::Conversion(string resultat)
{return atof(resultat.c_str());}