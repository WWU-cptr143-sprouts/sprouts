/*
 * This is the definitions file for miscellaneous structs including:
 *   Coord
 *   Connection
 *   Line
 */
#ifndef H_structs
#define H_structs

#include <vector>

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
    bool exists() const { return (line&&dest); }

    Connection() :line(NULL), dest(NULL) { }
    Connection(Line* l, Node* d) :line(l), dest(d) { }
};

#endif
