#include "stdafx.h"
#include "CommonFunctions.h"


CommonFunctions::CommonFunctions()
{
}


CommonFunctions::~CommonFunctions()
{
}

CString CommonFunctions::getDate()
{
	time_t timestamp;
	struct tm t;

	timestamp = time(NULL);
	localtime_s(&t, &timestamp);

	char date_char[255];
	sprintf_s(date_char, "%02d/%02d/%d %02dh%02d", t.tm_mon + 1, t.tm_mday,
		1900 + t.tm_year, t.tm_hour, t.tm_min);

	return date_char;
}

CString CommonFunctions::getDateUnderline()
{
	time_t timestamp;
	struct tm t;

	timestamp = time(NULL);
	localtime_s(&t, &timestamp);

	char date_char[255];
	sprintf_s(date_char, "%d_%02d_%02d_%02dh%02d",
		1900 + t.tm_year, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min);

	return date_char;
}