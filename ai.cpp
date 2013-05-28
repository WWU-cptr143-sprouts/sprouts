/*
*   implamentation of ai class goes here
*/
#include "headers/gameai.h"
bool GameAI::aiTurn()
{
    //  This will go in our cpp file
    vector<int> mValues;
    tGame = new Game(game);
    populateMList(); //will populate the possibleMoves vector
    populatemTable();
    //insert the move into the temp game
    tGame->insertNode(new Node(~));
    for(int i=0;i <mTable.size();i++)
    {
        int sum=0
        for(int j=0;j<mTable[i].size();j++)
        {
            sum+=(int)(mTable[i][j]);
        }
        mValues.push_back(sum);
    }
    n=max(mValues.begin(),mValues.end());
    //n=mValue.begin();
    *n

    delete tGame;
    return true;
}
