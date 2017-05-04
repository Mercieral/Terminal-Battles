#include "gameboard.hpp"
#include "game_piece.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

Game_Piece piece_array [5];

// empty constructor
Gameboard::Gameboard () {
    initializeBoard();
    initializeGamePieces();
    generateBoardPlacement();
    printBoard();
}

void Gameboard::initializeBoard () {
   for (int i = 0; i < BOARDSIZE; i++){
       for (int j = 0; j < BOARDSIZE; j++){
           boardArray[i][j] = 'w';
       }
   }
}

void Gameboard::initializeGamePieces() {
  Game_Piece aircraft_carrier = Game_Piece(5, 'a');
  piece_array[0] = aircraft_carrier;
  Game_Piece battleship = Game_Piece(4, 'b');
  piece_array[1] = battleship;
  Game_Piece destroyer = Game_Piece(3, 'd');
  piece_array[2] = destroyer;
  Game_Piece submarine = Game_Piece(3, 's');
  piece_array[3] = submarine;
  Game_Piece patrol_boat = Game_Piece(2, 'p');
  piece_array[4] = patrol_boat;
}

void Gameboard::printBoard() {
    cout << "------ Printing Game board ------\n";
    for (int i = 0; i < BOARDSIZE; i++){
        for (int j=0; j < BOARDSIZE; j++){
            cout << boardArray[i][j] << " ";
        }
        cout << "\n";
    }
     cout << "\n";
}

void Gameboard::generateBoardPlacement() {
  for (int i = 0; i < 5; i++) {
    srand(time(0) + i);
    int n = 0;
    int starting_peg = (rand() % 100);
    int orientation = (rand() % 2) + 1;
    int piece_length = piece_array[i].Get_Piece_Length();
    char piece_symbol = piece_array[i].Get_Piece_Symbol();
    while(true) {
      if (!checkStartingPeg(orientation, starting_peg, piece_length)) {
        orientation = (orientation % 2) + 1;
        starting_peg += 1;
        if (starting_peg >= 100) {
          starting_peg = starting_peg / 100;
        }
        n += 1;
      }
      else {
        break;
      }
    } //Piece Collision
    addPieceToBoardArray(orientation, starting_peg, piece_length, piece_symbol);
  }
}

bool Gameboard::checkStartingPeg(int orientation, int starting_peg, int piece_length) {
  if (orientation == 1) {
    if ((starting_peg - (piece_length * 10)) >= 0) {
      for (int j = 0; j < piece_length; j++) {
        if (boardArray[(starting_peg / 10)][(starting_peg % 10)] != 'w') {
          return false;
        } //Piece collision
        starting_peg -= 10;
      }
      return true;
    } //Piece Placement Up
    else {
      for (int j = 0; j < piece_length; j++) {
        if (boardArray[(starting_peg / 10)][(starting_peg % 10)] != 'w') {
          return false;
        } //Piece collision
        starting_peg += 10;
      }
      return true;
    } //Piece Placement Down
  } //Vertical Orientation
  else {
    if ((starting_peg + piece_length) < (((starting_peg / 10) + 1) * 10)) {
      for (int j = 0; j < piece_length; j++) {
        if (boardArray[(starting_peg / 10)][(starting_peg % 10)] != 'w') {
          return false;
        } //Piece collision
        starting_peg += 1;
      }
      return true;
    } //Piece Placement Right
    else {
      for (int j = 0; j < piece_length; j++) {
        if (boardArray[(starting_peg / 10)][(starting_peg % 10)] != 'w') {
          return false;
        } //Piece collision
        starting_peg -= 1;
      }
      return true;
    } //Piece Placement Left
  } //Horizontal Orientation
}

void Gameboard::addPieceToBoardArray(int orientation, int starting_peg, int piece_length, char piece_symbol) {
  if (orientation == 1) {
    if ((starting_peg - (piece_length * 10)) >= 0) {
      for (int j = 0; j < piece_length; j++) {
        boardArray[(starting_peg / 10)][(starting_peg % 10)] = piece_symbol;
        starting_peg -= 10;
      }
    }
    else {
      for (int j = 0; j < piece_length; j++) {
        boardArray[(starting_peg / 10)][(starting_peg % 10)] = piece_symbol;
        starting_peg += 10;
      }
    }
  } //Vertical Orientation
  else {
    if ((starting_peg + piece_length) < (((starting_peg / 10) + 1) * 10)) {
      for (int j = 0; j < piece_length; j++) {
        boardArray[(starting_peg / 10)][(starting_peg % 10)] = piece_symbol;
        starting_peg += 1;
      }
    }
    else {
      for (int j = 0; j < piece_length; j++) {
        boardArray[(starting_peg / 10)][(starting_peg % 10)] = piece_symbol;
        starting_peg -= 1;
      }
    }
  } //Horizontal Orientation
}
