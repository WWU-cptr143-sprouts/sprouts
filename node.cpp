/*
*
* These implimations likely are not complete
*/
#include "headers/node.h"

Node::Node(Coord point)
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
    return (connections[2].exists());
}

bool Node::vertical() const
{
    return !open[up]; // if the node has a line coming in from the top then it is vertical
}

// Note: we could pass in history by reference if we delete added entries at the end of each
// itertion
void Node::walk(vector<Area>& areas, Area history, Connection* connection)
{
    // Add this to the history if we're not already on the initial iteration
    if (connection)
        history.push_back(connection);
    // We have a circuit/loop if we're back to the start node
    if (connection->dest == this)
    {
        //rotate the area to allow for uniquness comparision
        Area::iterator ind;
        int oSize=history.size();
        Area rotatedHist(oSize);
        ind= min(history.begin(),history.end());
        if(ind==history.end()) //this should never happen
        {
            throw "Node::walk()-tantrum";
        }
        for(int i=0;i<oSize;i++)
        {
            //fixed itterator
            rotatedHist[i]=history[(*ind+i)%oSize];
            //int ind &min;newVector[i]=origional[(min+i)%origiona.size()]
        }
        // Add history to areas vector
        areas.push_back(history);
        return;
    }
    //walk each connection
    for (int i = 0; i < 3; i++)
    {
        // If a connection is filled and we have not already been to it, recurse
        // TODO: skip selfloops?
        if (connection[i].exists() && find(history.begin(), history.end(), connection[i]) == history.end())
            walk(areas, history, connection[i]);
    }
}

void Node::setAreasets(Areaset* sets[2])
{
    areasets[0]=sets[0];
    areasets[1]=sets[1];
}
