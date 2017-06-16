#pragma once

#include "GameBoard.h"
#include "Game.h"
#include "IBattleshipGameAlgo.h"
#include "Logger.h"
#include <vector>
#include <thread>
#include <unordered_map>

enum class Direction;

// A pair representing both rounds for each match
using shuffledPair = std::pair<std::pair<int, int>, std::pair<int, int>>;

class TournamentManager
{
public:
	int m_numOfActiveThreads;
	std::condition_variable m_fullThreadCapCv;
	std::mutex m_gamesMutex;

	TournamentManager(int numOfThread, const std::string& loggerFilePath,
				LoggerLevel level = LoggerLevel::WARNING);
	
	bool initializeBoards(const std::vector<std::string>& boardNames, std::string dirPath);
	bool initializeDlls(const std::vector<std::string>& dllNames, std::string dirPath);
	void startTournament();

private:
	int								m_numOfThreads;
	Logger							m_logger;
	std::vector<GameBoard>			m_gameBoards;
	std::vector<std::string>		m_playerNames;
	std::vector<std::function<IBattleshipGameAlgo*()>>	m_functionPointers;

	static const std::string FUNCTION_NAME;

	void printStartingMessage() const;

	void checkAndCreateBoard(const std::string& boardFile);

	GameBoard readBoardFromFile(const std::string& boardFilePath, bool& boardIsValid);

	bool boardIsValid(GameBoard& board);

	bool dfsShip(char currShip, const GameBoard& gameBoard, const GameBoard& dummyBoard,
		Coordinate coor, int & shipLen, Direction direction);

	bool dimensionsAreValid(std::string& firstLine, std::vector<int>& dims) const;

	bool markAllOfSameShip(char currShip, const GameBoard& gameBoard, const GameBoard& dummyBoard, Coordinate coor);

	bool dfsShipHelper(Direction direction, Direction currentDirection, char currShip, bool& stuck,
		const GameBoard & gameBoard, const GameBoard & dummyBoard, Coordinate coor, int & shipLen);

	bool checkAdjacentSquare(char currShip, const GameBoard & gameBoard, const GameBoard & dummyBoard, Coordinate coor);

	static void gamesScheduler(std::vector<shuffledPair>& matches, std::vector<int>& ids);

	static void fillBoardWithEmptyLayers(const GameBoard& gameBoard, int z);

	static bool shipTypesBalanced(std::array<std::unordered_map<char, int>, 2>& shipTypes);
};