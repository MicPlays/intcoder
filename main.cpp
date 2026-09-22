#include "intcoder.h"
#include "amplifiercontroller.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("usage: ./intcoder 'filepath' (DONT INCLUDE EXTENSION\n");
		exit(0);
	}
	AmplifierController ac = AmplifierController(argv[1]);
	//Intcoder ic = Intcoder(argv[1]);
	//ic.process();
	//ic.writeProgram();

}
