#include "game_piece.hpp"
#include <ncurses.h>
#include <iostream>
#include <sys/time.h>
#include <cstdlib>

#define BOARD_SIZE 10

/**
  The gameboard class object that handles holding the 2d array and
  all functions that operation the array.

  The character meanings are as follows -
  'w' - water
  'x' - hit
  'o' - miss
  'a' - aircraft carrier (5)
  'b' - battleship (4)
  's' - submarine (3)
  'd' - destroyer (3)
  'p' - patrol boat (2)

**/


class Gameboard {

    bool isHost;
public:
    char boardArray[BOARD_SIZE][BOARD_SIZE];

    Gameboard();

    int generateRandomBoard(bool host);

    int generateManualBoard();

    void dummyFunction();

    void printBoard();

private:
    void initializeBoard();

    void initializeGamePieces();

    void generateBoardPlacement();

    short checkStartingPeg(int orientation, int starting_peg, int piece_length);

    void addPieceToBoardArray(int orientation, int starting_peg, int piece_length, char piece_symbol);

    void setIsHost(bool host);

    void removePreviousHighlight(int cursor_x, int cursor_y, int ship_length, int orientation);

    void displayEmptyBoard();

    void displayGeneratedBoard();

    bool highlightShip(int cursor_x, int cursor_y, int ship_length, int orientation, char ship_symbol);

    void placeGamePiece(int cursor_x, int cursor_y, int orientation, int ship_length, char ship_symbol);

    bool acceptGameboard();
};
