#include "ConcurrentVector.h"

void ConcurrentVector::addAndNotify(Statistics val)
{
	{
		auto value = val;
		std::lock_guard<std::mutex> lk(m_writeMutex);
		if (!m_vec.empty()) // Accumulate values
			value += m_vec.back();
		m_vec.push_back(value);
		++m_size;
	}
	m_cv.notify_one();
}

ConcurrentVector::Statistics ConcurrentVector::get(int index)
{
	std::mutex localMutex;
	std::unique_lock<std::mutex> cvLock(localMutex);

	if (m_vec.size() <= index)
		m_cv.wait(cvLock, [this, index] { return m_size > index; });
	
	cvLock.unlock();
	return m_vec[index];
}
