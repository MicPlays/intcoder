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
		this->ac = ac;
	}
}

Amplifier::~Amplifier()
{
	for (int i = 0; i < 5; i++)
	{
		threads[i].join();	
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
	setCode(0, code);

	//start threads
	for (int i = 0; i < 5; i++)
	{
		sem_used[i] = Semaphore(0, &mtx[i], &cv[i]);
		sem_free[i] = Semaphore(1, &mtx[i], &cv[i]);
		threads[i] = std::thread(&Amplifier::intcoderProcess, this, i); 
	}

	sem_used[0].give();
	while (ac->signals.size() != size)
	{
		std::cout << ac->signals.size() << std::endl;
		if (code != getCode(0))
		{
			if (!ac->codes.empty())
			{
				sem_free[0].take();
				printf("Reading code...\n");
				code = ac->readCode();
				printf("Setting code...\n");
				setCode(0, code);
				printf("Set input!\n");

				sem_used[0].give();
				sem_free[0].give();
			}
			else 
			{
				sem_free[0].take();
				buf[0] = -1;
				sem_used[0].give();
				sem_free[0].give();
			}
		}
	}
	int max = ac->getMaxSignal();
	printf("Max: %i\n", max);
}

void Amplifier::intcoderProcess(int coderIndex)
{
	while (true)
	{
		//consume input in buffer
		printf("Thread %i waiting to consume\n", coderIndex);
		sem_used[coderIndex].take();

		//notify that we are working
		sem_free[coderIndex].take();
		printf("Thread %i working\n", coderIndex);
	
		//termination condition
		int input = buf[coderIndex];
		if (input == -1)	
		{	
			if (coderIndex < 4)
			{
				sem_free[coderIndex + 1].take();
				buf[coderIndex + 1] = input;
				sem_free[coderIndex + 1].give();
				sem_used[coderIndex + 1].give();
			}
			else int uads = ac->getMaxSignal(); 
			return;
		}
	
		std::string codeCopy = getCode(coderIndex);
		if (coderIndex == 0) setCode(coderIndex, "");

		std::stringstream stream;

		int numCode;
		stream << codeCopy[0];
		stream >> numCode;
		stream.clear();
		stream.str("");
		int inputs[2] = {numCode, input};
	
		intcoders[coderIndex].loadProgram();
		int output = intcoders[coderIndex].process(inputs, 2);

		if (coderIndex < 4)
		{

			sem_free[coderIndex + 1].take();
			//strip leading digit of code
			setCode(coderIndex + 1, codeCopy.substr(1));
			//write output to next buffer
			printf("Thread %i writing output\n", coderIndex);
			buf[coderIndex + 1] = output;
			
			//give buffer back
			sem_free[coderIndex + 1].give();
			sem_used[coderIndex + 1].give();
			printf("Thread %i wrote output!\n", coderIndex);
			//notify we are ready to receive another input
			sem_free[coderIndex].give();

			printf("Thread %i done, giving back mutex\n", coderIndex);

		}
		//for last intcoder we want to write output for now
		else
		{
			ac->writeSignal(output);
			sem_free[coderIndex].give();
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
