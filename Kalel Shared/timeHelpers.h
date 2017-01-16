#pragma once

#define RFC_1123	1
#define ANSI		2

#include <ctime>
#include <chrono>
#include <string>


std::chrono::system_clock::time_point NowTime();

std::string TimeTToString(time_t t);

std::string TimePointToString(std::chrono::system_clock::time_point tp);

std::chrono::system_clock::time_point StringToTimePoint(std::string str_time);

std::string GMTtime(int format);