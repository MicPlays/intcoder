intcoder: intcoder.o main.cpp amplifiercontroller.o
	g++ -o intcoder intcoder.o amplifiercontroller.o main.cpp

intcoder.o: intcoder.cpp intcoder.h
	g++ -o intcoder.o -c intcoder.cpp

amplifier.o: amplifier.h intcoder.h semaphore.h
	g++ -o amplifier.o -c amplifier.cpp

amplifiercontroller.o: amplifiercontroller.cpp amplifiercontroller.h amplifier.h
	g++ -o amplifiercontroller.o -c amplifiercontroller.cpp

debug: main.cpp intcoder.h intcoder.cpp amplifiercontroller.cpp amplifier.cpp
	g++ -Wall -o intcoder.o -g -c intcoder.cpp
	g++ -Wall -o amplifier.o -g -c amplifier.cpp
	g++ -Wall -o amplifiercontroller.o -g -c amplifiercontroller.cpp
	g++ -Wall -o intcoder -g intcoder.o amplifiercontroller.o main.cpp 

clean:
	rm -rf intcoder
	rm -rf intcoder.o
	rm -rf amplifiercontroller.o
	rm -rf amplifier.o
	rm -rf *.intcodeOBJ
