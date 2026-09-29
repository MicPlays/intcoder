#include "amplifiercontroller.h"

AmplifierController::AmplifierController()
{
	this->s = CodeStack();
	this->q = CodeQueue("56789");
	generateCodes();
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

