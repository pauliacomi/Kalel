#include "stdafx.h"
#include "ListOfInstrumentButtons.h"

#include "resource.h"
#include "Define_Instrument.h"


const int idcValveOpen[] = { IDC_OUVRIR1,IDC_OUVRIR2,IDC_OUVRIR3,IDC_OUVRIR4, IDC_OUVRIR5,IDC_OUVRIR6,IDC_OUVRIR7,IDC_OUVRIR8 };
const int idcValveClose[] = { IDC_FERMER1,IDC_FERMER2,IDC_FERMER3,IDC_FERMER4, IDC_FERMER5,IDC_FERMER6,IDC_FERMER7,IDC_FERMER8 };
const int idcValveTextBox[] = { IDC_TEMOIN_VANNE1,IDC_TEMOIN_VANNE2,IDC_TEMOIN_VANNE3,IDC_TEMOIN_VANNE4, IDC_TEMOIN_VANNE5,IDC_TEMOIN_VANNE6,IDC_TEMOIN_VANNE7,IDC_TEMOIN_VANNE8 };

const int idcEVOpen[] = { IDC_ACTIVER_EV1, IDC_ACTIVER_EV2 };
const int idcEVClose[] = { IDC_DESACTIVER_EV1, IDC_DESACTIVER_EV2 };
const int idcEVTextBox[] = { IDC_TEMOIN_EV1, IDC_TEMOIN_EV2 };

const int idcPumpOpen = IDC_ACTIVER_POMPE;
const int idcPumpClose = IDC_DESACTIVER_POMPE;
const int idcPumpTextBox = IDC_TEMOIN_POMPE;




ListOfInstrumentButtons::ListOfInstrumentButtons()
{
}

ListOfInstrumentButtons::~ListOfInstrumentButtons()
{
}

int ListOfInstrumentButtons::GetButtonID(int instrumentType, int instrumentNumber, bool shouldBeActivated)
{
	switch (instrumentType)
	{
	case INSTRUMENT_VALVE:
		if (shouldBeActivated)
			return idcValveOpen[instrumentNumber-1];
		else
			return idcValveClose[instrumentNumber - 1];
	case INSTRUMENT_EV:
		if (shouldBeActivated)
			return idcEVOpen[instrumentNumber - 1];
		else
			return idcEVClose[instrumentNumber - 1];
	case INSTRUMENT_PUMP:
		if (shouldBeActivated)
			return idcPumpOpen;
		else
			return idcPumpClose;
	default:
		break;
	}

	return 0;
}

int ListOfInstrumentButtons::GetTextboxID(int instrumentType, int instrumentNumber)
{
	switch (instrumentType)
	{
	case INSTRUMENT_VALVE:
		return idcValveTextBox[instrumentNumber - 1];
	case INSTRUMENT_EV:
		return idcEVTextBox[instrumentNumber - 1];
	case INSTRUMENT_PUMP:
		return idcPumpTextBox;
	default:
		break;
	}

	return 0;
}