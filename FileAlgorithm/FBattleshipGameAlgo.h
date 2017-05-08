#pragma once

#include "CommonBattleshipGameAlgo.h"
#include <list>
#include <set>



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

	/**
	* @implements IBattleshipGameAlgo::setBoard pure virtual function
	*
	* Copies board[i][j] to this->board[i][j]
	*
	* @param player - the player's index as given by game manager
	* @param board - the board to copy of
	* @param numRows - board's number of rows
	* @param numCols - board's number of columns
	*/
	void setBoard(int player, const char** board, int numRows, int numCols) override;


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


private:
	std::list<std::string>	m_playerMoves;
	bool					m_sameMovesFile = false;

	const static std::string ATTACK_SUFFIX;

	bool makeMovesQueue(const std::string& filePath);
	bool parseLineAndValidate(const std::string & str, std::pair<int, int>& coord) const;
};



