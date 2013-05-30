#ifndef H_GameGUI
#define H_GameGUI

#include <cmath>
#include <limits>
#include <vector>
#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include "gameai.h"

using namespace std;

// These start with capitals since we keep on having naming conflicts
enum State
{
    Blank,
    NodeClicked,
};

class GameGUI : public GameAI
{
    private:
        SDL_Surface* screen;

        // Where we are currently in the game. E.g., we just clicked on a node.
        State state;
        Line currentLine;

        // Assuming big-endian?
        static const int nodeRadius = 5; // Node radius
        static const int lineThick  = 1; // Line thickness
        static const int selectRadius = 20; // Defines how close you must click to a node
        static const Uint32 nodeCol = 0xFF4500FF;
        static const Uint32 lineCol = 0xFFFFFFFF;
    public:
        GameGUI(SDL_Surface*,int); // default constructor : feed it a surface for the screen
                                   // and initial sprouts number

        void cancel(); // Escape pressed
        void click(Coord); // Mouse clicked at (x,y)
        void cursor(Coord); // Cursor hovering at (x,y)
        ~GameGUI();

    private:
        // Find coordinate of center of screen
        Coord center() const;

        // SDL interface
        void lock();
        void unlock();

        // Redraw the background, lines, etc. Set lock to false when you draw multiple
        // things to the screen at the same time.
        void redraw(bool lock_screen = true);
        void line(Coord, Coord, Uint32 color);        // Draw a line
        void circle(Coord, int radius, Uint32 color); // Draw a circle
        bool validLine(Coord,Coord) const; // See if a pending line crosses another
        Coord straighten(Coord last, Coord point); // Snap point to 90 degrees of last
        double distance(Coord, Coord) const;
        Node* selectedNode(Coord); // Return node near cursor; if none, NULL
};

#endif
