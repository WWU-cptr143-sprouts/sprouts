#include <vector>

#include "area.h"
#include "structure.h"

#ifndef H_Node
#define H_Node

using namespace std;

class Node
{
protected:
    Coord loci;
    Areaset areaset[3];
    Connection connections[3];
    bool open[4];
public:
    Node();
    void walk(vector<Area>& areas); //Append new circuits/areas to this vector
    Connection* getConnAddr(); //return array connection
};

#endif
