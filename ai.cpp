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

    delete tGame;*/
    return true;
}

GameAI::~GameAI()
{
 //more later
}
