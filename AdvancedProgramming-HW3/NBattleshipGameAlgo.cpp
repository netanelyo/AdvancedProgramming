#include "NBattleshipGameAlgo.h"

IBattleshipGameAlgo* GetAlgorithm()
{
	return new NBattleshipGameAlgo();
}
