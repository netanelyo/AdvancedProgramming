#pragma once
#include "IBattleshipGameAlgo.h"
#include "BattleshipUtils.h"
#include "GameDisplayUtils.h"

#include <map>
#include <set>
#include <array>


using Player = IBattleshipGameAlgo;

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
	Game() : m_players({ nullptr, nullptr }), m_playersPoints({ 0, 0 }), m_playersShips({ 0, 0 }), m_playerIsDone({ false, false }),
			m_nextPlayer(0), m_quiet(false), m_delay(4000) {}
	/**
	 * Destructor
	 * 
	 * deletes the players' pointers
	 * 
	 */
	~Game();

	Game(const Game& g) = delete;
	Game& operator=(const Game& g) = delete;

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
	 * loads the dll files and initializes the players' board and the players. 
	 * 
	 * @param path - the absulote directory path containing the dll files
	 * @param dllSet - an ordered set containing the names of the dll files
	 * @return 
	 * false in case the dll loading or the players' initialization failed
	 * otherwise, true
	 */
	bool loadAndInitPlayersFromDLL(const std::string& path, std::set<std::string> dllSet);
	
	/**
	 * initializes game animation if "-quiet" argument wasn't passed:
	 * player A's ships are marked as capital letters colored in blue
	 * player B's ships are marked as lowercase letters colored in green
	 * empty squares are marked as "-" 
	 */
	void initializeGame() const;

	/**
	 * checks in an array of strings if one of the two following parameters appear:
	 * -quiet, -delay
	 * 
	 * @param begin - a pointer to the beginning of the array
	 * @param end - a pointer to the end of the array
	 */
	void checkParameters(char** begin, char** end);

private:
	
	/*
	 * a class that holds all the constant values used by Game
	 */
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
	std::array<size_t, 2>	m_playersPoints;
	std::array<size_t, 2>	m_playersShips;
	std::array<bool, 2>		m_playerIsDone;
	int						m_nextPlayer;
	bool					m_quiet;
	uint32_t				m_delay;
	char					m_gameBoard[GameConstants::BOARD_SIZE][GameConstants::BOARD_SIZE];

	/**
	* parses the board from the board file 
	* 
	* @param boardFile - a file stream to our board file
	*/
	void readBoardFromFile(std::ifstream& boardFile);

	/*
	* check the board meets the following requirements:  
	* each ship has the right size and there are no adjacent ships.
	* If one of these requirements isn't met, we mark in shouldPrint array that this error occurred
	* 
	*@param shouldPrint - an array containing zeros or ones so that each cell represents a different error on board
	*@param shipsErrorMsgMap - a map between a char representing a ship of a certain player and the index of its error in shouldPrint array
	*/
	void checkIfBoardIsValid(int shouldPrint[], std::map<char, int>& shipsErrorMsgMap);

	/**
	* checks each player has exactly 5 ships on board, otherwise this error is marked in shouldPrint array.
	* 
	*@param shouldPrint - an array containing zeros or ones so that each cell represents a different error on board 
	*/
	void checkShipsQuantity(int shouldPrint[]) const;

	/**
	* checks if the next turn can be passed to the other player
	* 
	* @param player - the current player
	* @return 
	* true in case the turn can be passed
	* otherwise, false
	*/
	bool canPassTurn(int player) const { return (player ? !(m_playerIsDone[0]) : !(m_playerIsDone[1])); }

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
	* Determines if the attack result was a sink or a hit.
	* 
	* @param rowCoord - the row coordinate of the current attack
	* @param colCoord - the column coordinate of the current attack
	* @return
	* Hit in case another square in the (rowCoord, colCoord) surrounding equals 'square'
	* otherwise Sink
	*/
	AttackResult determineAttackResult(char square, int rowCoord, int colCoord) const;

	/**
	* creates and sets a board for each player, which contains only the player's ships .
	* 
	* @param player - player's index
	*/
	void createBoardsForPlayers(int player);

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
	* @param row - the currShip row coordinate
	* @param col - the currShip column coordinate
	* @param shipLen - the current length of currShip on board
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
	* @param row - the currShip row coordinate
	* @param col - the currShip col coordinate
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
	* @param xCoord -  row coordinate of the sunk ship 
	* @param yCoord -  column coordinate of the sunk ship 
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

	/**
	* checks if a string appears in an array of strings (c strings)
	* 
	* @param begin - the beginning of the strings array
	* @param end - the end of the strings array
	* @param option - the string to be found
	* 
	* @return 
	* nullptr in case the string wasn't found in the array
	* otherwise, a pointer to the string in the array 
	*/
	static char** checkIfExists(char** begin, char** end, const std::string& option);

	
};