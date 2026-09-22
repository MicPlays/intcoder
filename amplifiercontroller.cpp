#include "amplifiercontroller.h"

AmplifierController::AmplifierController(const char* filePath)
{
	this->s = CodeStack();
	this->q = CodeQueue("01234");
	generateCodes();

	for (int i = 0; i < 5; i++)
	{
		this->intcoders[i] = Intcoder(filePath);
		this->threads[i] = std::thread(&AmplifierController::intcoderProcess, this, i);
		this->threads[i].join();
	}
	while (!codes.empty()){}
	int max = getMaxSignal();
	printf("Max: %i\n", max);
	
}

void AmplifierController::generateCodes()
{
	int queueSize = q.size();
	for (int i = 0; i < queueSize; i++)
	{
		s.push(q.pop());
		if (q.empty())
		{
			std::string code = s.writeStack(); 
			this->codes.push(code);
		}
		else generateCodes();
		q.push(s.pop());
	}
}

void AmplifierController::intcoderProcess(int coderIndex)
{
	while(!codes.empty())
	{
		//attempt acquire read lock (blocks if not available)
		std::string code = readCode();
				
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
			
		}
	//	std::cout << "Input: " << code << ", ";
	//	std::cout << "Output: " << output << std::endl;

		//attempt acquire write lock (blocks if not available)
		writeSignal(output);
	}
}

int AmplifierController::getMaxSignal()
{
	int max = signals[0];
	for (int i = 0; i < signals.size(); i++)
	{
		if (signals[i] > max)
			max = signals[i];
	}
	return max;
}

std::string AmplifierController::readCode()
{
	std::string code = "";
	{
		//take lock
		std::scoped_lock<std::mutex> lock(readMtx);

		//pop from queue
		code = codes.front();
		codes.pop();
	}
	//lock released out of scope (RAII)
	return code;
}

void AmplifierController::writeSignal(int signal)
{
	//take lock
	std::scoped_lock<std::mutex> lock(writeMtx);
	signals.emplace_back(signal);
	//lock released out of scope (RAII)
}

