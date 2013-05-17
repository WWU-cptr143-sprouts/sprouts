/*
This is the difintions file for miscilanious structs
inluding
Coord
Connection
Line
*/
#include <vector>
#ifndef H_structs
class Node;

struct Coord
{
    int x;
    int y;
};

struct Connection
{
    Line* line;
    Node* dest;
};

typedef vector<Coord> Line;
#endif
