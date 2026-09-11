#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <vector>

class Intcoder {
	
	public:
		int pc;
		int buf[4] = {0,0,0,0};
		std::vector<int> program;
		int progLen;
		const char *filePath;

		Intcoder(const char* filePath);
		void loadProgram();
		int writeProgram();
		void operation(int opcode, int params, int mask);
		void process();
		void printData(int opcode, int params, int mask);
		void findInputs(int desiredOutput, int results[2]);
		int getOpcodeParams(int opcode);
		int getOpcode(int inst);
		int getParamMask(int inst, int opcode, int params);	

};
