#include "Temperature.h"


// Constructors & Destructor
TemperatureInstruments::TemperatureInstruments(void) : NI_USB_9211A()
{
	NI_USB_9211A::SetDevNI_USB_9211A(GetPortTemperatures());
}


TemperatureInstruments::~TemperatureInstruments(void)
{
}


bool TemperatureInstruments::Read(double* Temperature_Calo, double* Temperature_Cage, double* Temperature_Piece)
{
	return NI_USB_9211A::LectureThermocouple_de_0_a_2(Temperature_Calo,Temperature_Cage,Temperature_Piece);
} 

bool TemperatureInstruments::Read(float* Temperature_Calo, float* Temperature_Cage, float* Temperature_Piece)
{
	return NI_USB_9211A::LectureThermocouple_de_0_a_2((double*)&Temperature_Calo,(double*)&Temperature_Cage,(double*)&Temperature_Piece);
} 

double TemperatureInstruments::ReadCalo()
{	return NI_USB_9211A::LectureThermocouple_0();	}

double TemperatureInstruments::ReadCage()
{	return NI_USB_9211A::LectureThermocouple_1();	}

double TemperatureInstruments::ReadPiece()
{	return NI_USB_9211A::LectureThermocouple_2();	}


int TemperatureInstruments::PortUSB()
{
	return NI_USB_9211A::GetDevNI_USB_9211A();
}

