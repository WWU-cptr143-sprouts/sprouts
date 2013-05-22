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
};

typedef vector<Coord> Line;

struct Connection
{
    Line* line;
    Node* dest;
    bool exists() const {return (line&&dest);}
};



#endif
