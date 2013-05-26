#include "headers/node.h"

Node::Node(Coord point, Connection con1, Connection con2)
    : loci(point)
{
    areasets[0] = NULL;
    areasets[1] = NULL;
    connections[0] = con1;
    connections[1] = con2;
    connections[2] = Connection();

    // Update openings for the initial connections if specified
    fill(&open[0], &open[0]+4, true); // By default every direction is open
    updateOpen();
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
    return !open[Up]; // if the node has a line coming in from the top then it is vertical
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
            updateOpen();
            return true;
        }
    }

    return false;
}

void Node::updateOpen()
{
    // For each of the connections, set open[dir] to false
    for (int i = 0; i < 3; i++)
    {
        if (connections[i].exists())
        {
            const Coord* other;

            // Note that this can be simplified if we always make sure a line
            // ends with the node pointed to by dest, but until then, check
            // based on coordinates
            const Line& line = *(connections[i].line);

            // A line must be at least the beginning and ending node
            if (line.size() < 2)
                throw InvalidLine();

            if (line.front() == loci) // At beginning
                other = &line[1];
            else if (line.back() == loci) // At end
                other = &line[line.size()-2];
            else // In the middle? It should be at the beginning or end!
                throw InvalidLine();

            // It can't be the same point
            if (loci == *other)
                throw InvalidLine();

            // Determine direction
            if (loci.x == other->x) // Vertical
            {
                if (loci.y < other->y) // Down
                    open[Down] = false;
                else // Up
                    open[Up] = false;
            }
            else if (loci.y == other->y) // Horizontal
            {
                if (loci.x < other->x) // Left
                    open[Left] = false;
                else // Up
                    open[Right] = false;
            }
            else // Neither, so invalid
            {
                throw InvalidLine();
            }
        }
    }
}

Node::~Node()
{

}

ostream& operator<<(ostream& os, const Connection& con)
{
    if (con.exists())
        os << "Node " << con.dest << " @ " << con.dest->loci
           << " via " << con.line << ":{ " << *con.line << " }";
    else
        os << "default";

    return os;
}
