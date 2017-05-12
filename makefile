CFLAGS= -g -Wall -Werror -std=c++11

all: build_object_files build_executable remove_object_files

debug: build_object_files build_executable

build_object_files: main.cpp game/game_piece.cpp game/game_piece.hpp game/game_board.cpp game/game_board.hpp user/user.hpp user/user.cpp user/host.cpp user/client.cpp
	$(CXX) $(CFLAGS) -o user.o -c user/user.cpp
	$(CXX) $(CFLAGS) -o host.o -c user/host.cpp
	$(CXX) $(CFLAGS) -o client.o -c user/client.cpp
	$(CXX) $(CFLAGS) -o game_piece.o -c game/game_piece.cpp
	$(CXX) $(CFLAGS) -o game_board.o -c game/game_board.cpp
	$(CXX) $(CFLAGS) -o main.o -c main.cpp

build_executable:
	$(CXX) $(CFLAGS) -o battleship main.o game_board.o game_piece.o client.o host.o user.o -lcurses

remove_object_files:
	rm -f *.o

clean:
	rm -f *.o battleship client
