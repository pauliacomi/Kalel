#pragma once
#ifndef NI_USB_9211A_H
#define NI_USB_9211A_H

#include "NIDAQmx.h"
#include <math.h>
#include <string>


class NI_USB_9211A
{
public:
	NI_USB_9211A(void);
	NI_USB_9211A(int dev);
	~NI_USB_9211A(void);


private:
	int DevNI_USB_9211A;						// USB port used
	double TC0, TC1, TC2, TC3;					// Store the data from the thermocouples
	bool LectureThermocouple();					// Reads all data from device
	std::string errorKeep;

public:

	int GetDevNI_USB_9211A();					// Get USB port
	void SetDevNI_USB_9211A(int dev);			// Set USB port

	
	bool LectureTousThermocouple(double* Valeur0,double* Valeur1,double* Valeur2,double* Valeur3);
	
	bool LectureThermocouple_de_0_a_2(double* Valeur0,double* Valeur1,double* Valeur2);
	bool LectureThermocouple_de_1_a_3(double* Valeur1,double* Valeur2,double* Valeur3);
	bool LectureThermocouple_0_1_3(double* Valeur0,double* Valeur1,double* Valeur3);
	bool LectureThermocouple_0_2_3(double* Valeur0,double* Valeur2,double* Valeur3);
	
	bool LectureThermocouple_0_1(double* Valeur0,double* Valeur1);
	bool LectureThermocouple_0_2(double* Valeur0,double* Valeur2);
	bool LectureThermocouple_0_3(double* Valeur0,double* Valeur3);
	bool LectureThermocouple_1_2(double* Valeur1,double* Valeur2);
	bool LectureThermocouple_1_3(double* Valeur1,double* Valeur3);
	bool LectureThermocouple_2_3(double* Valeur2,double* Valeur3);
	
	bool LectureThermocouple_0(double* Valeur0);
	double LectureThermocouple_0();
	bool LectureThermocouple_1(double* Valeur1);
	double LectureThermocouple_1();
	bool LectureThermocouple_2(double* Valeur2);
	double LectureThermocouple_2();
	bool LectureThermocouple_3(double* Valeur3);
	double LectureThermocouple_3();

	// Error retrieval

	void GetError(std::string* err);
};


#endif




