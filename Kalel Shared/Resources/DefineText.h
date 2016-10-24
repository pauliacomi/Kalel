#pragma once

#define	TEXT_NEWFILETEXT					"Nouveau_fichier_%s"
#define TEXT_SAMPLE							"Sample"

#define ERROR_PATHUNDEF						"The path chosen does not exist"



/***************************************************
//
//
//			AUTOMATION MESSAGES
//
//
****************************************************/
/*
//
//	GENERAL EXPERIMENT MESSAGES
//
*/

#define MESSAGE_FILLLINE					"***************************************************************"
#define MESSAGE_EXPSTART					"Experiment started"
#define MESSAGE_EXPFINISH					"Experiment finished"
#define MESSAGE_EXPCANCEL					"Experiment cancelled"
#define MESSAGE_EXPPAUSE					"Experiment paused"
#define MESSAGE_EXPRESUME					"Experiment resumed"
#define MESSAGE_WAIT_TIME					"Waiting for %d seconds..."
#define MESSAGE_THREAD_SHUTTINGDOWN			"Thread is shutting down"
#define MESSAGE_THREAD_SHUTDOWN				"Thread shut down"
#define MESSAGE_INSTRUMENT_INIT_FAIL		"Instrument initiation failed: \t\n%s"

/*
//
//	MESSAGES FOR VALVE AND PUMP COMMANDs
//
*/

#define MESSAGE_VALVE_OPENED				"Valve %d opened"
#define MESSAGE_VALVE_CLOSED				"Valve %d closed"
#define MESSAGE_EV_ACTIVATED				"EV %d activated"
#define MESSAGE_EV_DEACTIVATED				"EV %d deactivated"
#define MESSAGE_PUMP_ACTIVATED				"Pump activated"
#define MESSAGE_PUMP_DEACTIVATED			"Pump deactivated"
#define MESSAGE_VALVE_OPENCLOSE				"Opening and closing valve %d"
#define MESSAGE_VALVE_CLOSEALL				"Closing all valves"
#define MESSAGE_PUMP_VALVE_CLOSEALL			"Closing pump and valves"
#define MESSAGE_CLOSEEVERYTHING				"Closing everything"

/*
//
//	MESSAGES FOR SECURITY WARNINGS
//
*/

#define MESSAGE_WARNING_PHIGH				"Attention ! High PT above pressure limit \t\n Veuillez la mettre en dessous de : %f bar \t\n"
#define MESSAGE_WARNING_PHNORMAL			"HAUTE PRESSION (%f) DEPASSANT LA SECURITE DE LA BASSE PRESSION (%f)\r\n PAR MESURE DE SECURITE, FERMETURE DE LA VANNE 6\r\n"
#define MESSAGE_WARNING_CALOT_HIGH			"Attention ! Température du calo trop élevé par rapport à la température d'expérience\t\n"
#define MESSAGE_WARNING_CALOT_NORMAL		"Attention ! Température du calo trop faible par rapport à la température d'expérience\t\n"
#define MESSAGE_WARNING_CALOT_LOW			"Arrêt de l'expérience : La température du Calo étant trop élevée (supérieure à %d°C)\r\n"
#define MESSAGE_WARNING_PHIGH_BOX			"Arrêt de l'expérience : La température du Calo étant trop faible (inférieure à %d°C)\r\n"
#define MESSAGE_WARNING_PHIGH_V6			"High PT above pressure limit"
#define MESSAGE_WARNING_THIGH_BOX			"High PT has returned to normal"
#define MESSAGE_WARNING_TLOW_BOX			"Calorimeter temperature too high"
#define MESSAGE_WARNING_THIGH_STOP			"Calorimeter temperature above pressure limit"
#define MESSAGE_WARNING_TLOW_STOP			"Calorimeter temperature too low"

/*
//
//	MESSAGES FOR AUTOMATION STEPS
//
*/

// Initial checks messages
#define MESSAGE_NOSECURITY					"ATTENTION The security settings have been deactivated !\t\nDo you want to continue anyway?"
#define MESSAGE_CHECK_VALVES_OPEN			"Check if all the valves are set to automatic and that the valve to the sample is open"
#define MESSAGE_CHECK_INITIAL_STATE			"Initial experiment and machine verifications"
#define MESSAGE_CHECK_INITIAL_PRESSURE		"Checking residual pressure"
#define MESSAGE_CHECK_OPENV6_POSSIB			"Pressure recorded HPT : %f  Possibility to open valve 6"
#define MESSAGE_CHECK_INITIAL_TEMPERATURE	"Checking temperatures"					
#define MESSAGE_CHECK_TEMPERATURE_DIFF		"The temperature (= %f) is outside the bounds of the tepmerature limit (= %f) \t\nDo you want to wait for stabilisation (Yes),  continue anyway (No), or cancel the experiment (Cancel)?"
#define MESSAGE_WARNING_INITIAL_PRESSURE	"Waiting for temperature equilibration"
#define MESSAGE_WAIT_TEMP_EQUILIBRATION		"The PT high pressure (= %f) is higher than the set pressure limit (= %f) \t\nDo you want to continue anyway?"

// Equilibration messages
#define MESSAGE_EQUILIBRATION_STARTED		"Equilibration started"
#define MESSAGE_EQUILIBRATION_COMPLETE		"Equilibration complete"

// Adsorption messages
#define MESSAGE_ADSORPTION_STAGE_START		"Start of adsorption stage %d"
#define MESSAGE_ADSORPTION_STAGE_END		"End of adsorption stage %d"
#define MESSAGE_ADSORPTION_DOSE_START		"Start of adsorption stage %d dose %d"
#define MESSAGE_ADSORPTION_DOSE_END			"End of adsorption stage %d dose %d"
#define MESSAGE_ADSORPTION_OPENV			"Opening valve 5 for adsorption"
#define MESSAGE_ADSORPTION_CLOSEV			"Isolating sample"

// Desorption messages
#define MESSAGE_DESORPTION_STAGE_START		"Start of desorption stage %d"
#define MESSAGE_DESORPTION_STAGE_END		"End of desorption stage %d"
#define MESSAGE_DESORPTION_DOSE_START		"Start of desorption stage %d dose %d"
#define MESSAGE_DESORPTION_DOSE_END			"End of desorption stage %d dose %d"
#define MESSAGE_DESORPTION_OPENV			"Opening valve 5 for desorption"
#define MESSAGE_DESORPTION_CLOSEV			"Isolating sample"

// Injection messages
#define MESSAGE_INJECTION_ATTEMPT			"Starting injection in the reference volume"
#define MESSAGE_INJECTION_PROBLEM			"Injection problem"
#define MESSAGE_INJECTION_PROBLEM_BOX		"ATTENTION !!! Gas problem !!!\t\n (gaz moteur ou gaz d'analyse)\t\n Expérience en pause !\r\n N'oubliez pas de 'reprendre' l'expérience !"
#define MESSAGE_INJECTION_END				"Injection step complete"

// Outgassing messages
#define MESSAGE_OUTGAS_ATTEMPT				"Starting gas removal from reference volume"
#define MESSAGE_OUTGAS_PROBLEM				"Gas removal problem"
#define MESSAGE_OUTGAS_PROBLEM_BOX			"ATTENTION !!! Gas problem !!!\t\n (gaz moteur ou gaz d'analyse)\t\n Expérience en pause !\r\n N'oubliez pas de 'reprendre' l'expérience !"
#define MESSAGE_OUTGAS_END					"End of gas removal from reference volume"

// Pressure reporting messages
#define MESSAGE_PRESSURE_D_PI				"Initial pressure = %f"
#define MESSAGE_PRESSURE_D_PF				"Final pressure = %f"
#define MESSAGE_PRESSURE_D_DP				"Final pressure - Initial pressure = %f"
#define MESSAGE_PRESSURE_D_DPREQ			"Delta pressure = %f"

// Vaccumming messages
#define MESSAGE_VACUUM_STAGE_START			"Starting final outgassing"
#define MESSAGE_VACUUM_STAGE_END			"Completed final outgassing"
#define MESSAGE_VACUUM_HIGHPRESSURE_START	"Starting final outgassing of high pressure range volume"
#define MESSAGE_VACUUM_HIGHPRESSURE_END		"Completed final outgassing of high pressure range volume"
#define MESSAGE_VACUUM_LOWPRESSURE_START	"Starting final outgassing of low pressure range volume"
#define MESSAGE_VACUUM_LOWPRESSURE_END		"Completed final outgassing of low pressure range volume"
#define MESSAGE_VACUUM_FINALOUTGAS_START	"Starting final outgassing vacuum"
#define MESSAGE_VACUUM_FINALOUTGAS_END		"Completed final outgassing vacuum"
#define MESSAGE_VACUUM_USER_CHOICE			"Finished vacuum up to sample cell \t\nPress OK to turn off valves and pump"

#define MESSAGE_VACUUM_BOTTLE_CLOSE			"Fermer la bouteille !\tPuis appuyer sur OK\n"
#define MESSAGE_VACUUM_BOTTLE_START			"Mise sous vide jusqu'à la bouteille"
#define MESSAGE_VACUUM_BOTTLE_END			"Mise sous vide jusqu'à la bouteille terminée\t\n"