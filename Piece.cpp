#include "Piece.h"
#include "Well.h"
#include "Game.h"
#include "UserInterface.h"
using namespace std;


Piece::Piece(PieceType type)
:pieceGrid(4,vector<char>(4, ' ')), m_type(type)         // empty 4x4 grid
{
    // set coordinates of new piece to orientation 0
    setOrientation(0);
}


void Piece::setOrientation(int orientation)
{
    
    m_orientation = orientation;    // update member variable
    
    
    // reset piece grid to empty
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            pieceGrid[x][y] = ' ';
        }
    }
    
    // populate piece grid with coordinates of new orientation
    // pieceGrid[x][y]
    
    if (m_type == PIECE_T)
    {
        switch(orientation)
        {
            case 0:
                pieceGrid[1][0] = '#';
                pieceGrid[0][1] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[2][1] = '#';
                break;
            case 1:
                pieceGrid[1][0] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[1][2] = '#';
                pieceGrid[2][1] = '#';
                break;
            case 2:
                pieceGrid[0][1] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[1][2] = '#';
                pieceGrid[2][1] = '#';
                break;
            case 3:
                pieceGrid[0][1] = '#';
                pieceGrid[1][0] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[1][2] = '#';
                break;
        }
    }
    else if (m_type == PIECE_L)
    {
        switch(orientation)
        {
            case 0:
                pieceGrid[0][1] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[2][1] = '#';
                pieceGrid[0][2] = '#';
                break;
            case 1:
                pieceGrid[1][0] = '#';
                pieceGrid[2][0] = '#';
                pieceGrid[2][1] = '#';
                pieceGrid[2][2] = '#';
                break;
            case 2:
                pieceGrid[0][2] = '#';
                pieceGrid[1][2] = '#';
                pieceGrid[2][2] = '#';
                pieceGrid[2][1] = '#';
                break;
            case 3:
                pieceGrid[1][1] = '#';
                pieceGrid[1][2] = '#';
                pieceGrid[1][3] = '#';
                pieceGrid[2][3] = '#';
                break;
        }
    }
    else if (m_type == PIECE_J)
    {
        switch(orientation)
        {
            case 0:
                pieceGrid[0][1] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[2][1] = '#';
                pieceGrid[0][2] = '#';
                break;
            case 1:
                pieceGrid[1][3] = '#';
                pieceGrid[2][1] = '#';
                pieceGrid[2][2] = '#';
                pieceGrid[2][3] = '#';
                break;
            case 2:
                pieceGrid[1][1] = '#';
                pieceGrid[1][2] = '#';
                pieceGrid[2][2] = '#';
                pieceGrid[3][2] = '#';
                break;
            case 3:
                pieceGrid[1][0] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[1][2] = '#';
                pieceGrid[2][0] = '#';
                break;
        }
    }
    else if (m_type == PIECE_O)
    {
        // all orientations are the same
        pieceGrid[0][0] = '#';
        pieceGrid[0][1] = '#';
        pieceGrid[1][0] = '#';
        pieceGrid[1][1] = '#';
    }
    else if (m_type == PIECE_S)
    {
        switch(orientation)
        {
            case 0:
                pieceGrid[1][1] = '#';
                pieceGrid[2][1] = '#';
                pieceGrid[0][2] = '#';
                pieceGrid[1][2] = '#';
                break;
            case 1:
                pieceGrid[1][0] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[2][1] = '#';
                pieceGrid[2][2] = '#';
                break;
            case 2:
                pieceGrid[0][2] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[1][2] = '#';
                pieceGrid[2][1] = '#';
                break;
            case 3:
                pieceGrid[1][0] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[2][1] = '#';
                pieceGrid[2][2] = '#';
                break;
        }
    }
    else if (m_type == PIECE_Z)
    {
        switch(orientation)
        {
            case 0:
                pieceGrid[1][2] = '#';
                pieceGrid[2][2] = '#';
                pieceGrid[0][1] = '#';
                pieceGrid[1][1] = '#';
                break;
            case 1:
                pieceGrid[2][0] = '#';
                pieceGrid[2][1] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[1][2] = '#';
                break;
            case 2:
                pieceGrid[0][1] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[1][2] = '#';
                pieceGrid[2][2] = '#';
                break;
            case 3:
                pieceGrid[1][1] = '#';
                pieceGrid[1][2] = '#';
                pieceGrid[2][0] = '#';
                pieceGrid[2][1] = '#';
                break;
        }
    }
    else if (m_type == PIECE_I)
    {
        switch(orientation)
        {
            case 0:
                pieceGrid[0][1] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[2][1] = '#';
                pieceGrid[3][1] = '#';
                break;
            case 1:
                pieceGrid[1][0] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[1][2] = '#';
                pieceGrid[1][3] = '#';
                break;
            case 2:
                pieceGrid[0][1] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[2][1] = '#';
                pieceGrid[3][1] = '#';
                break;
            case 3:
                pieceGrid[1][0] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[1][2] = '#';
                pieceGrid[1][3] = '#';
                break;
        }
    }
    else if (m_type == PIECE_VAPOR)
    {
        // all orientations are the same
        pieceGrid[1][0] = '#';
        pieceGrid[2][0] = '#';
    }
    else if (m_type == PIECE_FOAM)
    {
        // all orientations are the same
        pieceGrid[1][1] = '#';

    }
    else if (m_type == PIECE_CRAZY)
    {
        switch(orientation)
        {
            case 0:
                pieceGrid[0][0] = '#';
                pieceGrid[3][0] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[2][1] = '#';
                break;
            case 1:
                pieceGrid[2][1] = '#';
                pieceGrid[2][2] = '#';
                pieceGrid[3][0] = '#';
                pieceGrid[3][3] = '#';
                break;
            case 2:
                pieceGrid[0][3] = '#';
                pieceGrid[1][2] = '#';
                pieceGrid[2][2] = '#';
                pieceGrid[3][3] = '#';
                break;
            case 3:
                pieceGrid[0][0] = '#';
                pieceGrid[0][3] = '#';
                pieceGrid[1][1] = '#';
                pieceGrid[1][2] = '#';
                break;
        }
    }
}


// display coordinates of piece
void Piece::display(Screen& screen, int x, int y)
{
    screen.gotoXY(x, y);
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            screen.gotoXY(i+x,j+y);
            
            if (pieceGrid[i][j] != ' ')             // if non empty space
                screen.printChar(pieceGrid[i][j]);  // print character
        }
    }
}


void Piece::erase(Screen& screen, int x, int y)
{
    screen.gotoXY(x,y);
   
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            screen.gotoXY(i+x,j+y);
            
            if (pieceGrid[i][j] != ' ')             // if non empty space
                screen.printChar(' ');              // override with empty space to clear
        }
    }
}

bool Piece::isCoordinateFilled(int x, int y)
{
    return (pieceGrid[x][y] != ' ');
}


