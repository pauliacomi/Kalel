#ifndef PARAMETRES_H
#define PARAMETRES_H
#pragma once

#include <string>

#define Dev_USB_Temp 2
#define Dev_USB_Vannes 1

void VerifParametres();
void Initialisation();

// Get

std::wstring GetServerAddress();
std::wstring GetServerPort();

// Set

void SetServerAddress(std::wstring address);
void SetServerPort(std::wstring port);

#endif