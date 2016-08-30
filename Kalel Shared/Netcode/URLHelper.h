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

	void SplitUrl(std::string const& url, std::string& protocol, std::string& server, std::string& path);
	bool RemoveProtocolFromUrl(std::string const& url, std::string& protocol, std::string& rest);
	void SplitGetReq(std::string et_req, std::string& path, std::map<std::string, std::string>& params);
};

#endif