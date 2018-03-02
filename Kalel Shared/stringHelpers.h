#pragma once

#include <string>
#include <sstream>


namespace stringh {

	std::string ReplaceInStr(const std::string& in, const std::string& search_for, const std::string& replace_with);

	void ToUpper(std::string& s);
	void ToLower(std::string& s);

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

	std::wstring s2ws(const std::string & str);
	std::string ws2s(const std::wstring & wstr);


	template<typename ... Args>
	std::string string_format(const std::string& format, Args ... args);
}
