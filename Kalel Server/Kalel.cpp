#include "stdafx.h"
#include "Kalel.h"

#include "Parameters/Parametres.h"
#include "../Kalel Shared/Netcode/json.hpp"

using json = nlohmann::json;

#define STRINGIFY(var) (#var)

Kalel::Kalel()
	: threadManager{ handles }
	, machineSettings{ new MachineSettings() }
{
	handles.logCollection = &logCollection;
	handles.errorCollection = &errorCollection;
	handles.dataCollection = &dataCollection;

	// Populate Machine Settings
	machineSettings->CaloName								= GetNomCalo();
	machineSettings->CaloEntete								= GetEnteteCalo();
	machineSettings->ActivationSecurite						= GetActivationSecurite();
	machineSettings->CheminFichierGeneral					= GetCheminFichierGeneral();
	machineSettings->HighPressureToMeasure					= GetMesureHautePression();
	machineSettings->LowPressureToMeasure					= GetMesureBassePression();
	machineSettings->NumberInstruments						= GetNumberInstruments();
	machineSettings->PortKeithley							= GetPortKeithley();
	machineSettings->PortMensor								= GetPortMensor();
	machineSettings->PortTemperatures						= GetPortTemperatures();
	machineSettings->PortVannes								= GetPortVannes();
	machineSettings->PresenceTuyereSonique					= GetPresenceTuyereSonique();
	machineSettings->PressionLimiteVide						= GetPressionLimiteVide();
	machineSettings->PressionSecuriteBassePression			= GetPressionSecuriteBassePression();
	machineSettings->PressionSecuriteHautePression			= GetPressionSecuriteHautePression();
	machineSettings->SensibiliteCalo						= GetSensibiliteCalo();
	machineSettings->SensibiliteCapteurBassePression		= GetSensibiliteCapteurBassePression();
	machineSettings->SensibiliteCapteurHautePression		= GetSensibiliteCapteurHautePression();
	machineSettings->VolumeP6								= GetVolumeP6();
	machineSettings->VolumeRef								= GetVolumeRef();
	for (int i = 0; i < machineSettings->NumberInstruments; i++)
	{
		machineSettings->COMInstruments.push_back(GetCOMInstrument(i));
		machineSettings->FunctionInstruments.push_back(GetFonctionInstrument(i));
		machineSettings->typeInstruments.push_back(GetTypeInstrument(i));
	}

	// Check to see whether the parameters file has been created
	VerifParametres();

	// Start server functionality
	server.SetLogs(serverLogs);
	auto func = std::bind(&Kalel::ServerProcessing, this, std::placeholders::_1, std::placeholders::_2);
	try	{
		server.Listen();
		server.Accept(func);
	}
	catch (const std::exception& e) {
		std::string err (e.what());
		serverLogs.push_back("Fatal Error: Cannot listen to sockets, Details:" + err);
	}

	for (size_t i = 0; i < 5; i++)
	{
		auto temp = std::make_shared<ExperimentData>();
		temp->experimentGraphPoints = i + 1;
		dataCollection.push_back(temp);
	}
}


Kalel::~Kalel()
{
}

void Kalel::GetLogs(std::vector<std::string> &logs) {
	logs.clear();
	logs.insert(logs.begin(), serverLogs.begin(), serverLogs.end());
}


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
		j["CaloName"]							= unicodeConverter.ws2s(machineSettings->CaloName);
		j["CaloEntete"]							= unicodeConverter.ws2s(machineSettings->CaloEntete);
		j["CheminFichierGeneral"]				= unicodeConverter.ws2s(machineSettings->CheminFichierGeneral);
		j["ActivationSecurite"]					= machineSettings->ActivationSecurite;
		j["HighPressureToMeasure"]				= machineSettings->HighPressureToMeasure;
		j["LowPressureToMeasure"]				= machineSettings->LowPressureToMeasure;
		j["NumberInstruments"]					= machineSettings->NumberInstruments;
		j["PortKeithley"]						= machineSettings->PortKeithley;
		j["PortMensor"]							= machineSettings->PortMensor;
		j["PortTemperatures"]					= machineSettings->PortTemperatures;
		j["PortVannes"]							= machineSettings->PortVannes;
		j["PresenceTuyereSonique"]				= machineSettings->PresenceTuyereSonique;
		j["PressionLimiteVide"]					= machineSettings->PressionLimiteVide;
		j["PressionSecuriteBassePression"]		= machineSettings->PressionSecuriteBassePression;
		j["PressionSecuriteHautePression"]		= machineSettings->PressionSecuriteHautePression;
		j["SensibiliteCalo"]					= machineSettings->SensibiliteCalo;
		j["SensibiliteCapteurBassePression"]	= machineSettings->SensibiliteCapteurBassePression;
		j["SensibiliteCapteurHautePression"]	= machineSettings->SensibiliteCapteurHautePression;
		j["VolumeP6"]							= machineSettings->VolumeP6;
		j["VolumeRef"]							= machineSettings->VolumeRef;
		j["COMInstruments"]						= machineSettings->COMInstruments;
		j["FunctionInstruments"]				= machineSettings->FunctionInstruments;
		j["typeInstruments"]					= machineSettings->typeInstruments;

		resp->answer_ = j.dump();
	}

	else if (req->path_ == "/api/machinesettings" && req->method_ == http::method::post) {
		if (req->content_type_ == http::mimetype::appjson) {
			resp->status_ = http::responses::ok;

			auto j = json::parse(req->entity_);

			machineSettings->CaloName								 = unicodeConverter.s2ws(j["CaloName"]);
			machineSettings->CaloEntete								 = unicodeConverter.s2ws(j["CaloEntete"]);
			machineSettings->ActivationSecurite						 = j["ActivationSecurite"];				
			machineSettings->CheminFichierGeneral					 = unicodeConverter.s2ws(j["CheminFichierGeneral"]);
			machineSettings->HighPressureToMeasure					 = j["HighPressureToMeasure"];				
			machineSettings->LowPressureToMeasure					 = j["LowPressureToMeasure"];				
			machineSettings->NumberInstruments						 = j["NumberInstruments"];					
			machineSettings->PortKeithley							 = j["PortKeithley"];						
			machineSettings->PortMensor								 = j["PortMensor"];							
			machineSettings->PortTemperatures						 = j["PortTemperatures"];					
			machineSettings->PortVannes								 = j["PortVannes"];							
			machineSettings->PresenceTuyereSonique					 = j["PresenceTuyereSonique"];				
			machineSettings->PressionLimiteVide						 = j["PressionLimiteVide"];					
			machineSettings->PressionSecuriteBassePression			 = j["PressionSecuriteBassePression"];		
			machineSettings->PressionSecuriteHautePression			 = j["PressionSecuriteHautePression"];		
			machineSettings->SensibiliteCalo						 = j["SensibiliteCalo"];					
			machineSettings->SensibiliteCapteurBassePression		 = j["SensibiliteCapteurBassePression"];	
			machineSettings->SensibiliteCapteurHautePression		 = j["SensibiliteCapteurHautePression"];	
			machineSettings->VolumeP6								 = j["VolumeP6"];							
			machineSettings->VolumeRef								 = j["VolumeRef"];							
			machineSettings->COMInstruments							 = j["COMInstruments"].get<std::vector<int>>();						
			machineSettings->FunctionInstruments					 = j["FunctionInstruments"].get<std::vector<int>>();
			machineSettings->typeInstruments						 = j["typeInstruments"].get<std::vector<int>>();	

			// Write everything to file
			SetNomCalo(								machineSettings->CaloName						); 
			SetEnteteCalo(							machineSettings->CaloEntete						); 
			SetActivationSecurite(					machineSettings->ActivationSecurite				); 
			SetCheminFichierGeneral(				machineSettings->CheminFichierGeneral			); 
			SetMesureHautePression(					machineSettings->HighPressureToMeasure			); 
			SetMesureBassePression(					machineSettings->LowPressureToMeasure			); 
			SetNumberInstruments(					machineSettings->NumberInstruments				); 
			SetPortKeithley(						machineSettings->PortKeithley					); 
			SetPortMensor(							machineSettings->PortMensor						); 
			SetPortTemperatures(					machineSettings->PortTemperatures				); 
			SetPortVannes(							machineSettings->PortVannes						); 
			SetPresenceTuyereSonique(				machineSettings->PresenceTuyereSonique			); 
			SetPressionLimiteVide(					machineSettings->PressionLimiteVide				); 
			SetPressionSecuriteBassePression(		machineSettings->PressionSecuriteBassePression	); 
			SetPressionSecuriteHautePression(		machineSettings->PressionSecuriteHautePression	); 
			SetSensibiliteCalo(						machineSettings->SensibiliteCalo				); 
			SetSensibiliteCapteurBassePression(		machineSettings->SensibiliteCapteurBassePression); 
			SetSensibiliteCapteurHautePression(		machineSettings->SensibiliteCapteurHautePression); 
			SetVolumeP6(							machineSettings->VolumeP6						); 
			SetVolumeRef(							machineSettings->VolumeRef						);
			for (int i = 0; i < machineSettings->NumberInstruments; i++)
			{
				SetCOMInstrument(i, machineSettings->COMInstruments[i]);
				SetFonctionInstrument(i, machineSettings->FunctionInstruments[i]);
				SetTypeInstrument(i, machineSettings->typeInstruments[i]);
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
		
		std::vector<std::shared_ptr<ExperimentData>>::iterator it;

		if (req->params_.at("last") == "0")
		{
			it = dataCollection.begin();
		}
		else
		{
			double requestedLastMeasurement;
			std::stringstream is;
			is << req->params_.at("last");
			is >> requestedLastMeasurement;

			it = std::find_if(dataCollection.end(), dataCollection.begin(), 
				[&requestedLastMeasurement](std::shared_ptr<ExperimentData> d) -> bool {return d->experimentGraphPoints == requestedLastMeasurement; });
		}
		
		json j;

		while (it != dataCollection.end())
		{
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(experimentInProgress				)]	=	(*it)->experimentInProgress			;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(experimentRecording				)]	=	(*it)->experimentRecording			;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(experimentWaiting					)]	=	(*it)->experimentWaiting			;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(experimentCommandsRequested		)]	=	(*it)->experimentCommandsRequested	;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(experimentStage					)]	=	(*it)->experimentStage				;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(verificationStep					)]	=	(*it)->verificationStep				;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(experimentStepStatus				)]	=	(*it)->experimentStepStatus			;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(experimentSubstepStage			)]	=	(*it)->experimentSubstepStage		;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(experimentDose					)]	=	(*it)->experimentDose				;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(experimentGraphPoints				)]	=	(*it)->experimentGraphPoints		;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(experimentPreviousStage			)]	=	(*it)->experimentPreviousStage		;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(experimentTime					)]	=	(*it)->experimentTime				;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(experimentTimeStart				)]	=	(*it)->experimentTimeStart			;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(timeToEquilibrate					)]	=	(*it)->timeToEquilibrate			;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(timeToEquilibrateCurrent			)]	=	(*it)->timeToEquilibrateCurrent		;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(injectionAttemptCounter			)]	=	(*it)->injectionAttemptCounter		;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(adsorptionCounter					)]	=	(*it)->adsorptionCounter			;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(desorptionCounter					)]	=	(*it)->desorptionCounter			;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(pressureInitial					)]	=	(*it)->pressureInitial				;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(pressureFinal						)]	=	(*it)->pressureFinal				;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(pressureHighOld					)]	=	(*it)->pressureHighOld				;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(resultCalorimeter					)]	=	(*it)->resultCalorimeter			;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(pressureLow						)]	=	(*it)->pressureLow					;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(pressureHigh						)]	=	(*it)->pressureHigh					;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(temperatureCalo					)]	=	(*it)->temperatureCalo				;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(temperatureCage					)]	=	(*it)->temperatureCage				;
			j[std::to_string((*it)->experimentGraphPoints)][	STRINGIFY(temperatureRoom					)]	=	(*it)->temperatureRoom				;

			++it;
		}

		resp->answer_ = j.dump();

	}

	else if (req->path_ == "/api/results") {
	}

	else {
		resp->status_ = http::responses::not_found;
	}
}