#pragma once

#define RFC_1123	1
#define ANSI		2

#include <ctime>
#include <chrono>
#include <string>


std::chrono::system_clock::time_point NowTime();

std::string GMTtime(int format);

std::string TimeTToStringLocal(const time_t & t);

std::string TimeTToStringGMT(const time_t & t);

std::string TimePointToString(const std::chrono::system_clock::time_point & tp);

std::chrono::system_clock::time_point StringToTimePoint(const std::string & str_time);

unsigned long long TimePointToULLong(const std::chrono::system_clock::time_point & tp);

std::chrono::system_clock::time_point ULLongToTimePoint(unsigned long long tp);