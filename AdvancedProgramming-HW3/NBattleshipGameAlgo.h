#pragma once
#include "CommonBattleshipGameAlgo.h"

/** 
*  class NBattleshipGameAlgo - a derived class of CommonBattleshipGameAlgo that plays
* a battleship game in a naive way
*/
class NBattleshipGameAlgo : public CommonBattleshipGameAlgo
{
public:
	NBattleshipGameAlgo() : CommonBattleshipGameAlgo() {}
	NBattleshipGameAlgo(const NBattleshipGameAlgo& b) = delete;
	NBattleshipGameAlgo& operator=(const NBattleshipGameAlgo& b) = delete;

};
