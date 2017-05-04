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
    cout << "This is the piece: " << piece_array[i].Get_Piece_Symbol() << '\n';
    int starting_peg = rand() % 100 + 1;
    cout << "Random Start: " << to_string(starting_peg) << '\n';
    int orientation = rand() % 2 + 1;
    cout << "Orientation: " << to_string(orientation) << '\n';
    if (orientation == 1) {
      //+- 10
    } //Vertical
    else {
      //+- 1
    } //Horizontal
  }
}
