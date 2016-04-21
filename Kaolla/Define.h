#pragma once
#ifndef DEFINE_H
#define DEFINE_H

#include "Define_Stages.h"
#include "Define_Experiment_Type.h"
#include "Define_Instrument.h"

#define GREY_OUT FALSE
#define UN_GREY_OUT TRUE

#define T_BETWEEN_MEASURE 10 //secondes

#define ERROR_MESURE -300000

#define PassageNul  0
#define PassageCOM1 1
#define PassageCOMs 2


// les demandes d'arrêts
#define INACTIF				100
#define PAUSE				105
#define ARRET_IMMEDIAT		101
#define ARRET_SOUSVIDE		102
#define ARRET_ETAPE			103
#define ARRET_DOSE			104
#define ARRET_URGENCE_HP	106
#define ARRET_URGENCE_TCH	107
#define ARRET_URGENCE_TCB	108
#define ARRET_COMMANDE		109

#define INACTIVE			100
#define STOP_IMMEDIAT		101
#define STOP_UNDERVACCUM	102
#define STOP_STEP			103
#define STOP_DOSE			104
#define PAUSE				105
#define STOP				106
#define STOP_EMERGENCY_HP	107
#define STOP_EMERGENCY_TCH	108
#define STOP_EMERGENCY_TCB	109
#define STOP_COMMAND		110
#define ACTIVE				111


#define ArretManip(); 		if(demande_arret==ARRET_IMMEDIAT)	 {AffichageArret(); return;}
#define ArretSousVide(); 	if(demande_arret==ARRET_SOUSVIDE)	 {AffichageArret(); return;}
#define ArretUrgenceHP();	if(demande_arret==ARRET_URGENCE_HP)	 {AffichageArret(); return;}
#define ArretUrgenceTCH();	if(demande_arret==ARRET_URGENCE_TCH) {AffichageArret(); return;}
#define ArretUrgenceTCB();  if(demande_arret==ARRET_URGENCE_TCB) {AffichageArret(); return;}

#define ArretManipSousFonction(); 		if(demande_arret==ARRET_IMMEDIAT)	 return;
#define ArretSousVideSousFonction(); 	if(demande_arret==ARRET_SOUSVIDE)	 return;
#define ArretUrgenceHPSousFonction();	if(demande_arret==ARRET_URGENCE_HP)	 return;
#define ArretUrgenceTCHSousFonction();	if(demande_arret==ARRET_URGENCE_TCH) return;
#define ArretUrgenceTCBSousFonction();  if(demande_arret==ARRET_URGENCE_TCB) return;

// Rajout

#define ArretManipLEA(); 		if(demande_arret==ARRET_IMMEDIAT)	 return FALSE;
#define ArretSousVideLEA(); 	if(demande_arret==ARRET_SOUSVIDE)	 return FALSE;
#define ArretUrgenceHPLEA();	if(demande_arret==ARRET_URGENCE_HP)	 return FALSE;
#define ArretUrgenceTCHLEA();	if(demande_arret==ARRET_URGENCE_TCH) return FALSE;
#define ArretUrgenceTCBLEA();	if(demande_arret==ARRET_URGENCE_TCB) return FALSE;
#define ArretDoseLEA(); 		if(demande_arret==ARRET_DOSE)		 return FALSE;
#define ArretEtapeLEA();	 	if(demande_arret==ARRET_ETAPE)		 return FALSE;
#define ArretCommandeLEA();		if(demande_arret==ARRET_COMMANDE)	 return FALSE;

#define PointArretLEA();				ArretDoseLEA();	ArretEtapeLEA(); ArretManipLEA(); ArretSousVideLEA(); ArretUrgenceHPLEA(); ArretUrgenceTCHLEA(); ArretUrgenceTCBLEA(); ArretCommandeLEA(); MiseEnPause();
#define PointArretSansDoseLEA();		ArretEtapeLEA(); ArretManipLEA(); ArretSousVideLEA(); ArretUrgenceHPLEA(); ArretUrgenceTCHLEA(); ArretUrgenceTCBLEA(); ArretCommandeLEA(); MiseEnPause();

#define PointArretMiseSousVideLEA();	ArretManipLEA(); MiseEnPause();

// Fin Rajout

#define ArretDoseBoucle();  		if(demande_arret==ARRET_DOSE) break;
#define ArretDoseSousFonction(); 	if(demande_arret==ARRET_DOSE) return;
#define ArretDoseFonction();  		if(demande_arret==ARRET_DOSE){ AffichageArret(); demande_arret=INACTIF; goto FinDose;}

#define ArretEtapeBoucle(); 		if(demande_arret==ARRET_ETAPE) break;
#define ArretEtapeSousFonction(); 	if(demande_arret==ARRET_ETAPE) return;
#define ArretEtapeFonction();  		if(demande_arret==ARRET_ETAPE){ AffichageArret(); demande_arret=INACTIF; return;}

#define ArretCommandeBoucle();			if(demande_arret==ARRET_COMMANDE) break;
#define ArretCommandeSousFonction(); 	if(demande_arret==ARRET_COMMANDE) return;
#define ArretCommandeFonction();  		if(demande_arret==ARRET_COMMANDE){ AffichageArret(); demande_arret=INACTIF;}


#define PointArretBoucle();						ArretDoseBoucle(); ArretEtapeBoucle(); ArretManip(); ArretSousVide(); ArretUrgenceHP(); ArretUrgenceTCH(); ArretUrgenceTCB(); ArretCommandeBoucle(); MiseEnPause();
#define PointArretSansDoseBoucle();				ArretEtapeBoucle(); ArretManip(); ArretSousVide(); ArretUrgenceHP(); ArretUrgenceTCH(); ArretUrgenceTCB(); ArretPCommandeBoucle();	MiseEnPause();

#define PointArretSousFonction();				ArretDoseSousFonction();ArretEtapeSousFonction(); ArretManipSousFonction(); ArretSousVideSousFonction(); ArretUrgenceHPSousFonction(); ArretUrgenceTCHSousFonction(); ArretUrgenceTCBSousFonction(); ArretCommandeSousFonction(); MiseEnPause();
#define PointArretSansDoseSousFonction();		ArretEtapeSousFonction(); ArretManipSousFonction(); ArretSousVideSousFonction(); ArretUrgenceHPSousFonction(); ArretUrgenceTCHSousFonction(); ArretUrgenceTCBSousFonction(); ArretCommandeSousFonction(); MiseEnPause();

#define PointArretFonction();					ArretDoseFonction();ArretEtapeFonction(); ArretManip(); ArretSousVide(); ArretUrgenceHP(); ArretUrgenceTCH(); ArretUrgenceTCB(); ArretCommandeFonction(); MiseEnPause();
#define PointArretSansDoseFonction();			ArretEtapeFonction(); ArretManip(); ArretSousVide(); ArretUrgenceHP(); ArretUrgenceTCH(); ArretUrgenceTCB(); ArretCommandeFonction(); MiseEnPause();

#define PointArretMiseSousVide();				ArretManip(); MiseEnPause();

#define ArretEtapeTemperature();		if(demande_arret==ARRET_ETAPE)		 {AffichageArret(); demande_arret=INACTIF; return 0;}
#define ArretManipTemperature();		if(demande_arret==ARRET_IMMEDIAT)	 {AffichageArret(); return 0;}
#define ArretSousVideTemperature();		if(demande_arret==ARRET_SOUSVIDE)	 {AffichageArret(); return 0;}
#define ArretUrgenceHPTemperature();	if(demande_arret==ARRET_URGENCE_HP)	 {AffichageArret(); return 0;}
#define ArretUrgenceTCHTemperature();	if(demande_arret==ARRET_URGENCE_TCH) {AffichageArret(); return 0;}
#define ArretUrgenceTCBTemperature();	if(demande_arret==ARRET_URGENCE_TCB) {AffichageArret(); return 0;}


// rajouter les arrets d'urgence sous la température

#endif
