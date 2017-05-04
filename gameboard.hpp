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
  bool isHost;
  public:
    Gameboard (bool host);
    void dummyFunction();
    void printBoard ();
  private:
    void initializeBoard();
    void initializeGamePieces();
    void generateBoardPlacement();
    bool checkStartingPeg(int orientation, int starting_peg, int piece_length);
    void addPieceToBoardArray(int orientation, int starting_peg, int piece_length, char piece_symbol);
    void setIsHost(bool host);
};
