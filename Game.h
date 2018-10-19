#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "Well.h"
#include "UserInterface.h"
#include "Piece.h"
#include <string>
#include <queue>


class Game
{
  public:
    Game(int width, int height);
    void play();
    bool playOneLevel();
    void displayPrompt(std::string s);
    void displayStatus();
    bool levelPassed();
    void printStatusItem(std::string label, int value);
    

  private:
    Well    m_well;
    Screen  m_screen;
    int     m_level = 1;            // start at level 1
    std::queue<Piece> m_pieces;     // queue to hold game pieces
    int     m_score;
    
};

#endif // GAME_INCLUDED




