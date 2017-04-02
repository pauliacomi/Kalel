
// This file should be rewritten as a class in the future

#include "Parametres.h"
#include "../../Kalel Shared/Resources/DefineInstruments.h"
#include "../../Kalel Shared/Com Classes/MachineSettings.h"
#include "../../Kalel Shared/Netcode/json.hpp"
#include "../../Kalel Shared/Com Classes/Serialization.h"

#include <fstream>

using json = nlohmann::json;

#define Fichier_parametres "../Parametres.ini"


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
	fs.open(Fichier_parametres, std::fstream::in | std::fstream::out | std::fstream::app);

	json j;
	j << fs;
	fs.close();

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
