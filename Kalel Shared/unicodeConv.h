#pragma once

#include <string>

class UnicodeConv
{
public:
	UnicodeConv();
	~UnicodeConv();
	std::wstring s2ws(const std::string & str);
	std::string ws2s(const std::wstring & wstr);
};

