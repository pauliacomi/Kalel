#include "StdAfx.h"
#include "Temperature.h"



// Constructors & Destructor
CTemperature::CTemperature(void) : NI_USB_9211A()
{
	NI_USB_9211A::SetDevNI_USB_9211A(GetPortTemperatures());
}


CTemperature::~CTemperature(void)
{
}


bool CTemperature::Read(double* Temperature_Calo, double* Temperature_Cage, double* Temperature_Piece)
{
	NI_USB_9211A::LectureThermocouple_de_0_a_2(Temperature_Calo,Temperature_Cage,Temperature_Piece);
	return true;
} 

bool CTemperature::Read(float* Temperature_Calo, float* Temperature_Cage, float* Temperature_Piece)
{
	NI_USB_9211A::LectureThermocouple_de_0_a_2((double*)&Temperature_Calo,(double*)&Temperature_Cage,(double*)&Temperature_Piece);
	return true;
} 

double CTemperature::ReadCalo()
{	return NI_USB_9211A::LectureThermocouple_0();	}

double CTemperature::ReadCage()
{	return NI_USB_9211A::LectureThermocouple_1();	}

double CTemperature::ReadPiece()
{	return NI_USB_9211A::LectureThermocouple_2();	}


int CTemperature::PortUSB()
{
	return NI_USB_9211A::GetDevNI_USB_9211A();
}




