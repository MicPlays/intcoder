#include "amplifiercontroller.h"

AmplifierController::AmplifierController(const char* filePath)
{
	//this->s = CodeStack();
	//this->q = CodeQueue("01234");
	//generateCodes();

	/*
	this->sm = Semaphore(5);
	for (int i = 0; i < 5; i++)
	{
		this->intcoders[i] = Intcoder();
	}
	*/
	this->intcoders[0] = Intcoder(filePath);
}

void AmplifierController::generateCodes()
{
	int queueSize = q.size();
	for (int i = 0; i < queueSize; i++)
	{
		s.push(q.pop());
		if (q.empty()) s.writeStack(); 
		else generateCodes();
		q.push(s.pop());
	}
}

void AmplifierController::readCodes()
{

}

void AmplifierController::intcoderProcess(int coderIndex, std::string code)
{
	//while(true)
	//{
	//	sm.take();
		int output = 0;
		std::string codeCopy = code;
		std::stringstream stream;
		for (int i = 0; i < 5; i++)
		{
			//reload program into intcoder memory
			intcoders[coderIndex].loadProgram();

			int numCode;
			stream << codeCopy[0];
			stream >> numCode;
			stream.clear();
			stream.str("");
			int inputs[2] = {numCode, output};

			output = intcoders[coderIndex].process(inputs, 2);

			//strip leading digit of code
			codeCopy = codeCopy.substr(1);
			printf("After Iteration: %i\n", i);
			std::cout << "Code: " << codeCopy << std::endl;
			std::cout << "Output: " << output << std::endl;
			
		}
		std::cout << "Input: " << code << ", ";
		std::cout << "Output: " << output << std::endl;

	//}
}

