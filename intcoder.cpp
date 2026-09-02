#include "intcoder.h"

Intcoder::Intcoder(const char* filePath)
{
	this->filePath = filePath;
	this->program = std::vector<int>(); 
	loadProgram();
	this->pc = 0;
       	this->r1 = 0;
       	this->r2 = 0;
	this->buf[0] = 0;
	this->buf[1] = 0;
	this->buf[2] = 0;
	this->buf[3] = 0;
}

void Intcoder::loadProgram()
{
	std::stringstream pStream;
	pStream << this->filePath << ".intcode";
	std::ifstream programFile(pStream.str());
	pStream.clear();
	pStream.str("");
	std::string str = "";
	if (programFile.is_open())
	{
		while (std::getline(programFile, str, ','))
		{
			pStream << str;
			int num = 0;
			pStream >> num;
			this->program.push_back(num);
			pStream.clear();
			pStream.str("");
		}
		programFile.close();
	}
}

int Intcoder::writeProgram()
{
	std::ofstream outputFile;
	std::stringstream pStream;
	pStream << this->filePath << ".intcodeOBJ";
	try {
		std::string str;
		outputFile.open(pStream.str());
		pStream.clear();
		pStream.str("");
		std::vector<int>::iterator it;
		for (it = program.begin(); it != program.end() - 1; it++ )
		{
			pStream << *it << ",";
			outputFile << pStream.str();
			pStream.clear();
			pStream.str("");
		}
		it = program.end() - 1;
		pStream << *it;
		outputFile << pStream.str();
		pStream.clear();
		pStream.str("");
		return 0;
	} catch (std::ofstream::failure const&){
		printf("Could not write file");
		return -1;
	}
}

void Intcoder::loadInstruction()
{
	std::vector<int>::iterator it = program.begin() + pc;
	for (int i = 0; i < 4; i++)
	{
		if (it != program.end())
		{
			buf[i] = *it;
			it++;
		}
	}
}

int Intcoder::operation()
{
	switch (buf[0])
	{
		case (1): return r1 + r2;
		case (2): return r1 * r2;
		default: return -1;
	}
}


void Intcoder::process()
{
	bool stillProcessing = true;
	while (stillProcessing)
	{
		loadInstruction();
		//if opcode 99 halt
		if (buf[0] == 99) 
		{
			stillProcessing = false;
			continue;
		}
		int l1 = 0;
		int l2 = 0;
		int l3 = 0;
		l1 = buf[1] - pc;
		r1 = program[pc + l1];
		l2 = buf[2] - pc;
		r2 = program[pc + l2];

		int result = operation();
		if (result == -1) 
		{
			printf("Program error: opcode does not match any entries in optable\n");
			exit(0);
		}
		l3 = buf[3] - pc;
		program[pc + l3] = result;
		pc += 4;
	}
}

void Intcoder::printData()
{
	printf("Buffer: ");
	for (int i = 0; i < 3; i++)
		std::cout << buf[i] << ",";
	std::cout << buf[3] << std::endl;
	
	printf("Registers: ");
	printf("R1: %i, R2: %i\n", r1, r2);

	printf("Program: ");
	std::vector<int>::iterator it;
	for (it = program.begin(); it != program.end() - 1; it++)
		std::cout << *it << ",";
	it = program.end() - 1;
	std::cout << *it << std::endl;
}

