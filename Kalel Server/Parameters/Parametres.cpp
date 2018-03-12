
#include "Parametres.h"
#include "../../Kalel Shared/log.h"
#include "../../Kalel Shared/Netcode/json.hpp"
#include "../../Kalel Shared/Resources/DefineInstruments.h"
#include "../../Kalel Shared/Com Classes/MachineSettings.h"
#include "../../Kalel Shared/Com Classes/Serialization.h"

#include <fstream>

using json = nlohmann::json;

#define FILE_SETTINGS "server_settings.json"


bool ParametersCheck()
{
	std::ifstream file_stream(FILE_SETTINGS, std::ifstream::in);
	bool ans = file_stream.is_open();
	file_stream.close();

	return ans;
}

void ParametersGet(MachineSettings& settings)
{
	std::fstream fs;
	std::string filestr;

	fs.open(FILE_SETTINGS, std::fstream::in);

	fs.seekg(0, std::ios::end);
	filestr.reserve(fs.tellg());
	fs.seekg(0, std::ios::beg);
	filestr.assign((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

	fs.close();
	json j;
	try
	{
		j = json::parse(filestr.c_str());
		settings = j;
	}
	catch (const std::exception& e)
	{
		settings = MachineSettings();
		LOG(logINFO) << "Could not read settings file: " << e.what();
		return;
	}
}

void ParametersSet(const MachineSettings& settings)
{
	std::fstream fs;
	fs.open(FILE_SETTINGS, std::fstream::out | std::ofstream::trunc);

	json j = settings;
	fs << j.dump(4);		// Pretty print
	fs.close();
}
