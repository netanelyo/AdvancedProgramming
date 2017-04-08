#include <cctype>
#include <iostream>

#include "Game.h"

#define canPass(x) x ? !(A.getIsDone()) : !(B.getIsDone())


GameState Game::playMove()
{
	std::pair<int, int> attackPair; 
	AttackResult result; 
	int xCoord, yCoord, winner = -1, currentPlayer = nextPlayer; 
	char square;   
	bool canPassTurn; 

	/*gets player's next move*/
	switch (currentPlayer)
	{
	case 0:
		attackPair = A.attack(); 
		break; 
	case 1:
		attackPair = B.attack();
		break; 
	default: 
		break; 
	}

	/*checks if both players got to EOF and if so -> game is over*/
	if (endOfAttacks())
	{
		return GameState::GAME_OVER; 
	}

	/*if only the current player got to EOF, it means that we got no attack and we only pass the turn to the opponent*/
	if ((A.getIsDone() && !currentPlayer) || (B.getIsDone() && currentPlayer))
	{
		nextPlayer ^= 1; 
		return GameState::CONTINUE_PLAYING; 
	}

	/*gets square coordinates*/
	xCoord = attackPair.first - 1;
	yCoord = attackPair.second - 1;
	/*gets square*/
	square = gameBoard[xCoord][yCoord];

	/*determines if the next turn can be passed to the opponent */
	canPassTurn = canPass(currentPlayer);

	/*determines the attack result and sets the next Turn and the player's points accordingly*/
	if (square == '0')
	{
		result = AttackResult::Miss;
		if (canPassTurn)
		{
			nextPlayer ^= 1;
		}
	}
	else
	{
		result = determineAttackResult(square, xCoord, yCoord);
		if (square == 'X' && canPassTurn)
		{
			nextPlayer ^= 1;
		}
		else
		{
			handlePointsAndNextTurn(result, square, currentPlayer, isupper(square));
		}
	}

	/*notifies players on the current result*/
	A.notifyOnAttackResult(currentPlayer, xCoord, yCoord, result);
	B.notifyOnAttackResult(currentPlayer, xCoord, yCoord, result); 
	
	/*checks if there is a winner*/
	if (A.getShipCounter() == 0)
	{
		winner = 0; 
	}
	else if (B.getShipCounter() == 0)
	{
		winner = 1; 
	}

	/*if there's a winner, we end the game*/
	if (winner > -1)
	{
		printEndOfGame(winner); 
		return GameState::GAME_OVER; 
	}

	return GameState::CONTINUE_PLAYING; 
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

void Game::printEndOfGame(int winner)
{
	if (winner == 0)
	{
		std::cout << "Player A won" << std::endl; 
	}

	else if (winner == 1)
	{
		std::cout << "Player B won" << std::endl;
	}

	std::cout << "Points:" << std::endl;
	std::cout << "Player A: " << A.getPoints() << std::endl;
	std::cout << "Player B: " << B.getPoints() << std::endl; 
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

AttackResult Game::determineAttackResult(char square, int xCoord, int yCoord)
{
	if ((xCoord != 0))
	{
		if ((gameBoard[xCoord - 1][yCoord] != '0') && (gameBoard[xCoord - 1][yCoord] != 'X'))
		{
			return AttackResult::Hit; 
		}
	}

	if (yCoord != 0)
	{
		if ((gameBoard[xCoord][yCoord - 1] != '0') && (gameBoard[xCoord][yCoord - 1] != 'X'))
		{
			return AttackResult::Hit;
		}
	}

	if (xCoord != 9)
	{
		if ((gameBoard[xCoord + 1][yCoord] != '0') && (gameBoard[xCoord + 1][yCoord] != 'X'))
		{
			return AttackResult::Hit;
		}
	}

	if (yCoord != 9)
	{
		if ((gameBoard[xCoord][yCoord + 1] != '0') && (gameBoard[xCoord][yCoord + 1] != 'X'))
		{
			return AttackResult::Hit;
		}
	}

	if (square == 'X')
	{
		return AttackResult::Miss; 
	}

	return AttackResult::Sink; 
}

void Game::handlePointsAndNextTurn(AttackResult result, char ship, int currentPlayer, bool isAShip)
{
	bool canPassTurn = canPass(currentPlayer);

	/*handles points in case of a sink*/
	if (result == AttackResult::Sink)
	{
		uint16_t points;

		switch (ship)
		{
		case 'B':
		case 'b':
			points = RUBBER_BOAT_POINTS;
			break;
		case 'P':
		case 'p':
			points = MISSILE_BOAT_POINTS; 
			break; 
		case 'M':
		case 'm': 
			points = SUBMARINE_POINTS; 
			break; 
		case 'D':
		case 'd':
			points = DESTROYER_POINTS; 
			break; 
		default:
			break;
		}

		/*A's ship sank*/
		if (isAShip)
		{
			/*Player B gets the points*/
			B.setPoints(B.getPoints() + points); 
	
		}
		/*B's ship sank*/
		else
		{
			/*Player A gets the points*/
			A.setPoints(A.getPoints() + points); 
		}		
	}

	/*passes the next turn to the opponent in case the current player hit his own ship and the opponent didn't get to EOF*/
	if ((isAShip && !currentPlayer && canPassTurn) || (!isAShip && currentPlayer && canPassTurn))
	{
		nextPlayer ^= 1; 
	}
}







