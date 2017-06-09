#pragma once
#include "IBattleshipGameAlgo.h"
#include "BattleshipGameUtils.h"
#include <set>
#include <map>
#include <algorithm>
#include "PlayerBoard.h"
#include <vector>

class SBattleshipGameAlgo : public IBattleshipGameAlgo
{
public:
	SBattleshipGameAlgo() : IBattleshipGameAlgo(), m_currentAttackingShip({ -1, -1, -1 }),
							m_lastAttackDirection(Direction::NON), m_randomState(true) {}

	void notifyOnAttackResult(int player, Coordinate coor, AttackResult result) override;

	Coordinate attack() override;

	void setBoard(const BoardData& board) override;

	void setPlayer(int player) override {}

	bool isRandom() const { return m_randomState; }

private:
	Coordinate	m_currentAttackingShip;
	Direction	m_lastAttackDirection;
	bool		m_randomState;
	PlayerBoard m_board;
	bool		m_isDone;
	int			m_playerID;
	std::set<std::string>				m_preferredAttackSquares;
	std::vector<Coordinate>				m_possibleMoves;
	std::map<std::string, Direction>	m_attackHelperMap;

	void markOppSankShip(Coordinate coor, bool& changed);
	//void missSetUpForNextAttack();
	void markRecursiveCall(int row, int col, int depth, bool& changed);
	void findNextMove();
	void hitNotify(Coordinate coor, int player);
	void sinkNotify(Coordinate coor, int player);
	Coordinate findNextEmptySquare(int row, int col, int depth, Direction nextAttackDirection);
	bool shouldAttackSquare(int row, int col, int depth, Direction next, Direction otherA, Direction otherB) const;
	Coordinate findNextIterative(int row, int col, int depth, Direction nextAttackDirection) const;
	Coordinate getNextRandomSquare();
	bool isDone() const { return m_isDone; }
	int playerID() const { return m_playerID; }
	static std::string generateKey(const Coordinate& coor);
	static Coordinate retrieveCoordinateFromString(const std::string& str);
	//bool hasShipInPerimiter(Coordinate coor, int axis, int upperBound) const;
};
