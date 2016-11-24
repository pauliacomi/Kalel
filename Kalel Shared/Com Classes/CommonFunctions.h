#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H
#pragma once

#include <string>

class CommonFunctions
{
public:
	CommonFunctions();
	~CommonFunctions();

	std::wstring getDate();
	std::wstring getDateUnderline();
};

#endif