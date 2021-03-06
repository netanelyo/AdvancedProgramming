#include "TournamentManager.h"
#include "BattleshipGameUtils.h"

#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <mutex>
#include <condition_variable>
#include "Printer.h"

#define LEGAL "Number of legal "
#define DEFAULT_THREADS 4

const std::string TournamentManager::FUNCTION_NAME = "GetAlgorithm";

TournamentManager::TournamentManager(int numOfThread, const std::string& loggerFilePath, LoggerLevel level)
	: m_numOfActiveThreads(0), m_numOfThreads(numOfThread), m_logger(loggerFilePath, level)
{
	if (m_numOfThreads <= 0)
	{
		m_logger.printToLogger(Logger::LoggerMessage::WARNING_INVALID_NUM_THREADS, LoggerLevel::WARNING);
		m_numOfThreads = DEFAULT_THREADS;
	}
}

bool TournamentManager::initializeBoards(const std::vector<std::string>& boardNames, std::string dirPath)
{
	for (const auto& boardName: boardNames)
	{
		checkAndCreateBoard(dirPath + boardName); 
	}

	return !m_gameBoards.empty();
}

bool TournamentManager::initializeDlls(const std::vector<std::string>& dllNames, std::string dirPath)
{
	typedef IBattleshipGameAlgo* (*GetPlayerType)();

	for (const auto& dllName: dllNames)
	{	
		auto dllPath = dirPath + dllName;
		auto hDll = LoadLibraryA(dllPath.c_str());
		if (!hDll)
		{
			m_logger.printToLogger(Logger::LoggerMessage::ERROR_CANT_OPEN_DLL + dllName, LoggerLevel::ERR);
			continue;
		}
		auto getPlayer = GetPlayerType(GetProcAddress(hDll, FUNCTION_NAME.c_str()));
		if (!getPlayer)
		{
			m_logger.printToLogger(Logger::LoggerMessage::ERROR_CANT_LOAD_DLL + dllName, LoggerLevel::ERR);
			continue;
		}

		m_functionPointers.push_back(getPlayer);
		m_playerNames.push_back(dllName);
	}

	return m_functionPointers.size() >= 2;
	
}

void TournamentManager::startTournament()
{
	printStartingMessage();
	Sleep(2000); // To let the opening message be shown for a reasonable time
	auto numOfPlayers = int(m_functionPointers.size());
	auto numOfBoards = int(m_gameBoards.size());
	auto numOfGames = numOfPlayers * (numOfPlayers - 1) * numOfBoards;
	
	if (m_numOfThreads > numOfGames)
	{
		m_logger.printToLogger(Logger::LoggerMessage::WARNING_LARGE_NUM_THREADS, LoggerLevel::WARNING);
		m_numOfThreads = numOfGames;
	}

	std::vector<int> ids;
	for (auto i = 0; i < numOfPlayers; i++)
	{
		ids.push_back(i);
	}

	std::vector<shuffledPair> matches;
	gamesScheduler(matches, ids);
	Printer printer((numOfPlayers - 1) * 2 * numOfBoards, m_playerNames);

	std::thread reader(&Printer::start, std::ref(printer));

	for (auto i = 0; i < 2; i++)
	{
		for (const auto& board : m_gameBoards)
		{
			for (const auto& match : matches)
			{
				std::pair<int, int> currMatch;
				switch (i)
				{
				case 0:
					currMatch = match.first;
					break;

				case 1:
					currMatch = match.second;
					break;

				default:
					break;
				}

				std::unique_lock<std::mutex> lk(m_gamesMutex);
				if (m_numOfActiveThreads >= m_numOfThreads)
				{
					m_fullThreadCapCv.wait(lk, [this] { return m_numOfActiveThreads < m_numOfThreads; });
				}
				++m_numOfActiveThreads;
				lk.unlock();

				auto idA = currMatch.first, idB = currMatch.second;
				std::shared_ptr<Player> playerA(m_functionPointers[idA]()), playerB(m_functionPointers[idB]());
				
				std::thread t(Game::start, board, playerA, playerB, idA, idB, std::ref(printer),
								std::ref(m_numOfActiveThreads), std::ref(m_fullThreadCapCv), std::ref(m_gamesMutex));
				t.detach();
			}
		}
	}
	reader.join();
}

void TournamentManager::printStartingMessage() const
{
	std::cout << LEGAL << "players: " << m_functionPointers.size() << std::endl;
	std::cout << LEGAL << "boards: " << m_gameBoards.size();
}

void TournamentManager::checkAndCreateBoard(const std::string& boardFilePath)
{
	auto boardFileIsValid = true;
	auto gameBoard = readBoardFromFile(boardFilePath, boardFileIsValid);
	
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
		m_logger.printToLogger(Logger::LoggerMessage::ERROR_CANT_OPEN_BOARD_FILE + boardFilePath, LoggerLevel::ERR);
		return GameBoard(0, 0, 0);
	}

	if (!std::getline(boardFile, line) || !dimensionsAreValid(line, dims))
	{
		boardIsValid = false;
		m_logger.printToLogger(Logger::LoggerMessage::ERROR_WRONG_DIMENSIONS_LINE + boardFilePath, LoggerLevel::ERR);
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
			m_logger.printToLogger(Logger::LoggerMessage::ERROR_ADJACENT_SHIPS + gameBoard.name(), LoggerLevel::ERR);
			return false;
		}
	}

	return true;
}

void TournamentManager::gamesScheduler(std::vector<shuffledPair>& matches, std::vector<int>& ids)
{
	if (ids.size() % 2 == 1)
		ids.push_back(-1);

	auto size = ids.size();
	auto numOfPlayers = size;
	auto numOfRounds = size - 1;
	auto half = size / 2;

	for (size_t i = 0; i < numOfRounds; i++)
	{
		for (size_t j = 0; j < half; j++)
		{
			// Push to vector pairs of player ids and the transposition
			if (ids[numOfPlayers - j - 1] == -1 || ids[j] == -1)
				continue;

			matches.push_back({ { ids[j], ids[numOfPlayers - j - 1] }, { ids[numOfPlayers - j - 1], ids[j] } });
		}
		ids.insert(ids.begin() + 1, ids[size - 1]);
		ids.pop_back();
	}
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
			m_logger.printToLogger(Logger::LoggerMessage::ERROR_INVALID_SHAPE_OR_SIZE + gameBoard.name(), LoggerLevel::ERR);
			return false;
		}

		shipLen++;
		dummyBoard.setBoardSquare(coor, 1);

		if (!dfsShip(currShip, gameBoard, dummyBoard, coor, shipLen, direction))
			return false;
	}

	else if (gameBoard.getBoardSquare(coor) != BattleshipGameUtils::Constants::EMPTY_SIGN)
	{
		m_logger.printToLogger(Logger::LoggerMessage::ERROR_ADJACENT_SHIPS + gameBoard.name(), LoggerLevel::ERR);
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
						m_logger.printToLogger(Logger::LoggerMessage::ERROR_INVALID_SHAPE_OR_SIZE + board.name(), LoggerLevel::ERR);
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