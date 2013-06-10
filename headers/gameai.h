#ifndef H_Gameai
#define H_Gameai

#include <vector>
#include "game.h"

/*class Pmove
{
    public:
        Line aLink;
        Node newNode;
        Line bLink;
};*/

class GameAI : public Game
{
    protected:
        int startingNodes;
        vector<Line> possibleMoves;
        int testUnuseableNodes;
        int wantedAreas;
        bool aiFirst;
    public:
        GameAI(); //needs to call game constructor;
        int notConnectableNodes() const; //check how many nodes are not connectable
        void populateMList(); //will populate the possibleMoves vector
        bool aiTurn(); //master function for making the AI have a turn
        Coord midNode(const Line&) const; //Creates a new node on the line.
        Line createLine(Node*, Node*) const; //Finds a path between two nodes
        bool requiredAreas(bool, int) const;  //Calculates the number of nodes to put the ai in an advantageous position.
        bool validSingleLine(const Line&,Coord,Coord) const; //copy/pasted from GUI
        bool validLine(Coord,Coord) const; // copy/pasted from GUI

        ~GameAI();
};

#endif
