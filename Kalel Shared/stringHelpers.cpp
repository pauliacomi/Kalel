#include "Forcelib.h"
#include "stringHelpers.h"

#include <locale>			// wstring/string conversions
#include <codecvt>			// wstring/string conversions
#include <algorithm>
#include <cctype>

#include <memory>
#include <iostream>
#include <cstdio>

#pragma warning(disable : 4996)		// Must disable the _sprintf error

namespace stringh {

	std::string ReplaceInStr(const std::string& in, const std::string& search_for, const std::string& replace_with) {
		std::string ret = in;

		std::string::size_type pos = ret.find(search_for);

		while (pos != std::string::npos) {
			ret = ret.replace(pos, search_for.size(), replace_with);
			pos = pos - search_for.size() + replace_with.size() + 1;
			pos = ret.find(search_for, pos);
		}

		return ret;
	}

	void ToUpper(std::string& s) {
		std::transform(s.begin(), s.end(), s.begin(), std::toupper);
	}

	void ToLower(std::string& s) {
		std::transform(s.begin(), s.end(), s.begin(), std::tolower);
	}


	using convert_type = std::codecvt_utf8_utf16<wchar_t>;

	std::wstring s2ws(const std::string& str)
	{
		std::wstring_convert<convert_type, wchar_t> converter;

		return converter.from_bytes(str);
	}

	std::string ws2s(const std::wstring& wstr)
	{
		std::wstring_convert<convert_type, wchar_t> converter;

		return converter.to_bytes(wstr);
	}



}
