#include "Automation.h"


// Wait a number of minutes
void Automation::WaitMinutes(int nbminutes)
{
	WaitSeconds(nbminutes * 60);
}

// Wait a number of seconds
void Automation::WaitSeconds(int nbsecondes)
{
	storage.currentData->timeToEquilibrate = nbsecondes;
	storage.currentData->experimentWaiting = true;
	controls.timerWaiting.TopChrono();
}
