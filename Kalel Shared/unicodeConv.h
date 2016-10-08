#pragma once

#include <string>

class UnicodeConv
{
public:
	UnicodeConv();
	~UnicodeConv();
	static std::wstring s2ws(const std::string & str);
	static std::string ws2s(const std::wstring & wstr);
};

