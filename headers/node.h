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

// Similar to above but for comparing a vector to a vector pointer
class AreaFind
{
    const Area& area;

    public:
        AreaFind(const Area& area) :area(area) { }

        bool operator()(Area* search)
        {
            return area == *search;
        }
};

// Used in std::min to sort, used to find unique areas
bool LineCmp(Connection* a, Connection* b);

// Thrown if the coordinate at the beginning or end of the line is not
// the coordinate of the node.
class InvalidLine
{
    Line line;

    public:
        InvalidLine(const Line& line) :line(line) { }
        friend ostream& operator<<(ostream&,const InvalidLine&);
};

// Thrown if we try to have a 90 degree corner when only two connections
// instead of coming in at 180 degrees like our design calls for.
class InvalidCorner { };

// Tried entering a node twice from the same direction. This means that
// somewhere line collision detection is not quite functioning.
class NodeEntryCollision { };

class Node
{
    enum Dir
    {
        Up=0, Right, Down, Left
    };

    protected:
        Coord loci;
        Areaset* areasets[2];
        Connection connections[3];
        bool open[4];
    public:
        Node(Coord, Connection = Connection(), Connection = Connection());
        //Append new circuits/areas to this vector: be sure to check if exists
        void walk(vector<Area*>&);
        void walk(vector<Area*>&, Area, Connection*, Node*);
        Connection* getConnAddr(); //return array connection
        bool dead() const; //true if dead node
        bool vertical() const; // runs on assumption that there are two connections; if we implement exceptions should thrown one if it does NOT have two nodes
        const Coord& getLoci() const { return loci; }
        void setAreasets(Areaset* sets[2]);
        bool addConnection(const Connection&); // adds connection to first available slot, returns false if all used already
        int conCount() const; // Number of connections

        // Determine which sides you can connect on. Note this assumes that x
        // increases from left to right and y increases from top to bottom.
        // (0,0) is the top left in most graphical toolkits.
        bool openUp()    const { return open[Up];    }
        bool openRight() const { return open[Right]; }
        bool openDown()  const { return open[Down];  }
        bool openLeft()  const { return open[Left];  }

        ~Node();
        friend class Game;
        friend ostream& operator<<(ostream&, const Game&);
        friend ostream& operator<<(ostream&, const Connection&);
    private:
        void updateOpen(); // update open array after adding a connection
};

// This is here since we want to print out the coordinate of the Node
// which requires that the class has been declared.
ostream& operator<<(ostream&,const Connection&);

#endif
