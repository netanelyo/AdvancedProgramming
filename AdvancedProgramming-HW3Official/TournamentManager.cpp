#include "TournamentManager.h"

#include <sstream>
#include <fstream>
#include <algorithm>
#include "BattleshipGameUtils.h"
#include <iostream>


int main(int argc, char** argv)
{
	TournamentManager t(1, "game.log", LoggerLevel::ERROR);
	
	t.initializeBoards();
}

bool TournamentManager::initializeBoards()
{
	checkAndCreateBoard(std::string("bad_board_4.sboard"));
	std::cout << m_gameBoards.size() << std::endl;
	std::ofstream file("testBoard.board", std::ios::trunc | std::ios::out);
	//m_gameBoards.back().printBoard(file);
	file.close();
	return true;
}

void TournamentManager::checkAndCreateBoard(const std::string& boardFilePath)
{
	auto boardFileIsValid = true;
	auto gameBoard = readBoardFromFile(boardFilePath, boardFileIsValid);
	
	std::cout << "Before boardFileIsValid" << std::endl;
	if (!boardFileIsValid)
		return;

	if (boardIsValid(gameBoard))
		m_gameBoards.push_back(std::move(gameBoard));

}

bool TournamentManager::dimensionsAreValid(std::string & firstLine, std::vector<int>& dims) const
{
	std::transform(firstLine.begin(), firstLine.end(), firstLine.begin(), ::tolower);
	return BattleshipGameUtils::splitStringByDelimiter(firstLine, dims, 'x');
}

void TournamentManager::fillBoardWithEmptyLayers(const GameBoard & gameBoard, int z)
{
	for (auto depth = z; depth < gameBoard.depth(); depth++)
	{
		for (auto row = 0; row < gameBoard.rows(); row++)
		{
			for (auto col = 0; col < gameBoard.cols(); col++)
			{
				gameBoard.setBoardSquare(Coordinate(row, col, depth), BattleshipGameUtils::Constants::EMPTY_SIGN);
			}
		}
	}
}

bool TournamentManager::shipTypesBalanced(std::array<std::unordered_map<char, int>, 2>& shipTypes)
{
	for (auto& type : shipTypes[0])
	{
		if (type.second != shipTypes[1][tolower(type.first)])
		{
			return false;
		}
	}
	return true;
}

GameBoard TournamentManager::readBoardFromFile(const std::string& boardFilePath, bool& boardIsValid)
{
	std::string			line;
	std::vector<int>	dims;
	std::ifstream boardFile(boardFilePath);
	if (!boardFile.is_open())
	{
		boardIsValid = false;
		m_logger.printToLogger(Logger::LoggerMessage::ERROR_CANT_OPEN_BOARD_FILE + boardFilePath, LoggerLevel::ERROR);
		return GameBoard(0, 0, 0);
	}

	if (!std::getline(boardFile, line) || !dimensionsAreValid(line, dims))
	{
		boardIsValid = false;
		m_logger.printToLogger(Logger::LoggerMessage::ERROR_WRONG_DIMENSIONS_LINE + boardFilePath, LoggerLevel::ERROR);
		return GameBoard(0, 0, 0);
	}
	GameBoard gameBoard(dims[1], dims[0], dims[2]);
	gameBoard.setBoardName(boardFilePath);

	std::getline(boardFile, line); // Skip line after dims

	for (auto z = 0; z < gameBoard.depth(); ++z)
	{
		if (!boardFile)
		{
			fillBoardWithEmptyLayers(gameBoard, z);
			return std::move(gameBoard);
		}
		
		auto needToFill = false;

		for (auto row = 0; row < gameBoard.rows(); ++row)
		{
			int len;
			if (!needToFill)
				std::getline(boardFile, line);

			if (line.empty() || !line.compare("\r"))
			{
				needToFill = true;
				len = 0;
			}
			else
			{
				len = int(line.length());
			}

			for (auto col = 0; col < gameBoard.cols(); ++col)
			{
				Coordinate currentCoor(row, col, z);
				if (col < len)
				{
					auto sq = line[col];
					switch (sq)
					{
					case 'B':
					case 'b':
					case 'P':
					case 'p':
					case 'M':
					case 'm':
					case 'D':
					case 'd':
						gameBoard.setBoardSquare(currentCoor, sq);
						break;
					default:
						gameBoard.setBoardSquare(currentCoor, BattleshipGameUtils::Constants::EMPTY_SIGN);
						break;
					}
				}
				else
					gameBoard.setBoardSquare(currentCoor, BattleshipGameUtils::Constants::EMPTY_SIGN);
			}
		}

		// Skip extra lines until EOF or an empty line
		do
		{
			std::getline(boardFile, line);
		} while (!line.empty() && line.compare("\r") && boardFile);
	}

	boardFile.close();
	return std::move(gameBoard);
}

bool TournamentManager::checkAdjacentSquare(char currShip, const GameBoard & gameBoard, const GameBoard & dummyBoard, Coordinate coor)
{
	if (dummyBoard.getBoardSquare(coor) == 0)
	{
		if (gameBoard.getBoardSquare(coor) == currShip)
		{
			dummyBoard.setBoardSquare(coor, 1);
			if (!markAllOfSameShip(currShip, gameBoard, dummyBoard, coor))
				return false;
		}

		else if (gameBoard.getBoardSquare(coor) != BattleshipGameUtils::Constants::EMPTY_SIGN)
		{
			m_logger.printToLogger(Logger::LoggerMessage::ERROR_ADJACENT_SHIPS + gameBoard.name(), LoggerLevel::ERROR);
			return false;
		}
	}

	return true;
}

bool TournamentManager::markAllOfSameShip(char currShip, const GameBoard & gameBoard, const GameBoard & dummyBoard, Coordinate coor)
{
	auto row = coor.row, col = coor.col, depth = coor.depth;
	if (row < gameBoard.rows() - 1)
	{
		coor.row++;
		if (!checkAdjacentSquare(currShip, gameBoard, dummyBoard, coor))
			return false;
		coor.row--;
	}

	if (col < gameBoard.cols() - 1)
	{
		coor.col++;
		if (!checkAdjacentSquare(currShip, gameBoard, dummyBoard, coor))
			return false;
		coor.col--;
	}

	if (depth < gameBoard.depth() - 1)
	{
		coor.depth++;
		if (!checkAdjacentSquare(currShip, gameBoard, dummyBoard, coor))
			return false;
		coor.depth--;
	}

	if (row > 0)
	{
		coor.row--;
		if (!checkAdjacentSquare(currShip, gameBoard, dummyBoard, coor))
			return false;
		coor.row++;
	}

	if (col > 0)
	{
		coor.col--;
		if (!checkAdjacentSquare(currShip, gameBoard, dummyBoard, coor))
			return false;
		coor.col++;
	}

	if (depth > 0)
	{
		coor.depth--;
		if (!checkAdjacentSquare(currShip, gameBoard, dummyBoard, coor))
			return false;
	}

	return true;
}

bool TournamentManager::dfsShipHelper(Direction direction, Direction currentDirection, char currShip, bool& stuck,
					const GameBoard& gameBoard, const GameBoard& dummyBoard, Coordinate coor, int& shipLen)
{
	if (gameBoard.getBoardSquare(coor) == currShip)
	{
		if (currentDirection != Direction::NON && currentDirection != direction)
		{
			m_logger.printToLogger(Logger::LoggerMessage::ERROR_INVALID_SHAPE_OR_SIZE + gameBoard.name(), LoggerLevel::ERROR);
			return false;
		}

		shipLen++;
		dummyBoard.setBoardSquare(coor, 1);

		if (!dfsShip(currShip, gameBoard, dummyBoard, coor, shipLen, direction))
			return false;
	}

	else if (gameBoard.getBoardSquare(coor) != BattleshipGameUtils::Constants::EMPTY_SIGN)
	{
		m_logger.printToLogger(Logger::LoggerMessage::ERROR_ADJACENT_SHIPS + gameBoard.name(), LoggerLevel::ERROR);
		return false;
	}

	else
		stuck = true;

	return true;
}

bool TournamentManager::dfsShip(char currShip, const GameBoard& gameBoard, const GameBoard& dummyBoard,
				Coordinate coor, int & shipLen, Direction direction)
{
	auto stuckDown = false;
	auto stuckRight = false;
	auto stuckIn = false;
	auto row = coor.row, col = coor.col, depth = coor.depth;
	if (row < gameBoard.rows() - 1)
	{
		coor.row++;
		if (!dfsShipHelper(Direction::VERTICAL, direction, currShip, stuckDown, gameBoard, dummyBoard, coor, shipLen))
			return false;
		coor.row--;
	}
	else
	{
		stuckDown = true;
	}

	if (col < gameBoard.cols() - 1)
	{
		coor.col++;
		if (!dfsShipHelper(Direction::HORIZONTAL, direction, currShip, stuckRight, gameBoard, dummyBoard, coor, shipLen))
			return false;
		coor.col--;
	}
	else
	{
		stuckRight = true;
	}

	if (depth < gameBoard.depth() - 1)
	{
		coor.depth++;
		if (!dfsShipHelper(Direction::DEPTH, direction, currShip, stuckIn, gameBoard, dummyBoard, coor, shipLen))
			return false;
		coor.depth--;
	}
	else
	{
		stuckIn = true;
	}

	if (stuckDown && stuckRight && stuckIn)
		return markAllOfSameShip(currShip, gameBoard, dummyBoard, coor);

	return true;
}

bool TournamentManager::boardIsValid(GameBoard & board)
{
	auto rows = board.rows(), cols = board.cols(), depth = board.depth();
	GameBoard dummyBoard(rows, cols, depth, true);
	auto shipLength = 1;
	std::array<std::unordered_map<char, int>, 2> shipTypesCountForPlayer =
	{
		std::unordered_map<char, int>({ { 'B', 0 }, { 'P', 0 }, { 'M', 0 }, { 'D', 0 } }),
		std::unordered_map<char, int>({ { 'b', 0 }, { 'p', 0 }, { 'm', 0 }, { 'd', 0 } })
	};
	
	for (auto z = 0; z < depth; z++)
	{
		for (auto row = 0; row < rows; ++row)
		{
			for (auto col = 0; col < cols; ++col)
			{
				Coordinate currentCoor(row, col, z); 
				if (dummyBoard.getBoardSquare(currentCoor) == 0)
				{
					auto currentShip = board.getBoardSquare(currentCoor);

					dummyBoard.setBoardSquare(currentCoor, 1);
					if (currentShip == BattleshipGameUtils::Constants::EMPTY_SIGN)
						continue;

					/* currentShip - candidate to be a valid ship */
					auto belongsToA = isupper(currentShip);

					if (!dfsShip(currentShip, board, dummyBoard, currentCoor, shipLength, Direction::NON))
						return false;

					if (shipLength != BattleshipGameUtils::getShipLength(currentShip))
					{
						m_logger.printToLogger(Logger::LoggerMessage::ERROR_INVALID_SHAPE_OR_SIZE + board.name(), LoggerLevel::ERROR);
						return false;
					}

					shipTypesCountForPlayer[!belongsToA][currentShip]++;
					board.incrementShipForPlayer(!belongsToA);
				}

				shipLength = 1;
			}
		}
	}

	if (board.getShipCountForPlayer(0) == 0 && board.getShipCountForPlayer(1) == 0)
		return false;

	if (!shipTypesBalanced(shipTypesCountForPlayer))
		m_logger.printToLogger(Logger::LoggerMessage::WARNING_UNBALANCED_BOARD + board.name(), LoggerLevel::WARNING);
	return true;
}
