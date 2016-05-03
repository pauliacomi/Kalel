// This class is used to make the interaction between threads and MFC-derived objects 
// as smooth as possible. All messages will be defined in the header and then will be 
// used to sent PostMessage commends to the specified window

#pragma once
#ifndef MFCMESSAGEHANDLER_H
#define MFCMESSAGEHANDLER_H

#define default_val -1	// This value is used as the default value for the optional parameters of the message display function. 
						// Make sure that it is set to a value that the actual parameters can never take

class MFCMessageHandler
{
	// Constructor and destructor
public:
	MFCMessageHandler();
	virtual ~MFCMessageHandler();
	
	// Window handle for message postback
	HWND windowHandle;

public:
	// Set the handle
	BOOL SetHandle(HWND h);

	// PostMessage functions
	BOOL ResetDocumentGraph();
	BOOL GraphSetTitle(CString title);
	BOOL GraphAddData(int num, double tps, double calorimeter, double lowPressure, double highPressure, double tempCalo, double tempCage, double tempPiece);
	BOOL GraphDataAutoStep(float temps, int mesure, int num_etape);

	BOOL UpdateDisplay();
	BOOL UnlockMenu();
	BOOL EnableStartButton();
	BOOL CancelExperiment();
	BOOL DisplayInitialPressure();
	BOOL DisplayFinalPressure();
	BOOL ExchangeData();
	BOOL DisplayMeasurement(int pParam);

	BOOL DisplayMessage(int pParam, int pInt1 = default_val, int pInt2 = default_val, double pDouble = default_val);								// Simple display message which takes an int

	BOOL DisplayAddMessage(int pParam);

	// Steps
	BOOL DisplayStep(int pParam);
	BOOL DisplaAddStep(CString pParam);
	BOOL DisplayPreviousStep();

	// Data
	BOOL DisplayCalorimeter();
	BOOL DisplayHighPressure();
	BOOL DisplayLowPressure();
	BOOL DisplayTemperatures();

	// Messageboxes
	BOOL DisplayMessageBox(int message, UINT nType, float pFloat = default_val);


	// What i really need
	void ExperimentStart();
	void ExperimentEnd();
};


#endif // !MFCMESSAGEHANDLER_H