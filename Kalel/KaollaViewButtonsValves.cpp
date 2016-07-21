
// KaollaView_Boutons.cpp : all the code for the valve buttons in the view
//

#include "stdafx.h"
#include "Kaolla.h"

#include "KaollaDoc.h"
#include "KaollaView.h"

#include "ThreadManager.h"

#include "ManualActionParam.h"			// The class that has the parameters inside
#include "DefineInstruments.h"			// Definitions for instruments
#include "ListOfInstrumentButtons.h"	// The class containing a list of the instruments' buttons ID's



// Single function to ask for thread start of manual controls
void CKaollaView::AskThreadForManualCommand(int instrumentType, int instrumentNumber, bool shouldBeActivated) {

	// Create a new list object
	ListOfInstrumentButtons list(instrumentType, instrumentNumber, shouldBeActivated);

	// Block the required button
	GetDlgItem(list.GetButtonID())->EnableWindow(FALSE);

	// Show that the action has started
	SetDlgItemText(list.GetTextboxID(), list.GetTempTextboxMessage());

	// lock the menu
	pApp->menuIsAvailable = false;

	// Create the storage object and then pass it to the threading function
	if (threadManager->maParam) {
		delete threadManager->maParam;
	}
	threadManager->maParam = new ManualActionParam(GetSafeHwnd(), instrumentType, instrumentNumber, shouldBeActivated);
	threadManager->ManualAction();
}


// Single function to update UI when receiving the command that the thread posted before finishing
LRESULT CKaollaView::OnThreadRequestButtonUpdate(WPARAM wParam, LPARAM lParam) {

	// Cast the parameters object and take ownership
	const ManualActionParam *maParam = reinterpret_cast<ManualActionParam*>(wParam);

	// Create a new list object
	ListOfInstrumentButtons list(maParam->instrumentType, maParam->instrumentNumber, maParam->shouldBeActivated);

	CString message;

	if (lParam) {
		// Disable required button
		GetDlgItem(list.GetButtonID())->EnableWindow(FALSE);

		// Enable required button
		GetDlgItem(list.GetOppositeButtonID())->EnableWindow(TRUE);

		// Write message in the textbox
		SetDlgItemText(list.GetTextboxID(), list.GetTextboxMessage());
	}
	else
	{
		AffichageMessages(list.GetTempTextboxMessage());
	}

	// unlock the menu
	pApp->menuIsAvailable = true;

	// Now delete the object
	delete maParam;

	return 0;
}



//-------------------- Individual functions for buttons ------------------


// Clicking on valve "open" type buttons
void CKaollaView::OnBnClickedOuvrir1()
{	AskThreadForManualCommand(INSTRUMENT_VALVE,1,true);	}

void CKaollaView::OnBnClickedOuvrir2()
{	AskThreadForManualCommand(INSTRUMENT_VALVE,2,true);	}

void CKaollaView::OnBnClickedOuvrir3()
{	AskThreadForManualCommand(INSTRUMENT_VALVE,3,true);	}

void CKaollaView::OnBnClickedOuvrir4()
{	AskThreadForManualCommand(INSTRUMENT_VALVE,4,true);	}

void CKaollaView::OnBnClickedOuvrir5()
{	AskThreadForManualCommand(INSTRUMENT_VALVE,5,true);	}

void CKaollaView::OnBnClickedOuvrir6()
{	AskThreadForManualCommand(INSTRUMENT_VALVE,6,true);	}

void CKaollaView::OnBnClickedOuvrir7()
{	AskThreadForManualCommand(INSTRUMENT_VALVE,7,true);	}

void CKaollaView::OnBnClickedOuvrir8()
{	AskThreadForManualCommand(INSTRUMENT_VALVE,8,true);	}


// Clicking on valve "close" type buttons
void CKaollaView::OnBnClickedFermer1()
{	AskThreadForManualCommand(INSTRUMENT_VALVE, 1, false);	}

void CKaollaView::OnBnClickedFermer2()
{	AskThreadForManualCommand(INSTRUMENT_VALVE, 2, false);	}

void CKaollaView::OnBnClickedFermer3()
{	AskThreadForManualCommand(INSTRUMENT_VALVE, 3, false);	}

void CKaollaView::OnBnClickedFermer4()
{	AskThreadForManualCommand(INSTRUMENT_VALVE, 4, false);	}

void CKaollaView::OnBnClickedFermer5()
{	AskThreadForManualCommand(INSTRUMENT_VALVE, 5, false);	}

void CKaollaView::OnBnClickedFermer6()
{	AskThreadForManualCommand(INSTRUMENT_VALVE, 6, false);	}

void CKaollaView::OnBnClickedFermer7()
{	AskThreadForManualCommand(INSTRUMENT_VALVE, 7, false);	}

void CKaollaView::OnBnClickedFermer8()
{	AskThreadForManualCommand(INSTRUMENT_VALVE, 8, false);	}


// The other buttons
void CKaollaView::OnBnClickedActiverEV1()
{	AskThreadForManualCommand(INSTRUMENT_EV, 1, true);	}

void CKaollaView::OnBnClickedDesactiverEV1()
{	AskThreadForManualCommand(INSTRUMENT_EV, 1, false);	}

void CKaollaView::OnBnClickedActiverEV2()
{	AskThreadForManualCommand(INSTRUMENT_EV, 2, true);;	}

void CKaollaView::OnBnClickedDesactiverEV2()
{	AskThreadForManualCommand(INSTRUMENT_EV, 2, false);	}

void CKaollaView::OnBnClickedActiverPompe()
{	AskThreadForManualCommand(INSTRUMENT_PUMP, 1, true);;	}

void CKaollaView::OnBnClickedDesactiverPompe()
{	AskThreadForManualCommand(INSTRUMENT_PUMP, 1, false);	}