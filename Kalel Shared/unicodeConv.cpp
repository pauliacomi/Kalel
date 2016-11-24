#include "unicodeConv.h"

#include <locale>			// wstring/string conversions
#include <codecvt>			// wstring/string conversions

UnicodeConv::UnicodeConv()
{
}


UnicodeConv::~UnicodeConv()
{
}

std::wstring UnicodeConv::s2ws(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8_utf16<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

std::string UnicodeConv::ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8_utf16<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}
