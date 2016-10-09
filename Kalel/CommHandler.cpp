#include "stdafx.h"
#include "CommHandler.h"

#include "Com Classes/ManualActionParam.h"
#include "Netcode/http_request.h"
#include "Netcode/Client.h"
#include "Netcode/json.hpp"
#include "Resources/StringTable.h"
#include "Com Classes/Serialization.h"

#include <functional>

using json = nlohmann::json;

#define STRINGIFY(var) (#var)

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
	//GetLog();
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
	localSettings = ptr;

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
		localStartTime = "";
		localMeasurementsMade = "";
	}
	else
	{
		localStartTime = std::to_string(startTime);
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

void CommHandler::GetLog()
{
}

void CommHandler::GetPorts()
{
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


/**********************************************************************************************************************************
// Request and response functions
**********************************************************************************************************************************/

void CommHandler::Handshake_req(http_request* r) {
	r->method_ = http::method::get;
	r->path_ = "/api/handshake";
}

void CommHandler::Handshake_resp(http_response* r) {

	if (r->status_ == http::responses::ok)
	{
		messageHandler.ConnectionComplete();
	}
	else if (r->status_ == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server not found"));
	}
}

void CommHandler::GetMachineSettings_req(http_request* r) {
	r->method_ = http::method::get;
	r->accept_ = http::mimetype::appjson;
	r->path_ = "/api/machinesettings";
}

void CommHandler::GetMachineSettings_resp(http_response* r) {

	if (r->status_ == http::responses::ok)
	{
		if (r->content_type_ == http::mimetype::appjson) {
			auto j = json::parse(r->answer_);
			MachineSettings receivedSettings;

			serialization::deserializeJSONtoMachineSettings(j, receivedSettings);
			receivedSettings.synced	= true;

			messageHandler.GotMachineSettings(receivedSettings);
			messageHandler.OnSync();
		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Corrupt response format"));
		}
	}
	else if (r->status_ == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server not found"));
	}
}

void CommHandler::SetMachineSettings_req(http_request* r) {
	r->method_			= http::method::post;
	r->content_type_	= http::mimetype::appjson;
	r->path_			= "/api/machinesettings";

	json j;
	serialization::serializeMachineSettingsToJSON(*localSettings, j);
	r->entity_ = j.dump();
}

void CommHandler::SetMachineSettings_resp(http_response* r) {
	
	if (r->status_ == http::responses::ok)
	{
		localSettings.reset();
		messageHandler.OnSync();
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Machine settings updated"));
	}
	else if (r->status_ == http::responses::internal_err)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server error, could not update settings"));
	}
}

void CommHandler::GetData_req(http_request* r) {
	r->method_ = http::method::get;
	r->accept_ = http::mimetype::appjson;
	r->path_ = "/api/experimentdata";
	r->params_.emplace("start", localStartTime);
	r->params_.emplace("measurements", localMeasurementsMade);
}

void CommHandler::GetData_resp(http_response* r) {

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
			}

			ExperimentData * receivedData = nullptr;
			MeasurementsArray * receivedDataArray = new MeasurementsArray();

			for (json::iterator i = j.begin(); i != j.end(); ++i)
			{
				receivedData = new ExperimentData();
				json j2 = j[i.key()];
				serialization::deserializeJSONtoExperimentData(j2, *receivedData);
				receivedDataArray->push_back(receivedData);
			}																													 

			messageHandler.ExchangeData(receivedDataArray);

		}
		else
		{
			messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Corrupt response format"));
		}
	}
	else if (r->status_ == http::responses::not_found)
	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_OK, true, _T("Server not found"));
	}
}