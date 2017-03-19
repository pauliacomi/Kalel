#ifndef COMCTRL_H
#define COMCTRL_H
#pragma once

#include "../../Kalel Server/MessageHandler.h"
#include "../../Kalel Server/Automation/FileWriter.h"
#include "../../Kalel Server/Backend/Wrapper Classes/ValveController.h"
#include "../../Kalel Shared/timeHelpers.h"

#include <memory>

class Controls {
public:

	Controls(Storage &h);
	~Controls(void);

	std::shared_ptr<MessageHandler> messageHandler;					// The message sender class. Thread Safe!
	std::shared_ptr<ValveController> valveControls;					// The valve control/query class
	std::shared_ptr<FileWriter> fileWriter;							// The file writing class

	// Timers
	timer timerExperiment;											// Class for measuring the time from the experiment start
	timer timerMeasurement;											// Class for measuring the time between each measurement
	timer timerWaiting;												// Class for measuring the time to wait
};



inline Controls::Controls(Storage &h)
{
	// Create objects from controls class
	fileWriter = std::make_shared<FileWriter>();
	messageHandler = std::make_shared<MessageHandler>(h);
	valveControls = std::make_shared<ValveController>(*messageHandler);
}

inline Controls::~Controls(void)
{
}


#endif