#include "BoardDataImpl.h"
#include "BattleshipGameUtils.h"


char BoardDataImpl::charAt(Coordinate c) const
{
	auto sq = m_board.getBoardSquare(c);
	if (sq != BattleshipGameUtils::Constants::EMPTY_SIGN)
	{
		if ((isupper(sq) && m_player == BattleshipGameUtils::Constants::PLAYER_A) || 
			(islower(sq) && m_player == BattleshipGameUtils::Constants::PLAYER_B))
			return sq;
		return BattleshipGameUtils::Constants::EMPTY_SIGN;
	}
	return sq;
}
