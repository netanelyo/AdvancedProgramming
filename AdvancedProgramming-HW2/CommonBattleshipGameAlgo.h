#pragma once
#include "IBattleshipGameAlgo.h"
#include "BattleshipUtils.h"
#include <fstream>


class CommonBattleshipGameAlgo : public IBattleshipGameAlgo
{
public:
	CommonBattleshipGameAlgo() : m_currentSeqMove(0, -1) {}

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

	/**
	* @implements IBattleshipGameAlgo::notifyOnAttackResult pure virtual function
	*
	* updates board if the attack result was a hit/sink of our ship
	* and decreases shipCounter by one if the attack result was a sink of our ship
	*
	* @param player - an integer represting the current player
	* @param row - the Y coordinate of the attack
	* @param col - the X coordinate of the attck
	* @param result - the attack result
	*
	*/
	virtual void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
	virtual bool init(const std::string& str) override { return true; }
	virtual std::pair<int, int> attack() override;

	bool	isDone() const { return m_isDone; } //returns true if the player has no more moves, and false otherwise
	int		getPlayerID() const { return m_playerID; }

protected:
	class Board
	{
	public:
		void setMembers(const char** board, size_t rows, size_t cols);
		void setBoardSquare(int i, int j, char val) const { m_board[i][j] = val; }

		char getBoardSquare(int i, int j) const { return m_board[i][j]; }
		size_t getBoardRows() const { return m_rows; }
		size_t getBoardCols() const { return m_cols; }
		std::pair<int, int> getNextEmptySquare(const std::pair<int, int>& pair) const;
		bool validateCoordinate(int row, int col) const
		{
			return (row > 0 && row < m_rows && col > 0 && col < m_cols);
		}
		
	private:
		char** m_board	= nullptr;
		size_t m_rows	= 0;
		size_t m_cols	= 0;
		
		friend class CommonBattleshipGameAlgo;

		Board() {}
		Board(const Board& b) = delete;
		Board& operator=(const Board& b) = delete;
		~Board();

	};

	Board				m_myBoard;
	bool				m_isDone		= false;
	int					m_playerID		= 0;
	std::pair<int, int> m_currentSeqMove;

};


