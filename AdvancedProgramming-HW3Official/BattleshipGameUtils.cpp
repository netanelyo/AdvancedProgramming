#include "BattleshipGameUtils.h"

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
