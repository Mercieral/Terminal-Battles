CFLAGS= -g -Wall -Werror

all: client

client: client.cpp game_piece.cpp game_piece.hpp game_board.cpp game_board.hpp
	$(CXX) $(CFLAGS) -o game_piece.o -c game_piece.cpp
	$(CXX) $(CFLAGS) -o game_board.o -c game_board.cpp
	$(CXX) $(CFLAGS) -o client.o -c client.cpp
	$(CXX) $(CFLAGS) -o battleship game_piece.o game_board.o client.o -lcurses
	rm -f *.o

clean:
	rm -f *.o battleship
