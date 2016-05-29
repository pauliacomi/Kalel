#include "stdafx.h"
#include "ManualActionParam.h"


ManualActionParam::ManualActionParam()
{
}

ManualActionParam::ManualActionParam(ManualActionParam * mParam)
{
	this->windowHandle = mParam->windowHandle;
	this->instrumentType = mParam->instrumentType;
	this->instrumentNumber = mParam->instrumentNumber;
	this->shouldBeActivated = mParam->shouldBeActivated;
}

ManualActionParam::ManualActionParam(HWND wHnd, int instrType, int instrNumber, bool shouldBeActive)
{
	windowHandle = wHnd;
	instrumentType = instrType;
	instrumentNumber = instrNumber;
	shouldBeActivated = shouldBeActive;
}

ManualActionParam::~ManualActionParam()
{
}
