// This is a small class which is used to return all the UI data regarding buttons

#ifndef LISTOFINTRUMENTBUTTONS_H
#define LISTOFINTRUMENTBUTTONS_H
#pragma once

#include "../Kalel Shared/Com Classes/ControlInstrumentState.h"

#include <vector>

class CKalelView;

class ListOfInstrumentButtons
{

public:
	ListOfInstrumentButtons(CKalelView & h);
	~ListOfInstrumentButtons();
	void Init();

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

	void StartCommand(int instrumentID, bool shouldBeActivated);
	void EndCommand(const ControlInstrumentStateData &data);
	void Update(const ControlInstrumentState &state);
};

#endif // !LISTOFINTRUMENTS_H