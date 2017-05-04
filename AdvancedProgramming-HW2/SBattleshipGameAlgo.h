#pragma once
#include "CommonBattleshipGameAlgo.h"
#include <unordered_set>
#include <unordered_map>

using _coordinate = std::pair<int, int>;

class SBattleshipGameAlgo : public CommonBattleshipGameAlgo
{
public:
	SBattleshipGameAlgo() : CommonBattleshipGameAlgo(), m_currentSeqMove(0, -1),
		m_currentAttackingShip(-1, -1), m_sequentialState(true) {}

	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;

	_coordinate attack() override;

private:
	_coordinate m_currentSeqMove;
	_coordinate m_currentAttackingShip;
	bool m_sequentialState;
	std::unordered_set<_coordinate> m_preferredAttackSquares;
	std::unordered_map <_coordinate, std::tuple<Direction, _coordinate>> m_attackHelperMap;
};
