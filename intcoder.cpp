#include "intcoder.h"
#include <bitset>

Intcoder::Intcoder(const char* filePath)
{
	this->filePath = filePath;
	this->program = std::vector<int>(); 
	loadProgram();
	this->pc = 0;
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


void Intcoder::operation(int opcode, int params, int mask)
{
	//bitmask value
	int j = 1;
	//program index is 1 higher than i because of opcode
	int progIndex = pc + 1;
	for (int i = 0; i < params; i++)
	{
		//immediate mode
		if (mask & j) buf[i] = program[progIndex + i];
		//position mode
		else buf[i] = program[program[progIndex + i]];	
		j *= 2;
	}
	switch (opcode)
	{
		case 1:
		{
			int result = buf[0] + buf[1];
			buf[2] = program[pc + (params+1)];
			program[buf[2]] = result;
			pc += 4;
			break;
		}
		case 2:
		{
			int result = buf[0] * buf[1];
			buf[2] = program[pc + (params+1)];
			program[buf[2]] = result;
			pc += 4;
			break;
		}
		case 3:
		{
			int input = 0;
			printf("Program input: ");
			std::cin >> input;
			program[program[pc + 1]] = input;
			pc += 2;
			break;
		}
		case 4:
		{
			std::cout << buf[0] << std::endl;
			pc += 2;
			break;
		}
		default:
		{
			printf("Error\n");
			exit(0);
		}

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
		if (program[pc] == 99)
		{
			stillProcessing = false;
			continue;
		}
		if (program[pc] > 99) 
		{
			int opcode = getOpcode(program[pc]);
			int params = getOpcodeParams(opcode);
			int mask = getParamMask(program[pc], opcode, params);
			//printData(opcode, params, mask);
			operation(opcode, params, mask);
		}
		else
		{
			int opcode = program[pc];
			int params = getOpcodeParams(opcode);
			//printData(opcode, params, 0);
			operation(opcode, params, 0);
		}
	}
}

int Intcoder::getOpcode(int inst)
{
	return inst % 100;
}

int Intcoder::getOpcodeParams(int opcode)
{
	switch (opcode)
	{
		case 1: return 2;
		case 2: return 2;
		case 3: return 0;
		case 4: return 1;
		default: return -1;
	}
}

int Intcoder::getParamMask(int inst, int opcode, int params)
{
	//extract parameter bits as string
	if (params == 0) return 0;
	int paramNum = (inst - opcode) /100;
	std::string paramStr = std::to_string(paramNum);
	int leadingZeroes = params - paramStr.size();
	std::stringstream stream;
	if (leadingZeroes > 0)
	{
		for (int i = 0; i < leadingZeroes; i++)
			stream << "0";
	}
	stream << paramStr;
	
	//parse and bitmask into integer
	std::string maskStr = stream.str();
	int mask = 0;
	int j = 0;
	stream.clear();
	stream.str("");
	for (int i = maskStr.size() - 1; i > -1; i--)
	{
	      	int bit = 0;
		stream << maskStr[i];
		stream >> bit;
		mask += bit << j;
		j++;
		stream.clear();
		stream.str("");		
	}
	return mask;
}

void Intcoder::printData(int opcode, int params, int mask)
{
	printf("Opcode: %i\n", opcode);
	printf("# of Params: %i\n", params);
	std::cout << "Mask: " << std::bitset<4>(mask) << std::endl;

	printf("Program: ");
	std::vector<int>::iterator it;
	for (it = program.begin(); it != program.end() - 1; it++)
		std::cout << *it << ",";
	it = program.end() - 1;
	std::cout << *it << std::endl;
}

