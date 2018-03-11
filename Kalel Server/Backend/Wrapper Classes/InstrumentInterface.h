#pragma once

class InstrumentInterface
{
public:
	virtual void SetComPort(int port) = 0;
	virtual int GetComPort() = 0;

	virtual double Read() { return 0; };
	virtual double Read(unsigned int channel) { return 0; };
	virtual double Read(unsigned int channel, unsigned int subchannel) { return 0; };

	virtual bool SetChannelAll(unsigned int channel, bool state) { return false; };
	virtual bool SetSubchannel(unsigned int channel, unsigned int subchannel, bool state) { return false; };
	virtual bool IsOpenSubchannel(unsigned int channel, unsigned int subchannel) { return false; };
};