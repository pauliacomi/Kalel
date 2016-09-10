#include "NI_USB_9211A.h"

#define DAQmxErrChk(functionCall) { if( DAQmxFailed(error=(functionCall)) ) { goto Error; } }

NI_USB_9211A::NI_USB_9211A(void)
{
	DevNI_USB_9211A = -1;
	TC0 = -25000; // voir si 25000 est déjà une erreur utilisé ou pas
	TC1 = -25000;
	TC2 = -25000;
	TC3 = -25000;
}

NI_USB_9211A::NI_USB_9211A(int dev)
{
	DevNI_USB_9211A = dev;
	LectureThermocouple(); // Et de ce fait, TC0,TC1,TC2 et TC3 sont initialisés
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


bool NI_USB_9211A::LectureThermocouple ()
{
	
	// Task parameters
    int32       error = 0;
    TaskHandle  taskHandle = 0;
    char        errBuff[2048]={'\0'};

    // Channel parameters
	char        chan[25];
    float64     min = 0;
    float64     max = 100.0;

    // Timing parameters
    char        source[] = "OnboardClock";
	uInt64      samplesPerChan = 1;
    float64     sampleRate = 4.0;

    // Data read parameters
    #define     bufferSize (uInt32)10
    float64     data[bufferSize];
	int32       pointsToRead = 1;
    int32       pointsRead;
    float64     timeout = 10.0;
	
	// Write channel string
	sprintf_s(chan,"Dev%d/ai0:3",DevNI_USB_9211A);

	// Create task
	DAQmxErrChk (DAQmxCreateTask ("ReadThermocouple", &taskHandle));
	DAQmxErrChk (DAQmxCreateAIThrmcplChan (taskHandle, chan, "", min, max, DAQmx_Val_DegC , DAQmx_Val_K_Type_TC, DAQmx_Val_BuiltIn , 0, ""));
	
	// Run task
	DAQmxErrChk (DAQmxStartTask (taskHandle));
	DAQmxErrChk (DAQmxReadAnalogF64 (taskHandle, pointsToRead, timeout, 0, data, bufferSize, &pointsRead, NULL));
	
	// Clear task
	if (taskHandle != 0)
    {
       DAQmxStopTask (taskHandle);
       DAQmxClearTask (taskHandle);
    }

	// Write data from returned array
	TC0 = data[0];
	TC1 = data[1];
	TC2 = data[2];
	TC3 = data[3];

	return true;

	
	// In case of error
Error:
    if (DAQmxFailed (error))
	{
		DAQmxGetExtendedErrorInfo (errBuff, 2048);
		errorKeep.assign(errBuff);
		TC0 = error;
		TC1 = error;
		TC2 = error;
		TC3 = error;
	}
	// Clear task to free memory
	if (taskHandle != 0)
	{
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}

	return false;
}

void NI_USB_9211A::GetError(std::string* err)
{
	*err = errorKeep;
}

//////////////////////////////////////////
//
// Individual functions to read data

bool NI_USB_9211A::LectureTousThermocouple(double* Valeur0,double* Valeur1,double* Valeur2,double* Valeur3)
{
	bool result = LectureThermocouple();
	*Valeur0 = TC0;
	*Valeur1 = TC1;
	*Valeur2 = TC2;
	*Valeur3 = TC3;

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_de_0_a_2(double* Valeur0,double* Valeur1,double* Valeur2)
{
	bool result = LectureThermocouple();
	*Valeur0 = TC0;
	*Valeur1 = TC1;
	*Valeur2 = TC2;

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_de_1_a_3(double* Valeur1,double* Valeur2,double* Valeur3)
{
	bool result = LectureThermocouple();
	*Valeur1 = TC1;
	*Valeur2 = TC2;
	*Valeur3 = TC3;

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_0_1_3(double* Valeur0,double* Valeur1,double* Valeur3)
{
	bool result = LectureThermocouple();
	*Valeur0 = TC0;
	*Valeur1 = TC1;
	*Valeur3 = TC3;

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_0_2_3(double* Valeur0,double* Valeur2,double* Valeur3)
{
	bool result = LectureThermocouple();
	*Valeur0 = TC0;
	*Valeur2 = TC2;
	*Valeur3 = TC3;

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_0_1(double* Valeur0,double* Valeur1)
{
	bool result = LectureThermocouple();
	*Valeur0 = TC0;
	*Valeur1 = TC1;

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_0_2(double* Valeur0,double* Valeur2)
{
	bool result = LectureThermocouple();
	*Valeur0 = TC0;
	*Valeur2 = TC2;

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_0_3(double* Valeur0,double* Valeur3)
{
	bool result = LectureThermocouple();
	*Valeur0 = TC0;
	*Valeur3 = TC3;

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_1_2(double* Valeur1,double* Valeur2)
{
	bool result = LectureThermocouple();
	*Valeur1 = TC1;
	*Valeur2 = TC2;

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_1_3(double* Valeur1,double* Valeur3)
{
	bool result = LectureThermocouple();
	*Valeur1 = TC1;
	*Valeur3 = TC3;

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_2_3(double* Valeur2,double* Valeur3)
{
	bool result = LectureThermocouple();
	*Valeur2 = TC2;
	*Valeur3 = TC3;

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_0(double* Valeur0)
{
	bool result = LectureThermocouple();
	*Valeur0 = TC0;

	if (result == true)
	{
		return true;
	}
	return false;
}


double NI_USB_9211A::LectureThermocouple_0()
{
	LectureThermocouple();
	return TC0;
}


bool NI_USB_9211A::LectureThermocouple_1(double* Valeur1)
{
	bool result = LectureThermocouple();
	*Valeur1 = TC1;

	if (result == true)
	{
		return true;
	}
	return false;
}

double NI_USB_9211A::LectureThermocouple_1()
{
	LectureThermocouple();
	return TC1;
}

bool NI_USB_9211A::LectureThermocouple_2(double* Valeur2)
{
	bool result = LectureThermocouple();
	*Valeur2 = TC2;

	if (result == true)
	{
		return true;
	}
	return false;
}

double NI_USB_9211A::LectureThermocouple_2()
{
	LectureThermocouple();
	return TC2;
}

bool NI_USB_9211A::LectureThermocouple_3(double* Valeur3)
{
	bool result = LectureThermocouple();
	*Valeur3 = TC3;

	if (result == true)
	{
		return true;
	}
	return false;
}

double NI_USB_9211A::LectureThermocouple_3()
{
	LectureThermocouple();
	return TC3;
}

