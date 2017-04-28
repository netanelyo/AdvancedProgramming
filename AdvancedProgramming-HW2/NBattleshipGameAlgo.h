#pragma once
#include "CommonBattleshipGameAlgo.h"

class NBattleshipGameAlgo : public CommonBattleshipGameAlgo
{
public:
	NBattleshipGameAlgo() : CommonBattleshipGameAlgo(), m_currentMove(0, -1) {}
	bool init(const std::string& path) override { return true; }
	std::pair<int, int> attack() override;

private:
	std::pair<int, int> m_currentMove;

};
