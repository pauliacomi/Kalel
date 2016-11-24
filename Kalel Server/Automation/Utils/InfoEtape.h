#ifndef INFOETAPE_H
#define INFOETAPE_H
#pragma once

#include <string>

class InfoEtape
{
public :
	int Index;
	std::string Nom;
	int FctArret;

	InfoEtape(int Index, std::string Nom, int FctArret);

};

#endif