CFLAGS= -g -Wall -Werror

all: client

client: client.cpp gameboard.cpp gameboard.hpp
	$(CXX) $(CFLAGS) -o gameboard.o -c gameboard.cpp -lcurses
	$(CXX) $(CFLAGS) -o client.o -c client.cpp -lcurses
	$(CXX) $(CFLAGS) -o client gameboard.o client.o -lcurses

clean:
	rm -f *.o client
