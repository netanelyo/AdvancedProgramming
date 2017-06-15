#pragma once
#include "ConcurrentVector.h"

class Printer
{
public:
	std::vector<ConcurrentVector> m_playersResults;
	
	Printer(int rounds, const std::vector<std::string>& names);
	void printRound() const;
	void start();

private:
	int m_maxWidth;
	int m_numOfRounds;
	std::vector<std::pair<std::string, ConcurrentVector::Statistics>> m_playersStats;
};
