#ifndef URLHELPER_H
#define URLHELPER_H
#pragma once

#include <string>
#include <map>

class URLHelper
{
public:
	URLHelper();
	~URLHelper();

	static void SplitUrl(std::string const& url, std::string& protocol, std::string& server, std::string& path);
	static bool RemoveProtocolFromUrl(std::string const& url, std::string& protocol, std::string& rest);
	static void SplitGetReq(std::string et_req, std::string& path, std::map<std::string, std::string>& params);
	static void BuildReq(std::string& request, std::string const& path, std::map<std::string, std::string> const& params);
};

#endif