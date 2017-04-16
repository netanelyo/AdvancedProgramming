#pragma once

#include <string>
#include <iostream>

/**
 * Parses a line (std::string) to a pair of indices.
 * 
 * @param str - the line to parse
 * @param coord - to be assigned with indices
 * 
 * @return true - if line is valid, false - o.w.
 */
bool parseLineAndValidate(const std::string& str, std::pair<int, int>& coord);

/**
 * Trims leading and trailing whitespaces of str.
 * 
 * @param str - the string with potential redundant whitespaces
 * 
 * @return the substring of str without leading and trailing whitespaces
 */
std::string trimWhitespaces(const std::string& str);

/**
 * Checks if str represents a valid index for the battleship game
 * 
 * @param str - the string to check
 * 
 * @return true - if str is valid, false - o.w.
 */
bool validateStr(const std::string& str);
