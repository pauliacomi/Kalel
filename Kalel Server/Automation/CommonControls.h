#ifndef COMCTRL_H
#define COMCTRL_H
#pragma once

#include "../../Kalel Server/MessageHandler.h"
#include "../../Kalel Server/Automation/FileWriter.h"
#include "../../Kalel Server/Backend/Wrapper Classes/Vannes.h"

#include <memory>

class Controls {
public:
	std::shared_ptr<MessageHandler> messageHandler;					// The message sender class
	std::shared_ptr<CVannes> valveControls;							// The valve control/query class
	std::shared_ptr<FileWriter> fileWriter;							// The file writing class

	std::shared_ptr<MachineSettings> machineLocalSettings;			// The local machine settings are here
	std::shared_ptr<ExperimentSettings> experimentLocalSettings;	// The local experiment settings are here

	// Timers
	CChrono timerExperiment;										// Class for measuring the time from the experiment start
	CChrono timerMeasurement;										// Class for measuring the time between each measurement
	CChrono timerWaiting;											// Class for measuring the time to wait
};


#endif