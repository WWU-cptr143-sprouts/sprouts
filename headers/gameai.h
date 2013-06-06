#ifndef H_Gameai
#define H_Gameai

#include <vector>
#include "game.h"

class Pmove
{
    public:
        Line aLink;
        Node newNode;
        Line bLink;
};

class GameAI : public Game
{
    protected:
        bool aiFirst, evenINodes;
        int startingNodes;
        vector<Pmove> possibleMoves;
        vector<vector<bool> > mTable;
    public:
        GameAI(); //needs to call game constructor;
        int willMakeNEArea(const Node&,const Node&,const Connection&,const Connection&); //check if a given move will result in a non empty new area;
        void populateMList(); //will populate the possibleMoves vector
        void populatemTable(); // populate the mtable table;
        bool aiTurn(); //master function for making the AI have a turn
        bool requiredAreas(bool, int);  //Calculates the number of nodes to put the ai in an advantageous position.
        ~GameAI();
};

#endif
