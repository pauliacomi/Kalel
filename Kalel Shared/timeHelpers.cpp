#include "timeHelpers.h"

#include "Netcode/stdHelpers.h"

#include <iomanip>

static const char *DAY_NAMES[] =
{ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char *MONTH_NAMES[] =
{ "Jan", "Feb", "Mar", "Apr", "May", "Jun",
"Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };


std::chrono::system_clock::time_point NowTime()
{
	auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	return now_ms;
}


std::string TimeTToString(time_t t)
{
	time(&t);
	tm gmt;
	gmtime_s(&gmt, &t);
	char asctime_remove_nl[26];
	asctime_s(asctime_remove_nl, 26, &gmt);
	asctime_remove_nl[24] = 0;

	return asctime_remove_nl;
}

std::string TimePointToString(std::chrono::system_clock::time_point tp)
{
	auto tt = std::chrono::system_clock::to_time_t(tp);
	auto str_time = TimeTToString(tt);

	auto epoch = tp.time_since_epoch();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
	auto fractional_seconds = ms.count() % 1000;
	str_time += ".";
	str_time += fractional_seconds;

	return str_time;
}


std::chrono::system_clock::time_point StringToTimePoint(std::string str_time)
{
	auto fractional_seconds = To<int>(str_time.substr(str_time.size() - 3));

	std::tm tm = {};
	std::stringstream ss(str_time.substr(str_time.size() - 20));
	ss >> std::get_time(&tm, "%w, %b %d %Y %H:%M:%S");
	auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));

	tp += std::chrono::milliseconds(fractional_seconds);

	return tp;
}

unsigned long long TimePointToULLong(std::chrono::system_clock::time_point tp)
{
	auto tt = std::chrono::system_clock::to_time_t(tp);
	auto str_time = TimeTToString(tt);

	auto epoch = tp.time_since_epoch();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
	auto fractional_seconds = ms.count() % 1000;
	str_time += ".";
	str_time += fractional_seconds;

	return str_time;
}


std::chrono::system_clock::time_point ULLongToTimePoint(unsigned long long start, unsigned long long duration)
{
	auto fractional_seconds = To<int>(str_time.substr(str_time.size() - 3));

	std::tm tm = {};
	std::stringstream ss(str_time.substr(str_time.size() - 20));
	ss >> std::get_time(&tm, "%w, %b %d %Y %H:%M:%S");
	auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));

	tp += std::chrono::milliseconds(fractional_seconds);

	return tp;
}

std::string GMTtime(int format)
{
	if (format == RFC_1123)
	{
		auto RFC1123_TIME_LEN = 29;
		time_t t;
		struct tm tm;
		char * buf = new char[RFC1123_TIME_LEN + 1];

		time(&t);
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
		time_t ltime = time(nullptr);
		return TimeTToString(ltime) + " GMT";
	}
	return std::string();
}
