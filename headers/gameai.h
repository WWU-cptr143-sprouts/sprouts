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
        bool aiFirst;
        int startingNodes;
        vector<Pmove> possibleMoves;
        vector<vector<bool> > mTable;
        int testMoveAreas, wantedAreas;
    public:
        GameAI(); //needs to call game constructor;
        int notConnectableNodes() const; //check how many nodes are not connectable
        void populateMList(); //will populate the possibleMoves vector
        void populatemTable(); // populate the mtable table;
        bool aiTurn(); //master function for making the AI have a turn
        void doPmove(Pmove); //Calls doMove from
        Pmove createLine(Node&, Node&);
        bool requiredAreas(bool, int);  //Calculates the number of nodes to put the ai in an advantageous position.

        ~GameAI();
};

#endif
