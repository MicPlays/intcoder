#include "intcoder.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("usage: ./intcoder 'filepath' (DONT INCLUDE EXTENSION\n");
		exit(0);
	}

	Intcoder ic = Intcoder(argv[1]);
	int results[2] = {0, 0}; 
	ic.findInputs(19690720, results);
	if (results[0] == -1 && results[1] == -1) exit(0);
	ic.writeProgram();

}
