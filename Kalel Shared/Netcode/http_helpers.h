#pragma once

#include <string>

#define RFC_1123	1
#define ANSI		2

// Get time in GMT format
std::string GMTtime(int format);

// Gets the method of the request
std::string ParseMethod(const std::string &method);

// Gets the status code of the request
std::string ParseStatusCode(const std::string & statuscode);