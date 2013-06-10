/*
 *   implementation of AI class goes here
 */
#include "headers/gameai.h"

GameAI::GameAI()
    :Game(), startingNodes(-1)
{

}


bool GameAI::aiTurn()
{
    // Johnathan's original dummy game code
    /*vector<int> mValues;
    //Game& cGame= *this;
    Game* tGame = new Game(static_cast<Game>(*this));
    populateMList(); //will populate the possibleMoves vector
    populatemTable();
    //insert the move into the temp game
    tGame->insertNode(new Node());
    for(int i=0;i <mTable.size();i++)
    {
        int sum=0;
        for(int j=0;j<mTable[i].size();j++)
        {
            sum+=(int)(mTable[i][j]);
        }
        mValues.push_back(sum);
    }
    int n=max_element(mValues.begin(),mValues.end());
    //n=mValue.begin();

    delete tGame;*/

    // Set start nodes and aiFirst on first AI turn
    if(startingNodes == -1)
    {
        if(moves()%2) //Second player, one move has been made
        {
            aiFirst = true;
            startingNodes = nodes.size()  - 1;
        }
        else // First player, no moves have been made yet
        {
            aiFirst = false;
            startingNodes = nodes.size();
        }
    }

    //Finds whether we want an even or odd number of areas
    //Moves%2 returns true if the ai is second player
    wantedAreas = requiredAreas(aiFirst ,startingNodes);
    populateMList();

    for(int i = 0; i < possibleMoves.size(); i++)
    {
        // Create new dummy game. This is GameAI since we have to access data
        // that only something inheriting from it can access.
        GameAI* tGame = new GameAI(*this);

        //insert the move into the temp game
        tGame->doMove(possibleMoves[i], midNode(possibleMoves[i]));
        testUnuseableNodes = tGame->notConnectableNodes();

        delete tGame;

        if (testUnuseableNodes%2 == wantedAreas)
        {
            doMove(possibleMoves[i], midNode(possibleMoves[i]));
            return true;
        }
    }
    //If an optimal move is not found, it does the first move in the possible moves vector
    doMove(possibleMoves[0], midNode(possibleMoves[0]));
    return true;
}

bool GameAI::requiredAreas(bool player, int startingNodes) const
{
    //This method determines what number of nodes the ai requires to win
    if((player == 0)&&(startingNodes == 1))
    {
       return 1;
    }
    else if((player == 0)&&(startingNodes%2 == 0))
    {
         return 0;
    }
    else if((player == 0)&&(startingNodes%2 == 1))
    {
        return 1;
    }
    else if((player == 1)&&(startingNodes%2 == 0))
    {
        return 1;
    }
    else if((player == 1)&&(startingNodes%2 == 1))
    {
        return 0;
    }

    throw "end of requiredAreas without returning anything";
}

void GameAI::populateMList()
{
    possibleMoves.clear();
    for(int i = 0; i < nodes.size()-1; i++)
    {
        for (int j = i+1; j < nodes.size()-1; j++)
        {
            if(connectable(*nodes[i],*nodes[j]))
            {
                if(validLine(nodes[i]->getLoci(),nodes[j]->getLoci(),true) || validLine(nodes[i]->getLoci(),nodes[j]->getLoci(),false))
                {
                    possibleMoves.push_back(createLine(nodes[i],nodes[j]));
                }
            }
        }
    }
}

int GameAI::notConnectableNodes() const
{
    int a = -1;
    vector<int> n;


    // Loop through all nodes and return the number of not connectable nodes
    for (int i = 0; i < nodes.size(); i++)
    {
        for (int j = i+1; j < nodes.size(); j++)
        {
            if (!connectable(*nodes[i], *nodes[j]) && !nodes[i]->dead() && !nodes[j]->dead())
            {
                n.push_back(i);
                break;
            }
        }
    }

    //cycle through n vector and remove all nodes that have identical area sets to a previous node
    do
    {
        a++;
        for(int i = a+1; i < n.size(); i++)
        {
            if(nodes[n[a]]->areasets == nodes[n[i]]->areasets)
                n.erase(n.begin()+i);
        }
    }
    while(n.size() > 0 && a != n.back());

    vector<int> count(n.size());

    //cycle through n vector and count how many other nodes in the n vector each node can connect to
    for (int i = 0; i < n.size(); i++)
    {
        for (int j = i+1; j < n.size(); j++)
        {
            if (connectable(*nodes[n[i]], *nodes[n[j]]))
            {
                count[i]++;
            }
        }
    }

    //deletes nodes with more than 1 connection
    for(int i = n.size()-1; i > 0; i--)
    {
        if(count[i] > 1)
        {
            n.erase(n.begin() + i);
        }
    }

    return n.size();
}

Line GameAI::createLine(Node* a, Node* b) const
{
    Line line;
    Coord temp;
    // Checks to see if a straight line is available
    if(a->getLoci().y != b->getLoci().y || a->getLoci().x != b->getLoci().x)
    {
        line.push_back(a->getLoci());
        line.push_back(b->getLoci());
    }
    else if(validLine(a->getLoci(), b->getLoci(), true))
    {
        temp.x = a->getLoci().x;
        temp.y = b->getLoci().y;
        line.push_back(a->getLoci());
        line.push_back(temp);
        line.push_back(b->getLoci());
    }
    else if(validLine(a->getLoci(), b->getLoci(), false))
    {
        temp.x = b->getLoci().x;
        temp.y = a->getLoci().y;
        line.push_back(a->getLoci());
        line.push_back(temp);
        line.push_back(b->getLoci());
    }

    return line;
}

Coord GameAI::midNode(const Line& line) const
{
    int half = line.size()/2;
    //TODO: Merge with gui function
    return Coord ((line[half-1].x+line[half].x)/2,
                  (line[half-1].y+line[half].y)/2);

}

bool GameAI::validSingleLine(const Line& line, Coord start, Coord end) const
{
    const int startX = start.x;
    const int startY = start.y;
    const int endX = end.x;
    const int endY = end.y;

    for (int j = 1; j < line.size(); j++)
    {
        const int A2 = line[j-1].x;
        const int B2 = line[j-1].y;
        const int A3 = line[j].x;
        const int B3 = line[j].y;

        if (endX == startX)
        {
            if(A2 != A3)
            {
                if(A2 > A3)
                {
                    if((startX > A3)&&(startX < A2))
                    {
                        if(startY > endY)
                        {
                            if((B2 < startY)&&(B2 > endY))
                                return false;
                        }
                        else
                            if((B2 > startY)&&(B2 < endY))
                                return false;
                    }
                }
                else
                    if((startX < A3)&&(startX > A2))
                    {
                        if(startY > endY)
                        {
                            if((B2 < startY)&&(B2 > endY))
                                return false;
                        }
                        else
                            if((B2 > startY)&&(B2 < endY))
                                return false;
                    }
            }
            else
                if(startX == A2)
                {
                    if(B2 > B3)
                    {
                        if(((startY > B3)&&(startY <B2))||((endY > B3)&&(endY < B2)))
                            return false;
                        if(startY < endY)
                        {
                            if((startY < B3)&&(endY > B3))
                                return false;
                        }
                        else
                            if((startY > B2)&&(endY < B2))
                                return false;
                    }
                    else
                    {
                        if(((startY > B2)&&(startY < B3))||((endY > B2)&&(endY < B3)))
                            return false;
                        if(startY < endY)
                        {
                            if((startY < B2)&&(endY > B2))
                                return false;
                        }
                        else
                            if((startY > B3)&&(endY < B3))
                                return false;
                    }
                }

        }
        else
        {
            if(B2 != B3)
            {
                if(B2 > B3)
                {
                    if((startY < B2)&&(startY > B3))
                    {
                        if(startX > endX)
                        {
                            if((A2 < startX)&&(A2 > endX))
                                return false;
                        }
                        else
                            if((A2 > startX)&&(A2 < endX))
                                return false;
                    }
                }
                else
                    if((startY > B2)&&(startY < B3))
                    {
                        if(startX > endX)
                        {
                            if((A2 < startX)&&(A2 > endX))
                                return false;
                        }
                        else
                            if((A2 > startX)&&(A2 < endX))
                                return false;
                    }
            }
            else
                if(startY == B2)
                {
                    if(A2 > A3)
                    {
                        if(((startX > A3)&&(startX < A2))||((endX > A3)&&(endX < A2)))
                            return false;
                        if(startX < endX)
                        {
                            if((startX < A3)&&(endX > A3))
                                return false;
                        }
                        else
                            if((startX > A2)&&(endX < A2))
                                return false;
                    }
                    else
                    {
                        if(((startX > A2)&&(startX < A3))||((endX > A2)&&(endX < A3)))
                            return false;
                        if(startX < endX)
                        {
                            if((startX < A2)&&(endX > A2))
                                return false;
                        }
                        else
                            if((startX > A3)&&(endX < A3))
                                return false;
                    }
                }
        }
    }

    return true;
}

bool GameAI::validLine(Coord start, Coord end) const
{
    for (int i = 0; i < lines.size(); i++)
    {
        const Line& line = *lines[i];

        if (!validSingleLine(line, start, end))
            return false;
    }

    return true;
}

bool GameAI::validLine(Coord a, Coord b, bool up) const
{
    Coord temp;
    if(up)
    {
        temp.x = a.x;
        temp.y = b.y;
        for (int i = 0; i < lines.size(); i++)
        {
            const Line& line = *lines[i];

            if (!validSingleLine(line, a, temp))
                return false;
        }
        for (int i = 0; i < lines.size(); i++)
        {
            const Line& line = *lines[i];

            if (!validSingleLine(line, temp, b))
                return false;
        }
    }
    else
    {
        temp.x = b.x;
        temp.y = a.y;
        for (int i = 0; i < lines.size(); i++)
        {
            const Line& line = *lines[i];

            if (!validSingleLine(line, a, temp))
                return false;
        }
        for (int i = 0; i < lines.size(); i++)
        {
            const Line& line = *lines[i];

            if (!validSingleLine(line, temp, b))
                return false;
        }
    }

    return true;
}

GameAI::~GameAI()
{

}
