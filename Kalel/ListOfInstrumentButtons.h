// This is a small class which is used to return all the UI data regarding buttons

#pragma once
#ifndef LISTOFINTRUMENTBUTTONS_H
#define LISTOFINTRUMENTBUTTONS_H

class ListOfInstrumentButtons
{

public:
	ListOfInstrumentButtons(int instrumentType, int instrumentNumber, bool shouldBeActivated);
	~ListOfInstrumentButtons();

private:
	int cButtonID;
	int cOppositeButtonID;
	int cTextboxID;
	CString tempTextboxMessage;
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