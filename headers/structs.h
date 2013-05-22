/*
 * This is the definitions file for miscellaneous structs including:
 *   Coord
 *   Connection
 *   Line
 */
#include <vector>
#ifndef H_structs
#define H_structs
using namespace std;

class Node;

struct Coord
{
    int x;
    int y;

    Coord() { }
    Coord(int x, int y) :x(x), y(y) { }
};

typedef vector<Coord> Line;

struct Connection
{
    Line* line;
    Node* dest;
    bool exists() const {return (line&&dest);}

    Connection() :line(NULL), dest(NULL) { }
};



#endif
