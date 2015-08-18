//
//  GameObjects.h
//  Project 3
//
//  Created by Jahan Cherian on 5/12/15.
//  Copyright (c) 2015 Jahan Cherian. All rights reserved.
//

#ifndef Project_3_GameObjects_h
#define Project_3_GameObjects_h

#include <string>
#include "utilities.h"


class Actors;

using namespace std;
class GameObjects
{
public:
    GameObjects(string name, char sym, string action, int dex, int dmg) //Start off weapon
    : m_name(name), m_sym(sym), m_action(action), bonusDex(dex), bonusDamage(dmg)
    {};
    GameObjects(string name, int r, int c, char sym, string action, int dex, int dmg)   //For weapons
    : m_name(name), m_row(r), m_col(c), m_sym(sym), m_action(action), bonusDex(dex), bonusDamage(dmg)
    {};
    GameObjects(string name, int r, int c, char sym, string action, char type)   //For scrolls
    : m_name(name), m_row(r), m_col(c), m_sym(sym), m_action(action), m_type(type)
    {};
    GameObjects(string name, int r, int c, char sym)    //For statue
    : m_name(name), m_row(r),m_col(c),m_sym(sym)
    {};
    virtual ~GameObjects(){};
    
    //Accessors
    string getAction() { return m_action; }
    string name() { return m_name;}
    int getRow() { return m_row;}
    int getCol() { return m_col;}
    char getSym() { return m_sym;}
    char getScrollType() {return m_type; }
    bool isScroll() const {
        if (m_sym == '?')
        {
            return true;
        }
        return false;
    }
    int getbonusDex() {return bonusDex;}
    int getbonusDamage() {return bonusDamage;}
    
private:
    string m_name;
    string m_action;
    int m_row;
    int m_col;
    int bonusDex, bonusDamage;
    char m_sym, m_type;
    bool m_isScroll;
    
};

//Weapons

class Mace : public GameObjects
{
public:
    Mace()
    :GameObjects("mace",')',"swings", 0,2)
    {};
    Mace(int r, int c)
    : GameObjects("mace", r, c, ')',"swings", 0,2)
    {};
    ~Mace(){};
private:
};

class ShortSword : public GameObjects
{
public:
    ShortSword()
    :GameObjects("short sword", ')', "slashes", 0, 2)
    {};
    ShortSword(int r, int c)
    : GameObjects("short sword", r, c, ')', "slashes", 0, 2)
    {};
    ~ShortSword(){};
};

class LongSword : public GameObjects
{
public:
    LongSword()
    :GameObjects("long sword",')', "swings", 2, 4)
    {};
    LongSword(int r, int c)
    : GameObjects("long sword", r, c, ')', "swings", 2, 4)
    {};
    ~LongSword(){};
};

class MagicAxe : public GameObjects
{
public:
    MagicAxe()
    : GameObjects("magic axe", ')', "chops", 5, 5)
    {};
    MagicAxe(int r, int c)
    : GameObjects("magic axe", r, c, ')', "chops", 5, 5)
    {};
    ~MagicAxe(){};
};

class MagicFangs : public GameObjects
{
public:
    MagicFangs()
    : GameObjects("magic fangs of sleep",')', "strikes", 3, 2)
    {};
    MagicFangs(int r, int c)
    : GameObjects("magic fangs of sleep", r, c, ')', "strikes", 3, 2)
    {};
    ~MagicFangs(){};
};

//Scrolls

class Teleportation : public GameObjects
{
public:
    Teleportation(int r, int c)
    : GameObjects("scroll of teleportation", r, c, '?', "You feel your body wrenched in space and time.", 't')
    {};
    ~Teleportation(){};
};

class Strength : public GameObjects
{
public:
    Strength(int r, int c)
    : GameObjects("scroll of raise strength", r, c, '?', "Your muscles bulge.", 's')
    {};
    ~Strength(){};
};
class Health : public GameObjects
{
public:
    Health(int r, int c)
    : GameObjects("scroll of enhance health", r, c, '?', "You feel your heart beating stronger.", 'h')
    {};
    ~Health(){};
};
class Armor : public GameObjects
{
public:
    Armor(int r, int c)
    : GameObjects("scroll of improve armor", r, c, '?', "Your armor glows blue.", 'a')
    {};
    ~Armor(){};
};

class Dexterity : public GameObjects
{
public:
    Dexterity(int r, int c)
    : GameObjects("scroll of enhance dexterity", r, c, '?', "You feel like less of a klutz.", 'd')
    {};
    ~Dexterity(){};
};
#endif
