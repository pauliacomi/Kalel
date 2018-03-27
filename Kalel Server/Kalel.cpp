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
	: controlMechanisms{ storage.machineSettings }
	, threadManager{ storage, controlMechanisms}
{

	//
	// Configure logging

	GeneralLog::ReportingLevel() = LOG_LEVEL;
	OutputGeneral::Info() =	&storage.infoLogs;
	OutputGeneral::Event() = &storage.eventLogs;
	OutputGeneral::Debug() = &storage.debugLogs;

	//
	// Start server functionality

	server.AddMethod(
		std::bind(&Kalel::Ping,						this, std::placeholders::_1, std::placeholders::_2), 
													"/api/handshake");
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

	server.SetCredentials("user", "kalel");																					// TODO get from machine settins
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
// MachineSettings
*********************************/
void Kalel::MachineSettingsSync(http_request* req, http_response* resp)
{
	LOG(logDEBUG2) << "Machine Settings " << req->method;
	timeh::timer t;
	t.Start();

	// GET
	if (req->method == http::method::get)
	{
		auto time = req->params.find("t");
		if (time != req->params.end() && timeh::StringToTimePoint(time->second) < storage.machineSettings.tp)
		{
			resp->status = http::responses::no_content;
		}
		else
		{
			json j = GetMachineSettings();

			resp->status = http::responses::ok;
			resp->content_type = http::mimetype::appjson;
			resp->body = j.dump();
		}
	}

	// SET
	if (req->method == http::method::post)
	{
		if (req->content_type == http::mimetype::appjson) {

			// Parse the input
			SetMachineSettings(json::parse(req->body));

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
	LOG(logDEBUG2) << "ExpSettings" << req->method;
	timeh::timer t;
	t.Start();

	// GET
	if (req->method == http::method::get)
	{
		json j = GetExperimentSettings();

		resp->status = http::responses::ok;
		resp->content_type = http::mimetype::appjson;
		resp->body = j.dump();
	}

	// SET
	if (req->method == http::method::post)
	{
		if (req->content_type == http::mimetype::appjson) {

			// Parse the input
			SetExperimentSettings(json::parse(req->body));

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
	LOG(logDEBUG2) << "Experiment Status" << req->method;
	timeh::timer t;
	t.Start();

	// GET
	if (req->method == http::method::get)
	{
		auto time = req->params.find("t");
		if (time != req->params.end() && timeh::StringToTimePoint(time->second) < storage.experimentStatus.tp)
		{
			resp->status = http::responses::no_content;
		}
		else
		{
			json j = storage.experimentStatus;

			resp->status = http::responses::ok;
			resp->content_type = http::mimetype::appjson;
			resp->body = j.dump();
		}
	}

	LOG(logDEBUG2) << "Experiment Status took " << std::to_string(t.TimeMilliseconds());
}


/*********************************
// Instrument State Sync
*********************************/
void Kalel::InstrumentStateSync(http_request* req, http_response* resp)
{
	LOG(logDEBUG2) << "Instrument state" << req->method;
	timeh::timer t;
	t.Start();

	// GET
	if (req->method == http::method::get)
	{
		ControlInstrumentState s{ threadManager.GetInstrumentStates() };
		json j;

		for (auto i : s)
		{
			j[std::to_string(i.first)] = i.second;
		}

		resp->status = http::responses::ok;
		resp->content_type = http::mimetype::appjson;
		resp->body = j.dump();
	}

	// SET
	if (req->method == http::method::post)
	{
		if (req->params.find("ID") != req->params.end() ||
			req->params.find("state") != req->params.end() )
		{
			auto instrumentID = stringh::To<int>(req->params.at("ID"));
			auto instrumentState = stringh::To<bool>(req->params.at("state"));

			threadManager.ThreadManualAction(instrumentID, instrumentState);
			storage.controlStateChanged = timeh::NowTime();	

			resp->status = http::responses::ok;
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
	LOG(logDEBUG2) << "Data sync";
	timeh::timer t;
	t.Start();

	if (req->method == http::method::get)
	{
		// Figure out which range of data to send by looking at the time requested

		std::map<std::chrono::system_clock::time_point, ExperimentData> localCollection;

		auto time = req->params.find("t");
		if (time == req->params.end())
		{
			localCollection = storage.dataCollection.get();
		}
		else
		{
			localCollection = storage.dataCollection.get(timeh::StringToTimePoint(time->second));
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
	LOG(logDEBUG2) << "Log sync " << req->method;
	timeh::timer t;
	t.Start();

	if (req->method == http::method::del)
	{
		storage.infoLogs.del();
		resp->status = http::responses::ok;
	}

	if (req->method == http::method::get)
	{
		// Figure out which range of logs to send by finding the requested timestamp

		TextStorage localCollection;

		auto time = req->params.find("t");
		if (time == req->params.end())
		{
			localCollection = storage.infoLogs.get();
		}
		else
		{
			localCollection = storage.infoLogs.get(timeh::StringToTimePoint(time->second));
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
	LOG(logDEBUG2) << "Request sync " << req->method;
	timeh::timer t;
	t.Start();

	if (req->method == http::method::del)
	{
		storage.eventLogs.del();
		resp->status = http::responses::ok;
	}

	if (req->method == http::method::get)
	{
		// Figure out which range of logs to send by finding the requested timestamp

		TextStorage localCollection;

		auto time = req->params.find("t");
		if (time == req->params.end())
		{
			localCollection = storage.eventLogs.get();
		}
		else
		{
			localCollection = storage.eventLogs.get(timeh::StringToTimePoint(time->second));
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
	LOG(logDEBUG2) << "Debug sync " << req->method;
	timeh::timer t;
	t.Start();

	if (req->method == http::method::del)
	{
		storage.debugLogs.del();
		resp->status = http::responses::ok;
	}

	if (req->method == http::method::get)
	{
		// Figure out which range of logs to send by finding the requested timestamp

		TextStorage localCollection;

		auto time = req->params.find("t");
		if (time == req->params.end())
		{
			localCollection = storage.debugLogs.get();
		}
		else
		{
			localCollection = storage.debugLogs.get(timeh::StringToTimePoint(time->second));
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
		if (req->params.find("action") != req->params.end())
		{
			if (req->params.at("action") == "start") {
				threadManager.StartAutomation();
			}
			else if (req->params.at("action") == "shutdown") {
				threadManager.ShutdownAutomation();
			}
			else if (req->params.at("action") == "restart") {
				threadManager.ShutdownAutomation();
				threadManager.StartAutomation();
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
		if (req->params.find("t") != req->params.end() ||
			req->params.find("i") != req->params.end())
		{
			int choice = stringh::To<int>(req->params.at("i"));
			switch (choice)
			{
			case CHOICE_NONE:
			case CHOICE_YES:
			case CHOICE_NO:
			case CHOICE_WAIT:
				resp->status = http::responses::ok;
				storage.eventLogs.del(timeh::StringToTimePoint(req->params.at("t")));
				threadManager.SetUserChoice(choice);
				break;
			default:
				resp->status = http::responses::not_acceptable;
				break;
			}
		}
	}
}




//
//
// Setting and getting of com classes
//


MachineSettings Kalel::GetMachineSettings()
{
	return storage.machineSettings;
}

unsigned Kalel::SetMachineSettings(const MachineSettings & ms)
{
	// Save to file
	ParametersSet(ms);

	// Check there are no experiments running
	if (storage.experimentStatus.experimentInProgress) {
		LOG(logERROR) << "Cannot change machine settings while an experiment is in progress"; 
		return 1; 
	}

	// Shutdown threads
	if (!threadManager.ShutdownMeasurement()) { return 2; };
	if (!threadManager.ShutdownAutomation()) { return 2; };

	// Reinitialise instruments
	controlMechanisms.instruments.Reset(ms);
	// Replace settings
	storage.machineSettings = ms;

	// Restart threads
	if (!threadManager.StartMeasurement()) { return 3; };
	if (!threadManager.StartAutomation()) { return 3; };

	// Operation successful
	return 0;
}

ExperimentSettings Kalel::GetExperimentSettings()
{
	return storage.experimentSettings;
}

void Kalel::SetExperimentSettings(const ExperimentSettings & es)
{
	// Create new experiment settings, logging change if experiment is running
	if (storage.experimentStatus.experimentInProgress == true) {
		controlMechanisms.fileWriter.RecordDataChange(false,
			es, storage.experimentSettings,
			storage.experimentStatus, storage.currentData);						// non-CSV
		controlMechanisms.fileWriter.RecordDataChange(true,
			es, storage.experimentSettings,
			storage.experimentStatus, storage.currentData);						// CSV
	}

	storage.setExperimentSettings(es);
}
