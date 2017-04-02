
// This file should be rewritten as a class in the future

#include "Parametres.h"
#include "../../Kalel Shared/Netcode/json.hpp"
#include "../../Kalel Shared/Resources/DefineInstruments.h"
#include "../../Kalel Shared/Com Classes/MachineSettings.h"
#include "../../Kalel Shared/Com Classes/Serialization.h"

#include <fstream>

using json = nlohmann::json;

#define Fichier_parametres "Parametres.ini"


bool ParametersCheck()
{
	std::ifstream file_stream(Fichier_parametres, std::ifstream::in);
	bool ans = file_stream.is_open();
	file_stream.close();

	// si ans=TRUE , alors le fichier existe, pas besoin de le créer
	// si ans=FALSE, alors le fichier n'existe pas, on le crée et on initialise les parametres

	return ans;
}

void ParametersGet(MachineSettings& settings)
{
	std::fstream fs;
	std::string file;

	fs.open(Fichier_parametres, std::fstream::in | std::fstream::out | std::fstream::app);

	fs.seekg(0, std::ios::end);
	file.reserve(fs.tellg());
	fs.seekg(0, std::ios::beg);

	file.assign((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
	fs.close();

	json j;
	j.parse(file.c_str());
	try
	{
	}
	catch (const std::exception& e)
	{
		settings = MachineSettings();
	}
	std::string g = j.dump();
	settings = j;
}

void ParametersSet(const MachineSettings& settings)
{
	std::fstream fs;
	fs.open(Fichier_parametres, std::fstream::in | std::fstream::out | std::fstream::app);

	json j = settings;
	fs << j.dump(4);
	fs.close();
}
