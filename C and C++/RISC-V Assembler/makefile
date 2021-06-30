#
#
#
#

CXXFLAGS = -g -ansi -pedantic -Wall -Wextra -Werror -std=c++14

rv32i: main.o rv32i.o memory.o hex.o registerfile.o
	g++ $(CXXFLAGS) -o rv32i main.o rv32i.o memory.o hex.o registerfile.o

main.o: main.cpp
	g++ $(CXXFLAGS) -c main.cpp

rv32i.o: rv32i.cpp rv32i.h
	g++ $(CXXFLAGS) -c rv32i.cpp

memory.o: memory.cpp memory.h
	g++ $(CXXFLAGS) -c memory.cpp

hex.o: hex.cpp hex.h
	g++ $(CXXFLAGS) -c hex.cpp

registerfile.o: registerfile.cpp registerfile.h
	g++ $(CXXFLAGS) -c registerfile.cpp

clean:
	rm -f *.o rv32i