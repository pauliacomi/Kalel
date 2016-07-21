#include "StdAfx.h"
#include "Manip_Auto.h"


using namespace std;


void CManip_Auto::LireAfficherBassePression()
{
	LectureBassePression();
	messageHandler.DisplayLowPressure();
}

void CManip_Auto::LireAfficherHautePression()
{
	LectureHautePression();
	messageHandler.DisplayHighPressure();
}

void CManip_Auto::LireAfficherCalo()
{
	LectureCalo();
	messageHandler.DisplayCalorimeter();
}


void CManip_Auto::InitialiserAfficherPressionInitiale(double init)
{
	PressionInitiale = init;
	messageHandler.DisplayInitialPressure();
}


void CManip_Auto::InitialiserAfficherPressionFinale(double init)
{
	PressionFinale = init;
	messageHandler.DisplayFinalPressure();
}