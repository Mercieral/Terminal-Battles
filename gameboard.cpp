#include "gameboard.hpp"
#include "game_piece.hpp"
#include <iostream>

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
  cout << "Hi There\n";
}
