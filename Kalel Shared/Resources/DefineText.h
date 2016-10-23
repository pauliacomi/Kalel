#pragma once


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