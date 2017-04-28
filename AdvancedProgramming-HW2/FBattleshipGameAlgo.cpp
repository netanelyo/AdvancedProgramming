#include "FBattleshipGameAlgo.h"

const std::string FBattleshipGameAlgo::ATTACK_SUFFIX = "attack";

void FBattleshipGameAlgo::setBoard(const char** board, int numRows, int numCols)
{
	for (auto i = 0; i < numRows; i++)
		for (auto j = 0; j < numCols; j++)
			setBoardCoord(i, j, board[i][j]);
}

bool FBattleshipGameAlgo::init(const std::string & path)
{
	DIR* dir;
	struct dirent* entry;
	std::set<std::string> filesSet;
	std::string tempFilename;
	auto cnt = 0;

	if ((dir = opendir(path.c_str())))
	{
		while ((entry = readdir(dir)))
		{
			tempFilename = entry->d_name;
			
			if (endsWith(tempFilename, ATTACK_SUFFIX))
				filesSet.insert(tempFilename);
		}

		closedir(dir);

		auto setSize = filesSet.size();

		if (setSize == 0)
			return false;

		auto fileIter = filesSet.begin();
		if (!(setSize == 1 || getPlayerID() == 0))
			++fileIter;
		
		return makeMovesQueue(*fileIter);
	}
	else
		return false;
}

std::pair<int, int> FBattleshipGameAlgo::attack()
{
	std::pair<int, int> attackCoordinate(-1, -1);
	std::string			line;

	/* Reads attacks file until a valid line or EOF have been reached */
	while (std::getline(playerMoves, line))
	{
		if (parseLineAndValidate(line, attackCoordinate))
			break;
	}

	/* If EOF reached then current player is done */
	if (!playerMoves)
	{
		isDone = true;
	}
	
	return attackCoordinate;
}

void FBattleshipGameAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	if (board[row][col] != '0' && board[row][col] != 'X')
	{
		switch (result)
		{
		case AttackResult::Sink:
			shipCounter--;
		case AttackResult::Hit:
			board[row][col] = 'X';
			break;
		default:
			break;
		}
	}
}

bool FBattleshipGameAlgo::makeMovesQueue(const std::string & filePath)
{
	std::string	line;
	std::ifstream movesFin(filePath);
	if (!movesFin.is_open())
	{
		return false;
	}

	while (getline(movesFin, line))
	{
		m_playerMoves.push_front(line);
	}

	return true;
}


