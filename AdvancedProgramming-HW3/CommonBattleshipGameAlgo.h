#pragma once
#include "IBattleshipGameAlgo.h"
#include "BattleshipUtils.h"


class CommonBattleshipGameAlgo : public IBattleshipGameAlgo
{
public:
	CommonBattleshipGameAlgo() : m_isDone(false), m_playerID(-1), m_currentSeqMove(0, -1) {}

	/**
	* @implements IBattleshipGameAlgo::setBoard pure virtual function
	*
	* Copies board[i][j] to this->board[i][j] 
	* and sets all the squares bordering our ships as "#" 
	*
	* @param player - the player's index as given by game manager
	* @param board - the board to copy 
	* @param numRows - board's number of rows
	* @param numCols - board's number of columns
	*/
	virtual void setBoard(int player, const char** board, int numRows, int numCols) override;

	/**
	* @implements IBattleshipGameAlgo::notifyOnAttackResult pure virtual function
	*
	* updates board if the attack result was a hit/sink of our ship
	*
	* @param player - an integer represting the current player
	* @param row - the row coordinate of the attack
	* @param col - the column coordinate of the attck
	* @param result - the attack result
	*
	*/
	virtual void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;

	/**
	* @implements IBattleshipGameAlgo::init pure virtual function
	* 
	* @param str - a directory path
	* @return true
	* 
	*/
	virtual bool init(const std::string& str) override { return true; }

	/**
	* @implements IBattleshipGameAlgo::attack pure virtual function
	* a naive implementation: 
	* finds the sequent empty square (which is not our ship or a bordering a square of our ship) 
	* and returns its coordinates as the next attack
	* 
	* @return 
	* <-1, -1> in case we reached the end of the board 
	* otherwise, the attack's coordinates within the board
	*/
	virtual std::pair<int, int> attack() override;

	bool	isDone() const { return m_isDone; } //returns true if the player has no more moves, and false otherwise
	int		getPlayerID() const { return m_playerID; } //returns player's Id number 

protected:
	/* a class represnting a 2D board of characters*/
	class Board
	{
	public:
		/**
		* sets the object's members
		* 
		* @param board - the board to be copied 
		* @param rows - the board's number of rows
		* @param cols - the board's number of columns
		*/
		void setMembers(const char** board, int rows, int cols);

		/**
		* sets board[i][j] to val
		* 
		* @param i - the row coordainte
		* @param j - the column coordinate
		* @param val - the value to be inserted
		*/
		void setBoardSquare(int i, int j, char val) const { m_board[i][j] = val; }

		char getBoardSquare(int i, int j) const { return m_board[i][j]; } //returns board[i][j]
		int getBoardRows() const { return m_rows; } //returns the board's number of rows
		int getBoardCols() const { return m_cols; } //returns the board's number of columns

		/**
		* searches for the next empty square in board
		*
		* @param pair - the board coordinate from which we need to start looking for the next empty square
		* @return 
		* <-1, -1> if there is no empty square following the given coordinate 
		* otherwise, the coordinates of the next empty square on board 
		*/
		std::pair<int, int> getNextEmptySquare(const std::pair<int, int>& pair) const;

		/**
		* checks the coordinates are valid board coordinates
		* @param row - the row coordinate
		* @param col - the column coordinate
		* @return 
		* false if (row, col) aren't valid board coordinates
		* otherwise, true
		*/
		bool validateCoordinate(int row, int col) const
		{
			return (row >= 0 && row < m_rows && col >= 0 && col < m_cols);
		}
		
	private:
		char** m_board	= nullptr;
		int m_rows	= 0;
		int m_cols	= 0;
		
		friend class CommonBattleshipGameAlgo;

		Board() {}
		Board(const Board& b) = delete;
		Board& operator=(const Board& b) = delete;
		~Board();

	};

	Board				m_myBoard;
	bool				m_isDone;
	int					m_playerID;
	std::pair<int, int> m_currentSeqMove;

};


