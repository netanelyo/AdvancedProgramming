#include "Game.h"

#define WRONG_SIZE_B_PLAYER_A "Wrong size or shape for ship B for player A"
#define WRONG_SIZE_P_PLAYER_A "Wrong size or shape for ship P for player A"
#define WRONG_SIZE_M_PLAYER_A "Wrong size or shape for ship M for player A"
#define WRONG_SIZE_D_PLAYER_A "Wrong size or shape for ship D for player A"
#define WRONG_SIZE_b_PLAYER_B "Wrong size or shape for ship b for player B"
#define WRONG_SIZE_p_PLAYER_B "Wrong size or shape for ship p for player B"
#define WRONG_SIZE_m_PLAYER_B "Wrong size or shape for ship m for player B"
#define WRONG_SIZE_d_PLAYER_B "Wrong size or shape for ship d for player B"
#define TOO_FEW_PLAYER_A "Too few ships for player A"
#define TOO_FEW_PLAYER_A_INDEX 8
#define TOO_MANY_PLAYER_A "Too many ships for player A"
#define TOO_MANY_PLAYER_A_INDEX 9
#define TOO_FEW_PLAYER_B "Too few ships for player B"
#define TOO_FEW_PLAYER_B_INDEX 10
#define TOO_MANY_PLAYER_B "Too many ships for player B"
#define	TOO_MANY_PLAYER_B_INDEX 11
#define ADJACENT_SHIPS "Adjacent Ships on Board"
#define ADJACENT_SHIPS_INDEX 12

#define canPass(x) x ? !(A.getIsDone()) : !(B.getIsDone())

bool Game::checkAndCreateBoard(std::ifstream & boardFile)
{
	std::map<char, int> shipsErrorMsgMap = { {'B', 0}, {'P', 1}, {'M', 2}, {'D', 3},
							{'b', 4}, {'p', 5}, {'m', 6}, {'d', 7} };
	int shouldPrintErrMsg[13] = { 0 };
	std::string errMsg[13] = { WRONG_SIZE_B_PLAYER_A , WRONG_SIZE_P_PLAYER_A, WRONG_SIZE_M_PLAYER_A,
			WRONG_SIZE_D_PLAYER_A, WRONG_SIZE_b_PLAYER_B, WRONG_SIZE_p_PLAYER_B, WRONG_SIZE_m_PLAYER_B,
			WRONG_SIZE_d_PLAYER_B, TOO_FEW_PLAYER_A, TOO_MANY_PLAYER_A, TOO_FEW_PLAYER_B,
			TOO_MANY_PLAYER_B, ADJACENT_SHIPS };

	std::string line; 
	size_t len;
	int dummyBoard[BOARD_SIZE][BOARD_SIZE] = { { 0 } };
	char currentShip;
	int shipLength		= 1;
	int invalidShape	= 0;
	int adjShips		= 0;
	bool belongsToA;

	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		if (!std::getline(boardFile, line))
		{
			len = 0; 
		}
		else 
		{
			len = line.length();
		}

		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			if (j < len)
			{
				switch (line[j])
				{
				case 'B':
				case 'b':
				case 'P':
				case 'p':
				case 'M':
				case 'm':
				case 'D':
				case 'd':
					gameBoard[i][j] = line[j];
					break;
				default:
					gameBoard[i][j] = '0';
					break;
				}
			}

			else
				gameBoard[i][j] = '0'; 
		}
	}

	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			if (dummyBoard[i][j] == 0)
			{
				dummyBoard[i][j] = 1;
				if (gameBoard[i][j] == '0')
					continue;

				/* gameBoard[i][j] - candidate to be a valid ship */
				currentShip = gameBoard[i][j];
				belongsToA = isupper(currentShip);

				dfsShip(currentShip, dummyBoard, i, j, shipLength, Direction::NON, invalidShape, adjShips);

				if (invalidShape || shipLength != getShipLen(currentShip))
				{
					shouldPrintErrMsg[shipsErrorMsgMap[currentShip]] = 1;
				}
				else
				{
					if (belongsToA)
						A.incrementShipCounter();
					else
						B.incrementShipCounter();
				}

				if (adjShips)
				{
					shouldPrintErrMsg[ADJACENT_SHIPS_INDEX] = 1;
				}
			}

			shipLength = 1;
			invalidShape = 0;
			adjShips = 0;
		}
	}

	if (A.getShipCounter() > 5)
	{
		shouldPrintErrMsg[TOO_MANY_PLAYER_A_INDEX] = 1;
	}
	if (A.getShipCounter() < 5)
	{
		shouldPrintErrMsg[TOO_FEW_PLAYER_A_INDEX] = 1;
	}
	if (B.getShipCounter() > 5)
	{
		shouldPrintErrMsg[TOO_MANY_PLAYER_B_INDEX] = 1;
	}
	if (B.getShipCounter() < 5)
	{
		shouldPrintErrMsg[TOO_FEW_PLAYER_B_INDEX] = 1;
	}

	if (printErrors(shouldPrintErrMsg, errMsg))
		return true;

	createBoardsForPlayers();
	return false;

}

void Game::dfsShip(char currShip, int dummy[][BOARD_SIZE], int row, int col,
					int& shipLen, Direction direction, int& invalidShape, int& adjShips) const
{
	bool stuckDown	= false;
	bool stuckRight = false;
	if (row < BOARD_SIZE - 1)
	{
		if (gameBoard[row + 1][col] == currShip)
		{
			if (direction == Direction::HORIZONTAL)
				invalidShape = 1;
			shipLen++;
			dummy[row + 1][col] = 1;
			dfsShip(currShip, dummy, row + 1, col, shipLen, Direction::VERTICAL, invalidShape, adjShips);
		}

		else if (gameBoard[row + 1][col] != '0') {
			adjShips = 1;
		}

		else
			stuckDown = true;
	}
	else
	{
		stuckDown = true;
	}

	if (col < BOARD_SIZE - 1)
	{
		if (gameBoard[row][col + 1] == currShip)
		{
			if (direction == Direction::VERTICAL)
				invalidShape = 1;
			shipLen++;
			dummy[row][col + 1] = 1;
			dfsShip(currShip, dummy, row, col + 1, shipLen, Direction::HORIZONTAL, invalidShape, adjShips);
		}

		else if (gameBoard[row][col + 1] != '0')
		{
			adjShips = 1;
		}

		else
			stuckRight = true;
	}
	else
	{
		stuckRight = true;
	}

	if (stuckDown && stuckRight)
		markAllOfSameShip(currShip, dummy, row, col, invalidShape, adjShips);
}

void Game::markAllOfSameShip(char currShip, int dummy[][BOARD_SIZE], int row, int col,
				int& invalidShape, int& adjShips) const
{
	if (row < BOARD_SIZE - 1 && dummy[row + 1][col] == 0)
	{
		if (gameBoard[row + 1][col] == currShip)
		{
			dummy[row + 1][col] = 1;
			markAllOfSameShip(currShip, dummy, row + 1, col, invalidShape, adjShips);
		}

		else if (gameBoard[row + 1][col] != '0')
			adjShips = 1;

	}

	if (col < BOARD_SIZE - 1 && dummy[row][col + 1] == 0)
	{
		if (gameBoard[row][col + 1] == currShip)
		{
			dummy[row][col + 1] = 1;
			markAllOfSameShip(currShip, dummy, row, col + 1, invalidShape, adjShips);
		}

		else if (gameBoard[row][col + 1] != '0')
			adjShips = 1;

	}
	
	if (row > 0 && dummy[row - 1][col] == 0)
	{
		if (gameBoard[row - 1][col] == currShip)
		{
			invalidShape = 1;
			dummy[row - 1][col] = 1;
			markAllOfSameShip(currShip, dummy, row - 1, col, invalidShape, adjShips);
		}

		else if (gameBoard[row - 1][col] != '0')
			adjShips = 1;
	}

	if (col > 0 && dummy[row][col - 1] == 0)
	{
		if (gameBoard[row][col - 1] == currShip)
		{
			invalidShape = 1;
			dummy[row][col - 1] = 1;
			markAllOfSameShip(currShip, dummy, row, col - 1, invalidShape, adjShips);
		}

		else if (gameBoard[row][col - 1] != '0')
			adjShips = 1;
	}
}

bool Game::printErrors(int shouldPrint[], std::string errors[])
{
	bool ret = false, first = false; 
	for (int i = 0; i < 13; ++i)
	{
		if (shouldPrint[i])
		{
			ret = true; 
			if (!first)
				std::cout << errors[i];
			else
				std::cout << std::endl << errors[i]; 
			first = true;
		}
	}
	return ret;
}

void Game::removeSankShip(int xCoord, int yCoord)
{
	gameBoard[xCoord][yCoord] = '0';
	if (xCoord > 0 && gameBoard[xCoord - 1][yCoord] == 'X')
	{
		removeSankShip(xCoord - 1, yCoord);
	}

	if (yCoord > 0 && gameBoard[xCoord][yCoord - 1] == 'X')
	{
		removeSankShip(xCoord, yCoord - 1);
	}

	if (xCoord < BOARD_SIZE - 1 && gameBoard[xCoord + 1][yCoord] == 'X')
	{
		removeSankShip(xCoord + 1, yCoord);
	}

	if (yCoord < BOARD_SIZE - 1 && gameBoard[xCoord][yCoord + 1] == 'X')
	{
		removeSankShip(xCoord, yCoord + 1);
	}
}

static int countSink = 0;

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
		nextPlayer ^= (canPassTurn ? 1 : 0); /* If canPassTurn -> nextPlayer changes */
	}
	else
	{
		gameBoard[xCoord][yCoord] = 'X';
		result = determineAttackResult(square, xCoord, yCoord);

		if (result == AttackResult::Miss)
		{
			nextPlayer ^= (canPassTurn ? 1 : 0);
		}
		else
		{
			handlePointsAndNextTurn(result, square, currentPlayer, isupper(square), canPassTurn);

			if (result == AttackResult::Sink)
			{
				removeSankShip(xCoord, yCoord);
			}
		}
	}

	/*notifies players on the current result*/
	A.notifyOnAttackResult(currentPlayer, xCoord, yCoord, result);
	B.notifyOnAttackResult(currentPlayer, xCoord, yCoord, result); 
	
	/*checks if there is a winner*/
	if (A.getShipCounter() == 0)
	{
		winner = 1; 
	}
	else if (B.getShipCounter() == 0)
	{
		winner = 0; 
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
	char** boardForA = new char*[BOARD_SIZE];
	char** boardForB = new char*[BOARD_SIZE];
	char c; 
	
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		boardForA[i] = new char[BOARD_SIZE];
		boardForB[i] = new char[BOARD_SIZE];

		for (int j = 0; j < BOARD_SIZE; j++)
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
	
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		delete[]boardForA[i];
		delete[]boardForB[i]; 
	}

	delete []boardForA; 
	delete []boardForB;

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
	std::cout << "Player B: " << B.getPoints(); 
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
	size_t len = getShipLen(square);
	size_t tmp = len - 1;
	char curr;
	
	if (square == 'X')
	{
		return AttackResult::Hit;
	}

	while (xCoord > len - tmp - 1 && tmp > 0)
	{
		curr = gameBoard[xCoord - (len - tmp)][yCoord];
		if (curr == '0')	break;
		if (curr == square) return AttackResult::Hit;
		tmp--;
	}

	tmp = len - 1;

	while (yCoord > len - tmp - 1 && tmp > 0)
	{
		curr = gameBoard[xCoord][yCoord - (len - tmp)];
		if (curr == '0')	break;
		if (curr == square) return AttackResult::Hit;
		tmp--;
	}

	tmp = len - 1;

	while (xCoord < BOARD_SIZE - (len - tmp) && tmp > 0)
	{
		curr = gameBoard[xCoord + (len - tmp)][yCoord];
		if (curr == '0')	break;
		if (curr == square) return AttackResult::Hit;
		tmp--;
	}

	tmp = len - 1;

	while (yCoord < BOARD_SIZE - (len - tmp) && tmp > 0)
	{
		curr = gameBoard[xCoord][yCoord + (len - tmp)];
		if (curr == '0')	break;
		if (curr == square) return AttackResult::Hit;
		tmp--;
	}

	return AttackResult::Sink; 
}

void Game::handlePointsAndNextTurn(AttackResult result, char ship, int currentPlayer, bool isAShip, bool canPassTurn)
{
	bool shouldPass = ((!currentPlayer && isAShip) || (currentPlayer && !isAShip)) && canPassTurn;

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
			points = 0;
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
	if (shouldPass)
	{
		nextPlayer ^= 1; 
	}
}


size_t Game::getShipLen(char ship)
{
	switch (ship)
	{
	case 'B':
	case 'b':
		return RUBBER_BOAT_LEN;
	case 'P':
	case 'p':
		return MISSILE_BOAT_LEN;
	case 'M':
	case 'm':
		return SUBMARINE_LEN;
	case 'D':
	case 'd':
		return DESTROYER_LEN;
	default:
		return 0;
	}
}
