#pragma once
#include "CommonBattleshipGameAlgo.h"

class NBattleshipGameAlgo : public CommonBattleshipGameAlgo
{
public:
	NBattleshipGameAlgo() : CommonBattleshipGameAlgo() {}
	bool init(const std::string& path) override { return true; }

	std::pair<int, int> attack() override;

};
