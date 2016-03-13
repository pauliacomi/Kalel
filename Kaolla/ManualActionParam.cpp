#include "stdafx.h"
#include "ManualActionParam.h"


ManualActionParam::ManualActionParam()
{
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
