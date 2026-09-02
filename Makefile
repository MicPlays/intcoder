intcoder: intcoder.o main.cpp
	g++ -o intcoder intcoder.o main.cpp

intcoder.o: intcoder.cpp intcoder.h
	g++ -o intcoder.o -c intcoder.cpp

debug: main.cpp intcoder.h intcoder.cpp
	g++ -Wall -o intcoder.o -g -c intcoder.cpp
	g++ -Wall -o intcoder -g intcoder.o main.cpp 

clean:
	rm -rf intcoder
	rm -rf intcoder.o
	rm -rf *.intcodeOBJ
