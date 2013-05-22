/*
*Implimentation of game class
* AChecker
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
    areas.clear();
    Connection* nodeConns;
    areasets.clear();
    areasets.push_back(defaultAreaset);
    // Find all Cuirticts/areas
    for(int i=0;i<nNodes;i++)
    {
        nodes[i].walk(areas);
    }
    nAreas=areas.size();
    //create and apply area sets to each node
    for(int i=0;i<nNodes;i++)
    {
        Areaset* nodeAS[2];
        if(nodes[i].dead()) continue;

        nodeConns = nodes[i].getConnAddr();
        /*
        * this if statment is only true if on a border, since conn 1
        * would have to be non null, and dead eliminated dead node
        */
        if(nodeConns[1].exists())
        {
            if(nodes[i].vertical())
            {
                tempLoci = nodes[i].getLoci();

                for(int i=0;i<nAreas;i++)
                {

                    tempLoci.x++;
                    //if its above added to the above area set
                    if(isInArea(areas[i],tempLoci))
                        tempSets[0].push_back(&areas[i]);
                    //Need to make x -1 to origiona
                    tempLoci.x--;
                    tempLoci.x--;

                    //if it is below then added to the below areas set
                    if(isInArea(areas[i],tempLoci))
                        tempSets[1].push_back(&areas[i]);

                    tempLoci.x++; //reset to origional coordinates
                }
            }
            else
            {
                tempLoci = nodes[i].getLoci();

                for(int i=0;i<nAreas;i++)
                {

                    tempLoci.y++;
                    //if its right added to the right area set
                    if(isInArea(areas[i],tempLoci))
                        tempSets[0].push_back(&areas[i]);
                    //Need to make x -1 to origiona
                    tempLoci.y--;
                    tempLoci.y--;

                    //if it is left then added to the left areas set
                    if(isInArea(areas[i],tempLoci))
                        tempSets[1].push_back(&areas[i]);

                    tempLoci.y++; //reset to origional coordinates
                }
            }
        }
        else if(nodeConns[0].exists()) //this must be after the above code
        {
            tempLoci = nodes[i].getLoci();

            for(int i=0;i<nAreas;i++)
            {
                if(isInArea(areas[i],tempLoci))
                    tempSets[0].push_back(&areas[i]);
            }
        }
        /*
        * At this point we are all VEYR sleepy
        * this sorts the area sets, does NOT add if duplicate
        * And then applies them to the node
        */
        sort(tempSets[0].begin(),tempSets[0].end());
        sort(tempSets[1].begin(),tempSets[1].end());
        vector<Areaset>::iterator itA=find(areasets.begin(),areasets.end(), tempSets[0]);
        if(itA==areasets.end())
        {
            areasets.push_back(tempSets[0]);
            nodeAS[0]=&areasets.back();
        }
        else nodeAS[0]=&(*itA);
        vector<Areaset>::iterator itB=find(areasets.begin(),areasets.end(), tempSets[1]);
        if(itB==areasets.end())
        {
            areasets.push_back(tempSets[1]);
            nodeAS[1]=&areasets.back();
        }
        else nodeAS[1]=&(*itB);
        //if(tempSets[0]==defaultAreaset)
        nodes[i].setAreasets(nodeAS);
    }
}

/*
*
*/
bool Game::isInArea(const Area& target, Coord position) const    //Blame luke for any problems here
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
                //if()
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
    return lCount%2; // if it is in the area then lCount,the number of lines between it and the origion, will be odd, and thus return twu
}

bool Game::connectable(const Node& nodea,const Node& nodeb) const
{
    return ((nodea.areasets[0]==nodeb.areasets[0] ||
    nodea.areasets[0]==nodeb.areasets[1] ||
    nodea.areasets[1]==nodeb.areasets[0] ||
    nodea.areasets[1]==nodeb.areasets[1] )
     && !nodea.dead() && !nodeb.dead());
}

Game::~Game()
{

}
