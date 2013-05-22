#include <vector>
#include <cstdlib>
#include <algorithm>

#include "area.h"
#include "structs.h"

#ifndef H_Node
#define H_Node
class Game;

using namespace std;

class Node
{
protected:
    Coord loci;
    Areaset* areasets[2];
    Connection connections[3];
    bool open[4];
public:
    //Node();
    Node(Coord, Connection = Connection(), Connection = Connection());
    void walk(vector<Area>& areas, Area history = Area(), Connection* connection = NULL);//Append new circuits/areas to this vector: be sure to check if exits
    Connection* getConnAddr(); //return array connection
    bool dead() const; //true if dead node
    bool vertical() const; // runs on assumption that there are two connections; if we impliment expectsions should thrown one if NOT two noded
    const Coord& getLoci() const {return loci;}
    void setAreasets(Areaset* sets[2]);
    ~Node();
    friend Game;

};

enum dir
{up=0,right,down,left};

#endif
