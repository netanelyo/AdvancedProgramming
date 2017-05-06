#pragma once
#include "CommonBattleshipGameAlgo.h"

#include <map>
#include <set>
#include <array>

using Player = CommonBattleshipGameAlgo;

enum class GameState 
{
	GAME_OVER, 
	CONTINUE_PLAYING
};

/**
* a class that manages a Battle Ship game between two players  
*/
class Game
{
public:
	/**
	* Constructor
	*/
	Game(): m_nextPlayer(0) {}
	~Game() { /*TODO delete pointers*/ } //destructor

	/**
	*
	* parses the boardFile and creates a board for the game
	*
	* @param boardFile - a file stream containing the board
	* @return
	* true if the board isn't a proper game board 
	* otherwise false
	*/
	bool checkAndCreateBoard(std::ifstream & boardFile);

	/**
	* plays one game move by asking the current player for his next attack and 
	* determining the result of this attack
	*
	* @return 
	* GAME_OVER if after the current move the game ended  
	* otherwise CONTINUE_PLAYING 
	*/
	GameState playMove();

	/**
	 * creates and sets a board for each player, which contains only the player's ships .
	 * 
	 * @param player - player's index
	 */
	void createBoardsForPlayers(int player);

	bool loadAndInitPlayersFromDLL(const std::string& path, std::set<std::string> dllSet);

private:

	class GameConstants
	{
	public:
		static const int BOARD_SIZE = 10;

		static const int TOO_FEW_PLAYER_A_INDEX = 8;
		static const int TOO_MANY_PLAYER_A_INDEX = 9;
		static const int TOO_FEW_PLAYER_B_INDEX = 10;
		static const int TOO_MANY_PLAYER_B_INDEX = 11;
		static const int ADJACENT_SHIPS_INDEX = 12;

		const static size_t RUBBER_BOAT_POINTS = 2;
		const static size_t MISSILE_BOAT_POINTS = 3;
		const static size_t SUBMARINE_POINTS = 7;
		const static size_t DESTROYER_POINTS = 8;

		const static size_t RUBBER_BOAT_LEN = 1;
		const static size_t MISSILE_BOAT_LEN = 2;
		const static size_t SUBMARINE_LEN = 3;
		const static size_t DESTROYER_LEN = 4;

		static const std::string WRONG_SIZE_B_PLAYER_A;
		static const std::string WRONG_SIZE_P_PLAYER_A;
		static const std::string WRONG_SIZE_M_PLAYER_A;
		static const std::string WRONG_SIZE_D_PLAYER_A;
		static const std::string WRONG_SIZE_b_PLAYER_B;
		static const std::string WRONG_SIZE_p_PLAYER_B;
		static const std::string WRONG_SIZE_m_PLAYER_B;
		static const std::string WRONG_SIZE_d_PLAYER_B;
		static const std::string TOO_FEW_PLAYER_A;
		static const std::string TOO_MANY_PLAYER_A;
		static const std::string TOO_FEW_PLAYER_B;
		static const std::string TOO_MANY_PLAYER_B;
		static const std::string ADJACENT_SHIPS;
	};

	std::array<Player*, 2>	m_players;
	int						m_nextPlayer;
	char					m_gameBoard[GameConstants::BOARD_SIZE][GameConstants::BOARD_SIZE];

	void readBoardFromFile(std::ifstream& boardFile);

	void checkIfBoardIsValid(int shouldPrint[], std::map<char, int>& shipsErrorMsgMap);

	void checkShipsQuantity(int shouldPrint[]) const;

	bool canPassTurn(int player) const { return (player ? !(m_players[0]->isDone()) : !(m_players[1]->isDone())); }

	/**
	* prints the game result when the game is over.
	* if there's a winner, prints who won and the points of each player
	* otherwise, prints only the points of each player
	* @param winner - an integer representing the winning player. In case of a tie, winner = -1
	*/
	void printEndOfGame(int winner) const; 

	/**
	* checks if both players don't have any more moves
	* @return
	* true if both players don't have any more moves
	* otherwise false
	*/
	bool endOfAttacks() const; 

	/**
	* determines  if the attack result  was a sink or a hit
	* 
	* @param rowCoord - the X coordinate of the current attack
	* @param colCoord - the Y coordinate of the current attack
	* @return
	* SINK in case the attack result was a sink
	* otherwise HIT 
	*/
	AttackResult determineAttackResult(char square, int rowCoord, int colCoord) const;

	/**
	* adds points to a player if necessary (if a sink occurred)
	* and sets the next player (=the player who has the next move)
	*
	* @param result - the current attack result 
	* @param ship - the symbol of the current attacked ship
	* @param isAship - a boolean represnting if the sinked ship belongs to player A
	* @param canPassTurn - a boolean representing if the next turn can be passed to the other player
	*/
	void handlePointsAndNextTurn(AttackResult result, char ship, int currentPlayer,
																bool isAship, bool canPassTurn);

	/**
	*
	* searches for all neighboring appearances of currShip on board in order to determine the validity of the board
	*
	* @param currShip - the symbol of the current ship
	* @param dummy - a binary matrix 
	* @param row - the currShip X coordinate
	* @param col - the currShip Y coordinate
	* @param len - the current length of currShip on board
	* @param direction - an enum representing the expected direction of currShip on board 
	* @param invalidShape - a boolean representing if the shape of currShip is invalid
	* @param adjShips - a boolean representing if there are adjacent ships on board
	*/
	void dfsShip(char currShip, int dummy[][GameConstants::BOARD_SIZE], int row,
			int col, int& shipLen, Direction direction, int& invalidShape, int& adjShips) const;
	/**
	* an helper function of dfsShip that finds all neighboring appearances of currShip on board
	*
	* @param currShip - the symbol of the current ship
	* @param dummy - a binary matrix 
	* @param row - the currShip X coordinate
	* @param col - the currShip Y coordinate
	* @param invalidShape - a boolean representing if the shape of currShip is invalid
	* @param adjShips - a boolean representing if there are adjacent ships on board
	*/
	void markAllOfSameShip(char currShip, int dummy[][GameConstants::BOARD_SIZE], int row,	int col,
					int& invalidShape, int& adjShips) const; 

	/**
	*
	* prints the flaged errors 
	*
	* @param shouldPrint - a binary array. If shouldPrint[i] = 1, errors[1] is printed; 
	* @return 
	* true if an error was printed
	* otherwise false
	*/
	static bool printErrors(int shouldPrint[], std::string errors[]);

	/**
	* removes a sunk ship from the game board
	*
	* @param xCoord -  X coordinate of the sunk ship 
	* @param yCoord -  Y coordinate of the sunk current 
	*/
	void removeSankShip(int xCoord, int yCoord);

	/**
	* returns the length of ship
	* @param ship - the symbol of the ship
	* @return 
	* 0 in case the symbol doesn't belong to any ship
	* otherwise the ship's length
	*/
	static size_t getShipLen(char ship);
	

};