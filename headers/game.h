#ifndef H_Game
#define H_Game

#include <vector>
#include <algorithm>
#include "node.h"

using namespace std;

class Game
{
    // TODO: don't rely on same address locations  with vectors
    private:
        vector<Area> areas;
        vector<Areaset> areasets;
    protected:
        vector<Node> nodes;
        vector<Line> lines;
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
