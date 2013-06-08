#ifndef H_GameGUI
#define H_GameGUI

#include <cmath>
#include <cstdlib> // Has abs(int)
#include <limits>
#include <vector>
#include <string>
#include <sstream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_gfxPrimitives.h>
#include "gameai.h"
#include "image.h"
#include "node.h"

using namespace std;

// These start with capitals since we keep on having naming conflicts
enum State
{
    Blank,
    NodeClicked,
    GameEnd
};

class GameGUI : public GameAI
{
    private:
        SDL_Surface* screen;
        TTF_Font* font;

        // The background image
        //Image* bg;

        // Where we are currently in the game. E.g., we just clicked on a node.
        State state;
        Line currentLine;
        SDL_Color textCol;

        // Assuming big-endian?
        static const int nodeRadius = 5; // Node radius
        static const int lineThick  = 1; // Line thickness
        static const int selectRadius = 10; // Defines how close you must click to a node
        static const Uint32 nodeCol = 0xFF4500FF;
        static const Uint32 lineCol = 0xFFFFFFFF;
        static const Uint32 player1Col = 0x069DD6FF; //Blue
        static const Uint32 player2Col = 0xFE0208FF; //Red

    public:
        GameGUI(SDL_Surface*); // default constructor : feed it a surface for the screen

        bool player1; // keeps track of what players turn it is
        bool error; //180 Connection Error - For text output


        void init(int); // Draw to screen with a certain number of nodes

        void cancel(); // Escape pressed
        State click(Coord); // Mouse clicked at (x,y)
        void cursor(Coord); // Cursor hovering at (x,y)
        bool playerTurn() const; // Returns the player1 variable
        ~GameGUI();

    private:
        // Find coordinate of center of screen
        Coord center() const;

        // SDL interface
        void lock();
        void unlock(); // Also draws error message(s) to screen

        // Redraw the background, lines, etc. Set lock to false when you draw multiple
        // things to the screen at the same time.
        void redraw(bool lock_screen = true);
        void line(Coord, Coord, Uint32 color);        // Draw a line
        void circle(Coord, int radius, Uint32 color); // Draw a circle
        bool validLine(Coord,Coord) const; // See if a pending line crosses another
        bool validSingleLine(const Line&,Coord,Coord) const; // Used in validLine() for each of the lines, removes duplicate code
        //bool validConnection; //Checks to see if connection to the last node in a line is valid. TODO: Implement if needed
        bool vertical(Coord,Coord); //Checks two points and determines if line to be drawn should be vertical
        Coord straighten(Coord last, Coord point); // Snap point at 90 degrees angles to last
        double distance(Coord, Coord) const;
        Node* selectedNode(Coord); // Return node near cursor; if none, NULL
        void displayPosition(Coord); // Print to screen, for debugging
        void displayError(const string&); //Print to screen, for 180 degree rule violation

        // Cleanup the line. Return true if successful.
        bool objectAvoidance(); // Redraw the line in an attempt to make it valid
        bool fixEndpoints();    // Redraw the end portions of the line
};

#endif
