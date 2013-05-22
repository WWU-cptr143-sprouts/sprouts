#ifndef H_Game
#define H_Game

#include <vector>
#include <algorithm>
#include "node.h"

using namespace std;

class Game
{
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
        void insertNode(const Node&);
        void insertLine(const Line&);

        ~Game();
};

#endif
