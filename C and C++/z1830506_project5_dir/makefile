CXXFLAGS = -g -ansi -pedantic -Wall -Wextra -Werror -std=c++14

z1830506_project5: assign5.o
	g++ $(CXXFLAGS) -o z1830506_project5 assign5.o

assign5.o: assign5.cpp pageTableEntry.h physicalMemory.h
	g++ $(CXXFLAGS) -c assign5.cpp

clean:
	rm -f *.o z1830506_project5