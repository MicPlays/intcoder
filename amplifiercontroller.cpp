#include "amplifiercontroller.h"

AmplifierController::AmplifierController()
{
	this->s = CodeStack();
	this->q = CodeQueue("01234");
	generateCodes();
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
