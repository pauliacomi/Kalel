#pragma once


/*
//
//	GENERAL EXPERIMENT MESSAGES
//
*/


#define MESSAGE_EXPSTART					"Experiment started"
#define MESSAGE_EXPFINISH					"Experiment finished"
#define MESSAGE_EXPCANCEL					"Experiment cancelled"
#define MESSAGE_EXPPAUSE					"Experiment paused"
#define MESSAGE_EXPRESUME					"Experiment resumed"
#define MESSAGE_WAIT_TIME					"Waiting for %d seconds..."
#define MESSAGE_THREAD_SHUTTINGDOWN			"Thread is shutting down"
#define MESSAGE_THREAD_SHUTDOWN_AUTO		"Automation thread shut down"
#define MESSAGE_THREAD_SHUTDOWN_MEAS		"Measurement thread shut down"
#define MESSAGE_THREAD_START_AUTO			"Automation thread started"
#define MESSAGE_THREAD_START_MEAS			"Measurement thread started"


/*
//
//	ERRORS
//
*/

#define ERROR_PATHUNDEF						"The path chosen does not exist"
#define ERROR_FILE_WRITE					"Cannot write to file!"
#define ERROR_INSTRUMENT_INIT_FAIL			"Instrument initiation failed: \t\n%s"
#define ERROR_INSTRUMENT_DESTROY_FAIL		"Instrument removal failed: \t\n%s"
#define ERROR_INSTRUMENT_PORTCHANGE_FAIL	"Instrument port change failed: \t\n%s"

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
#define MESSAGE_VALVE_CLOSEALL				"Closing all valves"
#define MESSAGE_PUMP_VALVE_CLOSEALL			"Closing pump and valves"
#define MESSAGE_CLOSEEVERYTHING				"Closing everything"

/*
//
//	MESSAGES FOR SECURITY WARNINGS
//
*/

#define MESSAGE_WARNING_PHIGH				"OVERPRESSURE: High range PT pressure (%2.3f) is over the maximum (%2.3f)."
#define MESSAGE_WARNING_PHNORMAL			"High range PT pressure back to normal"
#define MESSAGE_WARNING_PHIGH_V6			"OVERPRESSURE: Low range PT pressure (%2.3f) is over the maximum (%2.3f). For safety valve 6 is closed"
#define MESSAGE_WARNING_PHNORMAL_V6			"Low range PT pressure back to normal"

#define MESSAGE_WARNING_CALOT_HIGH			"Calorimeter temperature (%2.3f) higher than limit (%2.3f)"
#define MESSAGE_WARNING_CALOT_LOW			"Calorimeter temperature (%2.3f) lower than limit (%2.3f)"
#define MESSAGE_WARNING_CALOT_NORMAL		"Calorimeter temperature returned to normal"
#define MESSAGE_WARNING_THIGH_STOP			"Experiment stopping :Calorimeter temperature (%2.3f) higher than limit (%2.3f)"
#define MESSAGE_WARNING_TLOW_STOP			"Experiment stopping: Calorimeter temperature (%2.3f) lower than limit (%2.3f)"

/*
//
//	MESSAGES FOR AUTOMATION STEPS
//
*/

// Initial checks messages
#define MESSAGE_NOSECURITY					"The security settings have been deactivated !\t\nDo you want to continue anyway?"
#define MESSAGE_CHECK_VALVES_OPEN			"Check if all the valves are set to automatic and that the valve to the sample is open"
#define MESSAGE_CHECK_INITIAL_STATE			"Initial experiment and machine verifications"
#define MESSAGE_CHECK_INITIAL_PRESSURE		"Checking residual pressure"
#define MESSAGE_CHECK_OPENV6_POSSIB			"Current pressure: (%2.4f)  Proceeding to open valve 6"
#define MESSAGE_CHECK_INITIAL_TEMPERATURE	"Checking temperatures"					
#define MESSAGE_CHECK_TEMPERATURE_DIFF		"The temperature (%2.3f) is outside the bounds of the tepmerature limit (%2.3f). Do you want to wait until stabilisation (Retry), continue anyway, or cancel the experiment?"
#define MESSAGE_WARNING_INITIAL_PRESSURE	"The pressure (%2.4f) is higher than the set pressure limit (%2.4f). Do you want to continue anyway?"
#define MESSAGE_WAIT_TEMP_EQUILIBRATION		"Waiting for temperature equilibration"
#define MESSAGE_VERIFICATIONS_COMPLETE		"Verifications are now complete"

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
#define MESSAGE_INJECTION_ATTEMPT			"Starting injection attempt %d"
#define MESSAGE_INJECTION_PROBLEM			"Injection problem"
#define MESSAGE_INJECTION_PROBLEM_BOX		"Could not increase pressure to required limit through injection. Possible gas problem. Experiment paused - press resume to try again"
#define MESSAGE_INJECTION_END				"Injection attempt %d complete"

// Outgassing messages
#define MESSAGE_OUTGAS_ATTEMPT				"Starting gas removal from reference volume %d"
#define MESSAGE_OUTGAS_PROBLEM				"Gas removal problem"
#define MESSAGE_OUTGAS_PROBLEM_BOX			"ATTENTION !!! Gas problem !!!\t\n (gaz moteur ou gaz d'analyse)\t\n Exp�rience en pause !\r\n N'oubliez pas de 'reprendre' l'exp�rience !"
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
#define MESSAGE_VACUUM_BOTTLE_START			"Mise sous vide jusqu'� la bouteille"
#define MESSAGE_VACUUM_BOTTLE_END			"Mise sous vide jusqu'� la bouteille termin�e\t\n"