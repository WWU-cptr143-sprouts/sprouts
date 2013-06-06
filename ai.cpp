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
    Game* tGame = new Game(*this);
    populateMList(); //will populate the possibleMoves vector
    populatemTable();
    //insert the move into the temp game
    tGame->insertNode(new Node());
    for(int i=0;i <mTable.size();i++)
    {
        int sum=0
        for(int j=0;j<mTable[i].size();j++)
        {
            sum+=(int)(mTable[i][j]);
        }
        mValues.push_back(sum);
    }
    n=max_element(mValues.begin(),mValues.end());
    //n=mValue.begin();

    delete tGame;*/

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
    return true;
}

GameAI::~GameAI()
{
 //more later
}
