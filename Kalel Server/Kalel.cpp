#include "stdafx.h"
#include "Kalel.h"

#include "Parameters/Parametres.h"
#include "../Kalel Shared/Netcode/json.hpp"
#include "../Kalel Shared/Com Classes/Serialization.h"
#include "../Kalel Shared/Netcode/stdHelpers.h"

using json = nlohmann::json;

Kalel::Kalel()
	: threadManager{ storageVectors }
{
	//
	// Check to see whether the parameters file has been created
	VerifParametres();

	//
	// Create and populate Machine Settings
	storageVectors.machineSettings = std::make_shared<MachineSettings>();

	storageVectors.machineSettings->CaloName								= GetNomCalo();
	storageVectors.machineSettings->CaloEntete								= GetEnteteCalo();
	storageVectors.machineSettings->ActivationSecurite						= GetActivationSecurite();
	storageVectors.machineSettings->CheminFichierGeneral					= GetCheminFichierGeneral();
	storageVectors.machineSettings->HighPressureToMeasure					= GetMesureHautePression();
	storageVectors.machineSettings->LowPressureToMeasure					= GetMesureBassePression();
	storageVectors.machineSettings->NumberInstruments						= GetNumberInstruments();
	storageVectors.machineSettings->PortKeithley							= GetPortKeithley();
	storageVectors.machineSettings->PortMensor								= GetPortMensor();
	storageVectors.machineSettings->PortTemperatures						= GetPortTemperatures();
	storageVectors.machineSettings->PortVannes								= GetPortVannes();
	storageVectors.machineSettings->PresenceTuyereSonique					= GetPresenceTuyereSonique();
	storageVectors.machineSettings->PressionLimiteVide						= GetPressionLimiteVide();
	storageVectors.machineSettings->PressionSecuriteBassePression			= GetPressionSecuriteBassePression();
	storageVectors.machineSettings->PressionSecuriteHautePression			= GetPressionSecuriteHautePression();
	storageVectors.machineSettings->SensibiliteCalo							= GetSensibiliteCalo();
	storageVectors.machineSettings->SensibiliteCapteurBassePression			= GetSensibiliteCapteurBassePression();
	storageVectors.machineSettings->SensibiliteCapteurHautePression			= GetSensibiliteCapteurHautePression();
	storageVectors.machineSettings->VolumeP6								= GetVolumeP6();
	storageVectors.machineSettings->VolumeRef								= GetVolumeRef();
	for (int i = 0; i < storageVectors.machineSettings->NumberInstruments; i++)
	{
		storageVectors.machineSettings->COMInstruments.push_back(GetCOMInstrument(i));
		storageVectors.machineSettings->FunctionInstruments.push_back(GetFonctionInstrument(i));
		storageVectors.machineSettings->typeInstruments.push_back(GetTypeInstrument(i));
	}

	//
	// Start server functionality
	server.SetLogs(storageVectors.serverLogs, storageVectors.serverLogsMtx);
	auto func = std::bind(&Kalel::ServerProcessing, this, std::placeholders::_1, std::placeholders::_2);
	server.Accept(func);

}


Kalel::~Kalel()
{
}


void Kalel::GetLogs(std::string &logs) {
	logs.clear();

	for (std::map<std::string, std::string>::iterator it = storageVectors.automationInfoLogs.begin(); it != storageVectors.automationInfoLogs.end(); ++it)
	{
		logs += it->first;
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

void Kalel::ServerProcessing(http_request* req, http_response* resp) {

	/*********************************
	// Ping
	*********************************/
	if (req->path_		== "/api/handshake" && req->method_ == http::method::get) {
		resp->status_ = http::responses::ok;
		resp->content_type_ = http::mimetype::texthtml;
		resp->answer_ = R"(<!DOCTYPE html PUBLIC " -//IETF//DTD HTML 2.0//EN"><html><head><title>Hello</title></head><body><h1>Hello</h1></body></html>)";
	}


	/*********************************
	// Get MachineSettings
	*********************************/
	else if (req->path_ == "/api/machinesettings" && req->method_ == http::method::get) {
		resp->status_ = http::responses::ok;
		resp->content_type_ = http::mimetype::appjson;
		
		json j;
		serialization::serializeMachineSettingsToJSON(*storageVectors.machineSettings, j);
		resp->answer_ = j.dump();
	}


	/*********************************
	// Set MachineSettings
	*********************************/
	else if (req->path_ == "/api/machinesettings" && req->method_ == http::method::post) {
		if (req->content_type_ == http::mimetype::appjson) {
			resp->status_ = http::responses::ok;

			auto j = json::parse(req->entity_);
			serialization::deserializeJSONtoMachineSettings(j, *storageVectors.machineSettings);	

			// Write everything to file
			SetNomCalo(								storageVectors.machineSettings->CaloName						); 
			SetEnteteCalo(							storageVectors.machineSettings->CaloEntete						); 
			SetActivationSecurite(					storageVectors.machineSettings->ActivationSecurite				); 
			SetCheminFichierGeneral(				storageVectors.machineSettings->CheminFichierGeneral			); 
			SetMesureHautePression(					storageVectors.machineSettings->HighPressureToMeasure			); 
			SetMesureBassePression(					storageVectors.machineSettings->LowPressureToMeasure			); 
			SetNumberInstruments(					storageVectors.machineSettings->NumberInstruments				); 
			SetPortKeithley(						storageVectors.machineSettings->PortKeithley					); 
			SetPortMensor(							storageVectors.machineSettings->PortMensor						); 
			SetPortTemperatures(					storageVectors.machineSettings->PortTemperatures				); 
			SetPortVannes(							storageVectors.machineSettings->PortVannes						); 
			SetPresenceTuyereSonique(				storageVectors.machineSettings->PresenceTuyereSonique			); 
			SetPressionLimiteVide(					storageVectors.machineSettings->PressionLimiteVide				); 
			SetPressionSecuriteBassePression(		storageVectors.machineSettings->PressionSecuriteBassePression	); 
			SetPressionSecuriteHautePression(		storageVectors.machineSettings->PressionSecuriteHautePression	); 
			SetSensibiliteCalo(						storageVectors.machineSettings->SensibiliteCalo					); 
			SetSensibiliteCapteurBassePression(		storageVectors.machineSettings->SensibiliteCapteurBassePression	); 
			SetSensibiliteCapteurHautePression(		storageVectors.machineSettings->SensibiliteCapteurHautePression	); 
			SetVolumeP6(							storageVectors.machineSettings->VolumeP6						); 
			SetVolumeRef(							storageVectors.machineSettings->VolumeRef						);
			for (int i = 0; i < storageVectors.machineSettings->NumberInstruments; i++)
			{
				SetCOMInstrument(i,					storageVectors.machineSettings->COMInstruments[i]				);
				SetFonctionInstrument(i,			storageVectors.machineSettings->FunctionInstruments[i]			);
				SetTypeInstrument(i,				storageVectors.machineSettings->typeInstruments[i]				);
			}

		}
		else {
			resp->status_ = http::responses::bad_request;
		}
	}


	/*********************************
	// Get Data
	*********************************/
	else if (req->path_ == "/api/experimentdata" && req->method_ == http::method::get) {
		resp->status_ = http::responses::ok;
		resp->content_type_ = http::mimetype::appjson;
		
		// Figure out which range of data to send by looking at the time requested
		
		std::deque<std::shared_ptr<ExperimentData>>::reverse_iterator it;
		storageVectors.sharedMutex.lock();
		auto localCollection = storageVectors.dataCollection;
		storageVectors.sharedMutex.unlock();

		if (req->params_.empty()			 ||
			req->params_.at("start").empty() || 
			req->params_.at("measurements").empty())
		{
			it = localCollection.rend();
		}
		else
		{
			time_t experimentStartTime = To<time_t>(req->params_.at("start"));

			if (experimentStartTime != localCollection.begin()->get()->timeStart) {
				it = localCollection.rend();
			}
			else
			{
				long int numberOfMeasurements = To<long int>(req->params_.at("measurements"));
				it = std::find_if(localCollection.rbegin(), localCollection.rend(),
					[&numberOfMeasurements](std::shared_ptr<ExperimentData> d) -> bool {return d->measurementsMade == numberOfMeasurements; });
			}
		}
		
		if (it != localCollection.rbegin())						// If iterator is valid, send requested logs
		{
			json j;

			while (it != localCollection.rbegin())
			{
				json j2;
				--it;
				serialization::serializeExperimentDataToJSON(*(*it), j2);
				j.push_back(json::object_t::value_type({ std::to_string((*it)->measurementsMade), j2 }));
			}
		resp->answer_ = j.dump();

		}
		else
		{
			resp->status_ = http::responses::no_content;
		}
	}


	/*********************************
	// Get Logs
	*********************************/
	else if (req->path_ == "/api/experimentlogs" && req->method_ == http::method::get) {
		resp->status_ = http::responses::ok;
		resp->content_type_ = http::mimetype::appjson;

		// Figure out which range of logs to send by finding the requested timestamp

		std::map<std::string, std::string>::iterator it;
		storageVectors.autoInfoLogsMutex.lock();
		auto localCollection = storageVectors.automationInfoLogs;
		storageVectors.autoInfoLogsMutex.unlock();

		if (req->params_.empty() ||									// If parameters don't exist, send all the logs
			req->params_.at("time").empty() ||
			req->params_.at("time") == "start")
		{
			it = localCollection.begin();
		}
		else
		{
			it = localCollection.find(req->params_.at("time"));
			it ++;
		}

		if (it != localCollection.end())						// If iterator is valid, send requested logs
		{
			json j;

			while (it != localCollection.end())
			{
				j[it->first] = it->second;
				++it;
			}

			resp->answer_ = j.dump();
		}
		else
		{
			resp->status_ = http::responses::no_content;
		}
	}


	/*********************************
	// Set ExperimentSettings
	*********************************/
	else if (req->path_ == "/api/experimentdata" && req->method_ == http::method::post) {
		if (req->content_type_ == http::mimetype::appjson) {
			resp->status_ = http::responses::ok;

			auto j = json::parse(req->entity_);
			serialization::deserializeJSONtoExperimentSettings(j, *storageVectors.experimentSettings);
		}
		else {
			resp->status_ = http::responses::bad_request;
		}
	}


	/*********************************
	// Thread Commands
	*********************************/
	else if (req->path_ == "/api/thread"  && req->method_ == http::method::post) {

		if (!req->params_.empty() ||
			!req->params_.at("action").empty())
		{
			if (req->params_.at("action") == "start")			{
				threadManager.StartThread();
			}
			else if (req->params_.at("action") == "shutdown")	{
				threadManager.ShutdownThread();
			}
			else if (req->params_.at("action") == "restart")	{
				//threadManager.ResetThread();
			}
			else if (req->params_.at("action") == "reset")		{
				threadManager.ResetThread();
			}
			else if (req->params_.at("action") == "pause")		{
				threadManager.PauseThread();
			}
			else if (req->params_.at("action") == "resume")		{
				threadManager.ResumeThread();
			}

			resp->status_ = http::responses::ok;
		}
		else
		{
			resp->status_ = http::responses::conflict;
		}
	}

	/*********************************
	// Instrument Commands
	*********************************/
	else if (req->path_ == "/api/instrument"  && req->method_ == http::method::post) {
		if (!req->params_.empty() ||
			!req->params_.at("type").empty() ||
			!req->params_.at("number").empty() ||
			!req->params_.at("active").empty())
		{
			// threadManager.ManualAction();
		}
		else
		{
			resp->status_ = http::responses::conflict;
		}
	}

	/*********************************
	// Debug
	*********************************/
	else if (req->path_ == "/api/debug/testconnection" && req->method_ == http::method::get) {
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

	/*********************************
	// Default
	*********************************/
	else {
		resp->status_ = http::responses::not_found;
	}
}