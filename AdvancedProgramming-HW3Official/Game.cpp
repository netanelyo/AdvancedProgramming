#include "Game.h"
#include "BoardDataImpl.h"
#include "BattleshipGameUtils.h"

#define CHECK_CURRENT_SQUARE(i, j, k)	curr = m_board.getBoardSquare(Coordinate(i, j, k)); \
										if (curr == '0')	break; \
										if (curr == square) return AttackResult::Hit; \
										tmp--

Game::Game(GameBoard board, std::shared_ptr<Player> playerA, std::shared_ptr<Player> playerB) :
	m_board(board)
{
	setDataStructs(playerA, playerB);
}

void Game::resetGame(const GameBoard& board, std::shared_ptr<Player> playerA, std::shared_ptr<Player> playerB)
{
	m_board = board;
	setDataStructs(playerA, playerB); 
}

void Game::runGame()
{
	initPlayers();
	while (playMove() != GameState::GAME_OVER){}
}

void Game::setDataStructs(std::shared_ptr<Player> playerA, std::shared_ptr<Player> playerB)
{
	m_players[0] = playerA;
	m_players[1] = playerB;

	m_playersShips[0] = m_board.getShipCountForPlayer(0);
	m_playersShips[1] = m_board.getShipCountForPlayer(1);

	m_playersPoints[0] = 0;
	m_playersPoints[1] = 0;

	m_playerIsDone[0] = false;
	m_playerIsDone[1] = false;

	m_nextPlayer = 0;
}

void Game::initPlayers()
{
	m_players[0]->setPlayer(0); 
	m_players[1]->setPlayer(1); 

	BoardDataImpl boardDataForPlayers(m_board, 0);
	m_players[0]->setBoard(boardDataForPlayers);
	boardDataForPlayers.setPlayer(1); 
	m_players[1]->setBoard(boardDataForPlayers); 
}

void Game::removeSankShip(Coordinate coor) const
{
	m_board.setBoardSquare(coor, '@');

	coor.row--; 
	if (coor.row >= 0 && m_board.getBoardSquare(coor) == 'X')
	{
		removeSankShip(coor);
	}
	coor.row++;

	coor.col--;
	if (coor.col >= 0 && m_board.getBoardSquare(coor) == 'X')
	{
		removeSankShip(coor);
	}
	coor.col++; 

	coor.depth--;
	if (coor.depth >= 0 && m_board.getBoardSquare(coor) == 'X')
	{
		removeSankShip(coor); 
	}
	coor.depth++;

	coor.row++;
	if (coor.row <= m_board.rows() - 1 && m_board.getBoardSquare(coor) == 'X')
	{
		removeSankShip(coor);
	}
	coor.row--;

	coor.col++;
	if (coor.col <= m_board.cols() - 1 && m_board.getBoardSquare(coor) == 'X')
	{
		removeSankShip(coor);
	}
	coor.col--; 

	coor.depth++;
	if (coor.depth <= m_board.depth() - 1 && m_board.getBoardSquare(coor) == 'X')
	{
		removeSankShip(coor); 
	}
}

bool Game::endOfAttacks() const
{
	if (m_playerIsDone[0] && m_playerIsDone[1])
	{
		return true;
	}

	return false;
}

int Game::getShipLen(char ship)
{
	switch (ship)
	{
	case 'B':
	case 'b':
		return BattleshipGameUtils::Constants::RUBBER_BOAT_LEN;
	case 'P':
	case 'p':
		return BattleshipGameUtils::Constants::MISSILE_BOAT_LEN;
	case 'M':
	case 'm':
		return BattleshipGameUtils::Constants::SUBMARINE_LEN;
	case 'D':
	case 'd':
		return BattleshipGameUtils::Constants::DESTROYER_LEN;
	default:
		return 0;
	}
}
AttackResult Game::determineAttackResult(char square, Coordinate coor) const
{
	auto len = getShipLen(square);
	auto tmp = len - 1;
	char curr;
	int rowCoord = coor.row, colCoord = coor.col, depthCoord = coor.depth;

	if (square == 'X')
	{
		return AttackResult::Hit;
	}

	while (rowCoord > len - tmp - 1 && tmp > 0)
	{
		CHECK_CURRENT_SQUARE(rowCoord - (len - tmp), colCoord, depthCoord);
	}

	tmp = len - 1;

	while (colCoord > len - tmp - 1 && tmp > 0)
	{
		CHECK_CURRENT_SQUARE(rowCoord, colCoord - (len - tmp), depthCoord);
	}

	tmp = len - 1;
	
	while (depthCoord > len - tmp - 1 && tmp > 0)
	{
		CHECK_CURRENT_SQUARE(rowCoord, colCoord, depthCoord - (len - tmp));
	}

	tmp = len - 1;

	while (rowCoord < m_board.rows() - (len - tmp) && tmp > 0)
	{
		CHECK_CURRENT_SQUARE(rowCoord + (len - tmp), colCoord, depthCoord);
	}

	tmp = len - 1;

	while (colCoord < m_board.cols() - (len - tmp) && tmp > 0)
	{
		CHECK_CURRENT_SQUARE(rowCoord, colCoord + (len - tmp), depthCoord);
	}

	tmp = len - 1;

	while (depthCoord < m_board.depth() - (len - tmp) && tmp > 0)
	{
		CHECK_CURRENT_SQUARE(rowCoord, colCoord, depthCoord + (len - tmp));
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
			points = BattleshipGameUtils::Constants::RUBBER_BOAT_POINTS;
			break;
		case 'P':
		case 'p':
			points = BattleshipGameUtils::Constants::MISSILE_BOAT_POINTS;
			break;
		case 'M':
		case 'm':
			points = BattleshipGameUtils::Constants::SUBMARINE_POINTS;
			break;
		case 'D':
		case 'd':
			points = BattleshipGameUtils::Constants::DESTROYER_POINTS;
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

Game::GameState Game::playMove()
{
	AttackResult result;
	int  winner = -1, currentPlayer = m_nextPlayer;
	auto playerA = m_players[0];
	auto playerB = m_players[1];

	/*gets player's next move*/
	Coordinate attackCoord = m_players[currentPlayer]->attack();
	if (attackCoord.row == -1 && attackCoord.col == -1 && attackCoord.depth == -1)
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
	attackCoord.row--; 
	attackCoord.col--;
	attackCoord.depth--; 

	if (!m_board.coordinateIsValid(attackCoord))
	{
		return GameState::CONTINUE_PLAYING;
	}

	/*gets square*/
	auto square = m_board.getBoardSquare(attackCoord);

	/*determines if the next turn can be passed to the opponent */
	bool canPass = canPassTurn(currentPlayer);

	/*determines the attack result and sets the next Turn and the player's points accordingly*/
	if (square == BattleshipGameUtils::Constants::SPACE || square == '@')
	{
		result = AttackResult::Miss;
		m_nextPlayer ^= (canPass ? 1 : 0); /* If canPassTurn -> nextPlayer changes */
	}
	else
	{
		m_board.setBoardSquare(attackCoord, 'X');
		result = determineAttackResult(square, attackCoord);

		handlePointsAndNextTurn(result, square, currentPlayer, isupper(square), canPass);
		if (result == AttackResult::Sink)
		{
			removeSankShip(attackCoord);
		}
	}

	/*notifies players on the current result*/
	attackCoord.row++;
	attackCoord.col++;
	attackCoord.depth++;
	playerA->notifyOnAttackResult(currentPlayer, attackCoord, result);
	playerB->notifyOnAttackResult(currentPlayer, attackCoord, result);

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
		//TODO
		return GameState::GAME_OVER;
	}

	return GameState::CONTINUE_PLAYING;
}


