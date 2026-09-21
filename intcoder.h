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

		//handle automated inputs
		int *inputBuf;
		int inputBufSize;
		int inputBufCount;

		int output;

		std::vector<int> program;
		int progLen;
		const char *filePath;

		Intcoder() {}
		Intcoder(const char* filePath);
		void loadProgram();
		int writeProgram();
		void clearBuffers();
		void operation(int opcode, int params, int mask);
		int process(int *inputs, int size);
		void printData(int opcode, int params, int mask);
		//void findInputs(int desiredOutput, int results[2]);
		int getOpcodeParams(int opcode);
		int getOpcode(int inst);
		int getParamMask(int inst, int opcode, int params);	

};
