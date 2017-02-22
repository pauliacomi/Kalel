#include "timeHelpers.h"

#include "Netcode/stdHelpers.h"

#include <iomanip>

static const char *DAY_NAMES[] =
{ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char *MONTH_NAMES[] =
{ "Jan", "Feb", "Mar", "Apr", "May", "Jun",
"Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

static const char *DATE_FORMAT = "%Y-%m-%d %H:%M:%S";

std::chrono::system_clock::time_point NowTime()
{
	auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	return now_ms;
}


std::string GMTtime(int format)
{
	time_t t = std::time(nullptr);

	if (format == RFC_1123)
	{
		auto RFC1123_TIME_LEN = 29;
		struct tm tm;
		char * buf = new char[RFC1123_TIME_LEN + 1];

		gmtime_s(&tm, &t);

		strftime(buf, RFC1123_TIME_LEN + 1, "---, %d --- %Y %H:%M:%S GMT", &tm);
		memcpy(buf, DAY_NAMES[tm.tm_wday], 3);
		memcpy(buf + 8, MONTH_NAMES[tm.tm_mon], 3);

		std::string time(buf);
		delete[] buf;

		return time;
	}

	if (format == ANSI)
	{
		return TimeTToStringGMT(t);
	}
	return std::string();
}



std::string TimeTToStringLocal(const time_t & t)
{
	struct tm gmt;
	localtime_s(&gmt, &t);

	std::string time;
	std::stringstream str;
	str << std::put_time(&gmt, DATE_FORMAT);
	time = str.str();

	return time;
}



std::string TimeTToStringGMT(const time_t & t)
{
	struct tm gmt;
	gmtime_s(&gmt, &t);

	std::string time;
	std::stringstream str;
	str << std::put_time(&gmt, "%c %Z");
	time = str.str();

	return time;
}



std::string TimePointToString(const std::chrono::system_clock::time_point & tp)
{
	// Build the main time string
	auto tt = std::chrono::system_clock::to_time_t(tp);
	auto str_time = TimeTToStringLocal(tt);

	// Build the milisecond string
	auto epoch = tp.time_since_epoch();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
	auto fractional_seconds = ms.count() % 1000;

	std::string str_frsec = ".";
	if (fractional_seconds < 100)
	{
		str_frsec += "0";
	}
	str_frsec += std::to_string(fractional_seconds);

	// Return complete string
	return str_time + str_frsec;
}


std::chrono::system_clock::time_point StringToTimePoint(const std::string & str_time)
{
	// Cut the fractional seconds part
	auto fractional_seconds = To<int>(str_time.substr(str_time.size() - 3, 3));
	auto str_time_nofrac = str_time.substr(str_time.size() - 4, 4);

	// Generate the regular timepoint
	struct std::tm tm_time;
	std::stringstream ss(str_time_nofrac);
	ss >> std::get_time(&tm_time, DATE_FORMAT);
	auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm_time));
	
	// Add the remaining miliseconds
	tp += std::chrono::milliseconds(fractional_seconds);

	return tp;
}



unsigned long long TimePointToULLong(const std::chrono::system_clock::time_point & tp)
{
	auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(tp);
	auto value = tp.time_since_epoch();
	return value.count();
}


std::chrono::system_clock::time_point ULLongToTimePoint(unsigned long long tp)
{
	std::chrono::milliseconds dur(tp);
	std::chrono::system_clock::time_point dt(dur);

	return dt;
}


timer::timer(void)
{
}

timer::~timer(void)
{
}

// Start the timer object
void timer::Start()
{
	timeElapsed.zero();
	startTP = std::chrono::high_resolution_clock::now();
}

// Pause the timer object
void timer::Pause()
{
	auto endTP = std::chrono::high_resolution_clock::now();
	timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTP - startTP);
}

// Resume timer
void timer::Resume()
{
	startTP = std::chrono::high_resolution_clock::now();
}

int timer::TimeMilliseconds() const
{
	return timeElapsed.count();
}

int timer::TimeSeconds() const
{
	return std::chrono::duration_cast<std::chrono::seconds>(timeElapsed).count();
}

int timer::TimeMinutes() const
{
	return std::chrono::duration_cast<std::chrono::minutes>(timeElapsed).count();
}


