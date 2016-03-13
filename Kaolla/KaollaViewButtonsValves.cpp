
// KaollaView_Boutons.cpp : all the code for the valve buttons in the view
//

#include "stdafx.h"
#include "Kaolla.h"

#include "KaollaDoc.h"
#include "KaollaView.h"

#include "threads.h"

#include "ManualActionParam.h"			// The class that has the parameters inside
#include "Define_Instrument.h"			// Definitions for instruments
#include "ListOfInstrumentButtons.h"	// The class containing a list of the instruments' buttons ID's


// Single function to ask for thread start of manual controls

void CKaollaView::AskThreadForManualCommand(int instrumentType, int instrumentNumber, bool shouldBeActivated) {

	// Create a new list object
	ListOfInstrumentButtons list;

	// Block the required button
	GetDlgItem(list.GetButtonID(instrumentType, instrumentNumber, shouldBeActivated))->EnableWindow(FALSE);

	// Show that the action has started
	CString message;
	if (shouldBeActivated)
		message.Format(TEXT_OPENING);
	else
		message.Format(TEXT_CLOSING);
	SetDlgItemText(list.GetTextboxID(instrumentType, instrumentNumber), message);

	// Create the storage object and then pass it to the threading function
	ManualActionParam * maParam = new ManualActionParam(GetSafeHwnd(), instrumentType, instrumentNumber, shouldBeActivated);
	ManualAction(maParam);
}

// Single function to update UI when receiving the command that the thread posted before finishing

LRESULT CKaollaView::OnThreadRequestButtonUpdate(WPARAM wParam, LPARAM lParam) {

	// Cast the parameters object
	const ManualActionParam *maParam = reinterpret_cast<ManualActionParam*>(wParam);

	// Create a new list object
	ListOfInstrumentButtons list;

	CString message;

	if (lParam) {
		// Disable required button
		GetDlgItem(list.GetButtonID(maParam->instrumentType, maParam->instrumentNumber, maParam->shouldBeActivated))->EnableWindow(FALSE);

		// Enable required button
		GetDlgItem(list.GetButtonID(maParam->instrumentType, maParam->instrumentNumber, !maParam->shouldBeActivated))->EnableWindow(TRUE);

		// Write message in the textbox
		if (maParam->shouldBeActivated)
			message.Format(TEXT_OPENED);
		else
			message.Format(TEXT_CLOSED);
		SetDlgItemText(list.GetTextboxID(maParam->instrumentType, maParam->instrumentNumber), message);
	}
	else // need to work on this error
	{
		switch (maParam->instrumentType)
		{
		case INSTRUMENT_VALVE:
			message.Format(ERROR_CLOSEVALVE, maParam->instrumentNumber);
		case INSTRUMENT_EV:
			message.Format(ERROR_OPENEV1, maParam->instrumentNumber);
		case INSTRUMENT_PUMP:
			message.Format(ERROR_CLOSEVALVE, maParam->instrumentNumber);
		default:
			break;
		}
		AffichageMessages(message);
	}

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





//////////////// care about these

void CKaollaView::MarquerValvesEtPompeDesactivees()
{
	CString message;
	message.Format(TEXT_DEACTIVATED);

	m_StrTemoinEV1 = message;
	SetDlgItemText(IDC_TEMOIN_EV1,m_StrTemoinEV1);

	m_StrTemoinEV2 = message;
	SetDlgItemText(IDC_TEMOIN_EV2,m_StrTemoinEV2);

	m_StrTemoinPompe = message;
	SetDlgItemText(IDC_TEMOIN_POMPE,m_StrTemoinPompe);

	GetDlgItem(IDC_DESACTIVER_EV1)->EnableWindow(FALSE);
	GetDlgItem(IDC_ACTIVER_EV1)->EnableWindow(TRUE);
	GetDlgItem(IDC_DESACTIVER_EV2)->EnableWindow(FALSE);
	GetDlgItem(IDC_ACTIVER_EV2)->EnableWindow(TRUE);
	GetDlgItem(IDC_DESACTIVER_POMPE)->EnableWindow(FALSE);
	GetDlgItem(IDC_ACTIVER_POMPE)->EnableWindow(TRUE);
}

void CKaollaView::MarquerTousLesTemoinsFermes()
{
	// Message for the text boxes
	CString message;
	message.Format(TEXT_CLOSED);

	m_StrTemoinVanne1 = message;
	SetDlgItemText(IDC_TEMOIN_VANNE1, m_StrTemoinVanne1);
	m_StrTemoinVanne2 = message;
	SetDlgItemText(IDC_TEMOIN_VANNE2, m_StrTemoinVanne2);
	m_StrTemoinVanne3 = message;
	SetDlgItemText(IDC_TEMOIN_VANNE3, m_StrTemoinVanne3);
	m_StrTemoinVanne4 = message;
	SetDlgItemText(IDC_TEMOIN_VANNE4, m_StrTemoinVanne4);
	m_StrTemoinVanne5 = message;
	SetDlgItemText(IDC_TEMOIN_VANNE5, m_StrTemoinVanne5);
	m_StrTemoinVanne6 = message;
	SetDlgItemText(IDC_TEMOIN_VANNE6, m_StrTemoinVanne6);
	m_StrTemoinVanne7 = message;
	SetDlgItemText(IDC_TEMOIN_VANNE7, m_StrTemoinVanne7);
	m_StrTemoinVanne8 = message;
	SetDlgItemText(IDC_TEMOIN_VANNE8, m_StrTemoinVanne8);

	for (int i = 1; i <= 8; i++)
	{
		GetDlgItem(idc_fermer[i - 1])->EnableWindow(FALSE);
		GetDlgItem(idc_ouvrir[i - 1])->EnableWindow(TRUE);
	}

	// Message for the main dialog
	CString messageDialog;
	messageDialog.Format(TEXT_ALLCLOSED);
	AffichageMessages(messageDialog);
}