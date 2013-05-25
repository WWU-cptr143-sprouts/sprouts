/*
* Implementation of game class
* AChecker
*
* TODO:
*   - only allow connections going out at 180 degrees until third
*   - update openings[]
*   - when doing a self loop, does connectable() need to check for 2 free connections?
*/

#include "headers/game.h"

Game::Game()
{

}

void Game::updateAreas()
{
    int nNodes = nodes.size(), nAreas=0;
    Areaset tempSets[2];
    Coord tempLoci;
    Connection* nodeConns;

    for (int i = 0; i < areas.size(); ++i)
        delete areas[i];

    // ALWAYS skip the first one, which is static, the defaultAreaset.
    for (int i = 1; i < areasets.size(); ++i)
        delete areasets[i];

    areas.clear();
    areasets.clear();

    // When we unique the areasets, we need a catch-all one
    areasets.push_back(&defaultAreaset);

    // Find all Circuits/areas
    for(int i=0;i<nNodes;i++)
        nodes[i]->walk(areas);

    // This must be this late since Node::walk appends to it
    nAreas=areas.size();

    //create and apply area sets to each node
    for(int i=0;i<nNodes;i++)
    {
        Areaset* nodeAS[2];
        
        if(nodes[i]->dead())
            continue;

        nodeConns = nodes[i]->getConnAddr();

        /*
        * this if statement is only true if on a border, since conn 1
        * would have to be non null, and dead eliminated dead node
        */
        if(nodeConns[1].exists())
        {
            if(nodes[i]->vertical())
            {
                tempLoci = nodes[i]->getLoci();

                for(int i=0;i<nAreas;i++)
                {
                    tempLoci.x++;
                    //if its above added to the above area set
                    if(isInArea(*areas[i],tempLoci))
                        tempSets[0].push_back(areas[i]);
                    //Need to make x -1 to origin
                    tempLoci.x--;
                    tempLoci.x--;

                    //if it is below then added to the below areas set
                    if(isInArea(*areas[i],tempLoci))
                        tempSets[1].push_back(areas[i]);

                    tempLoci.x++; //reset to original coordinates
                }
            }
            else
            {
                tempLoci = nodes[i]->getLoci();

                for(int i=0;i<nAreas;i++)
                {

                    tempLoci.y++;
                    //if its right added to the right area set
                    if(isInArea(*areas[i],tempLoci))
                        tempSets[0].push_back(areas[i]);
                    //Need to make x -1 to origin
                    tempLoci.y--;
                    tempLoci.y--;

                    //if it is left then added to the left areas set
                    if(isInArea(*areas[i],tempLoci))
                        tempSets[1].push_back(areas[i]);

                    tempLoci.y++; //reset to original coordinates
                }
            }
        }
        else if(nodeConns[0].exists()) //this must be after the above code
        {
            tempLoci = nodes[i]->getLoci();

            for(int i=0;i<nAreas;i++)
            {
                if(isInArea(*areas[i],tempLoci))
                    tempSets[0].push_back(areas[i]);
            }
        }

        /*
        * This sorts the area sets, does NOT add if duplicate
        * And then applies them to the node
        */
        sort(tempSets[0].begin(),tempSets[0].end());
        sort(tempSets[1].begin(),tempSets[1].end());
        vector<Areaset*>::iterator itA=find_if(areasets.begin(),areasets.end(),
            PointerFind<Areaset>(tempSets[0]));
        if(itA==areasets.end())
        {
            areasets.push_back(&tempSets[0]);
            nodeAS[0]=areasets.back();
        }
        else nodeAS[0]=*itA;
        vector<Areaset*>::iterator itB=find_if(areasets.begin(),areasets.end(),
            PointerFind<Areaset>(tempSets[1]));
        if(itB==areasets.end())
        {
            areasets.push_back(&tempSets[1]);
            nodeAS[1]=areasets.back();
        }
        else nodeAS[1]=*itB;

        nodes[i]->setAreasets(nodeAS);
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
    int tSize=target.size(), lCount=0,lSize=0;
    for(int i=0;i<tSize;i++)
    {
        lSize=target[i]->line->size();
        for(int j=1;j<lSize;i++)
        {
            /*
            * This code checks for vertical lines
            * if this was change to y instead of x
            * then it would find horizontal lines
            */
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
    return lCount%2; // if it is in the area then lCount,the number of lines between it and the origin, will be odd, and thus return two
}

bool Game::connectable(const Node& nodea, const Node& nodeb) const
{
    return ((nodea.areasets[0]==nodeb.areasets[0] ||
             nodea.areasets[0]==nodeb.areasets[1] ||
             nodea.areasets[1]==nodeb.areasets[0] ||
             nodea.areasets[1]==nodeb.areasets[1] )
            && !nodea.dead() && !nodeb.dead());
}

Node& Game::insertNode(Coord coord, Connection con1, Connection con2)
{
    Node* node = new Node(coord, con1, con2);
    nodes.push_back(node);
    return *node;
}

Line& Game::insertLine(const Line& line)
{
    Line* keep = new Line(line);
    lines.push_back(keep);
    return *keep;
}

Game::~Game()
{
    for (int i = 0; i < areas.size(); ++i)
        delete areas[i];

    // ALWAYS skip the first one, which is static, the defaultAreaset.
    for (int i = 1; i < areasets.size(); ++i)
        delete areasets[i];
    
    for (int i = 0; i < nodes.size(); ++i)
        delete nodes[i];

    for (int i = 0; i < lines.size(); ++i)
        delete lines[i];
}

ostream& operator<<(ostream& os, const Game& g)
{
    // Areas
    for (int i = 0; i < g.areas.size(); i++)
    {
        cout << "Area " << g.areas[i] << ": ";

        for (int j = 0; j < g.areas[i]->size(); ++j)
        {
            const Area& area = *g.areas[i];
            cout << area[j] << " ";
        }

        cout << endl;
    }

    // Areasets
    for (int i = 0; i < g.areasets.size(); i++)
    {
        cout << "Areaset " << g.areasets[i] << endl;

        for (int j = 0; j < g.areasets[i]->size(); ++j)
        {
            const Areaset& areaset = *g.areasets[i];
            cout << "Area " << g.areasets[i] << ": " << areaset[j] << endl;
        }
    }

    // Nodes
    for (int i = 0; i < g.nodes.size(); i++)
    {
        cout << "Node " << g.nodes[i] << endl;

        for (int j = 0; j < 3; j++)
        {
            cout << "  Connection[" << j << "] " << g.nodes[i]->connections[j] << endl;
        }
    }
    
    // Lines
    for (int i = 0; i < g.lines.size(); i++)
    {
        cout << "Line " << g.lines[i] << ": " << *g.lines[i] << endl;
    }
    
    return os;
}
