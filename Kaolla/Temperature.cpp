#include "StdAfx.h"
#include "Temperature.h"



/*
CTemperature::CTemperature(void) : NI_USB_9211A(PortUSBTemperature())
{

}
*/

CTemperature::CTemperature(void) : NI_USB_9211A()
{

}

CTemperature::CTemperature(int dev) : NI_USB_9211A(dev)
{

}

CTemperature::~CTemperature(void)
{
}

void CTemperature::Temperature(double* Temperature_Calo, double* Temperature_Cage, double* Temperature_Piece)
{
	NI_USB_9211A::LectureThermocouple_de_0_a_2(Temperature_Calo,Temperature_Cage,Temperature_Piece);
	// Voir s'il faut mùettre des '*'
} 

void CTemperature::Temperature(float* Temperature_Calo, float* Temperature_Cage, float* Temperature_Piece)
{
	//NI_USB_9211A::LectureThermocouple_de_0_a_2((double*)Temperature_Calo,(double*)Temperature_Cage,(double*)Temperature_Piece);
	NI_USB_9211A::LectureThermocouple_de_0_a_2((double*)&Temperature_Calo,(double*)&Temperature_Cage,(double*)&Temperature_Piece);
	// Voir s'il faut mùettre des '*'
} 

double CTemperature::TemperatureCalo()
{	return NI_USB_9211A::LectureThermocouple_0();	}

double CTemperature::TemperatureCage()
{	return NI_USB_9211A::LectureThermocouple_1();	}

double CTemperature::TemperaturePiece()
{	return NI_USB_9211A::LectureThermocouple_2();	}



int CTemperature::PortUSBTemperature()
{
	return NI_USB_9211A::GetDevNI_USB_9211A();
}




