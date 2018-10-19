#ifndef PIECE_INCLUDED
#define PIECE_INCLUDED

#include <vector>
#include <string>


enum PieceType {
    PIECE_I, PIECE_L, PIECE_J, PIECE_T, PIECE_O, PIECE_S, PIECE_Z,
    PIECE_VAPOR, PIECE_FOAM, PIECE_CRAZY, NUM_PIECE_TYPES
};

PieceType chooseRandomPieceType(); // implemented in chetyris.cpp

class Game;
class Screen;
class Well;


class Piece
{
public:
    Piece(PieceType type);
    void display(Screen& screen, int x, int y);           // all pieces are displayed the same way
    void erase(Screen& screen, int x, int y);             // erase piece from screen
    bool isCoordinateFilled(int x, int y);                // checks if coordinate is filled in piece grid
    void setOrientation(int orientation);                 // hard code coordinates of each orientation
    
    // accessor functions
    int  getOrientation() const { return m_orientation; }
    PieceType getType() const { return m_type; }
    
    
private:
    PieceType m_type;                           // piece needs to know what type it is
    Game* m_game;
    Well* m_well;
    std::vector<std::vector<char>> pieceGrid;   // 4x4 grid to bound each piece
    int m_orientation = 0;                      // default orientation is 0


};



#endif // PIECE_INCLUDED






 

