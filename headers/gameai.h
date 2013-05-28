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
        vector<vector<bool>> mTable;
    public:
        GameAI(); //needs to call game constructor;
        int willMakeNEArea(const Node&,const Node&,const Connection&,const Connection&); //check if a given move will result in a non empty new area;
        void populateMList(); //will populate the possibleMoves vector
        void populatemTable(); // populate the mtable table;
        bool aiTurn(); //master function for making the AI have a turn

        ~GameAI();
};
#endif
