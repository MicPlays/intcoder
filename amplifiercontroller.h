#pragma once

#include <thread>
#include <iostream>
#include <condition_variable>
#include <stack>
#include <queue>
#include <string>
#include "semaphore.h"
#include "intcoder.h"

class CodeStack {
	private:
		char stack[5];
	public:
		int stackPointer;
		void push(char value)
		{
			if (stackPointer < 5)
			{
				stack[stackPointer] = value;
				stackPointer++;
			}
			else
			{
				printf("Exceeded stack size.\n");
				exit(0);
			}
		}
		char pop()
		{
			if (stackPointer > 0)
			{
				char val = stack[stackPointer - 1];
				stackPointer--;
				return val;
			}
			else 
			{
				printf("Exceeded stack lower limit.\n");
				exit(0);
			}
		}
		void writeStack()
		{
			for (int i = 0; i < stackPointer + 1; i++)
				printf("%c", stack[i]);
			printf("\n");
		}
		CodeStack() {this->stackPointer = 0;}
};

class CodeQueue {
	private:
		std::queue<char> queue;
	public:
		void push(char val) {queue.push(val);}
		char pop() 
		{
			char val = queue.front();
			queue.pop();
			return val;
		}
		CodeQueue() {}
		CodeQueue(std::string pool) {initQueue(pool);}
		void initQueue(std::string pool)
		{
			for (int i = 0; i < pool.size(); i++)
				queue.push(pool[i]);
		}
		int size() {return queue.size();}
		bool empty() {return queue.empty();}
};


class AmplifierController {

	public:
		CodeStack s;
		CodeQueue q;
		void generateCodes();
		void readCodes();

		//Semaphore sm;
		std::thread threads[5];
		Intcoder intcoders[5];
		void intcoderProcess(int coderIndex, std::string code);

		AmplifierController(const char* filePath);		
};
