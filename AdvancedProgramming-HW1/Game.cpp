#include <cctype>

#include "Game.h"


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

}



