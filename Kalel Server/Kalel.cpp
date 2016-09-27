#include "stdafx.h"
#include "Kalel.h"

#include "Parameters/Parametres.h"
#include "../Kalel Shared/Netcode/http_request.h"


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
	server.Listen();
	server.Accept(Get);
}


Kalel::~Kalel()
{
}

void Kalel::GetLogs(std::vector<std::string> &logs) {
	logs.clear();
	logs.insert(logs.begin(), serverLogs.begin(), serverLogs.end());
}


void Get(http_request* r) {

	if (r->path_ == "/api") {
		r->status_ = "202 OK";
	}
	else if (r->path_ == "/api/machinesettings") {
	}
	else if (r->path_ == "/api/experimentdata") {
	}
	else if (r->path_ == "/api/results") {
	}
	else {
		r->status_ = "404 Not Found";
	}
}