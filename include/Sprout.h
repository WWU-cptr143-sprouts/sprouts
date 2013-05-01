/**********************************************************************
File Name: Sprout.h
Author: Chris Wills & Ryan Allen                           Date: 6/5/07
File Purpose: Header file for the Sprout Class
Description: Defines the Sprout Class
Problems: Clicking on nodes not fully implemented
**********************************************************************/
#ifndef H_SPROUT
#define H_SPROUT

#include "draw.h"
#include "SDL_ttf.h"

class Game;

//The frames per second
const int FRAMES_PER_SECOND = 10;

//The dimensions for the node
const int NODE_WIDTH = 30;
const int NODE_HEIGHT = 30;

//status of the mouse
const int MOUSE_NORMAL = 0;
const int MOUSE_OVER = 1;
const int MOUSE_CLICK = 2;

// TODO: why definition right after all these constants?
#define SPROUT_RADIUS 12

struct SproutClips
{
    SDL_Rect clipsGreen[14];
    SDL_Rect clipsYellow[14];
    SDL_Rect clipsRed[14];
    SDL_Rect clipBlack[1];
    SDL_Surface *anime;
    SDL_Surface *click;
    TTF_Font * font;
};

class Sprout
{
    int ticks;
    int color;
    // The current frame
    int frame;
    // Animation status
    int status;
    // The clips/image for the sprout
    SproutClips * clips;
    // Identification for this sprout
    int sproutNumber;
    // The Surface that will hold the rendered Number
    SDL_Surface * number;
    // Renders the text for the number of the node
    void renderNumber();
    // Checks to see if the given (x,y) is not over another sprout
    bool available(int x, int y, int from, int to, Sprout ** sprouts);
    // Checks to see if this node will overlap with a node at (x,y)
    bool willOverlap(int x, int y);

    Game *parent;

public:
    // TODO: why is this public?
    SDL_Rect node;

    // Everything is randomized
    Sprout(SproutClips * theClips, Sprout ** sprouts, Game *game);
    // Set the (x,y) to the specified point and the setup as a 2-connection sprout
    Sprout(int num, int x, int y, SproutClips * theClips);
    // Sets the number of nodes and randomizes the rest
    Sprout(int num, int from, int to, SproutClips * theClips, Sprout ** sprouts, Game *game);
    // Copy constructor
    Sprout(Sprout * current);

    // State functions
    int   getState();
    void  setState(int);
    Point getPoint();
    void  setPoint(int x, int y);

    // Handles input
    bool isMouseOver(int x, int y);

    // Display Functions
    void hover(SDL_Surface * destination); // Shows the hover
    void click(SDL_Surface * destination); // Shows the click
    void show(SDL_Surface * destination);  // Shows the node
    void draw(SDL_Surface * destination);  // Calls the correct drawing function based on the stat
    void addConnection();                  // Changes the color based on the number of connections
    void removeConnection();               // Changes the color based on the number of
    int  getConnections();                 // returns the number of connections

    ~Sprout();
};

#endif
