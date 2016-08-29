#include "CommonFunctions.h"
#include <ctime>
#include <sstream>

CommonFunctions::CommonFunctions()
{
}


CommonFunctions::~CommonFunctions()
{
}

std::wstring CommonFunctions::getDate()
{
	time_t timestamp;
	struct tm t;

	timestamp = time(NULL);
	localtime_s(&t, &timestamp);

	std::wostringstream stringStream;

	stringStream	<< t.tm_mday			<< "/"
					<< t.tm_mon + 1			<< "/" 
					<< 1900 + t.tm_year		<< " "
					<< t.tm_hour			<< "h"
					<< t.tm_min				<< "min";

	return stringStream.str();
}

std::wstring CommonFunctions::getDateUnderline()
{
	time_t timestamp;
	struct tm t;

	timestamp = time(NULL);
	localtime_s(&t, &timestamp);

	std::wostringstream stringStream;

	stringStream	<< 1900 + t.tm_year		<< "_"
					<< t.tm_mon + 1			<< "_" 
					<< t.tm_mday			<< "_"
					<< t.tm_hour			<< "h_"
					<< t.tm_min				<< "min";

	return stringStream.str();
}