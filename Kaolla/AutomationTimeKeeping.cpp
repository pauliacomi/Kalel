
// This file contains Automation functions that are used to make the system wait


#include "StdAfx.h"
#include "Automation.h"


// Wait a number of minutes
void Automation::WaitMinutes(int nbminutes)
{
	WaitSeconds(nbminutes * 60);
}

// Wait a number of seconds
void Automation::WaitSeconds(int nbsecondes)
{
	experimentLocalData.timeToEquilibrate = nbsecondes;
	experimentLocalData.experimentWaiting = true;
	timerWaiting.TopChrono();
}
