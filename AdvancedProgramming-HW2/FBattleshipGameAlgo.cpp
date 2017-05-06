#include "FBattleshipGameAlgo.h"

const std::string FBattleshipGameAlgo::ATTACK_SUFFIX = "attack";

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
			
			if (BattleshipUtils::endsWith(tempFilename, ATTACK_SUFFIX))
				filesSet.insert(tempFilename);
		}

		closedir(dir);

		auto setSize = filesSet.size();

		if (setSize == 0)
			return false;

		auto fileIter = filesSet.begin();
		if (!(setSize == 1 || getPlayerID() == 0))
			++fileIter;
		
		return makeMovesQueue(path + (*fileIter));
	}
	else
		return false;
}

std::pair<int, int> FBattleshipGameAlgo::attack()
{
	std::pair<int, int> attackCoordinate(-1, -1);
	std::string			line;

	/* Reads attacks until a valid line or no moves left */
	while (!(m_playerMoves.empty()))
	{
		line = m_playerMoves.front();
		m_playerMoves.pop_front();
		if (BattleshipUtils::parseLineAndValidate(line, attackCoordinate))
			break;
	}

	/* If no moves left then current player is done */
	if (m_playerMoves.empty())
	{
		m_isDone = true;
	}
	
	return attackCoordinate;
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
		m_playerMoves.push_back(line);
	}

	movesFin.close();

	return true;
}

void FBattleshipGameAlgo::setBoard(int player, const char ** board, int numRows, int numCols)
{
	m_playerID = player;
	m_myBoard.setMembers(board, numRows, numCols);
}

IBattleshipGameAlgo* GetAlgorithm()
{
	return new FBattleshipGameAlgo();
}