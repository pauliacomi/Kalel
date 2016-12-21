#ifndef PARAMETRES_H
#define PARAMETRES_H
#pragma once

#include <string>

class Parameters {

	//constructor
public:
	Parameters();
	~Parameters();

	void VerifParametres();
	void Initialisation();

	// Get

	std::wstring GetServerAddress();
	std::wstring GetServerPort();
	int GetDataRefreshInterval();
	int GetGraphRefreshInterval();

	// Set

	void SetServerAddress(std::wstring address);
	void SetServerPort(std::wstring port);
	void SetDataRefreshInterval(int interval);
	void SetGraphRefreshInterval(int interval);
protected:
	const wchar_t * fileLocation;
};


#endif