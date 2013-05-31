#include "headers/gamegui.h"

GameGUI::GameGUI(SDL_Surface* screen, int count)
    :screen(screen), state(Blank)
{
    double theta = 0;

    // Create count nodes
    if (count > 0)
    {
        // Place first node in center
        insertNode(center());

        // Place the rest of the initial nodes along an ellipse at the center
        // of the screen
        for (int i = 1; i < count; i++)
        {
            insertNode(Coord(
                screen->w/3 * cos(theta) + screen->w/2,
                screen->h/3 * sin(theta) + screen->h/2));

            theta += 2*3.14/count;
        }
    }

    // Load background
    //std::string background = "images/background.jpg";
    //bg = new Image(background);

    redraw();
}

void GameGUI::redraw(bool lck)
{
    if (lck)
        lock();

    // Blank screen
    SDL_FillRect(screen, NULL, 0);

    // Background, note can't be locked for this to run
    //SDL_BlitSurface(bg->surface(), NULL, screen, NULL);

    // Draw nodes
    for (int i = 0; i < nodes.size(); i++)
    {
        circle(nodes[i]->getLoci(), nodeRadius, nodeCol);
    }

    // Draw lines
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 1; j < lines[i]->size(); j++)
        {
            line((*lines[i])[j-1], (*lines[i])[j], lineCol);
        }
    }

    // Draw temporary line
    for (int i = 1; i < currentLine.size(); i++)
    {
        line(currentLine[i-1], currentLine[i], lineCol);
    }

    if (lck)
        unlock();
}

// Lock the screen so we can access it
void GameGUI::lock()
{
    SDL_LockSurface(screen);
}

// Unlock and show it (double buffering)
void GameGUI::unlock()
{
    SDL_UnlockSurface(screen);
    SDL_Flip(screen);
}

void GameGUI::cancel()
{
    state = Blank;
    currentLine.clear();
    redraw();
}

void GameGUI::click(Coord location)
{
    Node* selected = selectedNode(location);
    int tempx,tempy;
    bool validfinish;
    //Coord point = location;
    //Coord last = currentLine.back();
    // Clicked on node to end
    if (selected && state == NodeClicked)
    {
        validfinish=false;
        //correct for last line to make it straight
        if (validLine(currentLine.back(),straighten(currentLine.back(), location)))//Does the previous line cross before the line is drawn to connect to the node.
        {
           if (((location.y<=(currentLine.back().y+(currentLine.back().x-location.x)))&&(location.y<=(currentLine.back().y-(currentLine.back().x-location.x))))||((location.y>=(currentLine.back().y+(currentLine.back().x-location.x)))&&(location.y>=(currentLine.back().y-(currentLine.back().x-location.x))))) //Is the line coming vertically into node?
            {
                if(validLine(Coord(selected->getLoci().x,currentLine.back().y),Coord(selected->getLoci().x,selected->getLoci().y))) //If Vertical, does the line intersect another line.
                {
                    //cancel();
                    validfinish=true; //If not, line becomes a valid move.
                    currentLine.back().x= selected->getLoci().x; //Change the x value to the one of the node so that it will correct and make a straight line
                    currentLine.push_back(straighten(currentLine.back(), location)); //Add line to currentLine
                }
            }
        else
        {
             {
                if(validLine(Coord(currentLine.back().x,selected->getLoci().y),Coord(selected->getLoci().x,selected->getLoci().y))) //If Horizontal, does the line intersect another line? ALWAYS TRUE???
                {
                    //cancel();
                    validfinish=true; //If not, line becomes a valid move.
                    currentLine.back().y= selected->getLoci().y; //Change the y value to the one of the node so that it will correct and make a straight line
                    currentLine.push_back(straighten(currentLine.back(), location)); //Add line to currentLine
                }
            }
        }
        //Draw new node if the line can connect.
        if(validfinish==true)
        {
            if (currentLine[(currentLine.size())/2].x==currentLine[(currentLine.size())/2-1].x) //Is the line vertical?
            {
                tempx=(currentLine[(currentLine.size())/2]).x;
                tempy=(currentLine[(currentLine.size())/2].y+currentLine[(currentLine.size())/2-1].y)/2;//put new node halfway between points - vert.
            }
            else
            {
                tempy=(currentLine[(currentLine.size())/2]).y;
                tempx=(currentLine[(currentLine.size())/2].x+currentLine[(currentLine.size())/2-1].x)/2;//put new node halfway between points - hor.
            }
            //doMove()
            insertNode(Coord(tempx,tempy));
            state=Blank;
        }
        }


        //cancel();
        // Steps:
        // 1. Select middle node
        // 2. Split line into two lines, one going from first node to middle node,
        //    one going from middle node to second node.
        // 3. insertLine for each of the two lines
        // 4. insertNode for the middle node
        // 5. a.addConnection and b.addConnection for first and second nodes
        // 6. updateAreas
    }
    // Clicked on node to start
    else
        if (selected)
        {
            currentLine.push_back(selected->getLoci());
            state = NodeClicked;
        }
    // Clicked to place a line

    else if ((state == NodeClicked) &&
        validLine(currentLine.back(),straighten(currentLine.back(), location)))//Remove the second half of the || Statement
    {
        currentLine.push_back(straighten(currentLine.back(), location));
        //state = LineClicked; // Took this out otherwise the node would have to be clicked twice for the move to end.
    }

    redraw();
}

void GameGUI::cursor(Coord location)
{
    if (state == NodeClicked)
    {
        lock();
        redraw(false);
        line(currentLine.back(), straighten(currentLine.back(), location), lineCol);
        unlock();
    }
}

Coord GameGUI::straighten(Coord last, Coord point)
{
    // Determine to snap vertically or horizontally
    if (((point.y<=(last.y+(last.x-point.x)))&&(point.y<=(last.y-(last.x-point.x))))||((point.y>=(last.y+(last.x-point.x)))&&(point.y>=(last.y-(last.x-point.x)))))
    {
        //validLine(coord(last.x, last.y), coord(last.x, point.y))

        //keeps line from backtracking on itself
        if(((point.y < last.y)&&(currentLine[currentLine.size()-2].y < last.y))||((point.y > last.y)&&(currentLine[currentLine.size()-2].y > last.y)))
            return Coord(point.x,last.y);

        return Coord(last.x, point.y);
    }
    else
    {
        //keeps line from backtracking on itself
        if(((point.x < last.x)&&(currentLine[currentLine.size()-2].x < last.x))||((point.x > last.x)&&(currentLine[currentLine.size()-2].x > last.x)))
            return Coord(last.x,point.y);
        return Coord(point.x, last.y);
    }
}

Coord GameGUI::center() const
{
    return Coord(screen->w/2, screen->h/2);
}

void GameGUI::line(Coord a, Coord b, Uint32 color)
{
    for (int i = -lineThick; i <= lineThick; i++)
        for (int j = -lineThick; j <= lineThick; j++)
            lineColor(screen, a.x+i, a.y+j, b.x+i, b.y+j, color);
}

void GameGUI::circle(Coord p, int radius, Uint32 color)
{
    circleColor(screen, p.x, p.y, radius, color);
}

// Select the closest node to the point if within the selectRadius, otherwise
// return NULL
Node* GameGUI::selectedNode(Coord point)
{
    int closestIndex = -1;
    double minDist = numeric_limits<double>::infinity();

    for (int i = 0; i < nodes.size(); i++)
    {
        double currentDist = distance(nodes[i]->getLoci(), point);

        if (currentDist < minDist)
        {
            minDist = currentDist;
            closestIndex = i;
        }
    }

    if (closestIndex != -1 && minDist <= selectRadius)
        return nodes[closestIndex];
    else
        return NULL;
}

double GameGUI::distance(Coord a, Coord b) const
{
    return sqrt(pow(1.0*a.x-b.x,2)+pow(1.0*a.y-b.y,2));
}

bool GameGUI::validLine(Coord start, Coord end) const
{
    int startX = start.x;
    int startY = start.y;
    int endX = end.x;
    int endY = end.y;


    // TODO: fix validLine() algorithm
    // http://www.onemoresoftwareblog.com/2011/11/two-line-segment-intersection-algorithm.html

    //code for checking among the line currently being drawn
    for (int j = 0; j < currentLine.size(); j++)
    {
        //for (int j = 1; j < line[i]->size(); j++)
        {
            //const Line& line = *lines[i];
            const int A2 = currentLine[j-1].x;
            const int B2 = currentLine[j-1].y;
            const int A3 = currentLine[j].x;
            const int B3 = currentLine[j].y;

            if (endX == startX)
            {
                if(A2 != A3)
                    if(A2 > A3)
                    {
                        if((startX > A3)&&(startX < A2))
                            if(startY > endY)
                            {
                                if((B2 < startY)&&(B2 > endY))
                                    return false;
                            }
                            else
                                if((B2 > startY)&&(B2 < endY))
                                    return false;
                    }
                    else
                        if((startX < A3)&&(startX > A2))
                            if(startY > endY)
                            {
                                if((B2 < startY)&&(B2 > endY))
                                    return false;
                            }
                            else
                                if((B2 > startY)&&(B2 < endY))
                                    return false;
            }
            else
            {
                if(B2 != B3)
                    if(B2 > B3)
                    {
                        if((startY < B2)&&(startY > B3))
                            if(startX > endX)
                            {
                                if((A2 < startX)&&(A2 > endX))
                                    return false;
                            }
                            else
                                if((A2 > startX)&&(A2 < endX))
                                    return false;
                    }
                    else
                        if((startY > B2)&&(startY < B3))
                            if(startX > endX)
                            {
                                if((A2 < startX)&&(A2 > endX))
                                    return false;
                            }
                            else
                                if((A2 > startX)&&(A2 < endX))
                                    return false;

            }

                        // If A is on one side and B is on the other side, then it intersects
            /*if (((A2-A0)*(B1-B0) + (B2-B0)*(A1-A0)) * ((A3-A0)*(B1-B0) + (B3-B0)*(A1-A0)) < 0 &&
                ((A0-A2)*(B3-B2) + (B0-B2)*(A3-A2)) * ((A1-A2)*(B3-B2) + (B1-B2)*(A3-A2)) < 0)
                return false;*/
        }
    }

    //code for already made lines, unsure of error
    /*for (int i = 0; i < line.size(); i++)
    {
        for (int j = 1; j < line[i]->size(); j++)
        {
            const Line& line = *lines[i];
            const int A2 = line[j-1].x;
            const int B2 = line[j-1].y;
            const int A3 = line[j].x;
            const int B3 = line[j].y;

            if (endY == point.y)
            {
                if(A2 != A3)
                    if(A2 > A3)
                    {
                        if((startX > A3)&&(startX < A2))
                            if(startY > endY)
                            {
                                if((B2 < startY)&&(B2 > endY))
                                    return false;
                            }
                            else
                                if((B2 > startY)&&(B2 < endY))
                                    return false;
                    }
                    else
                        if((startX < A3)&&(startX > A2))
                            if(startY > endY)
                            {
                                if((B2 < startY)&&(B2 > endY))
                                    return false;
                            }
                            else
                                if((B2 > startY)&&(B2 < endY))
                                    return false;
            }
            else
            {
                if(B2 != B3)
                    if(B2 > B3)
                    {
                        if((startY < B2)&&(startY > B3))
                            if(startX > endX)
                            {
                                if((A2 < startX)&&(A2 > endX))
                                    return false;
                            }
                            else
                                if((A2 > startX)&&(A2 < endX))
                                    return false;
                    }
                    else
                        if((startY > B2)&&(startY < B3))
                            if(startX > endX)
                            {
                                if((A2 < startX)&&(A2 > endX))
                                    return false;
                            }
                            else
                                if((A2 > startX)&&(A2 < endX))
                                    return false;

            }

                        // If A is on one side and B is on the other side, then it intersects
            /*if (((A2-A0)*(B1-B0) + (B2-B0)*(A1-A0)) * ((A3-A0)*(B1-B0) + (B3-B0)*(A1-A0)) < 0 &&
                ((A0-A2)*(B3-B2) + (B0-B2)*(A3-A2)) * ((A1-A2)*(B3-B2) + (B1-B2)*(A3-A2)) < 0)
                return false;*/
        //}
    //}

    // Line collision algorithm: http://stackoverflow.com/a/14177062
    /*for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 1; j < lines[i]->size(); j++)
        {
            const Line& line = *lines[i];
            const int A2 = line[j-1].x;
            const int B2 = line[j-1].y;
            const int A3 = line[j].x;
            const int B3 = line[j].y;

            // If A is on one side and B is on the other side, then it intersects
            if (((A2-A0)*(B1-B0) + (B2-B0)*(A1-A0)) * ((A3-A0)*(B1-B0) + (B3-B0)*(A1-A0)) < 0 &&
                ((A0-A2)*(B3-B2) + (B0-B2)*(A3-A2)) * ((A1-A2)*(B3-B2) + (B1-B2)*(A3-A2)) < 0)
                return false;
        }
    }

    // Same but for current line
    for (int j = 1; j < currentLine.size(); j++)
    {
        const int A2 = currentLine[j-1].x;
        const int B2 = currentLine[j-1].y;
        const int A3 = currentLine[j].x;
        const int B3 = currentLine[j].y;

        int sideOne = ((A2-A0)*(B1-B0) + (B2-B0)*(A1-A0)) * ((A3-A0)*(B1-B0) + (B3-B0)*(A1-A0));
        int sideTwo = ((A0-A2)*(B3-B2) + (B0-B2)*(A3-A2)) * ((A1-A2)*(B3-B2) + (B1-B2)*(A3-A2));

        // If A is on one side and B is on the other side, then it intersects
        if ((sideOne < 0 && sideTwo < 0) || (sideOne > 0 && sideTwo > 0))
            return false;
    }*/

    return true;
}

GameGUI::~GameGUI()
{
    //delete bg;
}
