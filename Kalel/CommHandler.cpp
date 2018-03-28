#include "stdafx.h"
#include "CommHandler.h"

// NetCode
#include "../Kalel Shared/Netcode/http_request.h"
#include "../Kalel Shared/Netcode/json.hpp"

// Resources
#include "../Kalel Shared/Resources/DefineInstruments.h"
#include "../Kalel Shared/Resources/DefineStages.h"

// Com Classes
#include "../Kalel Shared/Com Classes/Serialization.h"
#include "../Kalel Shared/Com Classes/ControlInstrumentState.h"

// Helpers
#include "../Kalel Shared/stringHelpers.h"
#include "../Kalel Shared/timeHelpers.h"

// STD
#include <functional>
#include <map>

using json = nlohmann::json;

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
			client.Request(request, callback, stringh::ws2s(address));
		}
		catch (const std::exception& e)	{
			messageHandler.DisplayMessageBox(MB_ICONERROR | MB_OK, stringh::s2ws(e.what()));
		}
	}
}

void CommHandler::SaveAddress(std::wstring address)
{
	localAddress = stringh::ws2s(address);
}

void CommHandler::SaveAuth(const std::wstring &username, const std::wstring &password)
{
	client.SetCredentials(stringh::ws2s(username), stringh::ws2s(password));
}


/*********************************
// MachineSettings
*********************************/
void CommHandler::GetMachineSettings(const std::chrono::system_clock::time_point &fromTime)
{
	auto request = std::bind(&CommHandler::GetMachineSettings_req, this, std::placeholders::_1, timeh::TimePointToISOString(fromTime));
	auto callback = std::bind(&CommHandler::GetMachineSettings_resp, this, std::placeholders::_1);

	try	{
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e)	{
		messageHandler.DisplayMessageBox(MB_ICONERROR | MB_OK, stringh::s2ws(e.what()));
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
		messageHandler.DisplayMessageBox(MB_ICONERROR | MB_OK, stringh::s2ws(e.what()));
	}
}


/*********************************
// ExperimentSettings
*********************************/
void CommHandler::GetExperimentSettings(const std::chrono::system_clock::time_point &fromTime)
{
	auto request = std::bind(&CommHandler::GetExperimentSettings_req, this, std::placeholders::_1, timeh::TimePointToISOString(fromTime));
	auto callback = std::bind(&CommHandler::GetExperimentSettings_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(MB_ICONERROR | MB_OK, stringh::s2ws(e.what()));
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
		messageHandler.DisplayMessageBox(MB_ICONERROR | MB_OK, stringh::s2ws(e.what()));
	}
}

/*********************************
// ExperimentStatus
*********************************/
void CommHandler::GetExperimentStatus(const std::chrono::system_clock::time_point &fromTime)
{
	auto request = std::bind(&CommHandler::GetExperimentStatus_req, this, std::placeholders::_1, timeh::TimePointToISOString(fromTime));
	auto callback = std::bind(&CommHandler::GetExperimentStatus_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(MB_ICONERROR | MB_OK, stringh::s2ws(e.what()));
	}
}

/*********************************
// Instrument State / Instrument control
*********************************/
void CommHandler::GetControlInstrumentState(const std::chrono::system_clock::time_point &fromTime)
{
	auto request = std::bind(&CommHandler::GetInstrumentState_req, this, std::placeholders::_1, timeh::TimePointToISOString(fromTime));
	auto callback = std::bind(&CommHandler::GetInstrumentState_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(MB_ICONERROR | MB_OK, stringh::s2ws(e.what()));
	}
}

void CommHandler::ManualCommand(int instrumentID, bool instrumentState)
{
	auto request = std::bind(&CommHandler::SetInstrumentState_req, this, std::placeholders::_1, instrumentID, instrumentState);
	auto callback = std::bind(&CommHandler::SetInstrumentState_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(MB_ICONERROR | MB_OK, stringh::s2ws(e.what()));
	}
}


/*********************************
// Data
*********************************/
void CommHandler::GetData(const std::chrono::system_clock::time_point &fromTime)
{
	if (!flagExperimentRequest)
	{
		flagExperimentRequest = true;

		auto request = std::bind(&CommHandler::GetData_req, this, std::placeholders::_1, timeh::TimePointToISOString(fromTime));
		auto callback = std::bind(&CommHandler::GetData_resp, this, std::placeholders::_1);

		try {
			client.Request(request, callback, localAddress);
		}
		catch (const std::exception& e) {
			messageHandler.DisplayMessageBox(MB_ICONERROR | MB_OK, stringh::s2ws(e.what()));
			flagExperimentRequest = false;
		}
	}
}


/*********************************
// Logs
*********************************/
void CommHandler::GetLog(const std::chrono::system_clock::time_point &fromTime)
{
	if (!flagLogsRequest)
	{
		flagLogsRequest = true;

		auto request = std::bind(&CommHandler::GetLogs_req, this, std::placeholders::_1, timeh::TimePointToISOString(fromTime));
		auto callback = std::bind(&CommHandler::GetLogs_resp, this, std::placeholders::_1);

		try {
			client.Request(request, callback, localAddress);
		}
		catch (const std::exception& e) {
			messageHandler.DisplayMessageBox(MB_ICONERROR | MB_OK, stringh::s2ws(e.what()));
			flagLogsRequest = false;
		}
	}
}


/*********************************
// Errors/requests
*********************************/
void CommHandler::GetRequests(const std::chrono::system_clock::time_point &fromTime)
{
	if (!flagReqRequest)
	{
		flagReqRequest = true;

		auto request = std::bind(&CommHandler::GetRequest_req, this, std::placeholders::_1, timeh::TimePointToISOString(fromTime));
		auto callback = std::bind(&CommHandler::GetRequest_resp, this, std::placeholders::_1);

		try {
			client.Request(request, callback, localAddress);
		}
		catch (const std::exception& e) {
			messageHandler.DisplayMessageBox(MB_ICONERROR | MB_OK, stringh::s2ws(e.what()));
			flagReqRequest = false;
		}
	}
}


/*********************************
// Automation control
*********************************/
void CommHandler::StartClient() { ThreadCommand("start"); }

void CommHandler::ShutdownClient() { ThreadCommand("shutdown"); }

void CommHandler::RestartClient() {	ThreadCommand("restart"); }

void CommHandler::ResetClient() { ThreadCommand("reset"); }

void CommHandler::PauseClient() { ThreadCommand("pause"); }

void CommHandler::ResumeClient() { ThreadCommand("resume"); }

void CommHandler::NextCommand() { ThreadCommand("next_command"); }

void CommHandler::NextStep() { ThreadCommand("next_step"); }

void CommHandler::NextSubstep() { ThreadCommand("next_substep"); }

void CommHandler::StopVacuum() { ThreadCommand("stop_vacuum"); }

void CommHandler::ThreadCommand(std::string command)
{
	auto request = std::bind(&CommHandler::ThreadCommand_req, this, std::placeholders::_1, command);
	auto callback = std::bind(&CommHandler::ThreadCommand_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(MB_ICONERROR | MB_OK, stringh::s2ws(e.what()));
	}
}


/*********************************
// User choice
*********************************/
void CommHandler::UserYes(const std::chrono::system_clock::time_point & time) { UserChoice(time, CHOICE_YES); }
void CommHandler::UserNo(const std::chrono::system_clock::time_point & time) { UserChoice(time, CHOICE_NO); }
void CommHandler::UserWait(const std::chrono::system_clock::time_point & time) { UserChoice(time, CHOICE_WAIT); }

void CommHandler::UserChoice(const std::chrono::system_clock::time_point & time, int choice)
{
	auto request = std::bind(&CommHandler::UserChoice_req, this, std::placeholders::_1, timeh::TimePointToISOString(time), choice);
	auto callback = std::bind(&CommHandler::UserChoice_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(MB_ICONERROR | MB_OK, stringh::s2ws(e.what()));
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
		messageHandler.DisplayMessageBox(MB_OK, _T("Server not found"));
		return 1;
	}
	else if(r->disconnected)
	{
		messageHandler.Disconnection();
		messageHandler.DisplayMessageBox(MB_OK, _T("Server disconnected"));
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
	r->params.emplace("t", fromTime);
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
				receivedSettings = new MachineSettings(json::parse(r->body));
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(MB_OK, stringh::s2ws(e.what()));
				return 1;
			}

			messageHandler.ExchangeMachineSettings(receivedSettings);
		}
		else
		{
			messageHandler.DisplayMessageBox(MB_OK, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(MB_OK, _T("Server does not support Machine Settings send"));
		return 1;
	}

	return 0;
}


/*********************************
// Set MachineSettings
*********************************/
unsigned CommHandler::SetMachineSettings_req(http_request* r, MachineSettings ms) {

	r->method		= http::method::post;
	r->content_type	= http::mimetype::appjson;
	r->path			= "/api/machinesettings";

	json j;
	try
	{
		j = ms;
	}
	catch (const std::exception& e)
	{
		messageHandler.DisplayMessageBox(MB_OK, stringh::s2ws(e.what()));
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
		messageHandler.DisplayMessageBox(MB_OK, _T("Server error, could not update settings"));
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
	r->params.emplace("t", fromTime);
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
				receivedSettings = new ExperimentSettings(json::parse(r->body));
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(MB_OK, stringh::s2ws(e.what()));
				return 1;
			}

			messageHandler.ExchangeExperimentSettings(receivedSettings);
		}
		else
		{
			messageHandler.DisplayMessageBox(MB_OK, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(MB_OK, _T("Server cannot get Experiment Settings"));
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

	json j = es;
	r->body = j.dump();

	return 0;
}

unsigned CommHandler::SetExperimentSettings_resp(http_response* r) {

	if (r->status == http::responses::ok)
	{
		messageHandler.OnSetExperimentSettings();
		return 0;
	}
	else if (r->status == http::responses::internal_err)
	{
		messageHandler.DisplayMessageBox(MB_OK, _T("Server error, could not start a new experiment"));
	}
	return 1;
}


/*********************************
// Get ExperimentStatus
*********************************/
unsigned CommHandler::GetExperimentStatus_req(http_request* r, std::string fromTime) {
	r->method = http::method::get;
	r->accept = http::mimetype::appjson;
	r->path = "/api/experimentstatus";
	r->params.emplace("t", fromTime);
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
				receivedStatus = new ExperimentStatus(json::parse(r->body));
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(MB_OK, stringh::s2ws(e.what()));
				return 1;
			}

			messageHandler.ExchangeExperimentStatus(receivedStatus);
		}
		else
		{
			messageHandler.DisplayMessageBox(MB_OK, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(MB_OK, _T("Server cannot get Experiment Status"));
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
	r->params.emplace("t", fromTime);
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
			instrumentState = new ControlInstrumentState();
			json j = json::parse(r->body);
			for (json::iterator i = j.begin(); i != j.end(); ++i)
			{
				instrumentState->emplace(stringh::To<unsigned int>(i.key()), j[i.key()]);
			}
		}
		catch (const std::exception& e)
		{
			messageHandler.DisplayMessageBox(MB_OK, stringh::s2ws(e.what()));
			return 1;
		}
		// Exchange
		messageHandler.ExchangeControlState(instrumentState);
	}
	else if (r->status == http::responses::not_found) {
		messageHandler.DisplayMessageBox(MB_OK, _T("Server cannot get Instrument State"));
		return 1;
	}
	return 0;
}


/*********************************
// Set machine Instrument State
*********************************/
unsigned CommHandler::SetInstrumentState_req(http_request * r, int instrumentID, bool instrumentState)
{
	r->method = http::method::post;
	r->path = "/api/instrument";
	r->params.emplace("ID", std::to_string(instrumentID));
	r->params.emplace("state", std::to_string(instrumentState));

	localInstrumentState.instrumentID = instrumentID;
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
		messageHandler.DisplayMessageBox(MB_OK, _T("Server cannot process thread command"));
		return 1;
	}
	else if (r->status == http::responses::bad_request)
	{
		messageHandler.DisplayMessageBox(MB_OK, _T("Bad request"));
		return 1;
	}
	else if (r->status == http::responses::not_found) {
		messageHandler.DisplayMessageBox(MB_OK, _T("Server cannot set Instrument State"));
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
	r->params.emplace("t", fromTime);
	return 0;
}

unsigned CommHandler::GetData_resp(http_response* r) {

	flagExperimentRequest = false;

	if (r->status == http::responses::ok)
	{
		if (r->content_type.find(http::mimetype::appjson) != std::string::npos) {
			
			// Parse JSON
			//////////////////////////////////////////////
			json j;
			try
			{
				j = json::parse(r->body);
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(MB_OK, stringh::s2ws(e.what()));
				return 1;
			}

			// Deserialise Data
			//////////////////////////////////////////////
			auto receivedDataArray = new std::map<std::chrono::system_clock::time_point, ExperimentData>();

			for (json::iterator i = j.begin(); i != j.end(); ++i)
			{
				try
				{
					receivedDataArray->emplace(timeh::ISOStringToTimePoint(i.key()), j[i.key()]);
				}
				catch (const std::exception& e)
				{
					messageHandler.DisplayMessageBox(MB_OK, stringh::s2ws(e.what()));
					delete receivedDataArray;
					return 1;
				}
			}																													 
			messageHandler.ExchangeData(receivedDataArray);
		}
		else
		{
			messageHandler.DisplayMessageBox(MB_OK, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(MB_OK, _T("Server cannot get Experiment Data"));
		return 1;
	}
	else if (r->disconnected)
	{
		messageHandler.Disconnection();
		messageHandler.DisplayMessageBox(MB_OK, _T("Server disconnected"));
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
	r->params.emplace("t", fromTime);
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
				j = json::parse(r->body);
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(MB_OK, stringh::s2ws(e.what()));
				return 1;
			}

			// Deserialise data
			//////////////////////////////////////////////
			auto receivedLogArray = new std::map<std::chrono::system_clock::time_point, std::wstring>();

			for (json::iterator i = j.begin(); i != j.end(); ++i)
			{
				try
				{
					receivedLogArray->emplace(timeh::ISOStringToTimePoint(i.key()), stringh::s2ws(j[i.key()]));
				}
				catch (const std::exception& e)	{
					messageHandler.DisplayMessageBox(MB_OK, stringh::s2ws(e.what()));
					delete receivedLogArray;
					return 1;
				}
			}

			messageHandler.ExchangeLogs(receivedLogArray);
		}
		else
		{
			messageHandler.DisplayMessageBox(MB_OK, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(MB_OK, _T("Server cannot get Experiment Log"));
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
	r->params.emplace("t", fromTime);
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
				j = json::parse(r->body);
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(MB_OK, stringh::s2ws(e.what()));
				return 1;
			}

			// Deserialise data
			//////////////////////////////////////////////
			auto receivedReqArray = new std::map<std::chrono::system_clock::time_point, std::wstring>();

			for (json::iterator i = j.begin(); i != j.end(); ++i)
			{
				try
				{
					receivedReqArray->emplace(timeh::ISOStringToTimePoint(i.key()), stringh::s2ws(j[i.key()]));
				}
				catch (const std::exception& e) {
					messageHandler.DisplayMessageBox(MB_OK, stringh::s2ws(e.what()));
					delete receivedReqArray;
					return 1;
				}
			}
			messageHandler.ExchangeRequests(receivedReqArray);
		}
		else
		{
			messageHandler.DisplayMessageBox(MB_OK, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(MB_OK, _T("Server cannot Get Experiment Request"));
		return 1;
	}

	return 0;
}

/*********************************
// Thread Commands
*********************************/
unsigned CommHandler::ThreadCommand_req(http_request * r, std::string command)
{
	r->method = http::method::post;
	r->path = "/api/thread";
	r->params.emplace("action", command);
	return 0;
}

unsigned CommHandler::ThreadCommand_resp(http_response * r)
{
	if (r->status == http::responses::ok)
	{
		messageHandler.DisplayMessageBox(MB_OK, _T("Command executed"));
		return 1;
	}
	else if (r->status == http::responses::conflict)
	{
		messageHandler.DisplayMessageBox(MB_OK, _T("Server cannot process thread command"));
		return 1;
	}
	else if (r->status == http::responses::bad_request)
	{

		return 1;
	}
	else if (r->status == http::responses::not_found) {
		messageHandler.DisplayMessageBox(MB_OK, _T("Server cannot find thread function"));
		return 1;
	}
	return 0;
}


/*********************************
// User Choice
*********************************/
unsigned CommHandler::UserChoice_req(http_request* r, std::string time, int choice)
{
	r->method = http::method::post;
	r->path = "/api/input";
	r->params.emplace("t", time);
	r->params.emplace("i", std::to_string(choice));
	return 0;
}

unsigned CommHandler::UserChoice_resp(http_response* r)
{
	if (r->status == http::responses::ok)
	{
		return 0;
	}
	else if (r->status == http::responses::conflict)
	{
		messageHandler.DisplayMessageBox(MB_OK, _T("Server does not recognise choice"));
	}
	else if (r->status == http::responses::not_found) {
		messageHandler.DisplayMessageBox(MB_OK, _T("Server cannot find choice"));
	}
	return 1;
}