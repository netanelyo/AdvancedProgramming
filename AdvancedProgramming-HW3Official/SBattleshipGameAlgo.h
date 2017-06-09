#pragma once
#include "IBattleshipGameAlgo.h"
#include "BattleshipGameUtils.h"
#include <map>
#include "PlayerBoard.h"
#include <vector>

class SBattleshipGameAlgo : public IBattleshipGameAlgo
{
public:
	SBattleshipGameAlgo() : IBattleshipGameAlgo(), m_currentAttackingShip({ -1, -1, -1 }),
							m_lastAttackDirection(Direction::NON), m_randomState(true),
							m_isDone(false), m_newBorn(true), m_playerID(-1) {}

	void notifyOnAttackResult(int player, Coordinate coor, AttackResult result) override;

	Coordinate attack() override;

	void setBoard(const BoardData& board) override;

	void setPlayer(int player) override { m_playerID = player; }

	bool isRandom() const { return m_randomState; }

private:
	Coordinate	m_currentAttackingShip;
	Direction	m_lastAttackDirection;
	bool		m_randomState;
	bool		m_isDone;
	bool		m_newBorn;
	int			m_playerID;
	PlayerBoard m_board;
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
	void markShipsBoarders();
	void reset();
	bool newBorn() const { return m_newBorn; }
	static std::string generateKey(const Coordinate& coor);
	static Coordinate retrieveCoordinateFromString(const std::string& str);
	//bool hasShipInPerimiter(Coordinate coor, int axis, int upperBound) const;
};
