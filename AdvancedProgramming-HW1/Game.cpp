#include <cctype>

#include "Game.h"


GameState Game::playMove()
{
	std::pair<int, int> attackPair; 

	/*gets player's next move*/
	switch (nextPlayer)
	{
	case 0:
		attackPair = A.attack(); 
		if (A.getIsDone())
		{
			if (endOfAttacks())
			{
				return GameState::GAME_OVER; 
			}

			nextPlayer = 1;
			return GameState::CONTINUE_PLAYING;
		}
		break; 
	case 1:
		attackPair = B.attack(); 
		if (B.getIsdone())
		{
			if (endOfAttacks)
			{
				return GameState::GAME_OVER;
			}
			nextPlayer = 0; 
			return GameState::CONTINUE_PLAYING; 
		}
		break; 
	default: 
		break; 
	}



	


	
}

void Game::createBoardsForPlayers()
{
	char boardForA[BOARD_SIZE][BOARD_SIZE];
	char boardForB[BOARD_SIZE][BOARD_SIZE];
	char c; 

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			c = gameBoard[i][j]; 
			if (c != '0')
			{
				boardForA[i][j] = isupper(c) ? c : '0';
				boardForB[i][j] = isupper(c) ? '0' : c;
			}
			else
			{
				boardForA[i][j] = '0';
				boardForB[i][j] = '0'; 
 			}		
		}
	}

	A.setBoard((const char**) boardForA, BOARD_SIZE, BOARD_SIZE); 
	B.setBoard((const char**) boardForB, BOARD_SIZE, BOARD_SIZE); 

}

bool Game::endOfAttacks()
{
	if (A.getIsDone() && B.getIsDone())
	{
		printEndOfGame(-1); 
		return true; 
	}

	return false; 
}





