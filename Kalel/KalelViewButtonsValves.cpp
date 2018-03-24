
// KalelView_Boutons.cpp : all the code for the valve buttons in the view
//

#include "stdafx.h"
#include "Kalel.h"
#include "KalelView.h"

#include "../Kalel Shared/Resources/DefineInstruments.h"			// Definitions for instruments



// Single function to ask for thread start of manual controls
void CKalelView::AskThreadForManualCommand(int instrumentType, int instrumentNumber, bool shouldBeActivated) {

	if (pApp->serverConnected)
	{
		// Update buttons
		buttonStates.StartCommand(instrumentType + instrumentNumber, shouldBeActivated);

		// lock the menu
		pApp->menuIsAvailable = false;

		// Create the storage object and then pass it to the threading function
		commHandler.ManualCommand(instrumentType + instrumentNumber, shouldBeActivated);
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}
}


//-------------------- Individual functions for buttons ------------------


// Clicking on valve "open" type buttons
void CKalelView::OnBnClickedOuvrir1()
{	AskThreadForManualCommand(CONTROLLER_VALVE,VALVE_1,true);	}

void CKalelView::OnBnClickedOuvrir2()
{	AskThreadForManualCommand(CONTROLLER_VALVE,VALVE_2,true);	}

void CKalelView::OnBnClickedOuvrir3()
{	AskThreadForManualCommand(CONTROLLER_VALVE,VALVE_3,true);	}

void CKalelView::OnBnClickedOuvrir4()
{	AskThreadForManualCommand(CONTROLLER_VALVE,VALVE_4,true);	}

void CKalelView::OnBnClickedOuvrir5()
{	AskThreadForManualCommand(CONTROLLER_VALVE, VALVE_5,true);	}

void CKalelView::OnBnClickedOuvrir6()
{	AskThreadForManualCommand(CONTROLLER_VALVE, VALVE_6,true);	}

void CKalelView::OnBnClickedOuvrir7()
{	AskThreadForManualCommand(CONTROLLER_VALVE, VALVE_7,true);	}

void CKalelView::OnBnClickedOuvrir8()
{	AskThreadForManualCommand(CONTROLLER_VALVE, VALVE_8,true);	}


// Clicking on valve "close" type buttons
void CKalelView::OnBnClickedFermer1()
{	AskThreadForManualCommand(CONTROLLER_VALVE, VALVE_1, false);	}

void CKalelView::OnBnClickedFermer2()
{	AskThreadForManualCommand(CONTROLLER_VALVE, VALVE_2, false);	}

void CKalelView::OnBnClickedFermer3()
{	AskThreadForManualCommand(CONTROLLER_VALVE, VALVE_3, false);	}

void CKalelView::OnBnClickedFermer4()
{	AskThreadForManualCommand(CONTROLLER_VALVE, VALVE_4, false);	}

void CKalelView::OnBnClickedFermer5()
{	AskThreadForManualCommand(CONTROLLER_VALVE, VALVE_5, false);	}

void CKalelView::OnBnClickedFermer6()
{	AskThreadForManualCommand(CONTROLLER_VALVE, VALVE_6, false);	}

void CKalelView::OnBnClickedFermer7()
{	AskThreadForManualCommand(CONTROLLER_VALVE, VALVE_7, false);	}

void CKalelView::OnBnClickedFermer8()
{	AskThreadForManualCommand(CONTROLLER_VALVE, VALVE_8, false);	}


// The other buttons
void CKalelView::OnBnClickedActiverEV1()
{	AskThreadForManualCommand(CONTROLLER_EV, EV_1, true);	}

void CKalelView::OnBnClickedDesactiverEV1()
{	AskThreadForManualCommand(CONTROLLER_EV, EV_1, false);	}

void CKalelView::OnBnClickedActiverEV2()
{	AskThreadForManualCommand(CONTROLLER_EV, EV_2, true);;	}

void CKalelView::OnBnClickedDesactiverEV2()
{	AskThreadForManualCommand(CONTROLLER_EV, EV_2, false);	}

void CKalelView::OnBnClickedActiverPompe()
{	AskThreadForManualCommand(CONTROLLER_PUMP, PUMP, true);;	}

void CKalelView::OnBnClickedDesactiverPompe()
{	AskThreadForManualCommand(CONTROLLER_PUMP, PUMP, false);	}