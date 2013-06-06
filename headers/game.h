/*
*   TODO:
*   need to add a game class copy constructor
*   implement tests of (in)equality operator for Areas
*   in a valid line function only check lines in the areasets of the start node
*       in the process keep track of & of checked lines, don't recheck
*
*   TONEVERDO list (wish list)
*   added a function to check validity of current game data state
*/

#ifndef H_Game
#define H_Game

#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include "node.h"
#include "structs.h"

using namespace std;

// Used for std::find since our vectors contain pointers
template<class T> class PointerFind
{
    public:
        const T& item;

        PointerFind(const T& item) :item(item) { }

        bool operator()(const T* search) const
        {
            return item == *search;
        }
};

// We'll throw this when trying to use connectable() with outdated areasets
class AreasOutdated { };

// Thrown in doMove() if the center point isn't on the line or the line
// doesn't end in two nodes (or runs through more than two).
class InvalidMove { };

// Thrown when two nodes aren't connectable when put into doMove()
class NotConnectable { };

class Game
{
    // Vectors of addresses since addresses of an element in a vector will
    // change as it grows
    private:
        bool updated;
        int moveCount;
        vector<Area*> areas;
    protected:
        vector<Areaset*> areasets;  // now protected instead of private for use in currentAreas() function
        vector<Node*> nodes;
        vector<Line*> lines;
    public:
        Game();
        Game(const Game&);
        void updateAreas(); //will call node.walk in its process
        void doMove(const Line&, Coord middle); // This is the function you'll use a LOT.
        int moves() const; // Returns how many times doMove has been called
        bool connectable(const Node&,const Node&) const;
        bool isInArea(const Area&,Coord) const;
        bool gameEnded() const; // Can any nodes be connected still?
        // Needed for initializing nodes on the screen
        Node& insertNode(Coord, Connection = Connection(), Connection = Connection());
        
        // Make this private? Only thing that uses it is the test suite?
        Line& insertLine(const Line&);

        // Used for debugging
        friend ostream& operator<<(ostream&, const Game&);
        friend class TestSuite;
    private:
        void clearAreas(); // empty areas/areasets and delete items pointed to
        void deleteLastNode(); // Undo last add, used in doMove
    public:
        ~Game();
};

ostream& operator<<(ostream&, const Game&);

#endif
