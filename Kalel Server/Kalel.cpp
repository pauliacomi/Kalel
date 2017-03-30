#include "Kalel.h"

#include "Parameters/Parametres.h"
#include "MessageHandler.h"

#include "../Kalel Shared/Netcode/json.hpp"
#include "../Kalel Shared/Netcode/stdHelpers.h"
#include "../Kalel Shared/unicodeConv.h"

#include "../Kalel Shared/Com Classes/Serialization.h"
#include "../Kalel Shared/Com Classes/ControlInstrumentState.h"
#include "../Kalel Shared/Com Classes/ExperimentData.h"
#include "../Kalel Shared/Com Classes/ExperimentSettings.h"
#include "../Kalel Shared/Com Classes/MachineSettings.h"

// Std
#include <vector>
#include <string>

using json = nlohmann::json;

Kalel::Kalel()
	: controlMechanisms{ storageVectors }
	, threadManager{ storageVectors, controlMechanisms}
{
	//
	// Check to see whether the parameters file has been created
	if (!ParametersCheck())
	{
		ParametersInit();		// If not, create it
	}

	//
	// Start server functionality
	server.SetLogs(storageVectors.serverLogs, storageVectors.serverLogsMtx);

	server.AddMethod(
		std::bind(&Kalel::Ping,						this, std::placeholders::_1, std::placeholders::_2), 
													"/api/handshake");
	server.AddMethod(
		std::bind(&Kalel::Sync,						this, std::placeholders::_1, std::placeholders::_2),
													"/api/sync");
	server.AddMethod(
		std::bind(&Kalel::MachineSettingsSync,		this, std::placeholders::_1, std::placeholders::_2), 
													"/api/machinesettings");
	server.AddMethod(
		std::bind(&Kalel::ExperimentSettingsSync,	this, std::placeholders::_1, std::placeholders::_2), 
													"/api/experimentsettings");
	server.AddMethod(
		std::bind(&Kalel::InstrumentStateSync,		this, std::placeholders::_1, std::placeholders::_2), 
													"/api/instrument");
	server.AddMethod(
		std::bind(&Kalel::DataSync,					this, std::placeholders::_1, std::placeholders::_2), 
													"/api/experimentdata");
	server.AddMethod(
		std::bind(&Kalel::LogSync,					this, std::placeholders::_1, std::placeholders::_2), 
													"/api/experimentlogs");
	server.AddMethod(
		std::bind(&Kalel::RequestSync,				this, std::placeholders::_1, std::placeholders::_2),
													"/api/experimentrequests");
	server.AddMethod(
		std::bind(&Kalel::AutomationControl,		this, std::placeholders::_1, std::placeholders::_2),
													"/api/thread");
	server.AddMethod(
		std::bind(&Kalel::ReloadParameters,			this, std::placeholders::_1, std::placeholders::_2), 
													"/api/reloadparameters");
	server.AddMethod(
		std::bind(&Kalel::Debug,					this, std::placeholders::_1, std::placeholders::_2), 
													"/api/debug/testconnection");

	server.Start();

	//
	// Start the measurement and automation threads
	threadManager.StartMeasurement();
	threadManager.StartAutomation();
}



Kalel::~Kalel()
{
	//
	// Stop the measurement and automation threads
	threadManager.ShutdownAutomation();
	threadManager.ShutdownMeasurement();

	// 
	// Server functionality is self-contained
}


void Kalel::GetLogs(std::string &logs) {
	logs.clear();

	auto localCollection = storageVectors.getInfoLogs();

	for (auto it = localCollection.begin(); it != localCollection.end(); ++it)
	{
		logs += timeh::TimePointToString(it->first);
		logs += "   ";
		logs += it->second;
		logs += "\r\n";
	}
}



/*********************************
//
//  Request processing
//
*********************************/

/*********************************
// Ping
*********************************/
void Kalel::Ping(http_request* req, http_response* resp)
{
	if (req->method_ == http::method::get)
	{
		resp->status_ = http::responses::ok;
		resp->content_type_ = http::mimetype::texthtml;
		resp->answer_ = R"(<!DOCTYPE html PUBLIC " -//IETF//DTD HTML 2.0//EN"><html><head><title>Hello</title></head><body><h1>Ping OK</h1></body></html>)";
	}
}


/*********************************
// Sync
*********************************/
void Kalel::Sync(http_request* req, http_response* resp)
{
	if (req->method_ == http::method::get)
	{
		if (!req->params_.empty())
		{
			bool timeMS;
			bool timeES;
			bool timeCS;

			// MachineSettings
			if (req->params_.at("MS").empty()) {
				timeMS = false;
			}
			else {
				if (timeh::StringToTimePoint(req->params_.at("MS")) > storageVectors.machineSettingsChanged)
					timeMS = true;
				else
					timeMS = false;
			}

			// ExperimentSettings
			if (req->params_.at("ES").empty()) {
				timeES = false;
			}
			else {
				if (timeh::StringToTimePoint(req->params_.at("ES")) > storageVectors.experimentSettingsChanged)
					timeES = true;
				else
					timeES = false;
			}

			// ControlState
			if (req->params_.at("CS").empty()) {
				timeCS = false;
			}
			else {
				if (timeh::StringToTimePoint(req->params_.at("CS")) > storageVectors.controlStateChanged)
					timeCS = true;
				else
					timeCS = false;
			}

			json j2;
			j2["MS"] = timeMS;
			j2["ES"] = timeES;
			j2["CS"] = timeCS;

			resp->status_		= http::responses::ok;
			resp->content_type_ = http::mimetype::appjson;
			resp->answer_		= j2.dump();
		}
	}
}



/*********************************
// MachineSettings
*********************************/
void Kalel::MachineSettingsSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	storageVectors.pushErrLogs(timeh::NowTime(), "Machine Settings" + req->method_);
	t.Start();

	// GET
	if (req->method_ == http::method::get)
	{
		json j = *storageVectors.machineSettings;
		
		resp->status_		= http::responses::ok;
		resp->content_type_ = http::mimetype::appjson;
		resp->answer_		= j.dump();
	}

	// SET
	if (req->method_ == http::method::post)
	{
		if (req->content_type_ == http::mimetype::appjson) {

			// Parse the input
			auto j = json::parse(req->entity_.c_str());
			
			// Create new settings
			storageVectors.setmachineSettings(std::make_shared<MachineSettings>(j));
			// Ensure all changes
			controlMechanisms.on_setmachineSettings();

			// Save to file
			ParametersReplace(*storageVectors.machineSettings);

			resp->status_ = http::responses::ok;
		}
		else {
			resp->status_ = http::responses::bad_request;
		}
	}

	storageVectors.pushErrLogs(timeh::NowTime(), std::to_string(t.TimeMilliseconds()));
}


/*********************************
// ExperimentSettings
*********************************/
void Kalel::ExperimentSettingsSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	storageVectors.pushErrLogs(timeh::NowTime(), "ExpSettings" + req->method_);
	t.Start();

	// GET
	if (req->method_ == http::method::get)
	{
		json j = *storageVectors.experimentSettings;

		resp->status_ = http::responses::ok;
		resp->content_type_ = http::mimetype::appjson;
		resp->answer_ = j.dump();
	}

	// SET
	if (req->method_ == http::method::post)
	{
		if (req->content_type_ == http::mimetype::appjson) {

			// Parse the input
			auto j = json::parse(req->entity_.c_str());

			// Create new experiment settings
			storageVectors.setnewExperimentSettings(std::make_shared<ExperimentSettings>(j));
			// Ensure all changes
			threadManager.SetModifiedData();

			resp->status_ = http::responses::ok;
		}
		else {
			resp->status_ = http::responses::bad_request;
		}
	}

	storageVectors.pushErrLogs(timeh::NowTime(), std::to_string(t.TimeMilliseconds()));
}


/*********************************
// Instrument State Sync
*********************************/
void Kalel::InstrumentStateSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	storageVectors.pushErrLogs(timeh::NowTime(), "Instrument state" + req->method_);
	t.Start();

	// GET
	if (req->method_ == http::method::get)
	{
		json j = threadManager.GetInstrumentStates();

		resp->status_ = http::responses::ok;
		resp->content_type_ = http::mimetype::appjson;
		resp->answer_ = j.dump();
	}

	// SET
	if (req->method_ == http::method::post)
	{
		if (!req->params_.empty() ||
			!req->params_.at("type").empty() ||
			!req->params_.at("number").empty() ||
			!req->params_.at("active").empty())
		{
			int instrumentType;
			if (req->params_.at("type") == "valve") {
				instrumentType = INSTRUMENT_VALVE;
			}
			else if (req->params_.at("type") == "ev") {
				instrumentType = INSTRUMENT_EV;
			}
			else if (req->params_.at("type") == "pump") {
				instrumentType = INSTRUMENT_PUMP;
			}
			else {
				instrumentType = To<int>(req->params_.at("type"));
			}

			auto instrumentNumber = To<int>(req->params_.at("number"));
			auto instrumentState = To<bool>(req->params_.at("active"));


			threadManager.ThreadManualAction(instrumentType, instrumentNumber, instrumentState);		// TODO: Should have a better way

			ControlInstrumentState instrumentStates(threadManager.GetInstrumentStates());
			json j = instrumentStates;
			
			resp->status_ = http::responses::ok;
			resp->content_type_ = http::mimetype::appjson;
			resp->answer_ = j.dump();
		}
		else
		{
			resp->status_ = http::responses::conflict;
		}
	}

	storageVectors.pushErrLogs(timeh::NowTime(), std::to_string(t.TimeMilliseconds()));
}


/*********************************
// Data sync
*********************************/
void Kalel::DataSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	storageVectors.pushErrLogs(timeh::NowTime(),  "Data sync");
	t.Start();

	if (req->method_ == http::method::get)
	{
		// Figure out which range of data to send by looking at the time requested

		std::unique_ptr<ExperimentDataStorageArray> localCollection;

		if (req->params_.empty() ||
			req->params_.at("time").empty() ||
			req->params_.at("time") == "start")
		{
			localCollection = std::make_unique<ExperimentDataStorageArray>(storageVectors.getData());
		}
		else
		{
			localCollection = std::make_unique<ExperimentDataStorageArray>(storageVectors.getData(timeh::StringToTimePoint(req->params_.at("time"))));
		}

		if (localCollection->size() != 0)						// If any exist
		{
			json j;

			std::string s = "Serialisation of " + std::to_string(localCollection->size());
			storageVectors.pushErrLogs(timeh::NowTime(), s);
			t.Start();
			
			for (const auto& kv : *localCollection) {
				j.push_back(json::object_t::value_type({ timeh::TimePointToString(kv.first), *(kv.second) }));
			}

			storageVectors.pushErrLogs(timeh::NowTime(), std::to_string(t.TimeMilliseconds()));

			resp->status_ = http::responses::ok;
			resp->content_type_ = http::mimetype::appjson;
			resp->answer_ = j.dump();
		}
		else
		{
			resp->status_ = http::responses::no_content;
		}
	}

	storageVectors.pushErrLogs(timeh::NowTime(), std::to_string(t.TimeMilliseconds()));
}


/*********************************
// Logs sync
*********************************/
void Kalel::LogSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	storageVectors.pushErrLogs(timeh::NowTime(), "Log sync");
	t.Start();

	if (req->method_ == http::method::get)
	{
		// Figure out which range of logs to send by finding the requested timestamp

		std::unique_ptr<TextStorage> localCollection;

		if (req->params_.empty() ||									// If parameters don't exist, send all the logs
			req->params_.at("time").empty() ||
			req->params_.at("time") == "start")
		{
			localCollection = std::make_unique<TextStorage>(storageVectors.getInfoLogs());
		}
		else
		{
			localCollection = std::make_unique<TextStorage>(storageVectors.getInfoLogs(timeh::StringToTimePoint(req->params_.at("time"))));
		}

		if (localCollection->size() != 0)							// If any exist
		{
			json j;

			for (const auto& kv : *localCollection) {
				j[timeh::TimePointToString(kv.first)] = kv.second;
			}

			resp->status_ = http::responses::ok;
			resp->content_type_ = http::mimetype::appjson;
			resp->answer_ = j.dump();
		}
		else
		{
			resp->status_ = http::responses::no_content;
		}
	}

	storageVectors.pushErrLogs(timeh::NowTime(), std::to_string(t.TimeMilliseconds()));
}


/*********************************
// Request sync
*********************************/
void Kalel::RequestSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	storageVectors.pushErrLogs(timeh::NowTime(), "Request sync");
	t.Start();

	if (req->method_ == http::method::get)
	{
		// Figure out which range of logs to send by finding the requested timestamp

		std::unique_ptr<TextStorage> localCollection;

		if (req->params_.empty() ||									// If parameters don't exist, send all the logs
			req->params_.at("time").empty() ||
			req->params_.at("time") == "start")
		{
			localCollection = std::make_unique<TextStorage>(storageVectors.getErrLogs());
		}
		else
		{
			localCollection = std::make_unique<TextStorage>(storageVectors.getInfoLogs(timeh::StringToTimePoint(req->params_.at("time"))));
		}

		if (localCollection->size() != 0)							// If any exist
		{
			json j;

			for (const auto& kv : *localCollection) {
				j[timeh::TimePointToString(kv.first)] = kv.second;
			}

			resp->status_ = http::responses::ok;
			resp->content_type_ = http::mimetype::appjson;
			resp->answer_ = j.dump();
		}
		else
		{
			resp->status_ = http::responses::no_content;
		}
	}

	storageVectors.pushErrLogs(timeh::NowTime(), std::to_string(t.TimeMilliseconds()));
}


/*********************************
// Automation control
*********************************/
void Kalel::AutomationControl(http_request* req, http_response* resp)
{
	if (req->method_ == http::method::post)
	{
		if (!req->params_.empty() ||
			!req->params_.at("action").empty())
		{
			if (req->params_.at("action") == "start") {
				threadManager.StartAutomation();
			}
			else if (req->params_.at("action") == "shutdown") {
				threadManager.ShutdownAutomation();
			}
			else if (req->params_.at("action") == "restart") {
				//threadManager.ResetThread();
			}
			else if (req->params_.at("action") == "reset") {
				threadManager.ResetAutomation();
			}
			else if (req->params_.at("action") == "pause") {
				threadManager.PauseAutomation();
			}
			else if (req->params_.at("action") == "resume") {
				threadManager.ResumeAutomation();
			}

			resp->status_ = http::responses::ok;
		}
		else
		{
			resp->status_ = http::responses::conflict;
		}
	}
}


/*******************************************
// Reload of parameters file into settings
*******************************************/
void Kalel::ReloadParameters(http_request * req, http_response * resp)
{
	// SET
	if (req->method_ == http::method::post)
	{

	}
}


/*********************************
// Debugging
*********************************/
void Kalel::Debug(http_request* req, http_response* resp)
{
	if (req->method_ == http::method::get)
	{
		resp->status_ = http::responses::ok;
		if (!req->params_.empty() ||
			!req->params_.at("return").empty()) {

			resp->content_type_ = http::mimetype::texthtml;

			for (size_t i = 0; i < To<size_t>(req->params_.at("return")); i++)
			{
				resp->answer_ += "test";
			}
		}
	}
}