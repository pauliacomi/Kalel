#include "stdafx.h"
#include "CommHandler.h"

#include "Com Classes\ManualActionParam.h"

CommHandler::CommHandler()
{
}


CommHandler::~CommHandler()
{
}

void CommHandler::Connect(std::wstring address)
{
	client.Connect(unicodeConverter.ws2s(address.c_str()));
}

void CommHandler::ManualCommand(int instrumentType, int instrumentNumber, bool shouldBeActivated)
{
	ManualActionParam request(instrumentType, instrumentNumber, shouldBeActivated);
}

int CommHandler::StartClient()
{
	return 0;
}

int CommHandler::ShutdownClient()
{
	return 0;
}

int CommHandler::RestartClient()
{
	return 0;
}

int CommHandler::ResetClient()
{
	return 0;
}

int CommHandler::PauseClient()
{
	return 0;
}

int CommHandler::ResumeClient()
{
	return 0;
}

void CommHandler::SetUserContinue()
{
}

void CommHandler::SetModifiedData()
{
}

