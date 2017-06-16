#include "Game.h"
#include "BoardDataImpl.h"


#define CHECK_CURRENT_SQUARE(i, j, k)	curr = m_board.getBoardSquare(Coordinate(i, j, k)); \
										if (curr == Constant::EMPTY_SIGN) break; \
										if (curr == square) return AttackResult::Hit; \
										tmp--

Game::Game(GameBoard board, std::shared_ptr<Player> playerA, std::shared_ptr<Player> playerB, int playerAId, int playerBId) :
	m_board(board)
{
	setDataStructs(playerA, playerB, playerAId, playerBId);
}

void Game::runGame(Printer& printer)
{
	initPlayers();
	while (playMove() != GameState::GAME_OVER) {}
	
	for (auto i = 0; i < 2; i++)
	{
		auto win = 0, lose = 0;
		if (m_winner != -1)
		{
			win = m_winner == i ? 1 : 0;
			lose = 1 - win;
		}
		ConcurrentVector::Statistics stats({m_playersPoints[i], m_playersPoints[1 - i], 1, win, lose});
		printer.m_playersResults[m_playersIDs[i]].addAndNotify(stats);
	}
}

void Game::start(GameBoard board, std::shared_ptr<Player> A, std::shared_ptr<Player> B, int idA, int idB,
	Printer& printer, int& active, std::condition_variable& cv, std::mutex& m)
{
	Game game(board, A, B, idA, idB);
	game.runGame(printer);
	std::lock_guard<std::mutex> lk(m);
	--active;
	cv.notify_one();
}

void Game::setDataStructs(std::shared_ptr<Player> playerA, std::shared_ptr<Player> playerB, int playerAId, int playerBId)
{
	m_players[Constant::PLAYER_A] = playerA;
	m_players[Constant::PLAYER_B] = playerB;

	m_playersIDs[Constant::PLAYER_A] = playerAId; 
	m_playersIDs[Constant::PLAYER_B] = playerBId;

	m_playersShips[Constant::PLAYER_A] = m_board.getShipCountForPlayer(Constant::PLAYER_A);
	m_playersShips[Constant::PLAYER_B] = m_board.getShipCountForPlayer(Constant::PLAYER_B);

	m_playersPoints[Constant::PLAYER_A] = 0;
	m_playersPoints[Constant::PLAYER_B] = 0;

	m_playerIsDone[Constant::PLAYER_A] = false;
	m_playerIsDone[Constant::PLAYER_B] = false;

	m_nextPlayer = Constant::PLAYER_A;
}

void Game::initPlayers()
{
	m_players[Constant::PLAYER_A]->setPlayer(Constant::PLAYER_A);
	m_players[Constant::PLAYER_B]->setPlayer(Constant::PLAYER_B);

	BoardDataImpl boardDataForPlayers(m_board, Constant::PLAYER_A);
	m_players[Constant::PLAYER_A]->setBoard(boardDataForPlayers);
	boardDataForPlayers.setPlayer(Constant::PLAYER_B);
	m_players[Constant::PLAYER_B]->setBoard(boardDataForPlayers);
}

void Game::removeSankShip(Coordinate coor) const
{
	m_board.setBoardSquare(coor, Constant::SINK_SIGN);

	coor.row--; 
	if (coor.row >= 0 && m_board.getBoardSquare(coor) == Constant::HIT_SIGN)
	{
		removeSankShip(coor);
	}
	coor.row++;

	coor.col--;
	if (coor.col >= 0 && m_board.getBoardSquare(coor) == Constant::HIT_SIGN)
	{
		removeSankShip(coor);
	}
	coor.col++; 

	coor.depth--;
	if (coor.depth >= 0 && m_board.getBoardSquare(coor) == Constant::HIT_SIGN)
	{
		removeSankShip(coor); 
	}
	coor.depth++;

	coor.row++;
	if (coor.row <= m_board.rows() - 1 && m_board.getBoardSquare(coor) == Constant::HIT_SIGN)
	{
		removeSankShip(coor);
	}
	coor.row--;

	coor.col++;
	if (coor.col <= m_board.cols() - 1 && m_board.getBoardSquare(coor) == Constant::HIT_SIGN)
	{
		removeSankShip(coor);
	}
	coor.col--; 

	coor.depth++;
	if (coor.depth <= m_board.depth() - 1 && m_board.getBoardSquare(coor) == Constant::HIT_SIGN)
	{
		removeSankShip(coor); 
	}
}

bool Game::endOfAttacks() const
{
	if (m_playerIsDone[Constant::PLAYER_A] && m_playerIsDone[Constant::PLAYER_B])
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
		return Constant::RUBBER_BOAT_LEN;
	case 'P':
	case 'p':
		return Constant::MISSILE_BOAT_LEN;
	case 'M':
	case 'm':
		return Constant::SUBMARINE_LEN;
	case 'D':
	case 'd':
		return Constant::DESTROYER_LEN;
	default:
		return 0;
	}
}
AttackResult Game::determineAttackResult(char square, Coordinate coor) const
{
	auto len = getShipLen(square);
	auto tmp = len - 1;
	char curr;
	auto rowCoord = coor.row, colCoord = coor.col, depthCoord = coor.depth;

	if (square == Constant::HIT_SIGN)
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
		|| ship == Constant::HIT_SIGN) && canPassTurn;

	/*handles points in case of a sink*/
	if (result == AttackResult::Sink)
	{
		uint16_t points;
		auto playerIndex = isAShip ? Constant::PLAYER_B : Constant::PLAYER_A;

		switch (ship)
		{
		case 'B':
		case 'b':
			points = Constant::RUBBER_BOAT_POINTS;
			break;
		case 'P':
		case 'p':
			points = Constant::MISSILE_BOAT_POINTS;
			break;
		case 'M':
		case 'm':
			points = Constant::SUBMARINE_POINTS;
			break;
		case 'D':
		case 'd':
			points = Constant::DESTROYER_POINTS;
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
	auto playerA = m_players[Constant::PLAYER_A];
	auto playerB = m_players[Constant::PLAYER_B];

	/*gets player's next move*/
	auto attackCoord = m_players[currentPlayer]->attack();
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
	auto canPass = canPassTurn(currentPlayer);

	/*determines the attack result and sets the next Turn and the player's points accordingly*/
	if (square == Constant::EMPTY_SIGN || square == Constant::HIT_SIGN)
	{
		result = AttackResult::Miss;
		m_nextPlayer ^= (canPass ? 1 : 0); /* If canPassTurn -> nextPlayer changes */
	}
	else
	{
		m_board.setBoardSquare(attackCoord, Constant::HIT_SIGN);
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
	if (m_playersShips[Constant::PLAYER_A] == 0)
	{
		winner = Constant::PLAYER_B;
	}
	else if (m_playersShips[Constant::PLAYER_B] == 0)
	{
		winner = Constant::PLAYER_A;
	}

	/*if there's a winner, we end the game*/
	if (winner > -1)
	{
		m_winner = winner;
		return GameState::GAME_OVER;
	}

	return GameState::CONTINUE_PLAYING;
}


