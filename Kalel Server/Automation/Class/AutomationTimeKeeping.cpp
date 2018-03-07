#include "Automation.h"


// Wait a number of minutes
void Automation::WaitMinutes(int nbminutes)
{
	WaitSeconds(nbminutes * 60);
}

// Wait a number of seconds
void Automation::WaitSeconds(int nbsecondes)
{
	// Record times
	storage.experimentStatus->timeEquilibrationStart = timeh::TimePointToULLong(timeh::NowTime());		// Save the waiting start time
	storage.experimentStatus->timeToEquilibrate = nbsecondes;
	storage.experimentStatus->experimentWaiting = true;
	controls.timerWaiting.Start();
}
