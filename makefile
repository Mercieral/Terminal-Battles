CFLAGS= -g -Wall -Werror

all: build_object_files build_executable remove_object_files

debug: build_object_files build_executable

build_object_files: main.cpp game_piece.cpp game_piece.hpp game_board.cpp game_board.hpp user.hpp user.cpp host.cpp client.cpp
	$(CXX) $(CFLAGS) -o user.o -c user.cpp
	$(CXX) $(CFLAGS) -o host.o -c host.cpp
	$(CXX) $(CFLAGS) -o client.o -c client.cpp
	$(CXX) $(CFLAGS) -o game_piece.o -c game_piece.cpp
	$(CXX) $(CFLAGS) -o game_board.o -c game_board.cpp
	$(CXX) $(CFLAGS) -o main.o -c main.cpp

build_executable: main.o game_board.o game_piece.o client.o host.o user.o
	$(CXX) $(CFLAGS) -o battleship main.o game_board.o game_piece.o client.o host.o user.o -lcurses

remove_object_files:
	rm -f *.o

clean:
	rm -f *.o battleship
