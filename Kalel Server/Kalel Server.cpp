
#include "stdafx.h"
#include <iostream>
#include <signal.h>

#include "Kalel.h"
#include "../Kalel Shared/log.h"

#define LOG_LEVEL		logDEBUG4			// Change the level of logging here
#define FILE_LOGGING	"server.log"		// Comment this line to disable file logging


bool runnning = true;
#ifdef _WIN32						// Windows specific

#ifdef _DEBUG				


#define _CRTDBG_MAP_ALLOC			// Look at memory leaks
#include <stdlib.h>  
#include <crtdbg.h>
#define new DEBUG_NEW

#endif // DEBUG

BOOL WINAPI win32consoleHandler(DWORD signal) {

	if (signal == CTRL_C_EVENT ||
		signal == CTRL_BREAK_EVENT ||
		signal == CTRL_CLOSE_EVENT) {
		printf("Exitting...\n");
		runnning = false;
		return TRUE;
	}
	return FALSE;
}

#else

void unixcontrolHandler(int s) {
	printf("Caught %d, Exitting...\n", s);
	runnning = false;
}

#endif

int main(int argc, char** argv) {

#ifdef FILE_LOGGING
	FILELog::ReportingLevel() = LOG_LEVEL;
	FILE * f;
	fopen_s(&f, FILE_LOGGING, "w");
	Output2FILE::Stream() = f;
#endif // FILE_LOGGING

#ifdef _WIN32
	if (!SetConsoleCtrlHandler(win32consoleHandler, TRUE)) {
		printf("\nERROR: Could not set control handler");
	}
#else
	signal(SIGINT, unixcontrolHandler);
#endif

	std::cout << "Starting server...\n";
	std::cout << "Press [Ctrl+C] to exit\n";

	Kalel mainBackend;

	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();

	while (runnning) {

		std::map<std::chrono::system_clock::time_point, std::string> logs = mainBackend.storageVectors.debugLogs.get(tp);

		for (const auto &iter : logs) {
			std::cout << timeh::TimePointToString(iter.first) << " " << iter.second;
		}

		tp = std::chrono::system_clock::now();

		Sleep(500);
	}
	std::cout << "Stopped";

#ifdef _DEBUG				
	_CrtDumpMemoryLeaks();	// Memory leaks
#endif

	return 0;
}
