#ifndef BASE64_H
#define BASE64_H
#pragma once

#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

#endif