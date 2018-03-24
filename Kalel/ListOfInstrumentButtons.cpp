#include "stdafx.h"
#include "ListOfInstrumentButtons.h"

#include "KalelView.h"

#include "resource.h"											// For the MFC button id's
#include "../Kalel Shared/Resources/DefineInstruments.h"		// For the instrument definitions
#include "../Kalel Shared/Resources/StringTable.h"				// For the error message definitions


const int idcOpen[] = { IDC_OUVRIR1,IDC_OUVRIR2,IDC_OUVRIR3,IDC_OUVRIR4, IDC_OUVRIR5,IDC_OUVRIR6,IDC_OUVRIR7,IDC_OUVRIR8, IDC_ACTIVER_EV1, IDC_ACTIVER_EV2, IDC_ACTIVER_POMPE };
const int idcClose[] = { IDC_FERMER1,IDC_FERMER2,IDC_FERMER3,IDC_FERMER4, IDC_FERMER5,IDC_FERMER6,IDC_FERMER7,IDC_FERMER8, IDC_DESACTIVER_EV1, IDC_DESACTIVER_EV2, IDC_DESACTIVER_POMPE};
const int idcTextBox[] = { IDC_TEMOIN_VANNE1,IDC_TEMOIN_VANNE2,IDC_TEMOIN_VANNE3,IDC_TEMOIN_VANNE4, IDC_TEMOIN_VANNE5,
IDC_TEMOIN_VANNE6,IDC_TEMOIN_VANNE7,IDC_TEMOIN_VANNE8, IDC_TEMOIN_EV1, IDC_TEMOIN_EV2, IDC_TEMOIN_POMPE };

ListOfInstrumentButtons::ListOfInstrumentButtons(CKalelView &h)
	: handle{ h }
{
}

ListOfInstrumentButtons::~ListOfInstrumentButtons()
{
}

void ListOfInstrumentButtons::Init()
{
	CString texboxText;
	size_t location = 0;

	for (auto &i : localState)
	{
		switch (i.first)
		{
		case ID_VALVE_1: location = 0; break;
		case ID_VALVE_2: location = 1; break;
		case ID_VALVE_3: location = 2; break;
		case ID_VALVE_4: location = 3; break;
		case ID_VALVE_5: location = 4; break;
		case ID_VALVE_6: location = 5; break;
		case ID_VALVE_7: location = 6; break;
		case ID_VALVE_8: location = 7; break;
		case ID_EV_1: location = 8; break;
		case ID_EV_2: location = 9; break;
		case ID_PUMP: location = 10; break;
		default: break;
		}
		if (i.second)
		{
			texboxText.Format(TEXT_OPENED);
		}
		else
		{
			texboxText.Format(TEXT_CLOSED);
		}
		handle.GetDlgItem(idcOpen[location])->EnableWindow(!i.second);
		handle.GetDlgItem(idcClose[location])->EnableWindow(i.second);
		handle.SetDlgItemText(idcTextBox[location], texboxText);
	}
}

void ListOfInstrumentButtons::StartCommand(int instrumentID, bool shouldBeActivated)
{
	int location = 0;

	switch (instrumentID)
	{
	case ID_VALVE_1: location = 0; break;
	case ID_VALVE_2: location = 1; break;
	case ID_VALVE_3: location = 2; break;
	case ID_VALVE_4: location = 3; break;
	case ID_VALVE_5: location = 4; break;
	case ID_VALVE_6: location = 5; break;
	case ID_VALVE_7: location = 6; break;
	case ID_VALVE_8: location = 7; break;
	case ID_EV_1: location = 8; break;
	case ID_EV_2: location = 9; break;
	case ID_PUMP: location = 10; break;
	default: break;
	}

	CString texboxText;
	cTextboxID = idcTextBox[location];

	if (shouldBeActivated) {
		cButtonID = idcOpen[location];
		cOppositeButtonID = idcClose[location];
		texboxText.Format(TEXT_ACTIVATING);
	}
	else {
		cButtonID = idcClose[location];
		cOppositeButtonID = idcOpen[location];
		texboxText.Format(TEXT_DEACTIVATING);
	}

	// Block the required buttons
	handle.GetDlgItem(cButtonID)->EnableWindow(FALSE);
	handle.GetDlgItem(cOppositeButtonID)->EnableWindow(FALSE);
	
	// Show that the action has started
	handle.SetDlgItemText(cTextboxID, texboxText);
}

void ListOfInstrumentButtons::EndCommand(const ControlInstrumentStateData &data)
{
	CString texboxText;
	size_t location = 0;

	switch (data.instrumentID)
	{
	case ID_VALVE_1: location = 0; break;
	case ID_VALVE_2: location = 1; break;
	case ID_VALVE_3: location = 2; break;
	case ID_VALVE_4: location = 3; break;
	case ID_VALVE_5: location = 4; break;
	case ID_VALVE_6: location = 5; break;
	case ID_VALVE_7: location = 6; break;
	case ID_VALVE_8: location = 7; break;
	case ID_EV_1: location = 8; break;
	case ID_EV_2: location = 9; break;
	case ID_PUMP: location = 10; break;
	default: break;
	}

	localState[data.instrumentID] = data.instrumentState;

	cTextboxID = idcTextBox[location];

	if (data.instrumentState) {
		cButtonID = idcOpen[location];
		cOppositeButtonID = idcClose[location];
		texboxText.Format(TEXT_ACTIVATED);
	}
	else {
		cButtonID = idcClose[location];
		cOppositeButtonID = idcOpen[location];
		texboxText.Format(TEXT_DEACTIVATED);
	}

	// Block the required buttons
	handle.GetDlgItem(cButtonID)->EnableWindow(FALSE);
	handle.GetDlgItem(cOppositeButtonID)->EnableWindow(TRUE);

	// Show that the action has started
	handle.SetDlgItemText(cTextboxID, texboxText);
}



void ListOfInstrumentButtons::Update(const ControlInstrumentState& state)
{
	CString texboxText;
	size_t location = 0;

	for (auto &i : state)
	{
		if (localState[i.first] != state.find(i.first)->second)
		{
			localState[i.first] = state.find(i.first)->second;

			// Do the GUI changes
			switch (i.first)
			{
			case ID_VALVE_1: location = 0; break;
			case ID_VALVE_2: location = 1; break;
			case ID_VALVE_3: location = 2; break;
			case ID_VALVE_4: location = 3; break;
			case ID_VALVE_5: location = 4; break;
			case ID_VALVE_6: location = 5; break;
			case ID_VALVE_7: location = 6; break;
			case ID_VALVE_8: location = 7; break;
			case ID_EV_1: location = 8; break;
			case ID_EV_2: location = 9; break;
			case ID_PUMP: location = 10; break;
			default: break;
			}

			if (i.second)
			{
				texboxText.Format(TEXT_OPENED);
			}
			else
			{
				texboxText.Format(TEXT_CLOSED);
			}
			handle.GetDlgItem(idcOpen[location])->EnableWindow(!i.second);
			handle.GetDlgItem(idcClose[location])->EnableWindow(i.second);
			handle.SetDlgItemText(idcTextBox[location], texboxText);
		}
	}
}
