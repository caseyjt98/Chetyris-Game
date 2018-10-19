#ifndef WELL_INCLUDED
#define WELL_INCLUDED

#include <vector>

class Screen;
class Piece;
class Game;

/////////////////////////// CORD CLASS //////////////////////////////

 class Coord
 {
public:
     Coord(): m_x(0), m_y(0) {}                  // default constructor for Coord (0,0)
     Coord(int x, int y) : m_x(x), m_y(y) {}
     
     // accessor functions
     int x() const { return m_x; }
     int y() const { return m_y; }
     
     // setter functions
     void setX(int x) { m_x = x; }
     void setY(int y) { m_y = y; }
     
     // comparison operator
     bool operator== (Coord other) { return (m_x == other.x() && m_y == other.y()); }
     
private:
     int        m_x;
     int        m_y;

 };


///////////////////////// WELL CLASS //////////////////////////////

class Well
{
    
public:
    Well();
    void newPiece(Piece* piece);                            // add next piece to well
    void display(Screen& screen, int x, int y);             // display well
    void updateWell(Screen& screen);
    void emptyWell();                                       // empty out filled coord vector
    
    
    // movement functions
    bool rotatePiece(Screen& screen);
    bool movePieceLeft(Screen& screen);
    bool movePieceRight(Screen& screen);
    bool movePieceDown(Screen& screen);
    
    // functions to check status
    void isRowFilled(Screen& screen, int y);                // y parameter indicates row to check
    bool willCollide(int xPos, int yPos);                   // check for valid move
    bool isRowFull(Screen& screen, int y);                  // check to see if row y is filled
    bool isCoordFilled(Coord coordToCheck);                 // check if coordinate exists in filledCoord vector
    
    // helper functions
    void vaporizeRow(Screen& screen, int yFilledRow);       // y parameter indicates row to vaporize
    void vaporBomb(Screen& screen);                         // performs special action of Vapor Bomb
    void changeToDollarSign(Screen& screen, int x,int y);   // changes piece to dollar sign once it is done falling
  
    
    // accessor functions
    int getNumRowsVaporized() const { return m_numRowsVaporized; }
    int getScore() const { return m_score; }
    int getxPos() const { return m_xPosition; }
    int getyPos() const { return m_yPosition; }
    
    
private:
    std::vector<std::vector<char>> well;            // 2D coordinate system of well
    std::vector<Coord> filledCoords;                // keeps track of filled spaces in well
    Game*  m_game;
    Piece* m_piece;                                 // pointer to current piece falling
    int    m_yPosition = -1;                        // tracks y position of falling piece
    int    m_xPosition = 4;                         // tracks x position of falling piece
    int    m_numRowsVaporized = 0;
    int    m_score = 0;
   
   

};


#endif // WELL_INCLUDED

// well class

