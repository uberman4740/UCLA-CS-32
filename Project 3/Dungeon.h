//
//  Dungeon.h
//  Project 3
//
//  Created by Jahan Cherian on 5/12/15.
//  Copyright (c) 2015 Jahan Cherian. All rights reserved.
//

#ifndef Project_3_Dungeon_h
#define Project_3_Dungeon_h

#include <string>
#include <vector>
#include "Actors.h"
using namespace std;

//////////////////////////////////////////////////////////////////////
//                       GLOBAL CONSTANTS                           //
//////////////////////////////////////////////////////////////////////

const int MAXROWS = 18;     //The Max row size
const int MAXCOLS = 70;     //The Max col size

//////////////////////////////////////////////////////////////////////
//                         CLASSES                                  //
//////////////////////////////////////////////////////////////////////

class Room  //Room class that stores the position of the Leftwall (leftmost Col), RightWall (rightmost col), TopWall (upper most row) and BottomWall (lowest row)
{
public:
    Room(int lW, int rW, int tW, int bW)
    : leftWall(lW), rightWall(rW), topWall(tW), bottomWall(bW)
    {};
    //Accessors
    int getCenterCol() const{ return ((leftWall+rightWall)/2); }    //Gets the center col
    int getCenterRow()const { return ((topWall + bottomWall)/2); }  //Gets the center row
    
    bool intersect(Room other)  //Checks if there is an intersection with another room based on the relative position of the walls
    {
        if (leftWall <= other.rightWall && other.leftWall <= rightWall &&
            topWall <= other.bottomWall && other.topWall <= bottomWall)
            return true;
        return false;
    }
    
    bool operator<(Room other) const    //Overload the < operator to be able to sort the rooms in order of their center columns
    {
        return this->getCenterCol() < other.getCenterCol();
    }
    
private:
    int leftWall, rightWall, topWall, bottomWall;
};

class Dungeon   //Dungeon class 
{
public:
    Dungeon(int level);
    ~Dungeon();
    void display() ;
    void makeDungeonMaze(int numRooms);
    void makeARoom(int sr, int sc, int rscale, int cscale);
    bool isWall(int r, int c) const;
    //Acessors
    int row() const { return m_row;}
    int col() const { return m_col;}
    char getCharAt(int r, int c) const { return m_grid[r][c]; }
    int getLevel() const { return m_level;}
    bool MonstersAt(int r, int c) const;
    bool gethasAttacked() const { return hasAttacked; }
    Player* player() const { return m_player; }
    vector<Actors*> getMonsters() { return m_monsters;}
    
    //Mutators
    bool addPlayer(int r, int c);
    int setLevel(int amount);
    bool addMonsters(char symbol, int row, int col);
    bool addGameObjects(char symbol, int row, int col);
    bool addStairCase(int row, int col);
    void setGrid(int row, int col, char symbol);
    GameObjects* getObjectAt(int row, int col);
    void setObjectAt(int row, int col, char monster);
    void removeObjectAt(int row, int col);
    void removeMonster();
    void connectRooms(Room one, Room two);
    void makeLevel(int level);
    void addEverything(int chooser, int level);
    bool addStatue(int row, int col);
    
private:
    int m_level;
    int m_row;
    int m_col;
    Player* m_player;   //Pointer to the player
    vector<Actors*> m_monsters;     //Creates a vector of pointers to monsters, that hold the monsters in the dungeon level
    vector<GameObjects*> m_objects; //Creates a vector of pointers to game objects, that hold all the items that can be picked up in the dungeon level
    vector<Room> VR;    //Holds a vector of rooms per level
    char m_grid[MAXROWS][MAXCOLS];  //The grid that holds the walls, rooms and staircase
    bool hasAttacked;
};


#endif
