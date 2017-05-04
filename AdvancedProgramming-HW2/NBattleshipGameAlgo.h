#pragma once
#include "CommonBattleshipGameAlgo.h"

class NBattleshipGameAlgo : public CommonBattleshipGameAlgo
{
public:
	NBattleshipGameAlgo() : CommonBattleshipGameAlgo() {}
	bool init(const std::string& path) override { return true; }

};
