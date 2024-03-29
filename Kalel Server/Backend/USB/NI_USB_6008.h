//*****************************************************************************************************
//
//	This class is the implementation for the National Instruments Controller
//	used for controlling the valves and pump
//
//*****************************************************************************************************

#pragma once

#include "NIDAQmx.h"
#include "../Wrapper Classes/InstrumentInterface.h"
#include <vector>
#include <bitset>
#include <mutex>

class NI_USB_6008 : InstrumentInterface
{
public:
	NI_USB_6008(int dev);
	~NI_USB_6008(void);
	
private:

	//*************************
	// Parameters for writing
	//*************************
	int portUSB;													// USB port
	std::vector<std::bitset<8>> portStates = { {0},{0} };

	// Read/Write parameters
	static const uInt32 maxBufSize	= 8;
	int32		samplesPerChannel	= 1;
	bool32		autostart			= true;
	float64     timeout				= 2.0f;

	// Read/write functions
	bool ReadPort(unsigned int port);
	bool ReadPortLine(unsigned int port, unsigned int line);
	bool WritePort(unsigned int port);
	bool WritePortLine(unsigned int port, unsigned int line);
	bool ReadDigital(char* chan, uInt8 * w_data);
	bool WriteDigital(char* chan, const unsigned long w_data);

private:
	std::mutex mutex;									// locks to prevent clash of threads on writing

public:
	
	void SetComPort(int dev) override;					// Set USB port
	int GetComPort() override;							// Get USB port

	// Functions to open / close stuff

	// Set the state in a custom way
	bool SetChannelCustom(unsigned int chan, bool customarray[8]);
	// Set the entire bits from a channel to a state
	bool SetChannelAll(unsigned int chan, bool state) override;
	// Set a particullar subchannel to a state
	bool SetSubchannel(unsigned int chan, unsigned int subchan, bool state) override;
	// Read if a subchannel is open or not
	bool IsOpenSubchannel(unsigned int chan, unsigned int subchan) override;
};