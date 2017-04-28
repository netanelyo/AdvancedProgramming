#pragma once
#include "CommonBattleshipGameAlgo.h"

class NBattleshipGameAlgo : public CommonBattleshipGameAlgo
{
public:
	NBattleshipGameAlgo() : CommonBattleshipGameAlgo(), m_currentMove(-1, -1) {}
	void setBoard(int player, const char** board, int numRows, int numCols) override;
	bool init(const std::string& path) override { return true; }
	std::pair<int, int> attack() override;
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;



private:
	std::pair<int, int> m_currentMove;

	std::pair<int, int> getNextValidSqaure(int i, int j) const;

};
