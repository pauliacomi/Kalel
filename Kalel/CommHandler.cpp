#include "stdafx.h"
#include "CommHandler.h"

#include "../Kalel Shared/Netcode/http_request.h"
#include "../Kalel Shared/Netcode/json.hpp"
#include "../Kalel Shared/Resources/StringTable.h"
#include "../Kalel Shared/Resources/DefineInstruments.h"
#include "../Kalel Shared/Com Classes/Serialization.h"
#include "../Kalel Shared/Com Classes/ControlInstrumentState.h"
#include "../Kalel Shared/Netcode/stdHelpers.h"
#include "../Kalel Shared/unicodeConv.h"
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
			client.Request(request, callback, UnicodeConv::ws2s(address.c_str()));
		}
		catch (const std::exception& e)	{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK,false, UnicodeConv::s2ws(e.what()));
		}
	}
}

void CommHandler::SaveAddress(std::wstring address)
{
	localAddress = UnicodeConv::ws2s(address.c_str());
}

/*********************************
// Sync
*********************************/
void CommHandler::Sync()
{
	sync = 6;

	GetMachineSettings();
	GetControlInstrumentState();
	GetExperimentSettings();
	GetData();
	GetLog();
	GetRequests();
}

unsigned int CommHandler::CheckSync()
{
	// Confirm sync
	if (sync < 0) {
		return 0;
	}
	else if (sync == 0) {
		messageHandler.SyncComplete();
	}
	else
		--sync;

	return 1;
}

/*********************************
// MachineSettings
*********************************/
void CommHandler::GetMachineSettings()
{
	auto request = std::bind(&CommHandler::GetMachineSettings_req, this, std::placeholders::_1);
	auto callback = std::bind(&CommHandler::GetMachineSettings_resp, this, std::placeholders::_1);

	try	{
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e)	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, UnicodeConv::s2ws(e.what()));
	}
}

void CommHandler::SetMachineSettings(std::shared_ptr<const MachineSettings> ptr)
{
	localMachineSettings = ptr;

	auto request = std::bind(&CommHandler::SetMachineSettings_req, this, std::placeholders::_1);
	auto callback = std::bind(&CommHandler::SetMachineSettings_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, UnicodeConv::s2ws(e.what()));
	}
}


/*********************************
// ExperimentSettings
*********************************/
void CommHandler::GetExperimentSettings()
{
	auto request = std::bind(&CommHandler::GetExperimentSettings_req, this, std::placeholders::_1);
	auto callback = std::bind(&CommHandler::GetExperimentSettings_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, UnicodeConv::s2ws(e.what()));
	}
}

void CommHandler::SetExperimentSettings(std::shared_ptr<const ExperimentSettings> ptr)
{
	localExperimentSettings = ptr;

	auto request = std::bind(&CommHandler::SetExperimentSettings_req, this, std::placeholders::_1);
	auto callback = std::bind(&CommHandler::SetExperimentSettings_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, UnicodeConv::s2ws(e.what()));
	}
}


/*********************************
// Instrument State / Instrument control
*********************************/
void CommHandler::GetControlInstrumentState()
{
	auto request = std::bind(&CommHandler::GetInstrumentState_req, this, std::placeholders::_1);
	auto callback = std::bind(&CommHandler::GetInstrumentState_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, UnicodeConv::s2ws(e.what()));
	}
}


void CommHandler::ManualCommand(int instrumentType, int instrumentNumber, bool shouldBeActivated)
{
	localInstrumentState.instrumentType = instrumentType;
	localInstrumentState.instrumentNumber = instrumentNumber;
	localInstrumentState.instrumentState = shouldBeActivated;

	auto request = std::bind(&CommHandler::SetInstrumentState_req, this, std::placeholders::_1);
	auto callback = std::bind(&CommHandler::SetInstrumentState_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, UnicodeConv::s2ws(e.what()));
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

		localExperimentTime = fromTime;

		auto request = std::bind(&CommHandler::GetData_req, this, std::placeholders::_1);
		auto callback = std::bind(&CommHandler::GetData_resp, this, std::placeholders::_1);

		try {
			client.Request(request, callback, localAddress);
		}
		catch (const std::exception& e) {
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, UnicodeConv::s2ws(e.what()));
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

		localLogsTime = fromTime;

		auto request = std::bind(&CommHandler::GetLogs_req, this, std::placeholders::_1);
		auto callback = std::bind(&CommHandler::GetLogs_resp, this, std::placeholders::_1);

		try {
			client.Request(request, callback, localAddress);
		}
		catch (const std::exception& e) {
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, UnicodeConv::s2ws(e.what()));
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

		localReqTime = fromTime;

		auto request = std::bind(&CommHandler::GetRequest_req, this, std::placeholders::_1);
		auto callback = std::bind(&CommHandler::GetRequest_resp, this, std::placeholders::_1);

		try {
			client.Request(request, callback, localAddress);
		}
		catch (const std::exception& e) {
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, UnicodeConv::s2ws(e.what()));
		}
	}
}


/*********************************
// Automation control
*********************************/
void CommHandler::StartClient()
{
	localThreadCommand = START;
	ThreadCommand();
}

void CommHandler::ShutdownClient()
{
	localThreadCommand = SHUTDOWN;
	ThreadCommand();
}

void CommHandler::RestartClient()
{
	localThreadCommand = RESTART;
	ThreadCommand();
}

void CommHandler::ResetClient()
{
	localThreadCommand = RESET;
	ThreadCommand();
}

void CommHandler::PauseClient()
{
	localThreadCommand = PAUSE;
	ThreadCommand();
}

void CommHandler::ResumeClient()
{
	localThreadCommand = RESUME;
	ThreadCommand();
}

void CommHandler::SetUserContinue()
{
}

void CommHandler::ThreadCommand()
{
	auto request = std::bind(&CommHandler::ThreadCommand_req, this, std::placeholders::_1);
	auto callback = std::bind(&CommHandler::ThreadCommand_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, UnicodeConv::s2ws(e.what()));
	}
}

void CommHandler::FunctionSampleVacuum()
{
	localFunctionalityCommand = SAMPLEVACUUM;
	FunctionalityCommand();
}

void CommHandler::FunctionBottleVacuum()
{
	localFunctionalityCommand = BOTTLEVACUUM;
	FunctionalityCommand();
}

void CommHandler::FunctionChangeBottle()
{
	localFunctionalityCommand = BOTTLECHANGE;
	FunctionalityCommand();
}


void CommHandler::FunctionalityCommand()
{
	auto request = std::bind(&CommHandler::FunctionalityCommand_req, this, std::placeholders::_1);
	auto callback = std::bind(&CommHandler::FunctionalityCommand_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, UnicodeConv::s2ws(e.what()));
	}
}

/*********************************
// Debugging
*********************************/
void CommHandler::TestConn()
{
	auto request = std::bind(&CommHandler::TestConn_req, this, std::placeholders::_1);
	auto callback = std::bind(&CommHandler::TestConn_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, UnicodeConv::s2ws(e.what()));
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
	r->method_ = http::method::get;
	r->path_ = "/api/handshake";
	return 0;
}

unsigned CommHandler::Handshake_resp(http_response* r) {

	if (r->status_ == http::responses::ok)
	{
		messageHandler.ConnectionComplete();
	}
	else if (r->status_ == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server not found"));
		return 1;
	}
	else if(r->disconnected_)
	{
		messageHandler.Disconnection();
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server disconnected"));
	}

	return 0;
}


/*********************************
// Get MachineSettings
*********************************/
unsigned CommHandler::GetMachineSettings_req(http_request* r) {
	r->method_ = http::method::get;
	r->accept_ = http::mimetype::appjson;
	r->path_ = "/api/machinesettings";
	return 0;
}

unsigned CommHandler::GetMachineSettings_resp(http_response* r) {

	if (r->status_ == http::responses::ok)
	{
		if (r->content_type_ == http::mimetype::appjson) {

			// Parse JSON
			//////////////////////////////////////////////
			json j;
			try
			{
				j = json::parse(r->answer_);
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, UnicodeConv::s2ws(e.what()));
				return 1;
			}

			// Deserisalise data
			//////////////////////////////////////////////
			MachineSettings receivedSettings;

			try
			{
				serialization::deserializeJSONtoMachineSettings(j, receivedSettings);
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, UnicodeConv::s2ws(e.what()));
				return 1;
			}

			messageHandler.ExchangeMachineSettings(receivedSettings);

			// Confirm sync
			CheckSync();
		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status_ == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server not found"));
		return 1;
	}

	return 0;
}


/*********************************
// Set MachineSettings
*********************************/
unsigned CommHandler::SetMachineSettings_req(http_request* r) {
	r->method_			= http::method::post;
	r->content_type_	= http::mimetype::appjson;
	r->path_			= "/api/machinesettings";

	json j;
	try
	{
		serialization::serializeMachineSettingsToJSON(*localMachineSettings, j);
	}
	catch (const std::exception& e)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, UnicodeConv::s2ws(e.what()));
		return 1;
	}

	r->entity_ = j.dump();

	return 0;
}

unsigned CommHandler::SetMachineSettings_resp(http_response* r) {
	
	if (r->status_ == http::responses::ok)
	{
		localMachineSettings.reset();
		messageHandler.OnSetMachineSettings();
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Machine settings updated"));
	}
	else if (r->status_ == http::responses::internal_err)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server error, could not update settings"));
		return 1;
	}

	return 0;
}


/*********************************
// Get ExperimentSettings
*********************************/
unsigned CommHandler::GetExperimentSettings_req(http_request* r) {
	r->method_ = http::method::get;
	r->accept_ = http::mimetype::appjson;
	r->path_ = "/api/experimentsettings";
	return 0;
}

unsigned CommHandler::GetExperimentSettings_resp(http_response* r) {

	if (r->status_ == http::responses::ok)
	{
		if (r->content_type_ == http::mimetype::appjson) {

			// Parse JSON
			//////////////////////////////////////////////
			json j;
			try
			{
				j = json::parse(r->answer_);
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, UnicodeConv::s2ws(e.what()));
				return 1;
			}

			// Deserisalise data
			//////////////////////////////////////////////
			ExperimentSettings receivedSettings;

			try
			{
				serialization::deserializeJSONtoExperimentSettings(j, receivedSettings);
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, UnicodeConv::s2ws(e.what()));
				return 1;
			}

			messageHandler.ExchangeExperimentSettings(receivedSettings);

			// Confirm sync
			CheckSync();
		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status_ == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server not found"));
		return 1;
	}

	return 0;
}


/*********************************
// Set ExperimentSettings
*********************************/
unsigned CommHandler::SetExperimentSettings_req(http_request* r) {
	r->method_ = http::method::post;
	r->content_type_ = http::mimetype::appjson;
	r->path_ = "/api/experimentsettings";

	json j;
	try
	{
		serialization::serializeExperimentSettingsToJSON(*localExperimentSettings, j);
	}
	catch (const std::exception& e)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, UnicodeConv::s2ws(e.what()));
		return 1;
	}

	r->entity_ = j.dump();

	return 0;
}

unsigned CommHandler::SetExperimentSettings_resp(http_response* r) {

	if (r->status_ == http::responses::ok)
	{
		messageHandler.OnSetExperimentSettings();
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Experiment settings updated"));
	}
	else if (r->status_ == http::responses::internal_err)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server error, could not start a new experiment"));
		return 1;
	}

	return 0;
}


/*********************************
// Get machine Instrument State
*********************************/
unsigned CommHandler::GetInstrumentState_req(http_request * r)
{
	r->method_ = http::method::get;
	r->path_ = "/api/instrument";

	return 0;
}

unsigned CommHandler::GetInstrumentState_resp(http_response * r)
{
	if (r->status_ == http::responses::ok)
	{
		json j;

		// Parse JSON
		//////////////////////////////////////////////
		try
		{
			j = json::parse(r->answer_);
		}
		catch (const std::exception& e)
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, UnicodeConv::s2ws(e.what()));
			return 1;
		}

		ControlInstrumentState instrumentState;

		// Deserialise Data
		//////////////////////////////////////////////
		try
		{
			serialization::deserializeJSONtoControlInstrumentState(j, instrumentState);
		}
		catch (const std::exception& e)
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, UnicodeConv::s2ws(e.what()));
			return 1;
		}

		messageHandler.ExchangeControlState(instrumentState);

		// Confirm sync
		CheckSync();
	}
	else if (r->status_ == http::responses::not_found) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server not found"));
		return 1;
	}
	return 0;
}


/*********************************
// Set machine Instrument State
*********************************/
unsigned CommHandler::SetInstrumentState_req(http_request * r)
{
	r->method_ = http::method::post;
	r->path_ = "/api/instrument";

	std::string localInstrumentType;
	std::string localInstrumentNumber;
	std::string localShouldBeActivated;

	switch (localInstrumentState.instrumentType)
	{
	case INSTRUMENT_VALVE:
		localInstrumentType = "valve";
		break;

	case INSTRUMENT_EV:
		localInstrumentType = "ev";
		break;

	case INSTRUMENT_PUMP:
		localInstrumentType = "pump";
		break;
	default:
		break;
	}

	localInstrumentNumber = std::to_string(localInstrumentState.instrumentNumber);
	localShouldBeActivated = std::to_string(localInstrumentState.instrumentState);

	r->params_.emplace("type", localInstrumentType);
	r->params_.emplace("number", localInstrumentNumber);
	r->params_.emplace("active", localShouldBeActivated);

	return 0;
}

unsigned CommHandler::SetInstrumentState_resp(http_response * r)
{
	if (r->status_ == http::responses::ok)
	{
		messageHandler.ExchangeControlStateSpecific(localInstrumentState);

		return 1;
	}
	else if (r->status_ == http::responses::conflict)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server cannot process thread command"));
		return 1;
	}
	else if (r->status_ == http::responses::bad_request)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Bad request"));
		return 1;
	}
	else if (r->status_ == http::responses::not_found) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server not found"));
		return 1;
	}
	return 0;
}


/*********************************
// Get Data
*********************************/
unsigned CommHandler::GetData_req(http_request* r) {
	r->method_ = http::method::get;
	r->accept_ = http::mimetype::appjson;
	r->path_ = "/api/experimentdata";
	r->params_.emplace("time", localExperimentTime);
	return 0;
}

unsigned CommHandler::GetData_resp(http_response* r) {

	if (r->status_ == http::responses::ok)
	{
		if (r->content_type_ == http::mimetype::appjson) {
			
			json j;

			// Parse JSON
			//////////////////////////////////////////////
			try
			{
				j = json::parse(r->answer_);
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, UnicodeConv::s2ws(e.what()));
				flagExperimentRequest = false;
				return 1;
			}

			// Deserialise Data
			//////////////////////////////////////////////
			std::shared_ptr<ExperimentData> receivedData = nullptr;
			auto receivedDataArray = new ExperimentDataStorageArray();

			for (json::iterator i = j.begin(); i != j.end(); ++i)
			{
				receivedData = std::make_shared<ExperimentData>();
				json j2 = j[i.key()];
				try
				{
					serialization::deserializeJSONtoExperimentData(j2, *receivedData);
				}
				catch (const std::exception& e)
				{
					messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, UnicodeConv::s2ws(e.what()));
					delete receivedDataArray;
					flagExperimentRequest = false;
					return 1;
				}
				receivedDataArray->insert(std::make_pair(StringToTimePoint(i.key()), receivedData));
			}																													 

			messageHandler.ExchangeData(receivedDataArray);

			// Confirm sync
			CheckSync();
		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Corrupt response format"));
			flagExperimentRequest = false;
			return 1;
		}
	}
	else if (r->status_ == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server not found"));
		flagExperimentRequest = false;
		return 1;
	}
	else if (r->disconnected_)
	{
		messageHandler.Disconnection();
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server disconnected"));
	}


	flagExperimentRequest = false;
	return 0;
}


/*********************************
// Get Logs
*********************************/
unsigned CommHandler::GetLogs_req(http_request * r)
{
	r->method_ = http::method::get;
	r->accept_ = http::mimetype::appjson;
	r->path_ = "/api/experimentlogs";
	r->params_.emplace("time", localLogsTime);
	return 0;
}

unsigned CommHandler::GetLogs_resp(http_response * r)
{
	if (r->status_ == http::responses::ok)
	{
		if (r->content_type_ == http::mimetype::appjson) {

			json j;

			// Parse JSON
			//////////////////////////////////////////////
			try
			{
				j = json::parse(r->answer_);
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, UnicodeConv::s2ws(e.what()));
				flagLogsRequest = false;
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
					receivedLog = UnicodeConv::s2ws(j[i.key()].get<std::string>().c_str());
				}
				catch (const std::exception& e)	{
					messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, UnicodeConv::s2ws(e.what()));
					delete receivedLogArray;
					flagLogsRequest = false;
					return 1;
				}
				receivedLogArray->insert(std::make_pair(StringToTimePoint(i.key()), receivedLog));
			}

			messageHandler.ExchangeLogs(receivedLogArray);

			// Confirm sync
			CheckSync();
		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Corrupt response format"));
			flagLogsRequest = false;
			return 1;
		}
	}
	else if (r->status_ == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server not found"));
		flagLogsRequest = false;
		return 1;
	}

	flagLogsRequest = false;

	return 0;
}

/*********************************
// Get Requests / Errors
*********************************/

unsigned CommHandler::GetRequest_req(http_request * r)
{
	r->method_ = http::method::get;
	r->accept_ = http::mimetype::appjson;
	r->path_ = "/api/experimentrequests";
	r->params_.emplace("time", localReqTime);
	return 0;
}

unsigned CommHandler::GetRequest_resp(http_response * r)
{
	if (r->status_ == http::responses::ok)
	{
		if (r->content_type_ == http::mimetype::appjson) {

			json j;

			// Parse JSON
			//////////////////////////////////////////////
			try
			{
				j = json::parse(r->answer_);
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, UnicodeConv::s2ws(e.what()));
				flagReqRequest = false;
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
					receivedReq = UnicodeConv::s2ws(j[i.key()].get<std::string>().c_str());
				}
				catch (const std::exception& e) {
					messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, UnicodeConv::s2ws(e.what()));
					delete receivedReqArray;
					flagReqRequest = false;
					return 1;
				}
				receivedReqArray->insert(std::make_pair(StringToTimePoint(i.key()), receivedReq));
			}
			messageHandler.ExchangeRequests(receivedReqArray);

			// Confirm sync
			CheckSync();
		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Corrupt response format"));
			flagReqRequest = false;
			return 1;
		}
	}
	else if (r->status_ == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server not found"));
		flagReqRequest = false;
		return 1;
	}

	flagReqRequest = false;

	return 0;
}

/*********************************
// Thread Commands
*********************************/
unsigned CommHandler::ThreadCommand_req(http_request * r)
{
	r->method_ = http::method::post;
	r->path_ = "/api/thread";

	std::string action;
	switch (localThreadCommand)
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

	r->params_.emplace("action", action);
	return 0;
}

unsigned CommHandler::ThreadCommand_resp(http_response * r)
{
	if (r->status_ == http::responses::ok)
	{

		return 1;
	}
	else if (r->status_ == http::responses::conflict)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server cannot process thread command"));
		return 1;
	}
	else if (r->status_ == http::responses::bad_request)
	{

		return 1;
	}
	else if (r->status_ == http::responses::not_found) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server not found"));
		return 1;
	}
	return 0;
}

unsigned CommHandler::FunctionalityCommand_req(http_request* r)
{
	r->method_ = http::method::post;
	r->path_ = "/api/functionality";

	std::string action;
	switch (localFunctionalityCommand)
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
		
	r->params_.emplace("action", action);
	return 0;
}

unsigned CommHandler::FunctionalityCommand_resp(http_response* r)
{
	if (r->status_ == http::responses::ok)
	{

		return 1;
	}
	else if (r->status_ == http::responses::conflict)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server cannot process functionality"));
		return 1;
	}
	else if (r->status_ == http::responses::bad_request)
	{

		return 1;
	}
	else if (r->status_ == http::responses::not_found) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server not found"));
		return 1;
	}
	return 0;
}

/*********************************
// Debugging
*********************************/

unsigned CommHandler::TestConn_req(http_request* r) {
	r->method_ = http::method::get;
	r->path_ = "/api/debug/testconnection";
	r->params_.emplace("return", "500");
	return 0;
}

unsigned CommHandler::TestConn_resp(http_response* r) {

	if (r->status_ == http::responses::ok)
	{
		debug_success++;
	}
	else if (r->status_ == http::responses::bad_request)
	{
		debug_fails++;
	}
	
	std::wstring s = _T("Success: ") + std::to_wstring(debug_success) ;
	s += _T(" Fail: ") + std::to_wstring(debug_fails);
	messageHandler.DisplayMessage(GENERIC_STRING, s);

	return 0;
}