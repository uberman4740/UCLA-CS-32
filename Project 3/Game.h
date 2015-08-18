// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

// You may add data members and other member functions to this class.
class Dungeon;

class Game
{
public:
	Game(int goblinSmellDistance);
    ~Game();
    int decodeDirection(char dir);  //Decodes character to ARROW_UP, ARROW_DOWN, ARROW_LEFT and ARROW_RIGHT
    void monstersMove();        //Moves the monsters
	void play();    //Allows gameplay
private:
    Dungeon* m_dungeon;    
    int goblinSmellDistance;
};

#endif // GAME_INCLUDED
