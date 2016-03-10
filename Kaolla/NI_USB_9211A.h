#pragma once
#ifndef NI_USB_9211A_H
#define NI_USB_9211A_H

#include "NIDAQmx.h"
#include <math.h>


class NI_USB_9211A
{
public:
	NI_USB_9211A(void);
	NI_USB_9211A(int dev);
	~NI_USB_9211A(void);
	
	int DevNI_USB_9211A;

	int GetDevNI_USB_9211A();
	void SetDevNI_USB_9211A(int dev);

	void LectureThermocouple();
	
	void LectureTousThermocouple(double* Valeur0,double* Valeur1,double* Valeur2,double* Valeur3);
	
	void LectureThermocouple_de_0_a_2(double* Valeur0,double* Valeur1,double* Valeur2);
	void LectureThermocouple_de_1_a_3(double* Valeur1,double* Valeur2,double* Valeur3);
	void LectureThermocouple_0_1_3(double* Valeur0,double* Valeur1,double* Valeur3);
	void LectureThermocouple_0_2_3(double* Valeur0,double* Valeur2,double* Valeur3);
	
	void LectureThermocouple_0_1(double* Valeur0,double* Valeur1);
	void LectureThermocouple_0_2(double* Valeur0,double* Valeur2);
	void LectureThermocouple_0_3(double* Valeur0,double* Valeur3);
	void LectureThermocouple_1_2(double* Valeur1,double* Valeur2);
	void LectureThermocouple_1_3(double* Valeur1,double* Valeur3);
	void LectureThermocouple_2_3(double* Valeur2,double* Valeur3);
	
	void LectureThermocouple_0(double* Valeur0);
	double LectureThermocouple_0();
	void LectureThermocouple_1(double* Valeur1);
	double LectureThermocouple_1();
	void LectureThermocouple_2(double* Valeur2);
	double LectureThermocouple_2();
	void LectureThermocouple_3(double* Valeur3);
	double LectureThermocouple_3();
	
	double TC0,TC1,TC2,TC3;

};


#endif




