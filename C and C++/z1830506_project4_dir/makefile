CXXFLAGS = -g -ansi -pedantic -lpthread -Wall -Wextra -Werror -std=c++14

z1830506_project4: assign4.o
	g++ $(CXXFLAGS) -o z1830506_project4 assign4.o

assign4.o: assign4.cpp
	g++ $(CXXFLAGS) -c assign4.cpp

clean:
	rm -f *.o z1830506_project4