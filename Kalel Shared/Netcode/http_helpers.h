#pragma once

#include <string>

#define RFC_1123	1
#define ANSI		2

// Get time in GMT format
std::string GMTtime(int format);