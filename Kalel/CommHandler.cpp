#include "stdafx.h"
#include "CommHandler.h"
#include "ManualActionParam.h"

CommHandler::CommHandler()
{
}


CommHandler::~CommHandler()
{
}

void CommHandler::ManualCommand(HWND hwid, int instrumentType, int instrumentNumber, bool shouldBeActivated)
{
	ManualActionParam request(HWND hwid, int instrumentType, int instrumentNumber, bool shouldBeActivated);
}

