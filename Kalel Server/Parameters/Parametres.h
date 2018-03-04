#pragma once

class MachineSettings;

bool ParametersCheck();
void ParametersGet(MachineSettings & settings);
void ParametersSet(const MachineSettings & settings);