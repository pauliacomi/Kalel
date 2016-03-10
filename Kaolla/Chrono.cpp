#include "StdAfx.h"
#include "Chrono.h"

CChrono::CChrono(void)
{
}

CChrono::~CChrono(void)
{
}

void CChrono::TopChrono()
{
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
	temps = 0;
}

float CChrono::TempsActuel()
{
	// il vaudra mieux utiliser la fonction QueryPerformanceCounter. 
	// La pr�cision du compteur utilis�e par cette fonction peut �tre connue 
	// � l'aide de QueryPerformanceFrequency. 
	// Ces deux fonctions travaillent sur des entiers larges (LARGE_INTERGER), 
	// une union repr�sentant un entier de 64 bits. 
	// Le membre QuadPart de cette union permet d'acc�der int�gralement � l'entier qu'il repr�sente.

	QueryPerformanceCounter(&end);
	// Conversion en milliseconde 
	// (plus pr�cis que lorsqu'on calcul en secondes... on ne sait pas pourquoi) 
	double elapsed = (1000.0 * (end.QuadPart - start.QuadPart)) / freq.QuadPart;
	temps = elapsed/1000.0;
	return temps; //en secondes
}

float CChrono::TempsActuel(LARGE_INTEGER endEtranger)
{
	double elapsed = (1000.0 * (endEtranger.QuadPart - start.QuadPart)) / freq.QuadPart;
	temps = elapsed/1000.0;
	return temps; //en secondes
}

int CChrono::TempsActuelMinute()
{
	return (int)(TempsActuel()/60.0);
}

int CChrono::TempsActuelMinute(LARGE_INTEGER endEtranger)
{
	return (int)(TempsActuel(endEtranger)/60.0);
}

float CChrono::fTempsActuelMinute()
{
	return TempsActuel()/60.0;
}

float CChrono::fTempsActuelMinute(LARGE_INTEGER endEtranger)
{
	return TempsActuel(endEtranger)/60.0;
}

float CChrono::TempsActuelMillis()
{
	return TempsActuel()*1000.0;
}

float CChrono::TempsActuelMillis(LARGE_INTEGER endEtranger)
{
	return TempsActuel(endEtranger)*1000.0;
}

CString CChrono::StrTempsActuel()
{
	CString StrTemps;
	int minutes = (int)temps/60;
	int secondes = (int)(temps - (60*minutes));
	StrTemps.Format(_T("%d min %02d"),minutes,secondes);
	return StrTemps;
}


void CChrono::ArretTemps()
{
	QueryPerformanceCounter(&pause);
}


void CChrono::RepriseTemps()
{
	QueryPerformanceCounter(&reprise);
	start.QuadPart = reprise.QuadPart - (pause.QuadPart - start.QuadPart);
}