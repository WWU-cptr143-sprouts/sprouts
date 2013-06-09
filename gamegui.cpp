#include "headers/gamegui.h"

GameGUI::GameGUI(SDL_Surface* screen)
    :GameAI(), screen(screen),
    font(TTF_OpenFont("images/LiberationSerif-Bold.ttf", 14)),
    state(Blank), nodeRadius(5), lineThick(1), selectRadius(10),
    player1(true), error(false)
{
    textCol.r = 255;
    textCol.g = 255;
    textCol.b = 255;
}

void GameGUI::init(int count, int radius1, int radius2, int thick)
{
    nodeRadius = radius1;
    selectRadius = radius2;
    lineThick = thick;
    player1 = true;
    error = false;
    if (nodes.size() == 0)
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
    }
    updateAreas();
    redraw();
}

void GameGUI::redraw(bool lck)
{
    if (lck)
        lock();

    // Blank screen
    SDL_FillRect(screen, NULL, 0);

    // Draw nodes
    for (int i = 0; i < nodes.size(); i++)
        circle(nodes[i]->getLoci(), nodeRadius, nodeCol);

    // Draw lines
    for (int i = 0; i < lines.size(); i++)
        for (int j = 1; j < lines[i]->size(); j++)
            line((*lines[i])[j-1], (*lines[i])[j], lineCol);

    // Draw temporary line
    for (int i = 1; i < currentLine.size(); i++)
        line(currentLine[i-1], currentLine[i], (player1)?player1Col:player2Col);

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

    // This really should not be here since this function is to unlock the
    // screen and not display text. However, it was easy to put it here and it
    // works.
    if (error)
        displayError("Error: need to connect at 180 degrees.");

    SDL_Flip(screen);
}

void GameGUI::cancel()
{
    currentLine.clear();
    state = Blank;
    redraw();
}

State GameGUI::click(Coord location)
{
    Node* selected = selectedNode(location);
    int tempx,tempy;
    bool validFinish; //Used to keep track of whether or not the line can be drawn.
    error = false; //Initialize as false every click

    // Don't do anything if it's dead
    if (selected && selected->dead())
        return state;

    // Used to determine if we can go straight to the second node if we clicked
    // one or if we have to draw another line to get there.
    Coord direct, selectedLoci;
    if (selected)
    {
        direct = straighten(location, selected->getLoci());
        selectedLoci = selected->getLoci(); //Location of node stored as a coord.
    }

    // Clicked on node to end, make sure this is at least the second line or
    // that the line is perfectly straight between nodes
    if (selected && state == NodeClicked && //If node is selected, a line has already been drawn, and if the node doesn't have 3 connections.
        // Either there's more than one line going from one node to another or the two nodes are perfectly in line horizontally or vertically
        (currentLine.size() > 1 || (currentLine.size() == 1 && (direct.x == currentLine.back().x || direct.y == currentLine.back().y))))
    {
        validFinish=false; //Reset validFinish

        //correct for last line to make it straight
        //combineLines(location);
        if (validLine(currentLine.back(),
                      straighten(currentLine.back(), Coord(selected->getLoci().x,selected->getLoci().y))))//Does extending previous line cross any lines.
        { //TODO Add statement here to ensure that connections come at 180 degrees when there is already one connection
           //Is the line coming vertically into node?
            if (vertical(currentLine.back(),
                         selected->getLoci()))
            {
                //If Vertical, does the line intersect another line. Adjusts Lines
                //combineLines(location);
                if(validLine(Coord(selected->getLoci().x,currentLine.back().y),
                             selected->getLoci()))
                {
                    //combineLines(location);
                    if ((selected->openRight() && selected->openLeft()) || (!selected->openRight() && !selected->openLeft())) //Checks if new line is valid, and ensures that line is at 180 if 1 connection exists
                    {
                        validFinish=true; //If not, line becomes a valid move.
                        //combineLines(location);
                        if(vertical(currentLine.back(),location)) //If last line coming in is vertical as well, delete last point.
                            currentLine.pop_back(); //It isn't necessary and it will create diagonal lines.
                        currentLine.back().x= selected->getLoci().x; //Change the x value to the one of the node so that it will correct and make a straight line

                    }
                    else
                        error = true;
                }
            }
            else
            {
                //If Horizontal, does the line intersect another line?
                //combineLines(location);
                if(validLine(Coord(currentLine.back().x,selected->getLoci().y),
                             selected->getLoci()))
                {
                    //combineLines(location);
                    if ((selected->openUp() && selected->openDown()) || (!selected->openUp() && !selected->openDown())) //Checks if new line is valid, and ensures that line is at 180
                    {
                        validFinish=true; //If not, line becomes a valid move.
                        //combineLines(location);

                        if(!vertical(currentLine.back(),location) && currentLine.size() > 1) //If last line coming in is horizontal as well, delete last point.
                            currentLine.pop_back(); //It isn't necessary and it will create diagonal lines.
                        currentLine.back().y = selected->getLoci().y; //Change the y value to the one of the node so that it will correct and make a straight line
                    }
                    else
                        error = true;
                }
            }

            //Calculate location of node to be added
            if(validFinish==true)
            {
                currentLine.push_back(selected->getLoci()); //Push the final node onto the vector.
                if (currentLine[(currentLine.size())/2].x==currentLine[(currentLine.size())/2-1].x) //Finds the middle line in the connections and checks if it is vertical.
                {
                    tempx=(currentLine[(currentLine.size())/2]).x;
                    tempy=(currentLine[(currentLine.size())/2].y+currentLine[(currentLine.size())/2-1].y)/2;//put new node halfway between points - vertically.
                }
                else
                {
                    tempy=(currentLine[(currentLine.size())/2]).y;
                    tempx=(currentLine[(currentLine.size())/2].x+currentLine[(currentLine.size())/2-1].x)/2;//put new node halfway between points - horizontally.
                }

                cout << "Middle: " << Coord(tempx, tempy) << " Line: " << currentLine << endl;
                doMove(currentLine,Coord(tempx,tempy));
                player1 = !player1;

                currentLine.clear();
                state = Blank;

                if (gameEnded())
                    state = GameEnd;
            }
        }
    }

    // Clicked on node to start, make sure this is the first node
    else if (selected && currentLine.size() == 0)
    {
       //Checks to see if there is already 1 connection coming out of the node, if so, the next will be adjusted to only come out at 180.
        currentLine.push_back(selected->getLoci());
        state = NodeClicked;
    }
    // Clicked to place a line
    else if (state == NodeClicked)
    {
        if (currentLine.size()==1) //If first line, ensure 180.
        {
            //Recreate selected
            for (int i = 0; i < nodes.size(); i++) //Finds which node was used to start currentLine.
            {
                if (nodes[i]->getLoci() == currentLine.front())
                {
                    selected = nodes[i];
                }
            }
            if (validLine(currentLine.back(), firststraighten(currentLine.back(), location, selected->openUp(), selected->openDown(),selected->openRight(),selected->openLeft())))
                currentLine.push_back(firststraighten(selected->getLoci(), location,
                    selected->openUp(),    selected->openDown(),
                    selected->openRight(), selected->openLeft()));
        }
        else
        {
            combineLines(location);
        }
    }

    redraw();

    return state;
}

void GameGUI::cursor(Coord location)
{
    Node* selected = NULL;

    if (state == NodeClicked)
    {
        lock();
        redraw(false);

        if (currentLine.size() == 1) //If it is the first line drawn out of node
        {
            // Which node was used to start currentLine
            selected = findNode(currentLine.front());

            line(selected->getLoci(), firststraighten(selected->getLoci(), location,
                    selected->openUp(),    selected->openDown(),
                    selected->openRight(), selected->openLeft()), (player1)?player1Col:player2Col);
        }
        else
            line(currentLine.back(), straighten(currentLine.back(), location), (player1)?player1Col:player2Col);

        unlock();
    }


}

bool GameGUI::vertical(Coord last, Coord point)
{
    if (((point.y<=(last.y+(last.x-point.x)))&&
         (point.y<=(last.y-(last.x-point.x))))||
        ((point.y>=(last.y+(last.x-point.x)))&&
         (point.y>=(last.y-(last.x-point.x)))))
         return true;
    else
        return false;
}

Coord GameGUI::firststraighten(Coord node, Coord cursor, bool up, bool down, bool right, bool left)
{
    if ((up&&!down)||(!up&&down)) //Checks to see if node has 1 line coming up or down out of it
        return Coord(node.x,cursor.y);
    else
        if ((right&&!left)||(!right&&left)) //Check left and right.
             return Coord (cursor.x, node.y);
        else
            return straighten(node, cursor);
}

void GameGUI::combineLines(Coord location)
{
    //Combine last two lines if they go in the same direction. This is necessary to prevent error in the straightening functinon.
    if (validLine(currentLine.back(),straighten(currentLine.back(), location)))
    {
        if (vertical(currentLine.back(),straighten(currentLine.back(), location)) && vertical(currentLine[currentLine.size()-2], currentLine.back())) //If last line and line to add are both vertical
        {
            currentLine.back() = Coord(currentLine.back().x, location.y);// last coord is changed to the extended line.
        }
            //currentLine.push_back(straighten(currentLine.back(), location));
        else if (!vertical(currentLine.back(),straighten(currentLine.back(), location)) && !vertical(currentLine[currentLine.size()-2], currentLine.back())) //If last line and line to add are both horizontal
            currentLine.back() = Coord(location.x, currentLine.back().y);// last coord is changed to the extended line.
        else
            currentLine.push_back(straighten(currentLine.back(), location));
    }
}

Coord GameGUI::straighten(Coord last, Coord point)
{
    // Determine to snap vertically or horizontally
    if (vertical(last, point))
    {
        //validLine(coord(last.x, last.y), coord(last.x, point.y))

        //keeps line from backtracking on itself
        if(currentLine.size() > 2 &&
           (((point.y < last.y)&&(currentLine[currentLine.size()-2].y < last.y))||
            ((point.y > last.y)&&(currentLine[currentLine.size()-2].y > last.y))))
            return Coord(point.x,last.y);

        return Coord(last.x, point.y);
    }
    else
    {
        //keeps line from backtracking on itself
        if(currentLine.size() > 2 &&
           (((point.x < last.x)&&(currentLine[currentLine.size()-2].x < last.x))||
            ((point.x > last.x)&&(currentLine[currentLine.size()-2].x > last.x))))
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
    for (int i = -lineThick; i < lineThick; i++)
        for (int j = -lineThick; j < lineThick; j++)
            lineColor(screen, a.x+i, a.y+j, b.x+i, b.y+j, color);
}

void GameGUI::circle(Coord p, int radius, Uint32 color)
{
    circleColor(screen, p.x, p.y, radius, color);
}

// Select the closest node to the point if within the selectRadius, otherwise
// return NULL
Node* GameGUI::selectedNode(Coord point) const
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

bool GameGUI::validSingleLine(const Line& line, Coord start, Coord end) const
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

bool GameGUI::validLine(Coord start, Coord end) const
{
    if (!validSingleLine(currentLine, start, end))
        return false;

    //code for checking among the line currently being drawn
    //code for already made lines, unsure of error
    for (int i = 0; i < lines.size(); i++)
    {
        const Line& line = *lines[i];

        if (!validSingleLine(line, start, end))
            return false;
    }

    return true;
}

void GameGUI::displayError(const string& msg)
{
    // Top left
    static SDL_Rect location;
    location.x = 0;
    location.y = 0;

    // Only update top left corner.
    location.w = 130;
    location.h = 20;

    SDL_Surface* error = TTF_RenderText_Blended(font, msg.c_str(), textCol);

    SDL_FillRect(screen, &location, 0);
    SDL_BlitSurface(error, NULL, screen , &location);
    //SDL_Flip(screen);
    SDL_FreeSurface(error);
}

void GameGUI::displayPosition(Coord c)
{
    ostringstream s;
    s << c;

    // Bottom left
    static SDL_Rect origin;
    origin.w = 60; // Approximate width and height
    origin.h = 20;
    origin.x = 0;
    origin.y = screen->h - origin.h;

    // Only update top left corner.

    SDL_Surface* hover = TTF_RenderText_Blended(font, s.str().c_str(), textCol);

    SDL_FillRect(screen, &origin, 0);
    SDL_BlitSurface(hover, NULL, screen , &origin);
    //SDL_Flip(screen);
    SDL_FreeSurface(hover);
}

bool GameGUI::playerTurn()
{
    return player1;
}

GameGUI::~GameGUI()
{

}
