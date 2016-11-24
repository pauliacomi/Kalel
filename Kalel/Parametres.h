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

	// Set

	void SetServerAddress(std::wstring address);
	void SetServerPort(std::wstring port);

protected:
	const wchar_t * fileLocation;
};


#endif