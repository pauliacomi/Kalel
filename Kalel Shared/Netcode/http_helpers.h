#pragma once

#include <string>

// Gets the method of the request
std::string ParseMethod(const std::string &method);

// Gets the status code of the request
std::string ParseStatusCode(const std::string & statuscode);