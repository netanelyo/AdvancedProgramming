#pragma once
#include "IBattleshipGameAlgo.h"
#include "BattleshipGameUtils.h"
#include <set>
#include <map>

class SBattleshipGameAlgo : public IBattleshipGameAlgo
{
public:
	SBattleshipGameAlgo() : IBattleshipGameAlgo(),	m_currentAttackingShip(-1, -1),
						m_randomState(true), m_lastAttackDirection(Direction::NON) {}

	void notifyOnAttackResult(int player, Coordinate coor, AttackResult result) override;

	Coordinate attack() override;

	void setBoard(const BoardData& board) override;

	void setPlayer(int player) override;

	bool isSequntial() const { return m_randomState; }

private:
	Coordinate	m_currentAttackingShip;
	bool		m_randomState;
	Direction	m_lastAttackDirection;
	std::set<Coordinate> m_preferredAttackSquares;
	std::map<Coordinate, Direction> m_attackHelperMap;

	void markOppSankShip(int row, int col, bool& changed);
	//void missSetUpForNextAttack();
	void markRecursiveCall(int row, int col, bool& changed);
	void findNextMove();
	void hitNotify(int row, int col, int player);
	void sinkNotify(int row, int col, int player);
	std::pair<int, int> findNextEmptySquare(int row, int col, Direction nextAttackDirection);
	std::pair<int, int> findNextIterative(int row, int col, Direction nextAttackDirection) const;
};
