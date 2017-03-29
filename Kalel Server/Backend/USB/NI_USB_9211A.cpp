#include "NI_USB_9211A.h"

#define DAQmxErrChk(functionCall) { if( DAQmxFailed(error=(functionCall)) ) { goto Error; } }

NI_USB_9211A::NI_USB_9211A(void)
{
	DevNI_USB_9211A = -1;
}

NI_USB_9211A::NI_USB_9211A(int dev)
{
	DevNI_USB_9211A = dev;
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


bool NI_USB_9211A::ReadThermocouple(float64 data[bufferSize])
{	
	// Task variables
    TaskHandle  taskHandle		= nullptr;
    int32       error			= 0;
	char        errBuff[2048]	= { '\0' };				// C string for error
	int32       pointsRead		= 0;

	// Write channel string
	sprintf_s(chan, "Dev%d/ai%d:%d", DevNI_USB_9211A, analog_start, analog_end);

	// Create thrermocouple Task and Channel
	DAQmxErrChk (DAQmxCreateTask ("ReadThermocouple", &taskHandle));
	DAQmxErrChk (DAQmxCreateAIThrmcplChan(taskHandle, chan, "", min, max, DAQmx_Val_DegC, DAQmx_Val_K_Type_TC, DAQmx_Val_BuiltIn, 0, ""));
	
	// Start Task (configure port)
	DAQmxErrChk (DAQmxStartTask (taskHandle));
	// Run task for single reading of value
	DAQmxErrChk (DAQmxReadAnalogF64 (taskHandle, pointsToRead, timeout, 0, data, bufferSize, &pointsRead, NULL));

	// In case of error
Error:
    if (DAQmxFailed (error))
	{
		DAQmxGetExtendedErrorInfo (errBuff, 2048);
		errorKeep.assign(errBuff);
		for (size_t i = 0; i < bufferSize; i++)
		{
			data[i] = error;
		}
	}

	// Clear task to free memory
	if (taskHandle != 0)
	{
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}

	// Return success
	if (error == 0)
		return true;
	return false;
}

void NI_USB_9211A::GetError(std::string* err)
{
	*err = errorKeep;
}

//////////////////////////////////////////
//
// Individual functions to read data

bool NI_USB_9211A::ReadAllThermocouples(double* Valeur0,double* Valeur1,double* Valeur2,double* Valeur3)
{
	float64     data[bufferSize];
	bool result = ReadThermocouple(data);

	*Valeur0 = data[0];
	*Valeur1 = data[1];
	*Valeur2 = data[2];
	*Valeur3 = data[3];

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_de_0_a_2(double* Valeur0,double* Valeur1,double* Valeur2)
{
	float64     data[bufferSize];
	bool result = ReadThermocouple(data);
	*Valeur0 = data[0];
	*Valeur1 = data[1];
	*Valeur2 = data[2];

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_de_1_a_3(double* Valeur1,double* Valeur2,double* Valeur3)
{
	float64     data[bufferSize];
	bool result = ReadThermocouple(data);
	*Valeur1 = data[1];
	*Valeur2 = data[2];
	*Valeur3 = data[3];

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_0_1_3(double* Valeur0,double* Valeur1,double* Valeur3)
{
	float64     data[bufferSize];
	bool result = ReadThermocouple(data);
	*Valeur0 = data[0];
	*Valeur1 = data[1];
	*Valeur3 = data[3];

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_0_2_3(double* Valeur0,double* Valeur2,double* Valeur3)
{
	float64     data[bufferSize];
	bool result = ReadThermocouple(data);
	*Valeur0 = data[0];
	*Valeur2 = data[2];
	*Valeur3 = data[3];

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_0_1(double* Valeur0,double* Valeur1)
{
	float64     data[bufferSize];
	bool result = ReadThermocouple(data);
	*Valeur0 = data[0];
	*Valeur1 = data[1];

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_0_2(double* Valeur0,double* Valeur2)
{
	float64     data[bufferSize];
	bool result = ReadThermocouple(data);
	*Valeur0 = data[0];
	*Valeur2 = data[2];

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_0_3(double* Valeur0,double* Valeur3)
{
	float64     data[bufferSize];
	bool result = ReadThermocouple(data);
	*Valeur0 = data[0];
	*Valeur3 = data[3];

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_1_2(double* Valeur1,double* Valeur2)
{
	float64     data[bufferSize];
	bool result = ReadThermocouple(data);
	*Valeur1 = data[1];
	*Valeur2 = data[2];

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_1_3(double* Valeur1,double* Valeur3)
{
	float64     data[bufferSize];
	bool result = ReadThermocouple(data);
	*Valeur1 = data[1];
	*Valeur3 = data[3];

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_2_3(double* Valeur2,double* Valeur3)
{
	float64     data[bufferSize];
	bool result = ReadThermocouple(data);
	*Valeur2 = data[2];
	*Valeur3 = data[3];

	if (result == true)
	{
		return true;
	}
	return false;
}

bool NI_USB_9211A::LectureThermocouple_0(double* Valeur0)
{
	float64     data[bufferSize];
	bool result = ReadThermocouple(data);
	*Valeur0 = data[0];

	if (result == true)
	{
		return true;
	}
	return false;
}


bool NI_USB_9211A::LectureThermocouple_1(double* Valeur1)
{
	float64     data[bufferSize];
	bool result = ReadThermocouple(data);
	*Valeur1 = data[1];

	if (result == true)
	{
		return true;
	}
	return false;
}


bool NI_USB_9211A::LectureThermocouple_2(double* Valeur2)
{
	float64     data[bufferSize];
	bool result = ReadThermocouple(data);
	*Valeur2 = data[2];

	if (result == true)
	{
		return true;
	}
	return false;
}


bool NI_USB_9211A::LectureThermocouple_3(double* Valeur3)
{
	float64     data[bufferSize];
	bool result = ReadThermocouple(data);
	*Valeur3 = data[3];

	if (result == true)
	{
		return true;
	}
	return false;
}
