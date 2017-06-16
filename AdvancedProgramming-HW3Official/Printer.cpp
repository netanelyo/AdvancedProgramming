#include "Printer.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <windows.h>


Printer::Printer(int rounds, const std::vector<std::string>& names) :
	m_playersResults(names.size()), m_maxWidth(0),
	m_numOfRounds(rounds), m_playersStats(names.size())
{
	size_t max = 0;
	for (size_t i = 0; i < names.size(); i++)
	{
		auto len = names[i].size();
		max = max > len ? max : len;
		m_playersStats[i].first = names[i];
	}
	m_maxWidth = int(max) + 5;
}

void Printer::printRound(const std::vector<std::pair<double, size_t>>& sortedPlayers) const
{
	system("cls");
	std::cout << // First line
		std::setw(MAX_WIDTH_NUM) << std::left << "#" <<
		std::setw(m_maxWidth) << std::left << "Player" <<
		std::setw(MAX_WIDTH_NUM) << std::left << "Wins" <<
		std::setw(MAX_WIDTH_NUM) << std::left << "Losses" <<
		std::setw(m_maxWidth) << std::left << "Win Ratio (%)" <<
		std::setw(m_maxWidth) << std::left << "Pts For" <<
		std::setw(m_maxWidth) << std::left << "Pts Against" << std::endl;

	auto pos = 1;
	for (const auto& playerRatio : sortedPlayers)
	{
		auto i = playerRatio.second;
		const auto& playerInfo = m_playersStats[i];
		const auto& playerStats = playerInfo.second;
		std::cout << std::endl <<
			std::setw(MAX_WIDTH_NUM) << pos++ <<
			std::setw(m_maxWidth) << playerInfo.first <<
			std::setw(MAX_WIDTH_NUM) << playerStats.wins <<
			std::setw(MAX_WIDTH_NUM) << playerStats.losses <<
			std::setw(m_maxWidth) << std::setprecision(3) << playerStats.winRatio() <<
			std::setw(m_maxWidth) << playerStats.pointsFor <<
			std::setw(m_maxWidth) << playerStats.pointsAgainst;
	}
}

void Printer::start()
{
	auto round = 0;
	while (round < m_numOfRounds)
	{
		std::vector<std::pair<double, size_t>> sortedPlayers;
		for (size_t i = 0; i < m_playersStats.size(); i++)
		{
			auto& playerStats = m_playersStats[i].second;
			playerStats = m_playersResults[i].get(round);
			sortedPlayers.push_back({ playerStats.winRatio(), i });
		}
		std::sort(sortedPlayers.begin(), sortedPlayers.end(), comparePairs);
		round++;
		printRound(sortedPlayers);
	}
}

bool Printer::comparePairs(const std::pair<double, size_t>& p1, const std::pair<double, size_t>& p2)
{
	return p1 > p2;
}
