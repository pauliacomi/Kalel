#include "Kalel.h"

// Helpers
#include "../Kalel Shared/stringHelpers.h"

// JSON
#include "../Kalel Shared/Netcode/json.hpp"
#include "../Kalel Shared/Com Classes/Serialization.h"

// Settings file
#include "Parameters/Parametres.h"

// Com classes
#include "../Kalel Shared/Com Classes/ControlInstrumentState.h"
#include "../Kalel Shared/Com Classes/ExperimentData.h"
#include "../Kalel Shared/Com Classes/ExperimentSettings.h"
#include "../Kalel Shared/Com Classes/MachineSettings.h"
#include "../Kalel Shared/log.h"

// Std
#include <vector>
#include <string>

#define LOG_LEVEL		logDEBUG4			// Change the level of logging here

using json = nlohmann::json;

Kalel::Kalel()
	: controlMechanisms{ storageVectors }
	, threadManager{ storageVectors, controlMechanisms}
{

	//
	// Configure logging

	GeneralLog::ReportingLevel() = LOG_LEVEL;
	OutputGeneral::Info() =	&storageVectors.infoLogs;
	OutputGeneral::Event() = &storageVectors.eventLogs;
	OutputGeneral::Debug() = &storageVectors.debugLogs;

	//
	// Start server functionality

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
		std::bind(&Kalel::ExperimentStatusSync,		this, std::placeholders::_1, std::placeholders::_2), 
													"/api/experimentstatus");
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
		std::bind(&Kalel::RequestSync,				this, std::placeholders::_1, std::placeholders::_2),
													"/api/debuglogs");
	server.AddMethod(
		std::bind(&Kalel::AutomationControl,		this, std::placeholders::_1, std::placeholders::_2),
													"/api/thread");
	server.AddMethod(
		std::bind(&Kalel::ReloadParameters,			this, std::placeholders::_1, std::placeholders::_2), 
													"/api/reloadparameters");

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
	// Set the outputs to null to prevent any lingering logs
	OutputGeneral::Info() = nullptr;
	OutputGeneral::Event() = nullptr;
	OutputGeneral::Debug() = nullptr;

	// 
	// Server functionality is self-contained
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
	LOG(logDEBUG) << "Handshake " << req->method_;
	if (req->method_ == http::method::get)
	{
		resp->status_ = http::responses::ok;
		resp->content_type_ = http::mimetype::texthtml;
		resp->answer_ = R"(<!DOCTYPE html PUBLIC " -//IETF//DTD HTML 2.0//EN">
					<html>
						<head>
							<title>Kalel Server</title>
						</head>
					<body>
						<h1>Ping OK</h1>
					</body>
					</html>)";
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
			bool timeMS = false;					// MachineSettings
			bool timeES = false;					// ExperimentSettings
			bool timeESt = false;					// ExperimentStatus
			bool timeCS = false;					// ControlState

			// MachineSettings
			if (!req->params_.at("MS").empty()) {
				if (timeh::StringToTimePoint(req->params_.at("MS")) > storageVectors.machineSettings->timeChanged)
					timeMS = true;
			}

			// ExperimentSettings
			if (!req->params_.at("ES").empty()) {
				if (timeh::StringToTimePoint(req->params_.at("ES")) > storageVectors.experimentSettings->timeChanged)
					timeES = true;
			}

			// ExperimentState
			if (!req->params_.at("ESt").empty()) {
				if (timeh::StringToTimePoint(req->params_.at("ESt")) > storageVectors.experimentStatus->timeChanged)
					timeES = true;
			}

			// ControlState
			if (!req->params_.at("CS").empty()) {
				if (timeh::StringToTimePoint(req->params_.at("CS")) > storageVectors.controlStateChanged)
					timeCS = true;
			}

			json j2;
			j2["MS"] = timeMS;
			j2["ES"] = timeES;
			j2["ESt"] = timeESt;
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
	LOG(logDEBUG) << "Machine Settings " << req->method_;
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
			ParametersSet(*storageVectors.machineSettings);

			resp->status_ = http::responses::ok;
		}
		else {
			resp->status_ = http::responses::bad_request;
		}
	}

	LOG(logDEBUG) << "Machine Settings took " << std::to_string(t.TimeMilliseconds());
}


/*********************************
// ExperimentSettings
*********************************/
void Kalel::ExperimentSettingsSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	LOG(logDEBUG) << "ExpSettings" << req->method_;
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
			auto newSettings = std::make_shared<ExperimentSettings>(j);

			// Create new experiment settings, logging change if experiment is running
			if (storageVectors.experimentStatus->experimentInProgress == true) {
				controlMechanisms.fileWriter.RecordDataChange(false, 
					*newSettings, *storageVectors.experimentSettings,
					*storageVectors.experimentStatus, *storageVectors.currentData);						// non-CSV
				controlMechanisms.fileWriter.RecordDataChange(true,
					*newSettings, *storageVectors.experimentSettings,
					*storageVectors.experimentStatus, *storageVectors.currentData);						// CSV
			}

			storageVectors.setExperimentSettings(newSettings);

			resp->status_ = http::responses::ok;
		}
		else {
			resp->status_ = http::responses::bad_request;
		}
	}

	LOG(logDEBUG) << "Experiment settings took " << std::to_string(t.TimeMilliseconds());
}


/*********************************
// ExperimentStatus
*********************************/
void Kalel::ExperimentStatusSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	LOG(logDEBUG) << "Experiment Status" << req->method_;
	t.Start();

	// GET
	if (req->method_ == http::method::get)
	{
		json j = *storageVectors.experimentStatus;

		resp->status_ = http::responses::ok;
		resp->content_type_ = http::mimetype::appjson;
		resp->answer_ = j.dump();
	}

	LOG(logDEBUG) << "Experiment Status took " << std::to_string(t.TimeMilliseconds());
}


/*********************************
// Instrument State Sync
*********************************/
void Kalel::InstrumentStateSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	LOG(logDEBUG) << "Instrument state" << req->method_;
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
				instrumentType = CONTROLLER_VALVE;
			}
			else if (req->params_.at("type") == "ev") {
				instrumentType = CONTROLLER_EV;
			}
			else if (req->params_.at("type") == "pump") {
				instrumentType = CONTROLLER_PUMP;
			}
			else {
				instrumentType = stringh::To<int>(req->params_.at("type"));
			}

			auto instrumentNumber = stringh::To<int>(req->params_.at("number"));
			auto instrumentState = stringh::To<bool>(req->params_.at("active"));


			threadManager.ThreadManualAction(instrumentType, instrumentNumber, instrumentState);		// TODO: Should have a better way

			json j = threadManager.GetInstrumentStates();
			
			resp->status_ = http::responses::ok;
			resp->content_type_ = http::mimetype::appjson;
			resp->answer_ = j.dump();
		}
		else
		{
			resp->status_ = http::responses::conflict;
		}
	}

	LOG(logDEBUG) << "Instrument state took " << std::to_string(t.TimeMilliseconds());
}


/*********************************
// Data sync
*********************************/
void Kalel::DataSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	LOG(logDEBUG) << "Data sync";
	t.Start();

	if (req->method_ == http::method::get)
	{
		// Figure out which range of data to send by looking at the time requested

		std::unique_ptr<ExperimentDataStorageArray> localCollection;

		if (req->params_.empty() ||
			req->params_.at("time").empty() ||
			req->params_.at("time") == "start")
		{
			localCollection = std::make_unique<ExperimentDataStorageArray>(storageVectors.dataCollection.get());
		}
		else
		{
			localCollection = std::make_unique<ExperimentDataStorageArray>(storageVectors.dataCollection.get(timeh::StringToTimePoint(req->params_.at("time"))));
		}

		if (localCollection->size() != 0)						// If any exist
		{
			json j;

			std::string s = "Serialisation of " + std::to_string(localCollection->size());
			LOG(logDEBUG) << s;
			t.Start();
			
			for (const auto& kv : *localCollection) {
				j.push_back(json::object_t::value_type({ timeh::TimePointToString(kv.first), *(kv.second) }));
			}

			LOG(logDEBUG) << "Serialisation took " << std::to_string(t.TimeMilliseconds());

			resp->status_ = http::responses::ok;
			resp->content_type_ = http::mimetype::appjson;
			resp->answer_ = j.dump();
		}
		else
		{
			resp->status_ = http::responses::no_content;
		}
	}

	LOG(logDEBUG) << std::to_string(t.TimeMilliseconds());
}


/*********************************
// Logs sync
*********************************/
void Kalel::LogSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	LOG(logDEBUG) << "Log sync";
	t.Start();

	if (req->method_ == http::method::get)
	{
		// Figure out which range of logs to send by finding the requested timestamp

		std::unique_ptr<TextStorage> localCollection;

		if (req->params_.empty() ||									// If parameters don't exist, send all the logs
			req->params_.at("time").empty() ||
			req->params_.at("time") == "start")
		{
			localCollection = std::make_unique<TextStorage>(storageVectors.infoLogs.get());
		}
		else
		{
			localCollection = std::make_unique<TextStorage>(storageVectors.infoLogs.get(timeh::StringToTimePoint(req->params_.at("time"))));
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

	LOG(logDEBUG) << "Logs took " << std::to_string(t.TimeMilliseconds());
}


/*********************************
// Request sync
*********************************/
void Kalel::RequestSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	LOG(logDEBUG) << "Request sync";
	t.Start();

	if (req->method_ == http::method::get)
	{
		// Figure out which range of logs to send by finding the requested timestamp

		std::unique_ptr<TextStorage> localCollection;

		if (req->params_.empty() ||									// If parameters don't exist, send all the logs
			req->params_.at("time").empty() ||
			req->params_.at("time") == "start")
		{
			localCollection = std::make_unique<TextStorage>(storageVectors.eventLogs.get());
		}
		else
		{
			localCollection = std::make_unique<TextStorage>(storageVectors.eventLogs.get(timeh::StringToTimePoint(req->params_.at("time"))));
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

	LOG(logDEBUG) << "requests took " << std::to_string(t.TimeMilliseconds());
}

/*********************************
// Debug sync
*********************************/
void Kalel::DebugSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	LOG(logDEBUG) << "Debug sync";
	t.Start();

	if (req->method_ == http::method::get)
	{
		// Figure out which range of logs to send by finding the requested timestamp

		std::unique_ptr<TextStorage> localCollection;

		if (req->params_.empty() ||									// If parameters don't exist, send all the logs
			req->params_.at("time").empty() ||
			req->params_.at("time") == "start")
		{
			localCollection = std::make_unique<TextStorage>(storageVectors.debugLogs.get());
		}
		else
		{
			localCollection = std::make_unique<TextStorage>(storageVectors.debugLogs.get(timeh::StringToTimePoint(req->params_.at("time"))));
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

	LOG(logDEBUG) << "Debug took " << std::to_string(t.TimeMilliseconds());
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
