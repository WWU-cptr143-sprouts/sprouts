#ifndef H_Node
#define H_Node

#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "area.h"
#include "structs.h"

using namespace std;

class Game;

// Determine if we've gone on a connection by the line instead of the
// connection, which is unidirectional
class LineFind
{
    const Line* line;

    public:
        LineFind(const Line* line) :line(line) { }

        bool operator()(Connection* search)
        {
            return line == search->line;
        }
};

// Thrown if the coordinate at the beginning or end of the line is not
// the coordinate of the node.
class InvalidLine { };

class Node
{
    protected:
        Coord loci;
        Areaset* areasets[2];
        Connection connections[3];
        bool open[4];
    public:
        Node(Coord, Connection = Connection(), Connection = Connection());
        void walk(vector<Area*>& areas, Area history = Area(), Connection* connection = NULL);//Append new circuits/areas to this vector: be sure to check if exits
        Connection* getConnAddr(); //return array connection
        bool dead() const; //true if dead node
        bool vertical() const; // runs on assumption that there are two connections; if we implement exceptions should thrown one if it does NOT have two nodes
        const Coord& getLoci() const { return loci; }
        void setAreasets(Areaset* sets[2]);
        bool addConnection(const Connection&); // adds connection to first available slot, returns false if all used already
        ~Node();
        friend class Game;
        friend ostream& operator<<(ostream&, const Game&);
        friend ostream& operator<<(ostream&, const Connection&);
    private:
        void updateOpen(); // update open array after adding a connection
};

enum Dir
{
    Up=0, Right, Down, Left
};

// This is here since we want to print out the coordinate of the Node
// which requires that the class has been declared.
ostream& operator<<(ostream&,const Connection&);

#endif
