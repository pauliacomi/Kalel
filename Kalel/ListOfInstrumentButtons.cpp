#include "stdafx.h"
#include "ListOfInstrumentButtons.h"

#include "KalelView.h"

#include "resource.h"											// For the MFC button id's
#include "../Kalel Shared/Resources/DefineInstruments.h"		// For the instrument definitions
#include "../Kalel Shared/Resources/StringTable.h"				// For the error message definitions


const int idcValveOpen[] = { IDC_OUVRIR1,IDC_OUVRIR2,IDC_OUVRIR3,IDC_OUVRIR4, IDC_OUVRIR5,IDC_OUVRIR6,IDC_OUVRIR7,IDC_OUVRIR8 };
const int idcValveClose[] = { IDC_FERMER1,IDC_FERMER2,IDC_FERMER3,IDC_FERMER4, IDC_FERMER5,IDC_FERMER6,IDC_FERMER7,IDC_FERMER8 };
const int idcValveTextBox[] = { IDC_TEMOIN_VANNE1,IDC_TEMOIN_VANNE2,IDC_TEMOIN_VANNE3,IDC_TEMOIN_VANNE4, IDC_TEMOIN_VANNE5,IDC_TEMOIN_VANNE6,IDC_TEMOIN_VANNE7,IDC_TEMOIN_VANNE8 };

const int idcEVOpen[] = { IDC_ACTIVER_EV1, IDC_ACTIVER_EV2 };
const int idcEVClose[] = { IDC_DESACTIVER_EV1, IDC_DESACTIVER_EV2 };
const int idcEVTextBox[] = { IDC_TEMOIN_EV1, IDC_TEMOIN_EV2 };

const int idcPumpOpen = IDC_ACTIVER_POMPE;
const int idcPumpClose = IDC_DESACTIVER_POMPE;
const int idcPumpTextBox = IDC_TEMOIN_POMPE;



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

	for (auto i = 0; i < localState.valves.size(); i++)
	{
		if (localState.valves[i])
		{
			texboxText.Format(TEXT_OPENED);
		}
		else
		{
			texboxText.Format(TEXT_CLOSED);
		}
		handle.GetDlgItem(idcValveOpen[i])->EnableWindow(localState.valves[i]);
		handle.GetDlgItem(idcValveClose[i])->EnableWindow(!localState.valves[i]);
		handle.SetDlgItemText(idcValveTextBox[i], texboxText);
	}

	for (auto i = 0; i < localState.EVs.size(); i++)
	{
		// Do the GUI changes
		if (localState.EVs[i])
		{
			texboxText.Format(TEXT_OPENED);
		}
		else
		{
			texboxText.Format(TEXT_CLOSED);
		}
		handle.GetDlgItem(idcEVOpen[i])->EnableWindow(localState.EVs[i]);
		handle.GetDlgItem(idcEVClose[i])->EnableWindow(!localState.EVs[i]);
		handle.SetDlgItemText(idcEVTextBox[i], texboxText);
	}


	for (auto i = 0; i < localState.pumps.size(); i++)
	{
		// Do the GUI changes
		if (localState.pumps[i])
		{
			texboxText.Format(TEXT_ACTIVATED);
		}
		else
		{
			texboxText.Format(TEXT_DEACTIVATED);
		}
		handle.GetDlgItem(idcPumpOpen)->EnableWindow(localState.pumps[i]);
		handle.GetDlgItem(idcPumpClose)->EnableWindow(!localState.pumps[i]);
		handle.SetDlgItemText(idcPumpTextBox, texboxText);
	}
}

void ListOfInstrumentButtons::StartCommand(int instrumentType, int instrumentNumber, bool shouldBeActivated)
{
	CString texboxText;

	switch (instrumentType)
	{
	case INSTRUMENT_VALVE:
	{
		cTextboxID = idcValveTextBox[instrumentNumber - 1];

		if (shouldBeActivated) {
			cButtonID = idcValveOpen[instrumentNumber - 1];
			cOppositeButtonID = idcValveClose[instrumentNumber - 1];
			texboxText.Format(TEXT_OPENING);
		}
		else {
			cButtonID = idcValveClose[instrumentNumber - 1];
			cOppositeButtonID = idcValveOpen[instrumentNumber - 1];
			texboxText.Format(TEXT_CLOSING);
		}
		break;
	}

	case INSTRUMENT_EV:
	{
		cTextboxID = idcEVTextBox[instrumentNumber - 1];

		if (shouldBeActivated) {
			cButtonID = idcEVOpen[instrumentNumber - 1];
			cOppositeButtonID = idcEVClose[instrumentNumber - 1];
			texboxText.Format(TEXT_OPENING);
		}
		else {
			cButtonID = idcEVClose[instrumentNumber - 1];
			cOppositeButtonID = idcEVOpen[instrumentNumber - 1];
			texboxText.Format(TEXT_CLOSING);
		}
		break;
	}

	case INSTRUMENT_PUMP:
	{
		cTextboxID = idcPumpTextBox;

		if (shouldBeActivated) {
			cButtonID = idcPumpOpen;
			cOppositeButtonID = idcPumpClose;
			texboxText.Format(TEXT_ACTIVATING);
		}
		else {
			cButtonID = idcPumpClose;
			cOppositeButtonID = idcPumpOpen;
			texboxText.Format(TEXT_DEACTIVATING);
		}
		break;
	}

	default:
		break;
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

	switch (data.instrumentType)
	{
	case INSTRUMENT_VALVE:
	{
		localState.valves[data.instrumentNumber - 1] = data.instrumentState;

		cTextboxID = idcValveTextBox[data.instrumentNumber - 1];

		if (data.instrumentState) {
			cButtonID = idcValveOpen[data.instrumentNumber - 1];
			cOppositeButtonID = idcValveClose[data.instrumentNumber - 1];
			texboxText.Format(TEXT_OPENED);
		}
		else {
			cButtonID = idcValveClose[data.instrumentNumber - 1];
			cOppositeButtonID = idcValveOpen[data.instrumentNumber - 1];
			texboxText.Format(TEXT_CLOSED);
		}
		break;
	}

	case INSTRUMENT_EV:
	{
		localState.EVs[data.instrumentNumber - 1] = data.instrumentState;

		cTextboxID = idcEVTextBox[data.instrumentNumber - 1];

		if (data.instrumentState) {
			cButtonID = idcEVOpen[data.instrumentNumber - 1];
			cOppositeButtonID = idcEVClose[data.instrumentNumber - 1];
			texboxText.Format(TEXT_OPENED);
		}
		else {
			cButtonID = idcEVClose[data.instrumentNumber - 1];
			cOppositeButtonID = idcEVOpen[data.instrumentNumber - 1];
			texboxText.Format(TEXT_CLOSED);
		}
		break;
	}

	case INSTRUMENT_PUMP:
	{
		localState.pumps[data.instrumentNumber - 1] = data.instrumentState;

		cTextboxID = idcPumpTextBox;

		if (data.instrumentState) {
			cButtonID = idcPumpOpen;
			cOppositeButtonID = idcPumpClose;
			texboxText.Format(TEXT_ACTIVATED);
		}
		else {
			cButtonID = idcPumpClose;
			cOppositeButtonID = idcPumpOpen;
			texboxText.Format(TEXT_DEACTIVATED);
		}
		break;
	}

	default:
		break;
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

	for (auto i = 0; i < state.valves.size(); i++)
	{
		if(localState.valves[i] != state.valves[i])
		{
			// Save the state
			localState.valves[i] = state.valves[i];

			// Do the GUI changes
			if (state.valves[i])
			{
				texboxText.Format(TEXT_OPENED);
			}
			else
			{
				texboxText.Format(TEXT_CLOSED);
			}
			handle.GetDlgItem(idcValveOpen[i])->EnableWindow(state.valves[i]);
			handle.GetDlgItem(idcValveClose[i])->EnableWindow(!state.valves[i]);
			handle.SetDlgItemText(idcValveTextBox[i], texboxText);
		}
	}

	for (auto i = 0; i < state.EVs.size(); i++)
	{
		if (localState.EVs[i] != state.EVs[i])
		{
			// Save the state
			localState.EVs[i] = state.EVs[i];

			// Do the GUI changes
			if (state.EVs[i])
			{
				texboxText.Format(TEXT_OPENED);
			}
			else
			{
				texboxText.Format(TEXT_CLOSED);
			}
			handle.GetDlgItem(idcEVOpen[i])->EnableWindow(state.EVs[i]);
			handle.GetDlgItem(idcEVClose[i])->EnableWindow(!state.EVs[i]);
			handle.SetDlgItemText(idcEVTextBox[i], texboxText);
		}
	}

	for (auto i = 0; i < state.pumps.size(); i++)
	{
		if (localState.pumps[i] != state.pumps[i])
		{
			// Save the state
			localState.pumps[i] = state.pumps[i];

			// Do the GUI changes
			if (state.pumps[i])
			{
				texboxText.Format(TEXT_ACTIVATED);
			}
			else
			{
				texboxText.Format(TEXT_DEACTIVATED);
			}
			handle.GetDlgItem(idcPumpOpen)->EnableWindow(state.pumps[i]);
			handle.GetDlgItem(idcPumpClose)->EnableWindow(!state.pumps[i]);
			handle.SetDlgItemText(idcPumpTextBox, texboxText);
		}
	}
}
