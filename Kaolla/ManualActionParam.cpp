#include "stdafx.h"
#include "ManualActionParam.h"


ManualActionParam::ManualActionParam()
{
}

ManualActionParam::ManualActionParam(HWND wHnd, int instrType, int instrNumber, bool instrIsActivated)
{
	windowHandle = wHnd;
	instrumentType = instrType;
	instrumentNumber = instrNumber;
	instrumentIsActivated = instrIsActivated;
}

ManualActionParam::~ManualActionParam()
{
}
