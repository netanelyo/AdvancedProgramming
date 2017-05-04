#pragma once
#include "CommonBattleshipGameAlgo.h"
#include <unordered_set>
#include <unordered_map>

using intPair = std::pair<int, int>;

class SBattleshipGameAlgo : public CommonBattleshipGameAlgo
{
public:
	SBattleshipGameAlgo() : CommonBattleshipGameAlgo(),	m_currentAttackingShip(-1, -1),
						m_sequentialState(true), m_lastAttackDirection(Direction::NON) {}

	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;

	intPair attack() override;

	bool isSequntial() const { return m_sequentialState; }

private:
	intPair		m_currentAttackingShip;
	bool		m_sequentialState;
	Direction	m_lastAttackDirection;
	std::unordered_set<intPair> m_preferredAttackSquares;
	std::unordered_map<intPair, std::tuple<Direction, intPair>> m_attackHelperMap;

	void markOppSankShip(int row, int col, bool& changed);
	void missSetUpForNextAttack();
	void markRecursiveCall(int row, int col, bool& changed);
	void findNextMove();
};
