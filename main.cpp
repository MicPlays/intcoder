#include "intcoder.h"
#include "amplifier.h"
#include "amplifiercontroller.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("usage: ./intcoder 'filepath' (DONT INCLUDE EXTENSION\n");
		exit(0);
	}
	AmplifierController ac = AmplifierController();
	Amplifier a(argv[1], &ac);
	std::thread t1(&Amplifier::run, &a);
	t1.join();

}
