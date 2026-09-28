#include "amplifier.h"

Amplifier::Amplifier(const char* filePath, AmplifierController *ac)
{
	buf[0] = 0;
	buf[1] = 0;
	buf[2] = 0;
	buf[3] = 0;
	buf[4] = 0;
	for (int i = 0; i < 5; i++)
	{
		this->intcoders[i] = Intcoder(filePath);
		this->intcoders[i].loadProgram();
		this->ac = ac;
		this->loopDone = false;
	}
}

void Amplifier::run()
{
	std::mutex mtx[5];
	std::condition_variable cv[5];
	std::thread threads[5];

	int size = ac->codes.size();

	//pop code from queue
	std::string code = ac->readCode();
	setCode(code);

	//start threads
	for (int i = 0; i < 5; i++)
	{
		sem_used[i] = Semaphore(0, &mtx[i], &cv[i]);
		sem_free[i] = Semaphore(1, &mtx[i], &cv[i]);
		threads[i] = std::thread(&Amplifier::intcoderProcess, this, i);
	}

	sem_used[0].give();
	bool done = false;
	while (ac->signals.size() != size)
	{
		if (getCode() == "")
		{
			if (!ac->codes.empty())
			{
				if (loopDone)
				{
					//sem_free[0].take();
					//code = ac->readCode();
					//setCode(code);
					//sem_used[0].give();
				}
			}
			else if (!done)
			{
				sem_free[0].take();
				buf[0] = -1;
				done = true;
				setCode("");
				sem_used[0].give();
			}
		}
	}
	int max = ac->getMaxSignal();
	printf("Max: %i\n", max);
	for (int i = 0; i < 5; i++)
	{
		threads[i].join();	
	}
}

void Amplifier::intcoderProcess(int coderIndex)
{
	while (true)
	{
		//consume input in buffer
		sem_used[coderIndex].take();

		printf("Thread %i working\n", coderIndex);

		//termination condition
		int input = buf[coderIndex];
		if (input == -1)	
		{	
			if (coderIndex < 4)
			{
				sem_free[coderIndex + 1].take();
				buf[coderIndex + 1] = input;
				sem_used[coderIndex + 1].give();
			}
			return;
		}
	
		std::string codeCopy = getCode();
		std::stringstream stream;
		int output;
		bool useCode = codeCopy.size() > 0;
		if (useCode)
		{
			int numCode;
			stream << codeCopy[0];
			stream >> numCode;
			stream.clear();
			stream.str("");
			int inputs[2] = {numCode, input};
		

			output = intcoders[coderIndex].process(inputs, 2);
		}
		else output = intcoders[coderIndex].process(&input, 1);
		if (coderIndex < 4)
		{
			sem_free[coderIndex + 1].take();
			//strip leading digit of code
			if (useCode) setCode(codeCopy.substr(1));
			//write output to next buffer
			printf("Thread %i writing output\n", coderIndex);
			buf[coderIndex + 1] = output;
			
			if (intcoders[coderIndex].done)
			{
				intcoders[coderIndex].clearBuffers();
				intcoders[coderIndex].loadProgram();
			}

			//notify we are ready to receive another input
			sem_used[coderIndex + 1].give();
			sem_free[coderIndex].give();

			printf("Thread %i done, giving back mutex\n", coderIndex);

		}
		else 
		{
			sem_free[0].take();
			//done with code
			if (useCode) setCode("");

			//write output to starting buffer
			if (!intcoders[coderIndex].done)
			{
				printf("Thread %i writing output to starting buffer\n", coderIndex);
				buf[0] = output;

				//notify we are ready to receive another input
				sem_used[0].give();
				sem_free[coderIndex].give();

			}
			else 
			{
				printf("Thread %i writing out signal\n", coderIndex);
				ac->writeSignal(output);
				printf("%i\n", output);
				intcoders[coderIndex].clearBuffers();
				intcoders[coderIndex].loadProgram();
				loopDone = true;
				sem_free[coderIndex].give();
			}
		}
	}
}

std::string Amplifier::getCode()
{
	std::scoped_lock<std::mutex> lock(codeMtx);
	return this->code;
	
}

void Amplifier::setCode(std::string code)
{
	std::scoped_lock<std::mutex> lock(codeMtx);
	this->code = code;
}
