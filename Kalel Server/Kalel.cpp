#include "stdafx.h"
#include "Kalel.h"

#include "Parameters/Parametres.h"
#include "../Kalel Shared/Netcode/json.hpp"


using json = nlohmann::json;


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
		resp->content_type_ = "text/html";
		resp->answer_ = R"(<!DOCTYPE html PUBLIC " -//IETF//DTD HTML 2.0//EN"><html><head><title>Hello</title></head><body><h1>Hello</h1></body></html>)";
	}

	else if (req->path_ == "/api/machinesettings") {
		resp->status_ = http::responses::ok;
		resp->content_type_ = "application/json";
		
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

	else if (req->path_ == "/api/experimentdata") {
	}

	else if (req->path_ == "/api/results") {
	}

	else {
		resp->status_ = http::responses::not_found;
	}
}