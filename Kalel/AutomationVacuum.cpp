
// This file contains Automation functions that are used to put things under vaccuum


#include "StdAfx.h"
#include "Automation.h"


void Automation::SampleVacuum()
{
	switch (experimentLocalData.experimentStepStatus)
	{
	case STEP_STATUS_START:
		experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;												// Set next step
		messageHandler.DisplayMessage(MESSAGE_DESORPTION_STAGE_START);													// Let GUI know the step change

		ControlMechanismsCloseAll();																					// Close all valves
		ActivatePump();
		ValveOpen(5);

		messageHandler.DisplayMessage(_T("Début Mise sous vide de la haute pression\r\n"));
		messageHandler.DisplayStep(_T("Mise sous vide : Haute Pression"));
		experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;

		break;

	case STEP_STATUS_INPROGRESS:
		
		if (experimentLocalData.pressureHigh > GetPressionSecuriteBassePression()) {

			ValveOpenClose(8);
			ValveOpenClose(7);
		}
		else
		{
			messageHandler.DisplayMessage(_T("Fin Mise sous vide de la haute pression\r\n"));
			messageHandler.DisplayMessage(_T("Début Mise sous vide de la basse pression\r\n"));
			messageHandler.DisplayStep(_T("Mise sous vide : Basse Pression"));
			ValveOpen(6);

			experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS + 1;
		}
		break;

	case STEP_STATUS_INPROGRESS + 1:


		if (experimentLocalData.pressureHigh > ?????) {

			ValveOpenClose(8);
			ValveOpenClose(7);
		}
		else
		{
			messageHandler.DisplayMessage(_T("Fin Mise sous vide de la basse pression\r\n"));

			ValveOpen(8);
			ValveOpen(7);

			experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS + 2;
		}
		break;

	case STEP_STATUS_INPROGRESS + 2:


	
		break;

	case STEP_STATUS_END:
		experimentLocalData.experimentStepStatus = STEP_STATUS_START;													// Let GUI know the step change

		if (experimentLocalData.desorptionCounter < experimentLocalSettings.dataDesorption.size())
		{
			experimentLocalData.desorptionCounter++;
		}
		else
		{
			experimentLocalData.experimentStage = STAGE_END_AUTOMATIC;														// Set desorption if all adsorption stages have been finished
		}
		break;
}

void Automation::BottleVacuum()
{
}
