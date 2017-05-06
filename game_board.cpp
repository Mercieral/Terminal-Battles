#include "game_board.hpp"
#include "game_piece.hpp"
#include <iostream>
#include <sys/time.h>
#include <cstdlib>

using namespace std;

Game_Piece piece_array[5];

// empty constructor
Gameboard::Gameboard(bool host) {
    setIsHost(host);
    initializeBoard();
    initializeGamePieces();
    generateBoardPlacement();
    printBoard();
}

void Gameboard::initializeBoard() {
    for (int i = 0; i < BOARDSIZE; i++) {
        for (int j = 0; j < BOARDSIZE; j++) {
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
    for (int i = 0; i < BOARDSIZE; i++) {
        for (int j = 0; j < BOARDSIZE; j++) {
            cout << boardArray[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void Gameboard::generateBoardPlacement() {
    //cout << "Am i host: " << to_string(isHost) << '\n';
    timeval t1;
    gettimeofday(&t1, NULL);
    if (isHost) {
        srand(t1.tv_usec * t1.tv_sec + 3000);
    } else {
        srand(t1.tv_usec * t1.tv_sec);
    }
    for (int i = 0; i < 5; i++) {
        int n = 0;
        int starting_peg = (rand() % 100);
        int orientation = (rand() % 2) + 1;
        int piece_length = piece_array[i].Get_Piece_Length();
        char piece_symbol = piece_array[i].Get_Piece_Symbol();
        //printf("random for %c peg = %d, orientation = %d\n", piece_symbol, starting_peg, orientation);
        int ret = 0;
        while ((ret = checkStartingPeg(orientation, starting_peg, piece_length)) != 1) {
            if (ret == 2) {
                orientation = (orientation == 1) ? 2 : 1;
                //printf("failed on wall! flipping orientation for %c peg = %d, orientation = %d\n", piece_symbol, starting_peg, orientation);
                ret = checkStartingPeg(orientation, starting_peg, piece_length);
                if (ret == 0) {
                    break;
                }
            }

            starting_peg = (rand() % 100);
            orientation = (rand() % 2) + 1;
            //printf("failed on ship! new random for %c peg = %d, orientation = %d\n", piece_symbol, starting_peg, orientation);
            n += 1;
        }
        //Piece Collision
        addPieceToBoardArray(orientation, starting_peg, piece_length, piece_symbol);
    }
    // printBoard();
    // while(true){};
}

// ret - 1 = good, 2 = collided with wall, 3 = collided with ship
short Gameboard::checkStartingPeg(int orientation, int starting_peg, int piece_length) {
    if (orientation == 1) {
        for (int j = 0; j < piece_length; j++) {
            if ((starting_peg / 10) + j >= 10) {
                return 2;
            } else if (boardArray[(starting_peg / 10) + j][(starting_peg % 10)] != 'w') {
                return 3;
            }
            //Piece collision
        }
        return 1;
    } //Vertical Orientation
    else {
        for (int j = 0; j < piece_length; j++) {
            if ((starting_peg % 10) - j < 0) {
                return 2;
            }
            if (boardArray[(starting_peg / 10)][(starting_peg % 10) - j] != 'w') {
                return 3;
            } //Piece collision
        }
        return 1;
    } //Horizontal Orientation
}

void Gameboard::addPieceToBoardArray(int orientation, int starting_peg, int piece_length, char piece_symbol) {
    if (orientation == 1) {
        for (int j = 0; j < piece_length; j++) {
            boardArray[(starting_peg / 10) + j][(starting_peg % 10)] = piece_symbol;
        }
    } //Vertical Orientation
    else {
        for (int j = 0; j < piece_length; j++) {
            boardArray[(starting_peg / 10)][(starting_peg % 10) - j] = piece_symbol;
        }
    } //Horizontal Orientation
}

void Gameboard::setIsHost(bool host) {
    isHost = host;
}

void Gameboard::dummyFunction() {
    //Do nothing make issue
}
