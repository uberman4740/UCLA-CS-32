//
//  Actors.h
//  Project 3
//
//  Created by Jahan Cherian on 5/12/15.
//  Copyright (c) 2015 Jahan Cherian. All rights reserved.
//

#ifndef Project_3_Actors_h
#define Project_3_Actors_h

#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include "utilities.h"
#include "GameObjects.h"
using namespace std;

//////////////////////////////////////////////////////////////////////
//                       GLOBAL CONSTANTS                           //
//////////////////////////////////////////////////////////////////////

const int MAXPOINTS = 99;   //The max value any points can take
const int MINPOINTS = 0;    //The smallest value any points can take

class Dungeon;

//////////////////////////////////////////////////////////////////////
//                         CLASSES                                  //
//////////////////////////////////////////////////////////////////////


class Actors
{
public:
    Actors(Dungeon* dung, int row, int col, int hp, int strp, int ap, int dp, int slp, char symbol, string name)
    : m_dungeon(dung), m_row(row), m_col(col), m_hp(hp), m_strp(strp), m_ap(ap),
      m_dp(dp), m_slp(slp), m_weapon(nullptr), m_symbol(symbol), m_name(name)
    {};
    
    //Acessors
    int getHP() const { return m_hp; }
    int getSTRP() const { return m_strp; }
    int getAP() const { return m_ap; }
    int getDP() const { return m_dp; }
    int getSLP() const { return m_slp; }
    GameObjects* getEquipped(){ return m_weapon;}
    int row() const { return m_row;}
    int col() const {return m_col;}
    bool isPlayerAt(int row, int col) const;
    string getAttacked() const { return m_attack; }
    Dungeon* getDungeon() { return m_dungeon; }
    string name() const { return m_name; }
    char symbol() const { return m_symbol; }
    bool isDead()
    {
        if(m_hp <= 0)
            return m_isDead = true;
        return m_isDead = false;
    }
    bool isAsleep() const
    {
        if (m_slp > 0)
            return true;
        return false;
    }
    
    //Mutators
    int setRow(int amount) { return m_row += amount; }
    int setCol(int amount) { return m_col += amount; }
    void setRowTo(int newRow) { m_row = newRow; }
    void setColTo(int newCol) { m_col = newCol; }
    int addHP(int amount)
    {
        if((m_hp+amount) <= MAXPOINTS)
            return m_hp += amount;
        m_hp = MAXPOINTS;
        return m_hp;
    }
    int addSTRP(int amount)
    {
        if((m_strp + amount) <= MAXPOINTS)
            return m_strp += amount;
        m_strp = MAXPOINTS;
        return m_strp;
    }
    int addAP(int amount)
    {
        if((m_ap + amount) <= MAXPOINTS)
            return m_ap += amount;
        m_ap = MAXPOINTS;
        return m_ap;
    }
    int addDP(int amount)
    {
        if((m_dp + amount) <= MAXPOINTS)
            return m_dp += amount;
        m_dp = MAXPOINTS;
        return m_dp;
    }
    int addSLP(int amount)
    {
        if ((m_slp+amount) <= 9)
            return m_slp += amount;
        m_slp = 9;
        return m_slp;
    }
    string setAttackedToEmpty()
    {
        m_attack = "";
        return m_attack;
    }
    bool teleport(int r, int c);
    void setEquipped(GameObjects* newWeapon) { m_weapon = newWeapon;}  
    bool setDead() { return m_isDead = true; }
    bool putToSleep(Actors* opponent);
    bool directionToDeltas(int dir, int& rowDelta, int& colDelta);
    
    //Virtual functions
    virtual void attack(Actors* opponent);
    virtual void dropItem(){};
    virtual void move(int dir);
    virtual bool moveMonsters(int endRow, int endCol, int smell);
    virtual ~Actors(){ delete m_weapon; }
    virtual void moveGoblin(int startRow, int startCol, int endRow, int endCol, int smell) {};

private:
    Dungeon* m_dungeon;
    GameObjects* m_weapon;
    int m_hp, m_strp, m_ap, m_dp, m_slp;
    char m_symbol;
    string m_name;
    string m_attack;
    int m_row, m_col;
    bool m_isDead;
};

class Player : public Actors        //Player is derived from Actors
{
public:
    Player(Dungeon* dung,int row, int col, int hp, int strp, int ap, int dp, int slp)
    : Actors(dung,row, col, 20, 2, 2, 2, 0,'@',"Player"), hasWon(false)
    {
        setEquipped(new ShortSword());          //The equipped weapon is defaulted to shortsword
        m_inventory.push_back(getEquipped());   //Push the equipped weapon into our inventory
        validPickUp = true;     //The pickup is defaulted to true, the only time we cant pick up is when our inventory is full
    };
    //Accessors
    bool getPickedUp() const { return hasPickedUp; }    //Returns true if we pick up an object
    bool getWon() const { return hasWon; }      //Returns true if we win
    
    //Mutators
    void inventory(char input);
    virtual ~Player(){};
    void cheat();
    bool pickUpItem(int row, int col);
    void heal();
    
private:
    vector<GameObjects*> m_inventory;
    bool hasPickedUp;
    bool validPickUp;
    bool hasWon;
};

class Bogeymen : public Actors      //Bogeyman is derived from Actors
{
public:
    Bogeymen(Dungeon* dung, int row, int col)
    : Actors(dung, row, col, randInt(6) + 5, randInt(2)+2, 2, randInt(2)+2, 0,'B', "Bogeyman")
    {
        setEquipped(new ShortSword());      //Default weapon is set to shortsword
    }
    virtual ~Bogeymen()
    {
        dropItem();     //When we destroy a bogeyman, we drop its respective item
    }
    virtual void dropItem();

private:
    
};
class Dragon : public Actors    //Dragon is derived from Actors
{
public:
    Dragon(Dungeon* dung, int row, int col)
    : Actors(dung, row, col, randInt(6)+20, 4, 4, 4, 0,'D', "Dragon")
    {
        setEquipped(new LongSword());   //Default weapon is set to longsword
    }
    virtual ~Dragon()
    {
        dropItem();     //When we destroy a dragon, we drop its respective item
    }
    virtual bool moveMonsters(int endRow, int endCol, int smell);   //We have to override this function, because a dragon doesnt move
    virtual void dropItem();
    
private:
    
};

class Snakewomen : public Actors    //Snakewomen is derived from Actors
{
public:
    Snakewomen(Dungeon* dung, int row, int col)
    : Actors(dung, row, col, randInt(4)+3, 2, 3, 3, 0, 'S',"Snakewoman")
    {
        setEquipped(new MagicFangs());  //Default weapon is set to Magic Fangs of sleep
    }
    virtual ~Snakewomen()
    {
        dropItem();     //When we destroy a snakewoman, we drop its respective item
    }    
    virtual void dropItem();
    
private:
    
};

class Goblin : public Actors        //Goblin is derived from Actors
{
public:
    Goblin(Dungeon* dung, int row, int col)
    : Actors(dung, row, col, randInt(6)+15, 3, 1, 1, 0, 'G',"Goblin")
    {
        setEquipped(new ShortSword());  //Default weapon is set to shortsword
        setSecondary();
    }
    virtual ~Goblin()
    {
        dropItem();     //When we destroy a goblin, we drop its respective item
    }
    virtual void dropItem();
    
    void setSecondary();      //We set a secondary grid for tracing a Goblins path equal to the grid from the dungeon
    virtual void fillPaths(int curRow, int curCol, int startRow, int startCol);
    virtual bool pathExists(int playerRow, int playerCol, int smell);
    virtual void traceBack(int curRow, int curCol, int endRow, int endCol); //Traces back from the B to A using the shortest path
    virtual void moveGoblin(int startRow, int startCol, int endRow, int endCol, int smell); //Moves the Goblin
    
private:
    int secondary[18][70];
    stack<int> directionStack;
    
};
#endif
