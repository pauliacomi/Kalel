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


std::string TimeTToStringLocal(const time_t & t)
{
	struct tm gmt;
	localtime_s(&gmt, &t);

	std::string time;
	std::stringstream str;
	str << std::put_time(&gmt, "%c");
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
	auto tt = std::chrono::system_clock::to_time_t(tp);
	std::string  str_time = TimeTToStringLocal(tt);

	auto epoch = tp.time_since_epoch();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
	auto fractional_seconds = ms.count() % 1000;

	std::string str_frsec = "." + std::to_string(fractional_seconds);
	str_time += str_frsec;

	return str_time;
}


std::chrono::system_clock::time_point StringToTimePoint(std::string str_time)
{
	auto fractional_seconds = To<int>(str_time.substr(str_time.size() - 3, 3));
	str_time.erase(str_time.size() - 4, 4);

	struct tm tm;
	std::stringstream ss(str_time);
	ss >> std::get_time(&tm, "%d/%b/%Y %H:%M:%S");
	auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
	if (ss.fail()) {
		true;
	}
	
	tp += std::chrono::milliseconds(fractional_seconds);

	ss.clear();
	ss << std::put_time(&tm, "%c");
	std::string check = TimePointToString(tp);
	check = ss.str();

	return tp;
}

//unsigned long long TimePointToULLong(std::chrono::system_clock::time_point tp)
//{
//	auto tt = std::chrono::system_clock::to_time_t(tp);
//	auto str_time = TimeTToString(tt);
//
//	auto epoch = tp.time_since_epoch();
//	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
//	auto fractional_seconds = ms.count() % 1000;
//	str_time += ".";
//	str_time += fractional_seconds;
//
//	return str_time;
//}


//std::chrono::system_clock::time_point ULLongToTimePoint(unsigned long long start, unsigned long long duration)
//{
//	auto fractional_seconds = To<int>(str_time.substr(str_time.size() - 3));
//
//	std::tm tm = {};
//	std::stringstream ss(str_time.substr(str_time.size() - 20));
//	ss >> std::get_time(&tm, "%w, %b %d %Y %H:%M:%S");
//	auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
//
//	tp += std::chrono::milliseconds(fractional_seconds);
//
//	return tp;
//}

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
