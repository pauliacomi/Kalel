
#include "StdAfx.h"

#include "NI_USB_9211A.h"

#define DAQmxErrChk(functionCall) { if( DAQmxFailed(error=(functionCall)) ) { goto Error; } }

NI_USB_9211A::NI_USB_9211A(void)
{
	DevNI_USB_9211A = -1;
	TC0 = -25000; // voir si 25000 est d�j� une erreur utilis� ou pas
	TC1 = -25000;
	TC2 = -25000;
	TC3 = -25000;
}

NI_USB_9211A::NI_USB_9211A(int dev)
{
	DevNI_USB_9211A = dev;
	LectureThermocouple(); // Et de ce fait, TC0,TC1,TC2 et TC3 sont initialis�s
}

NI_USB_9211A::~NI_USB_9211A(void)
{
}

int NI_USB_9211A::GetDevNI_USB_9211A()
{
	return DevNI_USB_9211A;
}

void NI_USB_9211A::SetDevNI_USB_9211A(int dev)
{
	DevNI_USB_9211A = dev;
}


void NI_USB_9211A::LectureThermocouple ()
{
	
   // Task parameters
    int32       error = 0;
    TaskHandle  taskHandle = 0;
    char        errBuff[2048]={'\0'};
//    int32       i;

    // Channel parameters
    //char        chan[] = "Dev1/ai0";
	char        chan[25];
    float64     min = 0;
    float64     max = 100.0;

    // Timing parameters
    char        source[] = "OnboardClock";
    //uInt64      samplesPerChan = 10;
	uInt64      samplesPerChan = 1;
    float64     sampleRate = 4.0;

    // Data read parameters
    #define     bufferSize (uInt32)10
    float64     data[bufferSize];
    //int32       pointsToRead = 10;
	int32       pointsToRead = 1;
    int32       pointsRead;
    float64     timeout = 10.0;
	




	sprintf(chan,"Dev%d/ai0:3",DevNI_USB_9211A);
	// Si on veut utiliser que la voie 1 et 3, faire du style: 
	// sprintf(chan,"Dev0/ai1,Dev0/ai3",DevNI_USB_9211A);

	DAQmxErrChk (DAQmxCreateTask ("", &taskHandle));
	DAQmxErrChk (DAQmxCreateAIThrmcplChan (taskHandle, chan, "", min, max, DAQmx_Val_DegC , DAQmx_Val_K_Type_TC, DAQmx_Val_BuiltIn , 0, ""));
	
	DAQmxErrChk (DAQmxStartTask (taskHandle));

	DAQmxErrChk (DAQmxReadAnalogF64 (taskHandle, pointsToRead, timeout, 0, data, bufferSize, &pointsRead, NULL));

	if (taskHandle != 0)
    {
       DAQmxStopTask (taskHandle);
       DAQmxClearTask (taskHandle);
	   //return -200;
	   //*retour = -200;
    }



	TC0=data[0];
	TC1=data[1];
	TC2=data[2];
	TC3=data[3];
	//return data[0];
	

Error:
    if (DAQmxFailed (error))
	{
	   DAQmxGetExtendedErrorInfo (errBuff, 2048);
		TC0=error;
		TC1=error;
		TC2=error;
		TC3=error;
	   //return error;
	   //*retour = error;
	}

    if (error)
	{
		TC0=error;
		TC1=error;
		TC2=error;
		TC3=error;
	}

	//return -1;
}

void NI_USB_9211A::LectureTousThermocouple(double* Valeur0,double* Valeur1,double* Valeur2,double* Valeur3)
{
	LectureThermocouple();
	*Valeur0 = TC0;
	*Valeur1 = TC1;
	*Valeur2 = TC2;
	*Valeur3 = TC3;
}

void NI_USB_9211A::LectureThermocouple_de_0_a_2(double* Valeur0,double* Valeur1,double* Valeur2)
{
	LectureThermocouple();
	*Valeur0 = TC0;
	*Valeur1 = TC1;
	*Valeur2 = TC2;
}

void NI_USB_9211A::LectureThermocouple_de_1_a_3(double* Valeur1,double* Valeur2,double* Valeur3)
{
	LectureThermocouple();
	*Valeur1 = TC1;
	*Valeur2 = TC2;
	*Valeur3 = TC3;
}

void NI_USB_9211A::LectureThermocouple_0_1_3(double* Valeur0,double* Valeur1,double* Valeur3)
{
	LectureThermocouple();
	*Valeur0 = TC0;
	*Valeur1 = TC1;
	*Valeur3 = TC3;
}

void NI_USB_9211A::LectureThermocouple_0_2_3(double* Valeur0,double* Valeur2,double* Valeur3)
{
	LectureThermocouple();
	*Valeur0 = TC0;
	*Valeur2 = TC2;
	*Valeur3 = TC3;
}

void NI_USB_9211A::LectureThermocouple_0_1(double* Valeur0,double* Valeur1)
{
	LectureThermocouple();
	*Valeur0 = TC0;
	*Valeur1 = TC1;
}

void NI_USB_9211A::LectureThermocouple_0_2(double* Valeur0,double* Valeur2)
{
	LectureThermocouple();
	*Valeur0 = TC0;
	*Valeur2 = TC2;
}

void NI_USB_9211A::LectureThermocouple_0_3(double* Valeur0,double* Valeur3)
{
	LectureThermocouple();
	*Valeur0 = TC0;
	*Valeur3 = TC3;
}

void NI_USB_9211A::LectureThermocouple_1_2(double* Valeur1,double* Valeur2)
{
	LectureThermocouple();
	*Valeur1 = TC1;
	*Valeur2 = TC2;
}

void NI_USB_9211A::LectureThermocouple_1_3(double* Valeur1,double* Valeur3)
{
	LectureThermocouple();
	*Valeur1 = TC1;
	*Valeur3 = TC3;
}

void NI_USB_9211A::LectureThermocouple_2_3(double* Valeur2,double* Valeur3)
{
	LectureThermocouple();
	*Valeur2 = TC2;
	*Valeur3 = TC3;
}

void NI_USB_9211A::LectureThermocouple_0(double* Valeur0)
{
	LectureThermocouple();
	*Valeur0 = TC0;
}


double NI_USB_9211A::LectureThermocouple_0()
{
	LectureThermocouple();
	return TC0;
}


void NI_USB_9211A::LectureThermocouple_1(double* Valeur1)
{
	LectureThermocouple();
	*Valeur1 = TC1;
}

double NI_USB_9211A::LectureThermocouple_1()
{
	LectureThermocouple();
	return TC1;
}

void NI_USB_9211A::LectureThermocouple_2(double* Valeur2)
{
	LectureThermocouple();
	*Valeur2 = TC2;
}

double NI_USB_9211A::LectureThermocouple_2()
{
	LectureThermocouple();
	return TC2;
}

void NI_USB_9211A::LectureThermocouple_3(double* Valeur3)
{
	LectureThermocouple();
	*Valeur3 = TC3;
}

double NI_USB_9211A::LectureThermocouple_3()
{
	LectureThermocouple();
	return TC3;
}

