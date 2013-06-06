/*
*   implementation of ai class goes here
*/
#include "headers/gameai.h"

GameAI::GameAI() : Game()
{
    // add more later maybe
}

bool GameAI::aiTurn()
{

    /*vector<int> mValues;
    //Game& cGame= *this;
    Game* tGame = new Game(static_cast<Game> (*this));
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

    delete tGame;

    //Finds whether we want an even or odd number of areas
    wantedAreas = requiredAreas();

    for(int i = 0; i < possibleMoves.length(); i++)
    {
        //create new game
            doPMove(possibleMoves<i>);
            testMoveAreas = currentAreas();
        //delete game
        if (testMoveAreas%2 == wantedAreas)
        {
            doPMove(possibleMoves<i>)
            return true;
        }
    }
    //If an optimal move is not found, it does the first move in the possible moves vector
    doPMove(possibleMoves<0>);
    */
    return true;
}

GameAI::~GameAI()
{
 //more later
}
/*
bool GameAI::requiredAreas(bool player, int startingNodes)
{
    //This method determines what number of nodes the ai requires to win
    if((player == 0)&&(startingNodes == 1)
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
}

void GameAI::populateMList()
{
    possibleMoves.clear();
    for(int i = 0; i < nodes.size-1; i++)
    {
        for int j = 0; j < nodes.size-1; j++)
        {
            if(connectable(nodes<i>,nodes<j>))
            {
                possibleMoves.push_back();
            }
        }
    }
}






*/


