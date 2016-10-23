#include "stdafx.h"
#include "CommHandler.h"

#include "Com Classes/ManualActionParam.h"
#include "Netcode/http_request.h"
#include "Netcode/json.hpp"
#include "Resources/StringTable.h"
#include "Resources/DefineInstruments.h"
#include "Com Classes/Serialization.h"
#include "unicodeConv.h"

#include <functional>

using json = nlohmann::json;

#define START		1
#define SHUTDOWN	2
#define RESTART		3
#define RESET		4
#define PAUSE		5
#define RESUME		6

CommHandler::CommHandler()
{
}


CommHandler::~CommHandler()
{
}

void CommHandler::SetHandle(HWND h){
	messageHandler.setHandle(h);
}

void CommHandler::Connect(std::wstring address)
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

void CommHandler::SaveAddress(std::wstring address)
{
	localAddress = UnicodeConv::ws2s(address.c_str());
}

void CommHandler::Sync()
{
	GetMachineSettings();
	GetData();
	GetLog();
}

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


void CommHandler::GetData(time_t startTime, long int measurementsMade)
{
	if (measurementsMade == 0)
	{
		localExperimentStartTime = "";
		localMeasurementsMade = "";
	}
	else
	{
		localExperimentStartTime = std::to_string(startTime);
		localMeasurementsMade = std::to_string(measurementsMade);
	}

	auto request = std::bind(&CommHandler::GetData_req, this, std::placeholders::_1);
	auto callback = std::bind(&CommHandler::GetData_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, UnicodeConv::s2ws(e.what()));
	}
}



void CommHandler::GetLog(time_t fromTime)
{
	auto request = std::bind(&CommHandler::GetLogs_req, this, std::placeholders::_1);
	auto callback = std::bind(&CommHandler::GetLogs_resp, this, std::placeholders::_1);

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


void CommHandler::ManualCommand(int instrumentType, int instrumentNumber, bool shouldBeActivated)
{
	switch (instrumentType)
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
	
	localInstrumentNumber = instrumentNumber;
	localShouldBeActivated = shouldBeActivated;
	
	auto request = std::bind(&CommHandler::GetLogs_req, this, std::placeholders::_1);
	auto callback = std::bind(&CommHandler::GetLogs_resp, this, std::placeholders::_1);

	try {
		client.Request(request, callback, localAddress);
	}
	catch (const std::exception& e) {
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, UnicodeConv::s2ws(e.what()));
	}
}

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


/**********************************************************************************************************************************
// Request and response functions
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

			receivedSettings.synced	= true;
			messageHandler.GotMachineSettings(receivedSettings);
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
// Get Data
*********************************/
unsigned CommHandler::GetData_req(http_request* r) {
	r->method_ = http::method::get;
	r->accept_ = http::mimetype::appjson;
	r->path_ = "/api/experimentdata";
	r->params_.emplace("start", localExperimentStartTime);
	r->params_.emplace("measurements", localMeasurementsMade);
	return 0;
}

unsigned CommHandler::GetData_resp(http_response* r) {

	if (r->status_ == http::responses::ok)
	{
		if (r->content_type_ == http::mimetype::appjson) {
			
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

			ExperimentData * receivedData = nullptr;
			MeasurementsArray * receivedDataArray = new MeasurementsArray();

			for (json::iterator i = j.begin(); i != j.end(); ++i)
			{
				receivedData = new ExperimentData();
				json j2 = j[i.key()];
				try
				{
					serialization::deserializeJSONtoExperimentData(j2, *receivedData);
				}
				catch (const std::exception& e)
				{
					messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, UnicodeConv::s2ws(e.what()));
					delete receivedData;
					delete receivedDataArray;
					return 1;
				}
				receivedDataArray->push_back(receivedData);
			}																													 

			messageHandler.ExchangeData(receivedDataArray);

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
// Get Logs
*********************************/
unsigned CommHandler::GetLogs_req(http_request * r)
{
	r->method_ = http::method::get;
	r->accept_ = http::mimetype::appjson;
	r->path_ = "/api/experimentlogs";
	r->params_.emplace("", localLogsTime);
	return 0;
}

unsigned CommHandler::GetLogs_resp(http_response * r)
{
	if (r->status_ == http::responses::ok)
	{
		if (r->content_type_ == http::mimetype::appjson) {

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

			std::string * receivedLog = nullptr;
			auto receivedLogArray = new std::deque<std::string *>();

			for (json::iterator i = j.begin(); i != j.end(); ++i)
			{
				receivedLog = new std::string();
				try
				{
					receivedLog = new std::string(j[i.key()].get<std::string>());
				}
				catch (const std::exception& e)	{
					messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, UnicodeConv::s2ws(e.what()));
					delete receivedLog;
					delete receivedLogArray;
					return 1;
				}
				receivedLogArray->push_back(receivedLog);
			}

			messageHandler.ExchangeLogs(receivedLogArray);

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
	}
	else if (r->status_ == http::responses::internal_err)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server error, could not start a new experiment"));
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
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server not found"));
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



/*********************************
// Instrument Commands
*********************************/
unsigned CommHandler::InstrumentCommand_req(http_request * r)
{
	r->method_ = http::method::post;
	r->path_ = "/api/instrument";

	r->params_.emplace("type", localInstrumentType);
	r->params_.emplace("number", localInstrumentNumber);
	r->params_.emplace("active", localShouldBeActivated);

	return 0;
}

unsigned CommHandler::InstrumentCommand_resp(http_response * r)
{
	if (r->status_ == http::responses::ok)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server not found"));
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