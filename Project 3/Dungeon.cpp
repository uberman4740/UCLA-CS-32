//
//  Dungeon.cpp
//  Project 3
//
//  Created by Jahan Cherian on 5/12/15.
//  Copyright (c) 2015 Jahan Cherian. All rights reserved.
//

#include <stdio.h>
#include "Dungeon.h"
#include "Actors.h"
#include <string>
#include <algorithm>
#include "GameObjects.h"
#include "utilities.h"
#include <iostream>
using namespace std;

Dungeon::Dungeon(int level)
: m_level(level), m_player(nullptr)
{}

Dungeon::~Dungeon()
{
    for (vector<Actors*>::iterator p = m_monsters.begin(); p != m_monsters.end();)
    {
        delete *p;
        p= m_monsters.erase(p);
    }
    for (vector<GameObjects*>::iterator q = m_objects.begin(); q != m_objects.end();)
    {
        delete *q;
        q = m_objects.erase(q);
    }
    VR.clear();
    delete m_player;
    
}

void Dungeon::makeARoom(int sr, int sc, int rscale, int cscale) //Make a room, based on the starting points and the scaling values
{
    if (rscale > 0 && cscale > 0)   //If our scales are positive, then just build the room normally
    {
        for (int r = sr ; r < sr+rscale; r++)
            for (int c = sc; c < sc+cscale; c++)
            {
                if (!isWall(r, c))
                    m_grid[r][c] = ' ';
            }
    }
    else if (rscale < 0 && cscale > 0)  //If our scaling in the row direction is negative, flip the room up
    {
        for (int r = sr + rscale ; r < sr; r++)
            for (int c = sc; c < sc+cscale; c++)
            {
                if (!isWall(r, c))
                    m_grid[r][c] = ' ';
            }
    }
    else if (rscale > 0 && cscale < 0)  //If our scaling in the column direction is negative flip the room left
    {
        for (int r = sr ; r < sr+rscale; r++)
            for (int c = sc + cscale; c < sc; c++)
            {
                if (!isWall(r, c))
                    m_grid[r][c] = ' ';
            }
    }
    else {      //If both the scalings are negative, then flip the entire room to its mirror image
        for (int r = sr+rscale ; r < sr; r++)
            for (int c = sc+cscale; c < sc; c++)
            {
                if (!isWall(r, c))
                    m_grid[r][c] = ' ';
            }
    }
}

bool Dungeon::isWall(int r, int c) const    //Check if we are at the boundary of the grid
{
    if (r == MAXROWS-1 || c == MAXCOLS - 1 || r == 0 || c == 0)
    {
        return true;
    }
    return false;
}


void Dungeon::makeDungeonMaze(int roomNum)  //Creates the rooms and corridors
{
    for (int i = 0; i < roomNum; i++)   //For the random number of rooms, create random start locations
    {
        
        int sr = randInt(MAXROWS - 2) + 1;
        int sc = randInt(MAXCOLS - 2) + 1;
    
        int rscale = randInt(3) + 4;    //Choose random scalings
        int cscale = randInt(8) + 5;
        
        //We break the grid up into four quadrants, which prevents the rooms from being generated out of bounds
        
        if (sc > MAXCOLS/2)     //If our startlocation exceeds the vertical halfway line, make cscale negative so as to flip the room left
        {
            cscale = - cscale;
        }
        if (sr > MAXROWS/2)     //If our startlocation exceeds the horizontal halfway line, make rscale negative so as to flip the room up
        {
            rscale = - rscale;
        }
        
        //For the following four if conditions, we look at the scalings and determine the walls based on these scalings, and then push the room
        //into our vector of rooms
        
        if (rscale > 0 && cscale > 0)
        {
            int lw = sc;
            int rw = sc + cscale;
            int tw = sr;
            int bw = sr + rscale;
            Room r = Room(lw, rw, tw, bw);
            VR.push_back(r);
        }
        else if (rscale < 0 && cscale > 0)
        {
            int lw = sc;
            int rw = sc + cscale;
            int tw = sr + rscale;
            int bw = sr;
            Room r = Room(lw, rw, tw, bw);
            VR.push_back(r);
        }
        else if (rscale > 0 && cscale < 0)
        {
            int lw = sc + cscale;
            int rw = sc;
            int tw = sr;
            int bw = sr + rscale;
            Room r = Room(lw, rw, tw, bw);
            VR.push_back(r);
        }
        else if (rscale < 0 && cscale < 0)
        {
            int lw = sc + cscale;
            int rw = sc;
            int tw = sr + rscale;
            int bw = sr;
            Room r = Room(lw, rw, tw, bw);
            VR.push_back(r);
        }
        
        bool intersect = false;
        
        for (int j = 0; j < i; j++) //Now go through our vector of rooms, and see if any intersect
        {
            if (VR[j].intersect(VR[i]))
            {
                VR.pop_back();      //If we have an intersection, then discard the new room and start over
                intersect = true;
                i--;
                break;
            }
        }
        if (!intersect) //If we never had an intersection, then build the room
        {
            makeARoom(sr, sc, rscale, cscale);
        }
    }
    sort(VR.begin(), VR.end()); //Sort the rooms from the leftmost centercolumn to the rightmost centercolumn

    for (int m = 0; m < VR.size(); m++)
    {
        if (m != VR.size()-1)
        {
            connectRooms(VR[m], VR[m+1]);   //For each room, connect the centers (going left to right on the grid)
        }
    }
}
bool Dungeon::addPlayer(int r, int c)
{
    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;
    
    if (getCharAt(r, c) == ' ' && !MonstersAt(r, c))    //If we can be generated in a valid position, create the player
    {
        m_player = new Player(this, r, c, 20, 2, 2, 2, 0);
        return true;
    }
    return false;
}

bool Dungeon::addMonsters(char symbol, int row, int col)
{
    if (getCharAt(row, col) != '#' && (player()->row() != row && player()->col() != col) && !MonstersAt(row, col) && getCharAt(row, col)!= '&')  //Find valid positions
    {
        switch (symbol) //Based on the symbol of the monster, we add that respective monster to the grid
        {
            case 'S':
                m_monsters.push_back(new Snakewomen(this, row, col)); 
                break;
            case 'G':
                m_monsters.push_back(new Goblin(this, row, col));
                break;
            case 'B':
                m_monsters.push_back(new Bogeymen(this, row, col));
                break;
            case 'D':
                m_monsters.push_back(new Dragon(this, row, col));
            default:
                break;
        }
        return true;
    }
    return false;
}


bool Dungeon::addGameObjects(char symbol, int row, int col)
{
    if (getCharAt(row, col) == ' ' && getCharAt(row, col) != '&') //We make sure we have a valid position
    {
        switch (symbol) //Based on the symbol ) or ? we add weapons and scrolls respectively
        {
            case ')':
                switch (randInt(3)) //Picks random weapons to place (can't place magic axe or magic fangs of sleep)
                {
                    case 0:
                        m_objects.push_back(new Mace(row,col));
                        break;
                    case 1:
                        m_objects.push_back(new ShortSword(row,col));
                        break;
                    case 2:
                        m_objects.push_back(new LongSword(row,col));
                        break;
                    default:
                        break;
                }
                break;
            case '?':
            {
                switch (randInt(4)) //Picks random scrolls to place (can't place teleportation)
                {
                    case 0:
                        m_objects.push_back(new Strength(row,col));
                        break;
                    case 1:
                        m_objects.push_back(new Health(row,col));
                        break;
                    case 2:
                        m_objects.push_back(new Armor(row,col));
                        break;
                    case 3:
                        m_objects.push_back(new Dexterity(row,col));
                        break;
                    default:
                        break;
                }
            }
            default:
                break;
        }
        return true;
    }
    return false;
}

bool Dungeon::addStairCase(int row, int col)    //Adds a staircase at a valid location
{
    if (getCharAt(row, col) == ' ' )
    {
        setGrid(row, col, '>');
        return true;
    }
    return false;
}

bool Dungeon::addStatue(int row, int col)   //Adds a statue at a valid location
{
    if (getCharAt(row, col) == ' ')
    {
        setGrid(row, col, '&');
        return true;
    }
    return false;
}
int Dungeon::setLevel(int amount)   //Increments the level as long as we are within the range
{
    int newLevel = m_level+amount;
    if (newLevel >= 0 && newLevel < 5)
    {
        m_level = newLevel;
        return newLevel;
    }
    return -1;  //Error!
}

void Dungeon::addEverything(int chooser, int level) //Generates everything based on the level
{
    if (level > 0)  //Only once we are past the first level, we clear out and delete all the monsters and objects from the level
    {
        for (vector<Actors*>::iterator p = m_monsters.begin(); p != m_monsters.end();)
        {
            delete *p;
            p= m_monsters.erase(p);
        }
        for (vector<GameObjects*>::iterator q = m_objects.begin(); q != m_objects.end();)
        {
            delete *q;
            q = m_objects.erase(q);
        }
        VR.clear();     //Clear out the rooms, to be able to generate new ones for the new level
    }
    int numOfRooms = 4 + randInt(6);    //Generate between 4 to 9 random rooms
    makeDungeonMaze(numOfRooms);    //Create the rooms and connect them
    int r = 1 + randInt(MAXROWS - 1);
    int c = 1 + randInt(MAXCOLS - 1);
    if (level == 0)     //If our level is zero, then try to add the player at our random location until it is valid
    {
        while (!addPlayer(r, c))
        {
            r = 1 + randInt(MAXROWS - 1);
            c = 1 + randInt(MAXCOLS - 1);
        }
    }
    else    //Otherwise teleport the player from one level to another
    {
        while (!m_player->teleport(r, c))
        {
            r = 1 + randInt(MAXROWS - 1);
            c = 1 + randInt(MAXCOLS - 1);
        }
    }
    int r2 = 1 + randInt(MAXROWS - 1);
    int c2 = 1 + randInt(MAXCOLS - 1);
    int objectChooser = randInt(4)+1;
    int numObjects = randInt(3)+1;
    for (int i = 0; i < numObjects; i++)
    {
        if (objectChooser == 1 || objectChooser == 2 || objectChooser == 3)
        {
            while(!addGameObjects(')', r2, c2)) //More likely to add a weapon to the grid to begin with
            {
                r2 = 1 + randInt(MAXROWS - 1);
                c2 = 1 + randInt(MAXCOLS - 1);
            }
        }
        else if(objectChooser == 4)
        {
            while(!addGameObjects('?', r2, c2))
            {
                r2 = 1 + randInt(MAXROWS - 1);
                c2 = 1 + randInt(MAXCOLS - 1);
            }
        }
        r2 = 1 + randInt(MAXROWS - 1);
        c2 = 1 + randInt(MAXCOLS - 1);
        objectChooser = randInt(4)+1;
    }
    
    int r3 = 1 + randInt(MAXROWS-1);
    int c3 = 1 + randInt(MAXCOLS-1);
    if (level == 4)     //If we are at level 4 then we add the statue
    {
        while(!addStatue(r3,c3))
        {
            r3 = 1 + randInt(MAXROWS-1);
            c3 = 1 + randInt(MAXCOLS-1);
        }
    }
    int r4 = 1 + randInt(MAXROWS - 1);
    int c4 = 1 + randInt(MAXCOLS - 1);
    int monsterChooser = randInt(chooser);  //The type of monster is dependent on the level
    int numMonsters = 2 + randInt(5*(level+1)); //Add the monsters based on the formula
    for (int m = 0; m < numMonsters; m++)
    {
        if (monsterChooser == 0)
        {
            while (!addMonsters('G', r4, c4))
            {
                r4 = 1 + randInt(MAXROWS - 1);
                c4 = 1 + randInt(MAXCOLS - 1);
            }
        }
        else if (monsterChooser == 1)
        {
            while (!addMonsters('S', r4, c4))
            {
                r4 = 1 + randInt(MAXROWS - 1);
                c4 = 1 + randInt(MAXCOLS - 1);
            }
        }
        else if (monsterChooser == 2)
        {
            while (!addMonsters('B', r4, c4))
            {
                r4 = 1 + randInt(MAXROWS - 1);
                c4 = 1 + randInt(MAXCOLS - 1);
            }
        }
        else if (monsterChooser == 3)
        {
            while (!addMonsters('D', r4, c4))
            {
                r4 = 1 + randInt(MAXROWS - 1);
                c4 = 1 + randInt(MAXCOLS - 1);
            }
        }
        r4 = 1 + randInt(MAXROWS - 1);
        c4 = 1 + randInt(MAXCOLS - 1);
        monsterChooser = randInt(chooser);
    }
    
    int r5 = 1 + randInt(MAXROWS-1);
    int c5 = 1 + randInt(MAXCOLS-1);
    if (level >= 0 && level < 4)    //Add the staircase to a random location for all levels except 4
    {
        while (!addStairCase(r5, c5))
        {
            r5 = 1 + randInt(MAXROWS-1);
            c5 = 1 + randInt(MAXCOLS-1);
        }
    }
}

void Dungeon::makeLevel(int level)
{
    //Construct newMaze
    //Per level:
        //add rooms
        //add player
        //add objects
        //add staircase
        //add the type of monster and the number of monsters respectively
    for (int r = 0; r < MAXROWS; r++)
        for (int c = 0; c < MAXCOLS; c++)
            m_grid[r][c] = '#';
    
    if (level == 0)
    {
        addEverything(2, 0);
    }
    if (level == 1)
    {
        addEverything(2, 1);
    }
    if (level == 2)
    {
        addEverything(3, 2);
    }
    if (level == 3)
    {
        addEverything(4, 3);
    }
    if (level == 4)
    {
        addEverything(4, 4);
    }
}

void Dungeon::display()
{
    char l_grid[MAXROWS][MAXCOLS];      //Create a local grid that copies the m_grid
    for (int i = 0; i < MAXROWS; i++)
    {
        for (int j = 0; j < MAXCOLS; j++)
        {
            l_grid[i][j] = m_grid[i][j];
        }
    }
    if (m_objects.size() > 0) {     //Add the objects around the grid
        for (int i = 0; i < m_objects.size(); i++)
        {
            char& gridObj = l_grid[m_objects[i]->getRow()][m_objects[i]->getCol()];
            if (l_grid[m_objects[i]->getRow()][m_objects[i]->getCol()] != '#')
            {
                gridObj = m_objects[i]->getSym();
            }
        }
    }
    if (m_monsters.size() > 0)      //Add the monsters around the grid
    {
        for (int k = 0; k < m_monsters.size(); k++)
        {
            char& gridMonst = l_grid[m_monsters[k]->row()][m_monsters[k]->col()];
            if (l_grid[m_monsters[k]->row()][m_monsters[k]->col()]) {
                gridMonst = m_monsters[k]->symbol();
            }
        }
    }
    if (m_player != NULL)       //Finally add the player
    {
        char& gridChar = l_grid[m_player->row()][m_player->col()];
        if (l_grid[m_player->row()][m_player->col()] != '#')
            gridChar = m_player->symbol();
    }
    clearScreen();  //Print out the local grid
    for (int r = 0; r < MAXROWS; r++)
    {
        for (int c = 0; c < MAXCOLS; c++)
            cout << l_grid[r][c];
        cout << endl;
    }
    
    //Print out the statistics of the grid

    cout << "Dungeon Level: " << m_level << ", Hit Points: " << m_player->getHP() << ", Armor: " << m_player->getAP() << ", Strength: " << m_player->getSTRP() << ", Dexterity: " << m_player->getDP() << endl;
    cout << endl;
    
    cout << m_player->getAttacked();    //print out the line of the player being attacked
    for (int l = 0; l < m_monsters.size(); l++)
    {
       cout << m_monsters[l]->getAttacked();    //print out the line of the respective monster being attacked
    }

}

void Dungeon::setGrid(int row, int col, char symbol)    //Sets the given location of the grid to the symbol we want
{
    m_grid[row][col] = symbol;
}

bool Dungeon::MonstersAt(int r, int c) const    //Returns true if a monster exists at the given location
{
    for (int i = 0; i < m_monsters.size(); i++)
    {
        if (m_monsters[i]->row() == r && m_monsters[i]->col() == c)
        {
            return true;
        }
    }
    return false;
}

void Dungeon::connectRooms(Room one, Room two)
{
    int cr = one.getCenterRow();
    int cc = one.getCenterCol();
    while (cc != two.getCenterCol() )
    {
        setGrid(cr, cc, ' ');
        cc++;
    }
    if (one.getCenterRow() > two.getCenterRow()) //The room we want to connect to is below us, so turn 90 degrees down
    {
        while (cr != two.getCenterRow())
        {
            setGrid(cr, cc, ' ');
            cr--;
            setGrid(cr, cc, ' ');
        }
    }
    else if (one.getCenterRow() < two.getCenterRow()) //The room we want to connect is above us, so turn 90 degrees up
    {
        while (cr != two.getCenterRow())
        {
            setGrid(cr, cc, ' ');
            cr++;
            setGrid(cr, cc, ' ');
        }
    }
    else    //We are at the location we want to be at
        return;
}

GameObjects* Dungeon::getObjectAt(int row, int col) //Returns the object at the given location if there is one, otherwise gives nullptr
{
    for (int i = 0; i < m_objects.size(); i++)
    {
        if (m_objects[i]->getRow() == row && m_objects[i]->getCol() == col)
        {
            return m_objects[i];
        }
    }
    return nullptr;
}

void Dungeon::removeObjectAt(int row, int col)   //If the player picks up the object, remove it from our vector of objects
{
    if (m_player->getPickedUp())
    {
        for (vector<GameObjects*>::iterator p = m_objects.begin(); p != m_objects.end();)
        {
            if (*p == getObjectAt(row, col))
            {
               // delete *p;
               p = m_objects.erase(p);
            }
            else
            {
                p++;
            }
        }
    }
}

void Dungeon::removeMonster()   //If we kill a monster, then we remove it and call its destructor which makes it drop an item
{
    for (vector<Actors*>::iterator p = m_monsters.begin(); p != m_monsters.end();)
    {
        if ((*p)->isDead())
        {
            delete *p;
            p = m_monsters.erase(p);
        }
        else
            p++;
    }
}

void Dungeon::setObjectAt(int row, int col, char monster)   //Based on the monster, we drop its respective item
{
    int scrollChooser = randInt(4);
    int weaponChooser = randInt(2);
    switch (monster)
    {
        case 'B':
            m_objects.push_back(new MagicAxe(row,col));
            break;
        case 'S':
            m_objects.push_back(new MagicFangs(row,col));
            break;
        case 'D':
            switch (scrollChooser)
        {
            case 0:
                m_objects.push_back(new Teleportation(row,col));
                break;
            case 1:
                m_objects.push_back(new Strength(row,col));
                break;
            case 2:
                m_objects.push_back(new Health(row,col));
                break;
            case 3:
                m_objects.push_back(new Armor(row,col));
                break;
            case 4:
                m_objects.push_back(new Dexterity(row,col));
                break;
            default:
                break;
        }
            break;
        case 'G':
            switch (weaponChooser)
            {
                case 0:
                    m_objects.push_back(new MagicAxe(row,col));
                    break;
                case 1:
                    m_objects.push_back(new MagicFangs(row,col));
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}