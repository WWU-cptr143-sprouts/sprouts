#ifndef H_BEZIER
#define H_BEZIER

#include "draw.h"
#include "game.h"
#include <SDL/SDL_gfxPrimitives.h>

enum Curve
{
    CURVE_START,
    CURVE_END
};

class Bezier
{
    Point points[4];

public:
    // No beginning coordinates
    Bezier();
    // For setting beginning coordinates
    Bezier(Point coord[4]);
    Bezier(int,int,int,int,int,int,int,int);
    Bezier(Point start);
    Bezier(Bezier *);
    Bezier(Point,Point,Point,Point);
    Bezier(int from, int to, Game * game);

    // Gets the coordinates of the selected point
    Point getcoord(int point);

    // Returns all of the coordinates
    Point * getcoord(void);

    // Sets the curve up with the second end point at (x,y)
    void setCurve(int x, int y);

    // Sets the coordinates of the selected point to the sent coordinates
    bool setcoord(int point, Point coord);

    // Sets the coordinates of the selected point to sent x and y
    bool setcoord(int point, int x, int y);

    // Sets the coordinates of the selected point by array
    bool setcoord(Point coords[4]);

    // Sets the coordinates of the selected point to sent x and y
    bool adjustCoord(int point, int x, int y);

    // Draws the curve
    void draw(SDL_Surface *);

    ~Bezier();
};

#endif
