// This is a small class which is used to hold the objects/variables which will be passed 
// from the UI to the threading functionality when a manual command is given to the system

#pragma once
#ifndef MANUALACTIONPARAM_H
#define MANUALACTIONPARAM_H


class ManualActionParam
{
public:
	ManualActionParam();
	ManualActionParam(ManualActionParam * mParam);
	ManualActionParam(int instrType, int instrNumber, bool instrIsActivated);
	~ManualActionParam();

public:
	int instrumentType;
	int instrumentNumber;
	bool shouldBeActivated;
};


#endif // !MANUALACTIONPARAM_H