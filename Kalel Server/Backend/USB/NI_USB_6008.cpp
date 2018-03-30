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
	// Lock for the remainder of function
	std::lock_guard<std::mutex> lk(mutex);

	portUSB = dev;
}


bool NI_USB_6008::ReadPort(unsigned int port)
{
	// Specific channel and line parameters
	char        chan[50]		= { '\0' };
	int			line_start		= 0;
	int			line_end		= 7;
	if (port == 1)	{ line_end = 3;}				// port 1 only has 4 lines

	// Write channel string
	sprintf_s(chan,"Dev%d/port%d/line%d:%d",portUSB, port, line_start, line_end);

	uInt8 read[8] = { 0 };

	// Read state
	if (ReadDigital(chan, read)) {
		//portStates[port].data();
		return true;
	}
	return false;
}

bool NI_USB_6008::WritePort(unsigned int port)
{
	// Specific channel and line parameters
	char        chan[50]		= { 0 };
	int			line_start		= 0;
	int			line_end		= 7;
	if (port == 1) { line_end = 3; }				// port 1 only has 4 line

	// Write channel string
	sprintf_s(chan, "Dev%d/port%d/line%d:%d", portUSB, port, line_start, line_end);

	// Write action
	return WriteDigital(chan, portStates[port].to_ulong());
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
		LOG(logDEBUG) << errBuff;
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

bool NI_USB_6008::WriteDigital(char chan[], const unsigned long w_data)
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
	DAQmxErrChk(DAQmxWriteDigitalU32(taskHandle, samplesPerChannel, autostart, timeout, DAQmx_Val_GroupByChannel, &w_data, &written, NULL));

Error:
	// In case of error
	if (DAQmxFailed(error))
	{
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		LOG(logDEBUG) << errBuff;
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
	// Lock for the remainder of function
	std::lock_guard<std::mutex> lk(mutex);

	int temp[8];
	for (int i : temp)
	{
		temp[i] = portStates[port][i];
		portStates[port][i] = customarray[i];
	}

	if (WritePort(port))
	{
		return true;
	}

	for (int i : temp)
	{
		portStates[port][i] = temp[i];
	}

	return false;
}


///// Whole ports

bool NI_USB_6008::SetChannelAll(unsigned int chan, bool state)
{
	// Lock for the remainder of function
	std::lock_guard<std::mutex> lk(mutex);

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
	// Lock for the remainder of function
	std::lock_guard<std::mutex> lk(mutex);

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



