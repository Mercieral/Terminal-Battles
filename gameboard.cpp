#include "gameboard.hpp"
#include <iostream>

using namespace std;

// empty constructor
Gameboard::Gameboard () {
    initializeBoard();
    printBoard();
}

void Gameboard::initializeBoard () {
   for (int i = 0; i < BOARDSIZE; i++){
       for (int j = 0; j < BOARDSIZE; j++){
           boardArray[i][j] = 'w';
       }
   }
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