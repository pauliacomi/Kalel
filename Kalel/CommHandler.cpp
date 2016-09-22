#include "stdafx.h"
#include "CommHandler.h"

#include "Com Classes\ManualActionParam.h"
#include "Netcode/http_request.h"
#include "Netcode/Client.h"

CommHandler::CommHandler()
{
}


CommHandler::~CommHandler()
{
}

void Initial(http_request* r) {

	r->method_ = "GET";
	r->path_ = "/api";
	r->params_ = {
		{"name","dog"},
		{"breed","lab"},
		{"age","1"}
	};
}

void CommHandler::Connect(std::wstring address)
{
	client.Connect(Initial, unicodeConverter.ws2s(address.c_str()));
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



void Get(http_request* r) {

	if (r->path_ == "/api") {
		r->status_ = "202 OK";
	}
	else if (r->path_ == "/api/machinesettings") {
	}
	else if (r->path_ == "/api/experimentdata") {
	}
	else if (r->path_ == "/api/results") {
	}
	else {
		r->status_ = "404 Not Found";
	}
}