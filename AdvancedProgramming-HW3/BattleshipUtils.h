#pragma once



enum class Direction
{
	NON,
	VERTICAL,
	HORIZONTAL,
};

class BattleshipUtils
{
public:

	/**
	* allocates a new board 
	*
	* @param rows - the board's number of rows
	* @param cols - the board's number of cols
	* @return 
	* a new board 
	*/
	static char** allocateBoard(int rows, int cols);

	/**
	* deletes the given board
	* 
	* @param board - the board to be deleted
	* @param rows - the board's number of rows
	*/
	static void	deallocateBoard(char** board, int rows);

};



