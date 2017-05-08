#include "FBattleshipGameAlgo.h"
#include <fstream>
#include <cctype>
#include <windows.h>


const std::string FBattleshipGameAlgo::ATTACK_SUFFIX = "attack";

bool FBattleshipGameAlgo::init(const std::string & path)
{
	HANDLE dir;
	WIN32_FIND_DATAA fileData;
	std::set<std::string> filesSet;
	std::string tempFilename;

	tempFilename = "\\*.attack";
	dir = FindFirstFileA((path + tempFilename).c_str(), &fileData);

	if (dir == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	do
	{
		tempFilename = fileData.cFileName;
		filesSet.insert(path + tempFilename);
	} while (FindNextFileA(dir, &fileData));

	auto setSize = filesSet.size();

	if (setSize == 0)
		return false;

	auto fileIter = filesSet.begin();
	if (!(setSize == 1 || getPlayerID() == 0))
		++fileIter;
		
	return makeMovesQueue(path + (*fileIter));
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
		if (parseLineAndValidate(line, attackCoordinate))
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

bool FBattleshipGameAlgo::parseLineAndValidate(const std::string& str, std::pair<int, int>& coord) const
{
	std::string				tempArr[2];		/* To hold potential strings representing row/col */
	std::string::size_type	loc;
	int						num;
	int						cnt;

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
		catch (std::invalid_argument exInv)
		{
			return false;
		}
		catch (std::out_of_range exOut)
		{
			return false;
		}

		if (cnt)
			coord.second = num;
		else
			coord.first = num;

		cnt++;
	}

	return true;
}
