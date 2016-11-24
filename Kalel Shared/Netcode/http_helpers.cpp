#include "http_helpers.h"
#include "http_define.h"

#include <time.h>

static const char *DAY_NAMES[] =
{ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char *MONTH_NAMES[] =
{ "Jan", "Feb", "Mar", "Apr", "May", "Jun",
"Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

std::string GMTtime(int format)
{
	if (format == RFC_1123)
	{
		const int RFC1123_TIME_LEN = 29;
		time_t t;
		struct tm tm;
		char * buf = (char*)malloc(RFC1123_TIME_LEN + 1);

		time(&t);
		gmtime_s(&tm, &t);

		strftime(buf, RFC1123_TIME_LEN + 1, "---, %d --- %Y %H:%M:%S GMT", &tm);
		memcpy(buf, DAY_NAMES[tm.tm_wday], 3);
		memcpy(buf + 8, MONTH_NAMES[tm.tm_mon], 3);

		std::string time(buf);
		delete buf;

		return time;
	}

	if (format = ANSI)
	{
		time_t ltime;
		time(&ltime);
		tm gmt;
		gmtime_s(&gmt, &ltime);
		char asctime_remove_nl[26];
		asctime_s(asctime_remove_nl, 26, &gmt);
		asctime_remove_nl[24] = 0;

		return std::string(asctime_remove_nl) + " GMT";
	}
	return std::string();
}



std::string ParseMethod(const std::string & method)
{
	if (method == http::method::get) {		
		return http::method::get;
	}
	else if (method == http::method::post) {
		return http::method::post;
	}
	else if (method == http::method::put) {
		return http::method::put;
	}
	else if (method == http::method::del) {
		return http::method::del;
	}
	else
	{
		return std::string();
	}
}


std::string ParseStatusCode(const std::string & statuscode)
{
	if (statuscode == http::responses::not_found.substr(0, 3)) {
		return http::responses::not_found;
	}
	else if (statuscode == http::responses::ok.substr(0, 3)) {
		return http::responses::ok;
	}
	else if (statuscode == http::responses::unauthorised.substr(0, 3)) {
		return http::responses::unauthorised;
	}
	else if (statuscode == http::responses::no_content.substr(0, 3)) {
		return http::responses::no_content;
	}
	else
	{
		return std::string();
	}
}