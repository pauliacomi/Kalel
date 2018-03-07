#include "../Forcelib.h"

#include "MachineSettings.h"

#include "../timeHelpers.h"
#include "../Resources/DefineInstruments.h"


MachineSettings::MachineSettings()
{
	ResetData();
}

MachineSettings::MachineSettings(const MachineSettings & p)
{
	ResetData();

	CaloName = p.CaloName;
	CaloEntete = p.CaloEntete;
	CheminFichierGeneral = p.CheminFichierGeneral;

	ActivationSecurite = p.ActivationSecurite;									// Used for activating the automatic security
	PressionSecuriteBassePression = p.PressionSecuriteBassePression;			// Considered the maximum pressure for the low range pressure transmitter
	PressionSecuriteHautePression = p.PressionSecuriteHautePression;			// Considered the maximum pressure for the high range pressure transmitter

	PressionLimiteVide = p.PressionLimiteVide;									// Used for determining the pressure considered "good vacuum"

	hasSonicNozzle = p.hasSonicNozzle;											// Used for user information (no calculations)
	VolumeRef = p.VolumeRef;													// Used for user information (no calculations)
	VolumeP6 = p.VolumeP6;														// Used for user information (no calculations)

	for (auto instrument : p.instruments) {
		AddInstrument(instrument.second, instrument.first);
	}
	for (auto reader : p.readers) {
		AddReader(reader.second, reader.first);
	}
	for (auto controller : p.controllers) {
		AddController(controller.second, controller.first);
	}
}


MachineSettings::~MachineSettings()
{
}


void MachineSettings::ResetData() {

	timeChanged = timeh::NowTime();

	CaloName = std::to_wstring(1);
	CaloEntete = std::to_wstring(1);
	CheminFichierGeneral = std::to_wstring(1);

	ActivationSecurite = true;						// Used for activating the automatic security
	PressionSecuriteBassePression = 1;				// Considered the maximum pressure for the low range pressure transmitter
	PressionSecuriteHautePression = 5;				// Considered the maximum pressure for the high range pressure transmitter

	PressionLimiteVide = 0.1f;						// Used for determining the pressure considered "good vacuum"
	
	hasSonicNozzle = false;							// Used for user information (no calculations)
	VolumeRef = 1;									// Used for user information (no calculations)
	VolumeP6 = 1;									// Used for user information (no calculations)

	instruments.empty();
	readers.empty();
	controllers.empty();
}

MachineSettings & MachineSettings::operator=(const MachineSettings * p)
{
	if (this != p) {  // make sure not same object

		ResetData();

		CaloName = p->CaloName;
		CaloEntete = p->CaloEntete;
		CheminFichierGeneral = p->CheminFichierGeneral;

		ActivationSecurite = p->ActivationSecurite;									// Used for activating the automatic security
		PressionSecuriteBassePression = p->PressionSecuriteBassePression;			// Considered the maximum pressure for the low range pressure transmitter
		PressionSecuriteHautePression = p->PressionSecuriteHautePression;			// Considered the maximum pressure for the high range pressure transmitter

		PressionLimiteVide = p->PressionLimiteVide;									// Used for determining the pressure considered "good vacuum"

		hasSonicNozzle = p->hasSonicNozzle;											// Used for user information (no calculations)
		VolumeRef = p->VolumeRef;													// Used for user information (no calculations)
		VolumeP6 = p->VolumeP6;														// Used for user information (no calculations)

		for (auto instrument : p->instruments) {
			AddInstrument(instrument.second, instrument.first);
		}
		for (auto reader : p->readers) {
			AddReader(reader.second, reader.first);
		}
		for (auto controller : p->controllers) {
			AddController(controller.second, controller.first);
		}

	}
	return *this;    // Return ref for multiple assignment
}


void MachineSettings::AddInstrument(Instrument i, unsigned int position /*=0*/)
{
	if (position == 0)
	{
		unsigned int placer = instruments.size() + 1;
		instruments.emplace(std::make_pair(placer, i));
		return;
	}
	if (instruments.find(position) != instruments.end()) {
		instruments[position].type = i.type;
		instruments[position].port = i.port;
	}
	else
	{
		if (i.type != 0)
		{
			instruments.emplace(std::make_pair(position, i));
		}
	}
}

void MachineSettings::AddReader(Reader r, unsigned int position)
{
	if (position == 0)
	{
		unsigned int placer = readers.size() + 1;
		readers.emplace(std::make_pair(placer, r));
		return;
	}
	if (readers.find(position) != readers.end()) {
		readers[position].type			= r.type		;
		readers[position].identifier	= r.identifier	;
		readers[position].sensitivity	= r.sensitivity	;
		readers[position].channel		= r.channel		;
		readers[position].instrument	= r.instrument	;
	}
	else
	{
		if (r.type != 0 && r.identifier != 0)
		{
			readers.emplace(std::make_pair(position, r));
		}
	}
}

void MachineSettings::AddController(Controller r, unsigned int position)
{
	if (position == 0)
	{
		unsigned int placer = controllers.size() + 1;
		controllers.emplace(std::make_pair(placer, r));
		return;
	}
	if (controllers.find(position) != controllers.end()) {
		controllers[position].type			= r.type		;
		controllers[position].identifier	= r.identifier	;
		controllers[position].sensitivity	= r.sensitivity	;
		controllers[position].channel		= r.channel		;
		controllers[position].instrument	= r.instrument	;
	}
	else
	{
		if (r.type != 0 && r.identifier != 0)
		{
			controllers.emplace(std::make_pair(position, r));
		}
	}
}

