#pragma once

#include "IBattleshipGameAlgo.h"
#include "BattleshipGameUtils.h"
#include "GameBoard.h"

using Player = IBattleshipGameAlgo;


class Game
{
public:
	Game(GameBoard board, std::shared_ptr<Player> playerA, std::shared_ptr<Player> playerB); 




private:
	enum class GameState
	{
		GAME_OVER,
		CONTINUE_PLAYING
	};
	
	GameBoard m_board;
	std::array<std::shared_ptr<Player>, 2>	m_players;
	std::array<int, 2>	    m_playersPoints;
	std::array<int, 2>	    m_playersShips;
	std::array<bool, 2>		m_playerIsDone;
	int						m_nextPlayer;
};

