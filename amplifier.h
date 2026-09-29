#pragma once

#include <thread>
#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "intcoder.h"
#include "semaphore.h"
#include "amplifiercontroller.h"

class Amplifier {
	private:
		std::string codes[5];

	public:

		Amplifier() {}
		Amplifier(const char* filepath, AmplifierController *ac); 

		Intcoder intcoders[5];

		Semaphore sem_used[5];
		Semaphore sem_free[5];

		std::thread threads[5];

		int buf[5];

		std::mutex codeMtx;
		
		AmplifierController *ac;		

		void run();
		void intcoderProcess(int coderIndex);
		std::string getCode(int index);
		void setCode(int index, std::string code);

};
