#include "game_board.hpp"

using namespace std;

Game_Piece piece_array[5];

Gameboard::Gameboard() {
  //empty constructor
}

int Gameboard::generateRandomBoard(bool host) {
    setIsHost(host);
    initializeBoard();
    initializeGamePieces();
    generateBoardPlacement();
    displayGeneratedBoard();

    bool isDeciding = true;
    while (isDeciding) {
      switch(getch())
      {
        case 'y':
          isDeciding = false;
          break;
        case 'n':
          initializeBoard();
          generateBoardPlacement();
          displayGeneratedBoard();
          break;
        case 'm':
          return 0;
        default:
          move(20, 52);
          printw("Invalid Input please enter Y/N to accept board\n");
          refresh();
          break;
      }
    }
    return 1;
}

void Gameboard::displayGeneratedBoard() {
  move(6, 1);
  printw(
      "  --------------------Your Board--------------\n"
      "   ____________________________________________\n"
      "   |  | A | B | C | D | E | F | G | H | I | J |\n"
      "   | 1|   |   |   |   |   |   |   |   |   |   |\n"
      "   | 2|   |   |   |   |   |   |   |   |   |   |\n"
      "   | 3|   |   |   |   |   |   |   |   |   |   |\n"
      "   | 4|   |   |   |   |   |   |   |   |   |   |\n"
      "   | 5|   |   |   |   |   |   |   |   |   |   |\n"
      "   | 6|   |   |   |   |   |   |   |   |   |   |\n"
      "   | 7|   |   |   |   |   |   |   |   |   |   |\n"
      "   | 8|   |   |   |   |   |   |   |   |   |   |\n"
      "   | 9|   |   |   |   |   |   |   |   |   |   |\n"
      "   |10|   |   |   |   |   |   |   |   |   |   |\n"
      "   --------------------------------------------\n"
      "                                                ||\n"
      "                                                ||\n"
      " Y    - accept board                            ||\n"
      " M    - generate board manually                 ||\n"
      " N    - generate new board                      ||\n");
  attron(A_UNDERLINE);
  move(21, 1);
  printw("instructions");
  attroff(A_UNDERLINE);
  move(13, 24);
  refresh();
  //int count = 0;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (boardArray[i][j] != 'w')
      {
        move(i + 9, (j * 4) + 8);
        addch(boardArray[i][j]);
        // refresh();
        // move(25 + count, 50);
        // printw("In for loop");
        // refresh();
      }
    }
  }
  refresh();
}

int Gameboard::generateManualBoard() {
  initializeBoard();
  initializeGamePieces();
  displayEmptyBoard();

  bool isPlacing = true;
  int cursor_x = 24, cursor_y = 13, ship_to_place = 0, orientation = 0;
  move(cursor_y, cursor_x);
  refresh();

  while (isPlacing)
  {
    Game_Piece ship = piece_array[ship_to_place];
    bool isValidPlacement = highlightShip(cursor_x, cursor_y, ship.Get_Piece_Length(), orientation, ship.Get_Piece_Symbol());
    switch (getch())
    {
    case KEY_LEFT:
    case 'a':
        if (orientation == 0)
        {
          if ((cursor_x - (4 * (ship.Get_Piece_Length() - 1))) > 8)
          {
              removePreviousHighlight(cursor_x, cursor_y, ship.Get_Piece_Length(), orientation);
              cursor_x -= 4;
          } //Still on board
        } //Orientation is Horizontal
        else
        {
          if (cursor_x > 8)
          {
              removePreviousHighlight(cursor_x, cursor_y, ship.Get_Piece_Length(), orientation);
              cursor_x -= 4;
          } //Still on board
        } //Orientation is Vertical
        break;
    case KEY_RIGHT:
    case 'd':
        if (cursor_x < 44)
        {
            removePreviousHighlight(cursor_x, cursor_y, ship.Get_Piece_Length(), orientation);
            cursor_x += 4;
        }
        break;
    case KEY_UP:
    case 'w':
        if (orientation == 1)
        {
          if ((cursor_y - (ship.Get_Piece_Length() - 1)) > 9)
          {
            removePreviousHighlight(cursor_x, cursor_y, ship.Get_Piece_Length(), orientation);
            cursor_y -= 1;
          } //Still on board
        } //Orientation is Vertical
        else
        {
          if (cursor_y > 9)
          {
              removePreviousHighlight(cursor_x, cursor_y, ship.Get_Piece_Length(), orientation);
              cursor_y -= 1;
          } //Still on board
        } //Orientation is Horizontal
        break;
    case KEY_DOWN:
    case 's':
        if (cursor_y < 18)
        {
            removePreviousHighlight(cursor_x, cursor_y, ship.Get_Piece_Length(), orientation);
            cursor_y += 1;
        }
        break;
    case 'r':
        if (orientation == 0)
        {
          if ((cursor_y - (ship.Get_Piece_Length() - 1)) >= 9)
          {
            removePreviousHighlight(cursor_x, cursor_y, ship.Get_Piece_Length(), orientation);
            orientation = 1;
          } //Still On Board
        } //Changing to Vertical
        else
        {
          if ((cursor_x - (4 * (ship.Get_Piece_Length() - 1))) >= 8)
          {
            removePreviousHighlight(cursor_x, cursor_y, ship.Get_Piece_Length(), orientation);
            orientation = 0;
          } //Still on Board
        } //Changing to Horizontal
        break;
    case 'g':
        return 0;
    case 10:
        if (isValidPlacement) {
          placeGamePiece(cursor_x, cursor_y, orientation, ship.Get_Piece_Length(), ship.Get_Piece_Symbol());
          if (ship_to_place == 4)
          {
            if (!acceptGameboard())
            {
              ship_to_place = 0;
              orientation = 0;
              cursor_y = 13;
              cursor_x = 24;
              initializeBoard();
              displayEmptyBoard();
              break;
            } //Remake Gameboard
            else
            {
              isPlacing = false;
              break;
            } //Accept Gameboard
          } //Final Ship Placed
          ship_to_place++;
          cursor_y = 13;
          cursor_x = 24;
          break;
        } //No Collision
        else
        {
          move(20, 52);
          printw("Collision cannot place ship");
        } //Collision
    } //End of switch case
    move(cursor_y, cursor_x);
    refresh();
  }
  return 1;
}

bool Gameboard::acceptGameboard() {
  move(20, 52);
  printw("Do you want to use this board? Enter Y/N");
  while(true) {
    switch (getch())
    {
      case 'y':
        return true;
      case 'n':
        return false;
      default:
        move(20, 52);
        printw("Invalid Input please enter Y/N to accept board\n");
        refresh();
    }
  }
}

void Gameboard::placeGamePiece(int cursor_x, int cursor_y, int orientation, int ship_length, char ship_symbol) {
  if (orientation == 0)
  {
    for (int i = 0; i < ship_length; i++) {
      boardArray[cursor_y - 9][((cursor_x - (4 * i)) - 8)/4] = ship_symbol;
    }
  } //Orientation is horizontal
  else
  {
    for (int i = 0; i < ship_length; i++) {
      boardArray[(cursor_y - i) - 9][(cursor_x - 8)/4] = ship_symbol;
    }
  } //Orientation is vertical
}

bool Gameboard::highlightShip(int cursor_x, int cursor_y, int ship_length, int orientation, char ship_symbol) {
  bool validPlacement = true;
  int i = 0;
  if (orientation == 0)
  {
    for (i = 0; i < ship_length; i++) {
      if (boardArray[cursor_y - 9][((cursor_x - (4 * i)) - 8)/4] != 'w')
      {
        validPlacement = false;
      } //Collision with placed ship
      else
      {
        move(cursor_y, cursor_x - (4 * i));
        attron(A_STANDOUT);
        addch(ship_symbol);
      } //No ship collision
    }
  } //Horizontal Orientation
  else
  {
    for (i = 0; i < ship_length; i++) {
      if (boardArray[(cursor_y - i) - 9][(cursor_x - 8)/4] != 'w')
      {
        validPlacement = false;
      } //Collision with placed ship
      else
      {
        move(cursor_y - i, cursor_x);
        attron(A_STANDOUT);
        addch(ship_symbol);
      } //No ship collision
    }
  } //Vertical Orientation
  attroff(A_STANDOUT);
  move(cursor_y, cursor_x);
  return validPlacement;
}

void Gameboard::removePreviousHighlight(int cursor_x, int cursor_y, int ship_length, int orientation) {
  int i = 0;
  if (orientation == 0)
  {
    for (i = 0; i < ship_length; i++) {
      if (boardArray[cursor_y - 9][((cursor_x - (4 * i)) - 8)/4] == 'w')
      {
        move(cursor_y, cursor_x - (4 * i));
        addch(' ');
      } //No collision with placed ship
    }
  } //Orientation is Horizontal
  else
  {
    for (i = 0; i < ship_length; i++) {
      if (boardArray[(cursor_y - i) - 9][(cursor_x - 8)/4] == 'w')
      {
        move(cursor_y - i, cursor_x);
        addch(' ');
      } //No collision with placed ship
    }
  } //Orientation is Vertical
}

void Gameboard::displayEmptyBoard() {
  move(6, 1);
  printw(
      "  --------------------Your Board--------------\n"
      "   ____________________________________________\n"
      "   |  | A | B | C | D | E | F | G | H | I | J |\n"
      "   | 1|   |   |   |   |   |   |   |   |   |   |\n"
      "   | 2|   |   |   |   |   |   |   |   |   |   |\n"
      "   | 3|   |   |   |   |   |   |   |   |   |   |\n"
      "   | 4|   |   |   |   |   |   |   |   |   |   |\n"
      "   | 5|   |   |   |   |   |   |   |   |   |   |\n"
      "   | 6|   |   |   |   |   |   |   |   |   |   |\n"
      "   | 7|   |   |   |   |   |   |   |   |   |   |\n"
      "   | 8|   |   |   |   |   |   |   |   |   |   |\n"
      "   | 9|   |   |   |   |   |   |   |   |   |   |\n"
      "   |10|   |   |   |   |   |   |   |   |   |   |\n"
      "   --------------------------------------------\n"
      "                                                ||\n"
      "                                                ||\n"
      " g  generate random board                       ||\n"
      " w, up arrow    - move the cursor up            ||\n"
      " a, left arrow  - move the cursor left          ||\n"
      " s, down arrow  - move the cursor down          ||\n"
      " d, right arrow - move the cursor right         ||\n"
      " r              - rotate piece                  ||\n"
      " enter          - place piece                   ||\n");
  attron(A_UNDERLINE);
  move(21, 1);
  printw("instructions");
  attroff(A_UNDERLINE);
  move(13, 24);
  refresh();
}

void Gameboard::initializeBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
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
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
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
