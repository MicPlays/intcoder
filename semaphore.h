#pragma once

#include <mutex>
#include <condition_variable>
#include <string>

class Semaphore
{
	private:
		int m_count;
		std::mutex mtx;
		std::condition_variable cv;

	public:
		Semaphore() {}
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

		void kill()
		{
			cv.notify_all();
		}

};

class DataSemaphore
{
	private:
		int m_count;
		std::mutex mtx;
		std::condition_variable cv;
		int buffer;

	public:
		DataSemaphore() {}
		explicit DataSemaphore(int initialCount) {m_count = initialCount;}

		int take()
		{
			std::unique_lock<std::mutex> lock(mtx);
			cv.wait(lock, [this] {return m_count > 0; });
			m_count -= 1;
			return buffer;
		}

		void give(int value)
		{
			std::unique_lock<std::mutex> lock(mtx);
			m_count += 1;
			buffer = value;
			cv.notify_one();
		}
		void kill()
		{
			cv.notify_all();
		}

};
