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
void CommHandler::Sync(bool initialSync, std::string fromTimeES, std::string fromTimeMS, std::string fromTimeCS)
{
	if (!flagSyncRequest) {

		flagSyncRequest = true;

		if (initialSync)
		{
			sync = 6;

			GetMachineSettings();
			GetControlInstrumentState();
			GetExperimentSettings();
			GetData();
			GetLog();
			GetRequests();
		}
		else
		{
			localExperimentSettingsTime = fromTimeES;
			localMachineSettingsTime = fromTimeMS;
			localControlStateTime = fromTimeCS;

			auto request = std::bind(&CommHandler::Sync_req, this, std::placeholders::_1);
			auto callback = std::bind(&CommHandler::Sync_resp, this, std::placeholders::_1);

			try {
				client.Request(request, callback, localAddress);
			}
			catch (const std::exception& e) {
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, UnicodeConv::s2ws(e.what()));
			}
		}
	}
}

unsigned int CommHandler::CheckSync()
{
	// Confirm sync
	if (sync < 0) {
		return 0;
	}
	else if (sync == 0) {
		messageHandler.SyncComplete();
		flagSyncRequest = false;
		--sync;
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
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server not found"));
		return 1;
	}
	else if(r->disconnected_)
	{
		messageHandler.Disconnection();
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server disconnected"));
		return 1;
	}

	return 0;
}


/*********************************
// Sync
*********************************/
unsigned CommHandler::Sync_req(http_request* r) {
	r->method_ = http::method::get;
	r->accept_ = http::mimetype::appjson;
	r->path_ = "/api/sync";

	r->params_.emplace("MS", localMachineSettingsTime);
	r->params_.emplace("ES", localExperimentSettingsTime);
	r->params_.emplace("CS", localControlStateTime);

	return 0;
}

unsigned CommHandler::Sync_resp(http_response* r) {

	flagSyncRequest = false;

	if (r->status_ == http::responses::ok)
	{
		// Parse JSON
		//////////////////////////////////////////////
		json j;
		try
		{
			j = json::parse(r->answer_.c_str());
		}
		catch (const std::exception& e)
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, UnicodeConv::s2ws(e.what()));
			return 1;
		}

		// Check changes and request updates if needed
		//////////////////////////////////////////////
		if (j["MS"] == false) {
			GetMachineSettings();
		}
		if (j["ES"] == false) {
			GetExperimentSettings();
		}
		if (j["CS"] == false) {
			GetControlInstrumentState();
		}

	}
	else if (r->status_ == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server not found"));
		return 1;
	}
	else if (r->disconnected_)
	{
		messageHandler.Disconnection();
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server disconnected"));
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
		if (r->content_type_.find(http::mimetype::appjson) != std::string::npos) {

			// Parse JSON
			//////////////////////////////////////////////
			json j;
			try
			{
				j = json::parse(r->answer_.c_str());
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, UnicodeConv::s2ws(e.what()));
				return 1;
			}

			// Deserisalise data
			//////////////////////////////////////////////
			MachineSettings receivedSettings;

			try
			{
				receivedSettings = j;
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, UnicodeConv::s2ws(e.what()));
				return 1;
			}

			messageHandler.ExchangeMachineSettings(receivedSettings);

			// Confirm sync
			CheckSync();
		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status_ == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server not found"));
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
		j = *localMachineSettings;
	}
	catch (const std::exception& e)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, UnicodeConv::s2ws(e.what()));
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
	}
	else if (r->status_ == http::responses::internal_err)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server error, could not update settings"));
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
		if (r->content_type_.find(http::mimetype::appjson) != std::string::npos) {

			// Parse JSON
			//////////////////////////////////////////////
			json j;
			try
			{
				j = json::parse(r->answer_.c_str());
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, UnicodeConv::s2ws(e.what()));
				return 1;
			}

			// Deserisalise data
			//////////////////////////////////////////////
			ExperimentSettings receivedSettings;

			try
			{
				receivedSettings = j;
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, UnicodeConv::s2ws(e.what()));
				return 1;
			}

			messageHandler.ExchangeExperimentSettings(receivedSettings);

			// Confirm sync
			CheckSync();
		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status_ == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server not found"));
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
		j = *localExperimentSettings;
	}
	catch (const std::exception& e)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, UnicodeConv::s2ws(e.what()));
		return 1;
	}

	r->entity_ = j.dump();

	return 0;
}

unsigned CommHandler::SetExperimentSettings_resp(http_response* r) {

	if (r->status_ == http::responses::ok)
	{
		localExperimentSettings.reset();
		messageHandler.OnSetExperimentSettings();
	}
	else if (r->status_ == http::responses::internal_err)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server error, could not start a new experiment"));
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
			j = json::parse(r->answer_.c_str());
		}
		catch (const std::exception& e)
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, UnicodeConv::s2ws(e.what()));
			return 1;
		}

		ControlInstrumentState instrumentState;

		// Deserialise Data
		//////////////////////////////////////////////
		try
		{
			instrumentState = j;
		}
		catch (const std::exception& e)
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, UnicodeConv::s2ws(e.what()));
			return 1;
		}

		messageHandler.ExchangeControlState(instrumentState);

		// Confirm sync
		CheckSync();
	}
	else if (r->status_ == http::responses::not_found) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server not found"));
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
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server cannot process thread command"));
		return 1;
	}
	else if (r->status_ == http::responses::bad_request)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Bad request"));
		return 1;
	}
	else if (r->status_ == http::responses::not_found) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server not found"));
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

	flagExperimentRequest = false;

	if (r->status_ == http::responses::ok)
	{
		if (r->content_type_.find(http::mimetype::appjson) != std::string::npos) {
			
			json j;

			// Parse JSON
			//////////////////////////////////////////////
			try
			{
				j = json::parse(r->answer_.c_str());
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, UnicodeConv::s2ws(e.what()));
				return 1;
			}

			// Deserialise Data
			//////////////////////////////////////////////
			auto receivedDataArray = new ExperimentDataStorageArray();

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
					messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, UnicodeConv::s2ws(e.what()));
					delete receivedDataArray;
					return 1;
				}
				receivedDataArray->insert(std::make_pair(timeh::StringToTimePoint(i.key()), receivedData));
			}																													 

			messageHandler.ExchangeData(receivedDataArray);

			// Confirm sync
			CheckSync();
		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status_ == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server not found"));
		return 1;
	}
	else if (r->disconnected_)
	{
		messageHandler.Disconnection();
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server disconnected"));
	}

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
	
	flagLogsRequest = false;

	if (r->status_ == http::responses::ok)
	{
		if (r->content_type_.find(http::mimetype::appjson) != std::string::npos) {

			json j;

			// Parse JSON
			//////////////////////////////////////////////
			try
			{
				j = json::parse(r->answer_.c_str());
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, UnicodeConv::s2ws(e.what()));
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
					messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, UnicodeConv::s2ws(e.what()));
					delete receivedLogArray;
					return 1;
				}
				receivedLogArray->insert(std::make_pair(timeh::StringToTimePoint(i.key()), receivedLog));
			}

			messageHandler.ExchangeLogs(receivedLogArray);

			// Confirm sync
			CheckSync();
		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status_ == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server not found"));
		return 1;
	}

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

	flagReqRequest = false;

	if (r->status_ == http::responses::ok)
	{
		if (r->content_type_.find(http::mimetype::appjson) != std::string::npos) {

			json j;

			// Parse JSON
			//////////////////////////////////////////////
			try
			{
				j = json::parse(r->answer_.c_str());
			}
			catch (const std::exception& e)
			{
				messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, UnicodeConv::s2ws(e.what()));
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
					messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, UnicodeConv::s2ws(e.what()));
					delete receivedReqArray;
					return 1;
				}
				receivedReqArray->insert(std::make_pair(timeh::StringToTimePoint(i.key()), receivedReq));
			}
			messageHandler.ExchangeRequests(receivedReqArray);

			// Confirm sync
			CheckSync();
		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Corrupt response format"));
			return 1;
		}
	}
	else if (r->status_ == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server not found"));
		return 1;
	}


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
		messageHandler.SyncComplete();
		return 1;
	}
	else if (r->status_ == http::responses::conflict)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server cannot process thread command"));
		return 1;
	}
	else if (r->status_ == http::responses::bad_request)
	{

		return 1;
	}
	else if (r->status_ == http::responses::not_found) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server not found"));
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
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server cannot process functionality"));
		return 1;
	}
	else if (r->status_ == http::responses::bad_request)
	{

		return 1;
	}
	else if (r->status_ == http::responses::not_found) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, false, _T("Server not found"));
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