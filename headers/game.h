#ifndef H_Game
#define H_Game

#include <list>
#include <algorithm>
#include "node.h"

using namespace std;

class Game
{
    // TODO: don't rely on same address locations with vectors
    private:
        list<Area> areas;
        list<Areaset> areasets;
    protected:
        list<Node> nodes;
        list<Line> lines;
    public:
        Game();
        void updateAreas(); //will call node.walk in its process
        bool connectable(const Node&,const Node&) const;
        bool isInArea(const Area&,Coord) const;
        Node& insert(const Node&);
        Line& insert(const Line&);

        ~Game();
};

#endif
