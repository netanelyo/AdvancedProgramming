#pragma once
#include <vector>
#include <mutex>
#include <atomic>

class ConcurrentVector
{
public:
	struct Statistics
	{
		int pointsFor;
		int pointsAgainst;
		int games;
		int wins;
		int losses;
		
		Statistics& operator+=(const Statistics& other)
		{
			pointsFor += other.pointsFor;
			pointsAgainst += other.pointsAgainst;
			games += other.games;
			wins += other.wins;
			losses += other.losses;
			return *this;
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
