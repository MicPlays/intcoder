#include "intcoder.h"

Intcoder::Intcoder(const char* filePath)
{
	this->filePath = filePath;
	this->program = std::vector<int>(); 
	loadProgram();
	writeProgram();	
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
			program.push_back(num);
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
		for (std::vector<int>::iterator it = program.begin(); it != program.end(); it++ )
		{
			pStream << *it;
			outputFile << pStream.str();
			pStream.clear();
			pStream.str("");
		}
		return 0;
	} catch (std::ofstream::failure const&){
		printf("Could not write file");
		return -1;
	}
}

void Intcoder::loadInstruction()
{

}

int Intcoder::operation()
{
	
}


void Intcoder::process()
{

}

