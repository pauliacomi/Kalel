#include "timeHelpers.h"
#include "stringHelpers.h"

#include <iomanip>

namespace timeh {

	static const char *DAY_NAMES[] =
	{ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	static const char *MONTH_NAMES[] =
	{ "Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	static const char *ISO_DATE_FORMAT = "%Y-%m-%dT%H:%M:%S";
	static const wchar_t *W_ISO_DATE_FORMAT = L"%Y-%m-%dT%H:%M:%S";


	std::chrono::system_clock::time_point NowTime()
	{
		return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	}


	std::string GMTtimeRFC1123()
	{
		std::time_t t = std::time(nullptr);

		int RFC1123_TIME_LEN = 30;
		struct std::tm gmt;
		char * buf = new char[RFC1123_TIME_LEN];

		gmtime_s(&gmt, &t);

		strftime(buf, RFC1123_TIME_LEN, "---, %d --- %Y %H:%M:%S GMT", &gmt);
		memcpy(buf, DAY_NAMES[gmt.tm_wday], 3);
		memcpy(buf + 8, MONTH_NAMES[gmt.tm_mon], 3);

		std::string time(buf);
		delete[] buf;

		return time;
	}


	std::string GMTtimeANSI()
	{
		std::time_t t = std::time(nullptr);
		struct std::tm gmt;
		gmtime_s(&gmt, &t);

		std::stringstream time;
		time << std::put_time(&gmt, "%c %Z");

		return time.str();
	}


	std::string TimeTToISOString(const std::time_t & t)
	{
		struct std::tm gmt;
		localtime_s(&gmt, &t);

		std::string time;
		std::stringstream str;
		str << std::put_time(&gmt, ISO_DATE_FORMAT);
		time = str.str();

		return time;
	}

	std::wstring TimeTToISOWString(const std::time_t & t)
	{
		struct std::tm gmt;
		localtime_s(&gmt, &t);

		std::wstringstream time;
		time << std::put_time(&gmt, W_ISO_DATE_FORMAT);

		return time.str();
	}


	std::string TimePointToISOString(const std::chrono::system_clock::time_point & tp)
	{
		// Build the main time string
		std::stringstream timestr;
		timestr << TimeTToISOString(std::chrono::system_clock::to_time_t(tp));

		// Build the milisecond string
		auto fractional_seconds = TimePointToMs(tp) % 1000;

		timestr << ".";
		timestr << std::setfill('0') << std::setw(3) << fractional_seconds;
		timestr << 'Z';

		// Return complete string
		return timestr.str();
	}

	std::wstring TimePointToISOWString(const std::chrono::system_clock::time_point & tp)
	{
		// Build the main time string
		std::wstringstream timestr;
		timestr << TimeTToISOWString(std::chrono::system_clock::to_time_t(tp));

		// Build the milisecond string
		auto fractional_seconds = TimePointToMs(tp) % 1000;

		timestr << L".";
		timestr << std::setfill(L'0') << std::setw(3) << fractional_seconds;
		timestr << L'Z';

		// Return complete string
		return timestr.str();
	}

	std::chrono::system_clock::time_point ISOStringToTimePoint(const std::string & str_time)
	{
		// Generate the regular timepoint
		struct std::tm tm_time;
		std::stringstream ss(str_time.substr(0, str_time.size() - 5));
		ss >> std::get_time(&tm_time, ISO_DATE_FORMAT);
		auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm_time));

		// Add the miliseconds
		tp += std::chrono::milliseconds(stringh::To<int>(str_time.substr(str_time.size() - 4, 3)));

		return tp;
	}

	//
	//
	//		C++11 timepoints <-> milliseconds (ullong)
	//
	//

	unsigned long long TimePointToMs(const std::chrono::system_clock::time_point & tp)
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
	}

	std::chrono::system_clock::time_point MsToTimePoint(unsigned long long tp)
	{
		return std::chrono::system_clock::time_point(std::chrono::milliseconds(tp));
	}

	//
	//
	//		A simple timer class
	//
	//

	timer::timer(void)
	{
	}

	timer::~timer(void)
	{
	}

	// Start the timer object
	void timer::Start()
	{
		running = true;
		timeElapsedPrevious = std::chrono::milliseconds{ 0 };
		startTP = std::chrono::high_resolution_clock::now();
	}

	// Pause the timer object
	void timer::Pause()
	{
		running = false;
		timeElapsedPrevious += TimeElapsed();
	}

	// Resume timer
	void timer::Resume()
	{
		running = true;
		startTP = std::chrono::high_resolution_clock::now();
	}

	int timer::TimeMilliseconds()
	{
		auto totalTE = timeElapsedPrevious + TimeElapsed();
		return totalTE.count();
	}

	int timer::TimeSeconds()
	{
		auto totalTE = timeElapsedPrevious + TimeElapsed();
		return std::chrono::duration_cast<std::chrono::seconds>(totalTE).count();
	}

	int timer::TimeMinutes()
	{
		auto totalTE = timeElapsedPrevious + TimeElapsed();
		return std::chrono::duration_cast<std::chrono::minutes>(totalTE).count();
	}

	std::chrono::milliseconds timer::TimeElapsed()
	{
		if (running) {
			return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTP);
		}
		else
			return timeElapsedPrevious;
	}


}