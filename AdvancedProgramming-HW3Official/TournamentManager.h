#pragma once

#include "GameBoard.h"
#include "Game.h"
#include "IBattleshipGameAlgo.h"
#include "Logger.h"
#include <vector>
#include <thread>
#include <unordered_map>

enum class Direction;

class TournamentManager
{
public:
	TournamentManager(int numOfThread, const std::string& loggerFilePath,
						LoggerLevel level = LoggerLevel::WARNING) :
							m_numOfThreads(numOfThread), m_logger(loggerFilePath, level) {}
	
	bool initializeBoards(const std::vector<std::string>& boardNames, std::string dirPath);
	bool initializeDlls(const std::vector<std::string>& dllNames, std::string dirPath);
	void startTournament();

private:
	int							m_numOfThreads;
	std::vector<std::thread>	m_threadPool;
	std::vector<GameBoard>		m_gameBoards;
	std::vector<std::function<IBattleshipGameAlgo*()>> m_functionPointers;
	std::vector<Game>			m_games;
	std::vector<std::shared_ptr<IBattleshipGameAlgo>> m_players;
	Logger						m_logger;

	static const std::string FUNCTION_NAME;

	void printStartingMessage() const;

	void checkAndCreateBoard(const std::string& boardFile);

	/**
	* parses the board from the board file
	*
	* @param boardFilePath - a file path to a potential board file
	*/
	GameBoard readBoardFromFile(const std::string& boardFilePath, bool& boardIsValid);

	/*
	* check the board meets the following requirements:
	* each ship has the right size and there are no adjacent ships.
	* If one of these requirements isn't met, we mark in shouldPrint array that this error occurred
	*/
	bool boardIsValid(GameBoard& board);

	/**
	*
	* searches for all neighboring appearances of currShip on board in order to determine the validity of the board
	*
	* @param currShip - the symbol of the current ship
	* @param dummyBoard - a binary matrix
	* @param coor - the currShip coordinate
	* @param shipLen - the current length of currShip on board
	* @param direction - an enum representing the expected direction of currShip on board
	*/
	bool dfsShip(char currShip, const GameBoard& gameBoard, const GameBoard& dummyBoard,
		Coordinate coor, int & shipLen, Direction direction);

	bool dimensionsAreValid(std::string& firstLine, std::vector<int>& dims) const;

	/**
	* an helper function of dfsShip that finds all neighboring appearances of currShip on board
	*
	* @param currShip - the symbol of the current ship
	* @param dummyBoard - a binary matrix
	* @param coor - the currShip coordinate
	*/
	bool markAllOfSameShip(char currShip, const GameBoard& gameBoard, const GameBoard& dummyBoard, Coordinate coor);

	bool dfsShipHelper(Direction direction, Direction currentDirection, char currShip, bool& stuck,
		const GameBoard & gameBoard, const GameBoard & dummyBoard, Coordinate coor, int & shipLen);

	bool checkAdjacentSquare(char currShip, const GameBoard & gameBoard, const GameBoard & dummyBoard, Coordinate coor);

	static void fillBoardWithEmptyLayers(const GameBoard& gameBoard, int z);

	static bool shipTypesBalanced(std::array<std::unordered_map<char, int>, 2>& shipTypes);
};
