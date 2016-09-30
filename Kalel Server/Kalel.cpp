#include "stdafx.h"
#include "Kalel.h"

#include "Parameters/Parametres.h"
#include "../Kalel Shared/Netcode/json.hpp"


Kalel::Kalel()
	: threadManager{handles}
{
	handles.logCollection = &logCollection;
	handles.errorCollection = &errorCollection;
	handles.dataCollection = &dataCollection;

	// Check to see whether the parameters file has been created
	VerifParametres();

	// Start server functionality
	server.SetLogs(serverLogs);
	try	{
		server.Listen();
		server.Accept(Get);
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


void Get(http_request* req, http_response* resp) {

	if (req->path_ == "/api/handshake") {
		resp->status_ = "202 OK";
		resp->content_type_ = "text/html";
		resp->answer_ = "<body><div>Handshake complete</div></body>";
	}

	else if (req->path_ == "/api/machinesettings") {
		resp->status_ = "202 OK";
		resp->content_type_ = "text/json";
		resp->answer_ = "";
	}

	else if (req->path_ == "/api/experimentdata") {
	}

	else if (req->path_ == "/api/results") {
	}

	else {
		resp->status_ = "404 Not Found";
	}
}