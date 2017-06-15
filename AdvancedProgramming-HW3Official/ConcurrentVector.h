#pragma once
#include <vector>
#include <mutex>
#include <atomic>

class ConcurrentVector
{
public:
	struct Statistics
	{
		int pointsFor = 0;
		int pointsAgainst = 0;
		int games = 0;
		int wins = 0;
		int losses = 0;

		Statistics(int ptsFor, int ptsAg, int gms, int wns, int loss) : 
				pointsFor(ptsFor), pointsAgainst(ptsAg), games(gms), wins(wns), losses(loss) {}

		Statistics() {}
		
		Statistics& operator+=(const Statistics& other)
		{
			pointsFor += other.pointsFor;
			pointsAgainst += other.pointsAgainst;
			games += other.games;
			wins += other.wins;
			losses += other.losses;
			return *this;
		}

		double winRatio() const
		{
			return 100 * (double(wins) / games);
		}
	};
	
	void addAndNotify(Statistics val);	// Thread safe
	Statistics get(int index);

private:
	std::vector<Statistics> m_vec = {};
	std::condition_variable m_cv;
	std::mutex m_writeMutex;
	std::atomic<int> m_size;
};
