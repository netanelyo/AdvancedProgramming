#pragma once
#include "IBattleshipGameAlgo.h"
#include "BattleshipCommon.h"

class NBattleshipGameAlgo : public IBattleshipGameAlgo
{
public:
	NBattleshipGameAlgo() : m_currentMove(-1, -1), m_points(0), m_shipCounter(0), m_isDone(false) {}
	//~NBattleshipGameAlgo();
	void setBoard(int player, const char** board, int numRows, int numCols) override;
	bool init(const std::string& path) override;
	std::pair<int, int> attack() override;
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;



private:
	char				m_board[BattleshipConstants::BOARD_SIZE][BattleshipConstants::BOARD_SIZE];
	std::pair<int, int> m_currentMove;
	size_t				m_points;
	size_t				m_shipCounter;
	bool				m_isDone;
	
	void setBoardCoord(int i, int j, char val) { m_board[i][j] = val; } //sets board[i][j] to val
	std::pair<int, int> getNextValidSqaure(int i, int j) const;

};
