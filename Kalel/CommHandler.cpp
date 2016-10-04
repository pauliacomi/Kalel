#include "stdafx.h"
#include "CommHandler.h"

#include "Com Classes/ManualActionParam.h"
#include "Netcode/http_request.h"
#include "Netcode/Client.h"
#include "Resources/StringTable.h"
#include "Netcode/json.hpp"

#include <functional>

using json = nlohmann::json;

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
		client.Request(request, callback, unicodeConverter.ws2s(address.c_str()));
	}
	catch (const std::exception& e)	{
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK,false, unicodeConverter.s2ws(e.what()));
	}
}

void CommHandler::SaveAddress(std::wstring address)
{
	localAddress = unicodeConverter.ws2s(address.c_str());
}

void CommHandler::Sync()
{
	GetMachineSettings();
	//GetData();
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
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, unicodeConverter.s2ws(e.what()));
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
		messageHandler.DisplayMessageBox(GENERIC_STRING, MB_ICONERROR | MB_OK, false, unicodeConverter.s2ws(e.what()));
	}
}


void CommHandler::GetData()
{
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
	r->method_ = "GET";
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
	r->method_ = "GET";
	r->path_ = "/api/machinesettings";
	r->accept_ = "application/json";
}

void CommHandler::GetMachineSettings_resp(http_response* r) {

	if (r->status_ == http::responses::ok)
	{
		if (r->content_type_ == "application/json") {
			auto j = json::parse(r->answer_);
			MachineSettings receivedSettings;

			receivedSettings.CaloName								 = unicodeConverter.s2ws(j["CaloName"]);
			receivedSettings.CaloEntete								 = unicodeConverter.s2ws(j["CaloEntete"]);
			receivedSettings.ActivationSecurite						 = j["ActivationSecurite"];				
			receivedSettings.CheminFichierGeneral					 = unicodeConverter.s2ws(j["CheminFichierGeneral"]);
			receivedSettings.HighPressureToMeasure					 = j["HighPressureToMeasure"];				
			receivedSettings.LowPressureToMeasure					 = j["LowPressureToMeasure"];				
			receivedSettings.NumberInstruments						 = j["NumberInstruments"];					
			receivedSettings.PortKeithley							 = j["PortKeithley"];						
			receivedSettings.PortMensor								 = j["PortMensor"];							
			receivedSettings.PortTemperatures						 = j["PortTemperatures"];					
			receivedSettings.PortVannes								 = j["PortVannes"];							
			receivedSettings.PresenceTuyereSonique					 = j["PresenceTuyereSonique"];				
			receivedSettings.PressionLimiteVide						 = j["PressionLimiteVide"];					
			receivedSettings.PressionSecuriteBassePression			 = j["PressionSecuriteBassePression"];		
			receivedSettings.PressionSecuriteHautePression			 = j["PressionSecuriteHautePression"];		
			receivedSettings.SensibiliteCalo						 = j["SensibiliteCalo"];					
			receivedSettings.SensibiliteCapteurBassePression		 = j["SensibiliteCapteurBassePression"];	
			receivedSettings.SensibiliteCapteurHautePression		 = j["SensibiliteCapteurHautePression"];	
			receivedSettings.VolumeP6								 = j["VolumeP6"];							
			receivedSettings.VolumeRef								 = j["VolumeRef"];							
			receivedSettings.COMInstruments							 = j["COMInstruments"].get<std::vector<int>>();						
			receivedSettings.FunctionInstruments					 = j["FunctionInstruments"].get<std::vector<int>>();
			receivedSettings.typeInstruments						 = j["typeInstruments"].get<std::vector<int>>();
			receivedSettings.synced									 = true;

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
	r->path_			= "/api/machinesettings";
	r->content_type_	= "application/json";

	json j;
	j["CaloName"]							= unicodeConverter.ws2s(localSettings->CaloName);
	j["CaloEntete"]							= unicodeConverter.ws2s(localSettings->CaloEntete);
	j["CheminFichierGeneral"]				= unicodeConverter.ws2s(localSettings->CheminFichierGeneral);
	j["ActivationSecurite"]					= localSettings->ActivationSecurite;
	j["HighPressureToMeasure"]				= localSettings->HighPressureToMeasure;
	j["LowPressureToMeasure"]				= localSettings->LowPressureToMeasure;
	j["NumberInstruments"]					= localSettings->NumberInstruments;
	j["PortKeithley"]						= localSettings->PortKeithley;
	j["PortMensor"]							= localSettings->PortMensor;
	j["PortTemperatures"]					= localSettings->PortTemperatures;
	j["PortVannes"]							= localSettings->PortVannes;
	j["PresenceTuyereSonique"]				= localSettings->PresenceTuyereSonique;
	j["PressionLimiteVide"]					= localSettings->PressionLimiteVide;
	j["PressionSecuriteBassePression"]		= localSettings->PressionSecuriteBassePression;
	j["PressionSecuriteHautePression"]		= localSettings->PressionSecuriteHautePression;
	j["SensibiliteCalo"]					= localSettings->SensibiliteCalo;
	j["SensibiliteCapteurBassePression"]	= localSettings->SensibiliteCapteurBassePression;
	j["SensibiliteCapteurHautePression"]	= localSettings->SensibiliteCapteurHautePression;
	j["VolumeP6"]							= localSettings->VolumeP6;
	j["VolumeRef"]							= localSettings->VolumeRef;
	j["COMInstruments"]						= localSettings->COMInstruments;
	j["FunctionInstruments"]				= localSettings->FunctionInstruments;
	j["typeInstruments"]					= localSettings->typeInstruments;

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