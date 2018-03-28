#include "Automation.h"

// Utilities
#include "../../../Kalel Shared/log.h"

// Wait a number of minutes
void Automation::WaitMinutes(int nbminutes, bool verbose /*=false*/)
{
	WaitSeconds(nbminutes * 60, verbose);
}

// Wait a number of seconds
void Automation::WaitSeconds(int nbsecondes, bool verbose /*=false*/)
{
	if (verbose)
	{
		LOG(logINFO) << MESSAGE_WAIT_TIME << nbsecondes;
	}
	// Record times
	storage.experimentStatus.timeEquilibrationStart = timeh::TimePointToMs(timeh::NowTime());		// Save the waiting start time
	storage.experimentStatus.timeToEquilibrate = nbsecondes;
	storage.experimentStatus.experimentWaiting = true;
	storage.timerWaiting.Start();
}
