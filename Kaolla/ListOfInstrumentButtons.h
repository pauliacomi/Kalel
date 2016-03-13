// This is a small class which is used to keep a list of all the intruments which can be accessed

#pragma once
#ifndef LISTOFINTRUMENTBUTTONS_H
#define LISTOFINTRUMENTBUTTONS_H

class ListOfInstrumentButtons
{

public:
	ListOfInstrumentButtons();
	~ListOfInstrumentButtons();

public:
	int GetButtonID(int instrumentType, int instrumentNumber, bool shouldBeActivated);
	int GetTextboxID(int instrumentType, int instrumentNumber);
};

#endif // !LISTOFINTRUMENTS_H