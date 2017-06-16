#pragma once
#include "ConcurrentVector.h"

class Printer
{
public:
	std::vector<ConcurrentVector> m_playersResults;
	
	Printer(int rounds, const std::vector<std::string>& names);
	void printRound(const std::vector<std::pair<double, size_t>>& sorted) const;
	void start();

private:
	int m_maxWidth;
	int m_numOfRounds;
	std::vector<std::pair<std::string, ConcurrentVector::Statistics>> m_playersStats;

	static bool comparePairs(const std::pair<double, size_t>& p1, const std::pair<double, size_t>& p2);

	static const int MAX_WIDTH_NUM = 10;
};
