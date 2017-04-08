#pragma once

#include <string>
#include <sstream>

bool parseLineAndValidate(const std::string& str, std::pair<int, int>& coord);
std::string trimWhitespaces(const std::string& str);
bool validateStr(const std::string& str);
