#include "../headers/game.h"
#include <iostream>

int main()
{
    Game g;
    g.updateAreas();

    Node a(Coord(0,0));
    Node b(Coord(10,10));
    Node c(Coord(20,20));

    g.insertNode(a);
    g.insertNode(b);
    g.insertNode(c);

    g.updateAreas();

    return 0;
}
