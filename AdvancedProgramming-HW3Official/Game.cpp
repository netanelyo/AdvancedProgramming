#include "Game.h"

Game::Game(GameBoard board, std::shared_ptr<Player> playerA, std::shared_ptr<Player> playerB) :
	m_board(board)
{
	m_players[0] = playerA;
	m_players[1] = playerB; 

	m_playersShips[0] = m_board.getShipCountForPlayer(0); 
	m_playersShips[1] = m_board.getShipCountForPlayer(1); 

	m_nextPlayer = 0;
}