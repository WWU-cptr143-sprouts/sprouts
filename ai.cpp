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
                possibleMoves.push_back(createLine(nodes[i],nodes[j]));
            }
        }
    }
}

int GameAI::notConnectableNodes() const
{
    int a = -1;
    vector<int> n;
    vector<int> count;

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
    while(a != n.back());

    //cycle through n vector and count how many other nodes in the n vector each node can connect to
    for (int i = 0; i < nodes.size(); i++)
    {
        for (int j = i+1; j < nodes.size(); j++)
        {
            if (connectable(*nodes[i], *nodes[j]))
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
    line.push_back(a->getLoci());
    line.push_back(b->getLoci());

    return line;
}

Coord GameAI::midNode(const Line& line) const
{
    int half = line.size()/2;
    //TODO: Merge with gui function
    return Coord ((line[half-1].x+line[half].x)/2,
                  (line[half-1].y+line[half].y)/2);

}

GameAI::~GameAI()
{

}
