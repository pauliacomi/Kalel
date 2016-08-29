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

LRESULT CommHandler::StartClient()
{
	return LRESULT();
}

LRESULT CommHandler::ShutdownClient()
{
	return LRESULT();
}

LRESULT CommHandler::RestartClient()
{
	return LRESULT();
}

LRESULT CommHandler::ResetClient()
{
	return LRESULT();
}

LRESULT CommHandler::PauseClient()
{
	return LRESULT();
}

LRESULT CommHandler::ResumeClient()
{
	return LRESULT();
}

void CommHandler::SetUserContinue()
{
}

void CommHandler::SetModifiedData()
{
}

