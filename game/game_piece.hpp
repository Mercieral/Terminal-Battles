/**
  Game_Piece class, used to create the game pieces on the board.
  Length and symbol are used to identify individual pieces, and
  getter methods are used to obtain information about the Game_Piece object
**/

class Game_Piece {
    int piece_length;
    char piece_symbol;
public:
    Game_Piece();

    Game_Piece(int length, char symbol);

    int Get_Piece_Length();

    char Get_Piece_Symbol();
};
