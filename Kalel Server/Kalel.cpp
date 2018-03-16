#include "Kalel.h"

// Helpers
#include "../Kalel Shared/stringHelpers.h"
#include "../Kalel Shared/log.h"
#include "../Kalel Shared/Resources/DefineStages.h"

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


// Std
#include <vector>
#include <string>

// Change the level of logging here

#define LOG_LEVEL		logDEBUG3

//
//
//eventLOGS : critical logging, that needs to be displayed to user
//
//		logEVENT
//			- an event that requires a user input
//		logERROR
//			- a critical error which requires user attention
//		logWARNING
//			- a warning which could impact functionality
//
//infoLOGS : logging regarding operation and automation
//
//		logINFO
//			- describes the status of the machine and the progress
//			- of automation functionality
//
//debugLOGS : debug information which may be useful
//		logDEBUG
//			- Instruments log errors in reading and writing
//		logDEBUG1
//			- Instruments log errors in port and connections
//			- HTTPServer logs errors in socket and http requests
//		logDEBUG2
//			- Instruments log connections and reading
//			- HTTPServer logs request type and completion time
//		logDEBUG3
//			- HTTPServer logs connections
//		logDEBUG4
//			- HTTPServer logs all requests and responses
//


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
		std::bind(&Kalel::DebugSync,				this, std::placeholders::_1, std::placeholders::_2),
													"/api/debuglogs");
	server.AddMethod(
		std::bind(&Kalel::AutomationControl,		this, std::placeholders::_1, std::placeholders::_2),
													"/api/thread");
	server.AddMethod(
		std::bind(&Kalel::UserInput,				this, std::placeholders::_1, std::placeholders::_2),
													"/api/input");

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
	LOG(logDEBUG2) << "Handshake " << req->method;
	if (req->method == http::method::get)
	{
		resp->status = http::responses::ok;
		resp->content_type = http::mimetype::texthtml;
		resp->body = R"(<!DOCTYPE html PUBLIC " -//IETF//DTD HTML 2.0//EN">
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
	if (req->method == http::method::get)
	{
		if (!req->params.empty())
		{
			bool timeMS = false;					// MachineSettings
			bool timeES = false;					// ExperimentSettings
			bool timeESt = false;					// ExperimentStatus
			bool timeCS = false;					// ControlState

			// MachineSettings
			if (!req->params.at("MS").empty()) {
				if (timeh::StringToTimePoint(req->params.at("MS")) > storageVectors.machineSettings.timeChanged)
					timeMS = true;
			}

			// ExperimentSettings
			if (!req->params.at("ES").empty()) {
				if (timeh::StringToTimePoint(req->params.at("ES")) > storageVectors.experimentSettings.timeChanged)
					timeES = true;
			}

			// ExperimentState
			if (!req->params.at("ESt").empty()) {
				if (timeh::StringToTimePoint(req->params.at("ESt")) > storageVectors.experimentStatus.timeChanged)
					timeES = true;
			}

			// ControlState
			if (!req->params.at("CS").empty()) {
				if (timeh::StringToTimePoint(req->params.at("CS")) > storageVectors.controlStateChanged)
					timeCS = true;
			}

			json j2;
			j2["MS"] = timeMS;
			j2["ES"] = timeES;
			j2["ESt"] = timeESt;
			j2["CS"] = timeCS;

			resp->status		= http::responses::ok;
			resp->content_type = http::mimetype::appjson;
			resp->body		= j2.dump();
		}
	}
}



/*********************************
// MachineSettings
*********************************/
void Kalel::MachineSettingsSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	LOG(logDEBUG2) << "Machine Settings " << req->method;
	t.Start();

	// GET
	if (req->method == http::method::get)
	{
		json j = storageVectors.machineSettings;
		
		resp->status		= http::responses::ok;
		resp->content_type = http::mimetype::appjson;
		resp->body		= j.dump();
	}

	// SET
	if (req->method == http::method::post)
	{
		if (req->content_type == http::mimetype::appjson) {

			// Parse the input
			auto j = json::parse(req->body);
			
			// Create new settings
			storageVectors.setmachineSettings(MachineSettings(j));

			// Ensure all changes
			controlMechanisms.on_setmachineSettings();

			// Save to file
			ParametersSet(storageVectors.machineSettings);

			resp->status = http::responses::ok;
		}
		else {
			resp->status = http::responses::bad_request;
		}
	}

	LOG(logDEBUG2) << "Machine Settings took " << std::to_string(t.TimeMilliseconds());
}


/*********************************
// ExperimentSettings
*********************************/
void Kalel::ExperimentSettingsSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	LOG(logDEBUG2) << "ExpSettings" << req->method;
	t.Start();

	// GET
	if (req->method == http::method::get)
	{
		json j = storageVectors.experimentSettings;

		resp->status = http::responses::ok;
		resp->content_type = http::mimetype::appjson;
		resp->body = j.dump();
	}

	// SET
	if (req->method == http::method::post)
	{
		if (req->content_type == http::mimetype::appjson) {

			// Parse the input
			auto j = json::parse(req->body);
			auto newSettings = ExperimentSettings(j);

			// Create new experiment settings, logging change if experiment is running
			if (storageVectors.experimentStatus.experimentInProgress == true) {
				controlMechanisms.fileWriter.RecordDataChange(false, 
					newSettings, storageVectors.experimentSettings,
					storageVectors.experimentStatus, storageVectors.currentData);						// non-CSV
				controlMechanisms.fileWriter.RecordDataChange(true,
					newSettings, storageVectors.experimentSettings,
					storageVectors.experimentStatus, storageVectors.currentData);						// CSV
			}

			storageVectors.setExperimentSettings(newSettings);

			resp->status = http::responses::ok;
		}
		else {
			resp->status = http::responses::bad_request;
		}
	}

	LOG(logDEBUG2) << "Experiment settings took " << std::to_string(t.TimeMilliseconds());
}


/*********************************
// ExperimentStatus
*********************************/
void Kalel::ExperimentStatusSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	LOG(logDEBUG2) << "Experiment Status" << req->method;
	t.Start();

	// GET
	if (req->method == http::method::get)
	{
		json j = storageVectors.experimentStatus;

		resp->status = http::responses::ok;
		resp->content_type = http::mimetype::appjson;
		resp->body = j.dump();
	}

	LOG(logDEBUG2) << "Experiment Status took " << std::to_string(t.TimeMilliseconds());
}


/*********************************
// Instrument State Sync
*********************************/
void Kalel::InstrumentStateSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	LOG(logDEBUG2) << "Instrument state" << req->method;
	t.Start();

	// GET
	if (req->method == http::method::get)
	{
		json j = threadManager.GetInstrumentStates();

		resp->status = http::responses::ok;
		resp->content_type = http::mimetype::appjson;
		resp->body = j.dump();
	}

	// SET
	if (req->method == http::method::post)
	{
		if (!req->params.empty() ||
			!req->params.at("type").empty() ||
			!req->params.at("number").empty() ||
			!req->params.at("active").empty())
		{
			int instrumentType;
			if (req->params.at("type") == "valve") {
				instrumentType = CONTROLLER_VALVE;
			}
			else if (req->params.at("type") == "ev") {
				instrumentType = CONTROLLER_EV;
			}
			else if (req->params.at("type") == "pump") {
				instrumentType = CONTROLLER_PUMP;
			}
			else {
				instrumentType = stringh::To<int>(req->params.at("type"));
			}

			auto instrumentNumber = stringh::To<int>(req->params.at("number"));
			auto instrumentState = stringh::To<bool>(req->params.at("active"));


			threadManager.ThreadManualAction(instrumentType, instrumentNumber, instrumentState);		// TODO: Should have a better way

			json j = threadManager.GetInstrumentStates();
			
			resp->status = http::responses::ok;
			resp->content_type = http::mimetype::appjson;
			resp->body = j.dump();
		}
		else
		{
			resp->status = http::responses::conflict;
		}
	}

	LOG(logDEBUG2) << "Instrument state took " << std::to_string(t.TimeMilliseconds());
}


/*********************************
// Data sync
*********************************/
void Kalel::DataSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	LOG(logDEBUG2) << "Data sync";
	t.Start();

	if (req->method == http::method::get)
	{
		// Figure out which range of data to send by looking at the time requested

		std::map<std::chrono::system_clock::time_point, ExperimentData> localCollection;

		if (req->params.empty() ||
			req->params.at("time").empty() ||
			req->params.at("time") == "start")
		{
			localCollection = storageVectors.dataCollection.get();
		}
		else
		{
			localCollection = storageVectors.dataCollection.get(timeh::StringToTimePoint(req->params.at("time")));
		}

		if (localCollection.size() != 0)						// If any exist
		{
			json j;

			LOG(logDEBUG) << "Serialisation of " << std::to_string(localCollection.size());
			t.Start();
			
			for (const auto& kv : localCollection) {
				j.push_back(json::object_t::value_type({ timeh::TimePointToString(kv.first), kv.second }));
			}

			LOG(logDEBUG) << "Serialisation took " << std::to_string(t.TimeMilliseconds());

			resp->status = http::responses::ok;
			resp->content_type = http::mimetype::appjson;
			resp->body = j.dump();
		}
		else
		{
			resp->status = http::responses::no_content;
		}
	}

	LOG(logDEBUG2) << std::to_string(t.TimeMilliseconds());
}


/*********************************
// Logs sync
*********************************/
void Kalel::LogSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	LOG(logDEBUG2) << "Log sync " << req->method;
	t.Start();

	if (req->method == http::method::del)
	{
		storageVectors.infoLogs.del();
		resp->status = http::responses::ok;
	}

	if (req->method == http::method::get)
	{
		// Figure out which range of logs to send by finding the requested timestamp

		TextStorage localCollection;

		if (req->params.empty() ||									// If parameters don't exist, send all the logs
			req->params.at("time").empty() ||
			req->params.at("time") == "start")
		{
			localCollection = storageVectors.infoLogs.get();
		}
		else
		{
			localCollection = storageVectors.infoLogs.get(timeh::StringToTimePoint(req->params.at("time")));
		}

		if (localCollection.size() != 0)							// If any exist
		{
			json j;

			for (const auto& kv : localCollection) {
				j[timeh::TimePointToString(kv.first)] = kv.second;
			}

			resp->status = http::responses::ok;
			resp->content_type = http::mimetype::appjson;
			resp->body = j.dump();
		}
		else
		{
			resp->status = http::responses::no_content;
		}
	}

	LOG(logDEBUG2) << "Logs took " << std::to_string(t.TimeMilliseconds());
}


/*********************************
// Request sync
*********************************/
void Kalel::RequestSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	LOG(logDEBUG2) << "Request sync " << req->method;
	t.Start();

	if (req->method == http::method::del)
	{
		storageVectors.eventLogs.del();
		resp->status = http::responses::ok;
	}

	if (req->method == http::method::get)
	{
		// Figure out which range of logs to send by finding the requested timestamp

		TextStorage localCollection;

		if (req->params.empty() ||									// If parameters don't exist, send all the logs
			req->params.at("time").empty() ||
			req->params.at("time") == "start")
		{
			localCollection = storageVectors.eventLogs.get();
		}
		else
		{
			localCollection = storageVectors.eventLogs.get(timeh::StringToTimePoint(req->params.at("time")));
		}

		if (localCollection.size() != 0)							// If any exist
		{
			json j;

			for (const auto& kv : localCollection) {
				j[timeh::TimePointToString(kv.first)] = kv.second;
			}

			resp->status = http::responses::ok;
			resp->content_type = http::mimetype::appjson;
			resp->body = j.dump();
		}
		else
		{
			resp->status = http::responses::no_content;
		}
	}

	LOG(logDEBUG2) << "Requests took " << std::to_string(t.TimeMilliseconds());
}


/*********************************
// Debug sync
*********************************/
void Kalel::DebugSync(http_request* req, http_response* resp)
{
	timeh::timer t;
	LOG(logDEBUG2) << "Debug sync " << req->method;
	t.Start();

	if (req->method == http::method::del)
	{
		storageVectors.debugLogs.del();
		resp->status = http::responses::ok;
	}

	if (req->method == http::method::get)
	{
		// Figure out which range of logs to send by finding the requested timestamp

		TextStorage localCollection;

		if (req->params.empty() ||									// If parameters don't exist, send all the logs
			req->params.at("time").empty() ||
			req->params.at("time") == "start")
		{
			localCollection = storageVectors.debugLogs.get();
		}
		else
		{
			localCollection = storageVectors.debugLogs.get(timeh::StringToTimePoint(req->params.at("time")));
		}

		if (localCollection.size() != 0)							// If any exist
		{
			json j;

			for (const auto& kv : localCollection) {
				j[timeh::TimePointToString(kv.first)] = kv.second;
			}

			resp->status = http::responses::ok;
			resp->content_type = http::mimetype::appjson;
			resp->body = j.dump();
		}
		else
		{
			resp->status = http::responses::no_content;
		}
	}

	LOG(logDEBUG2) << "Debug took " << std::to_string(t.TimeMilliseconds());
}



/*********************************
// Automation control
*********************************/
void Kalel::AutomationControl(http_request* req, http_response* resp)
{
	if (req->method == http::method::post)
	{
		if (!req->params.empty() ||
			!req->params.at("action").empty())
		{
			if (req->params.at("action") == "start") {
				threadManager.StartAutomation();
			}
			else if (req->params.at("action") == "shutdown") {
				threadManager.ShutdownAutomation();
			}
			else if (req->params.at("action") == "restart") {
				//threadManager.ResetThread();
			}
			else if (req->params.at("action") == "reset") {
				threadManager.ResetAutomation();
			}
			else if (req->params.at("action") == "pause") {
				threadManager.PauseAutomation();
			}
			else if (req->params.at("action") == "resume") {
				threadManager.ResumeAutomation();
			}
			else if (req->params.at("action") == "nextstage") {
			}
			else if (req->params.at("action") == "nextstep") {
			}
			else if (req->params.at("action") == "nextdose") {
			}

			resp->status = http::responses::ok;
		}
		else
		{
			resp->status = http::responses::conflict;
		}
	}
}


/*******************************************
// User input functionality
*******************************************/

void Kalel::UserInput(http_request * req, http_response * resp)
{
	// SET
	if (req->method == http::method::post)
	{
		if (!req->params.empty() ||
			!req->params.at("input").empty())
		{
			int choice = stringh::To<int>(req->params.at("action"));
			switch (choice)
			{
			case CHOICE_NONE:
			case CHOICE_YES:
			case CHOICE_NO:
			case CHOICE_WAIT:
				resp->status = http::responses::ok;
				threadManager.SetUserChoice(choice);
				break;
			default:
				resp->status = http::responses::not_acceptable;
				break;
			}
		}
	}
}
