
#include "stdafx.h"
#include <iostream>
#include <signal.h>

#include "Kalel.h"
#include "../Kalel Shared/log.h"

#define LOG_LEVEL		logDEBUG4			// Change the level of logging here
#define FILE_LOGGING	"server.log"		// Comment this line to disable file logging


bool runnning = true;
#ifdef _WIN32

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

	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();

	std::cout << "Starting server...\n";
	std::cout << "Press [Ctrl+C] to exit\n";

	Kalel mainBackend;

	Sleep(100);

	while (runnning) {

		std::cerr.sync_with_stdio(false);

		std::map<std::chrono::system_clock::time_point, std::string> elogs = mainBackend.storage.eventLogs.get(tp);
		std::map<std::chrono::system_clock::time_point, std::string> ilogs = mainBackend.storage.infoLogs.get(tp);
		tp = std::chrono::system_clock::now();

		for (const auto &iter : ilogs) {
			std::cerr << timeh::TimePointToISOString(iter.first) << " " << iter.second << std::flush;
		}
		for (const auto &iter : elogs) {
			std::cerr << timeh::TimePointToISOString(iter.first) << " " << iter.second << std::flush;
		}


		Sleep(500);
	}
	std::cout << "Stopped";

	return 0;
}
