#pragma once
#include "CommonBattleshipGameAlgo.h"
#include <set>
#include <map>

class SBattleshipGameAlgo : public CommonBattleshipGameAlgo
{
public:
	SBattleshipGameAlgo() : CommonBattleshipGameAlgo(),	m_currentAttackingShip(-1, -1),
						m_sequentialState(true), m_lastAttackDirection(Direction::NON) {}

	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;

	std::pair<int, int> attack() override;

	bool isSequntial() const { return m_sequentialState; }

private:
	std::pair<int, int>		m_currentAttackingShip;
	bool		m_sequentialState;
	Direction	m_lastAttackDirection;
	std::set<std::pair<int, int>> m_preferredAttackSquares;
	std::map<std::pair<int, int>, Direction> m_attackHelperMap;

	void markOppSankShip(int row, int col, bool& changed);
	//void missSetUpForNextAttack();
	void markRecursiveCall(int row, int col, bool& changed);
	void findNextMove();
	void hitNotify(int row, int col, int player);
	void sinkNotify(int row, int col, int player);
	std::pair<int, int> findNextEmptySquare(int row, int col, Direction nextAttackDirection);
	std::pair<int, int> findNextIterative(int row, int col, Direction nextAttackDirection) const;
};
