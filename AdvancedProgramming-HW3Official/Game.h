#pragma once

#include "IBattleshipGameAlgo.h"
#include "GameBoard.h"
#include "BattleshipGameUtils.h"
#include "Printer.h"
#include <mutex>

using Player = IBattleshipGameAlgo;
using Constant = BattleshipGameUtils::Constants;


class Game
{
public:
	Game(GameBoard board, std::shared_ptr<Player> playerA, std::shared_ptr<Player> playerB, 
			int playerAId, int playerBId);

	Game(const Game& game) = delete;
	Game(Game&& game) = delete;
	Game& operator=(const Game& game) = delete; 

	void runGame(Printer& printer);

	static void Game::start(GameBoard board, std::shared_ptr<Player> A, std::shared_ptr<Player> B, int idA, int idB,
		Printer& printer, int& active, std::condition_variable& cv, std::mutex& m);
	
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
	int						                m_nextPlayer = 0;
	int										m_winner = -1;
	
	bool canPassTurn(int player) const { return (player ? !(m_playerIsDone[0]) : !(m_playerIsDone[1])); }

	bool endOfAttacks() const;

	AttackResult determineAttackResult(char square, Coordinate coor) const;

	void setDataStructs(std::shared_ptr<Player> playerA, std::shared_ptr<Player> playerB, int playerAId, int playerBId);

	void initPlayers();

	void handlePointsAndNextTurn(AttackResult result, char ship, int currentPlayer,
		bool isAship, bool canPassTurn);

	void removeSankShip(Coordinate coor) const;

	GameState playMove();

	static int getShipLen(char ship);

};

