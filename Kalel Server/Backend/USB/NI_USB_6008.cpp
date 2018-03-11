#include "NI_USB_6008.h"

#include "../../../Kalel Shared/log.h"

#define DAQmxErrChk(functionCall) { if( DAQmxFailed(error=(functionCall)) ) { goto Error; } }
#define open 1
#define closed  0


NI_USB_6008::NI_USB_6008(int dev)
{
	portUSB = dev;
	ReadPort(0);				// to generate initial states of valves
	ReadPort(1);				// to generate initial states of valves
}

NI_USB_6008::~NI_USB_6008(void)
{
}

// Get and set functions for the USB port

int NI_USB_6008::GetComPort()
{
	return portUSB;
}

void NI_USB_6008::SetComPort(int dev)
{
	portUSB = dev;
}


bool NI_USB_6008::ReadPort(unsigned int port)
{
	// Specific channel and line parameters
	char        chan[50]		= { '\0' };
	int			line_start		= 0;
	int			line_end		= 7;
	
	// Write channel string
	sprintf_s(chan,"Dev%d/port%d/line%d:%d",portUSB, port, line_start, line_end);

	// Read state
	if (ReadDigital(chan, portStates[port].data())) {
		return true;
	}
	return false;
}

bool NI_USB_6008::WritePort(unsigned int port)
{
	// Specific channel and line parameters
	char        chan[50]		= { '\0' };
	int			line_start		= 0;
	int			line_end		= 7;

	// Write channel string
	sprintf_s(chan, "Dev%d/port%d/line%d:%d", portUSB, port, line_start, line_end);

	// Write action
	return WriteDigital(chan, portStates[port].data());
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
		LOG(logERROR) << errBuff;
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
		LOG(logERROR) << errBuff;
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


///////////////////////////////////////////////////////////
//
// Functions opening and closing different lines from ports

//// Complete set

bool NI_USB_6008::SetChannelCustom(unsigned int port, unsigned int customarray[8])
{
	if (port == 0)
	{
		int temp[8];
		for (int i = 0; i < 8; i++)
		{
			temp[i] = portStates[0][i];
			portStates[0][i] = customarray[i];
		}

		if (WritePort(0))
		{
			return true;
		}

		for (int i = 0; i < 8; i++)
		{
			portStates[0][i] = temp[i];
		}

	}
	else if (port == 1)
	{
		int temp[8];
		for (int i = 0; i < 8; i++)
		{
			temp[i] = portStates[1][i];
			portStates[1][i] = customarray[i];
		}

		if (WritePort(1))
		{
			return true;
		}

		for (int i = 0; i < 8; i++)
		{
			portStates[1][i] = temp[i];
		}
	}

	return false;
}


///// Whole ports

bool NI_USB_6008::SetChannelAll(unsigned int chan, bool state)
{
	int temp[8];
	for (int i : temp)
	{
		temp[i] = portStates[chan][i];
		portStates[chan][i] = state;
	}
	if (WritePort(chan)) {
		return true;
	}

	for (int i : temp)
	{
		portStates[chan][i] = temp[i];
	}
	return false;
}

///// Single lines

bool NI_USB_6008::SetSubchannel(unsigned int chan, unsigned int subchan, bool state)
{
	portStates[chan][subchan] = state;
	if(WritePort(chan))
	{
		return true;
	}
	
	portStates[chan][subchan] = !state;
	return false;
}



bool NI_USB_6008::IsOpenSubchannel(unsigned int chan, unsigned int subchan)
{return (portStates[chan][subchan]==open);}



