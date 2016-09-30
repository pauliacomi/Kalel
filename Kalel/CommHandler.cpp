#include "stdafx.h"
#include "CommHandler.h"

#include "Com Classes\ManualActionParam.h"
#include "Netcode/http_request.h"
#include "Netcode/Client.h"
#include "Resources/StringTable.h"

CommHandler::CommHandler()
{
}


CommHandler::~CommHandler()
{
}

void CommHandler::SetHandle(HWND h){

}

void CommHandler::Connect(std::wstring address)
{
	try
	{
		client.Request(Handshake_req, Handshake_resp, unicodeConverter.ws2s(address.c_str()));
	}
	catch (const std::exception& e)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK,false, e.what());
	}
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




void CommHandler::Handshake_req(http_request* r) {
	r->method_ = "GET";
	r->path_ = "/api/handshake";
}

void CommHandler::Handshake_resp(http_response* r) {

	//messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, "connected");

	if (r->status_ == "202 OK")
	{
		AfxMessageBox(_T("connected"));
	}
}

void getJson(http_request* r) {

	r->method_ = "GET";
	r->path_ = "/api";
	r->params_ = {
		{ "name","dog" },
		{ "breed","lab" },
		{ "age","1" }
	};
	r->accept_ = "text/json";
}