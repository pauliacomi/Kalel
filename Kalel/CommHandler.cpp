#include "stdafx.h"
#include "CommHandler.h"

#include "../Kalel Shared/Netcode/http_request.h"
#include "../Kalel Shared/Netcode/json.hpp"
#include "../Kalel Shared/Resources/StringTable.h"
#include "../Kalel Shared/Resources/DefineInstruments.h"
#include "../Kalel Shared/Com Classes/Serialization.h"
#include "../Kalel Shared/Com Classes/ControlInstrumentState.h"
#include "../Kalel Shared/stringHelpers.h"
#include "../Kalel Shared/timeHelpers.h"

#include <functional>

using json = nlohmann::json;

#define START		1
#define SHUTDOWN	2
#define RESTART		3
#define RESET		4
#define PAUSE		5
#define RESUME		6

#define SAMPLEVACUUM		1
#define BOTTLEVACUUM		2
#define BOTTLECHANGE		3

CommHandler::CommHandler()
{
}

CommHandler::~CommHandler()
{
}

void CommHandler::SetHandle(HWND h){
	messageHandler.setHandle(h);
}

/*********************************
//
//  Requests
//
*********************************/

/*********************************
// Ping
*********************************/
void CommHandler::Connect(std::wstring address)
{
	if (!address.empty())
	{
		auto request = std::bind(&CommHandler::Handshake_req, this, std::placeholders::_1);
		auto callback = std::bind(&CommHandler::Handshake_resp, this, std::placeholders::_1);

		try	{
			client.Request(request, callback, stringh::ws2s(address.c_str()));
		}
		catch (const std::exception& e)	{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK,false, stringh::s2ws(e.what()));
		}
	}
}

void CommHandler::SaveAddress(std::wstring address)
{
	localAddress = stringh::ws2s(address.c_str());
}

/*********************************
// MachineSettings
*********************************/
void CommHandler::GetMachineSettings(std::string fromTime)
{
	auto request = std::bind(&CommHandler::GetMachineSettings_req, this, std::placeholders::_1, fromTime);
	auto callback = std::bind(&CommHandler::GetMachineSettings_resp, this, std::placeholders::_1);

	try	{
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e)	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, stringh::s2ws(e.what()));
	}
}

void CommHandler::SetMachineSettings(const MachineSettings &ptr)
{
	auto request = std::bind(&CommHandler::SetMachineSettings_req, this, std::placeholders::_1, MachineSettings(ptr));
	auto callback = std::bind(&CommHandler::SetMachineSettings_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, stringh::s2ws(e.what()));
	}
}


/*********************************
// ExperimentSettings
*********************************/
void CommHandler::GetExperimentSettings(std::string fromTime)
{
	auto request = std::bind(&CommHandler::GetExperimentSettings_req, this, std::placeholders::_1, fromTime);
	auto callback = std::bind(&CommHandler::GetExperimentSettings_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, stringh::s2ws(e.what()));
	}
}

void CommHandler::SetExperimentSettings(const ExperimentSettings &ptr)
{
	auto request = std::bind(&CommHandler::SetExperimentSettings_req, this, std::placeholders::_1, ExperimentSettings(ptr));
	auto callback = std::bind(&CommHandler::SetExperimentSettings_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, stringh::s2ws(e.what()));
	}
}

/*********************************
// ExperimentStatus
*********************************/
void CommHandler::GetExperimentStatus(std::string fromTime)
{
	auto request = std::bind(&CommHandler::GetExperimentStatus_req, this, std::placeholders::_1, fromTime);
	auto callback = std::bind(&CommHandler::GetExperimentStatus_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, stringh::s2ws(e.what()));
	}
}

/*********************************
// Instrument State / Instrument control
*********************************/
void CommHandler::GetControlInstrumentState(std::string fromTime)
{
	auto request = std::bind(&CommHandler::GetInstrumentState_req, this, std::placeholders::_1, fromTime);
	auto callback = std::bind(&CommHandler::GetInstrumentState_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, stringh::s2ws(e.what()));
	}
}


void CommHandler::ManualCommand(int instrumentType, int instrumentNumber, bool instrumentState)
{
	auto request = std::bind(&CommHandler::SetInstrumentState_req, this, std::placeholders::_1, instrumentType, instrumentNumber, instrumentState);
	auto callback = std::bind(&CommHandler::SetInstrumentState_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, stringh::s2ws(e.what()));
	}
}


/*********************************
// Data
*********************************/
void CommHandler::GetData(std::string fromTime)
{
	if (!flagExperimentRequest)
	{
		flagExperimentRequest = true;

		auto request = std::bind(&CommHandler::GetData_req, this, std::placeholders::_1, fromTime);
		auto callback = std::bind(&CommHandler::GetData_resp, this, std::placeholders::_1);

		try {
			client.Request(request, callback, localAddress);
		}
		catch (const std::exception& e) {
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, stringh::s2ws(e.what()));
			flagExperimentRequest = false;
		}
	}
}


/*********************************
// Logs
*********************************/
void CommHandler::GetLog(std::string fromTime)
{
	if (!flagLogsRequest)
	{
		flagLogsRequest = true;

		auto request = std::bind(&CommHandler::GetLogs_req, this, std::placeholders::_1, fromTime);
		auto callback = std::bind(&CommHandler::GetLogs_resp, this, std::placeholders::_1);

		try {
			client.Request(request, callback, localAddress);
		}
		catch (const std::exception& e) {
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, stringh::s2ws(e.what()));
			flagLogsRequest = false;
		}
	}
}


/*********************************
// Errors/requests
*********************************/
void CommHandler::GetRequests(std::string fromTime)
{
	if (!flagReqRequest)
	{
		flagReqRequest = true;

		auto request = std::bind(&CommHandler::GetRequest_req, this, std::placeholders::_1, fromTime);
		auto callback = std::bind(&CommHandler::GetRequest_resp, this, std::placeholders::_1);

		try {
			client.Request(request, callback, localAddress);
		}
		catch (const std::exception& e) {
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, stringh::s2ws(e.what()));
			flagReqRequest = false;
		}
	}
}


/*********************************
// Automation control
*********************************/
void CommHandler::StartClient() { ThreadCommand(START); }

void CommHandler::ShutdownClient() { ThreadCommand(SHUTDOWN); }

void CommHandler::RestartClient() {	ThreadCommand(RESTART); }

void CommHandler::ResetClient() { ThreadCommand(RESET); }

void CommHandler::PauseClient() { ThreadCommand(PAUSE); }

void CommHandler::ResumeClient() { ThreadCommand(RESUME); }

void CommHandler::SetUserChoice()
{
}

void CommHandler::ThreadCommand(int command)
{
	auto request = std::bind(&CommHandler::ThreadCommand_req, this, std::placeholders::_1, command);
	auto callback = std::bind(&CommHandler::ThreadCommand_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, stringh::s2ws(e.what()));
	}
}

/*********************************
// Automation control
*********************************/
void CommHandler::FunctionSampleVacuum() { FunctionalityCommand(SAMPLEVACUUM); }

void CommHandler::FunctionBottleVacuum() { FunctionalityCommand(BOTTLEVACUUM); }

void CommHandler::FunctionChangeBottle() { FunctionalityCommand(BOTTLECHANGE); }


void CommHandler::FunctionalityCommand(int functionality)
{
	auto request = std::bind(&CommHandler::FunctionalityCommand_req, this, std::placeholders::_1, functionality);
	auto callback = std::bind(&CommHandler::FunctionalityCommand_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, stringh::s2ws(e.what()));
	}
}

/*********************************
// User choice
*********************************/

void CommHandler::UserChoice(int choice)
{
	auto request = std::bind(&CommHandler::UserChoice_req, this, std::placeholders::_1, choice);
	auto callback = std::bind(&CommHandler::UserChoice_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, stringh::s2ws(e.what()));
	}
}

/**********************************************************************************************************************************
//
//
// Request and response functions
//
//
**********************************************************************************************************************************/

/*********************************
// Ping
*********************************/
unsigned CommHandler::Handshake_req(http_request* r) {
	r->method = http::method::get;
	r->path = "/api/handshake";
	return 0;
}

unsigned CommHandler::Handshake_resp(http_response* r) {

	if (r->status == http::responses::ok)
	{
		messageHandler.ConnectionComplete();
	}
	else if (r->status == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server not found"));
		return 1;
	}
	else if(r->disconnected)
	{
		messageHandler.Disconnection();
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server disconnected"));
		return 1;
	}

	return 0;
}


/*********************************
// Get MachineSettings
*********************************/
unsigned CommHandler::GetMachineSettings_req(http_request* r, std::string fromTime) {
	r->method = http::method::get;
	r->accept = http::mimetype::appjson;
	r->path = "/api/machinesettings";
	r->params.emplace("time", fromTime);
	return 0;
}

unsigned CommHandler::GetMachineSettings_resp(http_response* r) {

	if (r->status == http::responses::ok)
	{
		if (r->content_type.find(http::mimetype::appjson) != std::string::npos) {

			// Deserisalise data
			//////////////////////////////////////////////
			MachineSettings * receivedSettings;

			try
			{
				*receivedSettings = json::parse(r->body.c_str());
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, stringh::s2ws(e.what()));
				return 1;
			}

			messageHandler.ExchangeMachineSettings(receivedSettings);
		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server does not support Machine Settings send"));
		return 1;
	}

	return 0;
}


/*********************************
// Set MachineSettings
*********************************/

unsigned CommHandler::SetMachineSettings_req(http_request* r, MachineSettings ms) {

	r->method			= http::method::post;
	r->content_type	= http::mimetype::appjson;
	r->path			= "/api/machinesettings";

	json j;
	try
	{
		j = ms;
	}
	catch (const std::exception& e)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, stringh::s2ws(e.what()));
		return 1;
	}

	r->body = j.dump();

	return 0;
}

unsigned CommHandler::SetMachineSettings_resp(http_response* r) {
	
	if (r->status == http::responses::ok)
	{
		messageHandler.OnSetMachineSettings();
	}
	else if (r->status == http::responses::internal_err)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server error, could not update settings"));
		return 1;
	}

	return 0;
}


/*********************************
// Get ExperimentSettings
*********************************/
unsigned CommHandler::GetExperimentSettings_req(http_request* r, std::string fromTime) {
	r->method = http::method::get;
	r->accept = http::mimetype::appjson;
	r->path = "/api/experimentsettings";
	r->params.emplace("time", fromTime);
	return 0;
}

unsigned CommHandler::GetExperimentSettings_resp(http_response* r) {

	if (r->status == http::responses::ok)
	{
		if (r->content_type.find(http::mimetype::appjson) != std::string::npos) {

			// Deserisalise data
			//////////////////////////////////////////////
			ExperimentSettings * receivedSettings;

			try
			{
				*receivedSettings = json::parse(r->body.c_str());
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, stringh::s2ws(e.what()));
				return 1;
			}

			messageHandler.ExchangeExperimentSettings(receivedSettings);
		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server cannot get Experiment Settings"));
		return 1;
	}

	return 0;
}


/*********************************
// Set ExperimentSettings
*********************************/
unsigned CommHandler::SetExperimentSettings_req(http_request* r, ExperimentSettings es) {
	r->method = http::method::post;
	r->content_type = http::mimetype::appjson;
	r->path = "/api/experimentsettings";

	json j;
	try
	{
		j = es;
	}
	catch (const std::exception& e)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, stringh::s2ws(e.what()));
		return 1;
	}

	r->body = j.dump();

	return 0;
}

unsigned CommHandler::SetExperimentSettings_resp(http_response* r) {

	if (r->status == http::responses::ok)
	{
		messageHandler.OnSetExperimentSettings();
	}
	else if (r->status == http::responses::internal_err)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server error, could not start a new experiment"));
		return 1;
	}

	return 0;
}


/*********************************
// Get ExperimentStatus
*********************************/
unsigned CommHandler::GetExperimentStatus_req(http_request* r, std::string fromTime) {
	r->method = http::method::get;
	r->accept = http::mimetype::appjson;
	r->path = "/api/experimentstatus";
	r->params.emplace("time", fromTime);
	return 0;
}

unsigned CommHandler::GetExperimentStatus_resp(http_response* r) {

	if (r->status == http::responses::ok)
	{
		if (r->content_type.find(http::mimetype::appjson) != std::string::npos) {

			// Deserisalise data
			//////////////////////////////////////////////
			ExperimentStatus * receivedStatus;

			try
			{
				*receivedStatus = json::parse(r->body.c_str());
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, stringh::s2ws(e.what()));
				return 1;
			}

			messageHandler.ExchangeExperimentStatus(receivedStatus);
		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server cannot get Experiment Status"));
		return 1;
	}

	return 0;
}


/*********************************
// Get machine Instrument State
*********************************/
unsigned CommHandler::GetInstrumentState_req(http_request * r, std::string fromTime)
{
	r->method = http::method::get;
	r->accept = http::mimetype::appjson;
	r->path = "/api/instrument";
	r->params.emplace("time", fromTime);
	return 0;
}

unsigned CommHandler::GetInstrumentState_resp(http_response * r)
{
	if (r->status == http::responses::ok)
	{
		// Deserialise Data
		ControlInstrumentState * instrumentState;
		try
		{
			*instrumentState = json::parse(r->body.c_str());
		}
		catch (const std::exception& e)
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, stringh::s2ws(e.what()));
			return 1;
		}
		// Exchange
		messageHandler.ExchangeControlState(instrumentState);
	}
	else if (r->status == http::responses::not_found) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server cannot get Instrument State"));
		return 1;
	}
	return 0;
}


/*********************************
// Set machine Instrument State
*********************************/
unsigned CommHandler::SetInstrumentState_req(http_request * r, int instrumentType, int instrumentNumber, int instrumentState)
{
	r->method = http::method::post;
	r->path = "/api/instrument";

	std::string localInstrumentType;

	switch (instrumentType)
	{
	case CONTROLLER_VALVE:
		localInstrumentType = "valve";
		break;

	case CONTROLLER_EV:
		localInstrumentType = "ev";
		break;

	case CONTROLLER_PUMP:
		localInstrumentType = "pump";
		break;
	default:
		break;
	}

	r->params.emplace("type", localInstrumentType);
	r->params.emplace("number", instrumentNumber);
	r->params.emplace("active", instrumentState);

	localInstrumentState.instrumentType = instrumentType;
	localInstrumentState.instrumentNumber = instrumentNumber;
	localInstrumentState.instrumentState = instrumentState;

	return 0;
}

unsigned CommHandler::SetInstrumentState_resp(http_response * r)
{
	if (r->status == http::responses::ok)
	{
		messageHandler.ExchangeControlStateSpecific(localInstrumentState);

		return 1;
	}
	else if (r->status == http::responses::conflict)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server cannot process thread command"));
		return 1;
	}
	else if (r->status == http::responses::bad_request)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Bad request"));
		return 1;
	}
	else if (r->status == http::responses::not_found) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server cannot set Instrument State"));
		return 1;
	}
	return 0;
}


/*********************************
// Get Data
*********************************/
unsigned CommHandler::GetData_req(http_request* r, std::string fromTime) {
	r->method = http::method::get;
	r->accept = http::mimetype::appjson;
	r->path = "/api/experimentdata";
	r->params.emplace("time", fromTime);
	return 0;
}

unsigned CommHandler::GetData_resp(http_response* r) {

	flagExperimentRequest = false;

	if (r->status == http::responses::ok)
	{
		if (r->content_type.find(http::mimetype::appjson) != std::string::npos) {
			
			json j;

			// Parse JSON
			//////////////////////////////////////////////
			try
			{
				j = json::parse(r->body.c_str());
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, stringh::s2ws(e.what()));
				return 1;
			}

			// Deserialise Data
			//////////////////////////////////////////////
			auto receivedDataArray = new std::map<std::chrono::system_clock::time_point, ExperimentData>();

			for (json::iterator i = j.begin(); i != j.end(); ++i)
			{
				std::shared_ptr<ExperimentData> receivedData = nullptr;
				json j2 = j[i.key()];
				try
				{
					receivedData = std::make_shared<ExperimentData>(j2);
				}
				catch (const std::exception& e)
				{
					messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, stringh::s2ws(e.what()));
					delete receivedDataArray;
					return 1;
				}
				receivedDataArray->insert(std::make_pair(timeh::StringToTimePoint(i.key()), *receivedData));
			}																													 

			messageHandler.ExchangeData(receivedDataArray);
		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server cannot get Experiment Data"));
		return 1;
	}
	else if (r->disconnected)
	{
		messageHandler.Disconnection();
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server disconnected"));
	}

	return 0;
}


/*********************************
// Get Logs
*********************************/
unsigned CommHandler::GetLogs_req(http_request * r, std::string fromTime)
{
	r->method = http::method::get;
	r->accept = http::mimetype::appjson;
	r->path = "/api/experimentlogs";
	r->params.emplace("time", fromTime);
	return 0;
}

unsigned CommHandler::GetLogs_resp(http_response * r)
{
	
	flagLogsRequest = false;

	if (r->status == http::responses::ok)
	{
		if (r->content_type.find(http::mimetype::appjson) != std::string::npos) {

			json j;

			// Parse JSON
			//////////////////////////////////////////////
			try
			{
				j = json::parse(r->body.c_str());
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, stringh::s2ws(e.what()));
				return 1;
			}

			// Deserialise data
			//////////////////////////////////////////////
			auto receivedLogArray = new std::map<std::chrono::system_clock::time_point, std::wstring>();

			for (json::iterator i = j.begin(); i != j.end(); ++i)
			{
				std::wstring receivedLog;
				try
				{
					receivedLog = stringh::s2ws(j[i.key()].get<std::string>().c_str());
				}
				catch (const std::exception& e)	{
					messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, stringh::s2ws(e.what()));
					delete receivedLogArray;
					return 1;
				}
				receivedLogArray->insert(std::make_pair(timeh::StringToTimePoint(i.key()), receivedLog));
			}

			messageHandler.ExchangeLogs(receivedLogArray);
		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server cannot get Experiment Log"));
		return 1;
	}

	return 0;
}

/*********************************
// Get Requests / Errors
*********************************/

unsigned CommHandler::GetRequest_req(http_request * r, std::string fromTime)
{
	r->method = http::method::get;
	r->accept = http::mimetype::appjson;
	r->path = "/api/experimentrequests";
	r->params.emplace("time", fromTime);
	return 0;
}

unsigned CommHandler::GetRequest_resp(http_response * r)
{

	flagReqRequest = false;

	if (r->status == http::responses::ok)
	{
		if (r->content_type.find(http::mimetype::appjson) != std::string::npos) {

			json j;

			// Parse JSON
			//////////////////////////////////////////////
			try
			{
				j = json::parse(r->body.c_str());
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, stringh::s2ws(e.what()));
				return 1;
			}

			// Deserialise data
			//////////////////////////////////////////////
			auto receivedReqArray = new std::map<std::chrono::system_clock::time_point, std::wstring>();

			for (json::iterator i = j.begin(); i != j.end(); ++i)
			{
				std::wstring receivedReq;
				try
				{
					receivedReq = stringh::s2ws(j[i.key()].get<std::string>().c_str());
				}
				catch (const std::exception& e) {
					messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, stringh::s2ws(e.what()));
					delete receivedReqArray;
					return 1;
				}
				receivedReqArray->insert(std::make_pair(timeh::StringToTimePoint(i.key()), receivedReq));
			}
			messageHandler.ExchangeRequests(receivedReqArray);
		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server cannot Get Experiment Request"));
		return 1;
	}


	return 0;
}

/*********************************
// Thread Commands
*********************************/
unsigned CommHandler::ThreadCommand_req(http_request * r, int command)
{
	r->method = http::method::post;
	r->path = "/api/thread";

	std::string action;
	switch (command)
	{
	case START:
		action = "start";
		break;
	case SHUTDOWN:
		action = "shutdown";
		break;
	case RESTART:
		action = "restart";
		break;
	case RESET:
		action = "reset";
		break;
	case PAUSE:
		action = "pause";
		break;
	case RESUME:
		action = "resume";
		break;
	default:
		break;
	}

	r->params.emplace("action", action);
	return 0;
}

unsigned CommHandler::ThreadCommand_resp(http_response * r)
{
	if (r->status == http::responses::ok)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Command executed"));
		return 1;
	}
	else if (r->status == http::responses::conflict)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server cannot process thread command"));
		return 1;
	}
	else if (r->status == http::responses::bad_request)
	{

		return 1;
	}
	else if (r->status == http::responses::not_found) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server cannot find thread function"));
		return 1;
	}
	return 0;
}

/*********************************
// Functionality Commands
*********************************/

unsigned CommHandler::FunctionalityCommand_req(http_request* r, int functionality)
{
	r->method = http::method::post;
	r->path = "/api/functionality";

	std::string action;
	switch (functionality)
	{
	case SAMPLEVACUUM:
		action = "cell_vacuum";
		break;
	case BOTTLEVACUUM:
		action = "bottle_vacuum";
		break;
	case BOTTLECHANGE:
		action = "bottle_change";
		break;
	default:
		break;
	}
		
	r->params.emplace("action", action);
	return 0;
}

unsigned CommHandler::FunctionalityCommand_resp(http_response* r)
{
	if (r->status == http::responses::ok)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Functionality requested"));
		return 1;
	}
	else if (r->status == http::responses::conflict)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server cannot process functionality"));
		return 1;
	}
	else if (r->status == http::responses::bad_request)
	{

		return 1;
	}
	else if (r->status == http::responses::not_found) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server cannot find functionality"));
		return 1;
	}
	return 0;
}


/*********************************
// User Choice
*********************************/

unsigned CommHandler::UserChoice_req(http_request* r, int choice)
{
	r->method = http::method::post;
	r->path = "/api/input";
	r->params.emplace("i", choice);
	return 0;
}

unsigned CommHandler::UserChoice_resp(http_response* r)
{
	if (r->status == http::responses::ok)
	{
		return 1;
	}
	else if (r->status == http::responses::conflict)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server cannot process functionality"));
		return 1;
	}
	else if (r->status == http::responses::bad_request)
	{

		return 1;
	}
	else if (r->status == http::responses::not_found) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server cannot find functionality"));
		return 1;
	}
	return 0;
}