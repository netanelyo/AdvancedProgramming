#pragma once

#include "PlayerBoard.h"
#include <memory>
#include <array>

using char_array	= std::unique_ptr<char[]>;
using char_matrix	= std::unique_ptr<char_array[]>;
using char_multidim = std::unique_ptr<char_matrix[]>;

class GameBoard : public PlayerBoard
{
public:
	GameBoard(int rows, int cols, int depth, bool defaultInit = false);
	GameBoard(GameBoard && board) noexcept : 
		PlayerBoard(std::move(board)),	
		m_playerShipCount(std::move(board.m_playerShipCount)), 
		m_boardFileName(board.m_boardFileName) {}
	GameBoard(const GameBoard& board);
	GameBoard& operator=(const GameBoard& board);

	void incrementShipForPlayer(int player) { m_playerShipCount[player]++; }
	int getShipCountForPlayer(int player) const { return m_playerShipCount[player]; }
	void printBoard(std::ofstream& toFile) const; //TODO: delete!!! !!! !!!!!!!!!! TODO TODO

	void setBoardName(const std::string& fileName) { m_boardFileName = fileName; }
	std::string name() const { return m_boardFileName; }

private:
	std::array<int, 2> m_playerShipCount;
	std::string m_boardFileName;

	void copyValuesToBoard(const GameBoard& board) const; 
};

