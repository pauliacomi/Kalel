#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#pragma once

#define default_val -1	// This value is used as the default value for the optional parameters of the message display function. 
// Make sure that it is set to a value that the actual parameters can never take

#include <string>
#include <memory>

#define MB_OK                       0x00000000L
#define MB_OKCANCEL                 0x00000001L
#define MB_ABORTRETRYIGNORE         0x00000002L
#define MB_YESNOCANCEL              0x00000003L
#define MB_YESNO                    0x00000004L
#define MB_RETRYCANCEL              0x00000005L
#define MB_CANCELTRYCONTINUE        0x00000006L
#define MB_ICONHAND                 0x00000010L
#define MB_ICONQUESTION             0x00000020L
#define MB_ICONEXCLAMATION          0x00000030L
#define MB_ICONASTERISK             0x00000040L
#define MB_ICONWARNING              MB_ICONEXCLAMATION
#define MB_ICONERROR                MB_ICONHAND
#define MB_ICONINFORMATION          0x00000040L


class ExperimentData;
class Storage;

class MessageHandler
{
	// Constructor and destructor
public:
	MessageHandler(Storage &h);
	virtual ~MessageHandler();

	// Message postback
	Storage & storage;

public:

	// PostMessage functions
	bool ExperimentStart();
	bool ExperimentEnd();
	bool ThreadShutdown();
	bool ExchangeData(const ExperimentData &pParam);

	bool DisplayMessage(std::string pParam, int pInt1 = default_val, int pInt2 = default_val, double pDouble = default_val);							// Simple display message which takes an int
	bool DisplayMessage(std::string pParam, std::string m);

	// Messageboxes
	bool DisplayMessageBox(std::string message, unsigned nType, bool blocksProgram, double pDouble1 = default_val, double pDouble2 = default_val);
	bool DisplayMessageBox(std::string pParam, unsigned nType, bool blocksProgram, std::string pString);
};


#endif // !MESSAGEHANDLER_H