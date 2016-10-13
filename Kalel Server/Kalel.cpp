#include "stdafx.h"
#include "Kalel.h"

#include "Parameters/Parametres.h"
#include "../Kalel Shared/Netcode/json.hpp"
#include "../Kalel Shared/Com Classes/Serialization.h"

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


	//// temp
	/*time_t t = time(0);
	for (size_t i = 0; i < 50; i++)
	{
		ExperimentData something;
		something.measurementsMade = i + 1;
		something.timeStart = t;
		storageVectors.dataCollection.push_back(std::make_shared<ExperimentData>(something));
	}*/
}


Kalel::~Kalel()
{
}

void Kalel::GetLogs(std::vector<std::string> * &logs) {
	logs = &storageVectors.serverLogs;
}


/*void Kalel::GetLogs(std::vector<std::string> &logs) {
	logs.clear();
	logs.insert(logs.begin(), storageVectors.serverLogs.begin(), storageVectors.serverLogs.end());
}*/

void Kalel::ServerProcessing(http_request* req, http_response* resp) {

	if (req->path_		== "/api/handshake") {
		resp->status_ = http::responses::ok;
		resp->content_type_ = http::mimetype::texthtml;
		resp->answer_ = R"(<!DOCTYPE html PUBLIC " -//IETF//DTD HTML 2.0//EN"><html><head><title>Hello</title></head><body><h1>Hello</h1></body></html>)";
	}

	else if (req->path_ == "/api/machinesettings" && req->method_ == http::method::get) {
		resp->status_ = http::responses::ok;
		resp->content_type_ = http::mimetype::appjson;
		
		json j;
		serialization::serializeMachineSettingsToJSON(*storageVectors.machineSettings, j);
		resp->answer_ = j.dump();
	}

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

	else if (req->path_ == "/api/experimentdata") {
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
			time_t experimentStartTime;
			std::stringstream is;
			is << req->params_.at("start");
			is >> experimentStartTime;
			is.clear();

			if (experimentStartTime != localCollection.begin()->get()->timeStart) {
				it = localCollection.rend();
			}
			else
			{
				long int numberOfMeasurements;
				is << req->params_.at("measurements");
				is >> numberOfMeasurements;
				it = std::find_if(localCollection.rbegin(), localCollection.rend(),
					[&numberOfMeasurements](std::shared_ptr<ExperimentData> d) -> bool {return d->measurementsMade == numberOfMeasurements; });
			}
		}
		
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
	
	else {
		resp->status_ = http::responses::not_found;
	}
}