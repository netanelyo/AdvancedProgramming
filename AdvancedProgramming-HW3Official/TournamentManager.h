#pragma once

#include <vector>
#include <thread>
#include "GameBoard.h"
#include "Game.h"
#include "IBattleshipGameAlgo.h"
#include "Logger.h"
#include <unordered_map>

enum class Direction;

class TournamentManager
{
public:
	TournamentManager(int numOfThread, const std::string& loggerFilePath, LoggerLevel level) :
	m_numOfThreads(numOfThread), m_logger(loggerFilePath, level) {}
	
	bool initializeBoards();

private:
	int							m_numOfThreads;
	std::vector<std::thread>	m_threadPool;
	std::vector<GameBoard>		m_gameBoards;
	std::vector<Game>			m_games;
	std::vector<std::shared_ptr<IBattleshipGameAlgo>> m_players;
	Logger						m_logger;

	void checkAndCreateBoard(const std::string& boardFile);

	/**
	* parses the board from the board file
	*
	* @param boardFile - a file stream to our board file
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
	* @param dummy - a binary matrix
	* @param row - the currShip row coordinate
	* @param col - the currShip column coordinate
	* @param shipLen - the current length of currShip on board
	* @param direction - an enum representing the expected direction of currShip on board
	* @param invalidShape - a boolean representing if the shape of currShip is invalid
	* @param adjShips - a boolean representing if there are adjacent ships on board
	*/
	bool dfsShip(char currShip, const GameBoard& gameBoard, const GameBoard& dummyBoard,
		Coordinate coor, int & shipLen, Direction direction);

	bool dimensionsAreValid(std::string& firstLine, std::vector<int>& dims) const;

	/**
	* an helper function of dfsShip that finds all neighboring appearances of currShip on board
	*
	* @param currShip - the symbol of the current ship
	* @param dummy - a binary matrix
	* @param row - the currShip row coordinate
	* @param col - the currShip col coordinate
	* @param invalidShape - a boolean representing if the shape of currShip is invalid
	* @param adjShips - a boolean representing if there are adjacent ships on board
	*/
	bool markAllOfSameShip(char currShip, const GameBoard& gameBoard, const GameBoard& dummyBoard, Coordinate coor);

	bool dfsShipHelper(Direction direction, Direction currentDirection, char currShip, bool& stuck,
		const GameBoard & gameBoard, const GameBoard & dummyBoard, Coordinate coor, int & shipLen);

	bool checkAdjacentSquare(char currShip, const GameBoard & gameBoard, const GameBoard & dummyBoard, Coordinate coor);

	static void fillBoardWithEmptyLayers(const GameBoard& gameBoard, int z);

	static bool shipTypesBalanced(std::array<std::unordered_map<char, int>, 2>& shipTypes);
};
