#define BOARDSIZE 10

/**
  The gameboard class object that handles holding the 2d array and 
  all functions that operateon the array.

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
  char boardArray [BOARDSIZE][BOARDSIZE];
  public:
    Gameboard ();
    void printBoard ();
  private:
    void initializeBoard();
};