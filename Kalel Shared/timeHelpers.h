#pragma once

#include <ctime>
#include <chrono>
#include <string>

namespace timeh {

	std::chrono::system_clock::time_point NowTime();

	std::string GMTtimeRFC1123();
	std::string GMTtimeANSI();

	std::string TimeTToISOString(const std::time_t & t);
	std::wstring TimeTToISOWString(const std::time_t & t);

	std::string TimePointToISOString(const std::chrono::system_clock::time_point & tp);
	std::wstring TimePointToISOWString(const std::chrono::system_clock::time_point & tp);

	std::chrono::system_clock::time_point ISOStringToTimePoint(const std::string & str_time);

	unsigned long long TimePointToMs(const std::chrono::system_clock::time_point & tp);
	std::chrono::system_clock::time_point MsToTimePoint(const unsigned long long tp);


	//		A simple timer class
	class timer
	{
	public:
		timer(void);
		~timer(void);

	private:
		// Time elapsed since the start of the clock. Is not actually generated until called
		std::chrono::milliseconds timeElapsedPrevious;
		// Timepoint when the clock started
		std::chrono::high_resolution_clock::time_point startTP;
		// check if running
		bool running = false;

	public:
		void Start();
		void Pause();
		void Resume();

		int TimeMilliseconds();
		int TimeSeconds();
		int TimeMinutes();

	private:
		std::chrono::milliseconds TimeElapsed();
	};
}
