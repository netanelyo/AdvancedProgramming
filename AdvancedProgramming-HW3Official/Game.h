#pragma once

#include "IBattleshipGameAlgo.h"
#include "GameBoard.h"
#include "BattleshipGameUtils.h"

using Player = IBattleshipGameAlgo;
using Constant = BattleshipGameUtils::Constants;


class Game
{
public:
	/*
	 * constructor
	 */
	Game(GameBoard board, std::shared_ptr<Player> playerA, std::shared_ptr<Player> playerB, 
			int playerAId, int playerBId); 

	Game(const Game& game) = delete;
	Game(Game&& game) = delete;
	Game& operator=(const Game& game) = delete; 

	void resetGame(const GameBoard& board, std::shared_ptr<Player> playerA, std::shared_ptr<Player> playerB);

	/**
	 * initializes players and runs the game 
	 */
	void runGame(); 
	



private:
	enum class GameState
	{
		GAME_OVER,
		CONTINUE_PLAYING
	};

	GameBoard m_board;
	std::array<std::shared_ptr<Player>, 2>	m_players;
	std::array<int, 2>						m_playersIDs; 
	std::array<int, 2>	                    m_playersPoints;
	std::array<int, 2>	                    m_playersShips;
	std::array<bool, 2>		                m_playerIsDone;
	int						                m_nextPlayer;
	
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
	* checks if both players don't have any more moves
	* @return
	* true if both players don't have any more moves
	* otherwise false
	*/
	bool endOfAttacks() const;

	/**
	* Determines if the attack result was a sink or a hit.
	*
	* @param coor - the coordinate of the current attack
	* @return
	* Hit in case another square in the coordinate's surrounding equals 'square'
	* otherwise Sink
	*/
	AttackResult determineAttackResult(char square, Coordinate coor) const;

	/**
	 * resets the relevant data structures for the new upcoming game
	 */
	void setDataStructs(std::shared_ptr<Player> playerA, std::shared_ptr<Player> playerB, int playerAId, int playerBId);

	/**
	* initializes players for the upcoming game by giving each player its number 
	* and its board data.
	*/
	void initPlayers();

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
	* removes a sunk ship from the game board
	*
	* @param coor -  coordinate of the sunk ship
	*/
	void removeSankShip(Coordinate coor) const;

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
	* returns the length of ship
	* @param ship - the symbol of the ship
	* @return
	* 0 in case the symbol doesn't belong to any ship
	* otherwise the ship's length
	*/
	static int getShipLen(char ship);

};

