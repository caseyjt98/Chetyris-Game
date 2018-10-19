#include "Game.h"
#include "Well.h"
#include "UserInterface.h"
#include "Piece.h"
#include <string>
#include <iomanip>

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 25;

const int WELL_X = 0;
const int WELL_Y = 0;

const int PROMPT_Y = 20;
const int PROMPT_X = 0;

const int SCORE_X = 16;
const int SCORE_Y = 8;

const int ROWS_LEFT_X = 16;
const int ROWS_LEFT_Y = 9;

const int LEVEL_X = 16;
const int LEVEL_Y = 10;

const int NEXT_PIECE_TITLE_X = 16;
const int NEXT_PIECE_TITLE_Y = 3;

const int NEXT_PIECE_X = 16;
const int NEXT_PIECE_Y = 4;


Game::Game(int width, int height)
 : m_screen(SCREEN_WIDTH, SCREEN_HEIGHT), m_level(1)
{
}

void Game::play()
{
    m_well.display(m_screen, WELL_X, WELL_Y);
    displayStatus();  //  score, rows left, level
    displayPrompt("Press the Enter key to begin playing Chetyris!");
    waitForEnter();  // [in UserInterface.h]
 
    for(;;)
    {
        if ( ! playOneLevel())
        {
            break;
        }
        displayPrompt("Good job!  Press the Enter key to start next level!");
        waitForEnter();
        
        // delete everything from filled coords and call update well
        m_well.emptyWell();
        m_well.updateWell(m_screen);
        m_level++;
    }
    displayPrompt("Game Over!  Press the Enter key to exit!");
    waitForEnter();
}

void Game::displayPrompt(std::string s)     
{
    m_screen.gotoXY(PROMPT_X, PROMPT_Y);
    m_screen.printStringClearLine(s);   // overwrites previous text
    m_screen.refresh();
}

void Game::displayStatus()
{
    
    m_screen.gotoXY(NEXT_PIECE_X, NEXT_PIECE_Y);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m_screen.gotoXY(i+NEXT_PIECE_X,j+NEXT_PIECE_Y);
            m_screen.printChar(' ');                // override with empty space to clear
        }
    }
    
    m_screen.gotoXY(NEXT_PIECE_X,NEXT_PIECE_Y);
    if (!m_pieces.empty())
    {
        m_screen.gotoXY(NEXT_PIECE_TITLE_X,NEXT_PIECE_TITLE_Y);
        m_screen.printStringClearLine("Next piece:");
        
        m_pieces.back().display(m_screen,NEXT_PIECE_X,NEXT_PIECE_Y);
    }
    
    m_screen.gotoXY(SCORE_X,SCORE_Y);
    printStatusItem("Score:", m_well.getScore());
    
    m_screen.gotoXY(ROWS_LEFT_X,ROWS_LEFT_Y);
    printStatusItem("Rows left:", m_level*5 - m_well.getNumRowsVaporized());
    
    m_screen.gotoXY(LEVEL_X,LEVEL_Y);
    printStatusItem("Level:", m_level);
    
    m_screen.refresh();
}

bool Game::playOneLevel()
{
    
    // while you have not failed the level
    m_pieces.push(Piece(chooseRandomPieceType()));
    m_pieces.push(Piece(chooseRandomPieceType()));  // start with two random pieces in queue
    displayStatus();
    PieceType pieceType;
    
    while (!levelPassed())
    {
     
        pieceType = m_pieces.front().getType();     // store the type of current piece
        
        m_well.newPiece(&m_pieces.front());          // well now has a pointer to next piece in queue
    
        // if initial position of piece overlaps with filled space
        if (m_well.willCollide(m_well.getxPos(),m_well.getyPos()))
            return false;                // game is over
        
        // Start a timer
        Timer timer;
        bool keepFalling = true;
        
        while (keepFalling == true && m_well.movePieceDown(m_screen))     // while status indicates to keep falling
            
        {
                
            double gravity =  (std::max(1000-(100*(m_level-1)), 100));
            char ch;
            
            
            while (timer.elapsed() < gravity)
            {
                // wait for appropriate amount of time, depending on level
                if (getCharIfAny(ch) == true)           // user input
                {
                    switch(ch)
                    {
                        case ARROW_UP:      // rotate
                            m_well.rotatePiece(m_screen);
                            break;
                        case ARROW_DOWN:    // move down (speed up falling)
                            if (keepFalling == true)
                                keepFalling = m_well.movePieceDown(m_screen);
                            break;
                        case ARROW_LEFT:    // move left
                            if (pieceType == PIECE_CRAZY)
                                m_well.movePieceRight(m_screen);        // crazy piece moves opposite direction
                            else
                                m_well.movePieceLeft(m_screen);
                            break;
                        case ARROW_RIGHT:   // move right
                            if (pieceType == PIECE_CRAZY)
                                m_well.movePieceLeft(m_screen);         // crazy piece moves opposite direction
                            else
                                m_well.movePieceRight(m_screen);
                            break;
                        case 'q':
                            return false;                               // exit program
                        case ' ':
                            keepFalling = m_well.movePieceDown(m_screen);
                            while (keepFalling)
                            {
                                keepFalling = m_well.movePieceDown(m_screen);     // move down while still valid move
                            }
                            break;
                    }
                }
            }
            timer.start();           // restart timer
            m_screen.refresh();
        }
        
        // push another random piece onto the queue
        m_pieces.push(Piece(chooseRandomPieceType()));
        
        // done processing previous piece, pop from queue (FIRST IN, FIRST OUT)
        m_pieces.pop();
        
        displayStatus();
        
    }
      return true; // level passed
}


bool Game::levelPassed()
{
    return (m_well.getNumRowsVaporized() >= m_level*5);
}


void Game::printStatusItem(std::string label, int value)
{
    std::string stringValue = std::to_string(value);
    unsigned long spacesAfterLabel = 11 - label.length();
    
    for (int i = 0; i < spacesAfterLabel; i++)
        label += " ";
    
    unsigned long numSpaces = (7 - stringValue.length());
    std::string rightJustifiedValue;
    
    for (int i=0; i < numSpaces; i++)
        rightJustifiedValue+= " ";
    rightJustifiedValue += stringValue;
    
    
    m_screen.printStringClearLine(label + rightJustifiedValue);
    
}



