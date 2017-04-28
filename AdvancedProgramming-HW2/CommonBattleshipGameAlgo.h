#pragma once
#include "IBattleshipGameAlgo.h"
#include "BattleshipUtils.h"


class CommonBattleshipGameAlgo : public IBattleshipGameAlgo
{
public:
	CommonBattleshipGameAlgo() {}

	/**
	* @implements IBattleshipGameAlgo::setBoard pure virtual function
	*
	* Copies board[i][j] to this->board[i][j]
	*
	* @param player - the player's index as given by game manager
	* @param board - the board to copy of
	* @param numRows - board's number of rows
	* @param numCols - board's number of columns
	*/
	virtual void setBoard(int player, const char** board, int numRows, int numCols) override;
	virtual bool init(const std::string& str) override { return true; }

	bool	isDone() const { return m_isDone; } //returns true if the player has no more moves, and false otherwise
	size_t	getPoints() const { return m_points; } //returns the player's current points 
	size_t	getShipCounter() const { return m_shipCounter; } //returns the number of ships the player has on board 
	int		getPlayerID() const { return m_playerID; }

	void setPoints(size_t pts) { m_points = pts; } //sets the player's points to pts
	void incrementShipCounter() { m_shipCounter++; }

private:
	class Board
	{
	public:
		
	private:
		char** m_board	= nullptr;
		size_t m_rows	= 0;
		size_t m_cols	= 0;
		
		friend class CommonBattleshipGameAlgo;

		Board() {}
		Board(const Board& b) = delete;
		Board& operator=(const Board& b) = delete;
		~Board();

		void setMembers(const char** board, size_t rows, size_t cols);
		void setBoardSquare(int i, int j, char val) const { m_board[i][j] = val; }

		char getBoardSquare(int i, int j) const { return m_board[i][j]; }
	};

	Board	m_myBoard;
	size_t	m_points		= 0;
	size_t	m_shipCounter	= 0;
	bool	m_isDone		= false;
	int		m_playerID		= 0;
};


