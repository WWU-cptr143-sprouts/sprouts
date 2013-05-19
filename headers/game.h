#include <algorithms>
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
    Game();
    void updateAreas(); //will call node.walk in its process
    bool connectable(const Node&,const Node&) const;
    bool isInArea(const Area&,const Coord&) const;
    ~Game();
}

#endif
