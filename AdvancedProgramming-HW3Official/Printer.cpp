#include "Printer.h"
#include <iostream>
#include <iomanip>
#include <string>


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
	m_maxWidth = int(max) + 12;
}

void Printer::printRound() const
{
	system("cls");
	std::cout << // First line
		std::setw(m_maxWidth) << std::left << "#" <<
		std::setw(m_maxWidth) << std::left << "Player" <<
		std::setw(m_maxWidth) << std::left << "Wins" <<
		std::setw(m_maxWidth) << std::left << "Losses" <<
		std::setw(m_maxWidth) << std::left << "Win Ratio (%)" <<
		std::setw(m_maxWidth) << std::left << "Pts For" <<
		std::setw(m_maxWidth) << std::left << "Pts Against" << std::endl;

	for (size_t i = 0; i < m_playersStats.size(); i++)
	{
		const auto& playerInfo = m_playersStats[i];
		const auto& playerStats = playerInfo.second;
		std::cout << std::endl <<
			std::setw(m_maxWidth) << i <<
			std::setw(m_maxWidth) << playerInfo.first <<
			std::setw(m_maxWidth) << playerStats.wins <<
			std::setw(m_maxWidth) << playerStats.losses <<
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
		for (size_t i = 0; i < m_playersStats.size(); i++)
		{
			m_playersStats[i].second = m_playersResults[i].get(round);
		}
		round++;
		printRound();
	}
}