CFLAGS= -g -Wall -Werror

all: client

client: client.cpp gameboard.cpp gameboard.hpp
	$(CXX) $(CFLAGS) -o gameboard.o -c gameboard.cpp
	$(CXX) $(CFLAGS) -o client.o -c client.cpp
	$(CXX) $(CFLAGS) -o client gameboard.o client.o

clean:
	rm -f *.o client
