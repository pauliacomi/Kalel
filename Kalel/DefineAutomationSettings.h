#pragma once
#ifndef DEFINE_AUTOMATION_SETTINGS_H
#define DEFINE_AUTOMATION_SETTINGS_H

// Time values
#define T_BETWEEN_RECORD				4		//seconds
#define T_BETWEEN_MEASURE				500		//miliseconds

#define ERROR_MESURE					-300000

// Temperature range for security checks
#define securite_temperature				5		// °C
#define security_temperature_initial		1		// °C

// Used to see how many times a dose will be attempted
#define tentatives_possible				2		// Tentatives avant d'augmenter le multiplicateur

// Used to set limits and margins during the injections
#define nb_injection					5		// nombre d'injections avant de déclarer un problème de gaz
#define marge_injection					0.01	//bar   Si on ne dépasse pas cette marge, on consiède que la pression n'a pas changé
#define marge_multiplicateur			1.2		//bar

// Time to wait
#define TIME_WAIT_VALVES					5			// intervalle 5s, temps d'attente en plus après ouverture des vannes
#define TIME_WAIT_PUMP						10			// intervalle 5s, temps d'attente en plus après ouverture des vannes


// Opening and closing times for the valves
#define temps_ouverture					1000				//ms = 1s
#define temps_fermeture					temps_ouverture		// => temps_fermeture identique a temps ouverture

// Defines for putting under vacuum
#define pression_pompe					0.5 // Minimum pressure to be put directly on the pump
#define temps_en_cas_d_urgence			10 // minutes
#define temps_defaut					30 // minutes
#define temps_bouteille					5 // minutes

#endif