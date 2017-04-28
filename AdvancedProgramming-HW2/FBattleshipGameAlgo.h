#pragma once

#include "CommonBattleshipGameAlgo.h"
#include <forward_list>
#include <set>
#include <fstream>
#include <cctype>
#include "dirent.h"

//#define BOARD_SIZE 10


/**
 * class FBattleshipGameAlgo - implements IBattleshipGameAlgo by playing
 * a battleship game out of input files (board and moves)
 */
class FBattleshipGameAlgo : public CommonBattleshipGameAlgo
{
public:

	/**
	 * Constructor
	 */
	FBattleshipGameAlgo() : CommonBattleshipGameAlgo() {}

	/**
	 * Destructor
	 */
	~FBattleshipGameAlgo() {}

	bool init(const std::string& path) override;

	/**
	 * @implements IBattleshipGameAlgo::attack pure virtual function
	 * 
	 * reads from file the next attack move, if exists. 
	 * @return
	 * <-1, -1> in case of EOF;
	 * otherwise - a pair of positive integers as the next attack  
	 */
	std::pair<int, int> attack() override;				

	/**
	* @implements IBattleshipGameAlgo::notifyOnAttackResult pure virtual function
	*
	* updates board if the attack result was a hit/sink of our ship
	* and decreases shipCounter by one if the attack result was a sink of our ship
	*
	* @param player - an integer represting the current player
	* @param row - the X coordinate of the attack
	* @param col - the Y coordinate of the attck
	* @param result - the attack result
	* 
	*/
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
private:
	std::forward_list<std::string> m_playerMoves;

	const static std::string ATTACK_SUFFIX;

	bool makeMovesQueue(const std::string& filePath);
};



