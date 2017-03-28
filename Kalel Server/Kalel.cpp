#include "Kalel.h"

#include "Parameters/Parametres.h"
#include "MessageHandler.h"

#include "../Kalel Shared/Netcode/json.hpp"
#include "../Kalel Shared/Netcode/stdHelpers.h"
#include "../Kalel Shared/Com Classes/Serialization.h"
#include "../Kalel Shared/Com Classes/ControlInstrumentState.h"

#include "../Kalel Shared/Com Classes/ExperimentData.h"
#include "../Kalel Shared/Com Classes/ExperimentSettings.h"
#include "../Kalel Shared/Com Classes/MachineSettings.h"
#include "../Kalel Shared/unicodeConv.h"

// Std
#include <vector>
#include <string>

using json = nlohmann::json;

Kalel::Kalel()
	: threadManager{ storageVectors }
{
	//
	// Check to see whether the parameters file has been created
	if (!ParametersCheck())
	{
		ParametersInit();		// If not, create it
	}

	//
	// Populate Machine Settings
	auto newMachineSettings = std::make_shared<MachineSettings>();
	ParametersGet(*newMachineSettings);
	storageVectors.setmachineSettings(newMachineSettings);
	// Set path
	storageVectors.experimentSettings->dataGeneral.chemin = storageVectors.machineSettings->CheminFichierGeneral;
	storageVectors.newExperimentSettings->dataGeneral.chemin = storageVectors.machineSettings->CheminFichierGeneral;

	//
	// Start server functionality
	server.SetLogs(storageVectors.serverLogs, storageVectors.serverLogsMtx);

	server.AddMethod(
		std::bind(&Kalel::Ping, this, std::placeholders::_1, std::placeholders::_2), 
		"/api/handshake"
	);
	server.AddMethod(
		std::bind(&Kalel::Sync, this, std::placeholders::_1, std::placeholders::_2),
		"/api/sync"
	);
	server.AddMethod(
		std::bind(&Kalel::MachineSettingsSync, this, std::placeholders::_1, std::placeholders::_2), 
		"/api/machinesettings"
	);
	server.AddMethod(
		std::bind(&Kalel::ExperimentSettingsSync, this, std::placeholders::_1, std::placeholders::_2), 
		"/api/experimentsettings"
	); 
	server.AddMethod(
		std::bind(&Kalel::InstrumentStateSync, this, std::placeholders::_1, std::placeholders::_2), 
		"/api/instrument"
	);
	server.AddMethod(
		std::bind(&Kalel::DataSync, this, std::placeholders::_1, std::placeholders::_2), 
		"/api/experimentdata"
	);
	server.AddMethod(
		std::bind(&Kalel::LogSync, this, std::placeholders::_1, std::placeholders::_2), 
		"/api/experimentlogs"
	);
	server.AddMethod(
		std::bind(&Kalel::RequestSync, this, std::placeholders::_1, std::placeholders::_2),
		"/api/experimentrequests"
	);
	server.AddMethod(
		std::bind(&Kalel::AutomationControl, this, std::placeholders::_1, std::placeholders::_2),
		"/api/thread"
	);
	server.AddMethod(
		std::bind(&Kalel::Debug, this, std::placeholders::_1, std::placeholders::_2), 
		"/api/debug/testconnection"
	);

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

	auto localCollection = storageVectors.getErrLogs();

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

			// TODO make this an actual value instead of machineSettingsChanged
			// ControlState
			if (req->params_.at("CS").empty()) {
				timeCS = false;
			}
			else {
				if (timeh::StringToTimePoint(req->params_.at("CS")) > storageVectors.machineSettingsChanged)
					timeCS = true;
				else
					timeCS = false;
			}

			json j2;
			j2["MS"] = timeMS;
			j2["ES"] = timeES;
			j2["CS"] = timeCS;

			resp->status_ = http::responses::ok;
			resp->content_type_ = http::mimetype::appjson;
			resp->answer_ = j2.dump();
		}
	}
}



/*********************************
// MachineSettings
*********************************/
void Kalel::MachineSettingsSync(http_request* req, http_response* resp)
{
	// GET
	if (req->method_ == http::method::get)
	{
		json j = *storageVectors.machineSettings;
		
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
			
			// Save to memory
			auto newMachineSettings = std::make_shared<MachineSettings>();
			*newMachineSettings = j;
			storageVectors.setmachineSettings(newMachineSettings);

			// Save to file
			ParametersReplace(*newMachineSettings);

			resp->status_ = http::responses::ok;
		}
		else {
			resp->status_ = http::responses::bad_request;
		}
	}
}


/*********************************
// ExperimentSettings
*********************************/
void Kalel::ExperimentSettingsSync(http_request* req, http_response* resp)
{
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

			auto newExpSettings = std::make_shared<ExperimentSettings>();
			*newExpSettings = j;
			storageVectors.setnewExperimentSettings(newExpSettings);

			threadManager.SetModifiedData();

			resp->status_ = http::responses::ok;
		}
		else {
			resp->status_ = http::responses::bad_request;
		}
	}
}


/*********************************
// Instrument State Sync
*********************************/
void Kalel::InstrumentStateSync(http_request* req, http_response* resp)
{
	// GET
	if (req->method_ == http::method::get)
	{
		ControlInstrumentState instrumentStates(threadManager.GetInstrumentStates());
		json j = instrumentStates;

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


			threadManager.ThreadManualAction(instrumentType, instrumentNumber, instrumentState);		// Should have a future/promise here

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
}


/*********************************
// Data sync
*********************************/
void Kalel::DataSync(http_request* req, http_response* resp)
{
	if (req->method_ == http::method::get)
	{
		// Figure out which range of data to send by looking at the time requested

		ExperimentDataStorageArray::iterator it;
		auto localCollection = storageVectors.getData();

		if (req->params_.empty() ||
			req->params_.at("time").empty() ||
			req->params_.at("time") == "start")
		{
			it = localCollection.begin();
		}
		else
		{
			it = localCollection.upper_bound(timeh::StringToTimePoint(req->params_.at("time")));
		}

		if (it != localCollection.end())						// If iterator is valid, send requested logs
		{
			//json j(localCollection);

			resp->status_ = http::responses::ok;
			resp->content_type_ = http::mimetype::appjson;
			//resp->answer_ = j.dump();
		}
		else
		{
			resp->status_ = http::responses::no_content;
		}
	}
}


/*********************************
// Logs sync
*********************************/
void Kalel::LogSync(http_request* req, http_response* resp)
{
	if (req->method_ == http::method::get)
	{
		// Figure out which range of logs to send by finding the requested timestamp

		std::map<std::chrono::system_clock::time_point, std::string>::iterator it;
		auto localCollection = storageVectors.getInfoLogs();

		if (req->params_.empty() ||									// If parameters don't exist, send all the logs
			req->params_.at("time").empty() ||
			req->params_.at("time") == "start")
		{
			it = localCollection.begin();
		}
		else
		{
			it = localCollection.upper_bound(timeh::StringToTimePoint(req->params_.at("time")));
		}

		if (it != localCollection.end())							// If iterator is valid, send requested logs
		{
			//json j(localCollection);

			resp->status_ = http::responses::ok;
			resp->content_type_ = http::mimetype::appjson;
			//resp->answer_ = j.dump();
		}
		else
		{
			resp->status_ = http::responses::no_content;
		}
	}
}


/*********************************
// Logs sync
*********************************/
void Kalel::RequestSync(http_request* req, http_response* resp)
{
	if (req->method_ == http::method::get)
	{
		// Figure out which range of logs to send by finding the requested timestamp

		std::map<std::chrono::system_clock::time_point, std::string>::iterator it;
		auto localCollection = storageVectors.getInfoLogs();

		if (req->params_.empty() ||									// If parameters don't exist, send all the logs
			req->params_.at("time").empty() ||
			req->params_.at("time") == "start")
		{
			it = localCollection.begin();
		}
		else
		{
			it = localCollection.upper_bound(timeh::StringToTimePoint(req->params_.at("time")));
		}

		if (it != localCollection.end())							// If iterator is valid, send requested logs
		{
			//json j(localCollection);

			resp->status_ = http::responses::ok;
			resp->content_type_ = http::mimetype::appjson;
			//resp->answer_ = j.dump();
		}
		else
		{
			resp->status_ = http::responses::no_content;
		}
	}
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