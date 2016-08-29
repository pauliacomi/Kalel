
// KalelView_Boutons.cpp : all the code for the valve buttons in the view
//

#include "stdafx.h"
#include "Kalel.h"

#include "KalelDoc.h"
#include "KalelView.h"

#include "ManualActionParam.h"										// The class that has the manual parameters stored
#include "../Kalel Shared/Resources/DefineInstruments.h"			// Definitions for instruments
#include "ListOfInstrumentButtons.h"								// The class containing a list of the instruments' buttons ID's



// Single function to ask for thread start of manual controls
void CKalelView::AskThreadForManualCommand(int instrumentType, int instrumentNumber, bool shouldBeActivated) {

	// Create a new list object
	ListOfInstrumentButtons list(instrumentType, instrumentNumber, shouldBeActivated);

	// Block the required button
	GetDlgItem(list.GetButtonID())->EnableWindow(FALSE);

	// Show that the action has started
	SetDlgItemText(list.GetTextboxID(), list.GetTempTextboxMessage());

	// lock the menu
	pApp->menuIsAvailable = false;

	// Create the storage object and then pass it to the threading function
	commHandler.ManualCommand(instrumentType, instrumentNumber, shouldBeActivated);
}


// Single function to update UI when receiving the command that the thread posted before finishing
LRESULT CKalelView::OnThreadRequestButtonUpdate(WPARAM wParam, LPARAM lParam) {

	// Cast the parameters object and take ownership
	std::auto_ptr<ManualActionParam> maParam(reinterpret_cast<ManualActionParam*>(wParam));

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
		CString * temp = new CString(list.GetTempTextboxMessage());
		AffichageMessages(0, (LPARAM) temp);
	}

	// unlock the menu
	pApp->menuIsAvailable = true;

	return 0;
}



//-------------------- Individual functions for buttons ------------------


// Clicking on valve "open" type buttons
void CKalelView::OnBnClickedOuvrir1()
{	AskThreadForManualCommand(INSTRUMENT_VALVE,1,true);	}

void CKalelView::OnBnClickedOuvrir2()
{	AskThreadForManualCommand(INSTRUMENT_VALVE,2,true);	}

void CKalelView::OnBnClickedOuvrir3()
{	AskThreadForManualCommand(INSTRUMENT_VALVE,3,true);	}

void CKalelView::OnBnClickedOuvrir4()
{	AskThreadForManualCommand(INSTRUMENT_VALVE,4,true);	}

void CKalelView::OnBnClickedOuvrir5()
{	AskThreadForManualCommand(INSTRUMENT_VALVE,5,true);	}

void CKalelView::OnBnClickedOuvrir6()
{	AskThreadForManualCommand(INSTRUMENT_VALVE,6,true);	}

void CKalelView::OnBnClickedOuvrir7()
{	AskThreadForManualCommand(INSTRUMENT_VALVE,7,true);	}

void CKalelView::OnBnClickedOuvrir8()
{	AskThreadForManualCommand(INSTRUMENT_VALVE,8,true);	}


// Clicking on valve "close" type buttons
void CKalelView::OnBnClickedFermer1()
{	AskThreadForManualCommand(INSTRUMENT_VALVE, 1, false);	}

void CKalelView::OnBnClickedFermer2()
{	AskThreadForManualCommand(INSTRUMENT_VALVE, 2, false);	}

void CKalelView::OnBnClickedFermer3()
{	AskThreadForManualCommand(INSTRUMENT_VALVE, 3, false);	}

void CKalelView::OnBnClickedFermer4()
{	AskThreadForManualCommand(INSTRUMENT_VALVE, 4, false);	}

void CKalelView::OnBnClickedFermer5()
{	AskThreadForManualCommand(INSTRUMENT_VALVE, 5, false);	}

void CKalelView::OnBnClickedFermer6()
{	AskThreadForManualCommand(INSTRUMENT_VALVE, 6, false);	}

void CKalelView::OnBnClickedFermer7()
{	AskThreadForManualCommand(INSTRUMENT_VALVE, 7, false);	}

void CKalelView::OnBnClickedFermer8()
{	AskThreadForManualCommand(INSTRUMENT_VALVE, 8, false);	}


// The other buttons
void CKalelView::OnBnClickedActiverEV1()
{	AskThreadForManualCommand(INSTRUMENT_EV, 1, true);	}

void CKalelView::OnBnClickedDesactiverEV1()
{	AskThreadForManualCommand(INSTRUMENT_EV, 1, false);	}

void CKalelView::OnBnClickedActiverEV2()
{	AskThreadForManualCommand(INSTRUMENT_EV, 2, true);;	}

void CKalelView::OnBnClickedDesactiverEV2()
{	AskThreadForManualCommand(INSTRUMENT_EV, 2, false);	}

void CKalelView::OnBnClickedActiverPompe()
{	AskThreadForManualCommand(INSTRUMENT_PUMP, 1, true);;	}

void CKalelView::OnBnClickedDesactiverPompe()
{	AskThreadForManualCommand(INSTRUMENT_PUMP, 1, false);	}