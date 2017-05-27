#pragma once

#include "CommonBattleshipGameAlgo.h"
#include <list>
#include <set>



/**
 * class FBattleshipGameAlgo - a derived class of CommonBattleshipGameAlgo that plays
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
	FBattleshipGameAlgo(const FBattleshipGameAlgo& b) = delete;
	FBattleshipGameAlgo& operator=(const FBattleshipGameAlgo& b) = delete;

	/**
	* @overrides CommonBattleshipGameAlgo::setBoard virtual function
	*
	* Copies board[i][j] to this->board[i][j]
	*
	* @param player - the player's index as given by game manager
	* @param board - the board to copy of
	* @param numRows - board's number of rows
	* @param numCols - board's number of columns
	*/
	void setBoard(int player, const char** board, int numRows, int numCols) override;


	/**
	* @overrides CommonBattleshipGameAlgo::init virtual function
	*
	* finds an attack file and initializes our moves for the game
	* 
	* @param path - the directory path in which to look for the attack file
	* @return 
	* false in case no attack file was found or the file couldn't be opened
	* otherwise, true
	*/
	bool init(const std::string& path) override;

	/**
	* @overrides CommonBattleshipGameAlgo::attack virtual function
    * 
	* gets the next attack move, if exists. 
	* @return
	* <-1, -1> in case we have no more moves;
	* otherwise - a pair of positive integers as the next attack  
	*/
	std::pair<int, int> attack() override;				


private:
	std::list<std::string>	m_playerMoves;
	bool					m_sameMovesFile = false;

	const static std::string ATTACK_SUFFIX;

	/**
	* inserts to a FIFO queue all the lines read from the attack file
	*
	* @param filePath - the path of the attack file
	* @return 
	* false if the file couldn't be opened
	* otherwise, true
	*/
	bool makeMovesQueue(const std::string& filePath);

	/**
	* parses the given line and checks it contains valid game board coordinates
	* 
	* @param str - the line to validate
	* @param coord - the coordinates to be updated in case the line is a valid attack line
	* @return 
	* true if the line contains a valid game board coordinates
	* otherwise, false
	*/
	bool parseLineAndValidate(const std::string & str, std::pair<int, int>& coord) const;
};



