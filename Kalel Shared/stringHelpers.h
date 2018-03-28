#pragma once

#include <string>
#include <sstream>


#pragma warning(disable : 4996)		// Must disable the _sprintf error

namespace stringh {

	std::string ReplaceInStr(const std::string& in, const std::string& search_for, const std::string& replace_with);

	void ToUpper(std::string& s);
	void ToLower(std::string& s);

	std::wstring s2ws(const std::string & str);
	std::string ws2s(const std::wstring & wstr);


	template <class T>
	T To(std::string const& s) {
		T ret;

		std::stringstream stream;
		stream << s;
		stream >> ret;

		return ret;
	}

	template<class T>
	std::string StringFrom(T const& t) {
		std::string ret;

		std::stringstream stream;
		stream << t;
		stream >> ret;

		return ret;
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
