#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Intcoder {
	
	public:
		int r1, r2, pc;
		int *buf;
		int progLen;
		const char *filePath;

		Intcoder(const char* filePath);
		int loadProgram();
		int writeProgram();
		void loadInstruction();
		int operation();
		void process();

};
