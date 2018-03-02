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

	// std:toupper and std::tolower are overloaded. Well...
	// http://gcc.gnu.org/ml/libstdc++/2002-11/msg00180.html
	char toLower_(char c) { return std::tolower(c); }
	char toUpper_(char c) { return std::toupper(c); }

	void ToUpper(std::string& s) {
		std::transform(s.begin(), s.end(), s.begin(), toUpper_);
	}

	void ToLower(std::string& s) {
		std::transform(s.begin(), s.end(), s.begin(), toLower_);
	}



	std::wstring s2ws(const std::string& str)
	{
		using convert_typeX = std::codecvt_utf8_utf16<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	std::string ws2s(const std::wstring& wstr)
	{
		using convert_typeX = std::codecvt_utf8_utf16<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}


	template<typename ... Args>
	std::string string_format(const std::string& format, Args ... args)
	{
		size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
		std::unique_ptr<char[]> buf(new char[size]);
		_snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
	}
}
