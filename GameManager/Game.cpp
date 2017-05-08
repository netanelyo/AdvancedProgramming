#include "Game.h"

#include <fstream>
#include <algorithm>

#define CHECK_CURRENT_SQUARE(i, j)	curr = m_gameBoard[i][j]; \
									if (curr == '0')	break; \
									if (curr == square) return AttackResult::Hit; \
									tmp--

const std::string Game::GameConstants::WRONG_SIZE_B_PLAYER_A	= "Wrong size or shape for ship B for player A";
const std::string Game::GameConstants::WRONG_SIZE_P_PLAYER_A	= "Wrong size or shape for ship P for player A";
const std::string Game::GameConstants::WRONG_SIZE_M_PLAYER_A	= "Wrong size or shape for ship M for player A";
const std::string Game::GameConstants::WRONG_SIZE_D_PLAYER_A	= "Wrong size or shape for ship D for player A";
const std::string Game::GameConstants::WRONG_SIZE_b_PLAYER_B	= "Wrong size or shape for ship b for player B";
const std::string Game::GameConstants::WRONG_SIZE_p_PLAYER_B	= "Wrong size or shape for ship p for player B";
const std::string Game::GameConstants::WRONG_SIZE_m_PLAYER_B	= "Wrong size or shape for ship m for player B";
const std::string Game::GameConstants::WRONG_SIZE_d_PLAYER_B	= "Wrong size or shape for ship d for player B";
const std::string Game::GameConstants::TOO_FEW_PLAYER_A			= "Too few ships for player A";
const std::string Game::GameConstants::TOO_MANY_PLAYER_A		= "Too many ships for player A";
const std::string Game::GameConstants::TOO_FEW_PLAYER_B			= "Too few ships for player B";
const std::string Game::GameConstants::TOO_MANY_PLAYER_B		= "Too many ships for player B";
const std::string Game::GameConstants::ADJACENT_SHIPS			= "Adjacent Ships on Board";


void Game::readBoardFromFile(std::ifstream & boardFile)
{
	std::string line;
	size_t		len;
	for (auto i = 0; i < GameConstants::BOARD_SIZE; ++i)
	{
		if (!std::getline(boardFile, line))
		{
			len = 0;
		}
		else
		{
			len = line.length();
		}

		for (auto j = 0; j < GameConstants::BOARD_SIZE; ++j)
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
					m_gameBoard[i][j] = line[j];
					break;
				default:
					m_gameBoard[i][j] = '0';
					break;
				}
			}

			else
				m_gameBoard[i][j] = '0';
		}
	}
}

void Game::checkIfBoardIsValid(int shouldPrintErrMsg[], std::map<char, int>& shipsErrorMsgMap)
{
	int dummyBoard[GameConstants::BOARD_SIZE][GameConstants::BOARD_SIZE] = { { 0 } };
	int shipLength = 1;
	int invalidShape = 0;
	int adjShips = 0;
	int belongsToA;
	char currentShip;

	for (auto i = 0; i < GameConstants::BOARD_SIZE; ++i)
	{
		for (auto j = 0; j < GameConstants::BOARD_SIZE; ++j)
		{
			if (dummyBoard[i][j] == 0)
			{
				dummyBoard[i][j] = 1;
				if (m_gameBoard[i][j] == '0')
					continue;

				/* gameBoard[i][j] - candidate to be a valid ship */
				currentShip = m_gameBoard[i][j];
				belongsToA = isupper(currentShip);

				dfsShip(currentShip, dummyBoard, i, j, shipLength, Direction::NON, invalidShape, adjShips);

				if (invalidShape || shipLength != getShipLen(currentShip))
				{
					shouldPrintErrMsg[shipsErrorMsgMap[currentShip]] = 1;
				}
				else
				{
					m_playersShips[(belongsToA ^ 0x1)]++;
				}

				if (adjShips)
				{
					shouldPrintErrMsg[GameConstants::ADJACENT_SHIPS_INDEX] = 1;
				}
			}

			shipLength = 1;
			invalidShape = 0;
			adjShips = 0;
		}
	}
}

void Game::checkShipsQuantity(int shouldPrintErrMsg[]) const
{
	if (m_playersShips[0] > 5)
	{
		shouldPrintErrMsg[GameConstants::TOO_MANY_PLAYER_A_INDEX] = 1;
	}
	if (m_playersShips[0] < 5)
	{
		shouldPrintErrMsg[GameConstants::TOO_FEW_PLAYER_A_INDEX] = 1;
	}
	if (m_playersShips[1] > 5)
	{
		shouldPrintErrMsg[GameConstants::TOO_MANY_PLAYER_B_INDEX] = 1;
	}
	if (m_playersShips[1] < 5)
	{
		shouldPrintErrMsg[GameConstants::TOO_FEW_PLAYER_B_INDEX] = 1;
	}
}

bool Game::checkAndCreateBoard(std::ifstream & boardFile)
{
	std::map<char, int> shipsErrorMsgMap = { {'B', 0}, {'P', 1}, {'M', 2}, {'D', 3},
							{'b', 4}, {'p', 5}, {'m', 6}, {'d', 7} };
	int shouldPrintErrMsg[13] = { 0 };
	std::string errMsg[13] = {GameConstants::WRONG_SIZE_B_PLAYER_A , GameConstants::WRONG_SIZE_P_PLAYER_A,
		GameConstants::WRONG_SIZE_M_PLAYER_A, GameConstants::WRONG_SIZE_D_PLAYER_A,
		GameConstants::WRONG_SIZE_b_PLAYER_B, GameConstants::WRONG_SIZE_p_PLAYER_B,
		GameConstants::WRONG_SIZE_m_PLAYER_B, GameConstants::WRONG_SIZE_d_PLAYER_B,
		GameConstants::TOO_FEW_PLAYER_A, GameConstants::TOO_MANY_PLAYER_A, GameConstants::TOO_FEW_PLAYER_B,
		GameConstants::TOO_MANY_PLAYER_B, GameConstants::ADJACENT_SHIPS };

	readBoardFromFile(boardFile);

	checkIfBoardIsValid(shouldPrintErrMsg, shipsErrorMsgMap);

	checkShipsQuantity(shouldPrintErrMsg);

	if (printErrors(shouldPrintErrMsg, errMsg))
		return true;

	return false;
}

void Game::dfsShip(char currShip, int dummy[][GameConstants::BOARD_SIZE], int row, int col,
					int& shipLen, Direction direction, int& invalidShape, int& adjShips) const
{
	auto stuckDown	= false;
	auto stuckRight = false;
	if (row < GameConstants::BOARD_SIZE - 1)
	{
		if (m_gameBoard[row + 1][col] == currShip)
		{
			if (direction == Direction::HORIZONTAL)
				invalidShape = 1;
			shipLen++;
			dummy[row + 1][col] = 1;
			dfsShip(currShip, dummy, row + 1, col, shipLen, Direction::VERTICAL, invalidShape, adjShips);
		}

		else if (m_gameBoard[row + 1][col] != '0') {
			adjShips = 1;
		}

		else
			stuckDown = true;
	}
	else
	{
		stuckDown = true;
	}

	if (col < GameConstants::BOARD_SIZE - 1)
	{
		if (m_gameBoard[row][col + 1] == currShip)
		{
			if (direction == Direction::VERTICAL)
				invalidShape = 1;
			shipLen++;
			dummy[row][col + 1] = 1;
			dfsShip(currShip, dummy, row, col + 1, shipLen, Direction::HORIZONTAL, invalidShape, adjShips);
		}

		else if (m_gameBoard[row][col + 1] != '0')
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

void Game::markAllOfSameShip(char currShip, int dummy[][GameConstants::BOARD_SIZE], int row, int col,
				int& invalidShape, int& adjShips) const
{
	if (row < GameConstants::BOARD_SIZE - 1 && dummy[row + 1][col] == 0)
	{
		if (m_gameBoard[row + 1][col] == currShip)
		{
			dummy[row + 1][col] = 1;
			markAllOfSameShip(currShip, dummy, row + 1, col, invalidShape, adjShips);
		}

		else if (m_gameBoard[row + 1][col] != '0')
			adjShips = 1;

	}

	if (col < GameConstants::BOARD_SIZE - 1 && dummy[row][col + 1] == 0)
	{
		if (m_gameBoard[row][col + 1] == currShip)
		{
			dummy[row][col + 1] = 1;
			markAllOfSameShip(currShip, dummy, row, col + 1, invalidShape, adjShips);
		}

		else if (m_gameBoard[row][col + 1] != '0')
			adjShips = 1;

	}
	
	if (row > 0 && dummy[row - 1][col] == 0)
	{
		if (m_gameBoard[row - 1][col] == currShip)
		{
			invalidShape = 1;
			dummy[row - 1][col] = 1;
			markAllOfSameShip(currShip, dummy, row - 1, col, invalidShape, adjShips);
		}

		else if (m_gameBoard[row - 1][col] != '0')
			adjShips = 1;
	}

	if (col > 0 && dummy[row][col - 1] == 0)
	{
		if (m_gameBoard[row][col - 1] == currShip)
		{
			invalidShape = 1;
			dummy[row][col - 1] = 1;
			markAllOfSameShip(currShip, dummy, row, col - 1, invalidShape, adjShips);
		}

		else if (m_gameBoard[row][col - 1] != '0')
			adjShips = 1;
	}
}

bool Game::printErrors(int shouldPrint[], std::string errors[])
{
	auto ret = false, first = false;
	for (auto i = 0; i < 13; ++i)
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

void Game::removeSankShip(int row, int col)
{
	m_gameBoard[row][col] = '@';
	if (!m_quiet) GameDisplayUtils::printSquare(col, row, '@', Color::RED);

	if (row > 0 && m_gameBoard[row - 1][col] == 'X')
	{
		removeSankShip(row - 1, col);
	}

	if (col > 0 && m_gameBoard[row][col - 1] == 'X')
	{
		removeSankShip(row, col - 1);
	}

	if (row < GameConstants::BOARD_SIZE - 1 && m_gameBoard[row + 1][col] == 'X')
	{
		removeSankShip(row + 1, col);
	}

	if (col < GameConstants::BOARD_SIZE - 1 && m_gameBoard[row][col + 1] == 'X')
	{
		removeSankShip(row, col + 1);
	}
}

GameState Game::playMove()
{
	std::pair<int, int> attackPair; 
	AttackResult result; 
	int rowCoord, colCoord, winner = -1, currentPlayer = m_nextPlayer; 
	char square;   
	bool canPass;
	auto playerA = m_players[0];
	auto playerB = m_players[1];

	/*gets player's next move*/
	attackPair = m_players[currentPlayer]->attack();
	if (attackPair == std::pair<int, int>(-1, -1))
	{
		m_playerIsDone[currentPlayer] = true;
	}

	/*checks if both players got to EOF and if so -> game is over*/
	if (endOfAttacks())
	{
		return GameState::GAME_OVER; 
	}

	/*if only the current player got to EOF, it means that we got no attack and we only pass the turn to the opponent*/
	if (m_playerIsDone[currentPlayer])
	{
		m_nextPlayer ^= 1; 
		return GameState::CONTINUE_PLAYING; 
	}

	/*gets square coordinates*/
	rowCoord = attackPair.first - 1;
	colCoord = attackPair.second - 1;
	//TODO check if game expects legal move or need to check
	/*gets square*/
	square = m_gameBoard[rowCoord][colCoord];

	/*determines if the next turn can be passed to the opponent */
	canPass = canPassTurn(currentPlayer);

	/*determines the attack result and sets the next Turn and the player's points accordingly*/
	if (square == '0' || square == '@')
	{
		result = AttackResult::Miss;
		m_nextPlayer ^= (canPass ? 1 : 0); /* If canPassTurn -> nextPlayer changes */

		if (!m_quiet && square != '@') GameDisplayUtils::printSquare(colCoord, rowCoord, '*', Color::WHITE);
	}
	else
	{
		m_gameBoard[rowCoord][colCoord] = 'X';
		result = determineAttackResult(square, rowCoord, colCoord);

		handlePointsAndNextTurn(result, square, currentPlayer, isupper(square), canPass);
		if (!m_quiet) GameDisplayUtils::printSquare(colCoord, rowCoord, 'X', Color::RED);

		if (result == AttackResult::Sink)
		{
			removeSankShip(rowCoord, colCoord);
		}
	}

	if (!m_quiet) Sleep(m_delay);

	/*notifies players on the current result*/
	playerA->notifyOnAttackResult(currentPlayer, rowCoord + 1, colCoord + 1, result);
	playerB->notifyOnAttackResult(currentPlayer, rowCoord + 1, colCoord + 1, result);
	
	/*checks if there is a winner*/
	if (m_playersShips[0] == 0)
	{
		winner = 1; 
	}
	else if (m_playersShips[1] == 0)
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

void Game::createBoardsForPlayers(int player)
{
	auto board = BattleshipUtils::allocateBoard(GameConstants::BOARD_SIZE, GameConstants::BOARD_SIZE);
	char c;
	
	for (auto i = 0; i < GameConstants::BOARD_SIZE; i++)
	{
		for (auto j = 0; j < GameConstants::BOARD_SIZE; j++)
		{
			c = m_gameBoard[i][j];
			if (c != '0')
			{
				board[i][j] = (player ^ isupper(c)) ? c : '0';
			}
			else
			{
				board[i][j] = '0';
 			}		
		}
	}

	m_players[player]->setBoard(player, const_cast<const char**>(board), GameConstants::BOARD_SIZE, GameConstants::BOARD_SIZE);

	BattleshipUtils::deallocateBoard(board, GameConstants::BOARD_SIZE);
}

void Game::printEndOfGame(int winner) const
{
	if (!m_quiet)
	{
		GameDisplayUtils::gotoxy(0, GameConstants::BOARD_SIZE);
		GameDisplayUtils::returnToDefault();
	}
	
	if (winner == 0)
	{
		std::cout << "Player A won" << std::endl; 
	}

	else if (winner == 1)
	{
		std::cout << "Player B won" << std::endl;
	}

	std::cout << "Points:" << std::endl;
	std::cout << "Player A: " << m_playersPoints[0] << std::endl;
	std::cout << "Player B: " << m_playersPoints[1];
}

bool Game::endOfAttacks() const
{
	if (m_playerIsDone[0] && m_playerIsDone[1])
	{
		printEndOfGame(-1);
		return true;
	}

	return false;
}

AttackResult Game::determineAttackResult(char square, int rowCoord, int colCoord) const
{
	auto len = getShipLen(square);
	auto tmp = len - 1;
	char curr;
	
	if (square == 'X')
	{
		return AttackResult::Hit;
	}

	while (rowCoord > len - tmp - 1 && tmp > 0)
	{
		CHECK_CURRENT_SQUARE(rowCoord - (len - tmp), colCoord);
	}

	tmp = len - 1;

	while (colCoord > len - tmp - 1 && tmp > 0)
	{
		CHECK_CURRENT_SQUARE(rowCoord, colCoord - (len - tmp));
	}

	tmp = len - 1;

	while (rowCoord < GameConstants::BOARD_SIZE - (len - tmp) && tmp > 0)
	{
		CHECK_CURRENT_SQUARE(rowCoord + (len - tmp), colCoord);
	}

	tmp = len - 1;

	while (colCoord < GameConstants::BOARD_SIZE - (len - tmp) && tmp > 0)
	{
		CHECK_CURRENT_SQUARE(rowCoord, colCoord + (len - tmp));
	}

	return AttackResult::Sink;
}

void Game::handlePointsAndNextTurn(AttackResult result, char ship, int currentPlayer, bool isAShip, bool canPassTurn)
{
	auto shouldPass = ((!currentPlayer && isAShip) || (currentPlayer && !isAShip)
													|| ship == 'X') && canPassTurn;

	/*handles points in case of a sink*/
	if (result == AttackResult::Sink)
	{
		uint16_t points;
		auto playerIndex = isAShip ? 1 : 0;

		switch (ship)
		{
		case 'B':
		case 'b':
			points = GameConstants::RUBBER_BOAT_POINTS;
			break;
		case 'P':
		case 'p':
			points = GameConstants::MISSILE_BOAT_POINTS;
			break; 
		case 'M':
		case 'm': 
			points = GameConstants::SUBMARINE_POINTS;
			break; 
		case 'D':
		case 'd':
			points = GameConstants::DESTROYER_POINTS;
			break; 
		default:
			points = 0;
			break;
		}

		m_playersPoints[playerIndex] += points;
		m_playersShips[(playerIndex ^ 0x1)]--;
	}

	/*passes the next turn to the opponent in case the current player hit his own ship and the opponent didn't get to EOF*/
	if (shouldPass)
	{
		m_nextPlayer ^= 1; 
	}
}


size_t Game::getShipLen(char ship)
{
	switch (ship)
	{
	case 'B':
	case 'b':
		return GameConstants::RUBBER_BOAT_LEN;
	case 'P':
	case 'p':
		return GameConstants::MISSILE_BOAT_LEN;
	case 'M':
	case 'm':
		return GameConstants::SUBMARINE_LEN;
	case 'D':
	case 'd':
		return GameConstants::DESTROYER_LEN;
	default:
		return 0;
	}
}

void Game::initializeGame() const
{
	if (m_quiet)
		return;

	system("cls");
	GameDisplayUtils::gotoxy(0, 0);
	GameDisplayUtils::consoleCursorVisibility(false);

	for (auto i = 0; i < GameConstants::BOARD_SIZE; i++)
	{
		for (auto j = 0; j < GameConstants::BOARD_SIZE; j++)
		{
			auto sq = m_gameBoard[i][j];
			if (sq == '0')
				GameDisplayUtils::setColor(Color::WHITE);

			else
			{
				if (isupper(sq))
					GameDisplayUtils::setColor(Color::BLUE);

				else
					GameDisplayUtils::setColor(Color::GREEN);
			}

			std::cout << sq;
		}
		std::cout << std::endl;
	}
}

bool Game::loadAndInitPlayersFromDLL(const std::string& path, std::set<std::string> dllSet)
{
	// define function of the type we expect
	typedef IBattleshipGameAlgo* (*GetPlayerType)();
	GetPlayerType getPlayer;

	auto dllIter = dllSet.begin();
	HINSTANCE hDll;
	std::string fullPath;

	for (auto i = 0; i < m_players.size(); i++)
	{
		fullPath = path + *dllIter;
		++dllIter;

		hDll = LoadLibraryA(fullPath.c_str());
		if (!hDll)
		{
			std::cout << "Cannot load dll: " << fullPath;
			return false;
		}

		getPlayer = (GetPlayerType)GetProcAddress(hDll, "GetAlgorithm");
		if (!getPlayer)
		{
			std::cout << "Cannot load dll: " << fullPath;
			return false;
		}

		m_players[i] = getPlayer();

		createBoardsForPlayers(i);

		if (!m_players[i]->init(path))
		{
			std::cout << "Algorithm initialization failed for dll: " << fullPath;
			return false;
		}
	}

	return true;
}

void Game::checkParameters(char ** begin, char ** end)
{
	auto iter = checkIfExists(begin, end, "-quiet");
	if (iter)
	{
		m_quiet = true;
		return;
	}

	iter = checkIfExists(begin, end, "-delay");
	if (iter)
	{
		++iter;
		m_delay = std::stoi(*iter);
	}
}

char** Game::checkIfExists(char ** begin, char ** end, const std::string & option)
{
	auto ret = begin;
	while (ret != end)
	{
		if (!option.compare(*ret))
			return ret;

		++ret;
	}
	return nullptr;
}





