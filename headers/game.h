
#include <vector>
#include "node.h"
#ifndef H_Game
#define H_Game

class Game
{
    protected:
    vector<Node> nodes;
    vector<Area> areas;
    vector<Areaset> areasets;
    vector<Line> lines;
    public:
    void walk(); //will call node.walk in its process
}

#endif
