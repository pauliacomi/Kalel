// This is a small class which is used to return all the UI data regarding buttons

#ifndef LISTOFINTRUMENTBUTTONS_H
#define LISTOFINTRUMENTBUTTONS_H
#pragma once

class ListOfInstrumentButtons
{

public:
	ListOfInstrumentButtons(int instrumentType, int instrumentNumber, bool shouldBeActivated);
	~ListOfInstrumentButtons();

private:
	int cButtonID;								// Id of open button
	int cOppositeButtonID;						// Id of close button
	int cTextboxID;								// Id of associated textbox
	CString tempTextboxMessage;					// String of the textbox
	CString textboxMessage;
	CString errorMessage;;

public:
	int GetButtonID();
	int GetOppositeButtonID();
	int GetTextboxID();
	CString GetTextboxMessage();
	CString GetTempTextboxMessage();
	CString GetErrorMessage();
};

#endif // !LISTOFINTRUMENTS_H