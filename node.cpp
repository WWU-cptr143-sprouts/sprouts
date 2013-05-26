/*
* These implications likely are not complete
*/
#include "headers/node.h"

Node::Node(Coord point, Connection con1, Connection con2)
    : loci(point)
{
    fill(&open[0], &open[0]+4, false);
    areasets[0] = NULL;
    areasets[1] = NULL;
    connections[0] = con1;
    connections[1] = con2;
    connections[2] = Connection();
}

Connection* Node::getConnAddr()
{
    return connections;
}

// NOTE: We MUST add keep these in order so that the last one filled is [2]
bool Node::dead() const
{
    return (connections[2].exists());
}

bool Node::vertical() const
{
    return !open[up]; // if the node has a line coming in from the top then it is vertical
}

// Note: we could pass in history by reference if we delete added entries at the end of each
// iteration
void Node::walk(vector<Area*>& areas, Area history, Connection* connection)
{
    // Add this to the history if we're not already on the initial iteration
    if (connection)
        history.push_back(connection);
    #ifdef DEBUG
    cout << "Currently in Node:" << this << " by following:" << connection << endl
        << "connection: " << connection ;
        if(connection) cout << "\tconnection->dest:" << connection->dest;
        cout << "\tthis:" << this
        << "\tconnection && connection->dest == this:" << (int)(connection && (connection->dest == this)) << endl;
    #endif
    // We have a circuit/loop if we're back to the start node
    if (connection && connection->dest == this)
    {
        #ifdef DEBUG
        cout << "entered first if statment" << endl;
        #endif
        //rotate the area to allow for uniqueness comparison
        Area::iterator iter;
        int oSize=history.size();
        Area rotatedHist(oSize);
        iter = min(history.begin(),history.end());

        if (iter==history.end()) //this should never happen
            throw "Node::walk() didn't find minimum";

        for (int i=0;i<oSize;i++)
            rotatedHist[i]=history[(iter-history.begin()+i)%oSize];

        // Add a copy of the rotated history to areas vector
        Area* keep = new Area(rotatedHist);
        areas.push_back(keep);
        return;
    }
    #ifdef DEBUG
    else
    {
        if (connection)
            cout << connection->dest << " vs. " << this << endl;
        else
            cout << "no connection" << endl;
    }
    #endif
    //walk each connection
    for (int i = 0; i < 3; i++)
    {
        // If a connection is filled and we have not already been to it, recurse
        if (connections[i].exists() && find_if(history.begin(), history.end(),
            LineFind(connections[i].line)) == history.end())
        {
            walk(areas, history, &connections[i]);
        }
    }
}

void Node::setAreasets(Areaset* sets[2])
{
    areasets[0]=sets[0];
    areasets[1]=sets[1];
}

bool Node::addConnection(const Connection& con)
{
    for (int i = 0; i < 3; i++)
    {
        if (!connections[i].exists())
        {
            connections[i] = con;
            return true;
        }
    }

    return false;
}

Node::~Node()
{
}
