#pragma once

#include <mutex>
#include <condition_variable>

class Semaphore
{
	private:
		int m_count;
		std::mutex mtx;
		std::condition_variable cv;

	public:
		explicit Semaphore(int initialCount) {m_count = initialCount;}
		void take()
		{
			std::unique_lock<std::mutex> lock(mtx);
			cv.wait(lock, [this] {return m_count > 0; });
			m_count -= 1;
			
		}

		void give()
		{
			std::unique_lock<std::mutex> lock(mtx);
			m_count += 1;
			cv.notify_one();
		}

};
