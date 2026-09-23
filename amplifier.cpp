#include "amplifier.h"

Amplifier::Amplifier(const char* filePath)
{
	//init intcoders, semaphores, and threads
	for (int i = 0; i < 5; i++)
	{	
		this->signal = 0;
		this->intcoders[i] = Intcoder(filePath);
		//no producers ready
		this->sem_used[i] = DataSemaphore(0);
		//all consumers ready
		this->sem_free[i] = Semaphore(1);
		this->threads[i] = std::thread(&Amplifier::intcoderProcess, this, i);
	}
}

Amplifier::~Amplifier()
{
	for (int i = 0; i < 5; i++)
	{
		stop = true;
		sem_used[i].kill();
		sem_free[i].kill();
		threads[i].join();
	}
}

void Amplifier::intcoderProcess(int coderIndex)
{
	while (true)
	{
		int input = sem_used[coderIndex].take();
		if (stop) return;
		sem_free[coderIndex].take();
		if (stop) return;

		std::string codeCopy = getCode(coderIndex);
		std::stringstream stream;

		int numCode;
		stream << codeCopy[0];
		stream >> numCode;
		stream.clear();
		stream.str("");
		int inputs[2] = {numCode, input};

		int output = intcoders[coderIndex].process(inputs, 2);

		if (coderIndex < 4)
		{
			//strip leading digit of code
			setCode(coderIndex + 1, codeCopy.substr(1));
	
			sem_used[coderIndex + 1].give(output);
			if (stop) return;
			sem_free[coderIndex].give();
			if (stop) return;
		}
		//for last intcoder we want to write output
		else
		{
			{
				std::scoped_lock<std::mutex> lock(signalMtx);
				signal = output;
			}
			sem_free[coderIndex].give();
			if (stop) return;
		}
	}
}

std::string Amplifier::getCode(int index)
{
	std::string code;
	{
		std::scoped_lock<std::mutex> lock(codeMtx);
		code = codes[index];
	}
	return code;
	
}

void Amplifier::setCode(int index, std::string code)
{
	std::scoped_lock<std::mutex> lock(codeMtx);
	codes[index] = code;	
}
