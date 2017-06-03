#include "BoardDataImpl.h"
#include "BattleshipGameUtils.h"


char BoardDataImpl::charAt(Coordinate c) const
{
	auto sq = m_gameBoard.getBoardSquare(c);
	if (sq != BattleshipGameUtils::Constants::SPACE)
	{
		if ((isupper(sq) && m_player == 0) || (islower(sq) && m_player == 1))
			return sq;
		return BattleshipGameUtils::Constants::SPACE;
	}
	return sq;
}
