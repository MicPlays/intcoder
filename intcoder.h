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
		int *buf;
		int progLen;
		const char *filePath;

		Intcoder(const char* filePath);
		void loadProgram();
		int writeProgram();
		void loadInstruction();
		int operation();
		void process();

};
