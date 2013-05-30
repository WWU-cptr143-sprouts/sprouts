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

    // Clicked on node to end
    if (selected && state == NodeClicked)
    {
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
    else if (selected)
    {
        currentLine.push_back(selected->getLoci());
        state = NodeClicked;
    }
    // Clicked to place a line
    else if ((state == NodeClicked || state == LineClicked) &&
        validLine(currentLine.back(), location))
    {
        currentLine.push_back(straighten(currentLine.back(), location));
        state = LineClicked;
    }

    redraw();
}

void GameGUI::cursor(Coord location)
{
    if (state == NodeClicked || state == LineClicked)
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
        return Coord(last.x, point.y);
    else
        return Coord(point.x, last.y);
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

bool GameGUI::validLine(Coord a, Coord b) const
{
    const int A0 = a.x;
    const int B0 = a.y;
    const int A1 = b.x;
    const int B1 = b.y;

    // TODO: fix validLine() algorithm
    // http://www.onemoresoftwareblog.com/2011/11/two-line-segment-intersection-algorithm.html

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

}
