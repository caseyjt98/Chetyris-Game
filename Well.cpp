#include "Well.h"
#include "UserInterface.h"
#include "Piece.h"
#include "Game.h"
#include <vector>
#include <iostream>
using namespace std;


Well::Well()
: well(12,vector<char>(18,' '))
{
    // displaying 10x18 format similar to well, with filled boundaries
    // well[x][y]
    // fill boundaries of well with @
 
    for (int y = 0; y < 18; y++)
    {
        well[0][y] = '@';   // fill left boundary
        well[11][y] = '@';  // fill right boundary
        
        // mark boundaries of well as occupied coordinates
        filledCoords.push_back(Coord(0,y));
        filledCoords.push_back(Coord(11,y));
    }
    
    for (int x = 0; x < 12; x++)
    {
        well[x][17] = '@';  // fill bottom boundary
        
        // mark boundaries of well as occupied coordinates
        filledCoords.push_back(Coord(x,17));
    }
}

void Well::display(Screen& screen, int x, int y)
{
    
    screen.gotoXY(x, y);
    
    // display well
    for (int y = 0; y < 18; y++)
    {
        for (int x = 0; x < 12; x++)
        {
            screen.gotoXY(x,y);
            screen.printChar(well[x][y]);
        }
    }
    screen.refresh();
}

// passes in the next piece to fall and sets it to well's current piece
void Well::newPiece(Piece* piece)
{
    // reset member variables to track position of new piece
    m_xPosition = 4;
    m_yPosition = -1;
    
    m_piece = piece;
}


bool Well::rotatePiece(Screen& screen)
{
    // record current orientation before rotation occurs
    int currOrientation = m_piece->getOrientation();
    
    // change orientation
    int newOrientation;
    if (m_piece->getOrientation() == 3)
        newOrientation = 0;                                     // return to orientation 0
    else
        newOrientation = m_piece->getOrientation() + 1;         // increment current orientation
    

    // clear old piece coordinates from the screen
    m_piece->erase(screen,m_xPosition, m_yPosition);
    
    // tenatively set the orientation and check for collision
    m_piece->setOrientation(newOrientation);
    
    if (willCollide(m_xPosition, m_yPosition))         // check tenative rotation for collision
    {
        m_piece->setOrientation(currOrientation);       // collision detected -- stay at current orientation
        m_piece->display(screen, m_xPosition, m_yPosition);
        return false;
    }

    // else NO collision detected ----
    
    // display piece with new orientation
    m_piece->display(screen, m_xPosition, m_yPosition);
    
    
    screen.refresh();
    
    return true;
    
}

bool Well::movePieceLeft(Screen& screen)
{
    bool status = true;                             // default status indicates: valid position
   
    
    // check if incremented position collides with any other filled coordinates in well
    if (willCollide(m_xPosition-1, m_yPosition))
        status = false;
    
    else
    {
        // if valid move
        m_piece->erase(screen,m_xPosition, m_yPosition);       // clear piece from old position
        m_xPosition--;                                          // actually decrement position
        m_piece->display(screen, m_xPosition, m_yPosition);    // display piece with incremented position
        screen.refresh();
    }
    return status;
    
}

bool Well::movePieceRight(Screen& screen)
{
    bool status = true;                                     // default status indicates: valid position
    
    
    // check if incremented position collides with any other filled coordinates in well
    if (willCollide(m_xPosition+1, m_yPosition))
        status = false;

    else
    {
        m_piece->erase(screen,m_xPosition, m_yPosition);       // clear piece from old position
        m_xPosition++;                                          // actually increment position
        m_piece->display(screen, m_xPosition, m_yPosition);    // display piece with incremented position
        screen.refresh();
    }
    
    return status;
}


bool Well::movePieceDown(Screen& screen)
{
    bool status = true;                 // default status indicates: valid position
    
     // check if incremented position exceeds boundaries of well or collides with an occupied coordinate in well
    if (willCollide(m_xPosition, m_yPosition+1))
    {

        
        status = false;                // status indicates: invalid position
    
        
       // check for special piece types
        
        if (m_piece->getType() == PIECE_VAPOR)
        {
            vaporBomb(screen);            // perform special action of vapor bomb
        }
        
        

        
        
        else            // piece has no special action
        {
            // record final filled coordinates
            screen.gotoXY(m_xPosition, m_yPosition);
            
            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    if (m_piece->isCoordinateFilled(x,y))       // if coordinate filled
                    {
                        // add coordinate to well's list of filled spaces
                        if (!isCoordFilled(Coord(m_xPosition+x, m_yPosition+y)))      // avoid duplicate coordinates
                        {
                            filledCoords.push_back( Coord(m_xPosition+x, m_yPosition+y) );
                            changeToDollarSign(screen, m_xPosition+x, m_yPosition+y);
                        }
                    }
                }
            }
        }
    }
    else    // piece CAN move down
    {
        m_piece->erase(screen,m_xPosition,m_yPosition);         // clear piece's old position
        m_yPosition++;                                          // actually increment position
        m_piece->display(screen, m_xPosition, m_yPosition);     // display piece with incremented y position
    }
    
    
    // keep track of newly vaporized rows
    int newlyVaporizedRows = 0;
    
    // check for filled rows, vaporize if applicable
    for (int y = 0; y < 17; y++)
    {
        if (isRowFull(screen,y))
        {
            vaporizeRow(screen, y);
            newlyVaporizedRows++;            // increment number of newly vaporized rows
        }
    }
    
    // assign points based on how many rows were vaporized at once
    if (newlyVaporizedRows == 1)
        m_score+= 100;                  // 100 points awarded for filling one row
    if (newlyVaporizedRows == 2)
        m_score+= 200;                  // 200 points awarded for filling two rows simultaneously
    if (newlyVaporizedRows == 3)
        m_score+= 400;                  // 400 points awarded for filling three rows simultaneously
    if (newlyVaporizedRows == 4)
        m_score+= 800;                  // 800 points awarded for filling four rows simultaneously
    if (newlyVaporizedRows == 5)
        m_score+= 1600;                 // 1600 points awarded for filling five rows simultaneously
    
    
    m_numRowsVaporized += newlyVaporizedRows; // update total number of vaporized rows
    
    
    screen.refresh();   // update screen

    return status;
}

bool Well::willCollide(int xPos, int yPos)      // (x,y) is possible incremented position
{
 
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (m_piece->isCoordinateFilled(x,y))       // if coordinate filled
            {
                Coord pieceCoord = Coord(xPos+x, yPos+y);     // translate coordinates of piece to well's coordinate system
                
                // check coordinate against well's filled coordinates
                for(std::vector<Coord>::iterator it = filledCoords.begin(); it != filledCoords.end(); ++it)
                {

                    if (*it == pieceCoord)
                        return true;            // moving piece will collide with filled position in well
                }
                
            }
        }
        
    }
    return false;               // otherwise, no collision -- valid move
}


void Well::changeToDollarSign(Screen& screen,int x,int y)
{
    screen.gotoXY(x,y);
    screen.printChar('$');
}


bool Well::isCoordFilled(Coord coordToCheck)
{
    for (std::vector<Coord>::iterator it = filledCoords.begin(); it != filledCoords.end(); ++it)
    {
        if (*it == coordToCheck)
        {
            return true;                // coordinate exists in vector of logged filledCoords
        }
    }
    return false;
}


bool Well::isRowFull(Screen& screen, int y)
{
    int numFilledSpaces = 0;
    for (int x = 1; x < 11; x++)
    {
        Coord coordToCheck = Coord(x,y);
        
        // loop through vector of filled coordinate to find coord
        for (std::vector<Coord>::iterator it = filledCoords.begin(); it != filledCoords.end(); ++it)
        {
            if (*it == coordToCheck)
            {
                numFilledSpaces++;          // if coord is filled, increment number of filled spaces
            }
        }
    }
    if (numFilledSpaces == 10)          // all spaces in row are filled
        return true;

    return false;
}


// this function shifts everything down and updates well's record of filled coordinates
void Well::vaporizeRow(Screen& screen, int yFilledRow)
{

    for (std::vector<Coord>::iterator it = filledCoords.begin(); it != filledCoords.end(); it++)
    {
        if (it->y() < 17 && it->x() > 0 && it->x() < 11)        // excluding boundaries of well
        {
            if (it->y() == yFilledRow)               // if current y coordinate matches y coordinate of vaporized row
            {
                filledCoords.erase(it);     // remove coordinate of vaporized row from filledCoords vector
                it--;                       // make up for lost item
            }
        }
    }


    // update remaining filled coordinates based on their new position after shifting down
    for (std::vector<Coord>::iterator it = filledCoords.begin(); it != filledCoords.end(); it++)
    {
        int currentY = it->y();
        
        if (it->y() < 17 && it->x() > 0 && it->x() < 11)        // excluding boundaries of well
        {
            if (currentY < yFilledRow)           // if coordinate lies above vaporized row -- shift down
            {
                it->setY(currentY+1);
            }
        }
    
    }
    updateWell(screen);
    
}

void Well::vaporBomb(Screen& screen)
{
    vector<Coord> affectedCoords;
    // coordinate of actual filled spaces in vapor's piece grid
    int vaporXPos = m_xPosition+1;
    int vaporYPos = m_yPosition;
    
    // bomb vaporizes itself and all blocks up to 2 below and 2 above it
    // push back coordinates of corresponding area
    affectedCoords.push_back(Coord(vaporXPos, vaporYPos-2));
    affectedCoords.push_back(Coord(vaporXPos+1, vaporYPos-2));
    affectedCoords.push_back(Coord(vaporXPos, vaporYPos-1));
    affectedCoords.push_back(Coord(vaporXPos+1, vaporYPos-1));
    affectedCoords.push_back(Coord(vaporXPos, vaporYPos));
    affectedCoords.push_back(Coord(vaporXPos+1, vaporYPos));
    affectedCoords.push_back(Coord(vaporXPos, vaporYPos+1));
    affectedCoords.push_back(Coord(vaporXPos+1, vaporYPos+1));
    affectedCoords.push_back(Coord(vaporXPos, vaporYPos+2));
    affectedCoords.push_back(Coord(vaporXPos+1, vaporYPos+2));
    
    
    // if affected Coordinates are filled, vaporize them (remove from filledCoords)
    for (std::vector<Coord>::iterator it1 = affectedCoords.begin(); it1 != affectedCoords.end(); it1++)
    {
        for (std::vector<Coord>::iterator it2 = filledCoords.begin(); it2 != filledCoords.end(); it2++)
        {
            if (it2->y() < 17 && it2->x() > 0 && it2->x() < 11)        // excluding boundaries of well
            {
                if (*it1 == *it2)
                {
                    filledCoords.erase(it2);
                    it2--;                       // make up for lost item
                }
            }
        }
    }
    updateWell(screen);
}


void Well::updateWell(Screen& screen)
{
    
    // clear inside of the well by resetting to empty char ' '
    for (int y = 0; y < 17; y++)
    {
        for (int x = 1; x < 11; x++)
        {
            screen.gotoXY(x,y);
            screen.printChar(' ');
        }
     }
    
    // display adjusted filled coordinates
    
    for (std::vector<Coord>::iterator it = filledCoords.begin(); it != filledCoords.end(); ++it)
    {
        if (it->y() < 17 && it->x() > 0 && it->x() < 11)        // excluding boundaries of well
        {
            screen.gotoXY(it->x(), it->y());
            screen.printChar('$');
         }
     }

    screen.refresh();
}


// empty the inside of the well by clearing grid spaces from well's record of filled coordinates
void Well::emptyWell()
{
    for (std::vector<Coord>::iterator it = filledCoords.begin(); it != filledCoords.end(); ++it)
    {
        if (it->y() < 17 && it->x() > 0 && it->x() < 11)        // excluding boundaries of well
        {
            filledCoords.erase(it);
            it--;
        }
    }
}




