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


	static char** allocateBoard(size_t rows, size_t cols);

	static void	deallocateBoard(char** board, size_t rows);

};



