// Note: how about this? http://www.codeproject.com/KB/graphics/BezierSpline.aspx

#ifndef H_SPROUT
#define H_SPROUT

#include <cmath>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "draw.h"

static const Uint32 defaultColor = 0xFFFFFFFF;
static const int defaultWidth = 1;

class Sprout
{
private:
    /* Constants */
    static const Uint32 defaultColor = 0xFFFFFFFF;		//
    static const Uint32 grayedColor = 0xD4D4D4D4;		// assuming big-endian?
    static const Uint32 highlightColor = 0xFF00FFFF;	//
    static const int sproutRadius = 5;
    static const int selectRadius = 15;
    static const int thickness = 1;		// thickness for drawing lines and points

    /* Structures and other storage stuff */
    struct sprout   { int xPoint,yPoint,degree; };
    struct connection { std::vector<int*> xPoints, yPoints; };

    std::vector<sprout*>  sprouts;	// all the sprouts
    std::vector<connection*> lines;	// all the lines

    int activeSprout;

    /* Variables */
    SDL_Surface *surface;	// screen for drawing
    bool doLockSurface;		// for internal use - make this false (be sure to reset it) if multiple draw commands need to be executed before flipping the surface
    bool highlighted;

    /* Private functions */
    void thickLine(SDL_Surface*,int,int,int,int,Uint32);	// draws a line of thickness n+2 (last parameter n)
    void connect(int,int,int,int);	// (by index) connect first sprout to second sprout through (x,y)
    void drawSprouts();		// draw all the sprouts
    void drawConnection(connection*);		// draw the current temporary line
    bool select(int,int);			// set sprout active it near x,y
    double dist(int,int,int,int);	// return distance between (x0,y0) and (x1,y1)
    int dist2(int,int,int,int);	// return square of distance between (x0,y0) and (x1,y1)
    bool lineValid(int,int,int,int);	// draw line segments from x0,y0 to x,y : returns false if hit another line
public:
    Sprout(SDL_Surface*,int);	// default constructor : feed it a surface and initial sprouts number

    void drawLines();
    bool highlightNear(int,int);	// highlight sprout if near x,y (calls select())
    bool connect();				// loop to connect two sprouts
};

#endif
