// This is a small class which is used to return all the UI data regarding buttons

#ifndef LISTOFINTRUMENTBUTTONS_H
#define LISTOFINTRUMENTBUTTONS_H
#pragma once

#include "../Kalel Shared/Com Classes/ControlInstrumentState.h"
#include "KalelView.h"

class ListOfInstrumentButtons
{

public:
	ListOfInstrumentButtons(CKalelView & h);
	~ListOfInstrumentButtons();

private:
	int cButtonID;								// Id of open button
	int cOppositeButtonID;						// Id of close button
	int cTextboxID;								// Id of associated textbox
	CString tempTextboxMessage;					// String of the textbox
	CString textboxMessage;
	CString errorMessage;;


	CKalelView & handle;

	ControlInstrumentState localState;

	std::vector<CString> valveText;
	std::vector<CString> evText;
	CString pumpText;


public:

	void StartCommand(int instrumentType, int instrumentNumber, bool shouldBeActivated);
	void EndCommand(int instrumentType, int instrumentNumber, bool shouldBeActivated);
	void Update(int instrumentType, int instrumentNumber, bool shouldBeActivated);
	void Update(const ControlInstrumentState &state);
};

#endif // !LISTOFINTRUMENTS_H