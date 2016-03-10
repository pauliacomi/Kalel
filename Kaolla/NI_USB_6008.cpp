#include "StdAfx.h"
#include "NI_USB_6008.h"

NI_USB_6008::NI_USB_6008(void)
{
	DevNI_USB_6008 = -1;
	for(int i=0;i<=7;i++)
		etatPort0[i]=0;
	for(int i=0;i<=3;i++)
		etatPort1[i]=0;
}

NI_USB_6008::NI_USB_6008(int dev)
{
	DevNI_USB_6008 = dev;
	for(int i=0;i<=7;i++)
		etatPort0[i]=0;
	for(int i=0;i<=3;i++)
		etatPort1[i]=0;
	FermerPort0Tous();
	FermerPort1Tous();
}

NI_USB_6008::~NI_USB_6008(void)
{
}

int NI_USB_6008::GetDevNI_USB_6008()
{
	return DevNI_USB_6008;
}

void NI_USB_6008::SetDevNI_USB_6008(int dev)
{
	DevNI_USB_6008 = dev;
}


bool NI_USB_6008::ActionPort0()
{
	// Channel parameters
	char        chan[50];
	sprintf(chan,"Dev%d/port0/line0:7",DevNI_USB_6008);

	// Write parameters
	uInt32      w_data [1];

	w_data[0] = 0;
	for (int i=0;i<=7;i++)
		if (etatPort0[i]==ouvert)
			w_data[0] += (int)pow(2.0,i);

	return ActionDigital(chan,w_data);
}

bool NI_USB_6008::ActionPort1()
{
	// Channel parameters
	char        chan[50];
	sprintf(chan,"Dev%d/port1/line0:3",DevNI_USB_6008);

	// Write parameters
	uInt32      w_data [1];

	w_data[0] = 0;
	for (int i=0;i<=3;i++)
		if (etatPort1[i]==ouvert)
			w_data[0] += (int)pow(2.0,i);

	return ActionDigital(chan,w_data);
}


bool NI_USB_6008::ActionDigital(char chan[], uInt32 w_data[]) 
{
	// Task parameters
	int32       error = 0;
	TaskHandle  taskHandle = 0;
	char        errBuff[2048];

	// Channel parameters
	// char        chan[50];

	// Write parameters
	//uInt32      w_data [1];
	int32       written;


	//sprintf(chan,"Dev%d/port0/line0:7",DevNI_USB_6008);

	// Create Digital Output (DO) Task and Channel
	DAQmxCreateTask ("", &taskHandle);
	DAQmxCreateDOChan(taskHandle,chan,"",DAQmx_Val_ChanForAllLines);

	// Start Task (configure port)
	//DAQmxErrChk (DAQmxBaseStartTask (taskHandle));
	DAQmxStartTask (taskHandle);

	//  Write 0x55 to port(s)
	//  Only 1 sample per channel supported for static DIO
	//  Autostart ON

/*
	w_data[0] = 0;
	for (int i=0;i<=7;i++)
		if (etatPort0[i]==ouvert)
			w_data[0] += (int)pow(2.0,i);
*/

	DAQmxWriteDigitalU32(taskHandle,1,1,10.0,DAQmx_Val_GroupByChannel,w_data,&written,NULL);

	
	if (taskHandle != 0)
	{
		DAQmxStopTask (taskHandle);
		DAQmxClearTask (taskHandle);
	}
	else
		return FALSE;
	//etat=ouvert;
	return TRUE;
}


bool NI_USB_6008::OuvrirPort0(int num)
{
	etatPort0[num]=ouvert;
	if(ActionPort0())
	{
		return TRUE;
	}
	//else
	etatPort0[num]=ferme;
	return FALSE;
}

bool NI_USB_6008::FermerPort0(int num)
{
	etatPort0[num]=ferme;
	if(ActionPort0())
	{
		return TRUE;
	}
	//else
	etatPort0[num]=ouvert;
	return FALSE;
}


bool NI_USB_6008::OuvrirPort1(int num)
{
	etatPort1[num]=ouvert;
	if(ActionPort1())
	{
		return TRUE;
	}
	//else
	etatPort1[num]=ferme;
	return FALSE;
}

bool NI_USB_6008::FermerPort1(int num)
{
	etatPort1[num]=ferme;
	if(ActionPort1())
	{
		return TRUE;
	}
	//else
	etatPort1[num]=ouvert;
	return FALSE;
}

bool NI_USB_6008::FermerPort0Tous()
{
	int temp[8];
	for(int i=0;i<8;i++)
	{
		temp[i] = etatPort0[i];
		etatPort0[i] = ferme;
	}
	if(ActionPort0())
		return TRUE;
	for(int i=0;i<8;i++)
	{
		etatPort0[i] = temp[i];
	}
	return FALSE;
}

bool NI_USB_6008::OuvrirPort0Tous()
{
	int temp[8];
	for(int i=0;i<8;i++)
	{
		temp[i] = etatPort0[i];
		etatPort0[i] = ouvert;
	}
	if(ActionPort0())
		return TRUE;
	for(int i=0;i<8;i++)
	{
		etatPort0[i] = temp[i];
	}
	return FALSE;
}

bool NI_USB_6008::FermerPort1Tous()
{
	int temp[4];
	for(int i=0;i<4;i++)
	{
		temp[i] = etatPort1[i];
		etatPort1[i] = ferme;
	}
	if(ActionPort1())
		return TRUE;
	for(int i=0;i<4;i++)
	{
		etatPort1[i] = temp[i];
	}
	return FALSE;
}

bool NI_USB_6008::OuvrirPort1Tous()
{
	int temp[4];
	for(int i=0;i<4;i++)
	{
		temp[i] = etatPort1[i];
		etatPort1[i] = ouvert;
	}
	if(ActionPort1())
		return TRUE;
	for(int i=0;i<4;i++)
	{
		etatPort1[i] = temp[i];
	}
	return FALSE;
}


bool NI_USB_6008::EstOuvertPort0(int num)
{return (etatPort0[num]==ouvert);}

bool NI_USB_6008::EstFermePort0(int num)
{return (etatPort0[num]==ferme);}


bool NI_USB_6008::EstOuvertPort1(int num)
{return (etatPort1[num]==ouvert);}

bool NI_USB_6008::EstFermePort1(int num)
{return (etatPort1[num]==ferme);}