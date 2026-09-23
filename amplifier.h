#pragma once

#include <thread>
#include <iostream>
#include <string>
#include <mutex>
#include <array>
#include "intcoder.h"
#include "semaphore.h"

constexpr int NUM_THREADS = 5;

class Amplifier {
	private:
		std::string codes[5];

	public:

		Amplifier() {}
		Amplifier(const char* filepath); 
		~Amplifier();

		std::array<Intcoder, NUM_THREADS> intcoders;
		std::array<std::thread, NUM_THREADS> threads;

		std::array<DataSemaphore, NUM_THREADS> sem_used;
		std::array<Semaphore, NUM_THREADS> sem_free;

		std::mutex codeMtx;
		std::mutex signalMtx;

		int signal;
		bool stop = false;

		void intcoderProcess(int coderIndex);
		std::string getCode(int index);
		void setCode(int index, std::string code);

};
