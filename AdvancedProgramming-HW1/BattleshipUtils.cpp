#include "BattleshipUtils.h"

#include <iostream>

bool parseLineAndValidate(const std::string& str, std::pair<int, int>& coord)
{
	std::string				tempStr;
	std::istringstream		strStream(str); /* To be used in getline()						  */
	std::string				tempArr[2];		/* To hold potential strings representing row/col */
	std::string::size_type	loc;
	int						num;
	int						cnt	= 0;

	/* Splitting str by ',' */
	loc = str.find_first_of(',');
	tempArr[0] = str.substr(0, loc);
	tempArr[1] = str.substr(loc + 1);

	cnt = 0;
	/* Checks if line is valid */
	for (auto s : tempArr)
	{
		tempStr = trimWhitespaces(s);	/* First, cleaning whitespaces		*/
		if (validateStr(tempStr))		/* Checks if it's a valid row/col	*/
		{
			num = std::stoi(tempStr);
			if (cnt)
				coord.second = num;
			else
				coord.first	 = num;
			cnt++;
		}
		else
		{
			return false;
		}
	}

	return true;
}

std::string trimWhitespaces(const std::string& str)
{
	std::string whitespace = " \t\r"; /* Whitespace, TAB or CR */

	/* Finds first char in str that is not a whitespace */
	const auto strBegin = str.find_first_not_of(whitespace);

	if (strBegin == std::string::npos) /* If there is none returns an empty string*/
		return "";

	/* Finds last char in str that is not a whitespace */
	const auto strEnd = str.find_last_not_of(whitespace);

	/* Range of chars in str with no leading/trailing whitespaces */
	const auto strRange = strEnd - strBegin + 1; 

	return str.substr(strBegin, strRange);
}

bool validateStr(const std::string& str)
{
	std::string::size_type len = str.length();
	if (len < 1 || len > 2) /* A valid row/col will be of length 1 or 2 */
		return false;
	else
	{
		if (len == 2 && !str.compare("10")) /* A valid row/col of length 2 must be "10" */
			return true;
		else if (len == 1)
		{
			/* A valid row/col of length 1 must be a digit > 0 */
			return (isdigit(str[0]) && str[0] != '0');
		}
		return false;
	}
}