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
	this->program.clear();
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

void Intcoder::clearBuffers()
{
	this->pc = 0;
       	this->r1 = 0;
       	this->r2 = 0;
	this->buf[0] = 0;
	this->buf[1] = 0;
	this->buf[2] = 0;
	this->buf[3] = 0;
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

void Intcoder::findInputs(int desiredOutput, int results[2])
{
	//load initial inputs into memory
	int input1 = 0;
	int input2 = 0;
	bool stillProcessing = true;
	bool success = false;
	while (stillProcessing)
	{
		printf("Current Inputs: %i, %i\n", input1, input2);
		process();
		printf("Output: %i\n", program[0]);
		//if output is not correct, adjust inputs, reload memory and try again
		if (program[0] != desiredOutput)
		{
			//inputs are between 0 and 99 so if we exceed those limits we have failed
			if (input1 >= 99 && input2 >= 99)
			{
				stillProcessing = false;
				success = false;
			}
			if (input2 >= 99)
			{
				input2 = -1;
				input1++;
			}
			input2++;
			loadProgram();
			clearBuffers();
			program[1] = input1;
			program[2] = input2;
		}
		else
		{
			stillProcessing = false;
			success = true;
		}
	}
	if (success)
	{
		printf("Correct inputs found!\n");
		printf("Input 1: %i, Input 2: %i\n", program[1], program[2]);
		results[0] = program[1]; 
		results[1] = program[2];
	}
	else 
	{
		printf("Could not find correct inputs :(\n");
		results[0] = -1;
		results[1] = -1;
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
		//values 2, 3, and 4 in buffer are memory locations
		//get input values through program counter
		l1 = buf[1] - pc;
		r1 = program[pc + l1];
		l2 = buf[2] - pc;
		r2 = program[pc + l2];

		//do operation
		int result = operation();
		if (result == -1) 
		{
			printf("Program error: opcode does not match any entries in optable\n");
			exit(0);
		}
		//store result by getting memory location and writing
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

