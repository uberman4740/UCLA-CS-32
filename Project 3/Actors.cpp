//
//  Actors.cpp
//  Project 3
//
//  Created by Jahan Cherian on 5/12/15.
//  Copyright (c) 2015 Jahan Cherian. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <cctype>
#include "Actors.h"
#include "utilities.h"
#include "Dungeon.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
//                            ACTORS                                //
//////////////////////////////////////////////////////////////////////


bool Actors::directionToDeltas(int dir, int& rowDelta, int& colDelta)   //Converts the various directions to changes in the row and cols
{
    switch (dir)
    {
        case ARROW_UP:     rowDelta = -1; colDelta =  0; break;
        case ARROW_DOWN:   rowDelta =  1; colDelta =  0; break;
        case ARROW_LEFT:   rowDelta =  0; colDelta = -1; break;
        case ARROW_RIGHT:  rowDelta =  0; colDelta =  1; break;
        default:     return false;
    }
    return true;
}

void Actors::move(int dir)
{
    m_attack = "";      //Set the attack string to empty
    int maxCanMove = 0;  // maximum distance player can move in direction dir
    switch (dir)
    {
        case ARROW_UP:     maxCanMove = m_row - 1;             break;
        case ARROW_DOWN:   maxCanMove = m_dungeon->row() - m_row; break;
        case ARROW_LEFT:   maxCanMove = m_col - 1;             break;
        case ARROW_RIGHT:  maxCanMove = m_dungeon->col() - m_col; break;
    }
    if (maxCanMove == 0)  // against wall
        return;
    int rowDelta;
    int colDelta;
    if (!directionToDeltas(dir, rowDelta, colDelta))
        return;
    if (isAsleep())     //If we are asleep, decrement the sleep counter and do not move
    {
        addSLP(-1);
        return;
    }
    if(m_dungeon->MonstersAt(m_row + rowDelta, m_col + colDelta))   //If we move into a monster
    {
        Actors* opponent = nullptr;
        for(int i = 0; i < m_dungeon->getMonsters().size(); i++)    //Get the monster we move into and make it our opponent
        {
            if (m_dungeon->getMonsters()[i]->row() == (m_row + rowDelta) && m_dungeon->getMonsters()[i]->col() == (m_col + colDelta))
            {
                opponent = m_dungeon->getMonsters()[i];
                break;
            }
        }
        if (opponent != nullptr)
        {
            attack(opponent);       //Attack the monster
        }
    }
    else if (!m_dungeon->MonstersAt(m_row + rowDelta, m_col + colDelta) && m_dungeon->getCharAt(m_row + rowDelta, m_col + colDelta) != '#') //If we are moving to a blank, then just update our coordinates
    {
        m_row += rowDelta;
        m_col += colDelta;
        return;
    }
    else if(m_dungeon->getCharAt(m_row + rowDelta, m_col + colDelta) == '#')    //If the position we are moving to is a wall, dont do anything
    {
       return;
    }
}

bool Actors::putToSleep(Actors* opponent)   //Puts our opponent to sleep
{
    if (trueWithProbability(0.2))   //1 in 5 chance of putting an actor to sleep
    {
        int sleeper = randInt(4)+2;
        if (!opponent->isAsleep()) //If not already asleep
            opponent->addSLP(sleeper);  //Make actor sleep
        else    //If already asleep
        {
            int currentSleep = opponent->getSLP();
            if (currentSleep >= sleeper)    //Set the sleep amount to max of the current or the new sleep amount
                opponent->addSLP(currentSleep);
            else
                opponent->addSLP(sleeper);
        }
        return true;
    }
    return false;
}

void Actors::attack(Actors* opponent)
{
    m_attack = "";      //Set the attack string to empty
    string extra;
    bool hit = false;   //Hit is initially false
    int attackerPoints = getDP() + m_weapon->getbonusDex();
    int opponentPoints = opponent->getDP() + opponent->m_weapon->getbonusDex();
    if (randInt(attackerPoints) >= randInt(opponentPoints))     //Calculates the chance of a hit based on dexterity
    {
        hit = true;
    }
    if (this == m_dungeon->player() || opponent == m_dungeon->player()) //So no monster vs. monster
    {
        if (opponent == m_dungeon->player())
            extra = "the ";
        else
            extra = "";
        
        if (hit)    //Successful hit
        {
            int damagePoints = randInt(this->getSTRP()+ m_weapon->getbonusDamage());    //Random damage amount
            opponent->addHP(-damagePoints);     //Change the hit points based on this damage amount
            if (!opponent->isDead())
            {
                if (opponent->getEquipped()->name() == "magic fangs of sleep")      //Special case for magic fangs of sleep
                {
                     m_attack = extra + m_name + " " + m_weapon->getAction() + " magic fangs at " + opponent->name() + " and hits.\n";
                    if (putToSleep(this))       //If we put the opponent to sleep
                    {
                        m_attack = extra + m_name + " " + m_weapon->getAction() + " magic fangs at " + opponent->name() + " and hits, putting " + opponent->name() + " to sleep.\n";
                    }
                }
                else if (this->getEquipped()->name() == "magic fangs of sleep")     //Case for the other involved actor wielding magic fangs of sleep
                {
                    m_attack = extra + m_name + " " + m_weapon->getAction() + " magic fangs at " + opponent->name() + " and hits.\n";
                    if (putToSleep(opponent))
                    {
                        m_attack = extra + m_name + " " + m_weapon->getAction() + " magic fangs at " + opponent->name() + " and hits, putting " + opponent->name() + " to sleep.\n";
                        return;
                    }
                }
                m_attack =  extra + m_name + " " + m_weapon->getAction() + " " + m_weapon->name() + " at " + opponent->name() + " and hits.\n";
            }
            else    //Case of where the opponent is killed
            {
                if (opponent->getEquipped()->name() == "magic fangs of sleep" || this->getEquipped()->name() == "magic fangs of sleep") //Special case for magic fangs of sleep
                    m_attack = extra + m_name + " " + m_weapon->getAction() + " magic fangs at " + opponent->name() + " and deals the final blow.\n";
               
                m_attack = extra + m_name + " " + m_weapon->getAction() + " " + m_weapon->name() + " at " + opponent->name() + " and deals the final blow.\n";
                opponent->setDead();
                m_dungeon->removeMonster(); //Remove the monster from the screen, we don't remove the player because if the player dies, we just quit the game
            }
        }
        else    //The hit is not valid so we miss
        {
            if (opponent->getEquipped()->name() == "magic fangs of sleep" || this->getEquipped()->name() == "magic fangs of sleep") //Special case for magic fangs of sleep
                m_attack = extra + m_name + " " + m_weapon->getAction() + " magic fangs at " + opponent->name() + " and misses.\n";
           
            m_attack = extra + m_name + " " + m_weapon->getAction() + " " + m_weapon->name() + " at " + opponent->name() + " and misses.\n";
        }
    }    
}

bool Actors::teleport(int r, int c)     //Teleports the actor to a valid location on the map
{
    if (m_dungeon->getCharAt(r, c) == ' ' || m_dungeon->getCharAt(r, c) == '?' ||
        m_dungeon->getCharAt(r, c) == ')' || m_dungeon->getCharAt(r, c) == '>' ||
        m_dungeon->getCharAt(r, c) == '&')
    {
        m_row = r;
        m_col = c;
        return true;
    }
    return false;
}

bool Actors::isPlayerAt(int row, int col) const     //Checks to see if the player is at the defined coordinates
{
    if (m_dungeon->getCharAt(row, col) == '@')
    {
        return true;
    }
    return false;
}

bool Actors::moveMonsters(int endRow, int endCol, int smell)        //Moves the trivial monsters such as Snakewomen and Bogeymen (NON RECURSIVE)
{
    m_attack = "";      //Empty string for the attack display
    if (isAsleep())     //If the monster is asleep, decrement its sleep by 1 and do nothing else
    {
        addSLP(-1);
        return false;
    }
    int distanceToMove = abs(m_row-endRow) + abs(m_col-endCol);     //Calculates the distance between the player and monster

    if (distanceToMove > smell)     //If this distance is ever greater than the smell range, then the monster can't move
    {
        return false;
    }
    else
    {
        if (m_dungeon->getCharAt(m_row-1, m_col) != '#' && !m_dungeon->MonstersAt(m_row-1, m_col))      //If the North position is valid
        {
            if ((abs(endRow-(m_row-1)) + abs(endCol-m_col)) <= distanceToMove)     //Make sure the move gets us closer to the player
            {
                if (((m_row-1) == endRow && m_col == endCol)) //We have gotten to the player, so attack
                {
                    attack(m_dungeon->player());
                    return true;
                }
                m_row--;    //Update the location of the monster
                return true;
            }
        }
 
        if (m_dungeon->getCharAt(m_row+1, m_col) != '#' && !m_dungeon->MonstersAt(m_row+1, m_col))  //If the South position is valid
        {
            if ((abs(endRow-(m_row+1)) + abs(endCol-m_col)) <= distanceToMove)     //Make sure the move gets us closer to the player
            {
                if (((m_row+1) == endRow && m_col == endCol)) //We have gotten to the player, so attack
                {
                    attack(m_dungeon->player());
                    return true;
                }
                m_row++;    //Update the location of the monster
                return true;
            }
        }
        if (m_dungeon->getCharAt(m_row, m_col-1) != '#' && !m_dungeon->MonstersAt(m_row, m_col-1))  //If the West position is valid
        {
            if ((abs(endRow-m_row) + abs(endCol-(m_col-1))) <= distanceToMove)     //Make sure the move gets us closer to the player
            {
                if ((m_row == endRow && (m_col-1) == endCol)) //We have gotten to the player, so attack
                {
                    attack(m_dungeon->player());
                    return true;
                }
                m_col--;    //Update the location of the monster
                return true;
            }
        }
        if (m_dungeon->getCharAt(m_row, m_col+1) != '#' && !m_dungeon->MonstersAt(m_row, m_col+1))  //If the East position is valid
        {
            if ((abs(endRow-m_row) + abs(endCol-(m_col+1))) <= distanceToMove)     //Make sure the move gets us closer to the player
            {
                if ((m_row == endRow && (m_col+1) == endCol)) //We have gotten to the player, so attack
                {
                    attack(m_dungeon->player());
                    return true;
                }
                m_col++;    //Update the location of the monster
                return true;
            }
        }
    }
    return false;
}


//////////////////////////////////////////////////////////////////////
//                            PLAYER                                //
//////////////////////////////////////////////////////////////////////

void Player::cheat()    //Cheat function, used for debugging purposes
{
    if (isAsleep())     //If we are asleep, we cant cheat
    {
        addSLP(-1);
        return;
    }
    int hp_amount = 50-getHP();     //Sets the health to 50
    addHP(hp_amount);
    int str_amount = 9-getSTRP();   //Sets the strength to 9
    addSTRP(str_amount);
}

void Player::inventory(char key)       //Manages everything to do with our inventory
{
    int t_r = 1 + randInt(MAXROWS+1);   //Random row and col for the teleportation scroll
    int t_c = 1 + randInt(MAXCOLS+1);
    
    if (isAsleep())     //If we are asleep we can't view our inventory
    {
        addSLP(-1);
        return;
    }
    
    clearScreen();
    cout << "Inventory:\n";
    char index = 'a';
    for (int i = 0; i < m_inventory.size(); i++)    //Displays the Inventory as default, for the characters i,r,w
    {
        cout << " " << index << ". " << m_inventory[i]->name() << endl;
        if (index < 'z')
        {
            index++;
            validPickUp = true;
        }
        else    //We can't have more than z items
        {
            validPickUp = false;
            index--;
        }
    }
    char user_input = getCharacter();
    
    switch(key)
    {
        case 'w':
            if (user_input <= index)    //If the users input is within the current index range
            {
                if (user_input >= 'a' && user_input <= 'z')     //And is a value between a and z
                {
                    if (m_inventory[user_input-'a']->getSym() == ')')   //If we choose a weapon
                    {
                        setEquipped(m_inventory[user_input-'a']);   //Change our equipped item, display and cout the message of what you are wielding
                        getDungeon()->display();
                        cout << "You are wielding " << getEquipped()->name() << "\n";
                        return;
                    }
                    else    //Otherwise we try to wield a scroll which is invalid and so we print out an error message
                    {
                        getDungeon()->display();
                        cout << "You can't wield a " << m_inventory[user_input-'a']->name() << endl;
                        return;
                    }
                }
                else    //If we werent within the range, we just display
                {
                    getDungeon()->display();
                    return;
                }
            }
            else
            {
                getDungeon()->display();
                return;
            }
        case 'r':
            if (user_input <= index)    //If the users input is within the current index range
            {
                if (user_input >= 'a' && user_input <= 'z') //And is a value between a and z
                {
                    if (m_inventory[user_input-'a']->getSym() == '?')   //If we choose a scroll
                    {
                        switch (m_inventory[user_input-'a']->getScrollType())   //We get the type of scroll represented by a character
                        {
                            case 't':   //Teleportation scroll
                                while (!teleport(t_r, t_c)) //Keep running until we get a valid teleportation location
                                {
                                    t_r = 1 + randInt(MAXROWS+1);
                                    t_c = 1 + randInt(MAXCOLS+1);
                                }
                                getDungeon()->display();
                                cout << "You read the scroll called " << m_inventory[user_input-'a']->name() << endl;
                                cout << "You feel your body wrenched in space and time.\n";
                                m_inventory.erase(m_inventory.begin() + (user_input-'a'));  //Remove from the inventory and then display the message of said scroll
                                return;
                            case 'h':   //Health scroll
                                addHP(randInt(6) + 3);  //Adds a random amount to the Hit points (3 to 8)
                                getDungeon()->display();
                                cout << "You read the scroll called " << m_inventory[user_input-'a']->name() << endl;
                                cout << "You feel your heart beating stronger.\n";
                                m_inventory.erase(m_inventory.begin() + (user_input-'a'));  //Remove from the inventory and then display the message of said scroll
                                return;
                            case 's':   //Strength scroll
                                addSTRP(randInt(3) + 1);    //Adds a random amount to the Strength points   (1 to 3)
                                getDungeon()->display();
                                cout << "You read the scroll called " << m_inventory[user_input-'a']->name() << endl;
                                cout << "You feel your muscles bulge.\n";
                                m_inventory.erase(m_inventory.begin() + (user_input-'a'));  //Remove from the inventory and then display the message of said scroll
                                return;
                            case 'a':   //Armor scroll
                                addAP(randInt(3) + 1);  //Adds a random amount to the Armor points (1 to 3)
                                getDungeon()->display();
                                cout << "You read the scroll called " << m_inventory[user_input-'a']->name() << endl;
                                cout << "Your armor glows blue.\n";
                                m_inventory.erase(m_inventory.begin() + (user_input-'a'));  //Remove from the inventory and then display the message of said scroll
                                return;
                            case 'd':   //Dexterity scroll
                                addDP(1);   //Adds one to dexterity value
                                getDungeon()->display();
                                cout << "You read the scroll called " << m_inventory[user_input-'a']->name() << endl;
                                cout << "You feel like less of a klutz.\n";
                                m_inventory.erase(m_inventory.begin() + (user_input-'a'));  //Remove from the inventory and then display the message of said scroll
                                return;
                            default:
                                break;
                        }
                    }
                    else    //Otherwise we try to read a weapon, which will produce an error message
                    {
                        getDungeon()->display();
                        cout << "You can't read a " << m_inventory[user_input-'a']->name() << endl;
                        return;
                    }
                }
            }
            else    //We exceed the bounds of the index and so we just return to the display
            {
                getDungeon()->display();
                return;
            }
        default:
            getDungeon()->display();
            break;
    }
}

bool Player::pickUpItem(int row, int col)   //Picks up the item at the relevant location, if it exists at said location
{
    if (isAsleep()) //If we are asleep we cant pick up
    {
        addSLP(-1);
        return false;
    }
    if (getDungeon()->getCharAt(row, col) == '&')   //If the item is a golden statue then we win
    {
        hasWon = true;
        return true;
    }
    if (getDungeon()->getObjectAt(row, col) != nullptr && validPickUp)  //If the object exists at the location, and we still have space in our inventory
    {
        m_inventory.push_back(getDungeon()->getObjectAt(row, col)); //Push it onto our inventory end
        hasPickedUp = true;
        return true;
    }
    else if (!validPickUp)  //Else our inventory is full
    {
        cout << "Your knapsack is full; you can't pick that up.\n";
        return false;
    }
    return false;
}

void Player::heal() // 1 in 10 chance of healing everytime we move, called in the Game::play()
{
    if (trueWithProbability(0.1))
        addHP(1);
}

//////////////////////////////////////////////////////////////////////
//                            BOGEYMAN                              //
//////////////////////////////////////////////////////////////////////

void Bogeymen::dropItem()       //Drops the item that pertains to a Bogeyman
{
    if (getDungeon()->getObjectAt(row(), col()) == nullptr && getDungeon()->getCharAt(row(), col()) != '&') //If there isnt already an item
    {
        if (trueWithProbability(0.1))   //1 in 10 chance of dropping
        {
            getDungeon()->setObjectAt(row(),col(),'B');
        }
    }
}

//////////////////////////////////////////////////////////////////////
//                            SNAKEWOMAN                            //
//////////////////////////////////////////////////////////////////////

void Snakewomen::dropItem() //Drops the item that pertains to a Snakewoman
{
    if (getDungeon()->getObjectAt(row(), col()) == nullptr && getDungeon()->getCharAt(row(), col()) != '&') //If there isnt already an item
    {
        if (trueWithProbability(0.3))   //1 in 3 chance of dropping
        {
            getDungeon()->setObjectAt(row(), col(), 'S');
        }
    }
}
//////////////////////////////////////////////////////////////////////
//                            DRAGON                                //
//////////////////////////////////////////////////////////////////////

bool Dragon::moveMonsters(int endRow, int endCol, int smell)    //Overriden moveMonsters for the Dragon
{
    setAttackedToEmpty();   //The attack string is set to empty
    if (((row()-1) == endRow && col() == endCol)) //We are always at the player is the player is anywhere near us, so attack
    {
        attack(getDungeon()->player());
        return true;
    }
    if (((row()+1) == endRow && col() == endCol))
    {
        attack(getDungeon()->player());
        return true;
    }
    if ((row() == endRow && (col()-1) == endCol))
    {
        attack(getDungeon()->player());
        return true;
    }
    if ((row() == endRow && (col()+1) == endCol))
    {
        attack(getDungeon()->player());
        return true;
    }
    return false;
}

void Dragon::dropItem() //Drop the item pertaining to a Dragon, 100% chance of dropping
{
    if (getDungeon()->getObjectAt(row(), col()) == nullptr && getDungeon()->getCharAt(row(), col()) != '&') //If there isnt an object already there
    {
        getDungeon()->setObjectAt(row(), col(), 'D');
    }
}
//////////////////////////////////////////////////////////////////////
//                            GOBLIN                                //
//////////////////////////////////////////////////////////////////////

void Goblin::dropItem() //Drop the item pertaining to a Goblin
{
    if (getDungeon()->getObjectAt(row(), col()) == nullptr && getDungeon()->getCharAt(row(), col()) != '&') //If there isnt an object already there
    {
        if (trueWithProbability(0.3))   //1 in 3 chance of dropping
        {
            getDungeon()->setObjectAt(row(), col(), 'G');
        }
    }
}


//Code for the recursive goblin move, but not fully functional



void Goblin::setSecondary()     //Copies over the grid from the dungeon
{
    for (int i = 0; i < 18; i++)
    {
        for (int j = 0; j < 70; j++)
        {
            if (getDungeon()->getCharAt(i, j) == '#' || getDungeon()->MonstersAt(i, j))
                secondary[i][j] = -1;
            else
                secondary[i][j] = 1000;
        }
    }
    secondary[this->row()][this->col()] = 0;
}

void Goblin::fillPaths(int curRow, int curCol, int startRow, int startCol)
{
    //Base case:
    bool decide = true;
    for (int i = 0; i < 18; i++)
    {
        for (int j = 0; j<70; j++)
        {
            if (i == startRow && j == startCol)
            {
                continue;
            }
            if (secondary[i][j] != -1 )
            {
                if (secondary[i][j] < 1000)
                {
                    decide = true && decide;
                }
                else
                    decide = false;
            }
        }
    }
    if (decide)
        return;
    
    if (secondary[curRow-1][curCol] != -1)
    {
        if(secondary[curRow-1][curCol] > secondary[curRow][curCol]+1)
        {
            secondary[curRow-1][curCol] = secondary[curRow][curCol] + 1;
            fillPaths(curRow-1, curCol, startRow, startCol);
        }
    }
    if (secondary[curRow+1][curCol] != -1)
    {
        if (secondary[curRow+1][curCol] > secondary[curRow][curCol]+1)
        {
            secondary[curRow+1][curCol] = secondary[curRow][curCol] + 1;
            fillPaths(curRow+1, curCol, startRow, startCol);
        }
    }
    if (secondary[curRow][curCol-1] != -1)
    {
        if (secondary[curRow][curCol-1] > secondary[curRow][curCol]+1)
        {
            secondary[curRow][curCol-1] = secondary[curRow][curCol] + 1;
            fillPaths(curRow, curCol-1, startRow, startCol);
        }
    }
    if (secondary[curRow][curCol+1] != -1)
    {
        if (secondary[curRow][curCol+1] > secondary[curRow][curCol]+1)
        {
            secondary[curRow][curCol+1] = secondary[curRow][curCol] + 1;
            fillPaths(curRow, curCol+1, startRow, startCol);
        }
    }
}

bool Goblin::pathExists(int playerRow, int playerCol, int smell)
{
    if (secondary[playerRow][playerCol] > smell)
        return false;
    return true;
}

void Goblin::traceBack(int curRow, int curCol, int endRow, int endCol)
{
    int tempEndRow = endRow;
    int tempEndCol = endCol;
    while (secondary[curRow][curCol] != 0)
    {
        directionStack.push(tempEndRow);
        directionStack.push(tempEndCol);
        int distance = secondary[curRow-1][curCol];
        int row_temp = curRow-1;
        int col_temp = curCol;
        if (distance > secondary[curRow+1][curCol])
        {
            distance = secondary[curRow+1][curCol];
            row_temp = curRow+1;
            col_temp = curCol;
        }
        if (distance > secondary[curRow][curCol-1])
        {
            distance = secondary[curRow][curCol-1];
            row_temp = curRow;
            col_temp = curCol-1;
        }
        if (distance > secondary[curRow][curCol+1])
        {
            distance = secondary[curRow][curCol+1];
            row_temp = curRow;
            col_temp = curCol+1;
        }
        tempEndRow = row_temp;
        tempEndCol = col_temp;
    }
}

void Goblin::moveGoblin(int startRow, int startCol, int endRow, int endCol, int smell)
{
    fillPaths(startRow, startCol, startRow, startCol);
    if (pathExists(endRow, endCol, smell))
    {
        traceBack(endRow, endCol, startRow, startCol);
    }
    else
        return;
    //move in direction of the top of the stack
    setColTo(directionStack.top());
    directionStack.pop();
    setRowTo(directionStack.top());
    directionStack.pop();
    
    if (startRow == endRow && startCol == endCol)
        attack(getDungeon()->player());
    while (!directionStack.empty())
        directionStack.pop();
}