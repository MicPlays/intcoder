#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <vector>


class Intcoder {
	
	public:
		int r1, r2, pc;
		std::vector<int> program;
		int buf[4] = {0, 0, 0, 0};
		int progLen;
		const char *filePath;

		Intcoder(const char* filePath);
		void loadProgram();
		int writeProgram();
		void loadInstruction();
		int operation();
		void process();
		void printData();
		void findInputs(int desiredOutput, int results[2]);
		void clearBuffers();

};
