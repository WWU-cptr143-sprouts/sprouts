/*
*
* These implimations likely are not complete
*/
#include "headers/node.h"

Node:Node(Coord point)
{
    loci=point;
    areasets[0] =0;
    areasets[1] =0;
}

Connection* Node::getConnAddr()
{
    return connections;
}

bool Node::dead() const
{
    return (connections[2]);
}
bool Node::vertical() const
{
    return !open[up]; // if the node has a line coming in from the top then it is vertical
}
void Node::walk(vector<Area>& areas)
{
    //to be implimented
}

void Node::setAreasets(Areaset* sets[2])
{
    areasets[0]=sets[0];
    areasets[1]=sets[1];
}
