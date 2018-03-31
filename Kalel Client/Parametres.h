#ifndef PARAMETRES_H
#define PARAMETRES_H
#pragma once

#include <string>

class Parameters {

	//constructor
public:
	Parameters();
	~Parameters();

	void Initialisation();

	// Get

	std::wstring GetUsername();
	std::wstring GetPassword();
	std::wstring GetServerAddress();
	std::wstring GetServerPort();
	int GetDataRefreshInterval();
	int GetGraphRefreshInterval();

	// Set

	void SetServerAddress(const std::wstring &address);
	void SetServerPort(const std::wstring &port);
	void SetCredentials(const std::wstring & username, const std::wstring & password);
	void SetDataRefreshInterval(int interval);
	void SetGraphRefreshInterval(int interval);
	int GetRemoteFolderLocation();
	void SetRemoteFolderLocation(const std::wstring &location);
protected:
	const wchar_t * fileLocation;
};


#endif