#ifndef MACHINESETTINGS_H
#define MACHINESETTINGS_H
#pragma once

#include <string>
#include <vector>

class MachineSettings
{
public:
	MachineSettings();
	~MachineSettings();

	long double SensibiliteCalo;
	float SensibiliteCapteurBassePression;
	float SensibiliteCapteurHautePression;
	bool ActivationSecurite;
	float PressionSecuriteBassePression;
	float PressionSecuriteHautePression;
	float PressionLimiteVide;
	bool PresenceTuyereSonique;
	float VolumeRef;
	float VolumeP6;

	int NumberInstruments;

	std::vector<int> typeInstruments;
	std::vector<int> COMInstruments;
	std::vector<int> FunctionInstruments;

	int TypeInstrument(int num);
	int COMInstrument(int num);
	int FonctionInstrument(int num);

	int PortKeithley;
	int PortMensor;
	int PortVannes;
	int PortTemperatures;

	std::wstring CaloName;
	std::wstring CaloEntete;

	std::wstring CheminFichierGeneral;

	bool CaloToMeasure;
	bool LowPressureToMeasure;
	bool HighPressureToMeasure;

	void SetInstrument(int num, int TypeInstrument, int COMInstrument, int FonctionInstrument);
	void SetTypeInstrument(int num, int TypeInstrument);
	void SetCOMInstrument(int num, int COMInstrument);
	void SetFunctionInstrument(int num, int FonctionInstrument);
};

#endif