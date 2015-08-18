#include "Pit.h"
#include "History.h"
#include "Player.h"
#include "globals.h"
int main()
{
    Pit pt(3, 3);
    pt.addPlayer(1, 1);
    pt.addSnake(1, 2);
    pt.addSnake(1, 2);
    pt.addSnake(2, 2);
    pt.addSnake(2, 3);
    pt.player()->move(RIGHT);
    pt.player()->move(DOWN);
    pt.player()->move(UP);
    pt.player()->move(UP);
    pt.player()->move(LEFT);
    pt.history().display();
}