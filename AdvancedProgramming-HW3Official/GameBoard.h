#pragma once

#include "IBattleshipGameAlgo.h"
#include <memory>
#include <array>

using char_array	= std::unique_ptr<char[]>;
using char_matrix	= std::unique_ptr<char_array[]>;
using char_multidim = std::unique_ptr<char_matrix[]>;

class GameBoard
{
public:
	//GameBoard()
	GameBoard(int rows, int cols, int depth, bool defaultInit = false);
	GameBoard(GameBoard&& board) noexcept;
	GameBoard(const GameBoard& board) = delete;
	GameBoard& operator=(const GameBoard& board) = delete;

	char getBoardSquare(Coordinate coor) const;
	int rows()	const { return m_rows; }
	int cols()	const { return m_cols; }
	int depth() const { return m_depth; }
	void setBoardSquare(Coordinate coor, char sq) const;
	Coordinate getNextEmptySquare() const; //TODO: maybe player will implement
	bool coordinateIsValid(Coordinate coor) const;

	void incrementShipForPlayer(int player) { m_playerShipCount[player]++; }
	void printBoard(); //TODO: delete!!! !!! !!!!!!!!!! TODO TODO

private:
	int m_rows;
	int m_cols;
	int m_depth;
	char_multidim m_gameBoard;
	std::array<int, 2> m_playerShipCount;
};

