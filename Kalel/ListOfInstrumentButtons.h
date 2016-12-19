// This is a small class which is used to return all the UI data regarding buttons

#ifndef LISTOFINTRUMENTBUTTONS_H
#define LISTOFINTRUMENTBUTTONS_H
#pragma once

#include "../Kalel Shared/Com Classes/ControlInstrumentState.h"

class CKalelView;

class ListOfInstrumentButtons
{

public:
	ListOfInstrumentButtons(CKalelView & h);
	~ListOfInstrumentButtons();

private:
	int cButtonID = 0;								// Id of open button
	int cOppositeButtonID = 0;						// Id of close button
	int cTextboxID = 0;								// Id of associated textbox
	CString tempTextboxMessage;						// String of the textbox
	CString textboxMessage;
	CString errorMessage;;


	CKalelView & handle;

	ControlInstrumentState localState;

	std::vector<CString> valveText;
	std::vector<CString> evText;
	CString pumpText;


public:

	void StartCommand(int instrumentType, int instrumentNumber, bool shouldBeActivated);
	void EndCommand(ControlInstrumentStateData data);
	void Update(const ControlInstrumentState &state);
	void Update(int instrumentType, int instrumentNumber, bool shouldBeActivated);
};

#endif // !LISTOFINTRUMENTS_H