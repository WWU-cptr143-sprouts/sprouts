#ifndef H_Gameai
#include "headers/game.h"


class Pmove
{
    public:
    Connection aLink;
    Node newNode;
    Connection bLink;
};

class GameAI : public Game
{
    protected:
        bool aiFirst, evenINodes;
        int startingNodes;
        vector<Pmoves> possibleMoves;
    public:
        GameAI();
        int willMakeNEArea(const Node&,const Node&,const Connection&,const Connection&); //check if a given move will result in a non empty new area;

        ~GameAI();
};
#endif
