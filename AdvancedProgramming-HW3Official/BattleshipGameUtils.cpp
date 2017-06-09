#include "BattleshipGameUtils.h"
#include <string>
#include <sstream>

bool BattleshipGameUtils::splitStringByDelimiter(const std::string & str, std::vector<int>& vec, char delim)
{
	std::vector<std::string> parsedVec;
	std::string tmp;
	auto firstLineStream = std::istringstream(str);
	while (std::getline(firstLineStream, tmp, delim))
		parsedVec.push_back(tmp);

	if (parsedVec.size() != 3)
		return false;
	try
	{
		for (const auto& num : parsedVec)
			vec.push_back(std::stoi(num));
	}
	catch (...)
	{
		return false;
	}
	return true;
}

int BattleshipGameUtils::getShipLength(char ship)
{
	switch (ship)
	{
	case 'B':
	case 'b':
		return Constants::RUBBER_BOAT_LEN;
	case 'P':
	case 'p':
		return Constants::MISSILE_BOAT_LEN;
	case 'M':
	case 'm':
		return Constants::SUBMARINE_LEN;
	case 'D':
	case 'd':
		return Constants::DESTROYER_LEN;
	default:
		return 0;
	}
}
