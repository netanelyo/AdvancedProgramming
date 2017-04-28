#include "BattleshipUtils.h"

bool BattleshipUtils::parseLineAndValidate(const std::string& str, std::pair<int, int>& coord)
{
	std::string				tempArr[2];		/* To hold potential strings representing row/col */
	std::string::size_type	loc;
	int						num;
	int						cnt	= 0;

	/* Splitting str by ',' */
	loc = str.find_first_of(',');
	tempArr[0] = str.substr(0, loc);

	if (loc < str.length() - 1)
		tempArr[1] = str.substr(loc + 1);
	else
		return false;

	cnt = 0;
	/* Checks if line is valid */
	for (const auto& s : tempArr)
	{
		try
		{
			num = std::stoi(s);
		}
		catch (const std::invalid_argument& exInv)
		{
			return false;
		}
		catch (const std::out_of_range& exOut)
		{
			return false;
		}

		if (cnt)
			coord.second = num;
		else
			coord.first	 = num;

		cnt++;
	}

	return true;
}

bool BattleshipUtils::endsWith(const std::string& filename, const std::string& suffix)
{
	auto extIndex = filename.find_last_of('.'); /* Index of extension in substring */
	auto len = filename.length();

	if (extIndex >= len)
		return false;

	return (!filename.compare(extIndex + 1, len, suffix));
}

char ** BattleshipUtils::allocateBoard(int rows, int cols)
{
	char** board = new char*[rows];

	for (auto i = 0; i < rows; i++)
	{
		board[i] = new char[cols];
	}
	return board;
}

void BattleshipUtils::deallocateBoard(char ** board, int rows)
{
	for (auto i = 0; i < rows; i++)
	{
		delete[] board[i];
	}

	delete[] board;
}
