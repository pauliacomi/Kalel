#ifndef PARAMETRES_H
#define PARAMETRES_H
#pragma once

class MachineSettings;

bool ParametersCheck();
void ParametersGet(MachineSettings & settings);
void ParametersSet(const MachineSettings & settings);

#endif