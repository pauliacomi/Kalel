#include "NI_USB_6008.h"

#define DAQmxErrChk(functionCall) { if( DAQmxFailed(error=(functionCall)) ) { goto Error; } }
#define open 1
#define closed  0

NI_USB_6008::NI_USB_6008(void)
{
	DevNI_USB_6008 = -1;
}

NI_USB_6008::NI_USB_6008(int dev)
{
	DevNI_USB_6008 = dev;
	ReadPort0();
	ReadPort1();
}

NI_USB_6008::~NI_USB_6008(void)
{
}

// Get and set functions for the USB port

int NI_USB_6008::GetDevNI_USB_6008()
{
	return DevNI_USB_6008;
}

void NI_USB_6008::SetDevNI_USB_6008(int dev)
{
	DevNI_USB_6008 = dev;
}


bool NI_USB_6008::ReadPort0()
{
	// Specific channel and line parameters
	char        chan[50]		= { '\0' };
	int			port			= 0;
	int			line_start		= 0;
	int			line_end		= 7;
	
	// Write channel string
	sprintf_s(chan,"Dev%d/port%d/line%d:%d",DevNI_USB_6008, port, line_start, line_end);

	// Read state
	if (ReadDigital(chan, etatPort0)) {
		return true;
	}
	return false;
}

bool NI_USB_6008::ReadPort1()
{
	// Specific channel and line parameters
	char        chan[50]		= { '\0' };
	int			port			= 1;
	int			line_start		= 0;
	int			line_end		= 3;

	// Write channel string
	sprintf_s(chan,"Dev%d/port%d/line%d:%d",DevNI_USB_6008, port, line_start, line_end);
	
	// Read state
	if (ReadDigital(chan, etatPort1)) {
		return true;
	}
	return false;
}

bool NI_USB_6008::WritePort0()
{
	// Specific channel and line parameters
	char        chan[50]		= { '\0' };
	int			port			= 0;
	int			line_start		= 0;
	int			line_end		= 7;

	// Write channel string
	sprintf_s(chan, "Dev%d/port%d/line%d:%d", DevNI_USB_6008, port, line_start, line_end);

	// Write action
	return WriteDigital(chan, etatPort0);
}

bool NI_USB_6008::WritePort1()
{
	// Specific channel and line parameters
	char        chan[50]		= { '\0' };
	int			port			= 1;
	int			line_start		= 0;
	int			line_end		= 3;

	// Write channel string
	sprintf_s(chan, "Dev%d/port%d/line%d:%d", DevNI_USB_6008, port, line_start, line_end);

	// Write action
	return WriteDigital(chan, etatPort1);
}


//******************************************************************
//				Read
//******************************************************************
//* Steps:
//*		1. Create a task.
//*		2. Create a Digital Input channel. Use one channel for all lines.
//*		3. Call the Start function to start the task.
//*		4. Read the digital data.This read function reads a single
//*       sample of digital data on demand, so no timeout is necessary.
//*		5. Call the Clear Task function to clear the Task.
//*		6. Display an error if any.

bool NI_USB_6008::ReadDigital(char chan[], uInt8 w_data[])
{
	// Task variables
	TaskHandle  taskHandle		= nullptr;
	int32       error			= 0;
	char        errBuff[2048]	= { '\0' };				// C string for error

	// Write variables
	int32       read			= 0;
	int32		bytesPerSamp	= 0;

	// Create Digital Output (DO) Task and Channel
	DAQmxErrChk (DAQmxCreateTask("ReadValves", &taskHandle));
	DAQmxErrChk (DAQmxCreateDIChan(taskHandle, chan, "", DAQmx_Val_ChanForAllLines));

	// Start Task (configure port)
	DAQmxErrChk (DAQmxStartTask (taskHandle));

	//  Read from ports
	//  Only 1 sample per channel supported for static DIO
	DAQmxErrChk (DAQmxReadDigitalLines(taskHandle, samplesPerChannel, timeout, DAQmx_Val_GroupByChannel, w_data, bufferSize, &read, &bytesPerSamp, NULL))

Error:
	// In case of error
	if (DAQmxFailed(error))
	{
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		errorKeep.assign(errBuff);
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


//******************************************************************
//				Write
//******************************************************************
//* Steps:
//*    1. Create a task.
//*    2. Create a Digital Output channel. Use one channel for all lines.
//*    3. Call the Start function to start the task.
//*    4. Write the digital Boolean array data. This write function
//*       writes a single sample of digital data on demand, so no
//*       timeout is necessary.
//*    5. Call the Clear Task function to clear the Task.
//*    6. Display an error if any.

bool NI_USB_6008::WriteDigital(char chan[], uInt8 w_data[])
{
	// Task variables
	TaskHandle  taskHandle		= nullptr;
	int32       error			= 0;
	char        errBuff[2048]	= { '\0' };				// C string for error

	// Write variables
	int32       written			= 0;

	// Create Digital Output (DO) Task and Channel
	DAQmxErrChk(DAQmxCreateTask("OperateValves", &taskHandle));
	DAQmxErrChk(DAQmxCreateDOChan(taskHandle, chan, "", DAQmx_Val_ChanForAllLines));

	// Start Task (configure port)
	DAQmxErrChk(DAQmxStartTask(taskHandle));

	//  Write 0x55 to port(s)
	//  Only 1 sample per channel supported for static DIO
	DAQmxErrChk(DAQmxWriteDigitalLines(taskHandle, samplesPerChannel, autostart, timeout, DAQmx_Val_GroupByChannel, w_data, &written, NULL));

Error:
	// In case of error
	if (DAQmxFailed(error))
	{
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		errorKeep.assign(errBuff);
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


void NI_USB_6008::GetError(std::string* err)
{
	*err = errorKeep;
}


///////////////////////////////////////////////////////////
//
// Functions opening and closing different lines from ports

//// Complete set

bool NI_USB_6008::SetPortCustom(int port, unsigned int customarray[8])
{
	if (port == 0)
	{
		int temp[8];
		for (int i = 0; i < 8; i++)
		{
			temp[i] = etatPort0[i];
			etatPort0[i] = customarray[i];
		}

		if (WritePort0())
		{
			return true;
		}

		for (int i = 0; i < 8; i++)
		{
			etatPort0[i] = temp[i];
		}

	}
	else if (port == 1)
	{
		int temp[8];
		for (int i = 0; i < 8; i++)
		{
			temp[i] = etatPort1[i];
			etatPort1[i] = customarray[i];
		}

		if (WritePort1())
		{
			return true;
		}

		for (int i = 0; i < 8; i++)
		{
			etatPort1[i] = temp[i];
		}
	}

	return false;
}

///// Single lines

bool NI_USB_6008::OuvrirPort0(int num)
{
	etatPort0[num] = open;
	if(WritePort0())
	{
		return true;
	}
	
	etatPort0[num] = closed;
	return false;
}

bool NI_USB_6008::FermerPort0(int num)
{
	etatPort0[num] = closed;
	if(WritePort0())
	{
		return true;
	}
	
	etatPort0[num] = open;
	return false;
}


bool NI_USB_6008::OuvrirPort1(int num)
{
	etatPort1[num] = open;
	if(WritePort1())
	{
		return true;
	}
	
	etatPort1[num]=closed;
	return false;
}

bool NI_USB_6008::FermerPort1(int num)
{
	etatPort1[num] = closed;
	if(WritePort1())
	{
		return true;
	}
	
	etatPort1[num] = open;
	return false;
}


///// Whole ports

bool NI_USB_6008::FermerPort0Tous()
{
	int temp[8];
	for (int i = 0; i < 8; i++)
	{
		temp[i] = etatPort0[i];
		etatPort0[i] = closed;
	}
	if (WritePort0()) {
		return true;
	}

	for (int i = 0; i < 8; i++)
	{
		etatPort0[i] = temp[i];
	}
	return false;
}

bool NI_USB_6008::OuvrirPort0Tous()
{
	int temp[8];
	for (int i = 0; i < 8; i++)
	{
		temp[i] = etatPort0[i];
		etatPort0[i] = open;
	}
	if (WritePort0()) {
		return true;
	}

	for (int i = 0; i < 8; i++)
	{
		etatPort0[i] = temp[i];
	}
	return false;
}

bool NI_USB_6008::FermerPort1Tous()
{
	int temp[4];
	for (int i = 0; i < 4; i++)
	{
		temp[i] = etatPort1[i];
		etatPort1[i] = closed;
	}
	if (WritePort0()) {
		return true;
	}

	for (int i = 0; i < 4; i++)
	{
		etatPort1[i] = temp[i];
	}
	return false;
}

bool NI_USB_6008::OuvrirPort1Tous()
{
	int temp[4];
	for (int i = 0; i < 4; i++)
	{
		temp[i] = etatPort1[i];
		etatPort1[i] = open;
	}
	if (WritePort1()) {
		return true;
	}

	for (int i = 0; i < 4; i++)
	{
		etatPort1[i] = temp[i];
	}
	return false;
}


bool NI_USB_6008::EstOuvertPort0(int num)
{return (etatPort0[num]==open);}

bool NI_USB_6008::EstFermePort0(int num)
{return (etatPort0[num]==closed);}


bool NI_USB_6008::EstOuvertPort1(int num)
{return (etatPort1[num]==open);}

bool NI_USB_6008::EstFermePort1(int num)
{return (etatPort1[num]==closed);}


