#include "Temperature.h"

// Constructors & Destructor
TemperatureInstruments::TemperatureInstruments(int port) : NI_USB_9211A(port) {}
TemperatureInstruments::~TemperatureInstruments(void) {}

// Functions
bool TemperatureInstruments::Read(double* Temperature_Calo, double* Temperature_Cage, double* Temperature_Piece)
{	return NI_USB_9211A::LectureThermocouple_de_0_a_2(Temperature_Calo,Temperature_Cage,Temperature_Piece);		} 

bool TemperatureInstruments::ReadCalo(double* Temperature_Calo)
{	return NI_USB_9211A::LectureThermocouple_0(Temperature_Calo);	}

bool TemperatureInstruments::ReadCage(double* Temperature_Cage)
{	return NI_USB_9211A::LectureThermocouple_1(Temperature_Cage);	}

bool TemperatureInstruments::ReadRoom(double* Temperature_Piece)
{	return NI_USB_9211A::LectureThermocouple_2(Temperature_Piece);	}

int TemperatureInstruments::GetReadPort()
{	return NI_USB_9211A::GetDevNI_USB_9211A();		}

void TemperatureInstruments::SetReadPort(int port)
{	return NI_USB_9211A::SetDevNI_USB_9211A(port);	}
