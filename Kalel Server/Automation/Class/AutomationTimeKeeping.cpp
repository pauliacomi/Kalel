#include "Automation.h"

// Utilities
#include "../../../Kalel Shared/log.h"
#include "../../../Kalel Shared/stringHelpers.h"

// Wait a number of minutes
void Automation::WaitMinutes(int nbminutes, bool verbose /*=false*/)
{
	WaitSeconds(nbminutes * 60, verbose);
}

// Wait a number of seconds
void Automation::WaitSeconds(int seconds, bool verbose /*=false*/)
{
	if (verbose)
	{
		LOG(logINFO) << stringh::string_format(MESSAGE_WAIT_TIME, seconds);
	}
	// Record times
	storage.experimentStatus.timeWaitStart = timeh::TimePointToMs(timeh::NowTime());		// Save the waiting start time
	storage.experimentStatus.timeToWait = seconds;
	storage.experimentStatus.isWaiting = true;
	storage.timerWaiting.Start();
}
