// Game.cpp

#include "Game.h"
#include "utilities.h"
#include <iostream>
#include "Dungeon.h"
using namespace std;


Game::Game(int goblinSmellDistance)
:goblinSmellDistance(goblinSmellDistance)
{
    m_dungeon = new Dungeon(0);     //Create a dungeon of level 0
}

Game::~Game()
{
    delete m_dungeon;
}

void Game::monstersMove()   //All the target locations for the monsters are set to the players position
{
    for (int i = 0; i < m_dungeon->getMonsters().size(); i++)
    {
        if (m_dungeon->getMonsters()[i]->symbol() == 'B')
        {
            m_dungeon->getMonsters()[i]->moveMonsters(m_dungeon->player()->row(),m_dungeon->player()->col(),5); //Bogeyman have smell of 5
        }
        else if (m_dungeon->getMonsters()[i]->symbol() == 'S')
        {
            m_dungeon->getMonsters()[i]->moveMonsters(m_dungeon->player()->row(),m_dungeon->player()->col(),3); //Snakewomen have smell of 3
        }
        else if (m_dungeon->getMonsters()[i]->symbol() == 'D')
        {
            m_dungeon->getMonsters()[i]->moveMonsters(m_dungeon->player()->row(),m_dungeon->player()->col(),0); //Dragons have a smell of 0, i.e. stationary
        }
        else if(m_dungeon->getMonsters()[i]->symbol() == 'G')
        {
           // m_dungeon->getMonsters()[i]->moveGoblin(m_dungeon->getMonsters()[i]->row(), m_dungeon->getMonsters()[i]->col(), m_dungeon->player()->row(), m_dungeon->player()->col(), goblinSmellDistance);
            m_dungeon->getMonsters()[i]->moveMonsters(m_dungeon->player()->row(), m_dungeon->player()->col(), goblinSmellDistance);
        }
    }
   
}
int Game::decodeDirection(char dir) //Decodes the h,j,k,l to their respective arrow keys
{
    switch (dir)
    {
        case 'k':  return ARROW_UP;
        case 'j':  return ARROW_DOWN;
        case 'h':  return ARROW_LEFT;
        case 'l':  return ARROW_RIGHT;
    }
    return -1;  // bad argument passed in!
}

void Game::play()
{
    m_dungeon->makeLevel(0);    //create our level 0
    m_dungeon->display();       //Display our level to begin with
    cout << endl;
    char move = getCharacter();     //Get character from the player
    while (move != 'q' && !m_dungeon->player()->isDead() && !m_dungeon->player()->getWon())     //While we havent quit, died or won
    {
        switch (move)
        {
            case 'k':
            case 'j':
            case 'h':
            case 'l':
                m_dungeon->player()->move(decodeDirection(move));   //moves in direction based on h,j,k,l
                m_dungeon->player()->heal();    //Heal at every move based on 1/10 chance
                monstersMove(); //After the player moves, move monsters
                m_dungeon->display();   //after every move display the dungeon
                break;
            case 'i':
            case 'w':
            case 'r':
                m_dungeon->player()->inventory(move);   //For each of i,w,r display the inventory and do its respective action
                break;
            case 'c':
                m_dungeon->player()->cheat();   //Cheat counts as a move and thus makes monsters move as well
                monstersMove();
                m_dungeon->display();
                break;
            case 'g':
                if(m_dungeon->player()->pickUpItem(m_dungeon->player()->row(), m_dungeon->player()->col())) //If an object is at our position
                {
                    if (m_dungeon->player()->getWon())  //If we won, by picking up a & (golden statue)
                    {
                        m_dungeon->display();
                        cout << "You pick up the golden idol" << endl;
                        cout << "Congratulations, you won!" << endl;
                        goto mylabel;       //If we win, break out of all these loops and go to the location defined as mylabel
                        break;
                    }
                    string pickUp = "You pick up a " + m_dungeon->getObjectAt(m_dungeon->player()->row(), m_dungeon->player()->col())->name();
                    if (m_dungeon->getObjectAt(m_dungeon->player()->row(), m_dungeon->player()->col())->isScroll())     //Different string printed for picking up a string
                        pickUp = "You pick up a scroll called " + m_dungeon->getObjectAt(m_dungeon->player()->row(), m_dungeon->player()->col())->name();
                    m_dungeon->removeObjectAt(m_dungeon->player()->row(), m_dungeon->player()->col());  //Remove the object from our display and our dungeon
                    monstersMove(); //Monsters move after you pick up an object
                    m_dungeon->display();
                    cout << pickUp; //Display what we picked up
                    break;
                }
                else
                    break;
            case '>':
                if (m_dungeon->getCharAt(m_dungeon->player()->row(), m_dungeon->player()->col()) == '>')
                {
                    m_dungeon->setLevel(1);     //If we enter a staircase, increment the level by 1
                    m_dungeon->makeLevel(m_dungeon->getLevel());    //Create the new level
                    m_dungeon->display();       //Display the level
                }
                break;
            default:
                break;
        }
        if (m_dungeon->player()->isDead())
            goto mylabel;
        move = getCharacter();      //Take in input from the user
    }
    
mylabel:    //Enter this point if we win
    if (m_dungeon->player()->isDead() || m_dungeon->player()->getWon())
    {
        cout << "Press q to exit the game\n";
        while(getCharacter() != 'q')   //Until we press q to exit, run in a loop
        {
            ;
        }
    }
}
