#include "stdafx.h"
#include "ListOfInstrumentButtons.h"

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


ListOfInstrumentButtons::ListOfInstrumentButtons(int instrumentType, int instrumentNumber, bool shouldBeActivated)
{
	CString eM, tbM, ttbM;

	switch (instrumentType)
	{
	case INSTRUMENT_VALVE:
	{
		cTextboxID = idcValveTextBox[instrumentNumber - 1];
		if (shouldBeActivated) {
			cButtonID = idcValveOpen[instrumentNumber - 1];
			cOppositeButtonID = idcValveClose[instrumentNumber - 1];
			eM.Format(ERROR_OPENVALVE, instrumentNumber);
			ttbM.Format(TEXT_OPENING);
			tbM.Format(TEXT_OPENED);
		}
		else {
			cButtonID = idcValveClose[instrumentNumber - 1];
			cOppositeButtonID = idcValveOpen[instrumentNumber - 1];
			eM.Format(ERROR_CLOSEVALVE, instrumentNumber);
			ttbM.Format(TEXT_CLOSING);
			tbM.Format(TEXT_CLOSED);
		}
		break;
	}

	case INSTRUMENT_EV:
	{
		cTextboxID = idcEVTextBox[instrumentNumber - 1];

		if (shouldBeActivated) {
			cButtonID = idcEVOpen[instrumentNumber - 1];
			cOppositeButtonID = idcEVClose[instrumentNumber - 1];
			eM.Format(ERROR_OPENEV, instrumentNumber);
			ttbM.Format(TEXT_OPENING);
			tbM.Format(TEXT_OPENED);
		}
		else {
			cButtonID = idcEVClose[instrumentNumber - 1];
			cOppositeButtonID = idcEVOpen[instrumentNumber - 1];
			eM.Format(ERROR_OPENEV, instrumentNumber);
			ttbM.Format(TEXT_CLOSING);
			tbM.Format(TEXT_CLOSED);
		}
		break;
	}

	case INSTRUMENT_PUMP:
	{
		cTextboxID = idcPumpTextBox;

		if (shouldBeActivated) {
			cButtonID = idcPumpOpen;
			cOppositeButtonID = idcPumpClose;
			eM.Format(ERROR_ACTIVATEPUMP, instrumentNumber);
			ttbM.Format(TEXT_ACTIVATING);
			tbM.Format(TEXT_ACTIVATED);
		}
		else {
			cButtonID = idcPumpClose;
			cOppositeButtonID = idcPumpOpen;
			eM.Format(ERROR_DEACTIVATEPUMP, instrumentNumber);
			ttbM.Format(TEXT_DEACTIVATING);
			tbM.Format(TEXT_DEACTIVATED);
		}
		break;
	}
		
	default:
		break;
	}

	// Save strings in class
	errorMessage = eM;
	tempTextboxMessage = ttbM;
	textboxMessage = tbM;
}

ListOfInstrumentButtons::~ListOfInstrumentButtons()
{
}


int ListOfInstrumentButtons::GetButtonID()
{
	return cButtonID;
}

int ListOfInstrumentButtons::GetOppositeButtonID()
{
	return cOppositeButtonID;
}

int ListOfInstrumentButtons::GetTextboxID()
{
	return cTextboxID;
}
CString ListOfInstrumentButtons::GetTextboxMessage()
{
	return textboxMessage;
}

CString ListOfInstrumentButtons::GetTempTextboxMessage()
{
	return tempTextboxMessage;
}

CString ListOfInstrumentButtons::GetErrorMessage()
{
	return errorMessage;
}