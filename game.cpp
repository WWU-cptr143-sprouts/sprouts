/*
* Implementation of game class, the AChecker
*/

#include "headers/game.h"

Game::Game()
    :updated(false), moveCount(0)
{

}

// Copy constructor, can be used in AI. Note that this will not update the
// areas. You shouldn't be copying this and then immediately calling
// connectable() a whole bunch of times. Call those immediate connectable calls
// on the object you copied from. After copying this and adding to it do you
// want to update the areas and then check connectable() again.
Game::Game(const Game& g)
    :updated(g.updated), nodes(g.nodes.size()), lines(g.lines.size())
{
    // newLines[oldAddress] = newAddress
    map<Line*, Line*> newLines;
    map<Node*, Node*> newNodes;

    // Copy all lines
    for (int i = 0; i < g.lines.size(); ++i)
    {
        Line* line = new Line(*g.lines[i]);
        lines[i] = line;
        newLines[g.lines[i]] = line;
    }

    // Recreate nodes
    for (int i = 0; i < g.nodes.size(); ++i)
    {
        Node* node = new Node(g.nodes[i]->loci);
        nodes[i] = node;
        newNodes[g.nodes[i]] = node;
    }

    // Recreate node connections
    for (int i = 0; i < g.nodes.size(); ++i)
    {
        Node* node = nodes[i];

        for (int j = 0; j < 3; ++j)
        {
            if (g.nodes[i]->connections[j].exists())
            {
                node->addConnection(Connection(
                    newLines[g.nodes[i]->connections[j].line],
                    newNodes[g.nodes[i]->connections[j].dest]));
            }
        }
    }
}

void Game::updateAreas()
{
    int nNodes = nodes.size(), nAreas=0;
    Coord tempLoci;
    Connection* nodeConns;

    // We have once again updated the area
    updated = true;

    clearAreas();

    // When we unique the areasets, we need a catch-all one
    areasets.push_back(&defaultAreaset);

    // Find all Circuits/areas
    for(int i=0;i<nNodes;i++)
    {
        nodes[i]->walk(areas);
        #ifdef DEBUG
        cout << "Walked node:" << i << endl << "game status:" << endl
            << *this << "\nEnd Game status" << endl;
        #endif
    }

    // This must be this late since Node::walk appends to it
    nAreas=areas.size();

    //create and apply area sets to each node
    for(int i=0;i<nNodes;i++)
    {
        if(nodes[i]->dead())
            continue;

        Areaset* tempSets[2];
        tempSets[0] = new Areaset();
        tempSets[1] = new Areaset();

        nodeConns = nodes[i]->getConnAddr();
        tempLoci = nodes[i]->getLoci();

        /*
        * this if statement is only true if on a border, since conn 1
        * would have to be non null, and dead eliminated dead node
        */
        if(nodeConns[1].exists())
        {
            if(nodes[i]->vertical())
            {
                for(int j=0;j<nAreas;j++)
                {
                    tempLoci.x++;
                    //if its above added to the above area set
                    if(isInArea(*areas[j],tempLoci))
                        tempSets[0]->push_back(areas[j]);
                    // We incremented x once, so decrement twice
                    tempLoci.x--;
                    tempLoci.x--;

                    //if it is below then added to the below areas set
                    if(isInArea(*areas[j],tempLoci))
                        tempSets[1]->push_back(areas[j]);

                    tempLoci.x++; //reset to original coordinates
                }
            }
            else
            {
                for(int j=0;j<nAreas;j++)
                {
                    tempLoci.y++;
                    //if its right added to the right area set
                    if(isInArea(*areas[j],tempLoci))
                        tempSets[0]->push_back(areas[j]);
                    // We incremented x once, so decrement twice
                    tempLoci.y--;
                    tempLoci.y--;

                    //if it is left then added to the left areas set
                    if(isInArea(*areas[j],tempLoci))
                        tempSets[1]->push_back(areas[j]);

                    tempLoci.y++; //reset to original coordinates
                }
            }
        }
        else // A node with either one or no connections
        {
            for(int j=0;j<nAreas;j++)
            {
                if(isInArea(*areas[j],tempLoci))
                {
                    // In "both directions" we're in this area. If we don't do
                    // this for both [0] and [1], we're always in the default
                    // area and can connect to points outside of this area if
                    // one of their areas is the default area.
                    tempSets[0]->push_back(areas[j]);
                    tempSets[1]->push_back(areas[j]);
                }
            }
        }

        /*
        * This sorts the area sets, does NOT add if duplicate
        * And then applies them to the node
        */
        sort(tempSets[0]->begin(),tempSets[0]->end());
        sort(tempSets[1]->begin(),tempSets[1]->end());

        vector<Areaset*>::iterator itA=find_if(areasets.begin(),areasets.end(),
            PointerFind<Areaset>(*tempSets[0]));

        if(itA==areasets.end())
        {
            areasets.push_back(tempSets[0]);
        }
        else
        {
            delete tempSets[0];
            tempSets[0]=*itA;
        }

        vector<Areaset*>::iterator itB=find_if(areasets.begin(),areasets.end(),
            PointerFind<Areaset>(*tempSets[1]));

        if(itB==areasets.end())
        {
            areasets.push_back(tempSets[1]);
        }
        else
        {
            delete tempSets[1];
            tempSets[1]=*itB;
        }

        nodes[i]->setAreasets(tempSets);
    }
}

/*
 * Look at horizontal lines and see if drawing a vertical line from the current
 * point to that line would cross it. If we cross an odd number of lines within
 * an area in the one direction (here, up), we're in the area since we only
 * allow 90 degree angles.
 */
bool Game::isInArea(const Area& target, Coord position) const    //Blame Luke for any problems here
{
    #ifdef DEBUG
    cout << "is in area loci; " << position << endl;
    #endif

    int tSize=target.size(), lCount=0,lSize=0;

    for(int i=0;i<tSize;i++)
    {
        lSize=target[i]->line->size();

        for(int j=1;j<lSize;j++)
        {
            /*
            * This code checks for vertical lines
            * if this was change to y instead of x
            * then it would find horizontal lines
            */
            #ifdef DEBUG
            cout << "target[i]:" << target[i] << endl;
            cout << "(target[i]->line):" << (target[i]->line) << endl;
            #endif
            const Line& lRef= *(target[i]->line);
            if((lRef[j-1].x==lRef[j].x && lRef[j].x < position.x))
            {
                int minY,maxY;
                minY=min(lRef[j].y,lRef[j-1].y);
                maxY=max(lRef[j].y,lRef[j-1].y);
                if(position.y <= maxY && position.y>=minY)
                {
                    lCount++;
                }
            }
        }
    }

    // If it is in the area then lCount, the number of lines between it and the
    // origin, will be odd, and thus return true.
    return lCount%2;
}

bool Game::connectable(const Node& nodea, const Node& nodeb) const
{
    if (!updated)
        throw AreasOutdated();

    // A self loop is possible if we have two free connections
    if (&nodea == &nodeb)
        return !nodea.connections[1].exists() && !nodea.connections[2].exists();

    // If either is dead, we can't connect
    if (nodea.dead() || nodeb.dead())
        return false;

    // Otherwise, see if there's a shared areaset
    return nodea.areasets[0] == nodeb.areasets[0] ||
           nodea.areasets[0] == nodeb.areasets[1] ||
           nodea.areasets[1] == nodeb.areasets[0] ||
           nodea.areasets[1] == nodeb.areasets[1];
}

Node& Game::insertNode(Coord coord, Connection con1, Connection con2)
{
    // We've changed something, must update after this
    updated = false;

    Node* node = new Node(coord, con1, con2);
    nodes.push_back(node);
    return *node;
}

Line& Game::insertLine(const Line& line)
{
        // We've changed something, must update after this
    updated = false;

    Line* keep = new Line(line);
    lines.push_back(keep);
    return *keep;
}

void Game::clearAreas()
{
    for (int i = 0; i < areas.size(); ++i)
        delete areas[i];

    // ALWAYS skip the first one, which is static, the defaultAreaset.
    for (int i = 1; i < areasets.size(); ++i)
        delete areasets[i];

    // Just to make sure we never try accessing those again
    areas.clear();
    areasets.clear();
}

Game::~Game()
{
    clearAreas();

    for (int i = 0; i < nodes.size(); ++i)
        delete nodes[i];

    for (int i = 0; i < lines.size(); ++i)
        delete lines[i];
}

ostream& operator<<(ostream& os, const Game& g)
{
    // Areas
    for (int i = 0; i < g.areas.size(); i++)
        os << "Area " << g.areas[i] << ": " << *g.areas[i] << endl;

    // Areasets
    for (int i = 0; i < g.areasets.size(); i++)
    {
        const Areaset& areaset = *(g.areasets[i]);
        os << "Areaset " << g.areasets[i] << endl;

        for (int j = 0; j < areaset.size(); j++)
            if (areaset[j])
                os << "  Area " << &areaset << ": " << *areaset[j] << endl;
    }

    // Nodes
    for (int i = 0; i < g.nodes.size(); i++)
    {
        os << "Node " << g.nodes[i]  << " @ " << g.nodes[i]->loci << " { U:"
            << g.nodes[i]->open[Up]   << ", R:" << g.nodes[i]->open[Right] << ", D:"
            << g.nodes[i]->open[Down] << ", L:" << g.nodes[i]->open[Left]  << " } "
            << "Areasets { " << g.nodes[i]->areasets[0] << ", "
               << g.nodes[i]->areasets[1] << " } " << endl;

        for (int j = 0; j < 3; j++)
            os << "  Connection[" << j << "] " << g.nodes[i]->connections[j] << endl;
    }

    // Lines
    for (int i = 0; i < g.lines.size(); i++)
        os << "Line " << g.lines[i] << ": " << *g.lines[i] << endl;

    return os;
}

void Game::doMove(const Line& line, Coord middle)
{
    // Determine the end nodes
    Node* a = NULL;
    Node* b = NULL;

    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i]->getLoci() == line.front())
        {
            if (a)
                throw InvalidMove();
            else
                a = nodes[i];
        }

        if (nodes[i]->getLoci() == line.back())
        {
            if (b)
                throw InvalidMove();
            else
                b = nodes[i];
        }
    }

    // Couldn't find nodes
    if (!a || !b)
        throw InvalidMove();

    if (line.size() == 0)
        throw InvalidLine(line);

    // Split the line using the middle coordinate
    int count = 0; // Add to first line when 0, second when 1
    Line AC_line;
    Line CB_line;

    // First point since we start at the second point, looking at the first
    // line segment
    AC_line.push_back(line.front());

    // When between two points, split there
    for (int i = 1; i < line.size(); i++)
    {
        if ((line[i].y == line[i-1].y && // Horizontal
                ((middle.x > line[i].x && middle.x < line[i-1].x)   ||
                 (middle.x < line[i].x && middle.x > line[i-1].x))) ||
            (line[i].x == line[i-1].x && // Vertical
                ((middle.y > line[i].y && middle.y < line[i-1].y)   ||
                 (middle.y < line[i].y && middle.y > line[i-1].y))))
        {
            ++count;
            AC_line.push_back(middle);
            CB_line.push_back(middle);
            CB_line.push_back(line[i]);
        }
        else
        {
            if (count == 0)
                AC_line.push_back(line[i]);
            else
                CB_line.push_back(line[i]);
        }
    }

    // We should have found a place to put the middle point
    if (count != 1)
        throw InvalidMove();

    // Note that if the middle point is on a corner, it will throw above because
    // we check that x or y is less than one and greater than the other, meaning
    // that it can't be equal to either.

    Line& AC = insertLine(AC_line);
    Line& CB = insertLine(CB_line);

    // Insert middle node C between nodes A and B
    Node& c = insertNode(middle,
            Connection(&AC, a),
            Connection(&CB, b));

    try
    {
        // Likewise add the connection to the start and end nodes
        a->addConnection(Connection(&AC, &c));
        b->addConnection(Connection(&CB, &c));
    }
    catch (...)
    {
        // Since it ran into problems, make sure we get rid of it.
        cout << "Before" << endl << *this << endl;
        deleteLastNode();
        cout << "After" << endl << *this << endl;
        throw;
    }

    ++moveCount;
    updateAreas();
}

int Game::moves() const
{
    return moveCount;
}

bool Game::gameEnded() const
{
    // Loop through all nodes and return true if none of them can be connected
    for (int i = 0; i < nodes.size(); i++)
    {
        for (int j = i+1; j < nodes.size(); j++)
        {
            if (connectable(*nodes[i], *nodes[j]))
            {
                return false;
            }
        }
    }

    return true;
}

void Game::deleteLastNode()
{
    Node& node = *nodes.back();

    for (int i = 0; i < 3; i++)
    {
        if (node.connections[i].exists())
        {
            Node& otherNode = *node.connections[i].dest;

            // Delete any reciprocal connections pointing to this node
            for (int j = 0; j < 3; j++)
            {
                if (otherNode.connections[j].dest == &node)
                {
                    otherNode.connections[j] = Connection();
                }
            }

            otherNode.updateOpen();

            // Delete line
            vector<Line*>::iterator iter = find(lines.begin(), lines.end(),
                node.connections[i].line);

            if (iter != lines.end())
                lines.erase(iter);

            if (node.connections[i].line)
                delete node.connections[i].line;
        }
    }

    delete nodes.back();
    nodes.pop_back();
}
