#include "amplifiercontroller.h"

AmplifierController::AmplifierController(const char* filePath)
{
	this->s = CodeStack();
	this->q = CodeQueue("01234");
	generateCodes();

	for (int i = 0; i < 5; i++)
	{
		this->amps[i] = Amplifier(filePath);
		this->threads[i] = std::thread(&AmplifierController::amplifierProcess, this, i);
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

void AmplifierController::amplifierProcess(int ampIndex)
{
	while(!codes.empty())
	{
		if (amps[i].signal != 0)
		{
			writeSignal(amps[ampIndex].signal);
			amps[ampIndex].signal = 0;
		}

		//for first unit test, just keep feeding codes as long as first intcoder is available and we have codes to process
		std::string code = readCode();
		amps[ampIndex].setCode(0, code);
		amps[ampIndex].sem_used[0].give(0);
		
		std::scoped_lock<std::mutex> lock(amps[ampIndex].signalMtx);
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

