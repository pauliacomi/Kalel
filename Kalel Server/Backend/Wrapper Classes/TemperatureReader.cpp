#include "TemperatureReader.h"

// Constructors & Destructor
TemperatureReader::TemperatureReader(int port) : NI_USB_9211A(port) {}
TemperatureReader::~TemperatureReader(void) {}

// Functions
bool TemperatureReader::Read(double* Temperature_Calo, double* Temperature_Cage, double* Temperature_Piece)
{	return NI_USB_9211A::LectureThermocouple_de_0_a_2(Temperature_Calo,Temperature_Cage,Temperature_Piece);		} 

bool TemperatureReader::ReadCalo(double* Temperature_Calo)
{	return NI_USB_9211A::LectureThermocouple_0(Temperature_Calo);	}

bool TemperatureReader::ReadCage(double* Temperature_Cage)
{	return NI_USB_9211A::LectureThermocouple_1(Temperature_Cage);	}

bool TemperatureReader::ReadRoom(double* Temperature_Piece)
{	return NI_USB_9211A::LectureThermocouple_2(Temperature_Piece);	}

int TemperatureReader::GetReadPort()
{	return NI_USB_9211A::GetDevNI_USB_9211A();		}

void TemperatureReader::SetReadPort(int port) {		
	// Lock for the remainder of function
	std::lock_guard<std::mutex> lk(ctrlmutex);
	return NI_USB_9211A::SetDevNI_USB_9211A(port);	
}
