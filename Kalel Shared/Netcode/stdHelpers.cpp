#include "Forcelib.h"
#include "stdHelpers.h"

#include <algorithm>
#include <cctype>

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
