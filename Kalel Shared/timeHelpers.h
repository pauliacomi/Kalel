#pragma once

#define RFC_1123	1
#define ANSI		2

#include <ctime>
#include <chrono>
#include <string>
#include <atomic>

namespace timeh {

	std::chrono::system_clock::time_point NowTime();

	std::string GMTtime(int format);

	std::string TimeTToStringLocal(const time_t & t);
	std::wstring TimeTToWStringLocal(const time_t & t);

	std::string TimeTToStringGMT(const time_t & t);

	std::string TimePointToString(const std::chrono::system_clock::time_point & tp);
	std::wstring TimePointToWString(const std::chrono::system_clock::time_point & tp);

	std::chrono::system_clock::time_point StringToTimePoint(const std::string & str_time);

	unsigned long long TimePointToULLong(const std::chrono::system_clock::time_point & tp);

	std::chrono::system_clock::time_point ULLongToTimePoint(unsigned long long tp);


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
		std::chrono::milliseconds GenerateTE();
	};
}
