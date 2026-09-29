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

class AmplifierController;

class Amplifier {
	private:
		std::string code;

	public:

		Amplifier() {}
		Amplifier(const char* filepath, AmplifierController *ac, std::mutex *mtx); 

		Intcoder intcoders[5];

		Semaphore sem_used[5];
		Semaphore sem_free[5];

		std::thread threads[5];

		int buf[5];

		std::mutex* codeMtx;
		
		AmplifierController *ac;		

		bool loopDone;

		void run();
		void intcoderProcess(int coderIndex);
		std::string getCode();
		void setCode(std::string code);

};
